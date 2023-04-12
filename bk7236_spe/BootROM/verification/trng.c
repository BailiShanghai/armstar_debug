#include "base.h"
#include "bk_uart.h"
#include "trng.h"
#include "ce_lite_trng.h"

uint8_t trn_content[CFG_READ_RANDOM_LEN] = {0};

void trng_printf_trn_content(void)
{
    uint32_t i;

    for(i = 0; i < sizeof(trn_content); i ++)
    {
        bk_printf("trn_content[%d] = 0x%02x\n", i, trn_content[i]);
    }
    bk_printf("\r\n");
}

int32_t trng_verification_main(void)
{
    int32_t ret = 0;

    bk_printf("trng_verification_main\n");
    ret = ce_trng_read(trn_content, sizeof(trn_content));
    if (ret != 0)
    {
        bk_printf("ce_trng_read FAILED(%X)\n", ret);
    }
    else
    {
        bk_printf("ce_trng_read SUCCESS\n");
        bk_printf("random data:", trn_content, sizeof(trn_content));
    }

    trng_printf_trn_content();

    return ret;
}
// eof
