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
#include "ce_lite_trng.h"
#include "mbedtls/md.h"
#include "mbedtls/rsa.h"
#include "mbedtls/bignum.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "test_data_rsa.h"

/**
 * There are some mbedtls rsa sign examples for your reference.
 *
 * Example List:
 *  - Example 1: rsa_2048_sign_sha1_example.
 *               It shows you how to generate a asn1 signature of the
 *               SHA1 message digest with 2048 keybits.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_TRNG, \c CE_LITE_RSASSA_SIGN, \c CE_LITE_SHA1,
 *               and \c CE_LITE_RSA_PKCS_V15 or \c CE_LITE_RSA_PKCS_V21
 *               in ce_lite_config.h.
 *
 *  - Example 2: rsa_2048_sign_sha256_example.
 *               It shows you how to generate a asn1 signature of the
 *               SHA256 message digest with 2048 keybits.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_TRNG, \c CE_LITE_RSASSA_SIGN, \c CE_LITE_SHA256,
 *               and \c CE_LITE_RSA_PKCS_V15 or \c CE_LITE_RSA_PKCS_V21
 *               in ce_lite_config.h.
 *
 *  - Example 3: rsa_4096_sign_sha256_example.
 *               It shows you how to generate a asn1 signature of the
 *               SHA256 message digest with 4096 keybits.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_TRNG, \c CE_LITE_RSASSA_SIGN, \c CE_LITE_SHA256,
 *               and \c CE_LITE_RSA_PKCS_V15 or \c CE_LITE_RSA_PKCS_V21
 *               in ce_lite_config.h.
 *
 * Usage:
 *  - Call rsa_sign_example_entry() function.
 *
 * Example Golden Data:
 *   Please refer to test_data_rsa.h
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

static int32_t rsa_sign(const uint8_t *rsa_n,
                        size_t rsa_n_size,
                        const uint8_t *rsa_p,
                        size_t rsa_p_size,
                        const uint8_t *rsa_q,
                        size_t rsa_q_size,
                        const uint8_t *rsa_d,
                        size_t rsa_d_size,
                        const uint8_t *rsa_e,
                        size_t rsa_e_size,
                        const uint8_t *message,
                        size_t message_size,
                        uint8_t *signature,
                        int use_sha256,
                        int use_pkcs_v21)
{
    int32_t ret = 0;
    int32_t padding = 0;
    size_t key_size = 0;
    uint8_t hash[32] = {0};
    uint32_t hash_size = 0;
    mbedtls_rsa_context rsa_ctx = {0};
    mbedtls_md_type_t hash_id = MBEDTLS_MD_NONE;

    if (use_pkcs_v21) {
        padding = MBEDTLS_RSA_PKCS_V21;
    } else {
        padding = MBEDTLS_RSA_PKCS_V15;
    }

    if (use_sha256) {
        hash_id = MBEDTLS_MD_SHA256;
    } else {
        hash_id = MBEDTLS_MD_SHA1;
    }

    /* Init local variables */
    mbedtls_rsa_init(&rsa_ctx, padding, hash_id);

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

    /* import RSA key */
    ret = mbedtls_rsa_import_raw(&rsa_ctx,
                                 rsa_n,
                                 rsa_n_size,
                                 rsa_p,
                                 rsa_p_size,
                                 rsa_q,
                                 rsa_q_size,
                                 rsa_d,
                                 rsa_d_size,
                                 rsa_e,
                                 rsa_e_size);
    if (ret != 0) {
        goto end;
    }

    /* get key_size */
    key_size = mbedtls_rsa_get_len(&rsa_ctx);

    /* check rsa key */
    ret = mbedtls_rsa_check_privkey(&rsa_ctx);
    if (ret != 0) {
        goto end;
    }
    ret = mbedtls_rsa_check_pub_priv(&rsa_ctx, &rsa_ctx);
    if (ret != 0) {
        goto end;
    }

    /* Verify */
    ret = mbedtls_rsa_pkcs1_sign(&rsa_ctx,
                                 rng_example,
                                 NULL,
                                 MBEDTLS_RSA_PRIVATE,
                                 hash_id,
                                 hash_size,
                                 hash,
                                 signature);
    PAL_LOG_INFO("RSA %d %s %s sign result:\n", 8 * key_size,
                 use_pkcs_v21 ? "PKCS_V21" : "PKCS_V15",
                 use_sha256 ? "SHA256" : "SHA1");
    if (0 == ret) {
        PAL_LOG_INFO("           PASS!\n");
    } else if (MBEDTLS_ERR_RSA_VERIFY_FAILED == ret) {
        PAL_LOG_INFO("           FAILED!\n");
    } else {
        PAL_LOG_INFO("           An error occurred, return: -0x%x\n", -ret);
    }

end:
    mbedtls_rsa_free(&rsa_ctx);
    return ret;
}

int32_t rsa_2048_sign_sha1_example(void)
{
    int32_t ret = 0;
    uint8_t sign_2048_buf[256] = {0};

    PAL_LOG_INFO("RSA 2048 Sign with SHA1...\n");
    /* Sign RSA_2048 + PKCS_V15 + SHA1 */
    ret = rsa_sign(rsa_2048_modulus,
                   sizeof(rsa_2048_modulus),
                   rsa_2048_prime1,
                   sizeof(rsa_2048_prime1),
                   rsa_2048_prime2,
                   sizeof(rsa_2048_prime2),
                   rsa_2048_priv_exponent,
                   sizeof(rsa_2048_priv_exponent),
                   rsa_2048_pub_exponent,
                   sizeof(rsa_2048_pub_exponent),
                   message,
                   sizeof(message),
                   sign_2048_buf,
                   0,
                   0);
    if (ret != 0) {
        return ret;
    }
    PAL_LOG_DUMP_BUF("sign_2048_v15_sha1",
                     sign_2048_buf,
                     sizeof(sign_2048_buf));

    /* Sign RSA_2048 + PKCS_V21 + SHA1 */
    ret = rsa_sign(rsa_2048_modulus,
                   sizeof(rsa_2048_modulus),
                   rsa_2048_prime1,
                   sizeof(rsa_2048_prime1),
                   rsa_2048_prime2,
                   sizeof(rsa_2048_prime2),
                   rsa_2048_priv_exponent,
                   sizeof(rsa_2048_priv_exponent),
                   rsa_2048_pub_exponent,
                   sizeof(rsa_2048_pub_exponent),
                   message,
                   sizeof(message),
                   sign_2048_buf,
                   0,
                   1);
    if (ret != 0) {
        return ret;
    }
    PAL_LOG_DUMP_BUF("sign_2048_v21_sha1",
                     sign_2048_buf,
                     sizeof(sign_2048_buf));

    return ret;
}

int32_t rsa_2048_sign_sha256_example(void)
{
    int32_t ret = 0;
    uint8_t sign_2048_buf[256] = {0};

    PAL_LOG_INFO("RSA 2048 Sign with SHA256...\n");
    /* Sign RSA_2048 + PKCS_V15 + SHA256 */
    ret = rsa_sign(rsa_2048_modulus,
                   sizeof(rsa_2048_modulus),
                   rsa_2048_prime1,
                   sizeof(rsa_2048_prime1),
                   rsa_2048_prime2,
                   sizeof(rsa_2048_prime2),
                   rsa_2048_priv_exponent,
                   sizeof(rsa_2048_priv_exponent),
                   rsa_2048_pub_exponent,
                   sizeof(rsa_2048_pub_exponent),
                   message,
                   sizeof(message),
                   sign_2048_buf,
                   1,
                   0);
    if (ret != 0) {
        return ret;
    }
    PAL_LOG_DUMP_BUF("sign_2048_v15_sha256",
                     sign_2048_buf,
                     sizeof(sign_2048_buf));

    /* Sign RSA_2048 + PKCS_V21 + SHA256 */
    ret = rsa_sign(rsa_2048_modulus,
                   sizeof(rsa_2048_modulus),
                   rsa_2048_prime1,
                   sizeof(rsa_2048_prime1),
                   rsa_2048_prime2,
                   sizeof(rsa_2048_prime2),
                   rsa_2048_priv_exponent,
                   sizeof(rsa_2048_priv_exponent),
                   rsa_2048_pub_exponent,
                   sizeof(rsa_2048_pub_exponent),
                   message,
                   sizeof(message),
                   sign_2048_buf,
                   1,
                   1);
    if (ret != 0) {
        return ret;
    }
    PAL_LOG_DUMP_BUF("sign_2048_v21_sha256",
                     sign_2048_buf,
                     sizeof(sign_2048_buf));
    return ret;
}

int32_t rsa_4096_sign_sha256_example(void)
{
    int32_t ret = 0;
    uint8_t sign_4096_buf[512] = {0};

    PAL_LOG_INFO("RSA 4096 Sign with SHA256...\n");
    /* Sign RSA_4096 + PKCS_V15 + SHA256 */
    ret = rsa_sign(rsa_4096_modulus,
                   sizeof(rsa_4096_modulus),
                   rsa_4096_prime1,
                   sizeof(rsa_4096_prime1),
                   rsa_4096_prime2,
                   sizeof(rsa_4096_prime2),
                   rsa_4096_priv_exponent,
                   sizeof(rsa_4096_priv_exponent),
                   rsa_4096_pub_exponent,
                   sizeof(rsa_4096_pub_exponent),
                   message,
                   sizeof(message),
                   sign_4096_buf,
                   1,
                   0);
    if (ret != 0) {
        return ret;
    }
    PAL_LOG_DUMP_BUF("sign_4096_v15_sha256",
                     sign_4096_buf,
                     sizeof(sign_4096_buf));

    /* Sign RSA_4096 + PKCS_V21 + SHA256 */
    ret = rsa_sign(rsa_4096_modulus,
                   sizeof(rsa_4096_modulus),
                   rsa_4096_prime1,
                   sizeof(rsa_4096_prime1),
                   rsa_4096_prime2,
                   sizeof(rsa_4096_prime2),
                   rsa_4096_priv_exponent,
                   sizeof(rsa_4096_priv_exponent),
                   rsa_4096_pub_exponent,
                   sizeof(rsa_4096_pub_exponent),
                   message,
                   sizeof(message),
                   sign_4096_buf,
                   1,
                   1);
    if (ret != 0) {
        return ret;
    }
    PAL_LOG_DUMP_BUF("sign_4096_v21_sha256",
                     sign_4096_buf,
                     sizeof(sign_4096_buf));
    return ret;
}


int32_t rsa_sign_example_entry(void)
{
    int32_t ret = 0;

    /* RSA 2048 example */
    ret = rsa_2048_sign_sha1_example();
    if (ret != 0) {
        return ret;
    }

    ret = rsa_2048_sign_sha256_example();
    if (ret != 0) {
        return ret;
    }

    /* RSA 4096 example */
    ret = rsa_4096_sign_sha256_example();
    return ret;
}
