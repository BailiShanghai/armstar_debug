#include "base.h"
#include "tt_idau.h"
#include "arm_cmse.h"
#include "cmsis_armclang.h"

void tt_scan_for_idau_region(void *start, void *end)
{
	void *pos = start;
	cmse_address_info_t info;
	int tt_region = TT_INVALID_VAL;
	
	while((uint32_t)end >= (uint32_t)pos)
	{
		info = cmse_TT(pos);
		
		if(info.flags.idau_region_valid)
		{
			if(tt_region != info.flags.idau_region)
			{
				tt_region = info.flags.idau_region;
				printf("[IDAU]addr:%p, region_id:%d\r\n", pos, tt_region);
			}
		}
		else
		{
			printf("[IDAU]addr:%p, region_content_invalid\r\n", pos);
		}
		
		pos += 1;
	}
}

void tt_scan_for_sau_region(void *start, void *end)
{
	void *pos = start;
	cmse_address_info_t info;
	int tt_region = TT_INVALID_VAL;
	
	while((uint32_t)end >= (uint32_t)pos)
	{
		info = cmse_TT(pos);
		
		if(info.flags.sau_region_valid)
		{
			if(tt_region != info.flags.sau_region)
			{
				tt_region = info.flags.sau_region;
				printf("[SAU]addr:%p, region_id:%d\r\n", pos, tt_region);
			}
		}
		else
		{
			printf("[SAU]addr:%p, region_content_invalid\r\n", pos);
		}
		
		pos += 1;
	}
}

void tt_scan_for_mpu_region(void *start, void *end)
{
	void *pos = start;
	cmse_address_info_t info;
	int tt_region = TT_INVALID_VAL;
	
	while((uint32_t)end >= (uint32_t)pos)
	{
		info = cmse_TT(pos);
		
		if(info.flags.mpu_region_valid)
		{
			if(tt_region != info.flags.mpu_region)
			{
				tt_region = info.flags.mpu_region;
				printf("[MPU]addr:%p, region_id:%d\r\n", pos, tt_region);
			}
		}
		else
		{
			printf("[MPU]addr: %p, region_content_invalid\r\n", pos);
		}
		
		pos += 1;
	}
}

void tt_verificaton_main(void)
{
	/*enter unprivilege level*/
	__set_CONTROL(0x01);
	
	/*scan the address space for obtaining the region number*/
	tt_scan_for_idau_region((void *)ADDR_SPACE_START, (void *)ADDR_SPACE_END);	
	tt_scan_for_sau_region((void *)ADDR_SPACE_START, (void *)ADDR_SPACE_END);	
	tt_scan_for_mpu_region((void *)ADDR_SPACE_START, (void *)ADDR_SPACE_END);
}

// eof