/*
 * Copyright (c) 2016 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @brief Driver for UART on ARM CMSDK APB UART.
 *
 * UART has two wires for RX and TX, and does not provide CTS or RTS.
 */

#include <stdint.h>
#include <stddef.h>
#include "device.h"
#include "arm_uart_drv.h"
#include "uart_dev.h"
#include "bk_uart.h" //TODO fix me!

bool g_uart_is_init = false;

/* convenience defines */
#define DEV_CFG(dev) \
    ((const struct uart_device_config * const)(dev)->config->config_info)
#define DEV_DATA(dev) \
    ((struct uart_cmsdk_apb_dev_data * const)(dev)->driver_data)
#define UART_STRUCT(dev) \
    ((volatile struct uart_cmsdk_apb *)(DEV_CFG(dev))->base)

/**
 * @brief Initialize UART channel
 *
 * This routine is called to reset the chip in a quiescent state.
 * It is assumed that this function is called only once per UART.
 *
 * @param dev UART device struct
 *
 * @return 0
 */
 
int32_t uart_init(struct device *dev)
{
	(void)dev;
	
	bk_uart1_init();
	g_uart_is_init = true;

    return 0;
}

/**
 * @brief Poll the device for input.
 *
 * @param dev UART device struct
 * @param c Pointer to character
 *
 * @return 0 if a character arrived, -1 if the input buffer if empty.
 */

static int32_t _uart_poll_in(struct device *dev, unsigned char *c)
{
    volatile struct uart_cmsdk_apb *uart = UART_STRUCT(dev);

    return uart_cmsdk_apb_poll_in(uart, c);
}

static int32_t _uart_poll_rx_ready(struct device *dev)
{
    volatile struct uart_cmsdk_apb *uart = UART_STRUCT(dev);

    return uart_cmsdk_apb_poll_rx_ready(uart);
}

/**
 * @brief Output a character in polled mode.
 *
 * Checks if the transmitter is empty. If empty, a character is written to
 * the data register.
 *
 * @param dev UART device struct
 * @param c Character to send
 *
 * @return Sent character
 */
static unsigned char _uart_poll_out(struct device *dev,
                         unsigned char c)
{
    volatile struct uart_cmsdk_apb *uart = UART_STRUCT(dev);

    return uart_cmsdk_apb_poll_out(uart, c);
}

const struct uart_driver_api uart_cmsdk_apb_driver_api = {
    .poll_in = _uart_poll_in,
    .poll_out = _uart_poll_out,
    .poll_rx_ready = _uart_poll_rx_ready,
};

void uart_putc(struct device *dev, uint8_t c)
{
    const struct uart_driver_api* drv_api;

    if (NULL == dev) {
        while(1);
    }

    drv_api = dev->driver_api;
    if (NULL == drv_api) {
        while(1);
    }

    drv_api->poll_out(dev, c);
}

int32_t uart_getc(struct device *dev, uint8_t *c)
{
    const struct uart_driver_api* drv_api;

    if (NULL == dev || NULL == c) {
        while(1);
    }

    drv_api = dev->driver_api;
    if (NULL == drv_api) {
        while(1);
    }

    return drv_api->poll_in(dev, c);
}

int32_t uart_rx_ready(struct device *dev)
{
    const struct uart_driver_api* drv_api;

    if (NULL == dev) {
        return 0;
    }

    drv_api = dev->driver_api;
    if (NULL == drv_api) {
        return 0;
    }

    return drv_api->poll_rx_ready(dev);
}
