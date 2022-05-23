/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/* copy from zephyr */
#include <stdint.h>
#include <unistd.h>  /* for off_t */
#include "string.h"
#if CONFIG_NO_DEV_DRV_FRAMWORK
#include "spi/spi_pl022/spi_drv.h"
#include "spi/spi_pl022/spi_pl022.h"
#else
#include "spi/spi_dev.h"
#endif
#include "spi_flash_w25qxxdv.h"
#include "spi_flash_w25qxxdv_defs.h"

#include "drv_dep.h"

static spi_flash_w25qxxdv_config_t _g_spi_flash_w25qxxdv_config;

struct spi_flash_data {
    struct spi_config spi_cfg;
    // struct k_sem sem;
};

static struct spi_flash_data _g_spi_flash_data;

static int spi_flash_wb_access(struct spi_flash_data *ctx,
                               uint8_t cmd,
                               bool addressed,
                               off_t offset,
                               void *data,
                               size_t length,
                               bool write)
{
    uint8_t access[4];
    struct spi_buf buf[2] = {{.buf = access}, {.buf = data, .len = length}};
    struct spi_buf_set tx = {
        .buffers = buf,
    };

    access[0] = cmd;

    if (addressed) {
        access[1] = (uint8_t)(offset >> 16);
        access[2] = (uint8_t)(offset >> 8);
        access[3] = (uint8_t)offset;

        buf[0].len = 4;
    } else {
        buf[0].len = 1;
    }

    tx.count = length ? 2 : 1;

    if (!write) {
        struct spi_buf_set rx = {.buffers = buf, .count = 2};

#if CONFIG_NO_DEV_DRV_FRAMWORK
        return spi_pl022_transceive(SPI_FLASH_PORT, &ctx->spi_cfg, &tx, &rx);
#else
        return spi_transceive(spi_0_device, &ctx->spi_cfg, &tx, &rx);
#endif
    }

#if CONFIG_NO_DEV_DRV_FRAMWORK
    return spi_pl022_transceive(SPI_FLASH_PORT, &ctx->spi_cfg, &tx, NULL);
#else
    return spi_write(spi_0_device, &ctx->spi_cfg, &tx);
#endif
}

static inline int spi_flash_wb_id(void)
{
    struct spi_flash_data *driver_data = &_g_spi_flash_data;
    uint32_t temp_data;
    uint8_t buf[3];

    if (spi_flash_wb_access(
            driver_data, W25QXXDV_CMD_RDID, false, 0, buf, 3, false)
        != 0) {
        return -1;
    }

    temp_data = ((uint32_t)buf[0]) << 16;
    temp_data |= ((uint32_t)buf[1]) << 8;
    temp_data |= (uint32_t)buf[2];
    if (temp_data != _g_spi_flash_w25qxxdv_config.w25qxxdv_id_value) {
        return -1;
    }

    return 0;
}

static uint8_t spi_flash_wb_reg_read(uint8_t reg)
{
    struct spi_flash_data *driver_data = &_g_spi_flash_data;

    if (spi_flash_wb_access(driver_data, reg, false, 0, &reg, 1, false)) {
        return 0;
    }

    return reg;
}

static inline void wait_for_flash_idle(void)
{
    uint8_t reg;

    do {
        reg = spi_flash_wb_reg_read(W25QXXDV_CMD_RDSR);
    } while (reg & W25QXXDV_WIP_BIT);
}

static int spi_flash_wb_reg_write(uint8_t reg)
{
    struct spi_flash_data *driver_data = &_g_spi_flash_data;

    if (spi_flash_wb_access(driver_data, reg, false, 0, NULL, 0, true) != 0) {
        return -1;
    }

    return 0;
}

int spi_flash_w25qxxdv_read(off_t offset, void *data, size_t len)
{
    struct spi_flash_data *driver_data = &_g_spi_flash_data;
    int ret;

    if (len > SPI_FLASH_W25QXXDV_MAX_DATA_LEN || offset < 0) {
        return -1;
    }

    // k_sem_take(&driver_data->sem, K_FOREVER);

    wait_for_flash_idle();

    ret = spi_flash_wb_access(
        driver_data, W25QXXDV_CMD_READ, true, offset, data, len, false);

    // k_sem_give(&driver_data->sem);

    return ret;
}

int spi_flash_w25qxxdv_write(off_t offset, const void *data, size_t len)
{
    struct spi_flash_data *driver_data = &_g_spi_flash_data;
    uint8_t reg;
    int ret;

    if (len > SPI_FLASH_W25QXXDV_MAX_DATA_LEN || offset < 0) {
        return -1;
    }

    // k_sem_take(&driver_data->sem, K_FOREVER);

    wait_for_flash_idle();

    reg = spi_flash_wb_reg_read(W25QXXDV_CMD_RDSR);
    if (!(reg & W25QXXDV_WEL_BIT)) {
        // k_sem_give(&driver_data->sem);
        return -1;
    }

    wait_for_flash_idle();

    /* Assume write protection has been disabled. Note that w25qxxdv
     * flash automatically turns on write protection at the completion
     * of each write or erase transaction.
     */
    ret = spi_flash_wb_access(
        driver_data, W25QXXDV_CMD_PP, true, offset, (void *)data, len, true);

    // k_sem_give(&driver_data->sem);
    return ret;
}

int spi_flash_w25qxxdv_write_protection_set(bool enable)
{
    // struct spi_flash_data *driver_data = &_g_spi_flash_data;
    uint8_t reg = 0;
    int ret;

    // k_sem_take(&driver_data->sem, K_FOREVER);

    wait_for_flash_idle();

    if (enable) {
        reg = W25QXXDV_CMD_WRDI;
    } else {
        reg = W25QXXDV_CMD_WREN;
    }

    ret = spi_flash_wb_reg_write(reg);

    // k_sem_give(&driver_data->sem);

    return ret;
}

static inline int spi_flash_w25qxxdv_erase_internal(off_t offset, size_t size)
{
    struct spi_flash_data *driver_data = &_g_spi_flash_data;
    bool need_offset                   = true;
    uint8_t erase_opcode;

    if (offset < 0) {
        return -1;
    }

    wait_for_flash_idle();

    /* write enable */
    spi_flash_wb_reg_write(W25QXXDV_CMD_WREN);

    wait_for_flash_idle();

    if (size == _g_spi_flash_w25qxxdv_config.max_size) {
        erase_opcode = W25QXXDV_CMD_CE;
        need_offset  = false;
    } else {
        switch (size) {
        case W25QXXDV_SECTOR_SIZE:
            erase_opcode = W25QXXDV_CMD_SE;
            break;
        case W25QXXDV_BLOCK32K_SIZE:
            erase_opcode = W25QXXDV_CMD_BE32K;
            break;
        case W25QXXDV_BLOCK_SIZE:
            erase_opcode = W25QXXDV_CMD_BE;
            break;
        default:
            return -1;
        }
    }
    /* Assume write protection has been disabled. Note that w25qxxdv
     * flash automatically turns on write protection at the completion
     * of each write or erase transaction.
     */
    return spi_flash_wb_access(
        driver_data, erase_opcode, need_offset, offset, NULL, 0, true);
}

int spi_flash_w25qxxdv_erase(off_t offset, size_t size)
{
    // struct spi_flash_data *driver_data = &_g_spi_flash_data;
    int ret                 = 0;
    uint32_t new_offset     = offset;
    uint32_t size_remaining = size;
    uint8_t reg;

    if ((offset < 0) || ((offset & W25QXXDV_SECTOR_MASK) != 0)
        || ((size + offset) > _g_spi_flash_w25qxxdv_config.max_size)
        || ((size & W25QXXDV_SECTOR_MASK) != 0)) {
        return -1;
    }

    // k_sem_take(&driver_data->sem, K_FOREVER);

    reg = spi_flash_wb_reg_read(W25QXXDV_CMD_RDSR);

    if (!(reg & W25QXXDV_WEL_BIT)) {
        // k_sem_give(&driver_data->sem);
        return -1;
    }

    while ((size_remaining >= W25QXXDV_SECTOR_SIZE) && (ret == 0)) {
        if (size_remaining == _g_spi_flash_w25qxxdv_config.max_size) {
            ret = spi_flash_w25qxxdv_erase_internal(offset, size);
            break;
        }

        if (size_remaining >= W25QXXDV_BLOCK_SIZE) {
            ret = spi_flash_w25qxxdv_erase_internal(new_offset,
                                                    W25QXXDV_BLOCK_SIZE);
            new_offset += W25QXXDV_BLOCK_SIZE;
            size_remaining -= W25QXXDV_BLOCK_SIZE;
            continue;
        }

        if (size_remaining >= W25QXXDV_BLOCK32K_SIZE) {
            ret = spi_flash_w25qxxdv_erase_internal(new_offset,
                                                    W25QXXDV_BLOCK32K_SIZE);
            new_offset += W25QXXDV_BLOCK32K_SIZE;
            size_remaining -= W25QXXDV_BLOCK32K_SIZE;
            continue;
        }

        if (size_remaining >= W25QXXDV_SECTOR_SIZE) {
            ret = spi_flash_w25qxxdv_erase_internal(new_offset,
                                                    W25QXXDV_SECTOR_SIZE);
            new_offset += W25QXXDV_SECTOR_SIZE;
            size_remaining -= W25QXXDV_SECTOR_SIZE;
            continue;
        }
    }

    // k_sem_give(&driver_data->sem);

    return ret;
}

static int spi_flash_wb_configure(void)
{
    struct spi_flash_data *data = &_g_spi_flash_data;

    data->spi_cfg.frequency = _g_spi_flash_w25qxxdv_config.spi_freq;
    data->spi_cfg.operation = SPI_WORD_SET(8);
    data->spi_cfg.slave     = _g_spi_flash_w25qxxdv_config.spi_slave_num;

    return spi_flash_wb_id();
}

int spi_flash_w25qxxdv_init(spi_flash_w25qxxdv_config_t *config)
{
    int ret;

    drv_memcpy(
        &_g_spi_flash_w25qxxdv_config, config, sizeof(spi_flash_w25qxxdv_config_t));

    ret = spi_flash_wb_configure();
    return ret;
}
