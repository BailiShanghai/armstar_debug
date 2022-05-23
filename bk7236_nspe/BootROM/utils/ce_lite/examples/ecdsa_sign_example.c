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
#include "mbedtls/md.h"
#include "mbedtls/ecp.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/bignum.h"
#include "test_data_ecdsa.h"

/**
 * There are some mbedtls ecdsa sign examples for your reference.
 *
 * Example List:
 *  - Example 1: ecdsa_sign_P256_SHA1_example.
 *               It shows you how to generate a raw signature of the
 *               SHA1 message digest with SECP256R1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_TRNG, \c CE_LITE_ECDSA_SIGN, \c CE_LITE_ECP_DP_SECP256R1
 *               and \c CE_LITE_SHA1 in ce_lite_config.h.
 *
 *  - Example 2: ecdsa_write_signature_P256_SHA1_example.
 *               It shows you how to generate a asn1 signature of the
 *               SHA1 message digest with SECP256R1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_TRNG, \c CE_LITE_ECDSA_SIGN, \c CE_LITE_ECP_DP_SECP256R1
 *               and \c CE_LITE_SHA1 in ce_lite_config.h.
 *
 *  - Example 3: ecdsa_sign_P521_SHA256_example.
 *               It shows you how to generate a raw signature of the
 *               SHA256 message digest with SECP521R1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_TRNG, \c CE_LITE_ECDSA_SIGN, \c CE_LITE_ECP_DP_SECP521R1
 *               and \c CE_LITE_SHA256 in ce_lite_config.h.
 *
 *  - Example 4: ecdsa_write_signature_P521_SHA256_example.
 *               It shows you how to generate a asn1 signature of the
 *               SHA256 message digest with SECP521R1 curve.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_TRNG, \c CE_LITE_ECDSA_SIGN, \c CE_LITE_ECP_DP_SECP521R1
 *               and \c CE_LITE_SHA256 in ce_lite_config.h.
 *
 * Usage:
 *  - Call ecdsa_sign_example_entry() function.
 *
 * Example Golden Data:
 *   Please refer to test_data_ecdsa.h
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

static int32_t ecdsa_sign(const uint8_t *ec_d,
                          size_t ec_d_size,
                          const uint8_t *message,
                          size_t message_size,
                          uint8_t *signature_r,
                          size_t signature_r_size,
                          uint8_t *signature_s,
                          size_t signature_s_size,
                          int use_secp256r1,
                          int use_sha256)
{
    int32_t ret = 0;
    uint8_t hash[32] = {0};
    uint32_t hash_size = 0;
    mbedtls_mpi d = {0};
    mbedtls_mpi r = {0};
    mbedtls_mpi s = {0};
    mbedtls_ecp_group grp = {0};

    /* Init local variables */
    mbedtls_mpi_init(&d);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);
    mbedtls_ecp_group_init(&grp);

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
    /* load private key d */
    ret = mbedtls_mpi_read_binary(&d, ec_d, ec_d_size);
    if (ret != 0) {
        goto end;
    }

    /* check priv key */
    ret = mbedtls_ecp_check_privkey(&grp, &d);
    if (ret != 0) {
        goto end;
    }

    /* generate signature */
    ret = mbedtls_ecdsa_sign(&grp,
                             &r,
                             &s,
                             &d,
                             hash,
                             hash_size,
                             rng_example,
                             NULL);
    PAL_LOG_INFO("ECDSA %s %s Sign result:\n",
                 use_secp256r1 ? "P256" : "P521",
                 use_sha256 ? "SHA256" : "SHA1");
    if (0 == ret) {
        PAL_LOG_INFO("           PASS!\n");
    } else {
        PAL_LOG_INFO(
            "An error occurred during ECDSA sign, return: -0x%x\n", -ret);
    }

    ret = mbedtls_mpi_write_binary(&r, signature_r, signature_r_size);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_mpi_write_binary(&s, signature_s, signature_s_size);
    if (ret != 0) {
        goto end;
    }

end:
    mbedtls_mpi_free(&d);
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_ecp_group_free(&grp);
    return ret;
}

static int32_t ecdsa_write_signature(const uint8_t *ec_d,
                                     size_t ec_d_size,
                                     const uint8_t *message,
                                     size_t message_size,
                                     uint8_t *signature_asn1,
                                     size_t *signature_asn1_size,
                                     int use_secp256r1,
                                     int use_sha256)
{
    int32_t ret = 0;
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
    /* load private key d */
    ret = mbedtls_mpi_read_binary(&(ecdsa_ctx.d), ec_d, ec_d_size);
    if (ret != 0) {
        goto end;
    }

    /* check priv key */
    ret = mbedtls_ecp_check_privkey(&(ecdsa_ctx.grp), &(ecdsa_ctx.d));
    if (ret != 0) {
        goto end;
    }

    /* write signature */
    ret = mbedtls_ecdsa_write_signature(&ecdsa_ctx,
                                        MBEDTLS_MD_NONE,
                                        hash,
                                        hash_size,
                                        signature_asn1,
                                        signature_asn1_size,
                                        rng_example,
                                        NULL);
    PAL_LOG_INFO("ECDSA %s %s write signature result:\n",
                 use_secp256r1 ? "P256" : "P521",
                 use_sha256 ? "SHA256" : "SHA1");
    if (0 == ret) {
        PAL_LOG_INFO("           PASS!\n");
    } else {
        PAL_LOG_INFO(
            "An error occurred during ECDSA Write Signature, return: -0x%x\n", -ret);
    }

end:
    mbedtls_ecdsa_free(&ecdsa_ctx);
    return ret;
}

int32_t ecdsa_sign_P256_SHA1_example(void)
{
    int32_t ret = 0;
    uint8_t sign_r_p256[32] = {0};
    uint8_t sign_s_p256[32] = {0};

    PAL_LOG_INFO("ECDSA Sign with SECP256R1 and SHA1...\n");
    ret = ecdsa_sign(ec_d_p256,
                     sizeof(ec_d_p256),
                     ecdsa_message,
                     sizeof(ecdsa_message),
                     sign_r_p256,
                     sizeof(sign_r_p256),
                     sign_s_p256,
                     sizeof(sign_s_p256),
                     1,
                     0);
    PAL_LOG_DUMP_BUF("P256 wih SHA1 signature r",
                     sign_r_p256,
                     sizeof(sign_r_p256));
    PAL_LOG_DUMP_BUF("P256 with SHA1 signature s",
                     sign_s_p256,
                     sizeof(sign_s_p256));
    return ret;
}

int32_t ecdsa_write_signature_P256_SHA1_example(void)
{
    int32_t ret = 0;
    size_t sign_size = 0;
    uint8_t sign_asn1_p256[73] = {0};

    PAL_LOG_INFO("ECDSA Write Signature with SECP256R1 and SHA1...\n");
    ret = ecdsa_write_signature(ec_d_p256,
                                sizeof(ec_d_p256),
                                ecdsa_message,
                                sizeof(ecdsa_message),
                                sign_asn1_p256,
                                &sign_size,
                                1,
                                0);
    PAL_LOG_DUMP_BUF("P256 with SHA1 asn1 signature",
                     sign_asn1_p256,
                     sign_size);
    return ret;
}

int32_t ecdsa_sign_P521_SHA256_example(void)
{
    int32_t ret = 0;
    uint8_t sign_r_p521[66] = {0};
    uint8_t sign_s_p521[66] = {0};

    PAL_LOG_INFO("ECDSA Sign with SECP521R1 and SHA256...\n");
    ret = ecdsa_sign(ec_d_p521,
                     sizeof(ec_d_p521),
                     ecdsa_message,
                     sizeof(ecdsa_message),
                     sign_r_p521,
                     sizeof(sign_r_p521),
                     sign_s_p521,
                     sizeof(sign_s_p521),
                     0,
                     1);
    PAL_LOG_DUMP_BUF("P521 with SHA256 signature r",
                     sign_r_p521,
                     sizeof(sign_r_p521));
    PAL_LOG_DUMP_BUF("P521 with SHA256 signature s",
                     sign_s_p521,
                     sizeof(sign_s_p521));
    return ret;
}

int32_t ecdsa_write_signature_P521_SHA256_example(void)
{
    int32_t ret = 0;
    size_t sign_size = 0;
    uint8_t sign_asn1_p521[141] = {0};

    PAL_LOG_INFO("ECDSA Write Signature with SECP521R1 and SHA256...\n");
    ret = ecdsa_write_signature(ec_d_p521,
                                sizeof(ec_d_p521),
                                ecdsa_message,
                                sizeof(ecdsa_message),
                                sign_asn1_p521,
                                &sign_size,
                                0,
                                1);
    PAL_LOG_DUMP_BUF("P521 with SHA256 asn1 signature",
                     sign_asn1_p521,
                     sign_size);
    return ret;
}

int32_t ecdsa_sign_example_entry(void)
{
    int32_t ret = 0;

    /* P256 with SHA1 */
    ret = ecdsa_sign_P256_SHA1_example();
    if (ret != 0) {
        return ret;
    }

    ret = ecdsa_write_signature_P256_SHA1_example();
    if (ret != 0) {
        return ret;
    }

    /* P521 with SHA256 */
    ret = ecdsa_sign_P521_SHA256_example();
    if (ret != 0) {
        return ret;
    }

    ret = ecdsa_write_signature_P521_SHA256_example();
    return ret;
}
