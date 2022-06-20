#include "base.h"
#include "bk_uart.h"
#include "reg_base.h"
#include "gpio_struct.h"
#include "bk_gpio.h"

gpio_hw_t *gpio_hw_ptr;

uint32_t gpio_uart1_config(void)
{
	gpio_hw_ptr = (gpio_hw_t *)SOC_AON_GPIO_REG_BASE;
	gpio_hw_ptr->gpio_num[GPIO_UART1_TX_ID].cfg.v = 0x78;
	gpio_hw_ptr->gpio_num[GPIO_UART1_RX_ID].cfg.v = 0x78;
	
	return 0;
}

uint32_t gpio_qspi_config(void)
{
	gpio_hw_ptr = (gpio_hw_t *)SOC_AON_GPIO_REG_BASE;
	gpio_hw_ptr->gpio_num[GPIO_QSPI_CLK_ID].cfg.v = 0x40;
	gpio_hw_ptr->gpio_num[GPIO_QSPI_CSN_ID].cfg.v = 0x40;
	gpio_hw_ptr->gpio_num[GPIO_QSPI_IO0_ID].cfg.v = 0x40;
	gpio_hw_ptr->gpio_num[GPIO_QSPI_IO1_ID].cfg.v = 0x40;
	gpio_hw_ptr->gpio_num[GPIO_QSPI_IO2_ID].cfg.v = 0x40;
	gpio_hw_ptr->gpio_num[GPIO_QSPI_IO3_ID].cfg.v = 0x40;

	return 0;
}
// eof
