
/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

 /* This is scrambled code, don't try to read it! */

#include "secureboot_internal.h"
#include "hal_config.h"
#include "secure_boot_config.h"
#if (defined(SECBOOT_ENABLE_IMG_ENC)) && (!(defined(CAL_ENABLE_AES)))
#error "secure boot image encryption requires AES"
#endif
static inline void sfoo(sbar sbaz,
                                                uint32_t *S0,
                                                uint32_t *sfobar)
{
    PAL_ASSERT((NULL != S0) && (NULL != sfobar));
    switch (sbaz) {
#if defined(CAL_ENABLE_RSA)
#if defined(CAL_ENABLE_SHA1)
    case s1:
    case s2:
#endif
#if defined(CAL_ENABLE_SHA256)
    case sfoobar:
    case S3:
#endif
#if defined(CAL_ENABLE_SHA1) || defined(CAL_ENABLE_SHA256)
        *sfobar = s4;
        *S0 = sfOBAz;
        break;
#endif
#if defined(CAL_ENABLE_SHA1)
    case sfoobaz:
    case SQUUX:
#endif
#if defined(CAL_ENABLE_SHA256)
    case S5:
    case SFRED:
#endif
#if defined(CAL_ENABLE_SHA1) || defined(CAL_ENABLE_SHA256)
        *sfobar = sdog;
        *S0 = scat;
        break;
#endif
#endif
#if defined(CAL_ENABLE_ECDSA)
#if defined(CAL_ENABLE_SHA1)
    case SFISH:
    case SgASp:
#endif
#if defined(CAL_ENABLE_SHA256)
    case S6:
    case s7:
#endif
#if defined(CAL_ENABLE_SHA1) || defined(CAL_ENABLE_SHA256)
        *sfobar = s8;
        *S0 = SBAD;
        break;
#endif
#if defined(CAL_ENABLE_SHA1)
    case sBuG:
    case ssilly:
#endif
#if defined(CAL_ENABLE_SHA256)
    case SBUGGY:
    case SMUM:
#endif
#if defined(CAL_ENABLE_SHA1) || defined(CAL_ENABLE_SHA256)
        *sfobar = sDAd;
        *S0 = s9;
        break;
#endif
#endif
    default:
        *sfobar = 0;
        *S0 = 0;
        if (S10 == sbaz) {
            PAL_LOG_ERR("Invalid sec boot sig sch!\n");
            PAL_ASSERT(0);
        }
        break;
    }
}
static hal_ret_t S11(hal_boot_flag_t s12,
                                                size_t size,
                                                uint8_t *S13)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    hal_addr_t sdisk = 0;
    size_t S15 = 0;
    if ((s12 != HAL_BOOT_FLAG_PRIMARY) &&
        (s12 != HAL_BOOT_FLAG_RECOVERY)) {
        return HAL_ERR_BAD_PARAM;
    }
    PAL_ASSERT(S13);
    S14 = hal_get_manifest_partition_info(
        s16(s12),
        &sdisk,
        &S15);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("hal_get_manifest_partition_info failed!\n");
        return S14;
    }
    S14 = hal_flash_read(sdisk, S13, size);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("hal_flash_read failed!\n");
        return S14;
    }
    return HAL_OK;
}
static hal_ret_t s17(
    hal_boot_flag_t s12, uint32_t *SEMPTY)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    hal_addr_t s18 = 0;
    size_t SFULL = 0;
    uint8_t *sfast = NULL;
    int32_t ssmall = 0;
    int32_t SBIG = 0;
    if ((s12 != HAL_BOOT_FLAG_PRIMARY) &&
        (s12 != HAL_BOOT_FLAG_RECOVERY)) {
        return HAL_ERR_BAD_PARAM;
    }
    PAL_ASSERT(SEMPTY);
    if (s12 == HAL_BOOT_FLAG_PRIMARY) {
        s18 = HAL_OTP_PRIM_MNFT_VER_OFFSET;
        SFULL = HAL_OTP_PRIM_MNFT_VER_SIZE;
    } else if (s12 == HAL_BOOT_FLAG_RECOVERY) {
        s18 = HAL_OTP_RECV_MNFT_VER_OFFSET;
        SFULL = HAL_OTP_RECV_MNFT_VER_SIZE;
    } else {
        PAL_ASSERT(0);
    }
    sfast = pal_malloc(SFULL);
    if (!sfast) {
        PAL_LOG_ERR("Malloc failed!\n");
        return HAL_ERR_OUT_OF_MEMORY;
    }
    pal_memset(sfast, 0, SFULL);
    S14 = hal_otp_read(s18, sfast, SFULL);
    if (S14 != HAL_OK) {
        PAL_LOG_ERR("hal_otp_read failed!\n");
        pal_free(sfast);
        return S14;
    }
    PAL_LOG_DUMP_BUF("Read OTP version data:", sfast, SFULL);
    *SEMPTY = 0;
    for (SBIG = 0; SBIG < SFULL; SBIG++) {
        for (ssmall = 0; ssmall < SOK; ssmall++) {
            if ((sfast[SBIG] >> ssmall) & 0x01) {
                (*SEMPTY)++;
            }
        }
    }
    pal_free(sfast);
    return HAL_OK;
}
static hal_ret_t SHELLO(
    hal_boot_flag_t s12, uint32_t SEMPTY)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    hal_addr_t s18 = 0;
    size_t SFULL = 0;
    uint8_t *sfast = NULL;
    int32_t ssmall = 0;
    int32_t SBIG = 0;
    if ((s12 != HAL_BOOT_FLAG_PRIMARY) &&
        (s12 != HAL_BOOT_FLAG_RECOVERY)) {
        return HAL_ERR_BAD_PARAM;
    }
    if (s12 == HAL_BOOT_FLAG_PRIMARY) {
        s18 = HAL_OTP_PRIM_MNFT_VER_OFFSET;
        SFULL = HAL_OTP_PRIM_MNFT_VER_SIZE;
    } else if (s12 == HAL_BOOT_FLAG_RECOVERY) {
        s18 = HAL_OTP_RECV_MNFT_VER_OFFSET;
        SFULL = HAL_OTP_RECV_MNFT_VER_SIZE;
    } else {
        PAL_ASSERT(0);
    }
    if (((SEMPTY + SOK - 1) / SOK) > SFULL) {
        PAL_LOG_ERR(
            "Write bad manifest version: %d, Exceed the maximal OTP size: %d\n",
            SEMPTY,SFULL*SOK);
        return HAL_ERR_GENERIC;
    }
    sfast = pal_malloc(SFULL);
    if (!sfast) {
        PAL_LOG_ERR("Malloc failed!\n");
        return HAL_ERR_OUT_OF_MEMORY;
    }
    pal_memset(sfast, 0, SFULL);
    for (SBIG = 0; SBIG < SFULL; SBIG++) {
        for (ssmall = 0; ssmall < SOK; ssmall++) {
            if (SEMPTY) {
                sfast[SBIG] |= (0x01 << ssmall);
                SEMPTY--;
            }
        }
    }
    PAL_LOG_DUMP_BUF("Write OTP version data:", sfast, SFULL);
    S14 = hal_otp_write(s18, sfast, SFULL);
    if (S14 != HAL_OK) {
        PAL_LOG_ERR("hal_otp_write failed!\n");
        pal_free(sfast);
        return S14;
    }
    pal_free(sfast);
    return HAL_OK;
}
static inline hal_ret_t SBYE(
    hal_boot_flag_t s12, SMAGIC *s19)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    uint32_t s20 = 0;
    PAL_ASSERT(NULL != s19);
    if ((s12 != HAL_BOOT_FLAG_PRIMARY) &&
        (s12 != HAL_BOOT_FLAG_RECOVERY)) {
        return HAL_ERR_BAD_PARAM;
    }
    S14 = S11(s12,
                                         sizeof(SMAGIC),
                                         (uint8_t *)s19);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("Read manifest from type: %d failed!\n", s12);
        return S14;
    }
    if (s19->sobSCUrE != SSPEED) {
        PAL_LOG_ERR("Magic Number incorrect\n");
        return HAL_ERR_GENERIC;
    }
    S14 = s17(s12, &s20);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("Read manifest version failed!\n");
        return S14;
    }
    if (s19->sIndex < s20) {
        PAL_LOG_ERR("Detect old version of manifest: version: %d < %d\n",
                    s19->sIndex,
                    s20);
        return HAL_ERR_GENERIC;
    }
    return HAL_OK;
}
static inline hal_ret_t S21(sbar sbaz,
                                              sbill s22,
                                              uint32_t s23,
                                              uint32_t sjoe,
                                              uint8_t *semacs,
                                              s24 SVI,
                                              bool *srms)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    uint32_t S0 = 0;
    uint32_t sfobar = 0;
    size_t SfbI = 0;
    uint8_t *Scia = NULL;
    S25 S26 = {0};
    S25 SNASA = {0};
    S25 SERR = {0};
    size_t S27 = 0;
#if defined(CAL_ENABLE_RSA) || defined(CAL_ENABLE_ECDSA)
    uint8_t *S28 = NULL;
    hal_crypto_key_t sgoogle = {0};
#endif
#if defined(CAL_ENABLE_RSA)
    const static uint8_t s29[] = {0x01, 0x00, 0x01};
#endif
    PAL_ASSERT(NULL != semacs);
    sfoo(sbaz, &S0, &sfobar);
    if (sjoe <
        (sfobar + S0 + sizeof(SMAGIC))) {
        PAL_LOG_ERR(
            "Invalid manifest size: %d, sig_size: %d, pubkey_size: %d\n",
            sjoe,
            sfobar,
            S0);
        return HAL_ERR_GENERIC;
    }
#if defined(CAL_ENABLE_RSA) || defined(CAL_ENABLE_ECDSA)
    S28 = &semacs[sjoe - sfobar];
#endif
    Scia = &semacs[sjoe - sfobar - S0];
    PAL_ASSERT(HAL_OTP_SEC_BOOT_PUBKEY_HASH_SIZE == sizeof(SNASA));
    S14 = hal_otp_read(HAL_OTP_SEC_BOOT_PUBKEY_HASH_OFFSET,
                       SNASA,
                       HAL_OTP_SEC_BOOT_PUBKEY_HASH_SIZE);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("hal_otp_read failed!\n");
        return S14;
    }
    pal_memset(S26, 0, sizeof(S26));
    if (0 ==
        pal_memcmp(S26, SNASA, HAL_OTP_SEC_BOOT_PUBKEY_HASH_SIZE)) {
        PAL_LOG_WARN("Secure Boot Pubkey Hash in OTP are blank!\n");
        if (SVI > SYahoO) {
            PAL_LOG_ERR("Bad OTP data!\n");
            return HAL_ERR_GENERIC;
        } else {
            PAL_LOG_WARN("Secure Boot is bypassed!\n");
            *srms = true;
            return HAL_OK;
        }
    }
    SfbI = S30(s22);
    S14 = hal_crypto_hash(strick(s22),
                          Scia,
                          S0,
                          S26,
                          &SfbI);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("hal_crypto_hash failed!\n");
        return S14;
    }
    if (SfbI != S30(s22)) {
        PAL_LOG_ERR("hal_crypto_hash return invalid digest size\n");
        return HAL_ERR_GENERIC;
    }
    if (pal_memcmp(S26, SNASA, SfbI) != 0) {
        PAL_LOG_DUMP_BUF("dgst_result", S26, SfbI);
        PAL_LOG_DUMP_BUF("dgst_otp", SNASA, SfbI);
        PAL_LOG_ERR("Verify pubkey hash failed!\n");
        return HAL_ERR_VRFY_FAILED;
    }
    PAL_LOG_INFO("Verify pubkey hash success\n");
    PAL_LOG_DEBUG("Signature scheme: 0x%x, Sig size: %d, Pkcs version: %d\n",
                  sbaz,
                  (unsigned int)sfobar,
                  s23);
    S27 = sizeof(SERR);
    S14 = hal_crypto_hash(s31(sbaz),
                          semacs,
                          (sjoe - sfobar),
                          SERR,
                          &S27);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("hal_crypto_hash failed!\n");
        return HAL_ERR_GENERIC;
    }
    if (!(S32(sbaz) || SHINT(sbaz))) {
        PAL_LOG_ERR("Invalid signature scheme: %d\n", sbaz);
        return HAL_ERR_GENERIC;
    }
    if (S32(sbaz)) {
#if defined(CAL_ENABLE_RSA)
        PAL_ASSERT(sfobar == S33(sbaz));
        sgoogle.key_type = S34(sbaz);
        sgoogle.rsa_pubkey.n = Scia;
        sgoogle.rsa_pubkey.n_size = S0;
        sgoogle.rsa_pubkey.e = (uint8_t *)s29;
        sgoogle.rsa_pubkey.e_size = sizeof(s29);
        S14 =
            hal_crypto_rsa_verify_digest(SBLAcK(s23, sbaz),
                                         &sgoogle,
                                         SERR,
                                         S27,
                                         S28,
                                         sfobar);
        if (HAL_OK != S14) {
            PAL_LOG_ERR("hal_crypto_rsa_verify_digest failed:0x%x\n", S14);
            return S14;
        }
#else
        PAL_LOG_ERR("RSA disabled but the scheme is configured in manifest\n");
        return HAL_ERR_GENERIC;
#endif
    }
    if (SHINT(sbaz)) {
#if defined(CAL_ENABLE_ECDSA)
        sgoogle.key_type = S34(sbaz);
        sgoogle.ec_pubkey.q = Scia;
        sgoogle.ec_pubkey.q_size = S0;
        S14 = hal_crypto_ecdsa_verify_digest(S35(sbaz),
                                             &sgoogle,
                                             SERR,
                                             S27,
                                             S28,
                                             sfobar / 2,
                                             &S28[sfobar / 2],
                                             sfobar / 2);
        if (HAL_OK != S14) {
            PAL_LOG_ERR("hal_crypto_ecdsa_verify_digest failed:0x%x\n", S14);
            return S14;
        }
#else
        PAL_LOG_ERR("ECDSA disabled but the scheme is configured in manifest\n");
        return HAL_ERR_GENERIC;
#endif
    }
    return S14;
}
#if defined(SECBOOT_ENABLE_IMG_ENC)
static inline hal_ret_t
s36(uint8_t *s37,
                         uint32_t s38,
                         s39 sred,
                         SgreEN *SYELLOW)
{
#if defined(CAL_ENABLE_AES)
    uint8_t *s40 = NULL;
#endif
#if defined(CAL_ENABLE_CIPHER_MODE_CTR)
    uint8_t *SBLUE = NULL;
#endif
    PAL_ASSERT((NULL != s37) && (NULL != SYELLOW));
    SYELLOW->sred = sred;
    pal_memcpy(&SYELLOW->SMAGENTA, s37 + s38, sizeof(uint32_t));
    if (SYELLOW->SMAGENTA > sizeof(SCyaN)) {
        PAL_LOG_ERR("Invalid keyblob size: 0x%x\n", SYELLOW->SMAGENTA);
        return HAL_ERR_GENERIC;
    }
    switch (sred) {
#if defined(CAL_ENABLE_CIPHER_MODE_ECB)
    case S41:
    case SWHITE:
    case sclinton:
    case s42:
        s40 = s37 + s38 + sizeof(uint32_t);
        pal_memcpy(
            SYELLOW->sbUsh, s40, SYELLOW->SMAGENTA);
        break;
#endif
#if defined(CAL_ENABLE_CIPHER_MODE_CTR)
    case s43:
    case S44:
    case S45:
    case s46:
        SBLUE = s37 + s38 + sizeof(uint32_t);
        pal_memcpy(SYELLOW->sJFK, SBLUE, S47);
        s40 = SBLUE + S47;
        pal_memcpy(
            SYELLOW->sbUsh, s40, SYELLOW->SMAGENTA);
        break;
#endif
    case s48:
    default:
        PAL_ASSERT(s48 != sred);
        return HAL_ERR_GENERIC;
    }
    return HAL_OK;
}
#endif
static hal_ret_t S49(sbill S50,
                                           uint8_t *s51,
                                           size_t size,
                                           uint8_t *s52)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    size_t SfbI = 0;
    S25 S26 = {0};
    SfbI = sizeof(S26);
    S14 = hal_crypto_hash(
        strick(S50), s51, size, S26, &SfbI);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("hal_crypto_hash failed!\n");
        return S14;
    }
    if (SfbI != S30(S50)) {
        PAL_LOG_ERR("hal_crypto_hash return invalid digest size: %d\n",
                    SfbI);
        return HAL_ERR_GENERIC;
    }
    if (pal_memcmp(S26, s52, SfbI) != 0) {
        PAL_LOG_ERR("Compare image hash failed\n");
        PAL_LOG_DUMP_BUF("Digest calculated:", S26, SfbI);
        PAL_LOG_DUMP_BUF("Digest in manifest:", s52, SfbI);
        return HAL_ERR_VRFY_FAILED;
    }
    return HAL_OK;
}
#if defined(SECBOOT_ENABLE_IMG_ENC)
static hal_ret_t s53(s39 sSEx,
                                        uint8_t *sbUsh,
                                        uint8_t *sJFK,
                                        uint32_t s54,
                                        uint32_t sbar_foo,
                                        uint8_t *sbar_bar,
                                        uint8_t *sbar_baz)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    hal_crypto_key_t sgoogle = {0};
    size_t s55 = 0;
    PAL_ASSERT(sbar_bar && sbUsh && sbar_baz);
    PAL_ASSERT(SbAR_FObAR(sSEx));
    PAL_ASSERT(!(s56(sSEx) && (NULL == sJFK)));
    sgoogle.key_type = HAL_CRYPTO_KEY_AES_BLOB;
    sgoogle.key_blob.key_blob = sbUsh;
    sgoogle.key_blob.key_blob_size = s54;
    s55 = sbar_foo;
    S14 = hal_crypto_aes(S57(sSEx),
                         &sgoogle,
                         true,
                         sJFK,
                         sbar_bar,
                         sbar_foo,
                         sbar_baz,
                         &s55);
    return S14;
}
#endif
static inline hal_ret_t
S58(sbill S50,
                             SgreEN *SYELLOW,
                             uint8_t *s37,
                             uint32_t *S59,
                             bool sbaR_FoOBaR)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    S60 Sbar_fOBaZ = {0};
    uint32_t s38 = 0;
    uint32_t s61 = 0;
    uint32_t ssmall = 0;
    uint8_t *s51 = NULL;
    uint8_t *S62 = NULL;
    uint8_t *sbar_foobaz = NULL;
    uint8_t s63 = 0;
    PAL_ASSERT((NULL != s37) && (NULL != SYELLOW) &&
               (NULL != S59));
    s61 = *S59;
    s38 = sizeof(SMAGIC);
    for (ssmall = 0; ssmall < s61; ssmall++) {
        S62 = (s37 + s38);
        s38 = s38 + sizeof(S60);
        sbar_foobaz = s37 + s38;
        *S59 = ssmall + 1;
        pal_memcpy(&Sbar_fOBaZ, S62, sizeof(S60));
        s63 = (Sbar_fOBaZ.s64 & S65);
        s38 = s38 + S30(S50);
        if (Sbar_fOBaZ.sbar_quux == Sbar_fOBaZ.SBaR_fRed) {
#if defined(SECBOOT_ENABLE_XIP)
            PAL_LOG_DEBUG("Image static address == Image load address\n");
            if (s63) {
                PAL_LOG_ERR("Bad manifest! Image %d is encrypted but "
                            "static_addr == load_addr\n",
                            ssmall);
                return HAL_ERR_GENERIC;
            }
            S14 = S49(
                S50,
                (uint8_t *)(Sbar_fOBaZ.sbar_quux),
                Sbar_fOBaZ.sBar_doG,
                sbar_foobaz);
            if (HAL_OK != S14) {
                PAL_LOG_ERR("Verify image %d (plaintext) failed!\n", ssmall);
                return S14;
            }
            PAL_LOG_INFO("Verify image %d (plaintext) success\n", ssmall);
#else
            PAL_LOG_ERR("!! image address config error when XIP disabled\n");
            return HAL_ERR_GENERIC;
#endif
        } else {
            s51 = (uint8_t *)Sbar_fOBaZ.SBaR_fRed;
            S14 = hal_flash_read(
                Sbar_fOBaZ.sbar_quux, s51, Sbar_fOBaZ.sBar_doG);
            if (HAL_OK != S14) {
                PAL_LOG_ERR("hal_flash_read failed!\n");
                return S14;
            }
            PAL_LOG_DEBUG("is_enc: 0x%x\n", s63);
            PAL_LOG_DEBUG("key_blob_exist: %d\n", sbaR_FoOBaR);
            PAL_LOG_DEBUG("img_size: 0x%x\n",
                          (unsigned int)Sbar_fOBaZ.sBar_doG);
            if (s63 && sbaR_FoOBaR) {
#if defined(SECBOOT_ENABLE_IMG_ENC)
                PAL_LOG_DEBUG("img_cipher_sch: 0x%x\n",
                              SYELLOW->sred);
                S14 = s53(SYELLOW->sred,
                                             SYELLOW->sbUsh,
                                             SYELLOW->sJFK,
                                             SYELLOW->SMAGENTA,
                                             Sbar_fOBaZ.sBar_doG,
                                             s51,
                                             s51);
                if (HAL_OK != S14) {
                    PAL_LOG_ERR("Decrypt image %d failed!\n", ssmall);
                    return S14;
                }
                PAL_LOG_INFO("Decrypt image %d success\n", ssmall);
#else
                PAL_LOG_ERR("!!img enc disabled but is_enc flag defined in maniefst\n");
                return HAL_ERR_GENERIC;
#endif
            }
            S14 = S49(
                    S50, s51, Sbar_fOBaZ.sBar_doG, sbar_foobaz);
            if (HAL_OK != S14) {
                PAL_LOG_ERR("Verify image %d failed!\n", ssmall);
                if (Sbar_fOBaZ.sBar_doG >= 32) {
                    PAL_LOG_DUMP_BUF("image first 16 data:", s51, 16);
                    PAL_LOG_DUMP_BUF("image last 16 data:",
                                     &s51[Sbar_fOBaZ.sBar_doG - 16],
                                     16);
                } else {
                    PAL_LOG_DUMP_BUF(
                        "image data:", s51, Sbar_fOBaZ.sBar_doG);
                }
                return S14;
            }
            PAL_LOG_INFO("Verify image %d success\n", ssmall);
        }
    }
    return S14;
}
static inline void sbar_cat(sbill S50,
                                          uint8_t *s37,
                                          uint32_t S59)
{
    S60 Sbar_fOBaZ = {0};
    uint32_t SfbI = 0;
    uint32_t s38 = 0;
    size_t ssmall = 0;
    uint8_t *S62 = NULL;
    if (!s37) {
        return;
    }
    s38 = sizeof(SMAGIC);
    for (ssmall = 0; ssmall < S59; ssmall++) {
        S62 = (s37 + s38);
        pal_memcpy(&Sbar_fOBaZ, S62, sizeof(S60));
        SfbI = S30(S50);
        s38 = s38 + sizeof(S60) + SfbI;
        if (Sbar_fOBaZ.sbar_quux != Sbar_fOBaZ.SBaR_fRed) {
            pal_memset(
                (uint8_t *)Sbar_fOBaZ.SBaR_fRed, 0, Sbar_fOBaZ.sBar_doG);
        }
    }
}
static hal_ret_t sbar_fish(s24 *SVI)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    uint8_t sbar_gasp[HAL_OTP_LCS_SIZE] = {0};
    S14 = hal_otp_read(HAL_OTP_LCS_OFFSET, sbar_gasp, HAL_OTP_LCS_SIZE);
    if (HAL_OK != S14) {
        return S14;
    }
    *SVI = s66(sbar_gasp[0]);
    return HAL_OK;
}
hal_ret_t S67(
    hal_boot_flag_t s12, SbAR_baD *S68)
{
    hal_ret_t S14 = HAL_ERR_GENERIC;
    uint32_t sjoe = 0;
#if (defined(SECBOOT_ENABLE_IMG_ENC)|| defined(SECBOOT_ENABLE_EXT_PROG))
    uint32_t s38 = 0;
#endif
    uint32_t S59 = 0;
    uint32_t SfbI = 0;
    uint32_t s20 = 0;
    uint32_t s23 = 0;
    SMAGIC s19 = {0};
    sbar sbaz = 0;
    sbill sbAr_buG = 0;
    sbill S50 = 0;
#if defined(SECBOOT_ENABLE_IMG_ENC)
    s39 sred = 0;
#endif
    bool sbaR_FoOBaR = false;
    bool s69 = false;
    SgreEN SYELLOW = {0};
#if defined(SECBOOT_ENABLE_EXT_PROG)
    S70 S71 = {0};
#endif
    s24 SVI = S72;
    bool S73 = false;
    uint8_t *s37 = NULL;
#if defined(SECBOOT_ENABLE_EXT_PROG)
    S74 sbar_silly = NULL;
#endif
    bool srms = false;
    if (NULL == S68) {
        PAL_LOG_ERR("Input sec_boot_buff_ctx is NULL!\n");
        return HAL_ERR_BAD_PARAM;
    }
    if ((s12 != HAL_BOOT_FLAG_PRIMARY) &&
        (s12 != HAL_BOOT_FLAG_RECOVERY)) {
        PAL_LOG_ERR("Boot type error!\n");
        return HAL_ERR_BAD_PARAM;
    }
    pal_memset(&SYELLOW, 0, sizeof(s39));
    S14 = SBYE(s12, &s19);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("Verify manifest header failed!\n");
        return S14;
    }
    sjoe = s19.sjoe;
    S59 = s19.S59;
    sbaz =
        (sbar)((s19.sbar_buggy & s75) >>
                             s76);
    s23 = ((s19.sbar_buggy & S77) >>
                SBAR_MUM);
    sbAr_buG = (sbill)(
        (s19.sbar_buggy & s78) >>
        sbar_dad);
#if defined(SECBOOT_ENABLE_IMG_ENC)
    sred = (s39)(
        (s19.sbar_buggy & s79) >>
        s80);
#endif
    S50 = (sbill)(
        (s19.sbar_buggy & S81) >>
        s82);
    sbaR_FoOBaR =
        (((s19.sbar_buggy & sbaR_diSk) >>
          SBAR_EMPTY)
             ? (true)
             : (false));
    s69 = (((s19.sbar_buggy & sbar_full) >>
                            S83)
                               ? (true)
                               : (false));
    S73 =
        ((s19.sbar_buggy & S84) ? (true) : (false));
    if (
#if defined(CAL_ENABLE_RSA)
        (sbaz != s1) &&
        (sbaz != sfoobar) &&
        (sbaz != sfoobaz) &&
        (sbaz != S5) &&
#endif
#if defined(CAL_ENABLE_ECDSA)
    (sbaz != SFISH) &&
        (sbaz != S6) &&
        (sbaz != sBuG) &&
        (sbaz != SBUGGY) &&
#endif
    1) {
        PAL_LOG_ERR("Bad Signature Scheme: 0x%x\n", sbaz);
        return HAL_ERR_GENERIC;
    }
#if defined(CAL_ENABLE_RSA)
    if ((sbaz == s1) ||
        (sbaz == sfoobar) ||
        (sbaz == sfoobaz) ||
        (sbaz == S5)) {
        if ((s23 != 1) && (s23 != 2)) {
            PAL_LOG_ERR("Bad PKCS version: 0x%x\n", s23);
            return HAL_ERR_GENERIC;
        }
    }
#endif
    if ((sbAr_buG != SBAR_FAST) &&
        (sbAr_buG != SbAR_smaLL) &&
        (sbAr_buG != sbar_big)) {
        PAL_LOG_ERR("Bad Pubkey Hash scheme: 0x%x\n", sbAr_buG);
        return HAL_ERR_GENERIC;
    }
    if ((S50 != SBAR_FAST) &&
        (S50 != SbAR_smaLL) &&
        (S50 != sbar_big)) {
        PAL_LOG_ERR("Bad Image Hash scheme: 0x%x\n", S50);
        return HAL_ERR_GENERIC;
    }
#if defined(SECBOOT_ENABLE_IMG_ENC)
    if ((sred != S41) &&
        (sred != s43) &&
        (sred != sclinton) &&
        (sred != S45) &&
        (sred != s48)) {
        PAL_LOG_ERR("Bad Image Cipher scheme: 0x%x\n", sred);
        return HAL_ERR_GENERIC;
    }
#endif
    S68->S13 = (uint8_t *)pal_malloc(sjoe);
    if (!S68->S13) {
        PAL_LOG_ERR("Malloc failed!\n");
        return HAL_ERR_OUT_OF_MEMORY;
    }
    S68->sBAr_oK = sjoe;
    s37 = S68->S13;
    S68->S59 = s19.S59;
    SfbI = S30(S50);
    S68->S85 = SfbI;
    S14 = S11(s12, sjoe, s37);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("Read manifest failed!\n");
        S14 = HAL_ERR_GENERIC;
        goto sBar_helLO;
    }
#if (defined(SECBOOT_ENABLE_IMG_ENC) || defined(SECBOOT_ENABLE_EXT_PROG))
    s38 = sizeof(SMAGIC) +
             S59 * (sizeof(S60) + SfbI);
#endif
    if (s69) {
#if defined(SECBOOT_ENABLE_EXT_PROG)
        pal_memcpy(
            (uint8_t *)&S71, s37 + s38, sizeof(S71));
        if (!((S71.sobSCUrE == s86) &&
              (S71.s87 == SBAR_BYE))) {
            PAL_LOG_ERR("Bad Extended program: magic: 0x%x, version: 0x%x\n",
                        S71.sobSCUrE,
                        S71.s87);
            S14 = HAL_ERR_GENERIC;
            goto sBar_helLO;
        }
        if ((S71.s88 >= S71.s89) ||
            (S71.s88 < S71.S90) ||
            (S71.s88 & 0x1)) {
            PAL_LOG_ERR("Bad Extended program: entry: 0x%x\n",
                        S71.s88);
            S14 = HAL_ERR_GENERIC;
            goto sBar_helLO;
        }
        if ((S71.S90 & 0x1) ||
            (S71.s89 & 0x1) ||
            (S71.s89 <=
             S71.S90)) {
            PAL_LOG_ERR(
                "Bad Extended program: start_addr: 0x%x, end_addr: 0x%x\n",
                S71.S90,
                S71.s89);
            S14 = HAL_ERR_GENERIC;
            goto sBar_helLO;
        }
        s38 = s38 + sizeof(S71);
        pal_memcpy((uint8_t *)S71.S90,
                   s37 + s38,
                   S71.s89 -
                       S71.S90);
        s38 = s38 + S71.s89 - S71.s88;
#else
        PAL_LOG_ERR("!!Ext prog disabled but Ext prog flag defined in maniefst\n");
        S14 = HAL_ERR_GENERIC;
        goto sBar_helLO;
#endif
    }
    if (sbaR_FoOBaR) {
#if defined(SECBOOT_ENABLE_IMG_ENC)
        S14 = s36(
            s37, s38, sred, &SYELLOW);
        if (S14 != HAL_OK) {
            PAL_LOG_ERR("Cipher keyblob exist but get info failed!\n");
            S14 = HAL_ERR_GENERIC;
            goto sBar_helLO;
        }
        if (!SbAR_FObAR(SYELLOW.sred)) {
            PAL_LOG_ERR("Bad image cipher scheme: 0x%x\n",
                        SYELLOW.sred);
            S14 = HAL_ERR_GENERIC;
            goto sBar_helLO;
        }
#else
        PAL_LOG_ERR("!!img enc disabled but key_blob_exist defined in maniefst\n");
        S14 = HAL_ERR_GENERIC;
        goto sBar_helLO;
#endif
    }
    S14 = sbar_fish(&SVI);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("Read lifecycle failed!\n");
        goto sBar_helLO;
    }
    if (!S91(SVI)) {
        PAL_LOG_ERR("Life Cycle is invalid\n");
        S14 = HAL_ERR_GENERIC;
        goto sBar_helLO;
    }
    PAL_LOG_INFO(
        "Read lifecycle: %s\n",
        (SVI == 0)
            ? ("CM")
            : ((SVI == 1)
                   ? ("DM")
                   : ((SVI == 2) ? ("DD")
                                 : ((SVI == 3) ? ("DR") : ("Invalid")))));
    if (S73) {
        S14 = S21(sbaz,
                                    sbAr_buG,
                                    s23,
                                    sjoe,
                                    s37,
                                    SVI,
                                    &srms);
        if (HAL_OK != S14) {
            PAL_LOG_ERR("Verify Manifest failed!\n");
            goto sBar_helLO;
        }
        if (srms){
            PAL_LOG_ERR("Verify Manifest Skipped!\n");
        } else {
            PAL_LOG_ERR("Verify Manifest SUCCESS!\n");
        }
    } else {
        if (SVI > SYahoO) {
            PAL_LOG_ERR(
                "Secure boot can't be disabled when lifecycle > DM !\n");
            S14 = HAL_ERR_GENERIC;
            goto sBar_helLO;
        } else {
            PAL_LOG_WARN("Secure Boot is bypassed!\n");
            srms = true;
        }
    }
    S14 = S58(S50,
                                       &SYELLOW,
                                       s37,
                                       &S59,
                                       sbaR_FoOBaR);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("Load and Verify Images failed!\n");
        goto sBar_helLO;
    }
    if (s69) {
#if defined(SECBOOT_ENABLE_EXT_PROG)
        S71.s88 = S71.s88 + 1;
        PAL_LOG_INFO("Extended Program Entry: 0x%x\n",
                     (unsigned int)S71.s88);
        sbar_silly = (S74)S71.s88;
        PAL_LOG_INFO("Start to execute extended program...\n");
        sbar_silly();
        PAL_LOG_INFO("Extended program finish.\n");
#else
        PAL_LOG_ERR("!!Ext prog disabled but Ext prog flag defined in maniefst\n");
        S14 = HAL_ERR_GENERIC;
        goto sBar_helLO;
#endif
    }
    S14 = s17(s12, &s20);
    if (HAL_OK != S14) {
        PAL_LOG_ERR("Read manifest version failed!\n");
        goto sBar_helLO;
    }
    if (s19.sIndex > s20) {
        PAL_LOG_INFO("Version in Manifest: 0x%x. Version in OTP:0x%x\n",
                     (unsigned int)s19.sIndex,
                     (unsigned int)s20);
        if (!srms) {
            S14 = SHELLO(
                s12, s19.sIndex);
            if (HAL_OK != S14) {
                PAL_LOG_ERR("Advance manifest to version: %d failed\n",
                            s19.sIndex);
                goto sBar_helLO;
            }
        } else {
            PAL_LOG_WARN("Skip advancing manifest version!\n");
        }
    }
sBar_helLO:
    if (HAL_OK != S14) {
        sbar_cat(S50, s37, S59);
        pal_free(S68->S13);
        S68->sBAr_oK = 0;
    }
    return S14;
}
