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
#include "mem_layout.h"
#include "hal_platform.h"

#define DBG_PRE_PROMPT "SECBOOT"
#include "dbg.h"
#include "hal.h"

#if defined(ARM_CE_DUBHE)
#include "ce_lite_common.h"
#endif
#define _GET_LCS(__buf__) ((__buf__)[0] & 0x07)
#define _LCS_DM_DATA (0x1)

extern void jump_to_next(uint32_t addr);
extern int32_t secure_boot_sample(uint32_t *next_img_addr);

extern hal_ret_t _update_dubhe_otp_data(void);	////

/**
 * @brief secure boot main entrypoint.
 */
extern int32_t hal_platform_init(void); 	////Dubhe
void BootRom_main(void)
{
    uint32_t next_img_addr = 0;

#if SECURE_BOOT
    int32_t ret = 0;
    uint8_t life_cycle[HAL_OTP_LCS_SIZE] = {0};

    /*jtag disable at beginning*/
    hal_jtag_disable();		////Mark this for FPGA test

#if defined(CE_LITE_OTP_PUF)
    PAL_LOG_INFO("Select PUF OTP.\n");
#elif defined(CE_LITE_OTP_DUMMY) /* CE_LITE_OTP_PUF */
    PAL_LOG_INFO("Select dummy OTP.\n");
#else /* CE_LITE_OTP_DUMMY */
    PAL_LOG_INFO("Select regular OTP.\n");
#endif /* !CE_LITE_OTP_PUF && !CE_LITE_OTP_DUMMY */

    ret = secure_boot_sample(&next_img_addr);
    if (0 != ret) {
        ERR("secure_boot_sample fail: 0x%x\n", ret);
        goto exit;
    }

    ret = hal_otp_init();
    if (HAL_OK != ret) {
        ERR("HAL OTP init failed: 0x%x\n", ret);
        goto exit;
    }

    /* read life cycle */
    ret = hal_otp_read(HAL_OTP_LCS_OFFSET, life_cycle, HAL_OTP_LCS_SIZE);
    if (HAL_OK != ret) {
        ERR("HAL OTP read life cycle failed: 0x%x\n", ret);
        goto exit;
    }

    hal_otp_cleanup();

    if (_GET_LCS(life_cycle) <= _LCS_DM_DATA) {
        hal_jtag_enable();
    }
#else
    next_img_addr = NEXT_IMAGE_ADDR;
#endif

#if (ARM_CE_DUBHE)
    ce_drv_exit();
#endif

    jump_to_next(next_img_addr);

#if SECURE_BOOT
exit:
#if (ARM_CE_DUBHE)
    ce_drv_exit();
#endif
#endif

	ERR("bootrom unusual\r\n");
    while ( 1 ) {
    }
	
    return;
}
