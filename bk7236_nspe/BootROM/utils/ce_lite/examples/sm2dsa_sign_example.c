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
#include "ce_lite_trng.h"
#include "ce_lite_config.h"
#include "mbedtls/ecp.h"
#include "mbedtls/md.h"
#include "mbedtls/sm3.h"
#include "mbedtls/sm2dsa.h"
#include "mbedtls/bignum.h"
#include "test_data_sm2dsa.h"

/**
 * There are some mbedtls sm2dsa sign examples for your reference.
 *
 * Example List:
 *  - Example 1: sm2dsa_sign_example.
 *               It shows you how to generate a raw signature of the
 *               SM3 message digest with SM2P256V1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_TRNG, \c CE_LITE_SM3,
 *               and \c CE_LITE_SM2DSA_SIGN in ce_lite_config.h.
 *
 *  - Example 2: sm2dsa_write_signature_example.
 *               It shows you how to generate a asn1 signature of the
 *               SM3 message digest with SM2P256V1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_TRNG, \c CE_LITE_SM3,
 *               and \c CE_LITE_SM2DSA_SIGN in ce_lite_config.h.
 *
 * Usage:
 *  - Call sm2dsa_sign_example_entry() function.
 *
 * Example Golden Data:
 *   Please refer to test_data_sm2dsa.h
 *
 * Note:
 *   Please make sure that mbedtls_platform_setup()
 *   have been called once before doing this example.
 */

static int rng_example(void *ctx, uint8_t *buf, size_t size)
{
    int ret = 0;

    ret = ce_trng_read(buf, size);
    if (ret != 0) {
        PAL_LOG_ERR("ce_trng_read FAILED(%X)\n", ret);
    }

    return ret;
}

static int32_t sm2dsa_sign(const uint8_t *sm2_d,
                           size_t sm2_d_size,
                           const uint8_t *sm2_qx,
                           size_t sm2_qx_size,
                           const uint8_t *sm2_qy,
                           size_t sm2_qy_size,
                           const uint8_t *message,
                           size_t message_size,
                           const char *id,
                           size_t id_size,
                           uint8_t *signature_r,
                           uint8_t *signature_s)
{
    int32_t ret = 0;
    uint8_t q_z = 1;
    uint8_t za[32] = {0};
    uint8_t hash[32] = {0};
    mbedtls_mpi d = {0};
    mbedtls_mpi r = {0};
    mbedtls_mpi s = {0};
    mbedtls_ecp_point Q = {0};
    mbedtls_ecp_group grp = {0};
    mbedtls_sm3_context sm3_ctx = {0};

    /* Init local variables */
    mbedtls_mpi_init(&d);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);
    mbedtls_ecp_point_init(&Q);
    mbedtls_ecp_group_init(&grp);
    mbedtls_sm3_init(&sm3_ctx);

    /* load sm2 group */
    ret = mbedtls_ecp_group_load(&grp, MBEDTLS_ECP_DP_SM2P256V1);
    if (ret != 0) {
        goto end;
    }
    /* load private key d */
    ret = mbedtls_mpi_read_binary(&d, sm2_d, sm2_d_size);
    if (ret != 0) {
        goto end;
    }
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

    /* generate ZA */
    ret = mbedtls_sm2_compute_id_digest(MBEDTLS_MD_SM3, &Q, id, id_size, za);
    if (ret != 0) {
        goto end;
    }
    /* compute final hash for sign */
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

    /* generate signature */
    ret = mbedtls_sm2dsa_sign(&r,
                              &s,
                              &d,
                              hash,
                              sizeof(hash),
                              rng_example,
                              NULL);
    if (0 == ret) {
        PAL_LOG_INFO("SM2DSA Sign PASS!\n");
    } else {
        PAL_LOG_INFO(
            "An error occurred during SM2DSA verify, return: -0x%x\n", -ret);
    }

    ret = mbedtls_mpi_write_binary(&r, signature_r, 32);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_mpi_write_binary(&s, signature_s, 32);
    if (ret != 0) {
        goto end;
    }

end:
    mbedtls_mpi_free(&d);
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_ecp_point_free(&Q);
    mbedtls_ecp_group_free(&grp);
    mbedtls_sm3_free(&sm3_ctx);
    return ret;
}

static int32_t sm2dsa_write_signature(const uint8_t *sm2_d,
                                      size_t sm2_d_size,
                                      const uint8_t *sm2_qx,
                                      size_t sm2_qx_size,
                                      const uint8_t *sm2_qy,
                                      size_t sm2_qy_size,
                                      const uint8_t *message,
                                      size_t message_size,
                                      const char *id,
                                      size_t id_size,
                                      uint8_t *signature_asn1,
                                      size_t *signature_asn1_size)
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

    /* load sm2 group */
    ret = mbedtls_ecp_group_load(&(sm2dsa_ctx.grp), MBEDTLS_ECP_DP_SM2P256V1);
    if (ret != 0) {
        goto end;
    }
    /* load private key d */
    ret = mbedtls_mpi_read_binary(&(sm2dsa_ctx.d), sm2_d, sm2_d_size);
    if (ret != 0) {
        goto end;
    }
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

    /* generate asn1 signature */
    ret = mbedtls_sm2dsa_write_signature(&sm2dsa_ctx,
                                         MBEDTLS_MD_SM3,
                                         hash,
                                         sizeof(hash),
                                         signature_asn1,
                                         signature_asn1_size,
                                         rng_example,
                                         NULL);
    if (0 == ret) {
        PAL_LOG_INFO("SM2DSA Write Signature PASS!\n");
    } else {
        PAL_LOG_INFO(
            "An error occurred during SM2DSA Write Signature, return: -0x%x\n", -ret);
    }

end:
    mbedtls_sm2dsa_free(&sm2dsa_ctx);
    mbedtls_sm3_free(&sm3_ctx);
    return ret;
}

int32_t sm2dsa_sign_example(void)
{
    int32_t ret = 0;
    uint8_t sign_r[32] = {0};
    uint8_t sign_s[32] = {0};

    PAL_LOG_INFO("SM2DSA Sign...\n");
    ret = sm2dsa_sign(sm2_d,
                      sizeof(sm2_d),
                      sm2_qx,
                      sizeof(sm2_qx),
                      sm2_qy,
                      sizeof(sm2_qy),
                      message_sm3,
                      sizeof(message_sm3),
                      sm2_id,
                      sm2_id_size,
                      sign_r,
                      sign_s);
    PAL_LOG_DUMP_BUF("SM2DSA signature r",
                     sign_r,
                     sizeof(sign_r));
    PAL_LOG_DUMP_BUF("SM2DSA signature s",
                     sign_s,
                     sizeof(sign_s));
    return ret;
}

int32_t sm2dsa_write_signature_example(void)
{
    int32_t ret = 0;
    size_t sign_size = 0;
    uint8_t sign_asn1[73] = {0};

    PAL_LOG_INFO("SM2DSA Write Signature...\n");
    ret = sm2dsa_write_signature(sm2_d,
                                 sizeof(sm2_d),
                                 sm2_qx,
                                 sizeof(sm2_qx),
                                 sm2_qy,
                                 sizeof(sm2_qy),
                                 message_sm3,
                                 sizeof(message_sm3),
                                 sm2_id,
                                 sm2_id_size,
                                 (uint8_t *)sign_asn1,
                                 &sign_size);
    PAL_LOG_DUMP_BUF("SM2DSA asn1 signature",
                     sign_asn1,
                     sign_size);
    return ret;
}

int32_t sm2dsa_sign_example_entry(void)
{
    int32_t ret = 0;

    ret = sm2dsa_sign_example();
    if (ret != 0) {
        return ret;
    }

    ret = sm2dsa_write_signature_example();
    return ret;
}