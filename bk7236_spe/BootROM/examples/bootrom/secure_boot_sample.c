/**
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and
 * any derivative work shall retain this copyright notice.
 *
 */
#include "base.h"
#include "pal.h"
#include "secure_boot_core.h"

int32_t secure_boot_sample(uint32_t *next_img_addr)
{
    hal_ret_t ret                = HAL_OK;
    sec_boot_ctx_t sec_boot_ctx  = NULL;
    sec_boot_img_info_t img_info = {0};

    ret = sec_boot_init(&sec_boot_ctx);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("sec_boot_init failed!\n");
        goto finish;
    }

    ret = sec_boot_verify_imgs(sec_boot_ctx);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("sec_boot_verify_imgs failed!\n");
        goto finish;
    }

    ret = sec_boot_get_img_info(sec_boot_ctx, 0, &img_info);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("sec_boot_get_img_info failed!\n");
        goto finish;
    }

    ret = sec_boot_cleanup(sec_boot_ctx);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("sec_boot_cleanup failed!\n");
        goto finish;
    }

    PAL_LOG_DEBUG("img_info.entry_addr: 0x%x\n",
                  (uint32_t)img_info.entry_addr);
    *next_img_addr = (uint32_t)img_info.entry_addr;

finish:
    return (int)ret;
}