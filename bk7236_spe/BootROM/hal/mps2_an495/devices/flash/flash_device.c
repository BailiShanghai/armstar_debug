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
#include "flash/flash_dev.h"

#define DRV_NAME "spi_flash_drv"

extern const struct flash_driver_api flash_api_funcs;
extern int32_t flash_init(struct device *dev);

static struct flash_dev_data _flash_dev_data = {
    .max_flash_size                    = 16 * 1024 * 1024,
    .spi_flash_config.spi_freq         = 12000000,
    .spi_flash_config.spi_slave_num    = 0,
    .spi_flash_config.spi_dev_id_value = 0x72362022,
};

DEVICE_DEFINE(spi_flash,
              DRV_NAME,
              &flash_init,
              &_flash_dev_data,
              NULL,
              DEVICE_LEVEL3,
              1,
              &flash_api_funcs);

struct device *spi_flash_device = DEVICE_GET(spi_flash);
