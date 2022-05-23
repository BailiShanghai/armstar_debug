/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#include "base.h"
#include "platform.h"
#include "spi/spi_dev.h"

#define DRV_NAME "spi_drv"

extern const struct spi_driver_api spi_api_funcs;
extern int32_t spi_init(struct device *dev);

static void cs_select(uint32_t port, bool is_select)
{
    if (0 == port) {
        if (is_select) {
            __MPS2_FPGAIO->misc &= (~(0x1 << FPGAIO_MISC_SPI_SS));
        } else {
            __MPS2_FPGAIO->misc |= (0x1 << FPGAIO_MISC_SPI_SS);
        }
    }
}

static struct spi_dev_data spi_dev_data_0 = {
    .port      = 0,
    .base_addr = MPS2_SSP1_BASE_NS,
    .clk_freq  = PERIPHERAL_CLOCK,
    .cs_select = cs_select,
};

DEVICE_DEFINE(spi_0,
              DRV_NAME,
              &spi_init,
              &spi_dev_data_0,
              NULL,
              DEVICE_LEVEL2,
              0,
              &spi_api_funcs);

struct device *spi_0_device = DEVICE_GET(spi_0);
