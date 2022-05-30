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
// eof
