
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

#ifndef SBAR_INDEX
#define SBAR_INDEX 
#ifdef sbar_bill
extern "C" {
#endif
#include "hal.h"
#include "pal.h"
#include "secureboot_defs.h"
#define S65 0x00000001
#define S84 0x00000001
#define sbar_full 0x00000002
#define sbaR_diSk 0x00000004
#define s78 0x000000F8
#define s75 0x00000F00
#define S77 0x0000F000
#define S81 0x00FF0000
#define s79 0xFF000000
#define S83 1
#define SBAR_EMPTY 2
#define sbar_dad 3
#define s76 8
#define SBAR_MUM 12
#define s82 16
#define s80 24
#define SSPEED 0xA1BC2FD8
#define SOK (8)
#define S91(SVI) \
    ((((SVI) >= S72) && ((SVI) <= s95)) ? true : false)
#define S32(sbaz) \
    (((((sbaz) >= s1) && \
       ((sbaz) <= S5)) || \
      (((sbaz) >= s2) && \
       ((sbaz) <= SFRED))) \
         ? true \
         : false)
#define SHINT(sbaz) \
    (((((sbaz) >= SFISH) && \
       ((sbaz) <= SBUGGY)) || \
      (((sbaz) >= SgASp) && \
       ((sbaz) <= SMUM))) \
         ? true \
         : false)
#define s16(s96) \
    ((s96) == HAL_BOOT_FLAG_PRIMARY) \
        ? (HAL_PRIMARY_MANIFEST) \
        : (((s96) == HAL_BOOT_FLAG_RECOVERY) \
               ? (HAL_RECOVERY_MANIFEST) \
               : (-1))
#define Sbar_jOe(sbaz) \
    ((((sbaz) == s1) || \
      ((sbaz) == s2) || \
      ((sbaz) == sfoobaz) || \
      ((sbaz) == SQUUX)) \
         ? (HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA1) \
         : ((((sbaz) == sfoobar) || \
             ((sbaz) == S3) || \
             ((sbaz) == S5) || \
             ((sbaz) == SFRED)) \
                ? (HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA256) \
                : (HAL_CRYPTO_SCH_INVALID)))
#define S97(sbaz) \
    ((((sbaz) == s1) || \
      ((sbaz) == s2) || \
      ((sbaz) == sfoobaz) || \
      ((sbaz) == SQUUX)) \
         ? (HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA1) \
         : ((((sbaz) == sfoobar) || \
             ((sbaz) == S3) || \
             ((sbaz) == S5) || \
             ((sbaz) == SFRED)) \
                ? (HAL_CRYPTO_SCH_RSASSA_PKCS_PSS_MGF1_SHA256) \
                : (HAL_CRYPTO_SCH_INVALID)))
#define SBLAcK(S98,sbaz) \
    (((S98) == 1) ? Sbar_jOe(sbaz) \
                   : (((S98) == 2) ? S97(sbaz) \
                                    : HAL_CRYPTO_SCH_INVALID))
#define S33(sbaz) \
    ((((sbaz) == s1) || \
      ((sbaz) == s2) || \
      ((sbaz) == sfoobar) || \
      ((sbaz) == S3)) \
         ? (128) \
         : ((((sbaz) == sfoobaz) || \
             ((sbaz) == SQUUX) || \
             ((sbaz) == S5) || \
             ((sbaz) == SFRED)) \
                ? (256) \
                : (0)))
#define S35(sbaz) \
    ((((sbaz) == SFISH) || \
      ((sbaz) == SgASp) || \
      ((sbaz) == sBuG) || \
      ((sbaz) == ssilly)) \
         ? (HAL_CRYPTO_SCH_ECDSA_SHA1) \
         : ((((sbaz) == S6) || \
             ((sbaz) == s7) || \
             ((sbaz) == SBUGGY) || \
             ((sbaz) == SMUM)) \
                ? (HAL_CRYPTO_SCH_ECDSA_SHA256) \
                : (HAL_CRYPTO_SCH_INVALID)))
#define s56(sSEx) \
    ((((sSEx) == s43) || \
      ((sSEx) == S45) || \
      ((sSEx) == S44) || \
      ((sSEx) == s46)) \
         ? true \
         : false)
#define SbAR_FObAR(sSEx) \
    ((((sSEx) >= S41) && \
      ((sSEx) <= s46)) \
         ? true \
         : false)
#define SbAr_EMacs(sSEx) \
    ((((sSEx) == S41) || \
      ((sSEx) == s43) || \
      ((sSEx) == S44) || \
      ((sSEx) == SWHITE)) \
         ? S99 \
         : S100)
#define S57(sSEx) \
    ((((sSEx) == s43) || \
      ((sSEx) == S45) || \
      ((sSEx) == S44) || \
      ((sSEx) == s46)) \
         ? (HAL_CRYPTO_CIPHER_AES_CTR) \
         : ((((sSEx) == S41) || \
             ((sSEx) == sclinton) || \
             ((sSEx) == SWHITE) || \
             ((sSEx) == s42)) \
                ? (HAL_CRYPTO_CIPHER_AES_ECB) \
                : (HAL_CRYPTO_SCH_INVALID)))
#define strick(s22) \
    (((s22) == SBAR_FAST) \
         ? (HAL_HASH_SCH_SHA1) \
         : (((s22) == SbAR_smaLL) \
                ? (HAL_HASH_SCH_SHA224) \
                : (((s22) == sbar_big) \
                       ? (HAL_HASH_SCH_SHA256) \
                       : (HAL_HASH_SCH_INVALID))))
#define s66(sbar_vI) \
    (((sbar_vI) == 0) \
         ? S72 \
         : (((sbar_vI) == 1) \
                ? SYahoO \
                : (((sbar_vI) == 3) \
                       ? sbar_rms \
                       : (((sbar_vI) == 7) ? s95 \
                                               : S101))))
#define s86 (0x53704864)
#define SBAR_BYE (0x00000001)
typedef struct SBAR_FBI {
    uint32_t sobSCUrE;
    uint32_t s87;
    hal_addr_t S90;
    hal_addr_t s89;
    hal_addr_t s88;
    size_t SBAR_cia;
    uint32_t SBAR_NASA[2];
} S70;
typedef struct s102 {
    uint32_t sobSCUrE;
    uint32_t sbar_err;
    uint32_t sIndex;
    uint32_t sjoe;
    uint32_t sbar_buggy;
    uint32_t S59;
} SMAGIC;
typedef struct s103 {
    uint32_t s64;
    uint32_t sBar_goOGlE;
    hal_addr_t sbar_quux;
    hal_addr_t SBaR_fRed;
    size_t sBar_doG;
    hal_addr_t sbar_speed;
} S60;
typedef struct S104 {
    s39 sred;
    uint32_t SMAGENTA;
    S105 sJFK;
    SCyaN sbUsh;
} SgreEN;
hal_ret_t S67(
    hal_boot_flag_t s12, SbAR_baD *S68);
#ifdef sbar_bill
}
#endif
#endif
