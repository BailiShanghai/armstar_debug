#include "base.h"
#include "reg.h"
#include "bk_common_types.h"
#include "platform.h"
#include "partition_star.h"
#include "mpc.h"
#include "bk_psram.h"

#define TEST_PSRAM_SIZE  (0x100000)

void psram_set_secure_sau_mpc(void)
{
	mpc_hw_t *mpc_hw = (mpc_hw_t *)(MPC_PSRAM_BASE_ADDR);
	UINT32 i, block_size, block_num, lut_int, lut_mod;

	/* set first half of memory as non-secure, second half as secure */
	SAU->RNR  = (6 & SAU_RNR_REGION_Msk);
	SAU->RBAR = (SAU_INIT_START6 & SAU_RBAR_BADDR_Msk);
	SAU->RLAR = (((SAU_INIT_START6 + TEST_PSRAM_SIZE - 1) & SAU_RLAR_LADDR_Msk)
				 | ((0 << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk)
				 | 1U);

	/*config mpc, psram test regin to non-secure*/
	mpc_hw->mpc_ctrl.cfg_sec_rsp = 0;
	mpc_hw->mpc_ctrl.gating_req = 0;
	mpc_hw->mpc_ctrl.auto_inc = 1;
	mpc_hw->mpc_ctrl.sec_lock = 1;

	block_size = (32 << mpc_hw->blk_size);
	block_num = (TEST_PSRAM_SIZE / block_size);
	if (block_num > mpc_hw->blk_max) {
		return;
	} else {
		mpc_hw->blk_idx = block_num;
	}

	lut_int = block_num / 32;
	lut_mod = block_num % 32;
	for (i = 0; i < lut_int; i++) {
		mpc_hw->blk_lut[i] = (UINT32)(~0x0);
	}
	for (i = 0; i < lut_mod; i++) {
		mpc_hw->blk_lut[lut_int] |= (0x1 << i);
	}
}

static UINT32 psram_get_version(void)
{
	UINT32 version;

	version = REG_READ(REG_PSRAM_VERSION_ID);

	return version;
}

static void psram_soft_reset(void)
{
	UINT32 value;

	/*reset psram chip*/
	value = REG_READ(REG_PSRAM_REQUEST);
	value |= PSRAM_REQUEST_SOFT_RESET;
	REG_WRITE(REG_PSRAM_REQUEST, value);

	/*reset psram controller*/
	REG_WRITE(REG_PSRAM_SOFT_RESET, (PSRAM_SOFT_RESET_MASK << PSRAM_SOFT_RESET_POSI));
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

void bk_psram_init(void)
{
	psram_set_secure_sau_mpc();
}

UINT32 bk_psram_read(char *user_buf, UINT32 count, UINT32 address)
{
	UINT32 i, reg_value, read_cnt;
	UINT32 value;
	UINT8 *pb = (UINT8 *)&value;

	if(count == 0 || user_buf == NULL) {
		return 0;
	}

	while (count) {
		REG_WRITE(REG_PSRAM_WR_ADDR, address);
		reg_value = REG_READ(REG_PSRAM_REQUEST);
		reg_value |= (PSRAM_REQUEST_REG_READ);
		REG_WRITE(REG_PSRAM_REQUEST, reg_value);
		value = REG_READ(REG_PSRAM_READ_DATA);

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
	UINT32 i, reg_value, write_cnt;
	UINT32 value;
	UINT8 *pb = (UINT8 *)&value;

	if (count == 0 || user_buf == NULL) {
		return 0;
	}

	while (count) {
		if (count < 4) {
			REG_WRITE(REG_PSRAM_WR_ADDR, address);
			reg_value = REG_READ(REG_PSRAM_REQUEST);
			reg_value |= (PSRAM_REQUEST_REG_READ);
			REG_WRITE(REG_PSRAM_REQUEST, reg_value);
			value = REG_READ(REG_PSRAM_READ_DATA);
			write_cnt = count;
		} else {
			write_cnt = 4;
		}

		for (i = 0; i < write_cnt; i++) {
			pb[i] = *user_buf++;
		}

		REG_WRITE(REG_PSRAM_WR_ADDR, address);
		REG_WRITE(REG_PSRAM_WRITE_DATA, value);
		reg_value = REG_READ(REG_PSRAM_REQUEST);
		reg_value |= (PSRAM_REQUEST_REG_WRITE);
		REG_WRITE(REG_PSRAM_REQUEST, reg_value);

		count -= write_cnt;
		address += write_cnt;
	}

	return 0;
}

