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

#define DBG_PRE_PROMPT "BS"
#include "dbg.h"

extern int32_t sys_device_do_config_level(int32_t level);

const char build_message[] =
    "Build Time: " __DATE__ " " __TIME__ " " BUILD_TYPE;

void proof_main(void);

int main(void)
{
    int32_t ret = 0;
	
	uart_init(0);
    ret = hal_platform_early_init();
    bk_printf("welcome to non-secure world\n");

#if CONFIG_MEM_LEAK
	extern int32_t mem_leak_check_init(void);
    ret = mem_leak_check_init();
    CHECK_RET("mem_leak_check_init failed!\n");
#endif

    ret = sys_device_do_config_level(DEVICE_LEVEL0);
    CHECK_RET("device level0 init fail 0x%08x\n", ret);

    bk_printf("==================================================================="
           "=======\n");
    bk_printf("| %-70s |\n", "version 1.0");
    bk_printf("| %-70s |\n", __DATE__);
    bk_printf("| %-70s |\n", __TIME__);
    bk_printf("==================================================================="
           "=======\n");

    ret = sys_device_do_config_level(DEVICE_LEVEL1);
    CHECK_RET("device level1 init fail 0x%08x\n", ret);

    ret = hal_platform_post_init();
    CHECK_RET("hal_platform_post_init failed!\n");

    ret = sys_device_do_config_level(DEVICE_LEVEL2);
    CHECK_RET("device level2 init fail 0x%08x\n", ret);

    ret = sys_device_do_config_level(DEVICE_LEVEL3);
    CHECK_RET("device level2 init fail 0x%08x\n", ret);

    apps_init();

    proof_main();
finish:
    /* should not be here */
    while (1)
        ;
}
// eof

