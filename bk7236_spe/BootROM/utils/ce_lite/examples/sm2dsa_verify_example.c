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
#include "mbedtls/ecp.h"
#include "mbedtls/md.h"
#include "mbedtls/sm3.h"
#include "mbedtls/sm2dsa.h"
#include "mbedtls/bignum.h"
#include "test_data_sm2dsa.h"

/**
 * There are some mbedtls sm2dsa verify examples for your reference.
 *
 * Example List:
 *  - Example 1: sm2dsa_verify_example.
 *               It shows you how to verify a raw signature of the
 *               SM3 message digest with SM2P256V1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_SM3 and \c CE_LITE_SM2DSA in ce_lite_config.h.
 *
 *  - Example 2: sm2dsa_read_signature_example.
 *               It shows you how to verify a asn1 signature of the
 *               SM3 message digest with SM2P256V1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_SM3 and \c CE_LITE_SM2DSA in ce_lite_config.h.
 *
 * Usage:
 *  - Call sm2dsa_verify_example_entry() function.
 *
 * Example Golden Data:
 *   Please refer to test_data_sm2dsa.h
 *
 * Note:
 *   Please make sure that mbedtls_platform_setup()
 *   have been called once before doing this example.
 */

static int32_t sm2dsa_verify(const uint8_t *sm2_qx,
                             size_t sm2_qx_size,
                             const uint8_t *sm2_qy,
                             size_t sm2_qy_size,
                             const uint8_t *message,
                             size_t message_size,
                             const char *id,
                             size_t id_size,
                             const uint8_t *signature_r,
                             size_t signature_r_size,
                             const uint8_t *signature_s,
                             size_t signature_s_size)
{
    int32_t ret = 0;
    uint8_t q_z = 1;
    uint8_t za[32] = {0};
    uint8_t hash[32] = {0};
    mbedtls_mpi r = {0};
    mbedtls_mpi s = {0};
    mbedtls_ecp_point Q = {0};
    mbedtls_sm3_context sm3_ctx = {0};

    /* Init local variables */
    mbedtls_ecp_point_init(&Q);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);
    mbedtls_sm3_init(&sm3_ctx);

    /* load point Q */
    ret = mbedtls_mpi_read_binary(&(Q.X), sm2_qx, sm2_qx_size);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_mpi_read_binary(&(Q.Y), sm2_qy, sm2_qy_size);
    if (ret != 0) {
        goto end;
    }
    /* Init Z value for Q. Always set to 1 */
    ret = mbedtls_mpi_read_binary(&(Q.Z), &q_z, 1);
    if (ret != 0) {
        goto end;
    }

    /* load signature r and s */
    ret = mbedtls_mpi_read_binary(&r, signature_r, signature_r_size);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_mpi_read_binary(&s, signature_s, signature_s_size);
    if (ret != 0) {
        goto end;
    }

    /* generate ZA */
    ret = mbedtls_sm2_compute_id_digest(MBEDTLS_MD_SM3, &Q, id, id_size, za);
    if (ret != 0) {
        goto end;
    }

    /* compute final hash for verification */
    ret = mbedtls_sm3_starts_ret(&sm3_ctx);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_sm3_update_ret(&sm3_ctx, za, sizeof(za));
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_sm3_update_ret(&sm3_ctx, message, message_size);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_sm3_finish_ret(&sm3_ctx, hash);
    if (ret != 0) {
        goto end;
    }

    /* verify signature */
    ret = mbedtls_sm2dsa_verify(hash,
                                sizeof(hash),
                                &Q,
                                &r,
                                &s);
    if (0 == ret) {
        PAL_LOG_INFO("SM2DSA Verify PASS!\n");
    } else if (MBEDTLS_ERR_ECP_VERIFY_FAILED == ret) {
        PAL_LOG_INFO("SM2DSA Verify FAILED!\n");
    } else {
        PAL_LOG_INFO(
            "An error occurred during SM2DSA verify, return: -0x%x\n", -ret);
    }

end:
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_ecp_point_free(&Q);
    mbedtls_sm3_free(&sm3_ctx);
    return ret;
}

static int32_t sm2dsa_read_signature(const uint8_t *sm2_qx,
                                     size_t sm2_qx_size,
                                     const uint8_t *sm2_qy,
                                     size_t sm2_qy_size,
                                     const uint8_t *message,
                                     size_t message_size,
                                     const char *id,
                                     size_t id_size,
                                     const uint8_t *signature_asn1,
                                     size_t signature_asn1_size)
{
    int32_t ret = 0;
    uint8_t q_z = 1;
    uint8_t za[32] = {0};
    uint8_t hash[32] = {0};
    mbedtls_sm3_context sm3_ctx = {0};
    mbedtls_sm2dsa_context sm2dsa_ctx = {0};

    /* Init local variables */
    mbedtls_sm2dsa_init(&sm2dsa_ctx);
    mbedtls_sm3_init(&sm3_ctx);

    /* load point Q */
    ret = mbedtls_mpi_read_binary(&(sm2dsa_ctx.Q.X), sm2_qx, sm2_qx_size);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_mpi_read_binary(&(sm2dsa_ctx.Q.Y), sm2_qy, sm2_qy_size);
    if (ret != 0) {
        goto end;
    }
    /* Init Z value for Q. Always set to 1 */
    ret = mbedtls_mpi_read_binary(&(sm2dsa_ctx.Q.Z), &q_z, 1);
    if (ret != 0) {
        goto end;
    }

    /* generate ZA */
    ret = mbedtls_sm2_compute_id_digest(MBEDTLS_MD_SM3, &(sm2dsa_ctx.Q), id, id_size, za);
    if (ret != 0) {
        goto end;
    }

    /* compute final hash for verification */
    ret = mbedtls_sm3_starts_ret(&sm3_ctx);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_sm3_update_ret(&sm3_ctx, za, sizeof(za));
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_sm3_update_ret(&sm3_ctx, message, message_size);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_sm3_finish_ret(&sm3_ctx, hash);
    if (ret != 0) {
        goto end;
    }

    /* verify signature */
    ret = mbedtls_sm2dsa_read_signature(&sm2dsa_ctx,
                                        hash,
                                        sizeof(hash),
                                        signature_asn1,
                                        signature_asn1_size);
    if (0 == ret) {
        PAL_LOG_INFO("SM2DSA Read Signature PASS!\n");
    } else if (MBEDTLS_ERR_ECP_VERIFY_FAILED == ret) {
        PAL_LOG_INFO("SM2DSA Read Signature FAILED!\n");
    } else {
        PAL_LOG_INFO(
            "An error occurred during SM2DSA Read Signature, return: -0x%x\n", -ret);
    }

end:
    mbedtls_sm2dsa_free(&sm2dsa_ctx);
    mbedtls_sm3_free(&sm3_ctx);
    return ret;
}

int32_t sm2dsa_verify_example(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("SM2DSA Verify...\n");
    ret = sm2dsa_verify(sm2_qx,
                        sizeof(sm2_qx),
                        sm2_qy,
                        sizeof(sm2_qy),
                        message_sm3,
                        sizeof(message_sm3),
                        sm2_id,
                        sm2_id_size,
                        signature_r,
                        sizeof(signature_r),
                        signature_s,
                        sizeof(signature_s));
    return ret;
}

int32_t sm2dsa_read_signature_example(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("SM2DSA Read Signature...\n");
    ret = sm2dsa_read_signature(sm2_qx,
                                sizeof(sm2_qx),
                                sm2_qy,
                                sizeof(sm2_qy),
                                message_sm3,
                                sizeof(message_sm3),
                                sm2_id,
                                sm2_id_size,
                                signature_asn1,
                                sizeof(signature_asn1));
    return ret;
}

int32_t sm2dsa_verify_example_entry(void)
{
    int32_t ret = 0;

    ret = sm2dsa_verify_example();
    if (ret != 0) {
        return ret;
    }

    ret = sm2dsa_read_signature_example();
    return ret;
}