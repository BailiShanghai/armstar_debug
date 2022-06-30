#include "base.h"
#include "bk_uart.h"
#include "dtcm.h"
#include <math.h>

dtgu_hw_t *dtgu_hw_ptr;
tcm_hw_t *tcm_hw_ptr;

uint32_t dtcm_lut_config(uint32_t total_blk_num, uint32_t previous_blk_num, uint32_t sec_state)
{
	uint32_t sec_mapping_bit = 0;
	uint32_t prv_reg_val = 0;
	uint32_t is_mix_flag = 0;
	uint32_t back_reg_val, mix_reg_val;
	uint32_t i, int_val, mod_val;

	if(STATE_NON_SECURE == sec_state){
		sec_mapping_bit = 1;
		prv_reg_val = 0xFFFFFFFF;
	}
	back_reg_val = ~prv_reg_val;

	int_val = previous_blk_num / 32;
	mod_val = previous_blk_num % 32;
	if(mod_val)
		is_mix_flag = 1;

	for(i = 0; i < ((total_blk_num + 31) / 32); i ++){
		if(i < int_val)
			dtgu_hw_ptr->sec_lookup_tab[i] = prv_reg_val;
		else
			dtgu_hw_ptr->sec_lookup_tab[i] = back_reg_val;
	}

	mix_reg_val = back_reg_val;
	for(i = 0; i < mod_val; i ++){
		mix_reg_val &= (~(1 << i));
		mix_reg_val |= ((sec_mapping_bit & 0x01) << i);
	}
	
	if(is_mix_flag){
		dtgu_hw_ptr->sec_lookup_tab[int_val] = mix_reg_val;
	}
	
	return 0;
}

uint32_t dtcm_config(void)
{
	uint32_t ret;
	uint32_t tcm_size, blk_num, blk_size;

	dtgu_hw_ptr->tgu_ctrl.data_busfault_en = 1;
	dtgu_hw_ptr->tgu_ctrl.ahbt_err_resp = 1;	
	tcm_hw_ptr->dtcm_ctrl.en = 1;
	
	tcm_size = tcm_hw_ptr->dtcm_ctrl.size;
	blk_num = pow(2, dtgu_hw_ptr->tgu_cfg.num_of_blks);
	blk_size = pow(2, dtgu_hw_ptr->tgu_cfg.blk_size + 5);
	bk_printf("dtcm total size:0x%x blk_num:0x%x, blk_size:0x%x\r\n", 
				blk_num * blk_size, 
				blk_num, 
				blk_size);

	/* previous section is secure, and the other is non-secure*/
	ret = dtcm_lut_config(blk_num, blk_num >> 1, STATE_SECURE);
	
	return ret;
}

uint32_t dtcm_init(void)
{
	uint32_t ret;
	
	tcm_hw_ptr = (tcm_hw_t *)TCM_BASE_ADDR;
	dtgu_hw_ptr = (dtgu_hw_t *)DTGU_BASE_ADDR;

	ret = dtcm_config();
	
	return ret;
}

uint32_t dtcm_data_access(uint32_t start, uint32_t end)
{
	uint32_t *head_ptr = (uint32_t *)start;
	uint32_t *tail_ptr = (uint32_t *)(end - 32);
	uint8_t *h, *t;
	uint32_t pos, i = 0;
	
	bk_printf("ns_world dtcm_data_access 0x%x----0x%x\r\n", start, end);

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

uint32_t dtcm_printf_ro_info(void)
{
	bk_printf("dtcm size:0x%x, 32kB: b0110\r\n", tcm_hw_ptr->dtcm_ctrl.size);
	bk_printf("mem_model feature:0x%x\r\n", tcm_hw_ptr->mem_model);
	
	return 0;
}

uint32_t dtcm_proof_main(void)
{
	dtcm_init();
	dtcm_printf_ro_info();

	dtcm_data_access(DTCM_NS_MEM_BASE_ADDR + 16 * 1024, DTCM_S_MEM_BASE_ADDR + 32 * 1024);
	dtcm_data_access(DTCM_S_MEM_BASE_ADDR + 0 * 1024, DTCM_S_MEM_BASE_ADDR + 16 * 1024);
	
	return 0;
}

// eof


