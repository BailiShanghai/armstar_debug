#include "base.h"
#include "bk_uart.h"
#include "vrf_flash.h"
#include "mpc.h"
#include "bk_flash.h"
#include "verification_config.h"

#if CONFIG_ENABLE_VERIFY_FLASH_DBUS
#define REG_READ(addr) (*((volatile uint32_t*)(addr)))
#define REG_WRITE(addr, v) (*((volatile uint32_t*)(addr))) = (v)

void verify_flash_dbus(void)
{
#define PPRO_0x6  0x41040024
#define FLASH_0xd 0x44030034
#define FLASH_0xe 0x44030038
        bk_printf("Before NS modify flash 0xd/0xe:\r\n");
        bk_printf("flash reg xd/%x: %x\r\n", FLASH_0xd, REG_READ(FLASH_0xd));
        bk_printf("flash reg xe/%x: %x\r\n", FLASH_0xe, REG_READ(FLASH_0xe));
	NSC_verify_flash_dbus();
}
#endif

mpc_hw_t *mpc_flash;
uint32_t vrf_flash_config(void)
{
    mpc_flash = (mpc_hw_t *)MPC_FLASH_BASE_ADDR;

    mpc_config_ctrl(mpc_flash, 1, 0, 0, 0);

#if (CONFIG_ENABLE_VERIFY_MULTI_CORE0_DCACHE || CONFIG_ENABLE_VERIFY_MULTI_CORE1_DCACHE)
    mpc_flash->blk_lut[0] = 0x0;
#else
    mpc_flash->blk_lut[0] = 0x0C;
#endif

#if CONFIG_ENABLE_VERIFY_FLASH_DBUS
    verify_flash_dbus();
#endif

    return 0;
}

uint32_t spe_config_sec_reg(void)
{
    SEC_REG_SETTING_T s_reg =
    {
        SEC_REG_ID0,
        STA_NON_SEC,
        SEC_REG_ENABLE,
        TEST_NS_START_ADDR,
        TEST_NS_END_ADDR
    };
    SEC_REG_SETTING_T ns_reg =
    {
        SEC_REG_ID1,
        STA_SEC,
        SEC_REG_ENABLE,
        TEST_S_START_ADDR,
        TEST_S_END_ADDR
    };

    flash_config_reg(&s_reg);
    flash_config_reg(&ns_reg);

    return 0;
}

uint32_t data_access_scan(uint32_t start, uint32_t end)
{
    uint32_t *head_ptr = (uint32_t *)start;
    uint32_t *tail_ptr = (uint32_t *)(end - 32);
    uint8_t *h, *t;
    uint32_t pos, i = 0;

    bk_printf("data_access_scan:0x%x:0x%x\r\n", start, end);

    *head_ptr = 0x5a5a5a5a;
    if(0x5a5a5a5a != *head_ptr)
    {
        bk_printf("head_failed:0x5a5a5a5a != 0x%x\r\n", *head_ptr);
    }
    *tail_ptr = 0x5a5a5a5a;
    if(0x5a5a5a5a != *tail_ptr)
    {
        bk_printf("tail_failed:0x5a5a5a5a != 0x%x\r\n", *tail_ptr);
    }

    h = (uint8_t *)start;
    pos = start;
    while(pos < end)
    {
        h[i] = i & 0xFF;
        i ++;

        pos += 1;
    }

    i = 0;
    pos = start;
    while(pos < end)
    {
        if(h[i] != (i & 0xFF))
            bk_printf("h[i]:0x%02x, (i & 0xFF)::0x%02x\r\n", h[i], i & 0xFF);
        i ++;

        pos += 1;
    }

    return 0;
}

uint32_t spe_access_sec_reg(void)
{
    data_access_scan(TEST_S_START_ADDR, TEST_S_END_ADDR);

    return 0;
}


uint32_t spe_access_non_sec_reg(void)
{
    data_access_scan(TEST_NS_START_ADDR, TEST_NS_END_ADDR);

    return 0;
}

uint32_t flash_data_access_verification_main(void)
{
    spe_config_sec_reg();

    //spe_access_sec_reg();
    //spe_access_non_sec_reg();

    return 0;
}

// eof

