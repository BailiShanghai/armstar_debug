/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 *
 * @brief This file defines the private data structures for spi flash driver
 */
/* copy from zephyr */
#ifndef __SPI_FLASH_W25QXXDV_H__
#define __SPI_FLASH_W25QXXDV_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>  /* for off_t */

#define SPI_FLASH_W25QXXDV_MAX_DATA_LEN    (256)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _spi_flash_w25qxxdv_config_t {
    uint32_t spi_freq;
    uint32_t spi_slave_num;
    uint32_t w25qxxdv_id_value;
    uint32_t max_size;
} spi_flash_w25qxxdv_config_t;

extern int spi_flash_w25qxxdv_erase(off_t offset, size_t len);
extern int spi_flash_w25qxxdv_init(spi_flash_w25qxxdv_config_t *config);
extern int spi_flash_w25qxxdv_write_protection_set(bool enable);
extern int spi_flash_w25qxxdv_write(off_t offset, const void *data, size_t len);
extern int spi_flash_w25qxxdv_read(off_t offset, void *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_W25QXXDV_H__ */
