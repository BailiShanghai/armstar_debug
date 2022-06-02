#include "base.h"
#include "sharedm.h"
#include "bk_uart.h"
#include "mpc.h"

uint32_t mem_access_test(uint32_t start, uint32_t end)
{
	uint32_t *head_ptr = (uint32_t *)start;
	uint32_t *tail_ptr = (uint32_t *)(end - 32);
	uint8_t *h, *t;
	uint32_t pos, i = 0;

	bk_printf("mem_access:0x%x:0x%x\r\n", start, end);
	
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

mpc_hw_t *shared_mem_mpc = 0;

void shared_mem_config_mpc(void)
{
	shared_mem_mpc = (mpc_hw_t *)MPC_SMEM0_BASE_ADDR;
	
	mpc_config_ctrl(shared_mem_mpc, 0, 0, 0, 0);
	shared_mem_mpc->blk_lut[0] = 0x02;
	bk_printf("4k--7 shared_mem_mpc->blk_size:0x%x\r\n", shared_mem_mpc->blk_size);
	
	shared_mem_mpc = (mpc_hw_t *)MPC_SMEM1_BASE_ADDR;
	
	mpc_config_ctrl(shared_mem_mpc, 0, 0, 0, 0);
	shared_mem_mpc->blk_lut[0] = 0x02;
	bk_printf("4k--7 shared_mem_mpc->blk_size:0x%x\r\n", shared_mem_mpc->blk_size);
	
	shared_mem_mpc = (mpc_hw_t *)MPC_SMEM2_BASE_ADDR;
	
	mpc_config_ctrl(shared_mem_mpc, 0, 0, 0, 0);
	shared_mem_mpc->blk_lut[0] = 0x02;
	bk_printf("4k--7 shared_mem_mpc->blk_size:0x%x\r\n", shared_mem_mpc->blk_size);
	
	shared_mem_mpc = (mpc_hw_t *)MPC_SMEM3_BASE_ADDR;
	
	mpc_config_ctrl(shared_mem_mpc, 0, 0, 0, 0);
	shared_mem_mpc->blk_lut[0] = 0x02;
	bk_printf("4k--7 shared_mem_mpc->blk_size:0x%x\r\n", shared_mem_mpc->blk_size);
	
	shared_mem_mpc = (mpc_hw_t *)MPC_SMEM4_BASE_ADDR;
	
	mpc_config_ctrl(shared_mem_mpc, 0, 0, 0, 0);
	shared_mem_mpc->blk_lut[0] = 0x02;
	bk_printf("4k--7 shared_mem_mpc->blk_size:0x%x\r\n", shared_mem_mpc->blk_size);
	
}

void secure_world_access_s_shared_memory(void)
{
	mem_access_test(SECURE_SHAREDM0_START_ADDR, SECURE_SHAREDM0_START_ADDR + VERIFY_DATA_LEN);
	mem_access_test(SECURE_SHAREDM1_START_ADDR, SECURE_SHAREDM1_START_ADDR + VERIFY_DATA_LEN);
	mem_access_test(SECURE_SHAREDM2_START_ADDR, SECURE_SHAREDM2_START_ADDR + VERIFY_DATA_LEN);
	mem_access_test(SECURE_SHAREDM3_START_ADDR, SECURE_SHAREDM3_START_ADDR + VERIFY_DATA_LEN);
	mem_access_test(SECURE_SHAREDM4_START_ADDR, SECURE_SHAREDM4_START_ADDR + VERIFY_DATA_LEN);
}

void secure_world_access_ns_shared_memory(void)
{
	mem_access_test(NON_SECURE_SHAREDM0_START_ADDR, NON_SECURE_SHAREDM0_START_ADDR + VERIFY_DATA_LEN);
}

#define CONFIG_ENABLE_VERIFY_SHARED_MEMORY_AT_NS_WORLD   1

void shared_memory_verification_main(void)
{
	shared_mem_config_mpc();

	#if (0 == CONFIG_ENABLE_VERIFY_SHARED_MEMORY_AT_NS_WORLD)
	secure_world_access_s_shared_memory();
	secure_world_access_ns_shared_memory();
	#endif
}
// eof

