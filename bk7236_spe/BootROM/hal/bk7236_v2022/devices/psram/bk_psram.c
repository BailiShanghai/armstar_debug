#include "base.h"
#include "reg.h"
#include "bk_common_types.h"
#include "platform.h"
#include "partition_star.h"
#include "mpc.h"
#include "bk_psram.h"
#include "bk_uart.h"

mpc_hw_t *g_psram_mpc_ptr = (mpc_hw_t *)(MPC_PSRAM_BASE_ADDR);

static void psram_config_mpc_lut(UINT32 blk_num, UINT32 blk_offset, UINT32 sec_state)
{
	UINT32 i;
	UINT32 lut_idx_offset, lut_mod_offset;
	UINT32 lut_tail_len;
	UINT32 lut_idx, lut_mod;

	/*get offset idx and mod*/
	lut_idx_offset = (blk_offset / 32);
	lut_mod_offset = (blk_offset % 32);

	/*set lut offset tail*/
	lut_tail_len = (blk_num > (32 - lut_mod_offset)) ? (32 - lut_mod_offset) : blk_num;
	lut_mod = lut_mod_offset + lut_tail_len;
	for (i = lut_mod_offset; i < lut_mod; i++) {
		if (sec_state == PSRAM_STATE_NON_SECURE) {
			g_psram_mpc_ptr->blk_lut[lut_idx_offset] |= (0x1 << i);
		} else {
			g_psram_mpc_ptr->blk_lut[lut_idx_offset] &= ~(0x1 << i);
		}
	}

	/*set left block num of lut*/
	lut_idx_offset++;
	lut_idx = ((blk_num - lut_tail_len) / 32) + lut_idx_offset;
	lut_mod = ((blk_num - lut_tail_len) % 32);
	for (i = lut_idx_offset; i < lut_idx; i++) {
		if (sec_state == PSRAM_STATE_NON_SECURE) {
			g_psram_mpc_ptr->blk_lut[i] = (UINT32)(~0x0);
		} else {
			g_psram_mpc_ptr->blk_lut[i] = (UINT32)(0x0);
		}
	}
	for (i = 0; i < lut_mod; i++) {
		if (sec_state == PSRAM_STATE_NON_SECURE) {
			g_psram_mpc_ptr->blk_lut[lut_idx] |= (0x1 << i);
		} else {
			g_psram_mpc_ptr->blk_lut[lut_idx] &= ~(0x1 << i);
		}
	}
}

static void psram_config_mpc_ctrl(UINT8 sec_resp, UINT8 gating_req, UINT8 auto_increase, UINT8 sec_lock)
{
	/*config mpc to auto increase*/
	g_psram_mpc_ptr->mpc_ctrl.cfg_sec_rsp = sec_resp;
	g_psram_mpc_ptr->mpc_ctrl.gating_req = gating_req;
	g_psram_mpc_ptr->mpc_ctrl.auto_inc = auto_increase;
	g_psram_mpc_ptr->mpc_ctrl.sec_lock = sec_lock;
}

static void psram_set_secure_mpc(UINT32 psram_size, UINT32 psram_offset, UINT32 sec_state)
{
	UINT32 blk_size, blk_idx, blk_num, blk_offset;

	/*get block number & block offset*/
	blk_size = (32 << g_psram_mpc_ptr->blk_size);
	blk_num = (psram_size / blk_size);
	blk_offset = (psram_offset / blk_size);
	blk_idx = blk_offset + blk_num;

	/*check block idx*/
	if (blk_idx > g_psram_mpc_ptr->blk_max) {
		bk_printf("idx error, idx:%d, max:%d\r\n", blk_idx, g_psram_mpc_ptr->blk_max);
		return;
	} else {
		g_psram_mpc_ptr->blk_idx = blk_idx;
	}

	/*config mpc lut*/
	psram_config_mpc_lut(blk_num, blk_offset, sec_state);
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

void bk_psram_init(UINT32 psram_size, UINT32 psram_offset, UINT32 sec_state)
{
	psram_config_mpc_ctrl(0, 0, 0, 0);
	psram_set_secure_mpc(psram_size, psram_offset, sec_state);
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

