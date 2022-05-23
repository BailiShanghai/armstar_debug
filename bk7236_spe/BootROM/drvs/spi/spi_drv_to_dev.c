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
#include "err.h"
#include "device.h"
#include "spi_dev.h"
#include "spi_pl022/spi_drv.h"
#include "spi_pl022/spi_pl022.h"

#define DRV_PROMPT "SPI DRV"
#include "drv_dep.h"

int32_t spi_init(struct device *dev)
{
    DRV_DBG("spi drv init!\n");
    struct spi_dev_data *data = dev->driver_data;

    if (NULL == data) {
        return ERR_NOT_FOUND;
    }
    if (spi_pl022_init(data->port, data->base_addr,
                       data->clk_freq, data->cs_select)) {
        return ERR_GENERIC_HW_FAULT;
    }
    return NO_ERROR;
}

static int32_t _spi_transceive(struct device *dev,
                         const struct spi_config *config,
                         const struct spi_buf_set *tx_bufs,
                         const struct spi_buf_set *rx_bufs)
{
    struct spi_dev_data *data = dev->driver_data;

    if (NULL == data) {
        return ERR_NOT_FOUND;
    }

    if (spi_pl022_transceive(data->port, config, tx_bufs, rx_bufs)) {
        return ERR_GENERIC_HW_FAULT;
    }
    return NO_ERROR;
}

const struct spi_driver_api spi_api_funcs = {
    .transceive = _spi_transceive,
    .release = NULL,
};

int32_t spi_transceive(struct device *dev,
                       struct spi_config *config,
                       struct spi_buf_set *tx_bufs,
                       struct spi_buf_set *rx_bufs)
{
    const struct spi_driver_api *api = dev->driver_api;

    if (NULL == api) {
        return ERR_NOT_FOUND;
    }

    return api->transceive(dev, config, tx_bufs, rx_bufs);
}

int32_t spi_read(struct device *dev,
                 struct spi_config *config,
                 struct spi_buf_set *rx_bufs)
{
    const struct spi_driver_api *api = dev->driver_api;

    if (NULL == api) {
        return ERR_NOT_FOUND;
    }

    return api->transceive(dev, config, NULL, rx_bufs);
}

int32_t spi_write(struct device *dev,
                  struct spi_config *config,
                  struct spi_buf_set *tx_bufs)
{
    const struct spi_driver_api *api = dev->driver_api;

    if (NULL == api) {
        return ERR_NOT_FOUND;
    }

    return api->transceive(dev, config, tx_bufs, NULL);
}

