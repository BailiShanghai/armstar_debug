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
#include "ce_lite_sm2dsa.h"
#include "ce_lite_common.h"
#include "ce_lite_config.h"
#include "mbedtls/bignum.h"
#include "mbedtls/sm2dsa.h"
#include "mbedtls/platform.h"

#if defined(CE_LITE_SM2DSA)
extern int ce_err_to_mbedtls_ecp_err(int errno);

void mbedtls_sm2dsa_init(mbedtls_sm2dsa_context *ctx)
{
    int ret = CE_SUCCESS;

    (void)ret;
    PAL_ASSERT(sizeof(ce_sm2dsa_context_t) ==
                sizeof(mbedtls_sm2dsa_context));

    ret = ce_sm2dsa_init((ce_sm2dsa_context_t *)ctx);
    PAL_ASSERT(CE_SUCCESS == ret);
}

void mbedtls_sm2dsa_free(mbedtls_sm2dsa_context *ctx)
{
    ce_sm2dsa_free((ce_sm2dsa_context_t *)ctx);
}

int mbedtls_sm2dsa_from_keypair(mbedtls_sm2dsa_context *ctx,
                                const mbedtls_ecp_keypair *key)
{
    return ce_err_to_mbedtls_ecp_err(
            ce_sm2dsa_from_keypair((ce_sm2dsa_context_t *)ctx,
                                   (const ce_ecp_keypair_t *)key));
}

int mbedtls_sm2dsa_verify(const unsigned char *buf,
                          size_t blen,
                          const mbedtls_ecp_point *Q,
                          const mbedtls_mpi *r,
                          const mbedtls_mpi *s)
{
    if (CE_SM3_HASH_SIZE != blen) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    return ce_err_to_mbedtls_ecp_err(
            ce_sm2dsa_verify(buf,
                             blen,
                             (const ce_ecp_point_t *)Q,
                             MPI2BN(r),
                             MPI2BN(s)));
}

int mbedtls_sm2_compute_id_digest(mbedtls_md_type_t md_type,
                                  const mbedtls_ecp_point *Q,
                                  const char *id,
                                  size_t idlen,
                                  unsigned char *za)
{
    if (MBEDTLS_MD_SM3 != md_type) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    return ce_err_to_mbedtls_ecp_err(
            ce_sm2_compute_id_digest(CE_ALG_SM3,
                                     (const ce_ecp_point_t *)Q,
                                     (const uint8_t *)id,
                                     idlen,
                                     za));
}

int mbedtls_sm2dsa_read_signature(mbedtls_sm2dsa_context *ctx,
                                  const unsigned char *hash,
                                  size_t hlen,
                                  const unsigned char *sig,
                                  size_t slen)
{
    if (CE_SM3_HASH_SIZE != hlen) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    return ce_err_to_mbedtls_ecp_err(
            ce_sm2dsa_read_signature((const ce_sm2dsa_context_t *)ctx,
                                     hash,
                                     hlen,
                                     sig,
                                     slen));
}

#if defined(CE_LITE_SM2DSA_SIGN)
int mbedtls_sm2dsa_sign(mbedtls_mpi *r,
                        mbedtls_mpi *s,
                        const mbedtls_mpi *d,
                        const unsigned char *buf,
                        size_t blen,
                        int (*f_rng)(void *, unsigned char *, size_t),
                        void *p_rng)
{
    if (CE_SM3_HASH_SIZE != blen) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    return ce_err_to_mbedtls_ecp_err(
            ce_sm2dsa_sign(MPI2BN(d),
                           buf,
                           blen,
                           MPI2BN(r),
                           MPI2BN(s),
                           (int32_t (*)(void *, uint8_t *, size_t))f_rng,
                           p_rng));
}

int mbedtls_sm2dsa_write_signature(mbedtls_sm2dsa_context *ctx,
                                   mbedtls_md_type_t md_alg,
                                   const unsigned char *hash,
                                   size_t hlen,
                                   unsigned char *sig,
                                   size_t *slen,
                                   int (*f_rng)(void *,
                                                unsigned char *,
                                                size_t),
                                   void *p_rng)
{
    if (CE_SM3_HASH_SIZE != hlen) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    (void)md_alg;
    return ce_err_to_mbedtls_ecp_err(
            ce_sm2dsa_write_signature((const ce_sm2dsa_context_t *)ctx,
                                      hash,
                                      hlen,
                                      sig,
                                      slen,
                                      (int32_t (*)(void *, uint8_t *, size_t))f_rng,
                                      p_rng));
}
#endif /* CE_LITE_SM2DSA_SIGN */
#endif /* CE_LITE_SM2DSA */