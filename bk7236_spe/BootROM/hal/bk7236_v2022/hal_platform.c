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
#include "hal_platform.h"
#include "platform.h"
#include "uart.h"

#include "mbedtls/platform.h"

#if (ARM_CE_DUBHE)
#include "ce_lite_common.h"
#endif

extern int32_t platform_init(void);
extern int32_t device_init(void);
extern void global_heap_init(void);
////Workaround
extern int stdout_putchar (int ch) ;
extern void bk_putchar(char c);

int32_t hal_platform_early_init(void)
{
    global_heap_init();
    return 0;
}

void mbedtls_init(void)
{
    mbedtls_platform_set_printf(printf);
    mbedtls_platform_set_calloc_free(kcalloc, kfree);
#if (ARM_CE_DUBHE)
    ce_drv_init();
#endif
    return;
}

int32_t hal_platform_init(void)
{
    mbedtls_init();
    return 0;
}

int32_t hal_platform_post_init(void)
{
    return 0;
}

void hal_udelay(uint32_t usecs)
{
    arch_sleep_us(usecs);
}

void hal_mdelay(uint32_t msecs)
{
    arch_sleep_us(msecs * 1000);
}

uint32_t hal_get_timestamp_second()
{
    return platform_get_timestamp_second();
}

uint32_t hal_get_timestamp_counter()
{
    return platform_get_timestamp_counter();
}

uint32_t hal_get_rand()
{
    return platform_get_rand_seed();
}

void hal_reset(void)
{
    SEC_WATCHDOG->CTRL = CMSDK_Watchdog_CTRL_RESEN_Msk;  // enable reset
    SEC_WATCHDOG->LOAD = 0x1ul;  // the minimum value of load
    SEC_WATCHDOG->ITOP = CMSDK_Watchdog_INTEGTESTEN_Msk;
    SEC_WATCHDOG->ITCR = CMSDK_Watchdog_INTEGTESTEN_Msk;
	
    while (1);  // wait for watchdog reset
}

void hal_uart_putc(uint8_t c)
{
    if ('\n' == c) {
        bk_putchar('\r');
    }
    return bk_putchar(c);
}

void hal_uart_putc_strict(uint8_t c)
{
	bk_putchar(c);
}

size_t hal_uart_puts(const uint8_t *s)
{
    int i      = 0;
    size_t len = strlen((const char *)s);

    for (i = 0; i < len; i++) {
        hal_uart_putc(s[i]);
    }
    return len;
}

int32_t hal_uart_getc(uint8_t *c)
{
    return 0;
}

/*
 * no data input return 0
 * other return !0
 *
 */
int32_t hal_uart_rx_ready()
{
    return 0;
}


#define MPS2_AN495_FPGA_LED_BASE MPS2_IO_FPGAIO_BASE_NS
void hal_led_on_off(int32_t led_id, bool is_on)
{
    if ((0 == led_id) || (1 == led_id)) {
        if (is_on) {
            *((volatile uint32_t *)MPS2_AN495_FPGA_LED_BASE) |= (0x1 << led_id);
        } else {
            *((volatile uint32_t *)MPS2_AN495_FPGA_LED_BASE) &= (~(0x1 << led_id));
        }
    }
    return;
}

#define _MPS2_SCC_S ((volatile MPS2_SCC_TypeDef *)MPS2_IO_SCC_BASE_S)
void hal_mcc_led_op(int32_t led_id, bool is_on)
{

    if (!((0 <= led_id) && (led_id <= 7))) {
        return;
    }

    if (is_on) {
        _MPS2_SCC_S->LEDS = _MPS2_SCC_S->LEDS | (0x1 << led_id);
    } else {
        _MPS2_SCC_S->LEDS = _MPS2_SCC_S->LEDS & (~(0x1 << led_id));
    }

    return;
}

#define CORTEX_M33_REG_DAUTHCTRL 0xE000EE04
#define CORTEX_M33_CPU_DEBUG_ENABLE_VALUE 0x0000000F
#define CORTEX_M33_CPU_DEBUG_DISABLE_VALUE 0x00000005
void hal_jtag_enable(void)
{
    *((volatile uint32_t*)CORTEX_M33_REG_DAUTHCTRL) =
        CORTEX_M33_CPU_DEBUG_ENABLE_VALUE;/*DAUTHCTRL register, jtag debug  enable*/
}

void hal_jtag_disable(void)
{
    *((volatile uint32_t*)CORTEX_M33_REG_DAUTHCTRL) =
        CORTEX_M33_CPU_DEBUG_DISABLE_VALUE;/*DAUTHCTRL register, jtag  disable*/
}
