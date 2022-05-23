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

#include "ce_lite_hash.h"
#include "ce_lite_common.h"
#include "ce_lite_config.h"
#include "mbedtls/sha256.h"
#include "mbedtls/platform.h"

#if defined(CE_LITE_SHA256) || defined(CE_LITE_SHA224)

#define MBEDTLS_SHA256_MAGIC       (0x53484132U)         /* SHA2 */

static int _convert_retval_to_mbedtls(int errno)
{
    switch (errno) {
        case CE_SUCCESS:
            errno = 0;
            break;
        case CE_ERROR_OOM:
            errno = MBEDTLS_ERR_SHA256_ALLOC_FAILED;
            break;
        case CE_ERROR_BAD_PARAMS:
        case CE_ERROR_NO_DATA:
            errno = MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
            break;
        case CE_ERROR_GENERIC:
        default:
            errno = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;
            break;
    }

    return errno;
}

void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    if (NULL == ctx) {
        return;
    }

    pal_memset(ctx, 0, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if (NULL == ctx) {
        return;
    }

    pal_memset(ctx, 0, sizeof(mbedtls_sha256_context));
}

int mbedtls_sha256_starts_ret(mbedtls_sha256_context *ctx, int is224)
{
    int ret = CE_SUCCESS;

    if (NULL == ctx) {
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }

    if ((ctx->ce_ctx != NULL) &&
        (MBEDTLS_SHA256_MAGIC == ctx->magic)) {
        mbedtls_printf("#ERROR %s %d ctx may double start\n",
                       __func__, __LINE__);
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }

    ctx->ce_ctx = (ce_dgst_ctx_t *)pal_calloc(1, sizeof(ce_dgst_ctx_t));
    if (NULL == ctx->ce_ctx) {
        ret = CE_ERROR_OOM;
        goto fin;
    }

    ret = ce_dgst_init(ctx->ce_ctx, is224 ? CE_ALG_SHA224 : CE_ALG_SHA256);
    if (ret != CE_SUCCESS) {
        pal_free(ctx->ce_ctx);
        ctx->ce_ctx = NULL;
        goto fin;
    }

    ret = ce_dgst_start(ctx->ce_ctx);
    if (CE_SUCCESS == ret) {
        ctx->magic = MBEDTLS_SHA256_MAGIC;
    } else {
        if (ctx->ce_ctx != NULL) {
            ce_dgst_free(ctx->ce_ctx);
            pal_free(ctx->ce_ctx);
            ctx->ce_ctx = NULL;
        }
    }

fin:
    return _convert_retval_to_mbedtls(ret);
}

int mbedtls_sha256_update_ret(mbedtls_sha256_context *ctx,
                              const unsigned char *input,
                              size_t ilen)
{
    if ((NULL == ctx) ||
        (ctx->magic != MBEDTLS_SHA256_MAGIC)) {
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }

    return _convert_retval_to_mbedtls(
            ce_dgst_update(ctx->ce_ctx,
                           input,
                           ilen));
}

int mbedtls_sha256_finish_ret(mbedtls_sha256_context *ctx,
                              unsigned char output[32])
{
    int ret = CE_SUCCESS;

    if ((NULL == ctx) || (NULL == output) ||
        (ctx->magic != MBEDTLS_SHA256_MAGIC)) {
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }

    ret = ce_dgst_finish(ctx->ce_ctx, output);

    if (ctx->ce_ctx != NULL) {
        ce_dgst_free(ctx->ce_ctx);
        pal_free(ctx->ce_ctx);
    }

    ctx->magic = 0;
    ctx->ce_ctx = NULL;

    return _convert_retval_to_mbedtls(ret);
}

int mbedtls_sha256_ret(const unsigned char *input,
                       size_t ilen,
                       unsigned char output[32],
                       int is224)
{
    return _convert_retval_to_mbedtls(
            ce_dgst(is224 ? CE_ALG_SHA224 : CE_ALG_SHA256,
                    input,
                    ilen,
                    output));
}
#endif /* CE_LITE_SHA256 || CE_LITE_SHA224 */