#include "base.h"
#include "sharedm.h"
#include "bk_uart.h"

void mem_access_test(uint32_t start_addr, uint32_t len)
{
	uint8_t *addr = (uint8_t *)start_addr;
	uint32_t i, err_cnt = 0;

	for(i = 0; i < len; i ++)
	{
		addr[i] = i & 0xff;
	}
	
	for(i = 0; i < len; i ++)
	{
		if(addr[i] != (i & 0xFF)){
			bk_printf("addr(0x%x)[i]=0x%02x, i & 0xff==0x%02x\r\n", &addr[i], addr[i], i & 0xff);
			err_cnt ++;
		}
	}

	bk_printf("\r\n", err_cnt);
}

void secure_world_access_s_shared_memory(void)
{
	mem_access_test(SECURE_SHAREDM_START_ADDR, VERIFY_DATA_LEN);
}

void secure_world_access_ns_shared_memory(void)
{
	mem_access_test(NON_SECURE_SHAREDM_START_ADDR, VERIFY_DATA_LEN);
}

void shared_memory_verification_main(void)
{
	secure_world_access_s_shared_memory();
	secure_world_access_ns_shared_memory();
}
// eof

