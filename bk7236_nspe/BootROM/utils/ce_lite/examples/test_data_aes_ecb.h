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
#ifndef MBEDLTS_TEST_DATA_AES_ECB_H
#define MBEDLTS_TEST_DATA_AES_ECB_H

#include "ce_lite_config.h"

#if defined(CE_LITE_AES) && defined(CE_LITE_CIPHER_MODE_ECB)

static const unsigned char aes_test_ecb_enc[16] = {
        0xC3,0x4C,0x05,0x2C,0xC0,0xDA,0x8D,0x73,0x45,0x1A,0xFE,0x5F,0x03,0xBE,0x29,0x7F
};

static const unsigned char aes_test_ecb_key[16] = {
        0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C
};

static const unsigned char aes_test_ecb_dec[16] = {
        0x55,0x5c,0x82,0x24,0xe2,0xc9,0x58,0x73,0x12,0x08,0x5b,0xfb,0x4e,0xcb,0x04,0xb8
};

#if defined(CE_LITE_OTP) &&  \
    defined(CE_LITE_OTP_NORMAL) && \
    defined(CE_LITE_OTP_WRITE)

static const unsigned char aes_test_ecb_keyladder_enc[16] = {
    0x2F,0xDE,0x50,0xC3,0x6B,0x03,0xD0,0xC3,0x00,0xC2,0xE6,0xDC,0x9F,0x4D,0xE6,0x3E
};

static const unsigned char aes_test_ecb_ek3[16] = {
    0x50,0xCF,0x0F,0x29,0xD1,0xCF,0x64,0x41,0xC5,0xEA,0xAC,0xDB,0xDD,0x9A,0xFC,0xF4
};

static const unsigned char aes_test_ecb_ek2[16] = {
    0x9C,0x82,0x04,0x57,0xB7,0x17,0xD9,0x4A,0x55,0x74,0x2F,0x30,0x34,0x16,0x3B,0x52
};

static const unsigned char aes_test_ecb_ek1[16] = {
    0xF5,0xA0,0x33,0x7B,0x4B,0xE8,0x41,0x84,0x51,0x4E,0x08,0x86,0x5D,0x08,0xBB,0x6E
};

static const unsigned char aes_test_ecb_keyladder_dec[16] = {
    0xB1,0xC7,0x82,0x33,0xD7,0x7F,0xC1,0x87,0xB3,0x5F,0x67,0x51,0xDE,0x1F,0xB4,0x39
};

#endif /* CE_LITE_OTP && CE_LITE_OTP_NORMAL && CE_LITE_OTP_WRITE */

static const unsigned char aes_test_ecb_ext_enc[32] = {
    0xaf,0x4b,0x21,0x8b,0x39,0x5b,0xad,0xa6,0x35,0x6f,0x52,0x60,0xde,0x24,0x84,0xaa,
    0x9a,0x84,0x52,0x64,0xf1,0x69,0x60,0xfe,0x8c,0xd6,0x24,0xd2,0x3d,0x34,0x98,0x57
};

static const unsigned char aes_test_ecb_ext_key[16] = {
    0xe1,0xc3,0x23,0x24,0xa9,0x59,0xac,0xea,0xe5,0x3d,0x83,0x6d,0x9b,0x1e,0x56,0x9f
};

static const unsigned char aes_test_ecb_ext_dec[32] = {
    0xee,0x70,0xba,0x02,0x38,0x6c,0xde,0xaf,0xe4,0x68,0x9c,0x4c,0xb1,0xc7,0x03,0x1f,
    0x42,0x4f,0x99,0x05,0x4c,0x8d,0xb3,0x33,0x62,0xc8,0x8f,0x59,0x07,0xbb,0xef,0x23
};

#if defined(CE_LITE_CIPHER_PADDING_PKCS7)

static const unsigned char aes_test_ecb_pkcs7_enc[30] = {
    0x7a,0xd5,0x18,0xf2,0x2e,0xbe,0xe5,0x48,0xb8,0xa8,0xb3,0x86,0xde,0x5b,0x17,0x15,
    0x4f,0x27,0x9b,0xba,0x80,0x6e,0x05,0xaf,0x34,0x2c,0x73,0x99,0x13,0x71
};

static const unsigned char aes_test_ecb_pkcs7_key[16] = {
    0xc7,0xd3,0xae,0xec,0x69,0x96,0x4c,0xa8,0x1a,0xeb,0x0d,0xb9,0x2e,0x63,0xaa,0x79
};

static const unsigned char aes_test_ecb_pkcs7_dec_pad[32] = {
    0xad,0xe8,0xe1,0xaf,0x3c,0x40,0xed,0x6a,0xe1,0x8b,0xb3,0x69,0x61,0x19,0x14,0x8a,
    0x02,0xce,0x22,0x4e,0xa3,0x8e,0x63,0x45,0xd5,0xcc,0x85,0x8a,0xa9,0x82,0x3f,0xe6
};

#endif /* CE_LITE_CIPHER_PADDING_PKCS7 */

#endif /* CE_LITE_AES && CE_LITE_CIPHER_MODE_ECB */

#endif /* MBEDLTS_TEST_DATA_AES_ECB_H */
