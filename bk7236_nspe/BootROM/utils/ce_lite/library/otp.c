/*
 * Copyright (c) 2021, Arm Technology (China) Co., Ltd.
 * All rights reserved.
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,
 * any derivative work shall retain this copyright notice.
 */

#include "ce_lite_otp.h"
#include "ce_lite_common.h"
#include "ce_lite_config.h"
#include "mbedtls/otp.h"
#include "mbedtls/platform.h"

#if defined(CE_LITE_OTP)
static int _convert_retval_to_mbedtls(int errno)
{
    switch (errno) {
        case CE_SUCCESS:
            errno = 0;
            break;
        case CE_ERROR_BAD_PARAMS:
        case CE_ERROR_BAD_INPUT_LENGTH:
        case CE_ERROR_BAD_INPUT_DATA:
            errno = MBEDTLS_ERR_OTP_BAD_INPUT_DATA;
            break;
        case CE_ERROR_OVERFLOW:
            errno = MBEDTLS_ERR_OTP_OVERFLOW;
            break;
        case CE_ERROR_ACCESS_DENIED:
        case CE_ERROR_SECURITY:
        default:
            errno = MBEDTLS_ERR_OTP_ACCESS_DENIED;
            break;
    }

    return errno;
}

static int _covert_otp_info_to_offset(mbedtls_otp_info_t type)
{
    int ofs = 0;
    mbedtls_otp_conf conf = {0};

    switch (type) {
        case MODEL_ID:
            ofs = MBEDTLS_OTP_MODEL_ID_OFFSET;
            break;
        case MODEL_KEY:
            ofs = MBEDTLS_OTP_MODEL_KEY_OFFSET;
            break;
        case DEV_ID:
            ofs = MBEDTLS_OTP_DEVICE_ID_OFFSET;
            break;
        case DEV_ROOT_KEY:
            ofs = MBEDTLS_OTP_DEVICE_RK_OFFSET;
            break;
        case SEC_BT_PK_HASH:
            ofs = MBEDTLS_OTP_SEC_BOOT_HASH_OFFSET;
            break;
        case SEC_DBG_PK_HASH:
            ofs = MBEDTLS_OTP_SEC_DEBUG_HASH_OFFSET;
            break;
        case LIFE_CYCLE:
            ofs = MBEDTLS_OTP_LCS_OFFSET;
            break;
        case LOCK_CTRL:
            ofs = MBEDTLS_OTP_LOCK_CTRL_OFFSET;
            break;
        case USR_NON_SEC_REGION:
            ofs = MBEDTLS_OTP_USER_NON_SEC_REGION_OFFSET;
            break;
        case USR_SEC_REGION:
            mbedtls_otp_get_conf(&conf);
            ofs = MBEDTLS_OTP_USER_SEC_REGION_OFFSET((&conf));
            break;
        case TEST_REGION:
            mbedtls_otp_get_conf(&conf);
            ofs = MBEDTLS_OTP_TEST_REGION_OFFSET((&conf));
            break;
        default:
            PAL_LOG_ERR("otp type error!\n");
            while (1);
    }

    return ofs;
}

int mbedtls_get_otp_info_ex(mbedtls_otp_info_t type,
                            void *output,
                            size_t size,
                            size_t offset)
{
    return _convert_retval_to_mbedtls(
            ce_otp_read(_covert_otp_info_to_offset(type) + offset,
                        output,
                        size));
}

int mbedtls_otp_get_conf(mbedtls_otp_conf *conf)
{
    int ret = 0;
    ce_otp_cfg_info_t local_conf = {0};

    if (NULL == conf) {
        return MBEDTLS_ERR_OTP_BAD_INPUT_DATA;
    }

    ret = ce_otp_get_cfg_info(&local_conf);
    if (CE_SUCCESS == ret) {
        conf->otp_exist = local_conf.otp_exist;
        conf->otp_tst_sz = local_conf.otp_tst_sz;
        conf->otp_s_sz = local_conf.otp_s_sz;
        conf->otp_ns_sz = local_conf.otp_ns_sz;
    }

    return _convert_retval_to_mbedtls(ret);
}

#if defined(CE_LITE_OTP_WRITE)
int mbedtls_set_otp_info_ex(mbedtls_otp_info_t type,
                            const void *input,
                            size_t ilen,
                            size_t offset)
{
    return _convert_retval_to_mbedtls(
            ce_otp_write(_covert_otp_info_to_offset(type) + offset,
                         input,
                         ilen));
}
#endif /* CE_LITE_OTP_WRITE */

#if defined(CE_LITE_OTP_PUF)
#if defined(CE_LITE_OTP_PUF_ENROLL)
int mbedtls_otp_puf_enrollment(void)
{
    return _convert_retval_to_mbedtls(
            ce_otp_puf_enroll());
}
#endif /* CE_LITE_OTP_PUF_ENROLL */

#if defined(CE_LITE_OTP_PUF_QUALITY_CHECK)
int mbedtls_otp_puf_quality_check(void)
{
    return _convert_retval_to_mbedtls(
            ce_otp_puf_quality_check());
}
#endif /* CE_LITE_OTP_PUF_QUALITY_CHECK */

#if defined(CE_LITE_OTP_PUF_INIT_MGN_READ)
int mbedtls_otp_puf_initial_margin_read(mbedtls_otp_info_t area,
                                        void *output,
                                        size_t *size,
                                        size_t offset)
{
    return _convert_retval_to_mbedtls(
            ce_otp_puf_init_mgn_read(_covert_otp_info_to_offset(area) + offset,
                                     output,
                                     *size));
}
#endif /* CE_LITE_OTP_PUF_INIT_MGN_READ */

#if defined(CE_LITE_OTP_PUF_PGM_MGN_READ)
int mbedtls_otp_puf_pgm_margin_read(mbedtls_otp_info_t area,
                                    void *output,
                                    size_t *size,
                                    size_t offset)
{
    return _convert_retval_to_mbedtls(
            ce_otp_puf_pgm_mgn_read(_covert_otp_info_to_offset(area) + offset,
                                     output,
                                     *size));
}
#endif /* CE_LITE_OTP_PUF_PGM_MGN_READ */
#endif /* CE_LITE_OTP_PUF */
#endif /* CE_LICE_OTP */
