
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

#ifndef SBAR_YAHOO
#define SBAR_YAHOO 
#ifdef sbar_bill
extern "C" {
#endif
#include "hal.h"
#include "pal.h"
#include "secure_boot_core.h"
#define S107(s108) ((void)s108)
#define s109 (0xFFFFFFFF)
#define s110 (20)
#define S111 (28)
#define S112 (32)
#define SBAR_TRICK (128)
#define s113 (256)
#define S99 (16)
#define S100 (32)
#define S47 (16)
#define SBAr_HinT (16)
#define s4 (128)
#define sdog (256)
#define sfOBAz (128)
#define scat (256)
#define SBAr_blacK (2 * (1 + 1 + 1 + (521 + 7) / 8) + 1 + 2)
#define SBAD (65)
#define s9 (133)
#define s8 (64)
#define sDAd (132)
#define S30(sbar_red) \
    (((sbar_red) == SBAR_FAST) \
         ? s110 \
         : (((sbar_red) == sbar_big) \
                ? S112 \
                : (((sbar_red) == SbAR_smaLL) \
                       ? S111 \
                       : 0)))
#define s31(sbaz) \
    ((((sbaz) == s1) || \
      ((sbaz) == s2) || \
      ((sbaz) == sfoobaz) || \
      ((sbaz) == SQUUX) || \
      ((sbaz) == SFISH) || \
      ((sbaz) == SgASp) || \
      ((sbaz) == sBuG) || \
      ((sbaz) == ssilly)) \
         ? (HAL_HASH_SCH_SHA1) \
         : ((((sbaz) == sfoobar) || \
             ((sbaz) == S3) || \
             ((sbaz) == S5) || \
             ((sbaz) == SFRED) || \
             ((sbaz) == S6) || \
             ((sbaz) == s7) || \
             ((sbaz) == SBUGGY) || \
             ((sbaz) == SMUM)) \
                ? (HAL_HASH_SCH_SHA256) \
                : (HAL_HASH_SCH_INVALID)))
#define S34(sbaz) \
    ((((sbaz) == s1) || \
      ((sbaz) == s2) || \
      ((sbaz) == sfoobar) || \
      ((sbaz) == S3)) \
         ? (HAL_CRYPTO_KEY_RSA_1024) \
         : ((((sbaz) == sfoobaz) || \
             ((sbaz) == SQUUX) || \
             ((sbaz) == S5) || \
             ((sbaz) == SFRED)) \
                ? (HAL_CRYPTO_KEY_RSA_2048) \
                : ((((sbaz) == SFISH) || \
                    ((sbaz) == SgASp) || \
                    ((sbaz) == S6) || \
                    ((sbaz) == s7)) \
                       ? (HAL_CRYPTO_KEY_ECP_NIST_256) \
                       : ((((sbaz) == sBuG) || \
                           ((sbaz) == \
                            ssilly) || \
                           ((sbaz) == SBUGGY) || \
                           ((sbaz) == \
                            SMUM)) \
                              ? (HAL_CRYPTO_KEY_ECP_NIST_521) \
                              : (HAL_CRYPTO_KEY_INVALID)))))
typedef uint8_t S25[S112];
typedef uint8_t S105[S47];
typedef uint8_t SCyaN[S100];
typedef int (*S74)(void);
typedef enum sbar_green {
    S72 = 0,
    SYahoO = 1,
    sbar_rms = 2,
    s95 = 3,
    S101 = 0xFF,
} s24;
typedef enum sbAR_YELLOw {
    S114 = 0,
    SBAR_FAST = 1,
    SbAR_smaLL = 2,
    sbar_big = 3,
} sbill;
typedef enum SBAR_BLUE {
    s48 = 0,
    S41 = 1,
    s43 = 2,
    sclinton = 3,
    S45 = 4,
    SWHITE = 5,
    S44 = 6,
    s42 = 7,
    s46 = 8,
} s39;
typedef enum SBAR_MAGENTA {
    S10 = 0,
    s1 = 1,
    sfoobar = 2,
    sfoobaz = 3,
    S5 = 4,
    SFISH = 5,
    S6 = 6,
    sBuG = 7,
    SBUGGY = 8,
    s2 = 9,
    S3 = 10,
    SQUUX = 11,
    SFRED = 12,
    SgASp = 13,
    s7 = 14,
    ssilly = 15,
    SMUM = 16,
} sbar;
typedef struct SBAr_CYan {
    uint8_t *S13;
    uint32_t sBAr_oK;
    uint32_t S59;
    uint32_t S85;
    bool S93;
} SbAR_baD;
#ifdef sbar_bill
}
#endif
#endif
