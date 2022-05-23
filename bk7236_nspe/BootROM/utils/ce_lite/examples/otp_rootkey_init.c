
/*
 * Copyright (C), 2021, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#include "ce_lite_config.h"
#include "pal_common.h"
#include "pal_log.h"
#include "pal_string.h"
#include "mbedtls/otp.h"
#include "test_data_otp.h"
#include "otp_rootkey_init.h"

int otp_rootkey_init(void)
{
#if defined(CE_LITE_OTP) &&  \
    defined(CE_LITE_OTP_NORMAL) && \
    defined(CE_LITE_OTP_WRITE) && \
    (CE_CFG_HOST_ID == 0)
    static bool is_rootkey_init = false;
    int ret = 0;

    /**
     * setting the root key if only it's not set yet.
     *
     * Note:
     *  the writing OTP root key can take place once and only
     *  once in a power cycle
     */
    if (is_rootkey_init) {
        return 0;
    }

    /**
     * \note The example only use 'ROOT_KEY'.
     *       Alternatively, you can set 'MODEL_KEY' by
     *       mbedtls_set_otp_info_ex(MODEL_KEY,
     *                               your_model_key_array,
     *                               MBEDTLS_OTP_MODEL_KEY_SIZE,
     *                               0);
     */
    ret = mbedtls_set_otp_info_ex(DEV_ROOT_KEY,
                                  otp_dev_root_key_data,
                                  MBEDTLS_OTP_DEVICE_RK_SIZE,
                                  0);
    if (ret != 0) {
        goto finish;
    }
    is_rootkey_init = true;
finish:
    return ret;
#else
    return 0;
#endif /* CE_LITE_OTP && CE_LITE_OTP_NORMAL && CE_LITE_OTP_WRITE &&
        * CE_CFG_HOST_ID == 0 */
}
