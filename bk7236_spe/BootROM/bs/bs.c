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
#include "app.h"
#include "bk_uart.h"
#include "verification_config.h"
#include "reg_base.h"
#include "soc.h"

#define DBG_PRE_PROMPT "BS"
#include "dbg.h"

extern int32_t sys_device_do_config_level(int32_t level);
extern void verification_main(void);

const char build_message[] =
    "Build Time: " __DATE__ " " __TIME__ " " BUILD_TYPE;

#define CONFIG_ENABLE_PLL    1

#if CONFIG_ENABLE_PLL
#define addSYSTEM_Reg0x40                 *((volatile unsigned long *) (0x44010000+0x40*4))
#define addSYSTEM_Reg0x45                 *((volatile unsigned long *) (0x44010000+0x45*4))
#define addSYSTEM_Reg0x46                 *((volatile unsigned long *) (0x44010000+0x46*4))
#define addSYSTEM_Reg0x8                  *((volatile unsigned long *) (0x44010000+0x8*4))
#define addSYSTEM_Reg0x9                  *((volatile unsigned long *) (0x44010000+0x9*4))

void hal_delay(int count)
{
	volatile int i = count;
	while (i > 0)
	{
		i --;
	}
}

/* From Wangjian, method to enable 7236 DPLL:
  * ana_reg5 set bit5
  * ana_reg0 set to 0x81185B57
  * ana_reg0 clear bit19
  * ana_reg0 set bit19
  */
void hal_enable_pll_120mhz(void)
{
	addSYSTEM_Reg0x45 = addSYSTEM_Reg0x45 | 0x20; 
	hal_delay(2000);
	
	addSYSTEM_Reg0x40 = 0x81185B57;

	hal_delay(2000);

	addSYSTEM_Reg0x40 &= 0xfff7ffff;
	addSYSTEM_Reg0x40 |= 0x00080000;

	addSYSTEM_Reg0x8 |= 0x033; 	// sel 480M PLL ; div4
}

void hal_enable_pll_240mhz(void)
{
	addSYSTEM_Reg0x45 = addSYSTEM_Reg0x45 | 0x20; 
	hal_delay(2000);
	
	addSYSTEM_Reg0x40 = 0x81185B57;

	hal_delay(2000);

	addSYSTEM_Reg0x40 &= 0xfff7ffff;
	addSYSTEM_Reg0x40 |= 0x00080000;

	addSYSTEM_Reg0x8 |= 0x031; 	// sel 480M PLL ; div2
}

void hal_printf_pll_registers(void)
{
	bk_printf("addSYSTEM_Reg0x45:0x%x\r\n", addSYSTEM_Reg0x45);
	bk_printf("addSYSTEM_Reg0x40:0x%x\r\n", addSYSTEM_Reg0x40);
	bk_printf("addSYSTEM_Reg0x8:0x%x\r\n", addSYSTEM_Reg0x8);
}
#endif /* CONFIG_ENABLE_PLL*/

void close_wdt(void)
{
	REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5A0000);
	REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA50000);
	REG_SET(SOC_WDT_REG_BASE + 4 * 2, 1, 1, 1);
	REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0x5A0000);
	REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0xA50000);
}

int main(void)
{
    int32_t ret = 0;

	close_wdt();
	
	uart_init(0);
	hal_enable_pll_120mhz();
	hal_printf_pll_registers();
	
	bk_printf("welcome to the secure world\r\n");
	
    ret = hal_platform_early_init();
    CHECK_RET("platform_early_init failed!\n");

#if CONFIG_MEM_LEAK
extern int32_t mem_leak_check_init(void);
    ret = mem_leak_check_init();
    CHECK_RET("mem_leak_check_init failed!\n");
#endif

	bk_printf("DEVICE_LEVEL0\r\n");
    ret = sys_device_do_config_level(DEVICE_LEVEL0);
    CHECK_RET("device level0 init fail 0x%08x\n", ret);

    bk_printf("==================================================================="
           "=======\n");
    bk_printf("| %-70s |\n", 
"version 1.0");
    bk_printf("| %-70s |\n", __DATE__);
    bk_printf("| %-70s |\n", __TIME__);
    bk_printf("==================================================================="
           "=======\n");

	#if CONFIG_ENABLE_PLATFORM_INIT
    ret = hal_platform_init();
    CHECK_RET("platform_init failed!\n");
	#else
	bk_printf("hal_platform_init bypass\r\n");
	#endif

	bk_printf("DEVICE_LEVEL1\r\n");
    ret = sys_device_do_config_level(DEVICE_LEVEL1);
    CHECK_RET("device level1 init fail 0x%08x\n", ret);

	#if CONFIG_ENABLE_PLATFORM_INIT
    ret = hal_platform_post_init();
    CHECK_RET("hal_platform_post_init failed!\n");
	#else
	bk_printf("hal_platform_post_init bypass\r\n");
	#endif

	bk_printf("DEVICE_LEVEL2\r\n");
    ret = sys_device_do_config_level(DEVICE_LEVEL2);
    CHECK_RET("device level2 init fail 0x%08x\n", ret);

	bk_printf("DEVICE_LEVEL3\r\n");
    ret = sys_device_do_config_level(DEVICE_LEVEL3);
    CHECK_RET("device level2 init fail 0x%08x\n", ret);

    apps_init();

    verification_main();
	
finish:
    /* should not be here */
    while (1)
    {
        ;
    }
		
		return 0;
}
// eof

