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
#include "ce_lite_config.h"
#if defined(CE_LITE_SM4)
#include "ce_lite_cipher.h"
#include "comm_util.h"
#include "mbedtls/sm4.h"

 /* magic string: "SM4" */
#define MBEDTLS_SM4_MAGIC (0x534D34U)

/**
 * Note: enum value CE_ENCRYPT on ce_lite_cipher.h is 0.
 *       but original MBEDTLS_SM4_ENCRYPT is 1.
 */
#define MBEDTLS_SM4_GET_DRV_MODE(_md_) \
        (((_md_) == MBEDTLS_SM4_ENCRYPT) ? CE_ENCRYPT : CE_DECRYPT)
#define MBEDTLS_SM4_GET_DRV_KEYSEL(_tp_) \
        (((_tp_) == MBEDTLS_SM4_KL_KEY_MODEL) ? CE_KLAD_KEY_MODEL : CE_KLAD_KEY_ROOT)

/**
 * key type
 */
typedef enum mbedtls_sm4_key_type_t {
    MBEDTLS_SM4_USER_KEY = 0,         /**< user key */
    MBEDTLS_SM4_SEC_KEY,              /**< secure key */
} mbedtls_sm4_key_type;

/**
 * private ce_sm4_data context.
 */
typedef struct mbedtls_ce_sm4_data {
    union {
        struct {
            uint8_t key[CE_SM4_KEY_SIZE];
            uint32_t keybits;
        } user;
        ce_sec_key_t sec;
    };
    mbedtls_sm4_key_type key_type;
    struct ce_cipher_ctx cipher;
} mbedtls_ce_sm4_data_t;

/**
 * Error code mapping
 *| ------------------------------------ | -------------------------- |
 *| **Mbedtls ALT**                      | **Ce Lite Driver**         |
 *| ------------------------------------ | -------------------------- |
 *| MBEDTLS_ERR_SM4_ALLOC_FAILED         | CE_ERROR_OOM               |
 *| MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH   | CE_ERROR_BAD_KEY_LENGTH    |
 *| MBEDTLS_ERR_SM4_INVALID_INPUT_LENGTH | CE_ERROR_BAD_INPUT_LENGTH  |
 *| MBEDTLS_ERR_SM4_BAD_INPUT_DATA       | CE_ERROR_BAD_PARAMS        |
 *| MBEDTLS_ERR_SM4_BAD_INPUT_DATA       | CE_ERROR_BAD_FORMAT        |
 *| MBEDTLS_ERR_SM4_BAD_INPUT_DATA       | CE_ERROR_BAD_PADDING       |
 *| MBEDTLS_ERR_SM4_HW_ACCEL_FAILED      | Other CE Error Code        |
 *| 0                                    | CE_SUCCESS                 |
 *| ------------------------------------ | -------------------------- |
 */
static int _convert_retval_to_mbedtls(int errno)
{
    switch (errno) {
    case CE_SUCCESS:
        errno = 0;
        break;
    case CE_ERROR_OOM:
        errno = MBEDTLS_ERR_SM4_ALLOC_FAILED;
        break;
    case CE_ERROR_BAD_PARAMS:
    case CE_ERROR_BAD_FORMAT:
    case CE_ERROR_BAD_PADDING:
        errno = MBEDTLS_ERR_SM4_BAD_INPUT_DATA;
        break;
    case CE_ERROR_BAD_INPUT_LENGTH:
        errno = MBEDTLS_ERR_SM4_INVALID_INPUT_LENGTH;
        break;
    case CE_ERROR_BAD_KEY_LENGTH:
        errno = MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH;
        break;
    case CE_ERROR_GENERIC:
    default:
        errno = MBEDTLS_ERR_SM4_HW_ACCEL_FAILED;
        break;
    }
    return errno;
}

static void _mbedtls_sec_key_to_te_sec_key(ce_sec_key_t *sec_key,
                                           mbedtls_sm4_sec_key_t *key)
{
    sec_key->sel = MBEDTLS_SM4_GET_DRV_KEYSEL(key->sel);
    sec_key->ek1bits = key->ek1bits;
    pal_memcpy(sec_key->eks, key->eks,
                sizeof(sec_key->eks) > sizeof(key->eks) ? \
                    sizeof(key->eks) : sizeof(sec_key->eks));
}

static int _sm4_set_key(mbedtls_sm4_context *ctx)
{
    int ret = CE_SUCCESS;

    if (MBEDTLS_SM4_USER_KEY == ctx->acd->key_type) {
        ret = ce_cipher_setkey(&ctx->acd->cipher,
                               ctx->acd->user.key,
                               ctx->acd->user.keybits);
    } else if (MBEDTLS_SM4_SEC_KEY == ctx->acd->key_type) {
        ret = ce_cipher_setseckey(&ctx->acd->cipher,
                                  &ctx->acd->sec);
    } else {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }

finish:
    return ret;
}

void mbedtls_sm4_init(mbedtls_sm4_context *ctx)
{
    if (NULL == ctx) {
        return;
    }
    if ((MBEDTLS_SM4_MAGIC == ctx->magic) &&
        (ctx->acd != NULL)) {
        PAL_LOG_ERR("#WARN %s %d ctx may double init\n",
                     __func__, __LINE__);
    }
    pal_memset(ctx, 0x00, sizeof(*ctx));
    ctx->acd = (mbedtls_ce_sm4_data_t *)pal_calloc(1,
                sizeof(*(ctx->acd)));
    PAL_ASSERT(NULL != ctx->acd);
    ctx->magic = MBEDTLS_SM4_MAGIC;
}

void mbedtls_sm4_free(mbedtls_sm4_context *ctx)
{
    if ((NULL == ctx) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        return;
    }
    if (NULL != ctx->acd) {
        ce_cipher_free(&ctx->acd->cipher);
        pal_memset(ctx->acd, 0x00, sizeof(*(ctx->acd)));
        pal_free(ctx->acd);
    }
    pal_memset(ctx, 0x00, sizeof(*ctx));
}

int mbedtls_sm4_setkey_enc(mbedtls_sm4_context *ctx,
                           const unsigned char *key,
                           unsigned int keybits)
{
    int ret = CE_SUCCESS;

    if ((NULL == ctx) || (NULL == key) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        ret = MBEDTLS_ERR_SM4_BAD_INPUT_DATA;
        goto finish;
    }
    if (NULL == ctx->acd) {
        ret = MBEDTLS_ERR_SM4_BAD_INPUT_DATA;
        goto finish;
    }
    if (128 != keybits) {
        ret = MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH;
        goto finish;
    }
    ctx->acd->user.keybits = keybits;
    pal_memcpy(ctx->acd->user.key, key, keybits / 8);
    ctx->acd->key_type = MBEDTLS_SM4_USER_KEY;
finish:
    return ret;
}

int mbedtls_sm4_setseckey_enc(mbedtls_sm4_context *ctx,
                              mbedtls_sm4_sec_key_t *key)
{
    int ret = CE_SUCCESS;
    ce_sec_key_t sec_key = {0};

    /* 0. Check input parameters. */
    if ((NULL == ctx) || (NULL == key) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        ret = MBEDTLS_ERR_SM4_BAD_INPUT_DATA;
        goto finish;
    }
    if (NULL == ctx->acd) {
        ret = MBEDTLS_ERR_SM4_BAD_INPUT_DATA;
        goto finish;
    }

    /* 1. Check mbedtls sm4 key type. */
    if ((MBEDTLS_SM4_KL_KEY_MODEL != key->sel) &&
        (MBEDTLS_SM4_KL_KEY_ROOT != key->sel)) {
        ret = MBEDTLS_ERR_SM4_BAD_INPUT_DATA;
        goto finish;
    }
    if (128 != key->ek1bits) {
        ret = MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH;
        goto finish;
    }

    /* 2. Cache mbedtls soft sec key. */
    _mbedtls_sec_key_to_te_sec_key(&sec_key, key);
    pal_memcpy(&ctx->acd->sec, &sec_key, sizeof(ce_sec_key_t));
    ctx->acd->key_type = MBEDTLS_SM4_SEC_KEY;

finish:
    return ret;
}

int mbedtls_sm4_setkey_dec(mbedtls_sm4_context *ctx,
                           const unsigned char *key,
                           unsigned int keybits)
{
    return mbedtls_sm4_setkey_enc(ctx, key, keybits);
}

int mbedtls_sm4_setseckey_dec(mbedtls_sm4_context *ctx,
                              mbedtls_sm4_sec_key_t *key)
{
    return mbedtls_sm4_setseckey_enc(ctx, key);
}

#if defined(CE_LITE_CIPHER_MODE_ECB)

int mbedtls_sm4_crypt_ecb(mbedtls_sm4_context *ctx,
                          int mode,
                          const unsigned char input[16],
                          unsigned char output[16])
{
    return mbedtls_sm4_crypt_ecb_ext(ctx,
                                     mode,
                                     CE_SM4_BLOCK_SIZE,
                                     input,
                                     output);
}

int mbedtls_sm4_crypt_ecb_ext(mbedtls_sm4_context *ctx,
                              int mode,
                              size_t inlen,
                              const unsigned char *input,
                              unsigned char *output)
{
    int ret = CE_SUCCESS;
    ce_algo_t alg = CE_ALG_SM4_ECB_NOPAD;

    /* 0. Check input parameters */
    if ((NULL == ctx) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if ((MBEDTLS_SM4_ENCRYPT != mode) &&
        (MBEDTLS_SM4_DECRYPT != mode)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if (NULL == ctx->acd) {
        ret = CE_ERROR_BAD_FORMAT;
        goto finish;
    }

    /* 1. Init CE Lite Driver. */
    ret = ce_cipher_init(&ctx->acd->cipher, alg);
    if (CE_SUCCESS != ret) {
        goto finish;
    }

    /* 2. Set key from soft key. */
    ret = _sm4_set_key(ctx);
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 3. Set operation mode. */
    ret = ce_cipher_set_op_mode(&ctx->acd->cipher,
                                MBEDTLS_SM4_GET_DRV_MODE(mode));
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 4. Call ce_cipher_crypt to update input buffer. */
    ret = ce_cipher_crypt(&ctx->acd->cipher,
                          false,
                          inlen,
                          input,
                          NULL,
                          output);

error:
    /* 5. Free ce_cipher_ctx_t space. */
    ce_cipher_free(&ctx->acd->cipher);
finish:
    return _convert_retval_to_mbedtls(ret);
}

#if defined(CE_LITE_CIPHER_PADDING_PKCS7)

int mbedtls_sm4_crypt_ecb_pkcs7(mbedtls_sm4_context *ctx,
                                int mode,
                                bool is_last,
                                size_t inlen,
                                const unsigned char *input,
                                size_t *outlen,
                                unsigned char *output)
{
    int ret = CE_SUCCESS;
    ce_algo_t alg = CE_ALG_SM4_ECB_PKCS7;

    /* 0. Check input parameters */
    if ((NULL == ctx) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if ((MBEDTLS_SM4_ENCRYPT != mode) &&
        (MBEDTLS_SM4_DECRYPT != mode)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if (NULL == ctx->acd) {
        ret = CE_ERROR_BAD_FORMAT;
        goto finish;
    }

    /* 1. Init CE Lite Driver. */
    ret = ce_cipher_init(&ctx->acd->cipher, alg);
    if (CE_SUCCESS != ret) {
        goto finish;
    }

    /* 2. Set key from software key. */
    ret = _sm4_set_key(ctx);
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 3. Set operation mode. */
    ret = ce_cipher_set_op_mode(&ctx->acd->cipher,
                                MBEDTLS_SM4_GET_DRV_MODE(mode));
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 4. Call ce_cipher_crypt to update input buffer. */
    ret = ce_cipher_crypt(&ctx->acd->cipher,
                          is_last,
                          inlen,
                          input,
                          outlen,
                          output);

error:
    /* 5. Free ce_cipher_ctx_t space. */
    ce_cipher_free(&ctx->acd->cipher);
finish:
    return _convert_retval_to_mbedtls(ret);
}

#endif  /* CE_LITE_CIPHER_PADDING_PKCS7 */

#endif  /* CE_LITE_CIPHER_MODE_ECB */

#ifdef CE_LITE_CIPHER_MULTI_BLOCKS

#if defined(CE_LITE_CIPHER_MODE_CBC)

int mbedtls_sm4_crypt_cbc(mbedtls_sm4_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int ret = CE_SUCCESS;
    uint8_t local_iv[CE_SM4_BLOCK_SIZE] = {0};
    ce_algo_t alg = CE_ALG_SM4_CBC_NOPAD;

    /* 0. Check input parameters */
    if ((NULL == ctx) ||
        (NULL == iv) ||
        (length && (NULL == input)) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if ((MBEDTLS_SM4_ENCRYPT != mode) &&
        (MBEDTLS_SM4_DECRYPT != mode)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if (NULL == ctx->acd) {
        ret = CE_ERROR_BAD_FORMAT;
        goto finish;
    }

    /* 1. Init CE Lite Driver. */
    ret = ce_cipher_init(&ctx->acd->cipher, alg);
    if (CE_SUCCESS != ret) {
        goto finish;
    }

    /* 2. Set key from soft key and IV. */
    ret = _sm4_set_key(ctx);
    if (CE_SUCCESS != ret) {
        goto error;
    }
    ret = ce_cipher_set_iv(&ctx->acd->cipher, iv);
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 3. Set operation mode. */
    ret = ce_cipher_set_op_mode(&ctx->acd->cipher,
                                MBEDTLS_SM4_GET_DRV_MODE(mode));
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /**
     * 4. Update IV before decrypting on decryption.
     *    When the \p length is less than \c CE_SM4_BLOCK_SIZE .
     *    IV will not be cached.
     */
    if ((MBEDTLS_SM4_DECRYPT == mode) &&
        (length >= CE_SM4_BLOCK_SIZE)) {
        pal_memcpy(local_iv,
                   input + length - sizeof(local_iv),
                   sizeof(local_iv));
    }

    /* 5. Call ce_cipher_crypto to update input buffer. */
    ret = ce_cipher_crypt(&ctx->acd->cipher,
                          false,
                          length,
                          input,
                          NULL,
                          output);
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 6. Recovery IV for user. */
    if (length >= CE_SM4_BLOCK_SIZE) {
        if (MBEDTLS_SM4_DECRYPT == mode) {
            pal_memcpy(iv, local_iv, CE_SM4_BLOCK_SIZE);
        } else {
            pal_memcpy(iv,
                       output + length - CE_SM4_BLOCK_SIZE,
                       CE_SM4_BLOCK_SIZE);
        }
    }

error:
    /* 7. Free ce_cipher_ctx_t space. */
    ce_cipher_free(&ctx->acd->cipher);
finish:
    pal_memset(local_iv, 0x00, sizeof(local_iv));
    return _convert_retval_to_mbedtls(ret);
}

#if defined(CE_LITE_CIPHER_PADDING_PKCS7)

int mbedtls_sm4_crypt_cbc_pkcs7(mbedtls_sm4_context *ctx,
                                int mode,
                                bool is_last,
                                unsigned char iv[16],
                                size_t inlen,
                                const unsigned char *input,
                                size_t *outlen,
                                unsigned char *output)
{
    int ret = CE_SUCCESS;
    uint8_t local_iv[CE_SM4_BLOCK_SIZE] = {0};
    ce_algo_t alg = CE_ALG_SM4_CBC_PKCS7;
    size_t olen = 0;

    /* 0. Check input parameters */
    if ((NULL == ctx) ||
        (NULL == iv) ||
        (NULL == input) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if ((MBEDTLS_SM4_ENCRYPT != mode) &&
        (MBEDTLS_SM4_DECRYPT != mode)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if (NULL == ctx->acd) {
        ret = CE_ERROR_BAD_FORMAT;
        goto finish;
    }

    /* 1. Init CE Lite Driver. */
    ret = ce_cipher_init(&ctx->acd->cipher, alg);
    if (CE_SUCCESS != ret) {
        goto finish;
    }

    /* 2. Set key from soft key and IV.*/
    ret = _sm4_set_key(ctx);
    if (CE_SUCCESS != ret) {
        goto error;
    }
    ret = ce_cipher_set_iv(&ctx->acd->cipher, iv);
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 3. Set operation mode. */
    ret = ce_cipher_set_op_mode(&ctx->acd->cipher,
                                MBEDTLS_SM4_GET_DRV_MODE(mode));
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /**
     * 4. Update IV before decrypting on decryption.
     *    When the \p inlen is less than \c CE_SM4_BLOCK_SIZE .
     *    IV will not be cached.
     */
    if ((MBEDTLS_SM4_DECRYPT == mode) &&
        (inlen >= CE_SM4_BLOCK_SIZE)) {
        pal_memcpy(local_iv,
                   input + inlen - CE_SM4_BLOCK_SIZE,
                   CE_SM4_BLOCK_SIZE);
    }

    /* 5. Call ce_cipher_crypto to update input buffer. */
    ret = ce_cipher_crypt(&ctx->acd->cipher,
                          is_last,
                          inlen,
                          input,
                          &olen,
                          output);
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /**
     * Recovery IV for user.
     * > When set \p is_last to \c true, the returned IV array
     *    should no longer be reused at next CBC ciphering.
     * > In encryption mode, when \p is_last is \c ture,
     *    the returned IV contains the value of pkcs7 padding.
     * > \p olen is always not less than \c CE_SM4_BLOCK_SIZE .
     **/
    if (MBEDTLS_SM4_ENCRYPT == mode) {
        pal_memcpy(iv,
                   output + olen - CE_SM4_BLOCK_SIZE,
                   CE_SM4_BLOCK_SIZE);
    }
    if ((MBEDTLS_SM4_DECRYPT == mode) &&
        (inlen >= CE_SM4_BLOCK_SIZE)) {
        pal_memcpy(iv, local_iv, CE_SM4_BLOCK_SIZE);
    }
    if (outlen != NULL) {
        *outlen = olen;
    }

error:
    /* 6. Free ce_cipher_ctx_t space. */
    ce_cipher_free(&ctx->acd->cipher);
finish:
    olen = 0;
    pal_memset(local_iv, 0x00, sizeof(local_iv));
    return _convert_retval_to_mbedtls(ret);
}

#endif  /* CE_LITE_CIPHER_PADDING_PKCS7 */

#endif  /* CE_LITE_CIPHER_MODE_CBC */

#if defined(CE_LITE_CIPHER_MODE_CTR)

int mbedtls_sm4_crypt_ctr(mbedtls_sm4_context *ctx,
                          size_t length,
                          size_t *nc_off,
                          unsigned char nonce_counter[16],
                          unsigned char stream_block[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int ret = CE_SUCCESS;
    size_t prolen = 0;
    size_t ofs = 0;
    size_t left = length;
    size_t kpos = 0;
    uint8_t in_buf[CE_SM4_BLOCK_SIZE] = {0};
    uint8_t out_buf[CE_SM4_BLOCK_SIZE] = {0};
    ce_algo_t alg = CE_ALG_SM4_CTR;

    /* 0. Check input parameters */
    if ((NULL == ctx) ||
        (NULL == nc_off) ||
        (NULL == output) ||
        (NULL == stream_block) ||
        (NULL == nonce_counter) ||
        (length && (NULL == input)) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if (*nc_off > 0x0FU) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if (NULL == ctx->acd) {
        ret = CE_ERROR_BAD_FORMAT;
        goto finish;
    }

    /* There is no need to call driver when inlen is 0. */
    if (!length) {
        goto finish;
    }

    /* 1. Consume user's keystream. */
    kpos = *nc_off;
    if (kpos > 0) {
        prolen = MIN(CE_SM4_BLOCK_SIZE - kpos, left);
        BS_XOR(output, input, stream_block + kpos, prolen);
        left -= prolen;
        ofs += prolen;
        kpos = (kpos + prolen) % CE_SM4_BLOCK_SIZE;
    }

    /* 2. if left != 0, need to call hw. */
    if (!left) {
        *nc_off = kpos;
        goto finish;
    }

    /* 3. init CE Lite Driver. */
    ret = ce_cipher_init(&ctx->acd->cipher, alg);
    if (CE_SUCCESS != ret) {
        goto finish;
    }

    /* 4. Set key from soft key. */
    ret = _sm4_set_key(ctx);
    if (CE_SUCCESS != ret) {
        goto error;
    }
    ret = ce_cipher_set_iv(&ctx->acd->cipher, nonce_counter);
    if (CE_SUCCESS != ret) {
        goto error;
    }
    ret = ce_cipher_set_op_mode(&ctx->acd->cipher,
                                CE_ENCRYPT);
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 5. Processing the largest block alignment data. */
    prolen = ROUND_DOWN(left, CE_SM4_BLOCK_SIZE);
    ret = ce_cipher_crypt(&ctx->acd->cipher,
                          false,
                          prolen,
                          input + ofs,
                          &prolen,
                          output + ofs);
    if (CE_SUCCESS != ret) {
        goto error;
    }
    left -= prolen;
    ofs += prolen;

    /* 6. Increase nonce_counter. */
    COUNTER_INCREASE(nonce_counter,
                     CE_SM4_BLOCK_SIZE,
                     CTR_SIZE,
                     prolen / CE_SM4_BLOCK_SIZE);

    /* 7. Processing the tail data. */
    if (left > 0) {
        pal_memset(in_buf, 0x00, sizeof(in_buf));
        pal_memcpy(in_buf, input + ofs, left);
        ret = ce_cipher_crypt(&ctx->acd->cipher,
                            false,
                            CE_SM4_BLOCK_SIZE,
                            in_buf,
                            &prolen,
                            out_buf);
        if (CE_SUCCESS != ret) {
            goto error;
        }
        pal_memcpy(output + ofs, out_buf, left);
        COUNTER_INCREASE(nonce_counter,
                         CE_SM4_BLOCK_SIZE,
                         CTR_SIZE,
                         1);
        /* 8. Cache kstream. */
        BS_XOR(stream_block,
               in_buf,
               out_buf,
               CE_SM4_BLOCK_SIZE);
    } else {
        /* 8. Cache kstream. */
        BS_XOR(stream_block,
               input + length - CE_SM4_BLOCK_SIZE,
               output + length - CE_SM4_BLOCK_SIZE,
               CE_SM4_BLOCK_SIZE);
    }
    *nc_off = left;

error:
    /* 9. Free ce_cipher_ctx_t space. */
    ce_cipher_free(&ctx->acd->cipher);
finish:
    prolen = 0;
    ofs = 0;
    left = 0;
    kpos = 0;
    pal_memset(in_buf, 0x00, sizeof(in_buf));
    pal_memset(out_buf, 0x00, sizeof(out_buf));
    return _convert_retval_to_mbedtls(ret);
}

#endif  /* CE_LITE_CIPHER_MODE_CTR */

#else   /* CE_LITE_CIPHER_MULTI_BLOCKS */

#if defined(CE_LITE_CIPHER_MODE_CBC)

int mbedtls_sm4_crypt_cbc(mbedtls_sm4_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int ret = CE_SUCCESS;
    ce_algo_t alg = CE_ALG_SM4_CBC_NOPAD;

    /* 0. Check input parameters */
    if ((NULL == ctx) ||
        (NULL == iv) ||
        (length && (NULL == input)) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if ((MBEDTLS_SM4_ENCRYPT != mode) &&
        (MBEDTLS_SM4_DECRYPT != mode)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if (NULL == ctx->acd) {
        ret = CE_ERROR_BAD_FORMAT;
        goto finish;
    }

    /* There is no need to call driver when inlen is 0. */
    if (!length) {
        goto finish;
    }

    /* 1. Init CE Lite Driver. */
    ret = ce_cipher_init(&ctx->acd->cipher, alg);
    if (CE_SUCCESS != ret) {
        goto finish;
    }

    /* 2. Set key from soft key and IV. */
    ret = _sm4_set_key(ctx);
    if (CE_SUCCESS != ret) {
        goto error;
    }
    ret = ce_cipher_set_iv(&ctx->acd->cipher, iv);
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 3. Set operation mode. */
    ret = ce_cipher_set_op_mode(&ctx->acd->cipher,
                                MBEDTLS_SM4_GET_DRV_MODE(mode));
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 4. Call ce_cipher_crypto to update input buffer. */
    ret = ce_cipher_crypt(&ctx->acd->cipher,
                          false,
                          length,
                          input,
                          NULL,
                          output);

error:
    /* 5. Free ce_cipher_ctx_t space. */
    ce_cipher_free(&ctx->acd->cipher);
finish:
    return _convert_retval_to_mbedtls(ret);
}

#if defined(CE_LITE_CIPHER_PADDING_PKCS7)

int mbedtls_sm4_crypt_cbc_pkcs7(mbedtls_sm4_context *ctx,
                                int mode,
                                bool is_last,
                                unsigned char iv[16],
                                size_t inlen,
                                const unsigned char *input,
                                size_t *outlen,
                                unsigned char *output)
{
    int ret = CE_SUCCESS;
    ce_algo_t alg = CE_ALG_SM4_CBC_PKCS7;

    /* 0. Check input parameters */
    if ((NULL == ctx) ||
        (NULL == iv) ||
        (NULL == input) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if ((MBEDTLS_SM4_ENCRYPT != mode) &&
        (MBEDTLS_SM4_DECRYPT != mode)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if (NULL == ctx->acd) {
        ret = CE_ERROR_BAD_FORMAT;
        goto finish;
    }

    /* 1. Init CE Lite Driver. */
    ret = ce_cipher_init(&ctx->acd->cipher, alg);
    if (CE_SUCCESS != ret) {
        goto finish;
    }

    /* 2. Set key from soft key and IV.*/
    ret = _sm4_set_key(ctx);
    if (CE_SUCCESS != ret) {
        goto error;
    }
    ret = ce_cipher_set_iv(&ctx->acd->cipher, iv);
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 3. Set operation mode. */
    ret = ce_cipher_set_op_mode(&ctx->acd->cipher,
                                MBEDTLS_SM4_GET_DRV_MODE(mode));
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 4. Call ce_cipher_crypto to update input buffer. */
    ret = ce_cipher_crypt(&ctx->acd->cipher,
                          is_last,
                          inlen,
                          input,
                          outlen,
                          output);

error:
    /* 5. Free ce_cipher_ctx_t space. */
    ce_cipher_free(&ctx->acd->cipher);
finish:
    return _convert_retval_to_mbedtls(ret);
}

#endif  /* CE_LITE_CIPHER_PADDING_PKCS7 */

#endif  /* CE_LITE_CIPHER_MODE_CBC */

#if defined(CE_LITE_CIPHER_MODE_CTR)

int mbedtls_sm4_crypt_ctr(mbedtls_sm4_context *ctx,
                          size_t length,
                          size_t *nc_off,
                          unsigned char nonce_counter[16],
                          unsigned char stream_block[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int ret = CE_SUCCESS;
    ce_algo_t alg = CE_ALG_SM4_CTR;
    (void)nc_off;
    (void)stream_block;

    /* 0. Check input parameters */
    if ((NULL == ctx) ||
        (NULL == output) ||
        (length && (NULL == input)) ||
        (ctx->magic != MBEDTLS_SM4_MAGIC)) {
        ret = CE_ERROR_BAD_PARAMS;
        goto finish;
    }
    if (NULL == ctx->acd) {
        ret = CE_ERROR_BAD_FORMAT;
        goto finish;
    }

    /* There is no need to call driver when inlen is 0. */
    if (!length) {
        goto finish;
    }

    /* 1. init CE Lite Driver. */
    ret = ce_cipher_init(&ctx->acd->cipher, alg);
    if (CE_SUCCESS != ret) {
        goto finish;
    }

    /* 2. Set key from soft key and IV. */
    ret = _sm4_set_key(ctx);
    if (CE_SUCCESS != ret) {
        goto error;
    }
    ret = ce_cipher_set_iv(&ctx->acd->cipher, nonce_counter);
    if (CE_SUCCESS != ret) {
        goto error;
    }
    ret = ce_cipher_set_op_mode(&ctx->acd->cipher,
                                CE_ENCRYPT);
    if (CE_SUCCESS != ret) {
        goto error;
    }

    /* 3. Processing the largest block alignment data. */
    ret = ce_cipher_crypt(&ctx->acd->cipher,
                          false,
                          length,
                          input,
                          NULL,
                          output);
error:
    /* 4. Free ce_cipher_ctx_t space. */
    ce_cipher_free(&ctx->acd->cipher);
finish:
    return _convert_retval_to_mbedtls(ret);
}

#endif  /* CE_LITE_CIPHER_MODE_CTR */

#endif  /* !CE_LITE_CIPHER_MULTI_BLOCKS */

#endif  /* CE_LITE_SM4 */