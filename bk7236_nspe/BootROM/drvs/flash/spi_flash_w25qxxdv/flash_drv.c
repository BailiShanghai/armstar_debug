/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#include <stdint.h>
#include <stddef.h>
#include "err.h"
#include "heap.h"
#include "string.h"
#include "flash_drv.h"
#include "spi_flash_w25qxxdv.h"
#include "spi_flash_w25qxxdv_defs.h"

#define DRV_PROMPT "FLASH DRV"
#include "drv_dep.h"

static size_t _g_flash_max_size = 0;

#define _FLASH_MAX_SIZE (_g_flash_max_size)

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef ROUND_DOWN
#define ROUND_DOWN(x, align) ((unsigned long)(x) & ~((unsigned long)align - 1))
#endif

#define FLASH_MAX_SIZE (_g_flash_max_size)

#define __MASK(x, align) ((x) & ((align)-1))
#define __IS_ALIGN(x, align) (0 == __MASK(x, align))

int32_t flash_drv_init(size_t max_flash_size, uint32_t spi_freq,
                       uint32_t spi_slave_num, uint32_t spi_dev_id_value)
{
    int32_t                     ret = NO_ERROR;
    spi_flash_w25qxxdv_config_t cfg = {0};

    cfg.max_size          = max_flash_size;
    cfg.spi_freq          = spi_freq;
    cfg.spi_slave_num     = spi_slave_num;
    cfg.w25qxxdv_id_value = spi_dev_id_value;

    _g_flash_max_size = max_flash_size;
    DRV_DBG("spi flash drv init!\n");
    ret = spi_flash_w25qxxdv_init(&cfg);
    DRV_CHECK_CONDITION(0 == ret, ERR_GENERIC_HW_FAULT,
                        "spi_flash_w25qxxdv_init failed!\n");

finish:
    return ret;
}

static int32_t _flash_drv_read_write(uint32_t offset, uint8_t *data,
                                     uint32_t len, bool is_write)
{
    int ret = 0;

#if 0
        DRV_DBG("flash_drv_read_write %s offset: 0x%x, len: 0x%x\n",
            (is_write ? ("[write]") : ("[read]")),
            offset,
            len);
#endif

    DRV_CHECK_CONDITION(len <= SPI_FLASH_W25QXXDV_MAX_DATA_LEN,
                        ERR_INVALID_ARGS, "Invalid data length: 0x%x > 0x%x\n",
                        len, SPI_FLASH_W25QXXDV_MAX_DATA_LEN);

    if (is_write) {
        ret = spi_flash_w25qxxdv_write(offset, data, len);
    } else {
        ret = spi_flash_w25qxxdv_read(offset, data, len);
    }
    DRV_CHECK_RET("spi_flash_w25qxxdv_read_write %s failed: 0x%x \n",
                  (is_write ? ("write") : ("read")), ret);
finish:
    return ret;
}

int32_t flash_drv_read(uint32_t offset, uint8_t *data, size_t len)
{
    int32_t ret = 0;
#if 0
    DRV_DBG("Flash Read offset: %d size: %d\n", offset, len);
#endif
    DRV_CHECK_CONDITION(offset < _FLASH_MAX_SIZE, ERR_INVALID_ARGS,
                        "Invalid offset: 0x%x\n", offset);
    DRV_CHECK_CONDITION(len < _FLASH_MAX_SIZE, ERR_INVALID_ARGS,
                        "Invalid len: 0x%x\n", len);
    DRV_CHECK_CONDITION((offset + len) < _FLASH_MAX_SIZE, ERR_INVALID_ARGS,
                        "Invalid offset + len: 0x%x + 0x%x\n", offset, len);

    ret = _flash_drv_read_write(offset, data, len, false);
    DRV_CHECK_RET("_flash_drv_read_write [read] failed: 0x%x \n", ret);

finish:
    return ret;
}

int32_t flash_drv_write(uint32_t offset, const uint8_t *data, size_t len)
{
    int32_t ret = 0;
#if 0
    DRV_DBG("Flash Write offset: %d size: %d\n", offset, len);
#endif
    DRV_CHECK_CONDITION(offset < _FLASH_MAX_SIZE, ERR_INVALID_ARGS,
                        "Invalid offset: 0x%x\n", offset);
    DRV_CHECK_CONDITION(len < _FLASH_MAX_SIZE, ERR_INVALID_ARGS,
                        "Invalid len: 0x%x\n", len);
    DRV_CHECK_CONDITION((offset + len) < _FLASH_MAX_SIZE, ERR_INVALID_ARGS,
                        "Invalid offset + len: 0x%x + 0x%x\n", offset, len);

    ret = _flash_drv_read_write(offset, (uint8_t *)data, len, true);
    DRV_CHECK_RET("_flash_drv_read_write [write] failed: 0x%x \n", ret);

finish:
    return ret;
}

int32_t flash_drv_erase(uint32_t offset, size_t len)
{
    int32_t ret      = 0;
    size_t  erase_sz = 0;

#if 0
    DRV_DBG("Flash Erase offset: %d size: %d\n", offset, len);
#endif

    DRV_CHECK_CONDITION(offset < _FLASH_MAX_SIZE, ERR_INVALID_ARGS,
                        "Invalid offset: 0x%x\n", offset);
    DRV_CHECK_CONDITION(len <= _FLASH_MAX_SIZE, ERR_INVALID_ARGS,
                        "Invalid len: 0x%x\n", len);
    DRV_CHECK_CONDITION((offset + len) <= _FLASH_MAX_SIZE, ERR_INVALID_ARGS,
                        "Invalid offset + len: 0x%x + 0x%x\n", offset, len);

    /* offset and len should be sector aligned */
    DRV_CHECK_CONDITION(__IS_ALIGN(offset, W25QXXDV_SECTOR_SIZE),
                        ERR_INVALID_ARGS, "Invalid offset: 0x%x\n", offset);
    DRV_CHECK_CONDITION(__IS_ALIGN(len, W25QXXDV_SECTOR_SIZE), ERR_INVALID_ARGS,
                        "Invalid len: 0x%x\n", len);

    if (offset == 0 && len == _FLASH_MAX_SIZE) {
        /* process whole flash */
        erase_sz = _FLASH_MAX_SIZE;
    } else if (__IS_ALIGN(offset, W25QXXDV_BLOCK_SIZE) &&
               (len == W25QXXDV_BLOCK_SIZE)) {
        /* process block */
        erase_sz = W25QXXDV_BLOCK_SIZE;
    } else if (__IS_ALIGN(offset, W25QXXDV_BLOCK32K_SIZE) &&
               (len == W25QXXDV_BLOCK32K_SIZE)) {
        /* process block32k erase */
        erase_sz = W25QXXDV_BLOCK32K_SIZE;
    } else if (len == W25QXXDV_SECTOR_SIZE) {
        /* process sector erase */
        erase_sz = W25QXXDV_SECTOR_SIZE;
    } else {
        erase_sz = 0;
    }

    DRV_CHECK_CONDITION(erase_sz, ERR_INVALID_ARGS,
                        "Invalid erase parameter: offset: 0x%x, size: 0x%x\n",
                        offset, len);

    ret = spi_flash_w25qxxdv_erase(offset, erase_sz);
    DRV_CHECK_RET("spi_flash_w25qxxdv_erase return failed: 0x%x\n", ret);
finish:
    return ret;
}

int32_t flash_drv_write_protection_set(bool enable)
{
    int32_t ret = 0;

    ret = spi_flash_w25qxxdv_write_protection_set(enable);
    DRV_CHECK_RET("spi_flash_w25qxxdv_write_protection_set %d failed!: "
                  "0x%x \n",
                  enable, ret);
finish:
    return ret;
}

