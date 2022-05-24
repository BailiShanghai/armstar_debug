#include "base.h"
#include "aca_sram.h"
#include "bk_uart.h"

uint32_t as_out_content[16] = {0};
uint32_t as_in_content[16] = {0x5a5a5a5a, 0xa5a5a5a5, 0x12345678, 0x87654321,
							  0xeeeeeeee, 0xffffffff, 0x11111111, 0x22222222,
							  0x33333333, 0x44444444, 0x55555555, 0x66666666,
							  0x77777777, 0x88888888, 0x99999999, 0xaaaaaaaa
							  };

void sram_read_data(uint32_t sram_addr,
                           uint32_t block_num,
                           uint8_t *data);
uint32_t sram_alloc_one(uint32_t blk_num);
void sram_write_data(uint32_t sram_addr,
                            uint32_t block_num,
                            const uint8_t *data);

int aca_sram_verification_main(void)
{
	int i;
	uint32_t sram_addr;

	sram_addr = sram_alloc_one(1);
	bk_printf("sram_addr:0x%x\r\n", sram_addr);

	sram_write_data(sram_addr, 1, (uint8_t *)as_in_content);
	sram_read_data(sram_addr, 1, (uint8_t *)as_out_content);

	for(i = 0; i < sizeof(as_in_content) / sizeof(as_in_content[0]); i ++){
		if(as_in_content[i] != as_out_content[i]){
			bk_printf("aca access failed:0x%x 0x%x\r\n", as_in_content[i], as_out_content[i]);
		}	
	}

	return 0;
}

// eof

