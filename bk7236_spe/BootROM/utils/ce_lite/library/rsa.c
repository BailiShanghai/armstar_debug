/*
 * Copyright (c) 2021, Arm Technology (China) Co., Ltd.
 * All rights reserved.
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,
 * any derivative work shall retain this copyright notice.
 */

#include "ce_lite_bn.h"
#include "ce_lite_rsa.h"
#include "ce_lite_common.h"
#include "ce_lite_config.h"
#include "mbedtls/bignum.h"
#include "mbedtls/md.h"
#include "mbedtls/rsa.h"
#include "mbedtls/platform.h"

#if defined(CE_LITE_RSASSA)
static int _convert_retval_to_mbedtls(int errno)
{
    switch (errno) {
        case CE_SUCCESS:
            errno = 0;
            break;
        case CE_ERROR_BAD_PARAMS:
        case CE_ERROR_BAD_INPUT_DATA:
            errno = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
            break;
        case CE_ERROR_INVAL_PADDING:
            errno = MBEDTLS_ERR_RSA_INVALID_PADDING;
            break;
        case CE_ERROR_VERIFY_SIG:
            errno = MBEDTLS_ERR_RSA_VERIFY_FAILED;
            break;
        case CE_ERROR_INVAL_KEY:
            errno = MBEDTLS_ERR_RSA_KEY_CHECK_FAILED;
            break;
        case CE_ERROR_GEN_RANDOM:
            errno = MBEDTLS_ERR_RSA_RNG_FAILED;
            break;
        case CE_ERROR_OOM:
            errno = MBEDTLS_ERR_MPI_ALLOC_FAILED;
            break;
        default:
            errno = MBEDTLS_ERR_RSA_HW_ACCEL_FAILED;
            break;
    }

    return errno;
}
#if defined(CE_LITE_RSA_PKCS_V15) || defined(CE_LITE_RSA_PKCS_V21)
static ce_algo_t mbedtls_md_type_to_ce_hash_algo(mbedtls_md_type_t md_type)
{
    switch (md_type) {

#if defined(CE_LITE_SHA256)
    case MBEDTLS_MD_SHA256:
        return CE_ALG_SHA256;
#endif /* CE_LITE_SHA256 */

#if defined(CE_LITE_SHA224)
    case MBEDTLS_MD_SHA224:
        return CE_ALG_SHA224;
#endif /* CE_LITE_SHA224 */

#if defined(CE_LITE_SHA1)
    case MBEDTLS_MD_SHA1:
        return CE_ALG_SHA1;
#endif /* CE_LITE_SHA1 */

    default:
        return CE_ERROR_INVAL_ALG;
    }
}
#endif /* (CE_LITE_RSA_PKCS_V15) || (CE_LITE_RSA_PKCS_V21) */

static size_t mbedtls_md_type_to_ce_hash_size(mbedtls_md_type_t md_type)
{
    switch (md_type) {

#if defined(CE_LITE_SHA256)
    case MBEDTLS_MD_SHA256:
        return CE_SHA256_HASH_SIZE;
#endif /* CE_LITE_SHA256 */

#if defined(CE_LITE_SHA224)
    case MBEDTLS_MD_SHA224:
        return CE_SHA224_HASH_SIZE;
#endif /* CE_LITE_SHA224 */

#if defined(CE_LITE_SHA1)
    case MBEDTLS_MD_SHA1:
        return CE_SHA1_HASH_SIZE;
#endif /* CE_LITE_SHA1 */

    default:
        return 0;
    }
}

void mbedtls_rsa_init(mbedtls_rsa_context *ctx,
                      int padding,
                      int hash_id)
{
    int ret = CE_SUCCESS;

    if ((MBEDTLS_RSA_PKCS_V15 != padding) &&
        (MBEDTLS_RSA_PKCS_V21 != padding)) {
        PAL_ASSERT(0);
    }

    if ((MBEDTLS_MD_SHA256 != hash_id) &&
        (MBEDTLS_MD_SHA224 != hash_id) &&
        (MBEDTLS_MD_SHA1 != hash_id) &&
        (MBEDTLS_MD_NONE != hash_id)) {
        PAL_ASSERT(0);
    }

    (void)ret;
    PAL_ASSERT((sizeof(ce_rsa_context_t) + 2 * sizeof(int)) ==
                (sizeof(mbedtls_rsa_context)));

    ret = ce_rsa_init((ce_rsa_context_t *)ctx);
    PAL_ASSERT(CE_SUCCESS == ret);

    ctx->padding = padding;
    ctx->hash_id = hash_id;
}

void mbedtls_rsa_free(mbedtls_rsa_context *ctx)
{
    ce_rsa_free((ce_rsa_context_t *)ctx);
}

int mbedtls_rsa_import(mbedtls_rsa_context *ctx,
                       const mbedtls_mpi *N,
                       const mbedtls_mpi *P, const mbedtls_mpi *Q,
                       const mbedtls_mpi *D, const mbedtls_mpi *E)
{
    return _convert_retval_to_mbedtls(
            ce_rsa_import((ce_rsa_context_t *)ctx,
                          MPI2BN(N),
                          MPI2BN(P),
                          MPI2BN(Q),
                          MPI2BN(D),
                          MPI2BN(E)));
}

int mbedtls_rsa_import_raw(mbedtls_rsa_context *ctx,
                           unsigned char const *N, size_t N_len,
                           unsigned char const *P, size_t P_len,
                           unsigned char const *Q, size_t Q_len,
                           unsigned char const *D, size_t D_len,
                           unsigned char const *E, size_t E_len)
{
    return _convert_retval_to_mbedtls(
            ce_rsa_import_raw((ce_rsa_context_t *)ctx,
                              N,
                              N_len,
                              P,
                              P_len,
                              Q,
                              Q_len,
                              D,
                              D_len,
                              E,
                              E_len));
}

size_t mbedtls_rsa_get_len(const mbedtls_rsa_context *ctx)
{
    return ctx->len;
}

int mbedtls_rsa_check_pubkey(const mbedtls_rsa_context *ctx)
{
    return _convert_retval_to_mbedtls(
            ce_rsa_check_pubkey((const ce_rsa_context_t *)ctx));
}

#if defined(CE_LITE_RSASSA_SIGN)
int mbedtls_rsa_check_privkey(const mbedtls_rsa_context *ctx)
{
    return _convert_retval_to_mbedtls(
            ce_rsa_check_privkey((const ce_rsa_context_t *)ctx));
}

int mbedtls_rsa_check_pub_priv(const mbedtls_rsa_context *pub,
                               const mbedtls_rsa_context *prv)
{
    return _convert_retval_to_mbedtls(
            ce_rsa_check_pub_priv((const ce_rsa_context_t *)pub,
                                  (const ce_rsa_context_t *)prv));
}

int mbedtls_rsa_pkcs1_sign(mbedtls_rsa_context *ctx,
                           int (*f_rng)(void *, unsigned char *, size_t),
                           void *p_rng,
                           int mode,
                           mbedtls_md_type_t md_alg,
                           unsigned int hashlen,
                           const unsigned char *hash,
                           unsigned char *sig)
{
    if ((NULL == ctx) ||
        (MBEDTLS_RSA_PRIVATE != mode)) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    hashlen = mbedtls_md_type_to_ce_hash_size(md_alg);
    if (0 == hashlen) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    switch(ctx->padding)
    {
#if defined(CE_LITE_RSA_PKCS_V15)
        case MBEDTLS_RSA_PKCS_V15:
            return _convert_retval_to_mbedtls(
                    ce_rsa_rsassa_pkcs1_v15_sign((ce_rsa_context_t *)ctx,
                                                 (int32_t (*)(void *, uint8_t *, size_t))f_rng,
                                                 p_rng,
                                                 mbedtls_md_type_to_ce_hash_algo(md_alg),
                                                 hashlen,
                                                 hash,
                                                 sig));

#endif /* CE_LITE_RSA_PKCS_V15 */

#if defined(CE_LITE_RSA_PKCS_V21)
        case MBEDTLS_RSA_PKCS_V21:
            PAL_ASSERT(MBEDTLS_RSA_SALT_LEN_MAX == CE_RSA_SALT_LEN_MAX);
            if (MBEDTLS_MD_NONE == ctx->hash_id) {
                return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
            }
            return _convert_retval_to_mbedtls(
                    ce_rsa_rsassa_pss_sign((ce_rsa_context_t *)ctx,
                                           (int32_t (*)(void *, uint8_t *, size_t))f_rng,
                                           p_rng,
                                           hash,
                                           hashlen,
                                           mbedtls_md_type_to_ce_hash_algo(ctx->hash_id),
                                           MBEDTLS_RSA_SALT_LEN_MAX,
                                           sig));
#endif /* CE_LITE_RSA_PKCS_V21 */

        default:
            PAL_ASSERT(0);
            return(MBEDTLS_ERR_RSA_INVALID_PADDING);
    }
}
#endif /* CE_LITE_RSASSA_SIGN */

int mbedtls_rsa_pkcs1_verify(mbedtls_rsa_context *ctx,
                             int (*f_rng)(void *, unsigned char *, size_t),
                             void *p_rng,
                             int mode,
                             mbedtls_md_type_t md_alg,
                             unsigned int hashlen,
                             const unsigned char *hash,
                             const unsigned char *sig)
{
    if ((NULL == ctx) ||
        (MBEDTLS_RSA_PUBLIC != mode)) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    hashlen = mbedtls_md_type_to_ce_hash_size(md_alg);
    if (0 == hashlen) {
        return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
    }

    switch(ctx->padding)
    {
#if defined(CE_LITE_RSA_PKCS_V15)
        case MBEDTLS_RSA_PKCS_V15:
            return _convert_retval_to_mbedtls(
                    ce_rsa_rsassa_pkcs1_v15_verify((ce_rsa_context_t *)ctx,
                                                   mbedtls_md_type_to_ce_hash_algo(md_alg),
                                                   hashlen,
                                                   hash,
                                                   sig));
#endif /* CE_LITE_RSA_PKCS_V15 */

#if defined(CE_LITE_RSA_PKCS_V21)
        case MBEDTLS_RSA_PKCS_V21:
            PAL_ASSERT(MBEDTLS_RSA_SALT_LEN_ANY == CE_RSA_SALT_LEN_ANY);
            if (MBEDTLS_MD_NONE == ctx->hash_id) {
                return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
            }
            return _convert_retval_to_mbedtls(
                    ce_rsa_rsassa_pss_verify((ce_rsa_context_t *)ctx,
                                             hash,
                                             hashlen,
                                             mbedtls_md_type_to_ce_hash_algo(ctx->hash_id),
                                             MBEDTLS_RSA_SALT_LEN_ANY,
                                             sig));
#endif /* CE_LITE_RSA_PKCS_V21 */

        default:
            PAL_ASSERT(0);
            return(MBEDTLS_ERR_RSA_INVALID_PADDING);
    }

}
#endif /* CE_LITE_RSASSA */
