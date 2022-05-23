/*
 * Copyright (c) 2016 Linaro Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Diver for ARM PrimeCell Synchronous Serial Port (PL022)
 *
 * Based on information from the reference manual:
 * DDI0194G_ssp_pl022_r1p3_trm.pdf
 */

/* copy from zephyr */
/* Values for Control Register 0, SSPCR0 */

#include <stdint.h>
#include "string.h"
#include "err.h"
#include "spi_pl022.h"
#include "spi_drv.h"

#define DRV_PROMPT "SPI_PL022"
#include "drv_dep.h"

#define SSPCR0_DSS(dss) ((dss) << 0) /* Data Size Select (num bits - 1) */
#define SSPCR0_FRF_SPI (0 << 4)      /* Motorola SPI frame format */
#define SSPCR0_FRF_TI (1 << 4)       /* TI synchronous serial frame format */
#define SSPCR0_FRF_MW (2 << 4)       /* National Microwire frame format */
#define SSPCR0_SPO (1 << 6)          /* SPI clock polarity */
#define SSPCR0_SPH (1 << 7)          /* SPI clock phase */
#define SSPCR0_SCR(scr) ((scr) << 8) /* Serial clock rate */

/* Values for Control Register 1, SSPCR1 */

#define SSPCR1_LBM (1 << 0) /* Loop back mode */
#define SSPCR1_SSE (1 << 1) /* Synchronous serial port enable */
#define SSPCR1_MS (1 << 2)  /* Master or slave mode select */
#define SSPCR1_SOD (1 << 3) /* Slave-mode output disable */

/* Values for Status Register, SSPSR */

#define SSPSR_TFE (1 << 0) /* Transmit FIFO empty */
#define SSPSR_TNF (1 << 1) /* Transmit FIFO not full */
#define SSPSR_RNE (1 << 2) /* Receive FIFO not empty */
#define SSPSR_RFF (1 << 3) /* Receive FIFO full */
#define SSPSR_BSY (1 << 4) /* PrimeCell SSP busy flag */

/* Values for interrupt mask/status registers SSPIMSC, SSPRIS and SSPMIS */

#define SSP_INT_ROR (1 << 0) /* Receive overrun interrupt flag */
#define SSP_INT_RT (1 << 1)  /* Receive timeout interrupt flag */
#define SSP_INT_RX (1 << 2)  /* Receive FIFO interrupt flag */
#define SSP_INT_TX (1 << 3)  /* Transmit FIFO interrupt flag */

/* Values for Interrupt Clear Register, SSPICR */

#define SSPICR_RORIC SSP_INT_ROR /* Clear Receive overrun interrupt */
#define SSPICR_RTIC SSP_INT_RT   /* Clear Receive timeout interrupt */

/* Values for DMA Control Register, SSPDMACR */

#define SSPDMACR_RXDMAE (1 << 0) /* Receive DMA Enable */
#define SSPDMACR_TXDMAE (1 << 1) /* Transmit DMA Enable */

#define FIFO_DEPTH 8 /* Number of entries in data FIFO */

static cs_select_func_t _g_cs_select_func = NULL;

struct spi_device;
struct spi_pl022_config;
struct spi_pl022_data;
#define DEV_CFG(spi_device)                                                    \
    (struct spi_pl022_config *)(&(spi_device->config_info))

#define DEV_DATA(spi_device)                                                   \
    (struct spi_pl022_data *)(&(spi_device->driver_data))

#define MAX_SPI_DEVICE_NUM (1)
/*
 * Hardware register layout,
 */
struct spi_pl022 {
    volatile uint32_t cr0;   /* Control register 0, macros SSPCR0_ */
    volatile uint32_t cr1;   /* Control register 1, macros SSPCR1_ */
    volatile uint32_t dr;    /* Data (Transmit/Receive FIFO) */
    volatile uint32_t sr;    /* Status register, macros SSPCR_  */
    volatile uint32_t cpsr;  /* Clock prescale register */
    volatile uint32_t imsc;  /* Interrupt mask set/clr, macros SSP_INT_ */
    volatile uint32_t ris;   /* Raw interrupt status, macros SSP_INT_ */
    volatile uint32_t mis;   /* Masked interrupt status, macros SSP_INT_ */
    volatile uint32_t icr;   /* Interrupt clear register, macros SSPICR_ */
    volatile uint32_t dmacr; /* DMA control register, macros SSPDMACR_ */
};

/* Device config */
struct spi_pl022_config {
    struct spi_pl022 *ssp;
    unsigned int      clk_freq;
    bool              is_irq_enable;
    void (*irq_config_func)();
};

struct spi_pl022_data;
typedef void (*spi_pl022_pio_fn_t)(struct spi_pl022_data *data,
                                   struct spi_pl022 *     ssp);

/* Driver state */
struct spi_pl022_data {

    struct spi_config config;

    struct spi_pl022 *ssp;

    const uint8_t *tx_buf;
    uint32_t       tx_remain;
    uint8_t *      rx_buf;
    uint32_t       rx_remain;

    spi_pl022_pio_fn_t transfer_fn;

    uint8_t data_bits;
    uint8_t polled_mode;
};

struct spi_device {
    struct spi_pl022_config config_info;
    struct spi_pl022_data   driver_data;
};

struct spi_device _g_spi_devices[MAX_SPI_DEVICE_NUM];
/*
 * Configure a host controller for operating against slaves.
 * This is an implementation of a function for spi_driver_api::configure
 */
static inline int pl022_configure(struct spi_pl022_data *        data,
                                  const struct spi_pl022_config *cfg,
                                  const struct spi_config *      config)
{
    struct spi_pl022 *ssp = cfg->ssp;

    uint32_t     cr0, cr1;
    unsigned int div, CPSDVR, SCR1;
    if (0 == drv_memcmp(&data->config, config, sizeof(struct spi_config))) {
        /* Nothing to do */
        // DRV_DBG("Context: 0x%x already configured!\n", config);
        return 0;
    }

    /* check config */

    if (SPI_HOLD_ON_CS & (config->operation)) {
        DRV_DBG("Skip the Unsupported configuration: SPI_HOLD_ON_CS\n");
    }

    if (SPI_LINES_SINGLE != (SPI_LINES_MASK & (config->operation))) {
        DRV_DBG("Skip the Unsupported configuration: MISO lines: 0x%x\n",
                (SPI_LINES_MASK & (config->operation)));
    }

    if (SPI_CS_ACTIVE_HIGH & (config->operation)) {
        DRV_DBG("SPI PL022 Driver doesn't support SPI_CS_ACTIVE_HIGH!\n");
        return -1;
    }

    if (SPI_OP_MODE_MASTER != SPI_OP_MODE_GET(config->operation)) {
        DRV_DBG(
            "Currently SPI PL022 Driver only support SPI_OP_MODE_MASTER!\n");
        return -1;
    }

    if (SPI_TRANSFER_LSB & SPI_OP_MODE_GET(config->operation)) {
        DRV_DBG("SPI PL022 Driver doesn't support LSB first!\n");
        return -1;
    }

    /* read data_bits from config */
    data->data_bits = SPI_WORD_SIZE_GET(config->operation);

    /*
     * The SPI clock is cfg->clk_freq / CPSDVR / (1+SCR)
     * where CPSDVR must be even and in range 2..254 and SCR is 0..255.
     *
     */
    div = (cfg->clk_freq) / (config->frequency);

    for (CPSDVR = 2; CPSDVR < 254; CPSDVR++) {
        for (SCR1 = 0; SCR1 < 255; SCR1++) {
            if ((CPSDVR * (1 + SCR1)) == div) {
                goto __end_calc;
            }
        }
    }

    DRV_DBG("Bad DIV: 0x%x\n", div);
    return -1;

__end_calc:
    DRV_DBG("Clock configuration: div: %d CPSDVR: 0x%x(%d), SCR1: 0x%x(%d)\n",
            div, CPSDVR, CPSDVR, SCR1, SCR1);

    ssp->cpsr = CPSDVR;

    cr0 = SSPCR0_FRF_SPI;    /* Use SPI frame format */
    cr0 |= SSPCR0_SCR(SCR1); /* Serial clock rate */

    if (config->operation & SPI_MODE_CPOL) {
        cr0 |= SSPCR0_SPO; /* Clock polarity */
    }

    if (config->operation & SPI_MODE_CPHA) {
        cr0 |= SSPCR0_SPH; /* Clock phase */
    }

    cr0 |= SSPCR0_DSS(data->data_bits - 1); /* Data bits */

    cr1 = 0;

    if (config->operation & SPI_MODE_LOOP) {
        cr1 |= SSPCR1_LBM; /* Loopback */
    }

    cr1 &= (~SSPCR1_MS); /* enable master mode only */

    cr1 |= SSPCR1_SSE; /* Enable port */

    DRV_DBG("Configure CR0: 0x%x, CR1: 0x%x, CPSDVR: 0x%x\n", cr0, cr1, CPSDVR);

    ssp->cr0 = cr0;
    ssp->cr1 = cr1;

    drv_memcpy(&data->config, config, sizeof(struct spi_config));

    DRV_DBG("Installed config %p (SPI_CLK: %uHz): freq %uHz,"
            " mode %u/%u/%u, slave %u\n",
            config, cfg->clk_freq, config->frequency,
            (SPI_MODE_GET(config->operation) & SPI_MODE_CPOL) ? 1 : 0,
            (SPI_MODE_GET(config->operation) & SPI_MODE_CPHA) ? 1 : 0,
            (SPI_MODE_GET(config->operation) & SPI_MODE_LOOP) ? 1 : 0,
            config->slave);

    drv_udelay(200);
    return 0;
}

#define _WATI_SSP_NOT_BUSY while (ssp->sr & SSPSR_BSY)

#define _WATI_SSP_TX_FIFO_NOT_FULL while (!(ssp->sr & SSPSR_TNF))

#define _WAIT_SSP_RX_FIFO_NOT_EMPTY while (!(ssp->sr & SSPSR_RNE))

#define __FRAME_SIZE_8BIT (1)
#define __FRAME_SIZE_16BIT (2)

static inline int _write_dr_frame(uint8_t *tx_buf, uint32_t frame_size,
                                  uint32_t frame_count, struct spi_pl022 *ssp)
{
    volatile uint32_t data = 0;
    uint32_t          i    = 0;

    if (__FRAME_SIZE_8BIT == (frame_size)) {
        for (i = 0; i < frame_count; i++) {
            /* wait fifo not full */
            _WATI_SSP_TX_FIFO_NOT_FULL;

            data = *((uint8_t *)(tx_buf));
            tx_buf++;
            // DRV_DBG(">>>>>>>>>>>>>write data8: 0x%x\n", data);
            ssp->dr = data;
        }
        return frame_count * 1;
    } else if (__FRAME_SIZE_16BIT == (frame_size)) {
        uint8_t data16_l, data16_h;

        for (i = 0; i < frame_count; i++) {
            /* wait fifo not full */
            _WATI_SSP_TX_FIFO_NOT_FULL;

            data16_l = *((uint8_t *)(tx_buf));
            tx_buf++;
            data16_h = *((uint8_t *)(tx_buf));
            tx_buf++;

            data = (data16_l | (data16_h << 8));
            // DRV_DBG(">>>>>>>>>>>>>write data16: 0x%x\n", data);
            ssp->dr = data;
        }
        return frame_count * 2;
    } else {
        DRV_DBG("Invalid frame size: 0x%x\n", frame_size);
        return -1;
    }
}

static inline int _read_dr_frame(uint8_t *rx_buf, uint32_t frame_size,
                                 uint32_t frame_count, struct spi_pl022 *ssp)
{
    volatile uint32_t data = 0;
    uint32_t          i    = 0;
    if (__FRAME_SIZE_8BIT == (frame_size)) {
        for (i = 0; i < frame_count; i++) {
            /* wait fifo not empty */
            _WAIT_SSP_RX_FIFO_NOT_EMPTY;

            data = ssp->dr;
            // DRV_DBG(">>>>>>>>>>>>>read data8: 0x%x\n", data);
            *((uint8_t *)(rx_buf)) = (data & 0xff);
            rx_buf++;
        }
        return frame_count * 1;
    } else if (__FRAME_SIZE_16BIT == (frame_size)) {
        volatile uint8_t data16_l, data16_h;
        for (i = 0; i < frame_count; i++) {
            /* wait fifo not empty */
            _WAIT_SSP_RX_FIFO_NOT_EMPTY;

            data = ssp->dr;
            // DRV_DBG(">>>>>>>>>>>>>read data16: 0x%x\n", data);

            data16_l = data & 0xff;
            data16_h = (data >> 8) & 0xff;

            *((uint8_t *)(rx_buf)) = data16_l;
            rx_buf++;
            *((uint8_t *)(rx_buf)) = data16_h;
            rx_buf++;
        }
        return frame_count * 2;
    } else {
        DRV_DBG("Invalid frame size: 0x%x\n", frame_size);
        return -1;
    }
}

/*
 * This function will try to send as many as tx_size and rx as many as rx_size
 * If failed to send or recive, return fail
 */
static inline int _transceive(uint8_t *tx_buf, uint32_t tx_size,
                              uint8_t *rx_buf, uint32_t rx_size,
                              uint32_t frame_size, struct spi_pl022 *ssp)
{
    int      ret                     = 0;
    uint32_t tmp_tx_data[FIFO_DEPTH];
    uint32_t tmp_rx_data[FIFO_DEPTH];

    uint32_t burst_count    = 0;
    uint32_t burst_tx_count = 0;
    uint32_t burst_rx_count = 0;

    drv_memset(tmp_tx_data, 0, FIFO_DEPTH);
    drv_memset(tmp_rx_data, 0, FIFO_DEPTH);

    if ((frame_size != __FRAME_SIZE_8BIT) &&
        (frame_size != __FRAME_SIZE_16BIT)) {
        DRV_DBG("Invalid frame size: 0x%x\n", frame_size);
        return -1;
    }
    if ((tx_size && (tx_size & (frame_size - 1))) ||
        (rx_size && (rx_size & (frame_size - 1)))) {
        DRV_DBG("Invalid tx size: 0x%x rx size: 0x%x frame size: 0x%x\n",
                tx_size, rx_size, frame_size);
        return -1;
    }

    /* wait until free */
    _WATI_SSP_NOT_BUSY;

    while (tx_size || rx_size) {
        // DRV_DBG("tx_size: 0x%x, rx_size: 0x%x\n", tx_size, rx_size);

        /* wait until free */
        _WATI_SSP_NOT_BUSY;

        burst_tx_count = 0;
        burst_rx_count = 0;

        if (tx_size) {
            burst_tx_count = tx_size / frame_size;
            if (burst_tx_count > FIFO_DEPTH) {
                burst_tx_count = FIFO_DEPTH;
            }
        }

        if (rx_size) {
            burst_rx_count = rx_size / frame_size;
            if (burst_rx_count > FIFO_DEPTH) {
                burst_rx_count = FIFO_DEPTH;
            }
        }

        if (burst_rx_count && burst_tx_count) {
            burst_count =
                ((burst_rx_count > burst_tx_count) ? (burst_tx_count)
                                                   : (burst_rx_count));
        } else if (burst_rx_count && (!burst_tx_count)) {
            burst_count = burst_rx_count;
        } else if ((!burst_rx_count) && burst_tx_count) {
            burst_count = burst_tx_count;
        } else {
            DRV_DBG(
                "Invalid burst count: burst_tx_count: 0x%x, burst_rx_count: "
                "0x%x\n",
                burst_tx_count, burst_rx_count);
            return -1;
        }

        /* send */
        if (tx_size && tx_buf) {
            ret = _write_dr_frame(tx_buf, frame_size, burst_count, ssp);
            if (ret < 0) {
                DRV_DBG("Write Frame Failed!\n");
                return ret;
            }
            tx_buf += ret;
            tx_size -= ret;
        } else {
            ret = _write_dr_frame((uint8_t *)(tmp_tx_data), frame_size,
                                  burst_count, ssp);
            if (ret < 0) {
                DRV_DBG("Write Frame Failed!\n");
                return ret;
            }
        }

        /* receive */
        if (rx_size && rx_buf) {
            ret = _read_dr_frame(rx_buf, frame_size, burst_count, ssp);
            if (ret < 0) {
                DRV_DBG("Write Frame Failed!\n");
                return ret;
            }
            rx_buf += ret;
            rx_size -= ret;
        } else {
            ret = _read_dr_frame((uint8_t *)(tmp_rx_data), frame_size,
                                 burst_count, ssp);
            if (ret < 0) {
                DRV_DBG("Write Frame Failed!\n");
                return ret;
            }
        }
    }

    return 0;
}

#if 1
static volatile uint32_t _g_counter0      = 0;
static volatile uint32_t _g_counter1      = 0;
static volatile uint32_t _g_counter_delta = 0;
volatile uint64_t        _g_total_counter = 0;
#endif

int spi_pl022_transceive(uint32_t port, const struct spi_config *config,
                         const struct spi_buf_set *tx_bufs,
                         const struct spi_buf_set *rx_bufs)
{
    uint8_t *             rx_buf, *tx_buf;
    uint32_t              rx_len, tx_len;
    uint32_t              rx_buf_idx, tx_buf_idx;
    const struct spi_buf *current_tx = NULL;
    const struct spi_buf *current_rx = NULL;

    struct spi_device *            spi_device = NULL;
    struct spi_pl022_data *        data       = NULL;
    const struct spi_pl022_config *cfg        = NULL;
    int                            ret        = 0;

    DRV_CHECK_CONDITION(port < MAX_SPI_DEVICE_NUM, -1, "bad port: %d\n", port);
    DRV_CHECK_CONDITION(tx_bufs || rx_bufs, -1, "tx and rx are NULL!\n");
    DRV_CHECK_CONDITION(NULL != config, -1, "config is NULL!\n");

    spi_device = &_g_spi_devices[port];
    data       = DEV_DATA(spi_device);
    cfg        = DEV_CFG(spi_device);

    ret = pl022_configure(data, cfg, config);
    DRV_CHECK_RET("SPI Configure failed: 0x%x!\n", ret);

#if 0
    _g_counter0 = fpga_scb_io_counter_read();
#endif

    _g_cs_select_func(port, true);
    drv_udelay(1);

    tx_buf_idx = 0;
    rx_buf_idx = 0;
    while (1) {
        if (tx_bufs && (tx_buf_idx != tx_bufs->count)) {
            current_tx = &(tx_bufs->buffers[tx_buf_idx]);
            tx_buf     = (uint8_t *)current_tx->buf;
            tx_len     = current_tx->len;
        } else {
            current_tx = NULL;
            tx_buf     = NULL;
            tx_len     = 0;
        }
        if (rx_bufs && (rx_buf_idx != rx_bufs->count)) {
            current_rx = &(rx_bufs->buffers[rx_buf_idx]);
            rx_buf     = (uint8_t *)current_rx->buf;
            rx_len     = current_rx->len;
        } else {
            current_rx = NULL;
            rx_buf     = NULL;
            rx_len     = 0;
        }

#if 0
        DRV_DBG("_transceive: tx_buf: 0x%x, tx_len: 0x%x, rx_buf: 0x%x, "
                    "rx_len: 0x%x\n",
                    tx_buf,
                    tx_len,
                    rx_buf,
                    rx_len);
#endif
        if ((tx_len && rx_len) && (tx_len != rx_len)) {
            DRV_DBG("Invalid tx_len: 0x%x, rx_len: 0x%x\n", tx_len, rx_len);
            ret = -1;
            break;
        }

        ret = _transceive(tx_buf, tx_len, rx_buf, rx_len, __FRAME_SIZE_8BIT,
                          data->ssp);
        if (ret) {
            DRV_DBG("_transceive failed: 0x%x!\n", ret);
            return ret;
        }

        if (tx_bufs && (tx_buf_idx != tx_bufs->count)) {
            tx_buf_idx++;
        }
        if (rx_bufs && (rx_buf_idx != rx_bufs->count)) {
            rx_buf_idx++;
        }

        if (((!(tx_bufs)) || (tx_bufs && (tx_buf_idx == tx_bufs->count))) &&
            ((!(rx_bufs)) || (rx_bufs && (rx_buf_idx == rx_bufs->count)))) {
            ret = 0;
            break;
        }
    }

    drv_udelay(1);
    _g_cs_select_func(port, false);
#if 0
    _g_counter1 = fpga_scb_io_counter_read();
    if (_g_counter1 > _g_counter0) {
        _g_counter_delta = (_g_counter1 - _g_counter0);
    } else {
        _g_counter_delta = (0xffffffff - _g_counter0 + _g_counter1);
    }
    //DRV_DBG("counter delta: %d, us: %d\n", _g_counter_delta, _g_counter_delta/20);
    _g_total_counter += _g_counter_delta;
#endif

    ret = 0;
finish:
    return ret;
}

int spi_pl022_init(uint32_t port, addr_t base_addr, uint32_t clk_freq,
                   cs_select_func_t cs_select)
{
    int                      ret        = 0;
    struct spi_pl022_data *  data       = NULL;
    struct spi_pl022_config *cfg        = NULL;
    struct spi_pl022 *       ssp        = NULL;
    struct spi_device *      spi_device = NULL;

    DRV_CHECK_CONDITION(port < MAX_SPI_DEVICE_NUM, ERR_INVALID_ARGS,
                        "bad port: %d\n", port);
    DRV_ASSERT_MSG(base_addr && clk_freq && cs_select, "Parameter is NULL\n");

    _g_cs_select_func = cs_select;

    spi_device = &_g_spi_devices[port];
    data       = DEV_DATA(spi_device);
    cfg        = DEV_CFG(spi_device);

    if (0 == port) {
        cfg->ssp             = (struct spi_pl022 *)base_addr;
        cfg->clk_freq        = clk_freq;
        cfg->is_irq_enable   = false;
        cfg->irq_config_func = NULL;
    }

    data->ssp = cfg->ssp;
    ssp       = data->ssp;

    /* Init ssp */
    ssp->cr1 = 0;
    /* Disable FIFO DMA */
    ssp->dmacr = 0;

    if ((cfg->is_irq_enable) && (cfg->irq_config_func)) {
        cfg->irq_config_func();
        data->polled_mode = false;
        // enable the Tx FIFO empty interrupt
        ssp->imsc = SSP_INT_TX;
    } else {
        data->polled_mode = true;

        // Mask all FIFO/IRQ interrupts
        ssp->imsc = 0x0;
    }
    // Clear existing IRQ's
    ssp->icr = SSPICR_RORIC | SSPICR_RTIC;

    ret = 0;
finish:
    return ret;
}
