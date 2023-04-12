#include "base.h"
#include "bk_uart.h"
#include "alg_aes.h"

extern int mbedtls_aes_cipher_ecb_enc(void);
extern int mbedtls_aes_cipher_ecb_dec(void);
extern int mbedtls_aes_example_entry(void);

void alg_aes_verification_main(void)
{
    int ret;

    bk_printf(" * Run mbedtls_cipher_ecb example of encryption...\r\n");
    ret = mbedtls_aes_cipher_ecb_enc();
    if ((ret) == 0)
    {
        bk_printf("[cipher_ecb_enc pass]\n");
    }
    else
    {
        bk_printf("[cipher_ecb_enc fail]\n");
    }

    bk_printf(" * Run mbedtls_cipher_ecb example of decryption...\r\n");
    ret = mbedtls_aes_cipher_ecb_dec();
    if ((ret) == 0)
    {
        bk_printf("[cipher_ecb_dec pass]\n");
    }
    else
    {
        bk_printf("[cipher_ecb_dec fail]\n");
    }

    mbedtls_aes_example_entry();
}
// eof

