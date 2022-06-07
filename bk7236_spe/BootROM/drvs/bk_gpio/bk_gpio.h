#pragma once
#include "gpio_struct.h"

#define GPIO_UART1_TX_ID    (11)
#define GPIO_UART1_RX_ID    (10)

#define GPIO_QSPI_CLK_ID    (22)
#define GPIO_QSPI_CSN_ID    (23)
#define GPIO_QSPI_IO0_ID    (24)
#define GPIO_QSPI_IO1_ID    (25)
#define GPIO_QSPI_IO2_ID    (26)
#define GPIO_QSPI_IO3_ID    (27)

extern uint32_t gpio_uart1_config(void);
extern uint32_t gpio_qspi_config(void);

// eof

