#include "base.h"
#include "shared_mem.h"
#include "bk_uart.h"
#include "base.h"
#include "shared_mem.h"
#include "bk_uart.h"

uint32_t mem_access_test(uint32_t start, uint32_t end)
{
	uint32_t *head_ptr = (uint32_t *)start;
	uint32_t *tail_ptr = (uint32_t *)(end - 8);
	uint8_t *h, *t;
	uint32_t pos, i = 0;

	bk_printf("ns_mem_access:0x%x:0x%x\r\n", start, end);
	
	*head_ptr = 0x5a5a5a5a;
	if(0x5a5a5a5a != *head_ptr){
		bk_printf("head_failed:0x5a5a5a5a != 0x%x\r\n", *head_ptr);
		return -1;
	}
	*tail_ptr = 0x5a5a5a5a;
	if(0x5a5a5a5a != *tail_ptr){
		bk_printf("tail_failed:0x5a5a5a5a != 0x%x\r\n", *tail_ptr);
		return -1;
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

void nsecure_world_access_s_shared_memory(void)
{
	mem_access_test(SECURE_SHAREDM0_START_ADDR, SECURE_SHAREDM0_START_ADDR + VERIFY_DATA_LEN);
}

void nsecure_world_access_ns_shared_memory(void)
{
	mem_access_test(NON_SECURE_SHAREDM0_START_ADDR, NON_SECURE_SHAREDM0_START_ADDR + VERIFY_DATA_LEN);
	mem_access_test(NON_SECURE_SHAREDM1_START_ADDR, NON_SECURE_SHAREDM1_START_ADDR + VERIFY_DATA_LEN);
	mem_access_test(NON_SECURE_SHAREDM2_START_ADDR, NON_SECURE_SHAREDM2_START_ADDR + VERIFY_DATA_LEN);
	mem_access_test(NON_SECURE_SHAREDM3_START_ADDR, NON_SECURE_SHAREDM3_START_ADDR + VERIFY_DATA_LEN);
	mem_access_test(NON_SECURE_SHAREDM4_START_ADDR, NON_SECURE_SHAREDM4_START_ADDR + VERIFY_DATA_LEN);
}

void shared_memory_proof_main(void)
{
	bk_printf("ns_world_access_ns_shared_memory\r\n");
	nsecure_world_access_ns_shared_memory();
	
	bk_printf("ns_world_access_s_shared_memory\r\n");
	nsecure_world_access_s_shared_memory();
}
// eof

