/*
 * Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef _TEST_DATA_ECDSA_H_
#define _TEST_DATA_ECDSA_H_

/*
 * The ECDSA test data is generated by OpenSSL tool. More information about
 * OpenSSL please see https://www.openssl.org/
 *
 * And The following steps show how to generate test data:
 *
 * 1. Generate one ec keypair.
 *      openssl genpkey -algorithm EC -pkeyopt ec_paramgen_curve:P-256 -out ecdsakey.pem
 *    This command will generate one EC keypair with SECP256R1 and save to ecdsakey.pem file.
 *    If want to use SECP521R1, replace "P-256" with "P-521".
 *
 * 2. Dump keypair:
 *      openssl ec -in ecdsakey.pem -text -noout -param_enc explicit
 *    Note: the public key follows SEC1 2.3.3 format: 0x04 || X || Y
 *
 * 3. Generate some message data. Here is a fixed string as message data:
 *      echo "message123" > message.bin
 *
 * 4. Generate hash.
 *      openssl dgst -sha256 -binary -out hash.bin message.bin
 *    This command will generate the SHA256 of message.bin and output to
 *    file hash.bin.
 *    If want to use SHA1, replace "-sha256" with "-sha1".
 *
 * for verify example:
 * 5. Generate sign.
 *      openssl pkeyutl -sign -in hash.bin -out sign.bin -inkey ecdsakey.pem
 *    This command will sign the hash data file hash.bin with key
 *    ecdsakey.pem, and output to file sign.bin.
 *
 * 6. Dump raw signature:
 *      openssl asn1parse -in sign.bin -inform DER -dump
 *    Note: the signature r and s printed in hex string format.
 *
 * 7. Dump signature in asn1:
 *      hexdump -C sign.bin
 */

static const unsigned char ecdsa_message[] = {
    0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x31, 0x32, 0x33, 0x0a
};

/* P256 test data */
static const unsigned char ec_d_p256[32] = {
    0x65, 0x0d, 0x8e, 0x34, 0x59, 0x78, 0x2f, 0x3a, 0x93, 0xda, 0x7a, 0x6c, 0xf2, 0xc6, 0x48, 0xdc,
    0x66, 0x07, 0xd0, 0x10, 0x02, 0xb9, 0x93, 0x6b, 0x8a, 0x8d, 0x03, 0xcb, 0xd9, 0x49, 0xe2, 0xb7
};

static const unsigned char ec_qx_p256[32] = {
    0x49, 0x72, 0xee, 0x4b, 0x84, 0x1e, 0xb7, 0xf2, 0x8d, 0xb1, 0x5f, 0xf9, 0x7a, 0x86, 0x5f, 0x45,
    0x49, 0x7e, 0x42, 0x1f, 0x82, 0xca, 0x5e, 0x35, 0x75, 0x0d, 0xf3, 0xfe, 0x86, 0x69, 0x0d, 0x2c
};

static const unsigned char ec_qy_p256[32] = {
    0xa5, 0x66, 0xe4, 0x7e, 0x94, 0x54, 0x24, 0xd3, 0x71, 0xfc, 0xa0, 0xff, 0xc6, 0xaa, 0x8d, 0x80,
    0xfb, 0xfc, 0xa5, 0xb2, 0x9c, 0x9a, 0xa0, 0x09, 0xb1, 0x96, 0x58, 0x9d, 0xc3, 0x4a, 0xba, 0xd9
};

static const unsigned char signature_r_p256_sha1[32] = {
    0xB0, 0x6A, 0x72, 0xD0, 0xE3, 0xEC, 0xAA, 0x14, 0x48, 0x1F, 0xB5, 0x57, 0x48, 0x7A, 0xE6, 0xBB,
    0x87, 0x95, 0xA1, 0x0B, 0x88, 0x96, 0xCE, 0xCB, 0x94, 0xEE, 0xFD, 0xCC, 0x83, 0x5A, 0x03, 0x60
};

static const unsigned char signature_s_p256_sha1[32] = {
    0x82, 0x60, 0x21, 0x2A, 0x10, 0x26, 0x97, 0x70, 0xC4, 0xDA, 0xE3, 0x87, 0x5E, 0x0B, 0xF3, 0x8B,
    0x87, 0xFE, 0x34, 0x5D, 0x0C, 0x4F, 0xA4, 0x54, 0x88, 0x6B, 0x35, 0xC6, 0x03, 0x06, 0x46, 0x82
};

static const unsigned char signature_asn1_p256_sha1[] = {
    0x30, 0x46, 0x02, 0x21, 0x00, 0xb0, 0x6a, 0x72, 0xd0, 0xe3, 0xec, 0xaa, 0x14, 0x48, 0x1f, 0xb5,
    0x57, 0x48, 0x7a, 0xe6, 0xbb, 0x87, 0x95, 0xa1, 0x0b, 0x88, 0x96, 0xce, 0xcb, 0x94, 0xee, 0xfd,
    0xcc, 0x83, 0x5a, 0x03, 0x60, 0x02, 0x21, 0x00, 0x82, 0x60, 0x21, 0x2a, 0x10, 0x26, 0x97, 0x70,
    0xc4, 0xda, 0xe3, 0x87, 0x5e, 0x0b, 0xf3, 0x8b, 0x87, 0xfe, 0x34, 0x5d, 0x0c, 0x4f, 0xa4, 0x54,
    0x88, 0x6b, 0x35, 0xc6, 0x03, 0x06, 0x46, 0x82
};

/* P521 test data */
static const unsigned char ec_d_p521[66] = {
    0x00, 0x0b, 0x98, 0xa3, 0xb3, 0xb7, 0xf5, 0x92, 0x91, 0x60, 0xf5, 0xa7, 0x3d, 0xf1, 0x4d, 0x03,
    0x7d, 0x99, 0xf2, 0xbe, 0x32, 0x28, 0x9a, 0x87, 0x75, 0x39, 0xf9, 0x03, 0xca, 0xca, 0x3b, 0xbe,
    0xbd, 0x5f, 0xb7, 0x08, 0x40, 0xba, 0xf9, 0x80, 0xde, 0x8e, 0xad, 0x9a, 0x2b, 0x00, 0xcf, 0xe1,
    0xd4, 0xff, 0xd1, 0xcf, 0xef, 0x4c, 0x6d, 0xbe, 0xa7, 0x98, 0x54, 0xbc, 0xc8, 0xaa, 0x86, 0x2c,
    0xa3, 0xf4
};

static const unsigned char ec_qx_p521[66] = {
    0x00, 0x0e, 0xe2, 0x57, 0x3b, 0x1a, 0xfe, 0x11, 0xcf, 0x8b, 0xe1, 0xee, 0x30, 0x77, 0xb5, 0xb4,
    0x32, 0x94, 0xdd, 0x6c, 0x9f, 0x38, 0x47, 0x65, 0x14, 0xc9, 0x6b, 0x3f, 0x26, 0x17, 0x20, 0x41,
    0x01, 0xca, 0x37, 0x30, 0xe8, 0x28, 0x5f, 0x45, 0x78, 0xa8, 0x8e, 0x34, 0x10, 0x75, 0x41, 0xf0,
    0x97, 0x09, 0x32, 0x6c, 0xb3, 0x25, 0x6d, 0xe9, 0xf9, 0x7a, 0x47, 0x20, 0xd2, 0x16, 0x75, 0xe2,
    0x28, 0xdf
};

static const unsigned char ec_qy_p521[66] = {
    0x01, 0xbc, 0x59, 0x96, 0x7d, 0x02, 0xa5, 0xea, 0x87, 0x33, 0x72, 0x83, 0xae, 0xe7, 0xe1, 0x9f,
    0xcb, 0x75, 0x07, 0x5d, 0xa6, 0xf5, 0x7f, 0x47, 0x9e, 0x7d, 0x3a, 0xf2, 0x0b, 0xa8, 0xb7, 0xf1,
    0x13, 0x41, 0x91, 0xae, 0x6f, 0x25, 0xb5, 0x89, 0xf7, 0x36, 0x40, 0xc5, 0x42, 0x51, 0xc4, 0xd5,
    0x6b, 0xef, 0xfa, 0x0a, 0x47, 0x7e, 0x0d, 0x43, 0x3f, 0x34, 0x39, 0x88, 0xa2, 0x3d, 0x6d, 0x9b,
    0x8c, 0x88
};

static const unsigned char signature_r_p521_sha256[] = {
    0x01, 0x52, 0x29, 0x90, 0x50, 0xF4, 0xA3, 0xBD, 0x66, 0x11, 0xA7, 0xE0, 0x6E, 0x97, 0xA1, 0xB8,
    0x17, 0xAD, 0x46, 0x24, 0x22, 0x06, 0x53, 0x63, 0x7B, 0x86, 0x59, 0x22, 0xDC, 0x25, 0x98, 0x96,
    0xCB, 0x68, 0xBC, 0xCD, 0x98, 0x3D, 0x08, 0x51, 0x92, 0xD8, 0x8E, 0xC0, 0x21, 0x86, 0x00, 0x50,
    0x4C, 0xD0, 0x05, 0xB4, 0xBB, 0x8E, 0x82, 0xF7, 0x10, 0xAF, 0xAB, 0x01, 0x12, 0xF4, 0xEF, 0x35,
    0x5F, 0x77
};
static const unsigned char signature_s_p521_sha256[] = {
    0x1F, 0x76, 0xF1, 0x45, 0xDA, 0x3D, 0x88, 0x53, 0x08, 0xD0, 0x5E, 0x6F, 0xE4, 0xCF, 0x51, 0xC2,
    0x9A, 0xFA, 0x91, 0x6A, 0x85, 0xCD, 0x32, 0x07, 0x16, 0xA9, 0x19, 0xCF, 0xC1, 0x2D, 0x08, 0x93,
    0xF5, 0xA1, 0x23, 0x92, 0x01, 0x85, 0x77, 0xD9, 0x7C, 0x2A, 0x9B, 0x55, 0xE1, 0x5C, 0x58, 0x04,
    0xBE, 0x33, 0x94, 0xC9, 0x3A, 0x3B, 0xAB, 0x96, 0x37, 0x42, 0xDA, 0xF3, 0xF5, 0xE5, 0x33, 0x99,
    0x6A
};

static const unsigned char signature_asn1_p521_sha256[] = {
    0x30, 0x81, 0x87, 0x02, 0x42, 0x01, 0x52, 0x29, 0x90, 0x50, 0xf4, 0xa3, 0xbd, 0x66, 0x11, 0xa7,
    0xe0, 0x6e, 0x97, 0xa1, 0xb8, 0x17, 0xad, 0x46, 0x24, 0x22, 0x06, 0x53, 0x63, 0x7b, 0x86, 0x59,
    0x22, 0xdc, 0x25, 0x98, 0x96, 0xcb, 0x68, 0xbc, 0xcd, 0x98, 0x3d, 0x08, 0x51, 0x92, 0xd8, 0x8e,
    0xc0, 0x21, 0x86, 0x00, 0x50, 0x4c, 0xd0, 0x05, 0xb4, 0xbb, 0x8e, 0x82, 0xf7, 0x10, 0xaf, 0xab,
    0x01, 0x12, 0xf4, 0xef, 0x35, 0x5f, 0x77, 0x02, 0x41, 0x1f, 0x76, 0xf1, 0x45, 0xda, 0x3d, 0x88,
    0x53, 0x08, 0xd0, 0x5e, 0x6f, 0xe4, 0xcf, 0x51, 0xc2, 0x9a, 0xfa, 0x91, 0x6a, 0x85, 0xcd, 0x32,
    0x07, 0x16, 0xa9, 0x19, 0xcf, 0xc1, 0x2d, 0x08, 0x93, 0xf5, 0xa1, 0x23, 0x92, 0x01, 0x85, 0x77,
    0xd9, 0x7c, 0x2a, 0x9b, 0x55, 0xe1, 0x5c, 0x58, 0x04, 0xbe, 0x33, 0x94, 0xc9, 0x3a, 0x3b, 0xab,
    0x96, 0x37, 0x42, 0xda, 0xf3, 0xf5, 0xe5, 0x33, 0x99, 0x6a
};

#endif /* _TEST_DATA_ECDSA_H_ */
