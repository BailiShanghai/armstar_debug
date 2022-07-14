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

	#if CONFIG_ENABLE_NSPE_PLATFORM_INIT
	uart_init(0);
    ret = hal_platform_early_init();
    bk_printf("welcome to non-secure world\n");

    bk_printf("==================================================================="
           "=======\n");
    bk_printf("| %-70s |\n", 
"version 1.0");
    bk_printf("| %-70s |\n", __DATE__);
    bk_printf("| %-70s |\n", __TIME__);
    bk_printf("==================================================================="
           "=======\n");
	#endif
		
    proof_main();
	
finish:
    /* should not be here */
    while (1)
        ;
}
// eof

