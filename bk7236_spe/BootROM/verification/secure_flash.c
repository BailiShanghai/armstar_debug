#include "base.h"
#include "hal_common.h"
#include "bk_flash.h"

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
#define SEC0_START_ADDR                    (0x02000000)
#define SEC0_END_ADDR                      (0x02010000)
#define SEC1_START_ADDR                    (0x12020000)
#define SEC1_END_ADDR                      (0x12030000)

hal_ret_t flash_op_read(hal_addr_t addr, uint8_t *buffer_p, size_t len);
hal_ret_t flash_op_write(hal_addr_t addr, const uint8_t *buffer_p, size_t len);

#define F_BUF_COUNT (64)
uint8_t f_op_buf[F_BUF_COUNT] = {0};

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

uint32_t sflash_access(void)
{
	hal_ret_t ret;

	ret = flash_op_read(SEC0_END_ADDR - 256, f_op_buf, sizeof(f_op_buf));
	sflash_buf_init();
	ret = flash_op_write(SEC0_END_ADDR - 256, f_op_buf, sizeof(f_op_buf));
	sflash_buf_flush();

	ret = flash_op_read(SEC1_END_ADDR - 256, f_op_buf, sizeof(f_op_buf));
	sflash_buf_init();
	ret = flash_op_write(SEC1_END_ADDR - 256, f_op_buf, sizeof(f_op_buf));
	sflash_buf_flush();

	(void)ret;
	
	return 0;
}

uint32_t secure_flash_verification_main(void)
{
	//secure_flash_config();
	sflash_access();
	
    return 0;
}
// eof

