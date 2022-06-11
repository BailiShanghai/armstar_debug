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

#include "ce_lite_config.h"
#if defined(CE_LITE_AES)
#include "pal_common.h"
#include "pal_log.h"
#include "pal_string.h"
#include "test_data_aes_ecb.h"
#include "test_data_aes_cbc.h"
#include "test_data_aes_ctr.h"
#include "mbedtls/aes.h"
#include "otp_rootkey_init.h"

/**
 * There are some mbedtls aes examples for your reference.
 *
 * Example Scope:
 *  - cipher mode is ecb(single/multiple).
 *  - cipher mode is ecb(secure key).
 *  - cipher mode is ecb(pkcs7 padding).
 *  - cipher mode is cbc(single/multiple).
 *  - cipher mode is cbc(secure key).
 *  - cipher mode is cbc(update stream iv).
 *  - cipher mode is cbc(pkcs7 padding).
 *  - cipher mode is ctr(single/multiple).
 *  - cipher mode is ctr(secure key).
 *  - cipher mode is ctr(update stream iv).
 *
 * Example List:
 *  - Example 1: Encrypt and decrypt a whole data block in ECB cipher mode.
 *               To enable ECB cipher mode, you must define the macro
 *               \c CE_LITE_CIPHER_MODE_ECB in ce_lite_config.h .
 *  - Example 2: Encrypt and decrypt a whole data block in ECB cipher mode
 *               using the secure key. To enable ECB cipher mode, you must
 *               define the macro \c CE_LITE_CIPHER_MODE_ECB in ce_lite_config.h .
 *               Since the example depends on the OTP, please ensure that the
 *               macro \c CE_LITE_OTP , \c CE_LITE_OTP_NORMAL and
 *               \c CE_LITE_OTP_WRITE are enabled.
 *               To run this example, you must define the macro
 *               \c CE_LITE_EXAMPLE_AES_ECB_SECKEY.
 *  - Example 3: Encrypt and decrypt multiple-blocks sliced data in ECB cipher mode.
 *               This is the extention of ECB cipher mode.
 *               To enable ECB cipher mode, you must define the macro
 *               \c CE_LITE_CIPHER_MODE_ECB.
 *  - Example 4: Encrypt and decrypt multiple-blocks sliced data in ECB cipher mode with
 *               PKCS7 padding. To enable ECB cipher mode, you must define the
 *               macro \c CE_LITE_CIPHER_MODE_ECB. To enable PKCS7 padding, you
 *               must define the macro \c CE_LITE_CIPHER_PADDING_PKCS7.
 *  - Example 5: Encrypt and decrypt a whole data block in CBC cipher mode.
 *               To enable CBC cipher mode, you must define the macro
 *               \c CE_LITE_CIPHER_MODE_CBC in ce_lite_config.h .
 *  - Example 6: Encrypt and decrypt multiple-blocks sliced data in CBC cipher
 *               mode with iv update. To enable CBC cipher mode, you must define the macro
 *               \c CE_LITE_CIPHER_MODE_CBC. To enable multiple-blocks
 *               en/decryption, you must define the macro
 *               \c CE_LITE_CIPHER_MULTI_BLOCKS in ce_lite_config.h .
 *  - Example 7: Encrypt and decrypt multiple-blocks sliced data in CBC cipher
 *               mode with PKCS7 padding. To enable CBC cipher mode, you must
 *               define the macro \c CE_LITE_CIPHER_MODE_CBC. To enable PKCS7
 *               padding, you must define the macro \c CE_LITE_CIPHER_PADDING_PKCS7.
 *  - Example 8: Encrypt and decrypt multiple-blocks sliced data in CBC cipher
 *               mode with PKCS7 padding and iv update. To enable CBC cipher mode,
 *               you must define the macro \c CE_LITE_CIPHER_MODE_CBC.
 *               To enable multiple-blocks en/decryption, you must define the macro
 *               \c CE_LITE_CIPHER_MULTI_BLOCKS in ce_lite_config.h .
 *  - Example 9: Encrypt and decrypt a whole data block in CBC cipher mode
 *               using the secure key. To enable CBC cipher mode, you must
 *               define the macro \c CE_LITE_CIPHER_MODE_CBC in ce_lite_config.h .
 *               Since the example depends on the OTP, please ensure that the
 *               macro \c CE_LITE_OTP , \c CE_LITE_OTP_NORMAL and
 *               \c CE_LITE_OTP_WRITE are enabled.
 *               To run this example, you must define the macro
 *               \c CE_LITE_EXAMPLE_AES_CBC_SECKEY.
 *  - Example 10: Encrypt and decrypt a whole data in CTR cipher mode.
 *               To enable CTR cipher mode, you must define the macro
 *               \c CE_LITE_CIPHER_MODE_CTR in ce_lite_config.h .
 *  - Example 11: Encrypt and decrypt multiple sliced data in CTR
 *               cipher mode. To enable CTR cipher mode, you must
 *               define the macro \c CE_LITE_CIPHER_MODE_CTR. To enable
 *               multiple sliced en/decryption, you must define the
 *               macro \c CE_LITE_CIPHER_MULTI_BLOCKS in ce_lite_config.h .
 *  - Example 12: Encrypt and decrypt a whole data block in CTR cipher mode
 *               using the secure key. To enable CTR cipher mode, you must
 *               define the macro \c CE_LITE_CIPHER_MODE_CTR in ce_lite_config.h .
 *               Since the example depends on the OTP, please ensure that the
 *               macro \c CE_LITE_OTP , \c CE_LITE_OTP_NORMAL and
 *               \c CE_LITE_OTP_WRITE are enabled.
 *               To run this example, you must define the macro
 *               \c CE_LITE_EXAMPLE_AES_CTR_SECKEY.
 *
 * Usage:
 *  - Call mbedtls_aes_example_entry() function.
 *
 * Example Golden Data:
 *  - The file "test_data_aes_ecb.h" includes aes en/de/key etc
 *    data array in ECB cipher mode.
 *  - The file "test_data_aes_cbc.h" includes aes en/de/key etc
 *    data array in CBC cipher mode.
 *  - The file "test_data_aes_ctr.h" includes aes en/de/key etc
 *    data array in CTR cipher mode.
 *
 * Test Data Generation:
 * The test data can be generated by openssl. This takes ECB
 * cipher mode data as an example.
 *
 * S1: Prepare the data of encryption binary file.
 *
 *     save the data of \p aes_test_ecb_enc as binary file
 *     and name it `ecb_enc.bin`
 *     `$ hexdump ecb_enc.bin` is showed as follow:
 *     ```
 *     0000000 c3 4c 05 2c c0 da 8d 73 45 1a fe 5f 03 be 29 7f
 *     ```
 * S2: Run openssl command line to generate the `ecb_dec.bin`
 *
 *     `$ openssl aes-128-ecb -e -in ecb_enc.bin -out ecb_dec.bin \
 *        -K 2B7E151628AED2A6ABF7158809CF4F3C `
 *     The HEX format \p aes_test_ecb_key should be assigned on '-K'
 *     argument.
 *
 * S3: Export out the data of `ecb_dec.bin`
 *
 *     `$ hexdump ecb_dec.bin` is showed as follow:
 *     ```
 *     0000000 55 5c 82 24 e2 c9 58 73 12 08 5b fb 4e cb 04 b8
 *     ```
 * You can get the data of other cipher mode by `openssl [mode]` changing.
 *
 * * For the cbc enc:
 *     `$ openssl aes-128-cbc -e -in cbc_enc.bin \
 *      -out cbc_dec.bin -iv your_iv -K your_key`
 *
 * * For the cbc dec:
 *     `$ openssl aes-128-cbc -d -in cbc_dec.bin \
 *      -out cbc_enc.bin -iv your_iv -K your_key`
 *
 * * For the cbc-pkcs7 enc:
 *     `$ openssl aes-128-cbc -e -in cbc_enc.bin \
 *      -out cbc_dec.bin -iv your_iv -K your_key pkcs7`
 *
 * * Similarly, CTR data can be exported in the same way.
 *   For more data, please refer to `openssl help`
 *
 */

/**
 * \brief Enable secure key example of AES ECB mode.
 *
 *  Warning:
 *  Please use this example carefully, as it would write the OTP
 *  and hence risk of damaging the chip.
 *
 *  Disable it by default.
 */
#undef CE_LITE_EXAMPLE_AES_ECB_SECKEY

/**
 * \brief Enable secure key example of AES CBC mode.
 *
 *  Warning:
 *  Please use this example carefully, as it would write the OTP
 *  and hence risk of damaging the chip.
 *
 *  Disable it by default.
 */
#undef CE_LITE_EXAMPLE_AES_CBC_SECKEY

/**
 * \brief Enable secure key example of AES CTR mode.
 *
 *  Warning:
 *  Please use this example carefully, as it would write the OTP
 *  and hence risk of damaging the chip.
 *
 *  Disable it by default.
 */
#undef CE_LITE_EXAMPLE_AES_CTR_SECKEY


#if defined(CE_LITE_CIPHER_MODE_ECB)

void dump_array_u8(uint8_t *ptr, uint32_t len)
{
	int i;

	for(i = 0; i < len; i ++){
		bk_printf(" 0x%02x ", ptr[i]);
	}
	
	bk_printf("\r\n");
}

__attribute__((section("aes_sec"))) uint8_t aes_result_buf[16] = {0};

int mbedtls_aes_cipher_ecb_enc(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
	bk_printf("mbedtls_aes_init\n");
    mbedtls_aes_init(&ctx);

	bk_printf("mbedtls_aes_setkey_enc\n");
    ret = mbedtls_aes_setkey_enc(&ctx, aes_test_ecb_key,
                                 sizeof(aes_test_ecb_key) << 3);
    if (ret != 0) {
        bk_printf("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

	bk_printf("mbedtls_aes_crypt_ecb\n");
    ret = mbedtls_aes_crypt_ecb(&ctx,
                                MBEDTLS_AES_ENCRYPT,
                                aes_test_ecb_enc,
                                aes_result_buf);
    if (ret != 0) {
        bk_printf("[error -0x%x]: aes encrypt ecb failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(aes_result_buf, aes_test_ecb_dec, sizeof(aes_result_buf));
    if (ret != 0) {
        bk_printf("[selftest failed] result_buf != aes_test_ecb_dec\n");
        bk_printf("result_buf:\n");
		dump_array_u8((uint8_t *)aes_result_buf, sizeof(aes_result_buf));
        bk_printf("aes_test_ecb_dec:\n");
		dump_array_u8((uint8_t *)aes_test_ecb_dec, sizeof(aes_test_ecb_dec));
    }

finish:
    mbedtls_aes_free(&ctx);
    return ret;
}

int mbedtls_aes_cipher_ecb_dec(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[16] = {0};

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_dec(&ctx, aes_test_ecb_key,
                                 sizeof(aes_test_ecb_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    ret = mbedtls_aes_crypt_ecb(&ctx,
                                MBEDTLS_AES_DECRYPT,
                                aes_test_ecb_dec,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt ecb failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ecb_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ecb enc\n");
    }

finish:
    mbedtls_aes_free(&ctx);
    return ret;
}

#if defined(CE_LITE_OTP) &&  \
    defined(CE_LITE_OTP_NORMAL) && \
    defined(CE_LITE_OTP_WRITE) && \
    defined(CE_LITE_EXAMPLE_AES_ECB_SECKEY)

static int _aes_ecb_keyladder_init(void)
{
    return otp_rootkey_init();
}

static int mbedtls_cipher_ecb_keyladder_enc(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[16] = {0};
    mbedtls_aes_sec_key_t sec_key = {0};

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    pal_memcpy(sec_key.ek1, aes_test_ecb_ek1,
               sizeof(aes_test_ecb_ek1));
    pal_memcpy(sec_key.ek2, aes_test_ecb_ek2,
               sizeof(aes_test_ecb_ek2));
    pal_memcpy(sec_key.ek3, aes_test_ecb_ek3,
               sizeof(aes_test_ecb_ek3));
    sec_key.ek1bits = sizeof(aes_test_ecb_ek1) << 3;
    sec_key.sel = MBEDTLS_AES_KL_KEY_ROOT;

    ret = mbedtls_aes_setseckey_enc(&ctx, &sec_key);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set secure key failed.\n", ret);
        goto finish;
    }

    ret = mbedtls_aes_crypt_ecb(&ctx,
                                MBEDTLS_AES_ENCRYPT,
                                aes_test_ecb_keyladder_enc,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes encrypt ecb failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ecb_keyladder_dec, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ecb enc\n");
    }

finish:
    pal_memset(&sec_key, 0, sizeof(mbedtls_aes_sec_key_t));
    mbedtls_aes_free(&ctx);
    return ret;
}

static int mbedtls_cipher_ecb_keyladder_dec(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[16] = {0};
    mbedtls_aes_sec_key_t sec_key = {0};

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    pal_memcpy(sec_key.ek1, aes_test_ecb_ek1,
               sizeof(aes_test_ecb_ek1));
    pal_memcpy(sec_key.ek2, aes_test_ecb_ek2,
               sizeof(aes_test_ecb_ek2));
    pal_memcpy(sec_key.ek3, aes_test_ecb_ek3,
               sizeof(aes_test_ecb_ek3));
    sec_key.ek1bits = sizeof(aes_test_ecb_ek1) << 3;
    sec_key.sel = MBEDTLS_AES_KL_KEY_ROOT;

    ret = mbedtls_aes_setseckey_dec(&ctx, &sec_key);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set secure key failed.\n", ret);
        goto finish;
    }

    ret = mbedtls_aes_crypt_ecb(&ctx,
                                MBEDTLS_AES_DECRYPT,
                                aes_test_ecb_keyladder_dec,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt ecb failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ecb_keyladder_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ecb dec\n");
    }

finish:
    pal_memset(&sec_key, 0, sizeof(mbedtls_aes_sec_key_t));
    mbedtls_aes_free(&ctx);
    return ret;
}

#endif /* CE_LITE_OTP && CE_LITE_OTP_NORMAL && CE_LITE_OTP_WRITE && \
        * CE_LITE_EXAMPLE_AES_ECB_SECKEY */

static int mbedtls_cipher_ecb_ext_enc(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[32] = {0};

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_enc(&ctx, aes_test_ecb_ext_key,
                                 sizeof(aes_test_ecb_ext_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    ret = mbedtls_aes_crypt_ecb_ext(&ctx,
                                    MBEDTLS_AES_ENCRYPT,
                                    sizeof(aes_test_ecb_ext_enc),
                                    aes_test_ecb_ext_enc,
                                    result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes encrypt ecb failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ecb_ext_dec, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ecb enc\n");
    }

finish:
    mbedtls_aes_free(&ctx);
    return ret;
}

static int mbedtls_cipher_ecb_ext_dec(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[32] = {0};

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_dec(&ctx, aes_test_ecb_ext_key,
                                 sizeof(aes_test_ecb_ext_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    ret = mbedtls_aes_crypt_ecb_ext(&ctx,
                                    MBEDTLS_AES_DECRYPT,
                                    sizeof(aes_test_ecb_ext_dec),
                                    aes_test_ecb_ext_dec,
                                    result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt ecb failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ecb_ext_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ecb dec\n");
    }

finish:
    mbedtls_aes_free(&ctx);
    return ret;
}

#if defined(CE_LITE_CIPHER_PADDING_PKCS7)

static int mbedtls_cipher_ecb_pkcs7_enc(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[32] = {0};
    size_t olen = 0;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_enc(&ctx, aes_test_ecb_pkcs7_key,
                                 sizeof(aes_test_ecb_pkcs7_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    ret = mbedtls_aes_crypt_ecb_pkcs7(&ctx,
                                      MBEDTLS_AES_ENCRYPT,
                                      true,
                                      sizeof(aes_test_ecb_pkcs7_enc),
                                      aes_test_ecb_pkcs7_enc,
                                      &olen,
                                      result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes encrypt ecb failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ecb_pkcs7_dec_pad, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ecb enc\n");
    }

finish:
    mbedtls_aes_free(&ctx);
    return ret;
}

static int mbedtls_cipher_ecb_pkcs7_dec(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[30] = {0};
    size_t olen = 0;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_dec(&ctx, aes_test_ecb_pkcs7_key,
                                 sizeof(aes_test_ecb_pkcs7_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    ret = mbedtls_aes_crypt_ecb_pkcs7(&ctx,
                                      MBEDTLS_AES_DECRYPT,
                                      true,
                                      sizeof(aes_test_ecb_pkcs7_dec_pad),
                                      aes_test_ecb_pkcs7_dec_pad,
                                      &olen,
                                      result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt ecb failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ecb_pkcs7_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ecb dec\n");
    }

finish:
    mbedtls_aes_free(&ctx);
    return ret;
}

#endif /* CE_LITE_CIPHER_PADDING_PKCS7 */

#endif /* CE_LITE_CIPHER_MODE_ECB */

#if defined(CE_LITE_CIPHER_MODE_CBC)

static int mbedtls_cipher_cbc_enc(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[256] = {0};
    uint8_t iv[16] = {0};

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_enc(&ctx, aes_test_cbc_key,
                                 sizeof(aes_test_cbc_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(iv, aes_test_cbc_iv, sizeof(iv));
    ret = mbedtls_aes_crypt_cbc(&ctx,
                                MBEDTLS_AES_ENCRYPT,
                                sizeof(aes_test_cbc_enc),
                                iv,
                                aes_test_cbc_enc,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes encrypt cbc failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_cbc_dec, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on cbc enc\n");
    }

finish:
    pal_memset(iv, 0, sizeof(iv));
    mbedtls_aes_free(&ctx);
    return ret;
}

#if defined(CE_LITE_CIPHER_MULTI_BLOCKS)

static int mbedtls_cipher_cbc_enc_multi_block(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[256] = {0};
    uint8_t iv[16] = {0};
    size_t i = 0;
    uint32_t block_size = 16;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_enc(&ctx, aes_test_cbc_key,
                                 sizeof(aes_test_cbc_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(iv, aes_test_cbc_iv, sizeof(iv));
    /* The IV will be updated every time when the crypt interface is called. */
    for (i = 0; i < sizeof(result_buf) / block_size; i++) {
        ret = mbedtls_aes_crypt_cbc(&ctx,
                                    MBEDTLS_AES_ENCRYPT,
                                    block_size,
                                    iv,
                                    aes_test_cbc_enc + i * block_size,
                                    result_buf + i * block_size);
        if (ret != 0) {
            PAL_LOG_ERR("[error -0x%x]: aes encrypt cbc failed.\n", ret);
            goto finish;
        }
    }

    ret = pal_memcmp(result_buf, aes_test_cbc_dec, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on cbc enc\n");
    }

finish:
    pal_memset(iv, 0, sizeof(iv));
    mbedtls_aes_free(&ctx);
    return ret;
}

#endif /* CE_LITE_CIPHER_MULTI_BLOCKS */

static int mbedtls_cipher_cbc_dec(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[256] = {0};
    uint8_t iv[16] = {0};

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_dec(&ctx, aes_test_cbc_key,
                                 sizeof(aes_test_cbc_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(iv, aes_test_cbc_iv, sizeof(iv));
    ret = mbedtls_aes_crypt_cbc(&ctx,
                                MBEDTLS_AES_DECRYPT,
                                sizeof(aes_test_cbc_dec),
                                iv,
                                aes_test_cbc_dec,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt cbc failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_cbc_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on cbc dec\n");
    }

finish:
    pal_memset(iv, 0, sizeof(iv));
    mbedtls_aes_free(&ctx);
    return ret;
}

#if defined(CE_LITE_CIPHER_MULTI_BLOCKS)

static int mbedtls_cipher_cbc_dec_multi_block(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[256] = {0};
    uint8_t iv[16] = {0};
    size_t i = 0;
    uint32_t block_size = 16;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_dec(&ctx, aes_test_cbc_key,
                                 sizeof(aes_test_cbc_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(iv, aes_test_cbc_iv, sizeof(iv));
    /* The IV will be updated every time when the crypt interface is called. */
    for (i = 0; i < sizeof(result_buf) / block_size; i ++) {
        ret = mbedtls_aes_crypt_cbc(&ctx,
                                    MBEDTLS_AES_DECRYPT,
                                    block_size,
                                    iv,
                                    aes_test_cbc_dec + i * block_size,
                                    result_buf + i * block_size);
        if (ret != 0) {
            PAL_LOG_ERR("[error -0x%x]: aes decrypt cbc failed.\n", ret);
            goto finish;
        }
    }

    ret = pal_memcmp(result_buf, aes_test_cbc_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on cbc dec\n");
    }

finish:
    pal_memset(iv, 0, sizeof(iv));
    mbedtls_aes_free(&ctx);
    return ret;
}

#endif /* CE_LITE_CIPHER_MULTI_BLOCKS */

#if defined(CE_LITE_CIPHER_PADDING_PKCS7)

static int mbedtls_cipher_cbc_pkcs7_enc(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[256] = {0};
    uint8_t iv[16] = {0};
    size_t olen = 0;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_enc(&ctx, aes_test_cbc_pkcs7_key,
                                 sizeof(aes_test_cbc_pkcs7_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(iv, aes_test_cbc_pkcs7_iv, sizeof(iv));
    ret = mbedtls_aes_crypt_cbc_pkcs7(&ctx,
                                      MBEDTLS_AES_ENCRYPT,
                                      true,
                                      iv,
                                      sizeof(aes_test_cbc_pkcs7_enc),
                                      aes_test_cbc_pkcs7_enc,
                                      &olen,
                                      result_buf);

    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes encrypt cbc failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_cbc_pkcs7_dec_pad,
                      sizeof(aes_test_cbc_pkcs7_dec_pad));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on cbc enc\n");
    }

finish:
    pal_memset(iv, 0, sizeof(iv));
    mbedtls_aes_free(&ctx);
    return ret;
}

static int mbedtls_cipher_cbc_pkcs7_dec(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[254] = {0};
    uint8_t iv[16] = {0};
    size_t olen = 0;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_dec(&ctx, aes_test_cbc_pkcs7_key,
                                 sizeof(aes_test_cbc_pkcs7_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(iv, aes_test_cbc_pkcs7_iv, sizeof(iv));
    ret = mbedtls_aes_crypt_cbc_pkcs7(&ctx,
                                      MBEDTLS_AES_DECRYPT,
                                      true,
                                      iv,
                                      sizeof(aes_test_cbc_pkcs7_dec_pad),
                                      aes_test_cbc_pkcs7_dec_pad,
                                      &olen,
                                      result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt cbc failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_cbc_pkcs7_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on cbc dec\n");
    }

finish:
    pal_memset(iv, 0, sizeof(iv));
    mbedtls_aes_free(&ctx);
    return ret;
}

#endif /* CE_LITE_CIPHER_PADDING_PKCS7 */

#if defined(CE_LITE_CIPHER_MULTI_BLOCKS)

static int mbedtls_cipher_cbc_pkcs7_enc_multi_block(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[256] = {0};
    uint8_t iv[16] = {0};
    size_t i = 0;
    size_t inlen = 0;
    uint32_t block_size = 16;
    bool is_last = false;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_enc(&ctx, aes_test_cbc_pkcs7_key,
                                 sizeof(aes_test_cbc_pkcs7_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(iv, aes_test_cbc_pkcs7_iv, sizeof(iv));
    /* The IV will be updated every time when the crypt interface is called. */
    for (i = 0, inlen = block_size; i < sizeof(result_buf) / block_size; i++) {
        if ((i + 1) * block_size >= sizeof(aes_test_cbc_pkcs7_enc)) {
            is_last = true;
            inlen = sizeof(aes_test_cbc_pkcs7_enc) - i * block_size;
        }

        ret = mbedtls_aes_crypt_cbc_pkcs7(&ctx,
                                          MBEDTLS_AES_ENCRYPT,
                                          is_last,
                                          iv,
                                          inlen,
                                          aes_test_cbc_pkcs7_enc + i * block_size,
                                          NULL,
                                          result_buf + i * block_size);
        if (ret != 0) {
            PAL_LOG_ERR("[error -0x%x]: aes encrypt cbc failed.\n", ret);
            goto finish;
        }
    }

    ret = pal_memcmp(result_buf, aes_test_cbc_pkcs7_dec_pad,
                      sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on cbc enc\n");
    }

finish:
    pal_memset(iv, 0, sizeof(iv));
    mbedtls_aes_free(&ctx);
    return ret;
}

static int mbedtls_cipher_cbc_pkcs7_dec_multi_block(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[256] = {0};
    uint8_t iv[16] = {0};
    size_t i = 0;
    uint32_t block_size = 16;
    bool is_last = false;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_dec(&ctx, aes_test_cbc_pkcs7_key,
                                 sizeof(aes_test_cbc_pkcs7_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(iv, aes_test_cbc_pkcs7_iv, sizeof(iv));
    /* The IV will be updated every time when the crypt interface is called. */
    for (i = 0; i < sizeof(result_buf) / block_size; i ++) {
        if ((i + 1) * block_size == sizeof(result_buf)) {
            is_last = true;
        }

        ret = mbedtls_aes_crypt_cbc_pkcs7(&ctx,
                                          MBEDTLS_AES_DECRYPT,
                                          is_last,
                                          iv,
                                          block_size,
                                          aes_test_cbc_pkcs7_dec_pad + i * block_size,
                                          NULL,
                                          result_buf + i * block_size);
        if (ret != 0) {
            PAL_LOG_ERR("[error -0x%x]: aes decrypt cbc pkcs7 failed.\n", ret);
            goto finish;
        }
    }

    ret = pal_memcmp(result_buf, aes_test_cbc_pkcs7_enc,
                      sizeof(aes_test_cbc_pkcs7_enc));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on cbc pkcs7 dec\n");
    }

finish:
    pal_memset(iv, 0, sizeof(iv));
    mbedtls_aes_free(&ctx);
    return ret;
}

#endif /* CE_LITE_CIPHER_MULTI_BLOCKS */

#if defined(CE_LITE_OTP) &&  \
    defined(CE_LITE_OTP_NORMAL) && \
    defined(CE_LITE_OTP_WRITE) && \
    defined(CE_LITE_EXAMPLE_AES_CBC_SECKEY)

static int _aes_cbc_keyladder_init(void)
{
    return otp_rootkey_init();
}

static int mbedtls_cipher_cbc_keyladder_enc(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[16] = {0};
    uint8_t iv[16] = {0};
    mbedtls_aes_sec_key_t sec_key = {0};

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    pal_memcpy(sec_key.ek1, aes_test_cbc_ek1,
               sizeof(aes_test_cbc_ek1));
    pal_memcpy(sec_key.ek2, aes_test_cbc_ek2,
               sizeof(aes_test_cbc_ek2));
    pal_memcpy(sec_key.ek3, aes_test_cbc_ek3,
               sizeof(aes_test_cbc_ek3));
    sec_key.ek1bits = sizeof(aes_test_cbc_ek1) << 3;
    sec_key.sel = MBEDTLS_AES_KL_KEY_ROOT;

    ret = mbedtls_aes_setseckey_enc(&ctx, &sec_key);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set secure key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(iv, aes_test_cbc_klad_iv, sizeof(iv));
    ret = mbedtls_aes_crypt_cbc(&ctx,
                                MBEDTLS_AES_ENCRYPT,
                                sizeof(aes_test_cbc_keyladder_enc),
                                iv,
                                aes_test_cbc_keyladder_enc,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes encrypt cbc failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_cbc_keyladder_dec, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on cbc enc\n");
    }

finish:
    pal_memset(&sec_key, 0, sizeof(mbedtls_aes_sec_key_t));
    mbedtls_aes_free(&ctx);
    return ret;
}

static int mbedtls_cipher_cbc_keyladder_dec(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[16] = {0};
    uint8_t iv[16] = {0};
    mbedtls_aes_sec_key_t sec_key = {0};

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    pal_memcpy(sec_key.ek1, aes_test_cbc_ek1,
               sizeof(aes_test_cbc_ek1));
    pal_memcpy(sec_key.ek2, aes_test_cbc_ek2,
               sizeof(aes_test_cbc_ek2));
    pal_memcpy(sec_key.ek3, aes_test_cbc_ek3,
               sizeof(aes_test_cbc_ek3));
    sec_key.ek1bits = sizeof(aes_test_cbc_ek1) << 3;
    sec_key.sel = MBEDTLS_AES_KL_KEY_ROOT;

    ret = mbedtls_aes_setseckey_dec(&ctx, &sec_key);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set secure key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(iv, aes_test_cbc_klad_iv, sizeof(iv));
    ret = mbedtls_aes_crypt_cbc(&ctx,
                                MBEDTLS_AES_DECRYPT,
                                sizeof(aes_test_cbc_keyladder_dec),
                                iv,
                                aes_test_cbc_keyladder_dec,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt cbc failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_cbc_keyladder_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on cbc dec\n");
    }

finish:
    pal_memset(&sec_key, 0, sizeof(mbedtls_aes_sec_key_t));
    mbedtls_aes_free(&ctx);
    return ret;
}

#endif /* CE_LITE_OTP && CE_LITE_OTP_NORMAL && CE_LITE_OTP_WRITE && \
        * CE_LITE_EXAMPLE_AES_CBC_SECKEY */

#endif /* CE_LITE_CIPHER_MODE_CBC */

#if defined(CE_LITE_CIPHER_MODE_CTR)

static int mbedtls_cipher_ctr_enc(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[513] = {0};
    uint8_t nounce_counter[16] = {0};
    uint8_t strblk[16] = {0};
    size_t nc_off = 0;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_enc(&ctx, aes_test_ctr_key,
                                 sizeof(aes_test_ctr_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(nounce_counter, aes_test_ctr_iv, sizeof(nounce_counter));
    pal_memcpy(strblk, aes_test_ctr_strblk, sizeof(strblk));
    ret = mbedtls_aes_crypt_ctr(&ctx,
                                sizeof(aes_test_ctr_enc),
                                &nc_off,
                                nounce_counter,
                                strblk,
                                aes_test_ctr_enc,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes encrypt ctr failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ctr_dec, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ctr enc\n");
    }

finish:
    pal_memset(nounce_counter, 0, sizeof(nounce_counter));
    pal_memset(strblk, 0, sizeof(strblk));
    mbedtls_aes_free(&ctx);
    return ret;
}

#if defined(CE_LITE_CIPHER_MULTI_BLOCKS)

static int mbedtls_cipher_ctr_enc_multi(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[513] = {0};
    uint8_t nounce_counter[16] = {0};
    uint8_t strblk[16] = {0};
    size_t nc_off = 0;
    size_t prolen = 0;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_enc(&ctx, aes_test_ctr_key,
                                 sizeof(aes_test_ctr_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(nounce_counter, aes_test_ctr_iv, sizeof(nounce_counter));
    pal_memcpy(strblk, aes_test_ctr_strblk, sizeof(strblk));

    /* Encrypt 147 bytes data at first. */
    prolen = 147;
    ret = mbedtls_aes_crypt_ctr(&ctx,
                                prolen,
                                &nc_off,
                                nounce_counter,
                                strblk,
                                aes_test_ctr_enc,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes encrypt ctr failed.\n", ret);
        goto finish;
    }
    /* Encrypt 513 - 147 bytes data at second. */
    /* The nounce_counter and stream block are updated for the next crypt. */
    ret = mbedtls_aes_crypt_ctr(&ctx,
                                sizeof(result_buf) - prolen,
                                &nc_off,
                                nounce_counter,
                                strblk,
                                aes_test_ctr_enc + prolen,
                                result_buf + prolen);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes encrypt ctr failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ctr_dec, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ctr enc\n");
    }

finish:
    pal_memset(nounce_counter, 0, sizeof(nounce_counter));
    pal_memset(strblk, 0, sizeof(strblk));
    mbedtls_aes_free(&ctx);
    return ret;
}

#endif /* CE_LITE_CIPHER_MULTI_BLOCKS */

static int mbedtls_cipher_ctr_dec(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[513] = {0};
    uint8_t nounce_counter[16] = {0};
    uint8_t strblk[16] = {0};
    size_t nc_off = 0;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_dec(&ctx, aes_test_ctr_key,
                                 sizeof(aes_test_ctr_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(nounce_counter, aes_test_ctr_iv, sizeof(nounce_counter));
    pal_memcpy(strblk, aes_test_ctr_strblk, sizeof(strblk));
    ret = mbedtls_aes_crypt_ctr(&ctx,
                                sizeof(aes_test_ctr_dec),
                                &nc_off,
                                nounce_counter,
                                strblk,
                                aes_test_ctr_dec,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt ctr failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ctr_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ctr dec\n");
    }

finish:
    pal_memset(nounce_counter, 0, sizeof(nounce_counter));
    pal_memset(strblk, 0, sizeof(strblk));
    mbedtls_aes_free(&ctx);
    return ret;
}

#if defined(CE_LITE_CIPHER_MULTI_BLOCKS)

static int mbedtls_cipher_ctr_dec_multi(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[513] = {0};
    uint8_t nounce_counter[16] = {0};
    uint8_t strblk[16] = {0};
    size_t nc_off = 0;
    size_t prolen = 0;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_dec(&ctx, aes_test_ctr_key,
                                 sizeof(aes_test_ctr_key) << 3);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set user key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(nounce_counter, aes_test_ctr_iv, sizeof(nounce_counter));
    pal_memcpy(strblk, aes_test_ctr_strblk, sizeof(strblk));

    /* decrypt 194 bytes at first. */
    prolen = 194;
    ret = mbedtls_aes_crypt_ctr(&ctx,
                                prolen,
                                &nc_off,
                                nounce_counter,
                                strblk,
                                aes_test_ctr_dec,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt ctr failed.\n", ret);
        goto finish;
    }
    /* decrypt 513 - 194 bytes at second. */
    /* The nounce_counter and stream block are updated for the next crypt. */
    ret = mbedtls_aes_crypt_ctr(&ctx,
                                sizeof(result_buf) - prolen,
                                &nc_off,
                                nounce_counter,
                                strblk,
                                aes_test_ctr_dec + prolen,
                                result_buf + prolen);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt ctr failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ctr_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ctr dec str\n");
    }

finish:
    pal_memset(nounce_counter, 0, sizeof(nounce_counter));
    pal_memset(strblk, 0, sizeof(strblk));
    mbedtls_aes_free(&ctx);
    return ret;
}

#endif /* CE_LITE_CIPHER_MULTI_BLOCKS */

#if defined(CE_LITE_OTP) &&  \
    defined(CE_LITE_OTP_NORMAL) && \
    defined(CE_LITE_OTP_WRITE) && \
    defined(CE_LITE_EXAMPLE_AES_CTR_SECKEY)

static int _aes_ctr_keyladder_init(void)
{
    return otp_rootkey_init();
}

static int mbedtls_cipher_ctr_keyladder_enc(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[16] = {0};
    uint8_t nounce_counter[16] = {0};
    uint8_t strblk[16] = {0};
    mbedtls_aes_sec_key_t sec_key = {0};
    size_t nc_off = 0;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    pal_memcpy(sec_key.ek1, aes_test_ctr_ek1,
               sizeof(aes_test_ctr_ek1));
    pal_memcpy(sec_key.ek2, aes_test_ctr_ek2,
               sizeof(aes_test_ctr_ek2));
    pal_memcpy(sec_key.ek3, aes_test_ctr_ek3,
               sizeof(aes_test_ctr_ek3));
    sec_key.ek1bits = sizeof(aes_test_ctr_ek1) << 3;
    sec_key.sel = MBEDTLS_AES_KL_KEY_ROOT;

    ret = mbedtls_aes_setseckey_enc(&ctx, &sec_key);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set secure key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(nounce_counter, aes_test_ctr_klad_iv, sizeof(nounce_counter));
    ret = mbedtls_aes_crypt_ctr(&ctx,
                                sizeof(aes_test_ctr_keyladder_enc),
                                &nc_off,
                                nounce_counter,
                                strblk,
                                aes_test_ctr_keyladder_enc,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes encrypt ctr failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ctr_keyladder_dec, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ctr enc\n");
    }

finish:
    pal_memset(&sec_key, 0, sizeof(mbedtls_aes_sec_key_t));
    pal_memset(strblk, 0, sizeof(strblk));
    mbedtls_aes_free(&ctx);
    return ret;
}

static int mbedtls_cipher_ctr_keyladder_dec(void)
{
    int ret = 0;
    mbedtls_aes_context ctx;
    uint8_t result_buf[16] = {0};
    uint8_t nounce_counter[16] = {0};
    uint8_t strblk[16] = {0};
    mbedtls_aes_sec_key_t sec_key = {0};
    size_t nc_off = 0;

    pal_memset(&ctx, 0, sizeof(mbedtls_aes_context));
    mbedtls_aes_init(&ctx);

    pal_memcpy(sec_key.ek1, aes_test_ctr_ek1,
               sizeof(aes_test_ctr_ek1));
    pal_memcpy(sec_key.ek2, aes_test_ctr_ek2,
               sizeof(aes_test_ctr_ek2));
    pal_memcpy(sec_key.ek3, aes_test_ctr_ek3,
               sizeof(aes_test_ctr_ek3));
    sec_key.ek1bits = sizeof(aes_test_ctr_ek1) << 3;
    sec_key.sel = MBEDTLS_AES_KL_KEY_ROOT;

    ret = mbedtls_aes_setseckey_dec(&ctx, &sec_key);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: set secure key failed.\n", ret);
        goto finish;
    }

    pal_memcpy(nounce_counter, aes_test_ctr_klad_iv, sizeof(nounce_counter));
    ret = mbedtls_aes_crypt_ctr(&ctx,
                                sizeof(aes_test_ctr_keyladder_dec),
                                &nc_off,
                                nounce_counter,
                                strblk,
                                aes_test_ctr_keyladder_dec,
                                result_buf);
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: aes decrypt ctr failed.\n", ret);
        goto finish;
    }

    ret = pal_memcmp(result_buf, aes_test_ctr_keyladder_enc, sizeof(result_buf));
    if (ret != 0) {
        PAL_LOG_ERR("[selftest failed] on ctr dec\n");
    }

finish:
    pal_memset(&sec_key, 0, sizeof(mbedtls_aes_sec_key_t));
    pal_memset(strblk, 0, sizeof(strblk));
    mbedtls_aes_free(&ctx);
    return ret;
}

#endif /* CE_LITE_OTP && CE_LITE_OTP_NORMAL && CE_LITE_OTP_WRITE && \
        * CE_LITE_EXAMPLE_AES_CTR_SECKEY*/

#endif /* CE_LITE_CIPHER_MODE_CTR */

#define CHECK_RET_RESULT(ret)           \
    do {                                \
        if ((ret) == 0) {               \
            PAL_LOG_INFO("[pass]\n");   \
        } else {                        \
            PAL_LOG_INFO("[fail]\n");   \
        }                               \
    } while(0);

int mbedtls_aes_example_entry(void)
{
    int ret = 0;

    PAL_LOG_INFO("Run mbedtls aes examples.\n");

#if defined(CE_LITE_CIPHER_MODE_ECB)
    /* Example 1: cipher mode is ECB. */
    PAL_LOG_INFO(" * Run mbedtls_cipher_ecb example of encryption...");
    ret = mbedtls_aes_cipher_ecb_enc();
    CHECK_RET_RESULT(ret);

    PAL_LOG_INFO(" * Run mbedtls_cipher_ecb example of decryption...");
    ret = mbedtls_aes_cipher_ecb_dec();
    CHECK_RET_RESULT(ret);

#if defined(CE_LITE_OTP) &&  \
    defined(CE_LITE_OTP_NORMAL) && \
    defined(CE_LITE_OTP_WRITE) && \
    defined(CE_LITE_EXAMPLE_AES_ECB_SECKEY)

    ret = _aes_ecb_keyladder_init();
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: ecb keyladder init failed.\n", ret);
    } else {
        /**
         * Example 2: cipher mode is ECB using keyladder.
         *
         * \note  In order to use the example, please ensure that:
         *        1. PUF mode is disable.
         *        2. LCS(life cycle status) is CM or DM.
         *        3. If you use rootkey, make sure the region of rootkey is blank
         *           and haven't been locked. Modelkey is the same.
         *        4. Do it once and only once in a power cycle.
         *        5. Call \p _aes_ecb_keyladder_init() to set rootkey(in the example) before running
         *           the \p mbedtls_cipher_ecb_keyladder_enc() or \p mbedtls_cipher_ecb_keyladder_dec()
         */
        PAL_LOG_INFO(" * Run mbedtls_cipher_ecb keyladder example of encryption...");
        ret = mbedtls_cipher_ecb_keyladder_enc();
        CHECK_RET_RESULT(ret);

        PAL_LOG_INFO(" * Run mbedtls_cipher_ecb keyladder example of decryption...");
        ret = mbedtls_cipher_ecb_keyladder_dec();
        CHECK_RET_RESULT(ret);
    }
#endif /* CE_LITE_OTP && CE_LITE_OTP_NORMAL && CE_LITE_OTP_WRITE && \
        * CE_LITE_EXAMPLE_AES_ECB_SECKEY */

    /* Example 3: cipher mode is ECB ext interfaces. */
    PAL_LOG_INFO(" * Run mbedtls_cipher_ecb_ext example of encryption...");
    ret = mbedtls_cipher_ecb_ext_enc();
    CHECK_RET_RESULT(ret);

    PAL_LOG_INFO(" * Run mbedtls_cipher_ecb_ext example of decryption...");
    ret = mbedtls_cipher_ecb_ext_dec();
    CHECK_RET_RESULT(ret);

#if defined(CE_LITE_CIPHER_PADDING_PKCS7)
    /* Example 4: cipher mode is ECB ext pkcs7 interfaces. */
    PAL_LOG_INFO(" * Run mbedtls_cipher_ecb pkcs7 example of encryption...");
    ret = mbedtls_cipher_ecb_pkcs7_enc();
    CHECK_RET_RESULT(ret);

    PAL_LOG_INFO(" * Run mbedtls_cipher_ecb pkcs7 example of decryption...");
    ret = mbedtls_cipher_ecb_pkcs7_dec();
    CHECK_RET_RESULT(ret);

#endif /* CE_LITE_CIPHER_PADDING_PKCS7 */

#endif /* CE_LITE_CIPHER_MODE_ECB */

#if defined(CE_LITE_CIPHER_MODE_CBC)
    /* Example 5: cipher mode is CBC single block. */
    PAL_LOG_INFO(" * Run mbedtls_cipher_cbc example of encryption...");
    ret = mbedtls_cipher_cbc_enc();
    CHECK_RET_RESULT(ret);

    PAL_LOG_INFO(" * Run mbedtls_cipher_cbc example of decryption...");
    ret = mbedtls_cipher_cbc_dec();
    CHECK_RET_RESULT(ret);

#if defined(CE_LITE_CIPHER_MULTI_BLOCKS)
    /* Example 6: cipher mode is CBC multiple-blocks with iv update. */
    PAL_LOG_INFO(" * Run mbedtls_cipher_cbc example of multiple-block encryption...");
    ret = mbedtls_cipher_cbc_enc_multi_block();
    CHECK_RET_RESULT(ret);

    PAL_LOG_INFO(" * Run mbedtls_cipher_cbc example of multiple-block decryption...");
    ret = mbedtls_cipher_cbc_dec_multi_block();
    CHECK_RET_RESULT(ret);

#endif /* CE_LITE_CIPHER_MULTI_BLOCKS */

#if defined(CE_LITE_CIPHER_PADDING_PKCS7)
    /* Example 7: cipher mode is CBC pkcs7 multiple-blocks. */
    PAL_LOG_INFO(" * Run mbedtls_cipher_cbc pkcs7 example of encryption...");
    ret = mbedtls_cipher_cbc_pkcs7_enc();
    CHECK_RET_RESULT(ret);

    PAL_LOG_INFO(" * Run mbedtls_cipher_cbc pkcs7 example of decryption...");
    ret = mbedtls_cipher_cbc_pkcs7_dec();
    CHECK_RET_RESULT(ret);

#endif /* CE_LITE_CIPHER_PADDING_PKCS7 */

#if defined(CE_LITE_CIPHER_MULTI_BLOCKS)
    /* Example 8: cipher mode is CBC pkcs7 multiple-blocks with iv update. */
    PAL_LOG_INFO(" * Run mbedtls_cipher_cbc pkcs7 example of multiple-block encryption...");
    ret = mbedtls_cipher_cbc_pkcs7_enc_multi_block();
    CHECK_RET_RESULT(ret);

    PAL_LOG_INFO(" * Run mbedtls_cipher_cbc pkcs7 example of multiple-block decryption...");
    ret = mbedtls_cipher_cbc_pkcs7_dec_multi_block();
    CHECK_RET_RESULT(ret);

#endif /* CE_LITE_CIPHER_MULTI_BLOCKS */

#if defined(CE_LITE_OTP) &&  \
    defined(CE_LITE_OTP_NORMAL) && \
    defined(CE_LITE_OTP_WRITE) && \
    defined(CE_LITE_EXAMPLE_AES_CBC_SECKEY)

    ret = _aes_cbc_keyladder_init();
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: cbc keyladder init failed.\n", ret);
    } else {
        /**
         * Example 9: cipher mode is CBC using keyladder.
         *
         * \note  In order to use the example, please ensure that:
         *        1. PUF mode is disable.
         *        2. LCS(life cycle status) is CM or DM.
         *        3. If you use rootkey, make sure the region of rootkey is blank
         *           and haven't been locked. Modelkey is the same.
         *        4. Do it once and only once in a power cycle.
         *        5. Call \p _aes_cbc_keyladder_init() to set rootkey(in the example) before running
         *           the \p mbedtls_cipher_cbc_keyladder_enc() or \p mbedtls_cipher_cbc_keyladder_dec()
         */
        PAL_LOG_INFO(" * Run mbedtls_cipher_cbc keyladder example of encryption...");
        ret = mbedtls_cipher_cbc_keyladder_enc();
        CHECK_RET_RESULT(ret);

        PAL_LOG_INFO(" * Run mbedtls_cipher_cbc keyladder example of decryption...");
        ret = mbedtls_cipher_cbc_keyladder_dec();
        CHECK_RET_RESULT(ret);
    }
#endif /* CE_LITE_OTP && CE_LITE_OTP_NORMAL && CE_LITE_OTP_WRITE && \
        * CE_LITE_EXAMPLE_AES_CBC_SECKEY */

#endif /* CE_LITE_CIPHER_MODE_CBC */

#if defined(CE_LITE_CIPHER_MODE_CTR)
    /* Example 10: cipher mode is CTR single data. */
    PAL_LOG_INFO(" * Run mbedtls_cipher_ctr example of encryption...");
    ret = mbedtls_cipher_ctr_enc();
    CHECK_RET_RESULT(ret);

    PAL_LOG_INFO(" * Run mbedtls_cipher_ctr example of decryption...");
    ret = mbedtls_cipher_ctr_dec();
    CHECK_RET_RESULT(ret);

#if defined(CE_LITE_CIPHER_MULTI_BLOCKS)
    /* Example 11: cipher mode is CTR multiple sliced datas with iv update. */
    PAL_LOG_INFO(" * Run mbedtls_cipher_ctr example of multiple encryption...");
    ret = mbedtls_cipher_ctr_enc_multi();
    CHECK_RET_RESULT(ret);

    PAL_LOG_INFO(" * Run mbedtls_cipher_ctr example of multiple decryption...");
    ret = mbedtls_cipher_ctr_dec_multi();
    CHECK_RET_RESULT(ret);
#endif /* CE_LITE_CIPHER_MULTI_BLOCKS */

#if defined(CE_LITE_OTP) &&  \
    defined(CE_LITE_OTP_NORMAL) && \
    defined(CE_LITE_OTP_WRITE) && \
    defined(CE_LITE_EXAMPLE_AES_CTR_SECKEY)

    ret = _aes_ctr_keyladder_init();
    if (ret != 0) {
        PAL_LOG_ERR("[error -0x%x]: ctr keyladder init failed.\n", ret);
    } else {
        /**
         * Example 12: cipher mode is CTR using keyladder.
         *
         * \note  In order to use the example, please ensure that:
         *        1. PUF mode is disable.
         *        2. LCS(life cycle status) is CM or DM.
         *        3. If you use rootkey, make sure the region of rootkey is blank
         *           and haven't been locked. Modelkey is the same.
         *        4. Do it once and only once in a power cycle.
         *        5. Call \p _aes_ctr_keyladder_init() to set rootkey(in the example) before running
         *           the \p mbedtls_cipher_ctr_keyladder_enc() or \p mbedtls_cipher_ctr_keyladder_dec()
         */
        PAL_LOG_INFO(" * Run mbedtls_cipher_ctr keyladder example of encryption...");
        ret = mbedtls_cipher_ctr_keyladder_enc();
        CHECK_RET_RESULT(ret);

        PAL_LOG_INFO(" * Run mbedtls_cipher_ctr keyladder example of decryption...");
        ret = mbedtls_cipher_ctr_keyladder_dec();
        CHECK_RET_RESULT(ret);
    }
#endif /* CE_LITE_OTP && CE_LITE_OTP_NORMAL && CE_LITE_OTP_WRITE && \
        * CE_LITE_EXAMPLE_AES_CTR_SECKEY */

#endif /* CE_LITE_CIPHER_MODE_CTR */

    PAL_LOG_INFO("Finish mbedtls aes examples.\n");

    return ret;
}

#endif /* CE_LITE_AES */
