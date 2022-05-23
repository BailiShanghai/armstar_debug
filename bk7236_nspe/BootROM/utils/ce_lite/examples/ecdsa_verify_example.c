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
#include "mbedtls/md.h"
#include "mbedtls/ecp.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/bignum.h"
#include "test_data_ecdsa.h"

/**
 * There are some mbedtls ecdsa verify examples for your reference.
 *
 * Example List:
 *  - Example 1: ecdsa_verify_P256_SHA1_example.
 *               It shows you how to verify a raw signature of the
 *               SHA1 message digest with SECP256R1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_ECDSA, \c CE_LITE_ECP_DP_SECP256R1
 *               and \c CE_LITE_SHA1 in ce_lite_config.h.
 *
 *  - Example 2: ecdsa_read_signature_P256_SHA1_example.
 *               It shows you how to verify a asn1 signature of the
 *               SHA1 message digest with SECP256R1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_ECDSA, \c CE_LITE_ECP_DP_SECP256R1
 *               and \c CE_LITE_SHA1 in ce_lite_config.h.
 *
 *  - Example 3: ecdsa_verify_P521_SHA256_example.
 *               It shows you how to verify a raw signature of the
 *               SHA256 message digest with SECP521R1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_ECDSA, \c CE_LITE_ECP_DP_SECP521R1
 *               and \c CE_LITE_SHA256 in ce_lite_config.h.
 *
 *  - Example 4: ecdsa_read_signature_P521_SHA256_example.
 *               It shows you how to verify a asn1 signature of the
 *               SHA256 message digest with SECP521R1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_ECDSA, \c CE_LITE_ECP_DP_SECP521R1
 *               and \c CE_LITE_SHA256 in ce_lite_config.h.
 *
 * Usage:
 *  - Call ecdsa_verify_example_entry() function.
 *
 * Example Golden Data:
 *   Please refer to test_data_ecdsa.h
 *
 * Note:
 *   Please make sure that mbedtls_platform_setup()
 *   have been called once before doing this example.
 */

static int32_t ecdsa_verify(const uint8_t *ec_qx,
                            size_t ec_qx_size,
                            const uint8_t *ec_qy,
                            size_t ec_qy_size,
                            const uint8_t *message,
                            size_t message_size,
                            const uint8_t *signature_r,
                            size_t signature_r_size,
                            const uint8_t *signature_s,
                            size_t signature_s_size,
                            int use_secp256r1,
                            int use_sha256)
{
    int32_t ret = 0;
    uint8_t q_z = 1;
    uint8_t hash[32] = {0};
    uint32_t hash_size = 0;
    mbedtls_mpi r = {0};
    mbedtls_mpi s = {0};
    mbedtls_ecp_point Q = {0};
    mbedtls_ecp_group grp = {0};

    /* Init local variables */
    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_point_init(&Q);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);

    /* Calculate message's hash */
    if (use_sha256) {
        ret = mbedtls_sha256_ret(message, message_size, hash, 0);
        if (ret != 0) {
            PAL_LOG_ERR("Calculate message hash with SHA256 failed!\n");
            goto end;
        }
        hash_size = 32;
    } else {
        ret = mbedtls_sha1_ret(message, message_size, hash);
        if (ret != 0) {
            PAL_LOG_ERR("Calculate message hash with SHA1 failed!\n");
            goto end;
        }
        hash_size = 20;
    }

    /* load ec group */
    if (use_secp256r1) {
        mbedtls_ecp_group_load(&grp, MBEDTLS_ECP_DP_SECP256R1);
    } else {
        mbedtls_ecp_group_load(&grp, MBEDTLS_ECP_DP_SECP521R1);
    }
    /* load point Q */
    ret = mbedtls_mpi_read_binary(&(Q.X), ec_qx, ec_qx_size);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_mpi_read_binary(&(Q.Y), ec_qy, ec_qy_size);
    if (ret != 0) {
        goto end;
    }
    /* init Z value for Q. always set to 1 */
    ret = mbedtls_mpi_read_binary(&(Q.Z), &q_z, 1);
    if (ret != 0) {
        goto end;
    }

    /* check pubkey */
    ret = mbedtls_ecp_check_pubkey(&grp, &Q);
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

    /* verify signature */
    ret = mbedtls_ecdsa_verify(&grp,
                               hash,
                               hash_size,
                               &Q,
                               &r,
                               &s);
    PAL_LOG_INFO("ECDSA %s %s verify result:\n",
                    use_secp256r1 ? "P256" : "P521",
                    use_sha256 ? "SHA256" : "SHA1");
    if (0 == ret) {
        PAL_LOG_INFO("           PASS!\n");
    } else if (MBEDTLS_ERR_ECP_VERIFY_FAILED == ret) {
        PAL_LOG_INFO("           FAILED!\n");
    } else {
        PAL_LOG_INFO(
            "An error occurred during ECDSA verify, return: -0x%x\n", -ret);
    }

end:
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_ecp_point_free(&Q);
    mbedtls_ecp_group_free(&grp);
    return ret;
}

static int32_t ecdsa_read_signature(const uint8_t *ec_qx,
                                    size_t ec_qx_size,
                                    const uint8_t *ec_qy,
                                    size_t ec_qy_size,
                                    const uint8_t *message,
                                    size_t message_size,
                                    const uint8_t *signature_asn1,
                                    size_t signature_asn1_size,
                                    int use_secp256r1,
                                    int use_sha256)
{
    int32_t ret = 0;
    uint8_t q_z = 1;
    uint8_t hash[32] = {0};
    uint32_t hash_size = 0;
    mbedtls_ecdsa_context ecdsa_ctx = {0};

    /* Init local variables */
    mbedtls_ecdsa_init(&ecdsa_ctx);

    /* Calculate message's hash */
    if (use_sha256) {
        ret = mbedtls_sha256_ret(message, message_size, hash, 0);
        if (ret != 0) {
            PAL_LOG_ERR("Calculate message hash with SHA256 failed!\n");
            goto end;
        }
        hash_size = 32;
    } else {
        ret = mbedtls_sha1_ret(message, message_size, hash);
        if (ret != 0) {
            PAL_LOG_ERR("Calculate message hash with SHA1 failed!\n");
            goto end;
        }
        hash_size = 20;
    }

    /* load ec group */
    if (use_secp256r1) {
        mbedtls_ecp_group_load(&(ecdsa_ctx.grp), MBEDTLS_ECP_DP_SECP256R1);
    } else {
        mbedtls_ecp_group_load(&(ecdsa_ctx.grp), MBEDTLS_ECP_DP_SECP521R1);
    }
    /* load point Q */
    ret = mbedtls_mpi_read_binary(&(ecdsa_ctx.Q.X), ec_qx, ec_qx_size);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_mpi_read_binary(&(ecdsa_ctx.Q.Y), ec_qy, ec_qy_size);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_mpi_read_binary(&(ecdsa_ctx.Q.Z), &q_z, 1);
    if (ret != 0) {
        goto end;
    }

    /* check pubkey */
    ret = mbedtls_ecp_check_pubkey(&(ecdsa_ctx.grp), &(ecdsa_ctx.Q));
    if (ret != 0) {
        goto end;
    }

    /* verify signature */
    ret = mbedtls_ecdsa_read_signature(&ecdsa_ctx,
                                       hash,
                                       hash_size,
                                       signature_asn1,
                                       signature_asn1_size);
    PAL_LOG_INFO("ECDSA %s %s read signature result:\n",
                    use_secp256r1 ? "P256" : "P521",
                    use_sha256 ? "SHA256" : "SHA1");
    if (0 == ret) {
        PAL_LOG_INFO("           PASS!\n");
    } else if (MBEDTLS_ERR_ECP_VERIFY_FAILED == ret) {
        PAL_LOG_INFO("           FAILED!\n");
    } else {
        PAL_LOG_INFO(
            "An error occurred during ECDSA verify, return: -0x%x\n", -ret);
    }

end:
    mbedtls_ecdsa_free(&ecdsa_ctx);
    return ret;
}

int32_t ecdsa_verify_P256_SHA1_example(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("ECDSA Verify with SECP256R1 and SHA1...\n");
    ret = ecdsa_verify(ec_qx_p256,
                       sizeof(ec_qx_p256),
                       ec_qy_p256,
                       sizeof(ec_qy_p256),
                       ecdsa_message,
                       sizeof(ecdsa_message),
                       signature_r_p256_sha1,
                       sizeof(signature_r_p256_sha1),
                       signature_s_p256_sha1,
                       sizeof(signature_s_p256_sha1),
                       1,
                       0);
    return ret;
}

int32_t ecdsa_read_signature_P256_SHA1_example(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("ECDSA Read Signature with SECP256R1 and SHA1...\n");
    ret = ecdsa_read_signature(ec_qx_p256,
                               sizeof(ec_qx_p256),
                               ec_qy_p256,
                               sizeof(ec_qy_p256),
                               ecdsa_message,
                               sizeof(ecdsa_message),
                               signature_asn1_p256_sha1,
                               sizeof(signature_asn1_p256_sha1),
                               1,
                               0);
    return ret;
}

int32_t ecdsa_verify_P521_SHA256_example(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("ECDSA Verify with SECP521R1 and SHA256...\n");
    ret = ecdsa_verify(ec_qx_p521,
                       sizeof(ec_qx_p521),
                       ec_qy_p521,
                       sizeof(ec_qy_p521),
                       ecdsa_message,
                       sizeof(ecdsa_message),
                       signature_r_p521_sha256,
                       sizeof(signature_r_p521_sha256),
                       signature_s_p521_sha256,
                       sizeof(signature_s_p521_sha256),
                       0,
                       1);
    return ret;
}

int32_t ecdsa_read_signature_P521_SHA256_example(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("ECDSA Read Signature with SECP521R1 and SHA256...\n");
    ret = ecdsa_read_signature(ec_qx_p521,
                               sizeof(ec_qx_p521),
                               ec_qy_p521,
                               sizeof(ec_qy_p521),
                               ecdsa_message,
                               sizeof(ecdsa_message),
                               signature_asn1_p521_sha256,
                               sizeof(signature_asn1_p521_sha256),
                               0,
                               1);
    return ret;
}

int32_t ecdsa_verify_example_entry(void)
{
    int32_t ret = 0;

    /* P256 with SHA1 */
    ret = ecdsa_verify_P256_SHA1_example();
    if (ret != 0) {
        return ret;
    }

    ret = ecdsa_read_signature_P256_SHA1_example();
    if (ret != 0) {
        return ret;
    }

    /* P521 with SHA256 */
    ret = ecdsa_verify_P521_SHA256_example();
    if (ret != 0) {
        return ret;
    }

    ret = ecdsa_read_signature_P521_SHA256_example();
    return ret;
}