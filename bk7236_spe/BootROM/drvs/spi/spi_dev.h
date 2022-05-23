/*
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Public API for SPI drivers and applications
 */

#ifndef __SPI_DEV_H__
#define __SPI_DEV_H__

#include <stdint.h>
#include "device.h"
#include "spi/spi_pl022/spi_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ADDR_T_DEFINED__
typedef unsigned long addr_t;
#define __ADDR_T_DEFINED__
#endif

extern struct device * spi_0_device;
/**
 * @typedef spi_api_io
 * @brief Callback API for I/O
 * See spi_transceive() for argument descriptions
 */
typedef int32_t (*spi_api_io)(struct device *dev,
             const struct spi_config *config,
             const struct spi_buf_set *tx_bufs,
             const struct spi_buf_set *rx_bufs);

/**
 * @typedef spi_api_release
 * @brief Callback API for unlocking SPI device.
 * See spi_release() for argument descriptions
 */
typedef int32_t (*spi_api_release)(struct device *dev,
            const struct spi_config *config);

struct spi_dev_data {
    uint32_t port;
    addr_t base_addr;
    uint32_t clk_freq;
    void (*cs_select)(uint32_t port, bool is_select);
    union {
        struct _user_data_type1_t {
            uint32_t data1;
            uint32_t data2;
        } user_data_type1;
    };
};

/**
 * @brief SPI driver API
 * This is the mandatory API any SPI driver needs to expose.
 */
struct spi_driver_api {
    spi_api_io transceive;
    spi_api_release release;
};

int32_t spi_init(struct device *dev);
int32_t spi_transceive(struct device *dev,
                       struct spi_config *config,
                       struct spi_buf_set *tx_bufs,
                       struct spi_buf_set *rx_bufs);
int32_t spi_read(struct device *dev,
                 struct spi_config *config,
                 struct spi_buf_set *rx_bufs);
int32_t spi_write(struct device *dev,
                  struct spi_config *config,
                  struct spi_buf_set *tx_bufs);

#ifdef __cplusplus
}
#endif
#endif /* __SPI_DEV_H__ */
