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
	return 0;
}

static inline int _read_dr_frame(uint8_t *rx_buf, uint32_t frame_size,
                                 uint32_t frame_count, struct spi_pl022 *ssp)
{
	return 0;
}

/*
 * This function will try to send as many as tx_size and rx as many as rx_size
 * If failed to send or recive, return fail
 */
static inline int _transceive(uint8_t *tx_buf, uint32_t tx_size,
                              uint8_t *rx_buf, uint32_t rx_size,
                              uint32_t frame_size, struct spi_pl022 *ssp)
{
    return 0;
}

static volatile uint32_t _g_counter0      = 0;
static volatile uint32_t _g_counter1      = 0;
static volatile uint32_t _g_counter_delta = 0;
volatile uint64_t        _g_total_counter = 0;

int spi_pl022_transceive(uint32_t port, const struct spi_config *config,
                         const struct spi_buf_set *tx_bufs,
                         const struct spi_buf_set *rx_bufs)
{
	return 0;
}

int spi_pl022_init(uint32_t port, addr_t base_addr, uint32_t clk_freq,
                   cs_select_func_t cs_select)
{
	return 0;
}

