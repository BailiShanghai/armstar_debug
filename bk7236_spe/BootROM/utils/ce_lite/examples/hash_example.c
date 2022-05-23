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
#include "pal_string.h"
#include "ce_lite_config.h"
#include "mbedtls/md.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sm3.h"
#include "test_data_hash.h"

/**
 * There are some mbedtls hash examples for your reference.
 *
 * Example List:
 *  - Example 1: sha1_example.
 *               It shows you how to generate a SHA1 message digest.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_SHA1 in ce_lite_config.h.
 *
 *  - Example 2: sha224_example.
 *               It shows you how to generate a SHA224 message digest.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_SHA224 in ce_lite_config.h.
 *
 *  - Example 3: sha256_example.
 *               It shows you how to generate a SHA256 message digest.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_SHA256 in ce_lite_config.h.
 *
 *  - Example 4: sm3_example.
 *               It shows you how to generate a SM3 message digest.
 *               To enable this example, you must define the macro
 *               \c CE_LITE_SM3 in ce_lite_config.h.
 *
 * Usage:
 *  - Call hash_example_entry() function.
 *
 * Example Golden Data:
 *   Please refer to test_data_hash.h
 *
 * Note:
 *   Please make sure that mbedtls_platform_setup()
 *   have been called once before doing this example.
 */

static int32_t sha1_verify(const uint8_t *message,
                           size_t message_size,
                           const uint8_t *ref_hash)
{
    int32_t ret = 0;
    uint8_t hash[20] = {0};
    mbedtls_sha1_context sha1_ctx = {0};

    mbedtls_sha1_init(&sha1_ctx);

    /* separated API test */
    ret = mbedtls_sha1_starts_ret(&sha1_ctx);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sha1_starts_ret failed!\n");
        goto end;
    }
    ret = mbedtls_sha1_update_ret(&sha1_ctx, message, message_size);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sha1_update_ret failed!\n");
        goto end;
    }
    ret = mbedtls_sha1_finish_ret(&sha1_ctx, hash);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sha1_finish_ret failed!\n");
        goto end;
    }
    if (pal_memcmp(hash, ref_hash, sizeof(hash)) != 0) {
        PAL_LOG_ERR("sha1 hash compare with reference failed!\n");
        goto end;
    }

    /* all-in-one API test */
    pal_memset(hash, 0x00, sizeof(hash));
    ret = mbedtls_sha1_ret(message, message_size, hash);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sha1_ret failed!\n");
        goto end;
    }
    if (pal_memcmp(hash, ref_hash, sizeof(hash)) != 0) {
        PAL_LOG_ERR("sha1 hash compare with reference failed!\n");
        goto end;
    }

    PAL_LOG_INFO("      PASS!\n");

end:
    mbedtls_sha1_free(&sha1_ctx);
    return ret;
}

static int32_t sha256_verify(const uint8_t *message,
                             size_t message_size,
                             const uint8_t *ref_hash,
                             uint8_t is_224)
{
    int32_t ret = 0;
    uint8_t hash[32] = {0};
    uint32_t hash_size = 0;
    mbedtls_sha256_context sha256_ctx = {0};

    mbedtls_sha256_init(&sha256_ctx);

    if (is_224) {
        hash_size = 28;
    } else {
        hash_size = 32;
    }

    /* separated API test */
    ret = mbedtls_sha256_starts_ret(&sha256_ctx, is_224);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sha256_starts_ret failed!\n");
        goto end;
    }
    ret = mbedtls_sha256_update_ret(&sha256_ctx, message, message_size);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sha256_update_ret failed!\n");
        goto end;
    }
    ret = mbedtls_sha256_finish_ret(&sha256_ctx, hash);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sha256_finish_ret failed!\n");
        goto end;
    }
    if (pal_memcmp(hash, ref_hash, hash_size) != 0) {
        PAL_LOG_ERR("sha256 hash compare with reference failed!\n");
        goto end;
    }

    /* all-in-one API test */
    pal_memset(hash, 0x00, sizeof(hash));
    ret = mbedtls_sha256_ret(message, message_size, hash, is_224);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sha256_ret failed!\n");
        goto end;
    }
    if (pal_memcmp(hash, ref_hash, hash_size) != 0) {
        PAL_LOG_ERR("sha256 hash compare with reference failed!\n");
        goto end;
    }

    PAL_LOG_INFO("      PASS!\n");

end:
    mbedtls_sha256_free(&sha256_ctx);
    return ret;
}

static int32_t sm3_verify(const uint8_t *message,
                          size_t message_size,
                          const uint8_t *ref_hash)
{
    int32_t ret = 0;
    uint8_t hash[32] = {0};
    mbedtls_sm3_context sm3_ctx = {0};

    mbedtls_sm3_init(&sm3_ctx);

    /* separated API test */
    ret = mbedtls_sm3_starts_ret(&sm3_ctx);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sm3_starts_ret failed!\n");
        goto end;
    }
    ret = mbedtls_sm3_update_ret(&sm3_ctx, message, message_size);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sm3_update_ret failed!\n");
        goto end;
    }
    ret = mbedtls_sm3_finish_ret(&sm3_ctx, hash);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sm3_finish_ret failed!\n");
        goto end;
    }
    if (0 != pal_memcmp(hash, ref_hash, sizeof(hash))) {
        PAL_LOG_ERR("sm3 hash compare with reference failed!\n");
        goto end;
    }

    /* all-in-one API test */
    pal_memset(hash, 0x00, sizeof(hash));
    ret = mbedtls_sm3_ret(message, message_size, hash);
    if (ret != 0) {
        PAL_LOG_ERR("mbedtls_sm3_ret failed!\n");
        goto end;
    }
    if (0 != pal_memcmp(hash, ref_hash, sizeof(hash))) {
        PAL_LOG_ERR("sm3 hash compare with reference failed!\n");
        goto end;
    }

    PAL_LOG_INFO("      PASS!\n");

end:
    mbedtls_sm3_free(&sm3_ctx);
    return ret;
}

int32_t sha1_example(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("SHA1 example...");
    ret = sha1_verify(message_hash,
                      sizeof(message_hash),
                      sha1_hash_reference);
    return ret;
}

int32_t sha224_example(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("SHA224 example...");
    ret = sha256_verify(message_hash,
                        sizeof(message_hash),
                        sha224_hash_reference,
                        1);
    return ret;
}

int32_t sha256_example(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("SHA256 example...");
    ret = sha256_verify(message_hash,
                        sizeof(message_hash),
                        sha256_hash_reference,
                        0);
    return ret;
}

int32_t sm3_example(void)
{
    int32_t ret = 0;

    PAL_LOG_INFO("SM3 example...");
    ret = sm3_verify(message_hash,
                     sizeof(message_hash),
                     sm3_hash_reference);
    return ret;
}

int32_t hash_example_entry(void)
{
    int32_t ret = 0;

    ret = sha1_example();
    if (ret != 0) {
        return ret;
    }

    ret = sha224_example();
    if (ret != 0) {
        return ret;
    }

    ret = sha256_example();
    if (ret != 0) {
        return ret;
    }

    ret = sm3_example();
    return ret;
}
