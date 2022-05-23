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

#include "arm_uart_drv.h"

/* UART Bits */
/* CTRL Register */
#define UART_TX_EN  (1 << 0)
#define UART_RX_EN  (1 << 1)
#define UART_TX_IN_EN   (1 << 2)
#define UART_RX_IN_EN   (1 << 3)
#define UART_TX_OV_EN   (1 << 4)
#define UART_RX_OV_EN   (1 << 5)
#define UART_HS_TM_TX   (1 << 6)

/* STATE Register */
#define UART_TX_BF  (1 << 0)
#define UART_RX_BF  (1 << 1)
#define UART_TX_B_OV    (1 << 2)
#define UART_RX_B_OV    (1 << 3)

/* INTSTATUS Register */
#define UART_TX_IN  (1 << 0)
#define UART_RX_IN  (1 << 1)
#define UART_TX_OV_IN   (1 << 2)
#define UART_RX_OV_IN   (1 << 3)

/**
 * @brief Set the baud rate
 *
 * This routine set the given baud rate for the UART.
 *
 * @param dev UART device struct
 *
 * @return N/A
 */
static void baudrate_set(volatile struct uart_cmsdk_apb *uart,
                         uint32_t sys_clk_freq,
                         uint32_t baud_rate)
{
    /*
     * If baudrate and/or sys_clk_freq are 0 the configuration remains
     * unchanged. It can be useful in case that Zephyr it is run via
     * a bootloader that brings up the serial and sets the baudrate.
     */
    if ((baud_rate != 0) && (sys_clk_freq != 0)) {
        /* calculate baud rate divisor */
        uart->bauddiv = (sys_clk_freq / baud_rate);
    }
}

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
int32_t uart_cmsdk_apb_init(volatile struct uart_cmsdk_apb *uart,
                            uint32_t sys_clk_freq,
                            uint32_t baud_rate)
{
    /* Set baud rate */
    baudrate_set(uart, sys_clk_freq, baud_rate);

    /* Enable receiver and transmitter */
    uart->ctrl = UART_RX_EN | UART_TX_EN;

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

int32_t uart_cmsdk_apb_poll_in(volatile struct uart_cmsdk_apb *uart, unsigned char *c)
{
   /* Wait for receiver to be ready */
    while (!(uart->state & UART_RX_BF)) {
        ; /* Wait */
    }

    /* got a character */
    *c = (unsigned char)uart->data;

    return 0;
}

int32_t uart_cmsdk_apb_poll_rx_ready(volatile struct uart_cmsdk_apb *uart)
{
    return uart->state & UART_RX_BF;
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
unsigned char uart_cmsdk_apb_poll_out(volatile struct uart_cmsdk_apb *uart,
                                      unsigned char c)
{
    /* Wait for transmitter to be ready */
    while (uart->state & UART_TX_BF) {
        ; /* Wait */
    }

    /* Send a character */
    uart->data = (uint32_t)c;
    return c;
}

