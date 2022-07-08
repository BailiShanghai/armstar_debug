#include "base.h"
#include "bk_uart.h"
#include "post_sim.h"

uint32_t data_access_mem(uint32_t start_addr, uint32_t end_addr)
{
	uint32_t *head_ptr = (uint32_t *)start_addr;
	uint32_t *tail_ptr = (uint32_t *)(end_addr - 10);
	uint8_t *h, *t;
	uint32_t pos, i = 0;
	
	bk_printf("data_access_mem 0x%x--0x%x\r\n", start_addr, end_addr);

	*head_ptr = 0x5a5a5a5a;
	if(0x5a5a5a5a != *head_ptr){
		bk_printf("head_failed:0x5a5a5a5a != 0x%x\r\n", *head_ptr);
	}
	*tail_ptr = 0x5a5a5a5a;
	if(0x5a5a5a5a != *tail_ptr){
		bk_printf("tail_failed:0x5a5a5a5a != 0x%x\r\n", *tail_ptr);
	}
	
	h = (uint8_t *)start_addr;
	pos = start_addr;
	while(pos < end_addr)
	{
		h[i] = i & 0xFF;
		i ++;
		
		pos += 1;
	}

	i = 0;
	pos = start_addr;
	while(pos < end_addr)
	{
		if(h[i] != (i & 0xFF))
			bk_printf("h[i]:0x%02x, (i & 0xFF)::0x%02x\r\n", h[i], i & 0xFF);
		i ++;
		
		pos += 1;
	}

	return 0;
}

extern uint32_t dtcm_init(void);
extern uint32_t dtcm_printf_ro_info(void);

uint32_t post_sim_verification_main(void)
{
	dtcm_init();
	dtcm_printf_ro_info();
	
#if CONFIG_ENABLE_TEST_MORE	
	bk_printf("data_access_dtcm\r\n");
	data_access_mem(VERIFY_DTCM_START_ADDR, VERIFY_DTCM_END_ADDR);

	bk_printf("data_access_SHARED_MEM0\r\n");
	data_access_mem(VERIFY_SHARED_MEM0_START_ADDR, VERIFY_SHARED_MEM0_END_ADDR);
	
	bk_printf("data_access_SHARED_MEM1\r\n");
	data_access_mem(VERIFY_SHARED_MEM1_START_ADDR, VERIFY_SHARED_MEM1_END_ADDR);
	
	bk_printf("data_access_SHARED_MEM2\r\n");
	data_access_mem(VERIFY_SHARED_MEM2_START_ADDR, VERIFY_SHARED_MEM2_END_ADDR);
	
	bk_printf("data_access_SHARED_MEM3\r\n");
	data_access_mem(VERIFY_SHARED_MEM3_START_ADDR, VERIFY_SHARED_MEM3_END_ADDR);
	
	bk_printf("data_access_SHARED_MEM4\r\n");
	data_access_mem(VERIFY_SHARED_MEM4_START_ADDR, VERIFY_SHARED_MEM4_END_ADDR);
	
	bk_printf("data_access_itcm\r\n");
	data_access_mem(VERIFY_ITCM_START_ADDR, VERIFY_ITCM_END_ADDR);
#endif

	return 0;
}
// eof

