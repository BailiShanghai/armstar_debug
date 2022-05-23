/*
 * Copyright (c) 2016-2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file arm_uart_drv.h
 * \brief Generic driver for ARM UART.
 */

#ifndef __ARM_UART_DRV_H__
#define __ARM_UART_DRV_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UART registers struct */
struct uart_cmsdk_apb {
    /* offset: 0x000 (r/w) data register    */
    volatile uint32_t  data;
    /* offset: 0x004 (r/w) status register  */
    volatile uint32_t  state;
    /* offset: 0x008 (r/w) control register */
    volatile uint32_t  ctrl;
    union {
        /* offset: 0x00c (r/ ) interrupt status register */
        volatile uint32_t  intstatus;
        /* offset: 0x00c ( /w) interrupt clear register  */
        volatile uint32_t  intclear;
    };
    /* offset: 0x010 (r/w) baudrate divider register */
    volatile uint32_t  bauddiv;
};

int32_t uart_cmsdk_apb_init(volatile struct uart_cmsdk_apb *uart,
                            uint32_t sys_clk_freq,
                            uint32_t baud_rate);
int32_t uart_cmsdk_apb_poll_in(volatile struct uart_cmsdk_apb *uart, unsigned char *c);
int32_t uart_cmsdk_apb_poll_rx_ready(volatile struct uart_cmsdk_apb *uart);
unsigned char uart_cmsdk_apb_poll_out(volatile struct uart_cmsdk_apb *uart,
                                      unsigned char c);
#ifdef __cplusplus
}
#endif
#endif /* __ARM_UART_DRV_H__ */
