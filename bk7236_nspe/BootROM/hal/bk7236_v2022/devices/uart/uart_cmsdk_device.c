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
#include "arm_uart/uart_dev.h"
#include "uart.h"

#define DEFAULT_UART_BAUDRATE 115200
#define DRV_NAME "uart0"

static const struct uart_device_config uart_cmsdk_apb_dev_cfg_0 = {
    .base = (uint8_t *)UART0_BASE_NS, .sys_clk_freq = UART_CLOCK,
};

static struct uart_cmsdk_apb_dev_data uart_cmsdk_apb_dev_data_0 = {
    .baud_rate = (uint32_t)DEFAULT_UART_BAUDRATE,
};

extern const struct uart_driver_api uart_cmsdk_apb_driver_api;

DEVICE_DEFINE(uart_cmsdk_apb_0,
              DRV_NAME,
              &uart_init,
              &uart_cmsdk_apb_dev_data_0,
              &uart_cmsdk_apb_dev_cfg_0,
              DEVICE_LEVEL0,
              0,
              &uart_cmsdk_apb_driver_api);

struct device *uart_0_dev = DEVICE_GET(uart_cmsdk_apb_0);
