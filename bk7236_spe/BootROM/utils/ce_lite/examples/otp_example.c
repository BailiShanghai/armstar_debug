/*
 * Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#include "pal_log.h"
#include "ce_lite_config.h"
#include "mbedtls/otp.h"
#include "test_data_otp.h"

#define OTP_REGION_MAX_SIZE        (64U)

/**
 * There are some mbedtls otp examples for your reference.
 *
 * Example List:
 *  - Example 1: otp_example.
 *               It shows you how to write or read in OTP region.
 *               And for PUF OTP, it also shows how to do PUF OTP operation.
 *               To enable this example, you must define one of the macro
 *               \c CE_LITE_OTP_NORMAL, \c CE_LITE_OTP_PUF,
 *               \c CE_LITE_OTP_DUMMY in ce_lite_config.h.
 *
 * Usage:
 *  - Call otp_example_entry() function.
 *
 * Example Golden Data:
 *   Please refer to test_data_otp.h
 *
 * Warning:
 * 1. please use this example carefully, as it would
 * write the OTP and hence risk of damaging the chip.
 *
 * Note:
 * 1. please make sure that mbedtls_platform_setup()
 * have been called once before doing this example.
 * 2. please make sure that the otp regions are all zero
 * before doing this example.
 *
 */

static void dump_otp_config(const mbedtls_otp_conf *conf)
{
    if(NULL == conf) {
        return;
    }

    PAL_LOG_INFO("\t\tOTP config:\n");
    PAL_LOG_INFO("\t\t\totp exist: %s\n",
                  conf->otp_exist ? "true" : "false");
    PAL_LOG_INFO("\t\t\ttest region size: %d\n", conf->otp_tst_sz);
    PAL_LOG_INFO("\t\t\tsecure region size: %d\n", conf->otp_s_sz);
    PAL_LOG_INFO("\t\t\tnone secure region size: %d\n", conf->otp_ns_sz);

    return;
}

static int32_t otp_example(void)
{
    int32_t ret = 0;
    mbedtls_otp_conf conf = {0};
    size_t local_sz = 0;
    uint8_t local_buf[OTP_REGION_MAX_SIZE] = {0};

    /* get config */
    ret = mbedtls_otp_get_conf(&conf);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_otp_get_conf failed!\n");
        goto end;
    }
    dump_otp_config(&conf);

#if defined(CE_LITE_OTP_PUF)
#if defined(CE_LITE_OTP_PUF_ENROLL)
    /* LCS should not be DD or DR before enrollment */
    ret = mbedtls_otp_puf_enrollment();
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_otp_puf_enrollment failed!\n");
        goto end;
    }
#endif /* CE_LITE_OTP_PUF_ENROLL */

#if defined(CE_LITE_OTP_PUF_QUALITY_CHECK)
    ret = mbedtls_otp_puf_quality_check();
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_otp_puf_quality_check failed!\n");
        goto end;
    }
#endif /* CE_LITE_OTP_PUF_QUALITY_CHECK */

#if defined(CE_LITE_OTP_PUF_INIT_MGN_READ)
    /* take margin read device ID for example */
    local_sz = MBEDTLS_OTP_MODEL_ID_SIZE;
    ret = mbedtls_otp_puf_initial_margin_read(MODEL_ID,
                                              local_buf,
                                              &local_sz,
                                              0);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_otp_puf_initial_margin_read failed!\n");
        goto end;
    }
#endif /* CE_LITE_OTP_PUF_INIT_MGN_READ */

#if defined(CE_LITE_OTP_PUF_PGM_MGN_READ)
    local_sz = MBEDTLS_OTP_DEVICE_ID_SIZE;
    ret = mbedtls_otp_puf_pgm_margin_read(DEV_ID,
                                          local_buf,
                                          &local_sz,
                                          0);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_otp_puf_pgm_margin_read failed!\n");
        goto end;
    }
#endif /* CE_LITE_OTP_PUF_PGM_MGN_READ */
#endif /* CE_LITE_OTP_PUF */

#if defined(CE_LITE_OTP_WRITE)
    /* otp read/write model key */
    ret = mbedtls_set_otp_info_ex(MODEL_KEY,
                                  otp_model_key_data,
                                  MBEDTLS_OTP_MODEL_KEY_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write Model KEY:",
                     otp_model_key_data,
                     MBEDTLS_OTP_MODEL_KEY_SIZE);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(MODEL_KEY,
                                  local_buf,
                                  MBEDTLS_OTP_MODEL_KEY_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Read Model KEY:",
                     local_buf,
                     MBEDTLS_OTP_MODEL_KEY_SIZE);

#if !defined(CE_LITE_OTP_PUF)
#if defined(CE_LITE_OTP_WRITE)
    /* otp read/write device root key */
    ret = mbedtls_set_otp_info_ex(DEV_ROOT_KEY,
                                  otp_dev_root_key_data,
                                  MBEDTLS_OTP_DEVICE_RK_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write Device Root KEY:",
                     otp_dev_root_key_data,
                     MBEDTLS_OTP_DEVICE_RK_SIZE);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(DEV_ROOT_KEY,
                                  local_buf,
                                  MBEDTLS_OTP_DEVICE_RK_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Read Device Root Key:",
                     local_buf,
                     MBEDTLS_OTP_DEVICE_RK_SIZE);
#endif /* !CE_LITE_OTP_PUF */

#if defined(CE_LITE_OTP_WRITE)
    /* otp read/write lcs */
    ret = mbedtls_set_otp_info_ex(LIFE_CYCLE,
                                  otp_lcs_data,
                                  MBEDTLS_OTP_LCS_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write LCS:",
                     otp_lcs_data,
                     MBEDTLS_OTP_LCS_SIZE);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(LIFE_CYCLE,
                                  local_buf,
                                  MBEDTLS_OTP_LCS_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("READ LSC:",
                     local_buf,
                     MBEDTLS_OTP_LCS_SIZE);

#if !defined(CE_LITE_OTP_DUMMY)
#if defined(CE_LITE_OTP_WRITE)
    /* otp write/read model id */
    ret = mbedtls_set_otp_info_ex(MODEL_ID,
                                  otp_model_id_data,
                                  MBEDTLS_OTP_MODEL_ID_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write Model ID:",
                     otp_model_id_data,
                     MBEDTLS_OTP_MODEL_ID_SIZE);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(MODEL_ID,
                                  local_buf,
                                  MBEDTLS_OTP_MODEL_ID_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Read Model ID:",
                     local_buf,
                     MBEDTLS_OTP_MODEL_ID_SIZE);

#if defined(CE_LITE_OTP_WRITE)
    /* otp write/read device id */
    ret = mbedtls_set_otp_info_ex(DEV_ID,
                                  otp_dev_id_data,
                                  MBEDTLS_OTP_DEVICE_ID_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write Device ID:",
                     otp_dev_id_data,
                     MBEDTLS_OTP_DEVICE_ID_SIZE);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(DEV_ID,
                                  local_buf,
                                  MBEDTLS_OTP_DEVICE_ID_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Read Device ID:",
                     local_buf,
                     MBEDTLS_OTP_DEVICE_ID_SIZE);

#if defined(CE_LITE_OTP_WRITE)
    /* otp read/write secure boot hash */
    ret = mbedtls_set_otp_info_ex(SEC_BT_PK_HASH,
                                  otp_sec_btpk_hash_data,
                                  MBEDTLS_OTP_SEC_BOOT_HASH_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write Secure Boot Public HASH:",
                     otp_sec_btpk_hash_data,
                     MBEDTLS_OTP_SEC_BOOT_HASH_SIZE);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(SEC_BT_PK_HASH,
                                  local_buf,
                                  MBEDTLS_OTP_SEC_BOOT_HASH_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Read Secure Boot HASH:",
                     local_buf,
                     MBEDTLS_OTP_SEC_BOOT_HASH_SIZE);

#if defined(CE_LITE_OTP_WRITE)
    /* otp read/write secure debug pubkey hash */
    ret = mbedtls_set_otp_info_ex(SEC_DBG_PK_HASH,
                                  otp_sec_dbgpk_hash_data,
                                  MBEDTLS_OTP_SEC_DEBUG_HASH_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write Secure Debug Public HASH:",
                     otp_sec_dbgpk_hash_data,
                     MBEDTLS_OTP_SEC_DEBUG_HASH_SIZE);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(SEC_DBG_PK_HASH,
                                  local_buf,
                                  MBEDTLS_OTP_SEC_DEBUG_HASH_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Read Secure Debug Public HASH:",
                     local_buf,
                     MBEDTLS_OTP_SEC_DEBUG_HASH_SIZE);

#if defined(CE_LITE_OTP_WRITE)
    /* otp read/write none secure region */
    ret = mbedtls_set_otp_info_ex(USR_NON_SEC_REGION,
                                  otp_usr_nonsec_region_data,
                                  conf.otp_ns_sz,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write None Secure Region:",
                     otp_usr_nonsec_region_data,
                     conf.otp_ns_sz);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(USR_NON_SEC_REGION,
                                  local_buf,
                                  conf.otp_ns_sz,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Read None Secure Region:",
                     local_buf,
                     conf.otp_ns_sz);

#if defined(CE_LITE_OTP_WRITE)
    /* otp read/write secure region */
    ret = mbedtls_set_otp_info_ex(USR_SEC_REGION,
                                  otp_usr_sec_region_data,
                                  conf.otp_s_sz,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write Secure Region:",
                     otp_usr_sec_region_data,
                     conf.otp_s_sz);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(USR_SEC_REGION,
                                  local_buf,
                                  conf.otp_s_sz,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("READ Secure Region:",
                     local_buf,
                     conf.otp_s_sz);

#if defined(CE_LITE_OTP_WRITE)
    /* otp read/write test region */
    ret = mbedtls_set_otp_info_ex(TEST_REGION,
                                  otp_usr_test_region_data,
                                  conf.otp_tst_sz,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write Test Region:",
                     otp_usr_test_region_data,
                     conf.otp_tst_sz);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(TEST_REGION,
                                  local_buf,
                                  conf.otp_tst_sz,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("READ Test Region:",
                     local_buf,
                     conf.otp_tst_sz);
#endif /* !CE_LITE_OTP_DUMMY */

#if defined(CE_LITE_OTP_WRITE)
    /* otp read/write lock control */
    ret = mbedtls_set_otp_info_ex(LOCK_CTRL,
                                  otp_lck_data,
                                  MBEDTLS_OTP_LOCK_CTRL_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Write Lock Control:",
                     otp_lck_data,
                     MBEDTLS_OTP_LOCK_CTRL_SIZE);
#endif /* CE_LITE_OTP_WRITE */
    ret = mbedtls_get_otp_info_ex(LOCK_CTRL,
                                  local_buf,
                                  MBEDTLS_OTP_LOCK_CTRL_SIZE,
                                  0);
    if (ret != 0) {
        goto end;
    }
    PAL_LOG_DUMP_BUF("Read Lock Control",
                     local_buf,
                     MBEDTLS_OTP_LOCK_CTRL_SIZE);

end:
    return ret;
}

int32_t otp_example_entry(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("OTP example...\n");
    ret = otp_example();

    return ret;
}
