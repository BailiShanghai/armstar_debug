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
#include "STAR_SE.h"

#define DBG_PRE_PROMPT "BS"
#include "dbg.h"

extern int32_t sys_device_do_config_level(int32_t level);
extern void verification_main(void);

const char build_message[] =
    "Build Time: " __DATE__ " " __TIME__ " " BUILD_TYPE;

void close_wdt(void)
{
    REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5A0000);
    REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA50000);
    REG_SET(SOC_WDT_REG_BASE + 4 * 2, 1, 1, 1);
    REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0x5A0000);
    REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0xA50000);
}

int32_t uart_init(struct device *dev);

int main(void)
{
    int32_t ret = 0;

    close_wdt();

	#if CONFIG_DIABLE_DEBUG_FEATURE
    hal_cpu_debug_disable();
	#endif

    uart_init(0);

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

