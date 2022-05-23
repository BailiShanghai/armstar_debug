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

#include "ce_lite_bn.h"
#include "ce_lite_ecp.h"
#include "ce_lite_ecdsa.h"
#include "ce_lite_common.h"
#include "ce_lite_config.h"
#include "mbedtls/bignum.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/platform.h"

#if defined(CE_LITE_ECDSA)
extern int ce_err_to_mbedtls_ecp_err(int errno);

void mbedtls_ecdsa_init(mbedtls_ecdsa_context *ctx)
{
    int ret = CE_SUCCESS;

    (void)ret;
    PAL_ASSERT(sizeof(ce_ecdsa_context_t) ==
                sizeof(mbedtls_ecdsa_context));

    ret = ce_ecdsa_init((ce_ecdsa_context_t *)ctx);
    PAL_ASSERT(CE_SUCCESS == ret);
}

void mbedtls_ecdsa_free( mbedtls_ecdsa_context *ctx )
{
    ce_ecdsa_free((ce_ecdsa_context_t *)ctx);
}

int mbedtls_ecdsa_from_keypair(mbedtls_ecdsa_context *ctx,
                               const mbedtls_ecp_keypair *key)
{
    return ce_err_to_mbedtls_ecp_err(
            ce_ecdsa_from_keypair((ce_ecdsa_context_t *)ctx,
                                  (const ce_ecp_keypair_t *)key));
}

#if defined(CE_LITE_ECDSA_SIGN)
int mbedtls_ecdsa_sign(mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                       const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                       int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    if (CE_SHA256_HASH_SIZE != blen &&
        CE_SHA1_HASH_SIZE != blen &&
        CE_SHA224_HASH_SIZE != blen) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    return ce_err_to_mbedtls_ecp_err(
            ce_ecdsa_sign((const ce_ecp_group_t *)grp,
                          MPI2BN(d),
                          buf,
                          blen,
                          MPI2BN(r),
                          MPI2BN(s),
                          (int32_t (*)(void *, uint8_t *, size_t))f_rng,
                          p_rng));
}

int mbedtls_ecdsa_write_signature(mbedtls_ecdsa_context *ctx,
                                  mbedtls_md_type_t md_alg,
                                  const unsigned char *hash, size_t hlen,
                                  unsigned char *sig, size_t *slen,
                                  int (*f_rng)(void *, unsigned char *, size_t),
                                  void *p_rng)
{
    if (CE_SHA256_HASH_SIZE != hlen &&
        CE_SHA1_HASH_SIZE != hlen &&
        CE_SHA224_HASH_SIZE != hlen) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    (void)md_alg;
    return ce_err_to_mbedtls_ecp_err(
            ce_ecdsa_write_signature((ce_ecdsa_context_t *)ctx,
                                     hash,
                                     hlen,
                                     sig,
                                     slen,
                                     (int32_t (*)(void *, uint8_t *, size_t))f_rng,
                                     p_rng));
}
#endif /* CE_LITE_ECDSA_SIGN */

int mbedtls_ecdsa_verify(mbedtls_ecp_group *grp,
                         const unsigned char *buf, size_t blen,
                         const mbedtls_ecp_point *Q, const mbedtls_mpi *r,
                         const mbedtls_mpi *s)
{
    if (CE_SHA256_HASH_SIZE != blen &&
        CE_SHA1_HASH_SIZE != blen &&
        CE_SHA224_HASH_SIZE != blen) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    return ce_err_to_mbedtls_ecp_err(
            ce_ecdsa_verify((const ce_ecp_group_t *)grp,
                            buf,
                            blen,
                            (const ce_ecp_point_t *)Q,
                            MPI2BN(r),
                            MPI2BN(s)));
}

int mbedtls_ecdsa_read_signature(mbedtls_ecdsa_context *ctx,
                                 const unsigned char *hash, size_t hlen,
                                 const unsigned char *sig, size_t slen)
{
    if (CE_SHA256_HASH_SIZE != hlen &&
        CE_SHA1_HASH_SIZE != hlen &&
        CE_SHA224_HASH_SIZE != hlen) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    return ce_err_to_mbedtls_ecp_err(
            ce_ecdsa_read_signature((ce_ecdsa_context_t *)ctx,
                                    hash,
                                    hlen,
                                    sig,
                                    slen));
}
#endif /* CE_LITE_ECDSA */