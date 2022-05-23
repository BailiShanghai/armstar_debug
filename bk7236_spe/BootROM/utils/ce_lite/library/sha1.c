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
#include "mbedtls/sha1.h"
#include "mbedtls/platform.h"

#if defined(CE_LITE_SHA1)
static int _convert_retval_to_mbedtls(int errno)
{
    switch (errno) {
        case CE_SUCCESS:
            errno = 0;
            break;
        case CE_ERROR_OOM:
            errno = MBEDTLS_ERR_SHA1_ALLOC_FAILED;
            break;
        case CE_ERROR_BAD_PARAMS:
        case CE_ERROR_NO_DATA:
            errno = MBEDTLS_ERR_SHA1_BAD_INPUT_DATA;
            break;
        case CE_ERROR_GENERIC:
        default:
            errno = MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
            break;
    }

    return errno;
}

void mbedtls_sha1_init(mbedtls_sha1_context *ctx)
{
    if (NULL == ctx) {
        return;
    }

    pal_memset(ctx, 0, sizeof(mbedtls_sha1_context));
}

void mbedtls_sha1_free(mbedtls_sha1_context *ctx)
{
    if (NULL == ctx) {
        return;
    }

    pal_memset(ctx, 0, sizeof(mbedtls_sha1_context));
}

int mbedtls_sha1_starts_ret(mbedtls_sha1_context *ctx)
{
    int ret = CE_SUCCESS;

    if (NULL == ctx) {
        return MBEDTLS_ERR_SHA1_BAD_INPUT_DATA;
    }

    if ((ctx->ce_ctx != NULL) &&
        (MBEDTLS_SHA1_MAGIC == ctx->magic)) {
        mbedtls_printf("#ERROR %s %d ctx may double start\n",
                        __func__, __LINE__);
        return MBEDTLS_ERR_SHA1_BAD_INPUT_DATA;
    }

    ctx->ce_ctx = (ce_dgst_ctx_t *)pal_calloc(1, sizeof(ce_dgst_ctx_t));
    if (NULL == ctx->ce_ctx) {
        ret = CE_ERROR_OOM;
        goto fin;
    }

    ret = ce_dgst_init(ctx->ce_ctx, CE_ALG_SHA1);
    if (ret != CE_SUCCESS) {
        pal_free(ctx->ce_ctx);
        ctx->ce_ctx = NULL;
        goto fin;
    }

    ret = ce_dgst_start(ctx->ce_ctx);
    if (CE_SUCCESS == ret) {
        ctx->magic = MBEDTLS_SHA1_MAGIC;
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

int mbedtls_sha1_update_ret(mbedtls_sha1_context *ctx,
                            const unsigned char *input,
                            size_t ilen)
{
    if ((NULL == ctx) ||
        (ctx->magic != MBEDTLS_SHA1_MAGIC)) {
        return MBEDTLS_ERR_SHA1_BAD_INPUT_DATA;
    }

    return _convert_retval_to_mbedtls(
            ce_dgst_update(ctx->ce_ctx,
                           input,
                           ilen));
}

int mbedtls_sha1_finish_ret(mbedtls_sha1_context *ctx,
                            unsigned char output[20])
{
    int ret = CE_SUCCESS;

    if ((NULL == ctx) || (NULL == output) ||
        (ctx->magic != MBEDTLS_SHA1_MAGIC)) {
        return MBEDTLS_ERR_SHA1_BAD_INPUT_DATA;
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

int mbedtls_sha1_ret(const unsigned char *input,
                     size_t ilen,
                     unsigned char output[20])
{
    return _convert_retval_to_mbedtls(
            ce_dgst(CE_ALG_SHA1,
                    input,
                    ilen,
                    output));
}
#endif /* CE_LITE_SHA1 */