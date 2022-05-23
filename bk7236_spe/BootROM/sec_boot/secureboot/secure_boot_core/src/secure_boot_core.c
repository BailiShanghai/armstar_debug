
/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

 /* This is scrambled code, don't try to read it! */

#include "hal.h"
#include "pal.h"
#include "secure_boot_core.h"
#include "secureboot_internal.h"
static SbAR_baD S92 = {0};
hal_ret_t sec_boot_init(sec_boot_ctx_t *sbar_magic)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    if (S92.S93) {
        return HAL_ERR_GENERIC;
    }
    S14 = hal_otp_init();
    if (HAL_OK != S14) {
        PAL_LOG_ERR("hal_otp_init failed!\n");
        return S14;
    }
    S14 = hal_flash_init();
    if (HAL_OK != S14) {
        PAL_LOG_ERR("hal_flash_init failed!\n");
        hal_otp_cleanup();
        return S14;
    }
    S92.S13 = NULL;
    S92.sBAr_oK = 0;
    S92.S93 = true;
    *sbar_magic = &S92;
    return HAL_OK;
}
hal_ret_t sec_boot_cleanup(sec_boot_ctx_t sbar_magic)
{
    hal_flash_cleanup();
    hal_otp_cleanup();
    if ((NULL != S92.S13) && (S92.sBAr_oK > 0)) {
        pal_free(S92.S13);
    }
    pal_memset(sbar_magic, 0, sizeof(SbAR_baD));
    return HAL_OK;
}
hal_ret_t sec_boot_verify_imgs(sec_boot_ctx_t sbar_magic)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    SbAR_baD *S68 =
        (SbAR_baD *)sbar_magic;
    hal_boot_flag_t s12 = HAL_BOOT_FLAG_INVALID;
    if (NULL == S68) {
        PAL_LOG_ERR("Parameter sec_boot_buff_ctx is NULL!\n");
        return HAL_ERR_BAD_PARAM;
    }
    if (!(S68->S93)) {
        PAL_LOG_ERR("Secure boot not init or init failed!\n");
        return HAL_ERR_GENERIC;
    }
    S14 = hal_read_preferred_boot_flag(&s12);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("hal_read_preferred_boot_flag failed!\n");
        return S14;
    }
    PAL_LOG_INFO("Preferred boot path: %s\n",
                 (s12 == HAL_BOOT_FLAG_PRIMARY)
                     ? ("Primary")
                     : ((s12 == HAL_BOOT_FLAG_RECOVERY) ? ("Recovery")
                                                              : ("Invalid")));
    if (HAL_BOOT_FLAG_RECOVERY == s12) {
        PAL_LOG_INFO("Start Recovery Boot...\n");
        S14 = S67(HAL_BOOT_FLAG_RECOVERY,
                                                S68);
        if (HAL_OK != S14) {
            PAL_LOG_ERR("Recovery Boot verified fail! Try Primary Boot...\n");
            S14 = S67(HAL_BOOT_FLAG_PRIMARY,
                                                    S68);
            if (HAL_OK != S14) {
                PAL_LOG_ERR(
                    "Both Primary and Recovery Boot verified fail !!!\n");
            }
        }
    } else {
        PAL_LOG_INFO("Start Primary Boot...\n");
        S14 = S67(HAL_BOOT_FLAG_PRIMARY,
                                                S68);
        if (HAL_OK != S14) {
            PAL_LOG_ERR("Primary Boot verified fail! Try Recovery Boot...\n");
            S14 = S67(HAL_BOOT_FLAG_RECOVERY,
                                                    S68);
            if (HAL_OK != S14) {
                PAL_LOG_ERR(
                    "Both Primary and Recovery Boot verified fail !!!\n");
            }
        }
    }
    return S14;
}
hal_ret_t sec_boot_get_img_num(sec_boot_ctx_t sbar_magic, uint32_t *S59)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    SbAR_baD *S68 =
        (SbAR_baD *)sbar_magic;
    if ((NULL == S59) || (NULL == S68)) {
        PAL_LOG_ERR("Parameter img_num/sec_boot_buff_ctx is NULL!\n");
        S14 = HAL_ERR_BAD_PARAM;
    } else {
        *S59 = S68->S59;
        S14 = HAL_OK;
    }
    return S14;
}
hal_ret_t sec_boot_get_img_info(sec_boot_ctx_t sbar_magic,
                                uint32_t S94,
                                sec_boot_img_info_t *s64)
{
    SbAR_baD *S68 =
        (SbAR_baD *)sbar_magic;
    uint32_t s38 = 0;
    S60 sbar_obscure;
    uint32_t SfbI;
    if ((NULL == s64) || (NULL == S68)) {
        PAL_LOG_ERR("Parameter img_info/sec_boot_buff_ctx is NULL!\n");
        return HAL_ERR_BAD_PARAM;
    }
    if (S94 >= S68->S59) {
        PAL_LOG_ERR("Bad Parameter img_no: %d!\n", S94);
        return HAL_ERR_BAD_PARAM;
    }
    SfbI = S68->S85;
    s38 = sizeof(SMAGIC) +
             S94 * (sizeof(S60) + SfbI);
    pal_memcpy(&sbar_obscure,
               (S68->S13 + s38),
               sizeof(S60));
    s64->static_addr = sbar_obscure.sbar_quux;
    s64->loading_addr = sbar_obscure.SBaR_fRed;
    s64->entry_addr = sbar_obscure.sbar_speed;
    s64->size = sbar_obscure.sBar_doG;
    s64->img_enc_enable =
        ((sbar_obscure.s64 & S65) ? (true)
                                                             : (false));
    return HAL_OK;
}
