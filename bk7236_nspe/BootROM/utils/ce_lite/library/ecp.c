/*
 * Copyright (c) 2021, Arm Technology (China) Co., Ltd.
 * All rights reserved.
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#include "ce_lite_bn.h"
#include "ce_lite_ecp.h"
#include "ce_lite_common.h"
#include "ce_lite_config.h"
#include "mbedtls/bignum.h"
#include "mbedtls/ecp.h"
#include "mbedtls/platform.h"

#if defined(CE_LITE_ECP)
int ce_err_to_mbedtls_ecp_err(int errno)
{
    switch (errno) {
        case CE_SUCCESS:
            errno = 0;
            break;
        case CE_ERROR_BAD_PARAMS:
        case CE_ERROR_BAD_INPUT_DATA:
        case CE_ERROR_NO_DATA:
            errno = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
            break;
        case CE_ERROR_SHORT_BUFFER:
            errno = MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
            break;
        case CE_ERROR_FEATURE_UNAVAIL:
            errno = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
            break;
        case CE_ERROR_VERIFY_SIG:
            errno = MBEDTLS_ERR_ECP_VERIFY_FAILED;
            break;
        case CE_ERROR_OOM:
            errno = MBEDTLS_ERR_ECP_ALLOC_FAILED;
            break;
        case CE_ERROR_GEN_RANDOM:
            errno = MBEDTLS_ERR_ECP_RANDOM_FAILED;
            break;
        case CE_ERROR_INVAL_KEY:
            errno = MBEDTLS_ERR_ECP_INVALID_KEY;
            break;
        default:
            errno = MBEDTLS_ERR_ECP_HW_FAILED;
            break;
    }
    return errno;
}

void mbedtls_ecp_point_init(mbedtls_ecp_point *pt)
{
    int ret = CE_SUCCESS;

    (void)ret;
    PAL_ASSERT(sizeof(ce_ecp_point_t) ==
                sizeof(mbedtls_ecp_point));

    ret = ce_ecp_point_init((ce_ecp_point_t *)pt);
    PAL_ASSERT(CE_SUCCESS == ret);
}

void mbedtls_ecp_group_init(mbedtls_ecp_group *grp)
{
    int ret = CE_SUCCESS;

    (void)ret;
    PAL_ASSERT(sizeof(ce_ecp_group_t) ==
                sizeof(mbedtls_ecp_group));

    ret = ce_ecp_group_init((ce_ecp_group_t *)grp);
    PAL_ASSERT(CE_SUCCESS == ret);
}

void mbedtls_ecp_keypair_init(mbedtls_ecp_keypair *key)
{
    int ret = CE_SUCCESS;

    if (NULL == key) {
        return;
    }

    (void)ret;
    PAL_ASSERT(sizeof(ce_ecp_keypair_t) ==
                sizeof(mbedtls_ecp_keypair));

    ret = ce_ecp_keypair_init((ce_ecp_keypair_t *)key);
    PAL_ASSERT(CE_SUCCESS == ret);
}

void mbedtls_ecp_point_free(mbedtls_ecp_point *pt)
{
    ce_ecp_point_free((ce_ecp_point_t *)pt);
}

void mbedtls_ecp_group_free(mbedtls_ecp_group *grp)
{
    ce_ecp_group_free((ce_ecp_group_t *)grp);
}

void mbedtls_ecp_keypair_free(mbedtls_ecp_keypair *key)
{
    if (NULL == key) {
        return;
    }

    ce_ecp_keypair_free((ce_ecp_keypair_t *)key);
}

int mbedtls_ecp_group_load( mbedtls_ecp_group *grp, mbedtls_ecp_group_id id )
{
    return ce_err_to_mbedtls_ecp_err(
            ce_ecp_group_load((ce_ecp_group_t *)grp,
                              (ce_ecp_group_id_t)id));
}

int mbedtls_ecp_check_pubkey(const mbedtls_ecp_group *grp,
                             const mbedtls_ecp_point *pt)
{
    return ce_err_to_mbedtls_ecp_err(
            ce_ecp_check_pubkey((ce_ecp_group_t *)grp,
                                (ce_ecp_point_t *)pt));
}

#if defined(CE_LITE_SM2DSA_SIGN) || defined(CE_LITE_SM2DSA_SIGN)
int mbedtls_ecp_check_privkey(const mbedtls_ecp_group *grp,
                              const mbedtls_mpi *d)
{
    return ce_err_to_mbedtls_ecp_err(
            ce_ecp_check_privkey((const ce_ecp_group_t *)grp,
                                 MPI2BN(d)));
}

int mbedtls_ecp_check_pub_priv(const mbedtls_ecp_keypair *pub,
                               const mbedtls_ecp_keypair *prv)
{
    return ce_err_to_mbedtls_ecp_err(
            ce_ecp_check_pub_priv((const ce_ecp_keypair_t *)pub,
                                  (const ce_ecp_keypair_t *)prv));
}
#endif /* CE_LITE_SM2DSA_SIGN || CE_LITE_SM2DSA_SIGN */
#endif /* CE_LITE_ECP */