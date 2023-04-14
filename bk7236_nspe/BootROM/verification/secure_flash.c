#include "base.h"
#include "hal_common.h"
#include "bk_flash.h"
#include "bk_common_types.h"

/*
#define REG_FLASH_SEC_START_ADDR0           (FLASH_BASE + 13 * 4)
#define REG_FLASH_SEC_END_ADDR0             (FLASH_BASE + 14 * 4)
 
#define REG_FLASH_SEC_START_ADDR1           (FLASH_BASE + 15 * 4)
#define REG_FLASH_SEC_END_ADDR1             (FLASH_BASE + 16 * 4)
 
#define REG_FLASH_SEC_START_ADDR2           (FLASH_BASE + 17 * 4)
#define REG_FLASH_SEC_END_ADDR2             (FLASH_BASE + 18 * 4)
 
#define REG_FLASH_SEC_START_ADDR3           (FLASH_BASE + 19 * 4)
#define REG_FLASH_SEC_END_ADDR3             (FLASH_BASE + 20 * 4)

#define FIELD_SEC_ADDR_POSI                    (0)
#define FIELD_SEC_ADDR_MASK                    (0xFFFFFF)
#define FIELD_SEC_ADDR_EN                      (0x01UL << 24)
#define FIELD_NON_SEC_SET                      (0x01UL << 25)
 */
#define FLASH_DATA_BASE_ADDR               (0x02000000) 
#define SEC0_START_ADDR                    (0x02000000)
#define SEC0_END_ADDR                      (0x02010000)
#define SEC1_START_ADDR                    (0x12020000)
#define SEC1_END_ADDR                      (0x12030000)

#define F_BUF_COUNT (64)
uint8_t f_op_buf[F_BUF_COUNT] = {0};
uint32_t g_debug_flag = 1;

uint32_t secure_flash_config(void)
{
	REG_WRITE(REG_FLASH_SEC_END_ADDR0, SEC0_END_ADDR & FIELD_SEC_ADDR_MASK);
	REG_WRITE(REG_FLASH_SEC_START_ADDR0, FIELD_SEC_ADDR_EN | (SEC0_START_ADDR & FIELD_SEC_ADDR_MASK));
	
	REG_WRITE(REG_FLASH_SEC_END_ADDR1, SEC1_END_ADDR & FIELD_SEC_ADDR_MASK);
	REG_WRITE(REG_FLASH_SEC_START_ADDR1, FIELD_SEC_ADDR_EN | (SEC1_START_ADDR & FIELD_SEC_ADDR_MASK));
	
	return 0;
}

void sflash_buf_init(void)
{
	uint32_t i;

	for(i = 0; i < sizeof(f_op_buf);i ++)
	{
		f_op_buf[i] = i + 1;
	}
}

void sflash_buf_flush(void)
{
	uint32_t i;

	for(i = 0; i < sizeof(f_op_buf);i ++)
	{
		f_op_buf[i] = 0;
	}
}
UINT32 bk_flash_read(char *user_buf, UINT32 count, UINT32 address);
UINT32 bk_flash_write(char *user_buf, UINT32 count, UINT32 address);
void bk_flash_set_protect(PROTECT_TYPE type);

uint32_t sflash_access(void)
{
	UINT32 ret;

	bk_flash_set_protect(FLASH_PROTECT_NONE);
	ret = bk_flash_read((char *)f_op_buf, sizeof(f_op_buf), 0);

	ret = bk_flash_read((char *)f_op_buf, sizeof(f_op_buf), SEC0_END_ADDR - 256 - FLASH_DATA_BASE_ADDR);
	sflash_buf_init();
	ret = bk_flash_write((char *)f_op_buf, sizeof(f_op_buf), SEC0_END_ADDR - 256 - FLASH_DATA_BASE_ADDR);
	sflash_buf_flush();
	ret = bk_flash_read((char *)f_op_buf, sizeof(f_op_buf), SEC0_END_ADDR - 256 - FLASH_DATA_BASE_ADDR);

	ret = bk_flash_read((char *)f_op_buf, sizeof(f_op_buf), SEC1_END_ADDR - 256 - FLASH_DATA_BASE_ADDR);
	sflash_buf_init();
	ret = bk_flash_write((char *)f_op_buf, sizeof(f_op_buf), SEC1_END_ADDR - 256 - FLASH_DATA_BASE_ADDR);
	sflash_buf_flush();
	ret = bk_flash_read((char *)f_op_buf, sizeof(f_op_buf), SEC1_END_ADDR - 256 - FLASH_DATA_BASE_ADDR);

	(void)ret;
	
	return 0;
}

uint32_t secure_flash_proof_main(void)
{
	if(0 == g_debug_flag)
		sflash_access();
	
	while(4){;}
	return 0;
}
// eof

