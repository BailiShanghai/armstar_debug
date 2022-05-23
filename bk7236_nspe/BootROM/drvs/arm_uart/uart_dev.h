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

#ifndef __UART_DEV_H__
#define __UART_DEV_H__

#include <stdint.h>
#include "device.h"

#ifndef __ASSEMBLY__
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief UART device configuration.
 *
 * @param port Base port number * @param base Memory mapped base address
 * @param regs Register address
 * @param sys_clk_freq System clock frequency in Hz
 */
struct uart_device_config {
    union {
        uint32_t port;
        uint8_t *base;
        uint32_t regs;
    };

    uint32_t sys_clk_freq;
};

/* Device data structure */
struct uart_cmsdk_apb_dev_data {
    uint32_t baud_rate;    /* Baud rate */
};

struct uart_driver_api {
    /** Console I/O function */
    int32_t (*poll_in)(struct device *dev, unsigned char *p_char);
    int32_t (*poll_rx_ready)(struct device *dev);
    unsigned char (*poll_out)(struct device *dev, unsigned char out_char);
};

#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __UART_DEV_H__ */
