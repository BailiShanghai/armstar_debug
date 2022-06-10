#include "base.h"
#include "bk_uart.h"
#include "v_flash.h"
#include "bk_flash.h"

#if CONFIG_AT_SECURE_WORLD
mpc_hw_t *mpc_flash;

uint32_t vrf_flash_config(void)
{
	mpc_flash = (mpc_hw_t *)MPC_FLASH_BASE_ADDR;
	
	mpc_config_ctrl(mpc_flash, 0, 0, 0, 0);
	mpc_flash->blk_lut[0] = 0x0C;
	
	return 0;
}

uint32_t spe_config_sec_reg(void)
{
	SEC_REG_SETTING_T s_reg = {
		SEC_REG_ID0,
		STA_NON_SEC,
		SEC_REG_ENABLE,
		TEST_NS_START_ADDR,
		TEST_NS_END_ADDR
		};
	SEC_REG_SETTING_T ns_reg = {
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
#endif

uint32_t data_access_scan(uint32_t start, uint32_t end)
{
	uint32_t *head_ptr = (uint32_t *)start;
	uint32_t *tail_ptr = (uint32_t *)(end - 32);
	uint8_t *h, *t;
	uint32_t pos, i = 0;

	bk_printf("data_access_scan:0x%x:0x%x\r\n", start, end);
	
	*head_ptr = 0x5a5a5a5a;
	if(0x5a5a5a5a != *head_ptr){
		bk_printf("head_failed:0x5a5a5a5a != 0x%x\r\n", *head_ptr);
	}
	*tail_ptr = 0x5a5a5a5a;
	if(0x5a5a5a5a != *tail_ptr){
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

uint32_t flash_data_access_proof_main(void)
{
#if CONFIG_AT_SECURE_WORLD
	spe_config_sec_reg();
#endif
	
	spe_access_non_sec_reg();
	spe_access_sec_reg();
	
	return 0;
}

// eof

