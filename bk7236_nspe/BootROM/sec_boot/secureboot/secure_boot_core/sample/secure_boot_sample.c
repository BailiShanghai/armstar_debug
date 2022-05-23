/**
 * @if copyright_display
 *              Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *              All rights reserved
 *
 *          The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *              to Arm Technology (China) Co., Ltd. It is subject to the terms
 * of a License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *              restricting among other things, the use, reproduction,
 * distribution and transfer.  Each of the embodiments, including this
 * information and any derivative work shall retain this copyright notice.
 * @endif
 */

#include <stdio.h>
#include "secure_boot_core.h"

extern void jump_to_image(uint32_t load_addr);

int secure_boot_do_boot(void)
{
    uint32_t img_num             = 0;
    hal_ret_t ret                = HAL_OK;
    sec_boot_ctx_t sec_boot_ctx  = NULL;
    sec_boot_img_info_t img_info = {0};

    ret = sec_boot_init(&sec_boot_ctx);
    if (HAL_OK != ret) {
        printf("sec_boot_init failed!\n");
        goto finish;
    }

    ret = sec_boot_verify_imgs(sec_boot_ctx);
    if (HAL_OK != ret) {
        printf("sec_boot_verify_imgs failed!\n");
        goto finish;
    }

    ret = sec_boot_get_img_num(sec_boot_ctx, &img_num);
    if (HAL_OK != ret) {
        printf("sec_boot_get_img_num failed!\n");
        goto finish;
    }
    printf("image number:%d\n", img_num);

    /*there should be at least one image(index 0 used as below)
     *in secure boot manifest
     */
    ret = sec_boot_get_img_info(sec_boot_ctx, 0, &img_info);
    if (HAL_OK != ret) {
        printf("sec_boot_get_img_info failed!\n");
        goto finish;
    }

    ret = sec_boot_cleanup(sec_boot_ctx);
    if (HAL_OK != ret) {
        printf("sec_boot_cleanup failed!\n");
        goto finish;
    }

    printf("img_info.entry_addr: 0x%x\n", (unsigned int)img_info.entry_addr);

finish:
    return (int)ret;
}
