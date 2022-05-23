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
#ifndef MBEDTLS_TEST_DATA_SM4_CTR_H
#define MBEDTLS_TEST_DATA_SM4_CTR_H

#include "ce_lite_config.h"

#if defined(CE_LITE_SM4) && defined(CE_LITE_CIPHER_MODE_CTR)

static const unsigned char sm4_test_ctr_enc[513] = {
    0xc0, 0x39, 0xed, 0x94, 0xd3, 0xab, 0x2f, 0x43, 0x5d, 0x21, 0xb5, 0x3e, 0x4e, 0x78, 0x28, 0x1d,
    0xfd, 0x23, 0xdb, 0x75, 0xf6, 0x96, 0x53, 0xf9, 0xbd, 0x3a, 0x92, 0x3d, 0xa4, 0xa3, 0xa9, 0x77,
    0x49, 0x98, 0x97, 0xb7, 0x40, 0xd1, 0xe8, 0x3d, 0x0f, 0x33, 0x13, 0xe7, 0xba, 0x0d, 0x21, 0x13,
    0x43, 0x32, 0x52, 0x82, 0x0e, 0x2d, 0xf5, 0xb4, 0x5e, 0x7e, 0x3f, 0x38, 0xc6, 0xaf, 0xe6, 0x89,
    0x11, 0xac, 0x1a, 0x88, 0x7b, 0x2d, 0x40, 0xb0, 0xa4, 0xca, 0x25, 0xd4, 0x92, 0xe2, 0x02, 0x34,
    0x86, 0x04, 0x06, 0x03, 0x93, 0x2b, 0x03, 0xac, 0x37, 0xf0, 0x9b, 0x40, 0x5f, 0xcd, 0x7a, 0xc9,
    0xe4, 0x6f, 0xbf, 0x65, 0xb7, 0xb7, 0x26, 0x78, 0xe2, 0xec, 0x75, 0x1c, 0xdb, 0xfe, 0x78, 0xd4,
    0xe1, 0x69, 0x19, 0xfc, 0x03, 0x63, 0x6a, 0xf4, 0xdc, 0x50, 0xa6, 0x15, 0xe2, 0xd3, 0xf2, 0xe7,
    0x37, 0x7c, 0xd4, 0x90, 0x6f, 0x73, 0xdd, 0x1c, 0x6e, 0xcb, 0xc4, 0xb1, 0xf7, 0x90, 0x28, 0x7e,
    0xf3, 0xec, 0x60, 0xcb, 0xdf, 0xbd, 0x39, 0xe1, 0x76, 0x70, 0xff, 0xf1, 0x77, 0x3e, 0x0f, 0x0c,
    0x5b, 0x85, 0x5b, 0x26, 0x9b, 0x97, 0xce, 0xd3, 0x58, 0xeb, 0x2a, 0x8b, 0x7a, 0x19, 0xec, 0x2c,
    0x99, 0x75, 0x53, 0xe2, 0x3f, 0xd5, 0xeb, 0x7c, 0x93, 0xca, 0xf0, 0x5e, 0x92, 0x01, 0x84, 0x4d,
    0xf2, 0x1d, 0x26, 0x2d, 0x7d, 0x25, 0xd5, 0xc5, 0x64, 0xd8, 0x2c, 0xc6, 0x15, 0xc2, 0x4d, 0x3e,
    0x06, 0x5d, 0xda, 0x92, 0xa7, 0x41, 0xf9, 0x65, 0x91, 0xd5, 0xbf, 0x31, 0xb3, 0x47, 0x6a, 0x04,
    0x6f, 0xff, 0xb6, 0xa2, 0x0e, 0xa4, 0xd0, 0xd2, 0xb6, 0xa9, 0xbc, 0x59, 0x24, 0xde, 0xaf, 0x9b,
    0x33, 0x7a, 0x33, 0xbb, 0xb9, 0x52, 0xe8, 0x4a, 0x06, 0xf7, 0xe1, 0xda, 0x63, 0xb3, 0xf2, 0xb8,
    0x09, 0xe8, 0x13, 0xa2, 0x56, 0xa4, 0x6e, 0x85, 0xde, 0x48, 0xbc, 0xf9, 0xff, 0x20, 0x67, 0x74,
    0xe6, 0x3d, 0x5f, 0x44, 0xf3, 0x14, 0x1a, 0xf4, 0xe5, 0xbd, 0x72, 0xee, 0xe9, 0xa6, 0x22, 0xa2,
    0x3c, 0xe2, 0x49, 0x7a, 0xe2, 0x59, 0xb2, 0x3b, 0x03, 0xd4, 0xb8, 0x57, 0xf3, 0xfa, 0x47, 0xad,
    0x14, 0x23, 0x67, 0x47, 0xa2, 0xc0, 0xf8, 0x53, 0x97, 0xe4, 0x13, 0x7f, 0xb3, 0x76, 0x0b, 0x5a,
    0x2b, 0x2f, 0x37, 0x94, 0x7f, 0x56, 0xcb, 0xc3, 0xee, 0xfd, 0xb0, 0xad, 0x2e, 0x98, 0xd5, 0x4d,
    0x0d, 0xde, 0x2d, 0x41, 0xa6, 0x0e, 0x5d, 0x7b, 0x3d, 0x59, 0x74, 0xb7, 0x0b, 0x6d, 0xbd, 0x7b,
    0x6d, 0xec, 0xe7, 0xfa, 0x3a, 0xf8, 0x29, 0xc4, 0x98, 0x5e, 0x1d, 0xd9, 0x09, 0x04, 0x8c, 0x72,
    0xda, 0xd2, 0x13, 0xc1, 0xa7, 0xb8, 0x9e, 0x15, 0x8f, 0x78, 0x93, 0x8b, 0xa1, 0x09, 0xd1, 0x07,
    0xc9, 0xfb, 0x7a, 0xb5, 0xd4, 0x79, 0x0f, 0x05, 0xab, 0x54, 0x95, 0x34, 0x79, 0xbe, 0x57, 0xf9,
    0x8d, 0xd9, 0xf7, 0x13, 0x6e, 0xf3, 0x07, 0x6b, 0x8c, 0x83, 0x04, 0x6c, 0x52, 0x08, 0x73, 0xa4,
    0x22, 0xf6, 0xff, 0x75, 0x31, 0x3c, 0xdb, 0xac, 0xe8, 0x5f, 0xfd, 0xda, 0xa2, 0x74, 0xe0, 0xdf,
    0x83, 0x0f, 0x55, 0xb1, 0x04, 0x98, 0x46, 0xdd, 0x8b, 0x0b, 0x7f, 0x5d, 0xd8, 0x90, 0xce, 0xc1,
    0xe4, 0x62, 0x67, 0xb3, 0x2e, 0x61, 0xf0, 0xb8, 0xa1, 0x10, 0x30, 0x04, 0x1c, 0x6d, 0x37, 0xc4,
    0xf0, 0xd3, 0x14, 0x62, 0x95, 0xf1, 0x2a, 0x42, 0x9a, 0xbb, 0x18, 0xe9, 0xd6, 0x59, 0x90, 0xad,
    0x96, 0x85, 0x5e, 0xe1, 0x9f, 0x80, 0xd6, 0x90, 0xec, 0xde, 0xf6, 0x80, 0x4b, 0xc1, 0x36, 0x31,
    0xcf, 0x71, 0xbb, 0x8d, 0x9d, 0xc9, 0xca, 0x30, 0xec, 0x44, 0x02, 0x31, 0x42, 0x9f, 0x27, 0xc2,
    0xe2
};

static const unsigned char sm4_test_ctr_key[16] = {
    0xce, 0xb0, 0x09, 0xae, 0xa4, 0x45, 0x44, 0x51, 0xfe, 0xad, 0xf0, 0xe6, 0xb3, 0x6f, 0x45, 0x55
};

static const unsigned char sm4_test_ctr_iv[16] = {
    0x76, 0x40, 0x43, 0xc4, 0x94, 0x60, 0xb7, 0x48, 0x8e, 0x74, 0x87, 0xfd, 0x81, 0xd4, 0xfb, 0xa2
};

static const unsigned char sm4_test_ctr_strblk[16] = {
    0x8e,0x51,0x43,0x98,0x39,0xef,0xcc,0xa0,0x04,0x8a,0xd6,0x9a,0x51,0x56,0xbf,0xc5,
};

static const unsigned char sm4_test_ctr_dec[513] = {
    0x4d, 0xa5, 0x15, 0x1e, 0x1b, 0xc7, 0xd8, 0x53, 0x16, 0x71, 0xdc, 0xdd, 0xeb, 0x6b, 0xab, 0x34,
    0x56, 0xa4, 0x8f, 0x91, 0x40, 0x64, 0xe4, 0x63, 0x51, 0x0b, 0x72, 0x4a, 0x5f, 0xfa, 0x89, 0xe5,
    0x4c, 0x22, 0x6c, 0xf8, 0xcb, 0xd8, 0x69, 0xdd, 0xaa, 0xb9, 0x9d, 0x4d, 0x28, 0x55, 0x68, 0x19,
    0x39, 0xec, 0x89, 0xd9, 0xbc, 0xa3, 0x2c, 0xfe, 0x93, 0x13, 0x50, 0x42, 0x0f, 0x6a, 0x70, 0xc7,
    0xfb, 0x03, 0xb5, 0x4a, 0xa5, 0xcb, 0x92, 0x38, 0xb5, 0x96, 0x8c, 0x6e, 0x6b, 0x7e, 0xcb, 0x73,
    0x70, 0x1c, 0x92, 0xa3, 0x75, 0xa7, 0x9f, 0xc5, 0xea, 0x28, 0xdb, 0xbf, 0xe5, 0x89, 0xfd, 0xc9,
    0x07, 0x45, 0x39, 0x7e, 0xc1, 0x12, 0x47, 0x4c, 0x68, 0x97, 0x22, 0x88, 0x4f, 0x38, 0x73, 0x39,
    0xc5, 0x68, 0x46, 0xce, 0x2f, 0xcd, 0x45, 0x34, 0x08, 0x52, 0x32, 0xa7, 0xc1, 0x2e, 0x4f, 0x87,
    0x0e, 0x11, 0xc9, 0x45, 0x8e, 0x68, 0xf8, 0xb1, 0x2e, 0x37, 0x7f, 0x4d, 0x91, 0x5a, 0xaf, 0x48,
    0xbb, 0xdb, 0x13, 0xdd, 0x1f, 0x61, 0xd1, 0xc2, 0x9d, 0x51, 0x08, 0x4c, 0x4a, 0x97, 0x96, 0xff,
    0xe9, 0xc1, 0xbb, 0x7c, 0x8c, 0x2b, 0x96, 0x75, 0x6f, 0x12, 0xd7, 0xff, 0x2d, 0x07, 0x19, 0xa9,
    0xb8, 0x77, 0xaa, 0xf1, 0xf3, 0x29, 0x67, 0x6e, 0x31, 0x17, 0xc3, 0x61, 0xb2, 0x85, 0x05, 0xd1,
    0x60, 0x39, 0x36, 0x72, 0xfa, 0xb5, 0x20, 0x36, 0xf3, 0x42, 0x99, 0x09, 0x7b, 0x7f, 0x46, 0xe5,
    0xca, 0xcc, 0x79, 0x2b, 0xb1, 0x3d, 0xab, 0x63, 0xbc, 0xe2, 0xbd, 0x35, 0x0e, 0x13, 0x32, 0xc0,
    0xd5, 0xf1, 0x87, 0x35, 0xd4, 0xfc, 0xa5, 0x56, 0x7c, 0xc9, 0x51, 0xee, 0x63, 0x1a, 0xd8, 0xb4,
    0x75, 0xef, 0x17, 0x5e, 0xb9, 0xe1, 0xa4, 0x2b, 0x4c, 0x23, 0xe6, 0x67, 0xa3, 0xa8, 0x39, 0x87,
    0xf8, 0x11, 0x51, 0x2f, 0xa8, 0xe8, 0x10, 0x95, 0xe9, 0xd5, 0x0d, 0xf0, 0xdd, 0x87, 0xb3, 0xc5,
    0x31, 0x33, 0xa3, 0x1f, 0x5a, 0x55, 0x0d, 0x73, 0x1d, 0x05, 0xb6, 0x98, 0x73, 0x76, 0x94, 0x39,
    0xdc, 0x3d, 0x44, 0xbf, 0xfb, 0x1c, 0xad, 0x3c, 0x01, 0x44, 0x97, 0x9e, 0x8b, 0x4a, 0x18, 0x92,
    0xf1, 0xe0, 0x5f, 0x7c, 0x44, 0x6c, 0x20, 0xca, 0xd7, 0x90, 0x69, 0x9e, 0xfe, 0x54, 0xdb, 0x16,
    0x1d, 0xdc, 0xa7, 0x2f, 0x2d, 0x41, 0xe1, 0x76, 0x2e, 0x76, 0x80, 0xae, 0x64, 0x7a, 0x5e, 0x8a,
    0x2f, 0x1e, 0x18, 0x0e, 0x73, 0xca, 0x96, 0x78, 0xf3, 0x59, 0x74, 0xa3, 0xda, 0x6e, 0xab, 0xd8,
    0x29, 0xa2, 0xb9, 0x11, 0x11, 0xe1, 0x2a, 0x00, 0x2a, 0xfb, 0xa1, 0x53, 0x7a, 0xfd, 0x26, 0xc7,
    0x90, 0x1d, 0x3d, 0x05, 0xd1, 0x44, 0x5d, 0x2d, 0xfc, 0x1f, 0x92, 0x40, 0x1a, 0x1d, 0x0c, 0xdc,
    0x60, 0x51, 0x58, 0x76, 0x51, 0xc9, 0xc3, 0xb6, 0x62, 0x31, 0x3c, 0x3d, 0xae, 0x4f, 0x7d, 0x4a,
    0x31, 0xa2, 0x7a, 0xba, 0xba, 0x4b, 0xf4, 0x48, 0xeb, 0x0e, 0x3e, 0x8e, 0xec, 0x4b, 0xaf, 0xb6,
    0xb3, 0x2b, 0x3b, 0xed, 0x74, 0xc3, 0x50, 0xd5, 0x0a, 0x60, 0x82, 0xf3, 0x5a, 0xa6, 0x60, 0xd8,
    0x1e, 0x45, 0xef, 0x1d, 0x15, 0x3d, 0xd7, 0x4d, 0x6e, 0x05, 0x1e, 0x48, 0x2c, 0x17, 0xde, 0xa1,
    0x6d, 0xfd, 0x05, 0x93, 0xe3, 0x97, 0x0f, 0xe5, 0x69, 0xa8, 0x3d, 0x73, 0xd1, 0x39, 0x15, 0x95,
    0x8f, 0xde, 0xd6, 0x29, 0xca, 0xd2, 0xf8, 0x24, 0x06, 0xe0, 0xb6, 0xb9, 0x1b, 0x75, 0x21, 0x83,
    0x0d, 0xb2, 0xf5, 0x8e, 0x0d, 0xed, 0xd0, 0x1f, 0xfe, 0xa8, 0x6b, 0xa2, 0xe3, 0x30, 0xc0, 0x85,
    0x38, 0x64, 0x39, 0x6c, 0x0c, 0x04, 0xad, 0x55, 0x0a, 0xa1, 0x68, 0x34, 0x29, 0x5b, 0x45, 0x50,
    0x34
};

#if defined(CE_LITE_OTP) &&  \
    defined(CE_LITE_OTP_NORMAL) && \
    defined(CE_LITE_OTP_WRITE)

static const unsigned char sm4_test_ctr_keyladder_enc[32] = {
    0xdd,0x11,0x3f,0x3a,0x02,0x7e,0x26,0x98,0x34,0x6a,0x98,0x5c,0x27,0x68,0x62,0x5c,
    0x4f,0x31,0x2b,0x0c,0x7e,0xb1,0xff,0xad,0x10,0x0d,0xe3,0xee,0x30,0xd4,0xb9,0x16
};

static const unsigned char sm4_test_ctr_ek3[16] = {
    0x50,0xCF,0x0F,0x29,0xD1,0xCF,0x64,0x41,0xC5,0xEA,0xAC,0xDB,0xDD,0x9A,0xFC,0xF4
};

static const unsigned char sm4_test_ctr_ek2[16] = {
    0x9C,0x82,0x04,0x57,0xB7,0x17,0xD9,0x4A,0x55,0x74,0x2F,0x30,0x34,0x16,0x3B,0x52
};

static const unsigned char sm4_test_ctr_ek1[16] = {
    0xF5,0xA0,0x33,0x7B,0x4B,0xE8,0x41,0x84,0x51,0x4E,0x08,0x86,0x5D,0x08,0xBB,0x6E
};

static const unsigned char sm4_test_ctr_klad_iv[16] = {
    0xb9,0x91,0xd6,0x9c,0x46,0xe6,0xb6,0x88,0x30,0x5a,0x73,0x1d,0x08,0x5e,0x72,0xe9
};

static const unsigned char sm4_test_ctr_keyladder_dec[32] = {
    0xf4,0x43,0x9d,0x8d,0xd6,0xa9,0xb4,0x5f,0xe2,0xe8,0xa5,0x27,0x97,0x36,0x37,0x42,
    0x78,0xff,0x35,0xb5,0x1b,0xdb,0xbb,0xb6,0xda,0xcb,0xeb,0xa2,0x2a,0xc6,0x81,0x20
};

#endif /* CE_LITE_OTP && CE_LITE_OTP_NORMAL && CE_LITE_OTP_WRITE */

#endif /* CE_LITE_SM4 && CE_LITE_CIPHER_MODE_CTR */

#endif /* MBEDTLS_TEST_DATA_SM4_CTR_H */