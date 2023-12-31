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
#ifndef MBEDTLS_TEST_DATA_AES_CBC_H
#define MBEDTLS_TEST_DATA_AES_CBC_H

#include "ce_lite_config.h"

#if defined(CE_LITE_AES) && defined(CE_LITE_CIPHER_MODE_CBC)

static const unsigned char aes_test_cbc_enc[256] = {
    0x13,0x9e,0x26,0xaf,0xc5,0x72,0x44,0xbc,0x6d,0x78,0x50,0x66,0x2f,0x66,0x8f,0x8e,
    0x4f,0xa0,0x34,0x03,0x7c,0x72,0x20,0x46,0x12,0xbd,0x7b,0x74,0xbe,0xf7,0x38,0x11,
    0x9d,0xe6,0x03,0x8b,0x4f,0xcc,0x42,0x16,0xa7,0xd0,0x8d,0x9b,0x7d,0x9e,0x10,0x36,
    0x9d,0x38,0x35,0x76,0x31,0xa3,0x23,0x54,0x74,0x1e,0xc1,0x16,0xd3,0x18,0x59,0xfb,
    0xdf,0x2a,0x7b,0xe4,0x2b,0x0d,0xd3,0xa0,0xb2,0x0f,0x9a,0xe9,0x7e,0xc8,0x0e,0x1e,
    0x13,0xea,0x6a,0x20,0xb9,0x0f,0x68,0x06,0xe4,0xb7,0xad,0x7d,0xca,0xb1,0x83,0x10,
    0xa2,0x9e,0x9f,0xd8,0x39,0x47,0x8f,0x7a,0x8f,0x70,0x57,0xbd,0x90,0xef,0xec,0x5f,
    0xb4,0x1e,0x62,0xe8,0xd6,0x35,0xc5,0x87,0x52,0x27,0x94,0xcd,0xe4,0x53,0xeb,0xb5,
    0xa2,0xd9,0x28,0x61,0x34,0x43,0xab,0x5a,0xd9,0xc9,0x38,0x50,0xba,0x35,0x0c,0x4c,
    0x8c,0xd7,0xc7,0xaa,0x79,0x2f,0x0d,0x00,0x27,0x90,0x08,0x04,0x50,0xbe,0xd8,0x7b,
    0x92,0x08,0x9b,0xb7,0x6d,0xe1,0xc2,0x2e,0x13,0xce,0xbd,0xa3,0xd5,0x22,0x46,0xb9,
    0x27,0xee,0x57,0x28,0xe8,0x7a,0x27,0x2f,0x3c,0x2e,0xbe,0xd0,0xfa,0xd1,0xad,0x91,
    0xb4,0xb4,0x2c,0x43,0xce,0x45,0xc0,0xdb,0x73,0x44,0x65,0x57,0xb4,0x4c,0xda,0x4a,
    0xbb,0xc6,0x25,0x8c,0x5f,0x7a,0x24,0xd5,0xac,0xc4,0xc3,0x0a,0xcb,0x7d,0x7e,0x48,
    0x04,0x40,0xba,0x33,0x79,0xca,0x50,0x1d,0x4f,0xf5,0xbd,0x8e,0x4b,0xee,0xef,0xe6,
    0xcd,0x00,0xe7,0x3f,0xd9,0x65,0xd0,0xcc,0x60,0x27,0x80,0x7b,0xe3,0x7e,0x07,0x85
};

static const unsigned char aes_test_cbc_key[16] = {
    0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C
};

static const unsigned char aes_test_cbc_iv[16] = {
    0x55,0x5c,0x82,0x24,0xe2,0xc9,0x58,0x73,0x12,0x08,0x5b,0xfb,0x4e,0xcb,0x04,0xb8
};

static const unsigned char aes_test_cbc_dec[256] = {
    0xb0,0x6a,0x73,0x0c,0x8d,0x5a,0x62,0x73,0x91,0x86,0x65,0x0c,0x96,0xfe,0x67,0x34,
    0x50,0x5f,0x0c,0x11,0x4d,0xe7,0xf0,0xdb,0x0d,0xed,0xbd,0xb6,0xd5,0x90,0xf5,0x93,
    0xe1,0xfb,0x76,0x7e,0xc3,0xdf,0x45,0xa0,0x8c,0x18,0xcf,0xee,0x77,0x9d,0xbe,0x72,
    0xdc,0xe8,0xc8,0x4b,0xb3,0x3a,0xec,0xf3,0x4e,0xdc,0xd6,0xc8,0x63,0x11,0xfd,0xd4,
    0x69,0xbc,0x8c,0xfe,0xf4,0x9b,0xad,0x45,0x4d,0x3e,0x63,0x49,0xce,0xe7,0x81,0x7a,
    0x62,0x58,0xcf,0xc7,0x74,0xc2,0x41,0x12,0x61,0x7b,0xb7,0x22,0xd8,0x15,0x1d,0x70,
    0x9f,0x06,0x5c,0x79,0x07,0x38,0xdb,0x16,0xef,0x70,0x3e,0x59,0x7c,0x09,0x40,0x74,
    0x46,0x63,0x8c,0x58,0xe2,0x57,0xff,0x67,0x55,0x56,0x87,0x1a,0x56,0x22,0x5a,0x2f,
    0xb8,0x16,0x91,0xf3,0x04,0x9e,0x47,0xb7,0xef,0x71,0x39,0xd8,0x30,0xec,0x62,0xaa,
    0x0b,0xeb,0x7e,0xba,0xd0,0x76,0x2a,0x6e,0xed,0xa8,0x7f,0x87,0x16,0xa2,0xda,0x9f,
    0xb4,0x70,0xdc,0xa4,0x1f,0xc5,0x20,0x47,0x5f,0x41,0xf0,0x3e,0x6a,0xdc,0xc4,0x75,
    0xcf,0x11,0x88,0x78,0x98,0x3b,0x4e,0x5e,0xc0,0x25,0x0e,0xe6,0xfc,0xeb,0x0a,0xaa,
    0x22,0x9c,0x39,0xca,0x16,0x80,0x38,0x6a,0xcc,0x91,0x81,0x50,0x16,0x99,0x94,0x0d,
    0x32,0x56,0x6b,0x92,0x92,0x5e,0x52,0xb8,0x6e,0x4b,0x79,0x3f,0x79,0x87,0x21,0xdd,
    0x4e,0xa9,0xf2,0x84,0x7a,0xe9,0xf4,0x3d,0x4e,0xc7,0x46,0x6d,0x3d,0x3c,0x88,0x4e,
    0xa0,0x7d,0xda,0xd8,0x06,0x1a,0x35,0xa5,0x42,0x50,0x5a,0xdf,0x5d,0x43,0x36,0x3e
};

#if defined(CE_LITE_CIPHER_PADDING_PKCS7)
static const unsigned char aes_test_cbc_pkcs7_enc[254] = {
    0x8e,0xe2,0x06,0x64,0x5c,0x87,0x92,0xb4,0xf9,0xb3,0x50,0xa1,0x45,0x16,0x33,0x99,
    0xf3,0x6e,0x05,0x50,0x82,0x2c,0xa8,0x59,0x93,0x04,0x43,0x4b,0xc2,0xc2,0x9a,0x77,
    0xc8,0x11,0x55,0xfc,0x1f,0xe6,0x30,0xc0,0x90,0xb3,0x42,0xef,0x83,0x70,0xf1,0x73,
    0xa7,0x5e,0x85,0x21,0x7a,0x37,0x65,0x3e,0x53,0x15,0x1e,0x8e,0x2d,0xe1,0x6c,0xaf,
    0xce,0xd1,0x65,0x5e,0xb5,0x9d,0x6e,0xa6,0x62,0x96,0xc3,0x19,0xd1,0x71,0xce,0xb5,
    0x13,0x69,0x7b,0xc8,0x7f,0x3f,0x85,0x4a,0x63,0xca,0x96,0xdf,0x3a,0x10,0x47,0xa3,
    0x67,0x5a,0x2e,0xa0,0x00,0x85,0x4b,0xc2,0x80,0xbc,0xef,0x12,0x8a,0x60,0x5d,0x6a,
    0x5d,0x88,0x25,0x74,0xfc,0x6c,0xff,0x61,0x1f,0xdc,0xe8,0xa0,0xc0,0x24,0xae,0xdb,
    0x50,0xe9,0x37,0x30,0x31,0x0d,0x5d,0xed,0x5f,0xdf,0xb9,0x0f,0x3e,0xb0,0x1b,0xd0,
    0x25,0x7d,0x09,0x52,0xf1,0xf5,0xd5,0xa3,0xbc,0x87,0x4f,0x70,0x0d,0xf0,0x67,0xa5,
    0xdc,0x5c,0x6d,0x9d,0xd4,0x82,0xf3,0xff,0x65,0x65,0xa6,0xe5,0x18,0xdd,0x19,0x62,
    0x14,0x0e,0x85,0x62,0xdf,0x2f,0xfd,0xaf,0xe7,0x42,0x36,0xe4,0xf4,0xc8,0x85,0x18,
    0x46,0x47,0xd9,0x18,0x7b,0xd5,0x60,0xe5,0x31,0x4c,0xf6,0x4c,0x99,0xda,0x72,0xed,
    0x12,0x45,0xf4,0x83,0x83,0xca,0x0e,0xeb,0x22,0x44,0xbc,0x77,0x0f,0x13,0x8f,0x0c,
    0x27,0xeb,0x0c,0x1c,0xcd,0x0a,0x74,0xf4,0x3a,0xc7,0x31,0xa7,0xca,0xd4,0xe3,0x83,
    0x63,0x03,0xe4,0x7e,0xc8,0x71,0x5e,0x58,0xcd,0xea,0xbb,0x59,0x9d,0xd8
};

static const unsigned char aes_test_cbc_pkcs7_key[16] = {
    0xc8,0x46,0x78,0xbd,0x1f,0xe6,0x5b,0x46,0xe2,0x72,0xd8,0x99,0x03,0xdf,0xe2,0xa0
};

static const unsigned char aes_test_cbc_pkcs7_iv[16] = {
    0x6f,0xba,0x4e,0x2e,0x64,0x50,0xdd,0xa4,0xa0,0x3d,0x4e,0xa1,0xd1,0x8a,0x61,0x3a
};

static const unsigned char aes_test_cbc_pkcs7_dec_pad[256] = {
    0x1b,0xea,0x46,0x4a,0x1c,0xad,0xf4,0x93,0x3a,0x93,0x2b,0xb3,0xa5,0x24,0x9b,0xfa,
    0xa1,0xe6,0x0f,0x60,0x47,0x61,0xb7,0x26,0xde,0x9b,0x0c,0x58,0xb7,0x13,0x08,0xe8,
    0xff,0x43,0x80,0xc8,0x42,0xd1,0x9a,0xfd,0x76,0x04,0xea,0xcb,0xf8,0xb3,0x1d,0xaa,
    0x6b,0x87,0xe9,0x41,0x43,0x53,0x1e,0xf8,0x0c,0x96,0x89,0x6f,0xba,0xbe,0xc8,0x04,
    0xc7,0x3d,0xc7,0xad,0x64,0x32,0x57,0xc2,0xe9,0x63,0x9f,0x6d,0xa3,0x93,0x55,0x9e,
    0x45,0x0b,0x13,0xcc,0x89,0x20,0xa4,0x44,0xd3,0x2e,0x45,0x7b,0xa8,0x29,0x11,0x4e,
    0x4e,0x05,0x24,0xca,0xee,0xe2,0x38,0xc8,0x87,0x7e,0x8e,0xaf,0x1a,0xf7,0x29,0xaa,
    0x19,0x73,0x8b,0x90,0xb8,0x6e,0x46,0xa4,0xd9,0x76,0x19,0x65,0x67,0xd8,0x1b,0x56,
    0xa8,0x10,0x9d,0x1a,0x57,0x55,0x4d,0x5d,0x59,0x5b,0xf3,0xd7,0x38,0xf4,0x21,0x69,
    0x71,0x96,0x99,0xd4,0x78,0xf0,0xf6,0xbd,0x43,0x8d,0x34,0xdb,0x93,0xe9,0xdc,0x18,
    0x83,0x3d,0x44,0xf7,0x46,0x7a,0xce,0x6d,0x75,0x50,0x66,0x89,0x3f,0xa5,0x01,0x44,
    0xa6,0x51,0x04,0xa0,0xf6,0xd4,0x59,0x40,0xdb,0x67,0x10,0x4a,0x77,0x17,0x49,0x21,
    0xb4,0x37,0x83,0x2c,0x27,0x87,0xbf,0xa1,0x88,0x94,0xe4,0x5e,0xaf,0xd0,0x1d,0x67,
    0x0e,0xca,0x6f,0x6a,0x57,0x6c,0xc8,0x3d,0x3e,0x75,0xdb,0xe1,0x5c,0xc0,0xd9,0x42,
    0x57,0xf7,0x26,0xee,0xe2,0xc2,0x11,0xcd,0x5d,0x0f,0x2a,0x11,0x9f,0x24,0x4c,0xd4,
    0xb7,0xcd,0x35,0xa3,0x3f,0xfd,0xd6,0x40,0xd5,0x6f,0x5a,0x49,0xa0,0x71,0xae,0x64
};

#endif /* CE_LITE_CIPHER_PADDING_PKCS7 */

#if defined(CE_LITE_OTP) &&  \
    defined(CE_LITE_OTP_NORMAL) && \
    defined(CE_LITE_OTP_WRITE)

static const unsigned char aes_test_cbc_keyladder_enc[16] = {
    0x2F,0xDE,0x50,0xC3,0x6B,0x03,0xD0,0xC3,0x00,0xC2,0xE6,0xDC,0x9F,0x4D,0xE6,0x3E
};

static const unsigned char aes_test_cbc_ek3[16] = {
    0x50,0xCF,0x0F,0x29,0xD1,0xCF,0x64,0x41,0xC5,0xEA,0xAC,0xDB,0xDD,0x9A,0xFC,0xF4
};

static const unsigned char aes_test_cbc_ek2[16] = {
    0x9C,0x82,0x04,0x57,0xB7,0x17,0xD9,0x4A,0x55,0x74,0x2F,0x30,0x34,0x16,0x3B,0x52
};

static const unsigned char aes_test_cbc_ek1[16] = {
    0xF5,0xA0,0x33,0x7B,0x4B,0xE8,0x41,0x84,0x51,0x4E,0x08,0x86,0x5D,0x08,0xBB,0x6E
};

static const unsigned char aes_test_cbc_klad_iv[16] = {
    0x6f,0xba,0x4e,0x2e,0x64,0x50,0xdd,0xa4,0xa0,0x3d,0x4e,0xa1,0xd1,0x8a,0x61,0x3a
};

static const unsigned char aes_test_cbc_keyladder_dec[16] = {
    0xaf,0xea,0x37,0x58,0x88,0xed,0x56,0x1c,0xa0,0x95,0x02,0x52,0xe0,0xfc,0xeb,0xb1
};

#endif /* CE_LITE_OTP && CE_LITE_OTP_NORMAL && CE_LITE_OTP_WRITE */

#endif /* CE_LITE_AES && CE_LITE_CIPHER_MODE_CBC */

#endif /* MBEDTLS_TEST_DATA_AES_CBC_H */
