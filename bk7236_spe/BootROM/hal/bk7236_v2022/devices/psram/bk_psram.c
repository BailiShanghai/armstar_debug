#include "base.h"
#include "reg.h"
#include "bk_common_types.h"
#include "platform.h"
#include "partition_star.h"
#include "mpc.h"
#include "bk_psram.h"
#include "bk_uart.h"
#include "bk_flash.h"
#include "sys.h"

mpc_hw_t *g_psram_mpc_ptr = (mpc_hw_t *)(MPC_PSRAM_BASE_ADDR);

static UINT32 psram_get_version(void)
{
	UINT32 version;

	version = REG_READ(REG_PSRAM_VERSION_ID);

	return version;
}

static void psram_soft_reset(void)
{
	UINT32 value;

	/*reset psram controller*/
	REG_WRITE(REG_PSRAM_SOFT_RESET, (PSRAM_SOFT_RESET_MASK << PSRAM_SOFT_RESET_POSI));

	/*reset psram chip*/
	value = REG_READ(REG_PSRAM_REQUEST);
	value |= PSRAM_REQUEST_SOFT_RESET;
	REG_WRITE(REG_PSRAM_REQUEST, value);
}

static UINT32 psram_get_type(void)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= (PSRAM_TYPE_MASK << PSRAM_TYPE_POSI);
	value >>= PSRAM_TYPE_POSI;

	return value;
}

static void psram_set_type(UINT32 type)
{
	UINT32 value;

	if (type > 5) {
		return;
	}

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= ~(PSRAM_TYPE_MASK << PSRAM_TYPE_POSI);
	value |= (type << PSRAM_TYPE_POSI);
	REG_WRITE(REG_PSRAM_CONFIG, value);
}

static UINT32 psram_get_reg_read_latency(void)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= (PSRAM_REG_READ_LATENCY_MASK << PSRAM_REG_READ_LATENCY_POSI);
	value >>= PSRAM_REG_READ_LATENCY_POSI;

	return value;
}

static void psram_set_reg_read_latency(UINT32 latency)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= ~(PSRAM_REG_READ_LATENCY_MASK << PSRAM_REG_READ_LATENCY_POSI);
	value |= (latency << PSRAM_REG_READ_LATENCY_POSI);
	REG_WRITE(REG_PSRAM_CONFIG, value);
}

static UINT32 psram_get_reg_write_latency(void)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= (PSRAM_REG_WRITE_LATENCY_MASK << PSRAM_REG_WRITE_LATENCY_POSI);
	value >>= PSRAM_REG_WRITE_LATENCY_POSI;

	return value;
}

static void psram_set_reg_write_latency(UINT32 latency)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= ~(PSRAM_REG_WRITE_LATENCY_MASK << PSRAM_REG_WRITE_LATENCY_POSI);
	value |= (latency << PSRAM_REG_WRITE_LATENCY_POSI);
	REG_WRITE(REG_PSRAM_CONFIG, value);
}

static UINT32 psram_get_data_read_latency(void)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= (PSRAM_DATA_READ_LATENCY_MASK << PSRAM_DATA_READ_LATENCY_POSI);
	value >>= PSRAM_DATA_READ_LATENCY_POSI;

	return value;
}

static void psram_set_data_read_latency(UINT32 latency)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= ~(PSRAM_DATA_READ_LATENCY_MASK << PSRAM_DATA_READ_LATENCY_POSI);
	value |= (latency << PSRAM_DATA_READ_LATENCY_POSI);
	REG_WRITE(REG_PSRAM_CONFIG, value);
}

static UINT32 psram_get_data_write_latency(void)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= (PSRAM_DATA_WRITE_LATENCY_MASK << PSRAM_DATA_WRITE_LATENCY_POSI);
	value >>= PSRAM_DATA_WRITE_LATENCY_POSI;

	return value;
}

static void psram_set_data_write_latency(UINT32 latency)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= ~(PSRAM_DATA_WRITE_LATENCY_MASK << PSRAM_DATA_WRITE_LATENCY_POSI);
	value |= (latency << PSRAM_DATA_WRITE_LATENCY_POSI);
	REG_WRITE(REG_PSRAM_CONFIG, value);
}

static UINT32 psram_get_select_addr(void)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= (PSRAM_ADDR_SELECT_MASK << PSRAM_ADDR_SELECT_POSI);
	value >>= PSRAM_ADDR_SELECT_POSI;

	return value;
}

static void psram_set_select_addr(UINT32 addr)
{
	UINT32 value;

	value = REG_READ(REG_PSRAM_CONFIG);
	value &= ~(PSRAM_ADDR_SELECT_MASK << PSRAM_ADDR_SELECT_POSI);
	value |= (addr << PSRAM_ADDR_SELECT_POSI);
	REG_WRITE(REG_PSRAM_CONFIG, value);
}

UINT32 bk_psram_ctrl(UINT32 cmd, void *param)
{
	UINT32 value;

	switch (cmd) {
		case CMD_PSRAM_GET_VERSION:
			(*(UINT32 *)param) = psram_get_version();
			break;

		case CMD_PSRAM_SOFT_RESET:
			psram_soft_reset();
			break;

		case CMD_PSRAM_GET_TYPE:
			(*(UINT32 *)param) = psram_get_type();
			break;

		case CMD_PSRAM_SET_TYPE:
			value = (*(UINT32 *)param);
			psram_set_type(value);
			break;

		case CMD_PSRAM_GET_REG_READ_LATENCY:
			(*(UINT32 *)param) = psram_get_reg_read_latency();
			break;

		case CMD_PSRAM_SET_REG_READ_LATENCY:
			psram_set_reg_read_latency(*(UINT32 *)param);
			break;

		case CMD_PSRAM_GET_REG_WRITE_LATENCY:
			(*(UINT32 *)param) = psram_get_reg_write_latency();
			break;

		case CMD_PSRAM_SET_REG_WRITE_LATENCY:
			psram_set_reg_write_latency(*(UINT32 *)param);
			break;

		case CMD_PSRAM_GET_DATA_READ_LATENCY:
			(*(UINT32 *)param) = psram_get_data_read_latency();
			break;

		case CMD_PSRAM_SET_DATA_READ_LATENCY:
			psram_set_data_read_latency(*(UINT32 *)param);
			break;

		case CMD_PSRAM_GET_DATA_WRITE_LATENCY:
			(*(UINT32 *)param) = psram_get_data_write_latency();
			break;

		case CMD_PSRAM_SET_DATA_WRITE_LATENCY:
			psram_set_data_write_latency(*(UINT32 *)param);
			break;

		case CMD_PSRAM_GET_SELECT_ADDR:
			(*(UINT32 *)param) = psram_get_select_addr();
			break;

		case CMD_PSRAM_SET_SELECT_ADDR:
			psram_set_select_addr(*(UINT32 *)param);
			break;
	}

	return 0;
}

void bk_psram_mpc_config(UINT32 psram_size, UINT32 psram_offset, UINT32 sec_state)
{
	mpc_config_ctrl(g_psram_mpc_ptr, 0, 0, 0, 0);
	mpc_set_secure(g_psram_mpc_ptr, psram_size, psram_offset, sec_state);
}

void bk_psram_mpc_cofig_II(void)
{
	mpc_config_ctrl(g_psram_mpc_ptr, 0, 0, 0, 0);

	g_psram_mpc_ptr->blk_idx = 0;
	g_psram_mpc_ptr->blk_lut[0] = 0x1;
}

static void Delay(volatile uint32_t times)
{
	while(times--);
}

static void bk_psram_wait_finish(uint8_t mode)
{
	uint32_t value;

	value = REG_READ(REG_PSRAM_REQUEST);
	while (value & mode) {
		value = REG_READ(REG_PSRAM_REQUEST);
	}
}

static uint32_t bk_psram_cmd_read(uint32_t address)
{
	uint32_t reg_value, value;

	REG_WRITE(REG_PSRAM_WR_ADDR, address);
	reg_value = REG_READ(REG_PSRAM_REQUEST);
	reg_value |= (PSRAM_REQUEST_REG_READ);
	REG_WRITE(REG_PSRAM_REQUEST, reg_value);
	bk_psram_wait_finish(PSRAM_REQUEST_REG_READ);
	value = REG_READ(REG_PSRAM_READ_DATA);

	return value;
}

static void bk_psram_cmd_write(uint32_t address, uint32_t value)
{
	uint32_t reg_value;

	REG_WRITE(REG_PSRAM_WR_ADDR, address);
	REG_WRITE(REG_PSRAM_WRITE_DATA, value);
	reg_value = REG_READ(REG_PSRAM_REQUEST);
	reg_value |= (PSRAM_REQUEST_REG_WRITE);
	REG_WRITE(REG_PSRAM_REQUEST, reg_value);
	bk_psram_wait_finish(PSRAM_REQUEST_REG_WRITE);
}

void bk_psram_init(void)
{
	uint32_t value;

	/*choose clk 320M*/
	value = REG_READ(REG_SYS_CK_CTRL);
	value &= ~(ERG_SYS_CKSEL_PSRAM);
	REG_WRITE(REG_SYS_CK_CTRL, value);

	/*div 2*/
	value = REG_READ(REG_SYS_CK_CTRL);
	value |= REG_SYS_CKDIV_PSRAM;
	REG_WRITE(REG_SYS_CK_CTRL, value);

	/*psram clk enable*/
	value = REG_READ(REG_SYS_CLK_EN);
	value |= REG_SYS_PSRAM_CKEN;
	REG_WRITE(REG_SYS_CLK_EN, value);

	/*psram controller reset*/
	value = REG_READ(REG_PSRAM_SOFT_RESET);
	value |= (PSRAM_SOFT_RESET_MASK << PSRAM_SOFT_RESET_POSI);
	REG_WRITE(REG_PSRAM_SOFT_RESET, value);

	value = 0xd8054043;
	REG_WRITE(REG_PSRAM_CONFIG, value);

	REG_WRITE(REG_PSRAM_REQUEST, PSRAM_REQUEST_SOFT_RESET);
	Delay(5000);

	bk_psram_cmd_read(0x00000000);
	value = REG_READ(REG_PSRAM_READ_DATA);
	value = (value & ~(0x7 << 10)) | (0x4 << 10);
	bk_psram_cmd_write(0x00000000, value);

	bk_psram_cmd_read(0x00000004);
	value = REG_READ(REG_PSRAM_READ_DATA);
	value = (value & ~(0x7 << 13)) | (0x6 << 13);
	bk_psram_cmd_write(0x00000004, value);

	Delay(100);

	value = REG_READ(REG_SYS_CK_CTRL);
	value &= ~(REG_SYS_CKDIV_PSRAM);
	REG_WRITE(REG_SYS_CK_CTRL, value);
}

UINT32 bk_psram_read(char *user_buf, UINT32 count, UINT32 address)
{
	UINT32 i, read_cnt;
	UINT32 value;
	UINT8 *pb = (UINT8 *)&value;

	if(count == 0 || user_buf == NULL) {
		return 0;
	}

	while (count) {
		value = REG_READ(address);
		bk_printf("psram read: %x:%x\r\n", address, value);
		read_cnt = count < 4 ? count : 4;

		for (i = 0; i < read_cnt; i++) {
			*user_buf++ = pb[i];
		}

		count -= read_cnt;
		address += read_cnt;
	}

	return 0;
}


UINT32 bk_psram_write(char *user_buf, UINT32 count, UINT32 address)
{
	UINT32 i, write_cnt;
	UINT32 value;
	UINT8 *pb = (UINT8 *)&value;

	if (count == 0 || user_buf == NULL) {
		return 0;
	}

	while (count) {
		if (count < 4) {
			value = REG_READ(address);
			write_cnt = count;
		} else {
			write_cnt = 4;
		}

		for (i = 0; i < write_cnt; i++) {
			pb[i] = *user_buf++;
		}

		bk_printf("psram write: %x:%x\r\n", address, value);
		REG_WRITE(address, value);
		count -= write_cnt;
		address += write_cnt;
	}

	return 0;
}

