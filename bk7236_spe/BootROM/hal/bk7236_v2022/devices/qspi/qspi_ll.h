#ifndef _QSPI_LL_H_
#define _QSPI_LL_H_

#pragma once

#include "reg_base.h"

#ifdef __cplusplus
extern "C" {
#endif

qspi_hw_t *g_qspi_hw_cfg;

static inline void qspi_ll_init(void)
{
	g_qspi_hw_cfg = (qspi_hw_t *)SOC_QSPI_REG_BASE;
}

static inline uint32_t qspi_get_device_id(qspi_hw_t *hw)
{
	return (hw->device_id);
}

static inline uint32_t qspi_get_version_id(qspi_hw_t *hw)
{
	return (hw->version_id);
}

static inline void qspi_set_bps_clk_gate(qspi_hw_t *hw, bool value)
{
	hw->glb_ctrl.bps_clk_gate = value;
}

static inline bool qspi_get_bps_clk_gate(qspi_hw_t *hw)
{
	return (hw->glb_ctrl.bps_clk_gate);
}

static inline void qspi_soft_reset(qspi_hw_t *hw)
{
	hw->glb_ctrl.soft_reset = 1;
}

static inline uint32_t qspi_get_core_status(qspi_hw_t *hw)
{
	return (hw->core_status);
}

static inline void qspi_set_A_cmd(qspi_hw_t *hw, uint8_t cmd_idx, uint8_t cmd_value)
{
	switch (cmd_idx) {
		case 1:
			hw->CMD_A_H.CMD_1 = cmd_value;
			break;

		case 2:
			hw->CMD_A_H.CMD_2 = cmd_value;
			break;

		case 3:
			hw->CMD_A_H.CMD_3 = cmd_value;
			break;

		case 4:
			hw->CMD_A_H.CMD_4 = cmd_value;
			break;

		case 5:
			hw->CMD_A_L.CMD_5 = cmd_value;
			break;

		case 6:
			hw->CMD_A_L.CMD_6 = cmd_value;
			break;

		case 7:
			hw->CMD_A_L.CMD_7 = cmd_value;
			break;

		case 8:
			hw->CMD_A_L.CMD_8 = cmd_value;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}
}

static inline uint8_t qspi_get_A_cmd(qspi_hw_t *hw, uint8_t cmd_idx)
{
	uint8_t cmd_value = 0;

	switch (cmd_idx) {
		case 1:
			cmd_value = hw->CMD_A_H.CMD_1;
			break;

		case 2:
			cmd_value = hw->CMD_A_H.CMD_2;
			break;

		case 3:
			cmd_value = hw->CMD_A_H.CMD_3;
			break;

		case 4:
			cmd_value = hw->CMD_A_H.CMD_4;
			break;

		case 5:
			cmd_value = hw->CMD_A_L.CMD_5;
			break;

		case 6:
			cmd_value = hw->CMD_A_L.CMD_6;
			break;

		case 7:
			cmd_value = hw->CMD_A_L.CMD_7;
			break;

		case 8:
			cmd_value = hw->CMD_A_L.CMD_8;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}

	return cmd_value;
}

static inline void qspi_set_A_line(qspi_hw_t *hw, uint8_t cmd_idx, uint8_t cmd_line)
{
	switch (cmd_idx) {
		case 1:
			hw->CMD_A_CFG1.CMD1_LINE = cmd_line;
			break;

		case 2:
			hw->CMD_A_CFG1.CMD2_LINE = cmd_line;
			break;

		case 3:
			hw->CMD_A_CFG1.CMD3_LINE = cmd_line;
			break;

		case 4:
			hw->CMD_A_CFG1.CMD4_LINE = cmd_line;
			break;

		case 5:
			hw->CMD_A_CFG1.CMD5_LINE = cmd_line;
			break;

		case 6:
			hw->CMD_A_CFG1.CMD6_LINE = cmd_line;
			break;

		case 7:
			hw->CMD_A_CFG1.CMD7_LINE = cmd_line;
			break;

		case 8:
			hw->CMD_A_CFG1.CMD8_LINE = cmd_line;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}
}

static inline uint8_t qspi_get_A_line(qspi_hw_t *hw, uint8_t cmd_idx)
{
	uint8_t cmd_line = 0;

	switch (cmd_idx) {
		case 1:
			cmd_line = hw->CMD_A_CFG1.CMD1_LINE;
			break;

		case 2:
			cmd_line = hw->CMD_A_CFG1.CMD2_LINE;
			break;

		case 3:
			cmd_line = hw->CMD_A_CFG1.CMD3_LINE;
			break;

		case 4:
			cmd_line = hw->CMD_A_CFG1.CMD4_LINE;
			break;

		case 5:
			cmd_line = hw->CMD_A_CFG1.CMD5_LINE;
			break;

		case 6:
			cmd_line = hw->CMD_A_CFG1.CMD6_LINE;
			break;

		case 7:
			cmd_line = hw->CMD_A_CFG1.CMD7_LINE;
			break;

		case 8:
			cmd_line = hw->CMD_A_CFG1.CMD8_LINE;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}

	return cmd_line;
}

static inline void qspi_set_A_data_line(qspi_hw_t *hw, uint8_t data_line)
{
	hw->CMD_A_CFG2.DATA_LINE = data_line;
}

static inline uint8_t qspi_get_A_data_line(qspi_hw_t *hw)
{
	return (hw->CMD_A_CFG2.DATA_LINE);
}

static inline void qspi_set_A_dummy_clk(qspi_hw_t *hw, uint8_t dummy_clk)
{
	hw->CMD_A_CFG2.DUMMY_CLK = dummy_clk;
}

static inline uint8_t qspi_get_A_dummy_clk(qspi_hw_t *hw)
{
	return (hw->CMD_A_CFG2.DUMMY_CLK);
}

static inline void qspi_set_A_dummy_mode(qspi_hw_t *hw, uint8_t dummy_mode)
{
	hw->CMD_A_CFG2.DUMMY_MODE = dummy_mode;
}

static inline uint8_t qspi_get_A_dummy_mode(qspi_hw_t *hw)
{
	return (hw->CMD_A_CFG2.DUMMY_MODE);
}

static inline void qspi_set_A_cmd_mode(qspi_hw_t *hw, uint8_t cmd_mode)
{
	hw->CMD_A_CFG2.CMD_MODE = cmd_mode;
}

static inline uint8_t qspi_get_A_cmd_mode(qspi_hw_t *hw)
{
	return (hw->CMD_A_CFG2.CMD_MODE);
}

static inline void qspi_set_B_cmd(qspi_hw_t *hw, uint8_t cmd_idx, uint8_t cmd_value)
{
	switch (cmd_idx) {
		case 1:
			hw->CMD_B_H.CMD_1 = cmd_value;
			break;

		case 2:
			hw->CMD_B_H.CMD_2 = cmd_value;
			break;

		case 3:
			hw->CMD_B_H.CMD_3 = cmd_value;
			break;

		case 4:
			hw->CMD_B_H.CMD_4 = cmd_value;
			break;

		case 5:
			hw->CMD_B_L.CMD_5 = cmd_value;
			break;

		case 6:
			hw->CMD_B_L.CMD_6 = cmd_value;
			break;

		case 7:
			hw->CMD_B_L.CMD_7 = cmd_value;
			break;

		case 8:
			hw->CMD_B_L.CMD_8 = cmd_value;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}
}

static inline uint8_t qspi_get_B_cmd(qspi_hw_t *hw, uint8_t cmd_idx)
{
	uint8_t cmd_value = 0;

	switch (cmd_idx) {
		case 1:
			cmd_value = hw->CMD_B_H.CMD_1;
			break;

		case 2:
			cmd_value = hw->CMD_B_H.CMD_2;
			break;

		case 3:
			cmd_value = hw->CMD_B_H.CMD_3;
			break;

		case 4:
			cmd_value = hw->CMD_B_H.CMD_4;
			break;

		case 5:
			cmd_value = hw->CMD_B_L.CMD_5;
			break;

		case 6:
			cmd_value = hw->CMD_B_L.CMD_6;
			break;

		case 7:
			cmd_value = hw->CMD_B_L.CMD_7;
			break;

		case 8:
			cmd_value = hw->CMD_B_L.CMD_8;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}

	return cmd_value;
}

static inline void qspi_set_B_line(qspi_hw_t *hw, uint8_t cmd_idx, uint8_t cmd_line)
{
	switch (cmd_idx) {
		case 1:
			hw->CMD_B_CFG1.CMD1_LINE = cmd_line;
			break;

		case 2:
			hw->CMD_B_CFG1.CMD2_LINE = cmd_line;
			break;

		case 3:
			hw->CMD_B_CFG1.CMD3_LINE = cmd_line;
			break;

		case 4:
			hw->CMD_B_CFG1.CMD4_LINE = cmd_line;
			break;

		case 5:
			hw->CMD_B_CFG1.CMD5_LINE = cmd_line;
			break;

		case 6:
			hw->CMD_B_CFG1.CMD6_LINE = cmd_line;
			break;

		case 7:
			hw->CMD_B_CFG1.CMD7_LINE = cmd_line;
			break;

		case 8:
			hw->CMD_B_CFG1.CMD8_LINE = cmd_line;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}
}

static inline uint8_t qspi_get_B_line(qspi_hw_t *hw, uint8_t cmd_idx)
{
	uint8_t cmd_line;

	switch (cmd_idx) {
		case 1:
			cmd_line = hw->CMD_B_CFG1.CMD1_LINE;
			break;

		case 2:
			cmd_line = hw->CMD_B_CFG1.CMD2_LINE;
			break;

		case 3:
			cmd_line = hw->CMD_B_CFG1.CMD3_LINE;
			break;

		case 4:
			cmd_line = hw->CMD_B_CFG1.CMD4_LINE;
			break;

		case 5:
			cmd_line = hw->CMD_B_CFG1.CMD5_LINE;
			break;

		case 6:
			cmd_line = hw->CMD_B_CFG1.CMD6_LINE;
			break;

		case 7:
			cmd_line = hw->CMD_B_CFG1.CMD7_LINE;
			break;

		case 8:
			cmd_line = hw->CMD_B_CFG1.CMD8_LINE;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}

	return cmd_line;
}

static inline void qspi_set_B_data_line(qspi_hw_t *hw, uint8_t data_line)
{
	hw->CMD_B_CFG2.DATA_LINE = data_line;
}

static inline uint8_t qspi_get_B_data_line(qspi_hw_t *hw)
{
	return (hw->CMD_B_CFG2.DATA_LINE);
}

static inline void qspi_set_B_dummy_clk(qspi_hw_t *hw, uint8_t dummy_clk)
{
	hw->CMD_B_CFG2.DUMMY_CLK = dummy_clk;
}

static inline uint8_t qspi_get_B_dummy_clk(qspi_hw_t *hw)
{
	return (hw->CMD_B_CFG2.DUMMY_CLK);
}

static inline void qspi_set_B_dummy_mode(qspi_hw_t *hw, uint8_t dummy_mode)
{
	hw->CMD_B_CFG2.DUMMY_MODE = dummy_mode;
}

static inline uint8_t qspi_get_B_dummy_mode(qspi_hw_t *hw)
{
	return (hw->CMD_B_CFG2.DUMMY_MODE);
}

static inline void qspi_set_B_cmd_mode(qspi_hw_t *hw, uint8_t cmd_mode)
{
	hw->CMD_B_CFG2.CMD_MODE = cmd_mode;
}

static inline uint8_t qspi_get_B_cmd_mode(qspi_hw_t *hw)
{
	return (hw->CMD_B_CFG2.CMD_MODE);
}

static inline void qspi_set_C_cmd(qspi_hw_t *hw, uint8_t cmd_idx, uint8_t cmd_value)
{
	switch (cmd_idx) {
		case 1:
			hw->CMD_C_H.CMD_1 = cmd_value;
			break;

		case 2:
			hw->CMD_C_H.CMD_2 = cmd_value;
			break;

		case 3:
			hw->CMD_C_H.CMD_3 = cmd_value;
			break;

		case 4:
			hw->CMD_C_H.CMD_4 = cmd_value;
			break;

		case 5:
			hw->CMD_C_L.CMD_5 = cmd_value;
			break;

		case 6:
			hw->CMD_C_L.CMD_6 = cmd_value;
			break;

		case 7:
			hw->CMD_C_L.CMD_7 = cmd_value;
			break;

		case 8:
			hw->CMD_C_L.CMD_8 = cmd_value;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
 	}
}

static inline uint8_t qspi_get_C_cmd(qspi_hw_t *hw, uint8_t cmd_idx)
{
	uint8_t cmd_value;

	switch (cmd_idx) {
		case 1:
			cmd_value = hw->CMD_C_H.CMD_1;
			break;

		case 2:
			cmd_value = hw->CMD_C_H.CMD_2;
			break;

		case 3:
			cmd_value = hw->CMD_C_H.CMD_3;
			break;

		case 4:
			cmd_value = hw->CMD_C_H.CMD_4;
			break;

		case 5:
			cmd_value = hw->CMD_C_L.CMD_5;
			break;

		case 6:
			cmd_value = hw->CMD_C_L.CMD_6;
			break;

		case 7:
			cmd_value = hw->CMD_C_L.CMD_7;
			break;

		case 8:
			cmd_value = hw->CMD_C_L.CMD_8;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}

	return cmd_value;
}

static inline void qspi_set_C_line(qspi_hw_t *hw, uint8_t cmd_idx, uint8_t cmd_line)
{
	switch (cmd_idx) {
		case 1:
			hw->CMD_C_CFG1.CMD1_LINE = cmd_line;
			break;

		case 2:
			hw->CMD_C_CFG1.CMD2_LINE = cmd_line;
			break;

		case 3:
			hw->CMD_C_CFG1.CMD3_LINE = cmd_line;
			break;

		case 4:
			hw->CMD_C_CFG1.CMD4_LINE = cmd_line;
			break;

		case 5:
			hw->CMD_C_CFG1.CMD5_LINE = cmd_line;
			break;

		case 6:
			hw->CMD_C_CFG1.CMD6_LINE = cmd_line;
			break;

		case 7:
			hw->CMD_C_CFG1.CMD7_LINE = cmd_line;
			break;

		case 8:
			hw->CMD_C_CFG1.CMD8_LINE = cmd_line;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}
}

static inline uint8_t qspi_get_C_line(qspi_hw_t *hw, uint8_t cmd_idx)
{
	uint8_t cmd_line;

	switch (cmd_idx) {
		case 1:
			cmd_line = hw->CMD_C_CFG1.CMD1_LINE;
			break;

		case 2:
			cmd_line = hw->CMD_C_CFG1.CMD2_LINE;
			break;

		case 3:
			cmd_line = hw->CMD_C_CFG1.CMD3_LINE;
			break;

		case 4:
			cmd_line = hw->CMD_C_CFG1.CMD4_LINE;
			break;

		case 5:
			cmd_line = hw->CMD_C_CFG1.CMD5_LINE;
			break;

		case 6:
			cmd_line = hw->CMD_C_CFG1.CMD6_LINE;
			break;

		case 7:
			cmd_line = hw->CMD_C_CFG1.CMD7_LINE;
			break;

		case 8:
			cmd_line = hw->CMD_C_CFG1.CMD8_LINE;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}

	return cmd_line;
}

static inline void qspi_cmd_C_start(qspi_hw_t *hw)
{
	hw->CMD_C_CFG2.CMD_START = 1;
}

static inline void qspi_set_C_data_len(qspi_hw_t *hw, uint8_t data_len)
{
	hw->CMD_C_CFG2.DATA_LEN = data_len;
}

static inline uint8_t qspi_get_C_data_len(qspi_hw_t *hw)
{
	return (hw->CMD_C_CFG2.DATA_LEN);
}

static inline void qspi_set_C_data_line(qspi_hw_t *hw, uint8_t data_line)
{
	hw->CMD_C_CFG2.DATA_LINE = data_line;
}

static inline uint8_t qspi_get_C_data_line(qspi_hw_t *hw)
{
	return (hw->CMD_C_CFG2.DATA_LINE);
}

static inline void qspi_set_C_dummy_clk(qspi_hw_t *hw, uint8_t dummy_clk)
{
	hw->CMD_C_CFG2.DUMMY_CLK = dummy_clk;
}

static inline uint8_t qspi_get_C_dummy_clk(qspi_hw_t *hw)
{
	return (hw->CMD_C_CFG2.DUMMY_CLK);
}

static inline void qspi_set_C_dummy_mode(qspi_hw_t *hw, uint8_t dummy_mode)
{
	hw->CMD_C_CFG2.DUMMY_MODE = dummy_mode;
}

static inline uint8_t qspi_get_C_dummy_mode(qspi_hw_t *hw)
{
	return (hw->CMD_C_CFG2.DUMMY_MODE);
}

static inline void qspi_set_D_cmd(qspi_hw_t *hw, uint8_t cmd_idx, uint8_t cmd_value)
{
	switch (cmd_idx) {
		case 1:
			hw->CMD_D_H.CMD_1 = cmd_value;
			break;

		case 2:
			hw->CMD_D_H.CMD_2 = cmd_value;
			break;

		case 3:
			hw->CMD_D_H.CMD_3 = cmd_value;
			break;

		case 4:
			hw->CMD_D_H.CMD_4 = cmd_value;
			break;

		case 5:
			hw->CMD_D_L.CMD_5 = cmd_value;
			break;

		case 6:
			hw->CMD_D_L.CMD_6 = cmd_value;
			break;

		case 7:
			hw->CMD_D_L.CMD_7 = cmd_value;
			break;

		case 8:
			hw->CMD_D_L.CMD_8 = cmd_value;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}
}

static inline uint8_t qspi_get_D_cmd(qspi_hw_t *hw, uint8_t cmd_idx)
{
	uint8_t cmd_value;

	switch (cmd_idx) {
		case 1:
			cmd_value = hw->CMD_D_H.CMD_1;
			break;

		case 2:
			cmd_value = hw->CMD_D_H.CMD_2;
			break;

		case 3:
			cmd_value = hw->CMD_D_H.CMD_3;
			break;

		case 4:
			cmd_value = hw->CMD_D_H.CMD_4;
			break;

		case 5:
			cmd_value = hw->CMD_D_L.CMD_5;
			break;

		case 6:
			cmd_value = hw->CMD_D_L.CMD_6;
			break;

		case 7:
			cmd_value = hw->CMD_D_L.CMD_7;
			break;

		case 8:
			cmd_value = hw->CMD_D_L.CMD_8;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}

	return cmd_value;
}

static inline void qspi_set_D_line(qspi_hw_t *hw, uint8_t cmd_idx, uint8_t cmd_line)
{
	switch (cmd_idx) {
		case 1:
			hw->CMD_D_CFG1.CMD1_LINE = cmd_line;
			break;

		case 2:
			hw->CMD_D_CFG1.CMD2_LINE = cmd_line;
			break;

		case 3:
			hw->CMD_D_CFG1.CMD3_LINE = cmd_line;
			break;

		case 4:
			hw->CMD_D_CFG1.CMD4_LINE = cmd_line;
			break;

		case 5:
			hw->CMD_D_CFG1.CMD5_LINE = cmd_line;
			break;

		case 6:
			hw->CMD_D_CFG1.CMD6_LINE = cmd_line;
			break;

		case 7:
			hw->CMD_D_CFG1.CMD7_LINE = cmd_line;
			break;

		case 8:
			hw->CMD_D_CFG1.CMD8_LINE = cmd_line;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}
}

static inline uint8_t qspi_get_D_line(qspi_hw_t *hw, uint8_t cmd_idx)
{
	uint8_t cmd_line;

	switch (cmd_idx) {
		case 1:
			cmd_line = hw->CMD_D_CFG1.CMD1_LINE;
			break;

		case 2:
			cmd_line = hw->CMD_D_CFG1.CMD2_LINE;
			break;

		case 3:
			cmd_line = hw->CMD_D_CFG1.CMD3_LINE;
			break;

		case 4:
			cmd_line = hw->CMD_D_CFG1.CMD4_LINE;
			break;

		case 5:
			cmd_line = hw->CMD_D_CFG1.CMD5_LINE;
			break;

		case 6:
			cmd_line = hw->CMD_D_CFG1.CMD6_LINE;
			break;

		case 7:
			cmd_line = hw->CMD_D_CFG1.CMD7_LINE;
			break;

		case 8:
			cmd_line = hw->CMD_D_CFG1.CMD8_LINE;
			break;

		default:
			bk_printf("wrong cmd index %d\r\n", cmd_idx);
	}

	return cmd_line;
}

static inline void qspi_set_D_data_line(qspi_hw_t *hw, uint8_t data_line)
{
	hw->CMD_D_CFG2.DATA_LINE = data_line;
}

static inline uint8_t qspi_get_D_data_line(qspi_hw_t *hw)
{
	return (hw->CMD_D_CFG2.DATA_LINE);
}

static inline void qspi_set_D_dummy_clk(qspi_hw_t *hw, uint8_t dummy_clk)
{
	hw->CMD_D_CFG2.DUMMY_CLK = dummy_clk;
}

static inline uint8_t qspi_get_D_dummy_clk(qspi_hw_t *hw)
{
	return (hw->CMD_D_CFG2.DUMMY_CLK);
}

static inline void qspi_set_D_dummy_mode(qspi_hw_t *hw, uint8_t dummy_mode)
{
	hw->CMD_D_CFG2.DUMMY_MODE = dummy_mode;
}

static inline uint8_t qspi_get_D_dummy_mode(qspi_hw_t *hw)
{
	return (hw->CMD_D_CFG2.DUMMY_MODE);
}

static inline void qspi_set_D_cmd_mode(qspi_hw_t *hw, uint8_t cmd_mode)
{
	hw->CMD_D_CFG2.CMD_MODE = cmd_mode;
}

static inline uint8_t qspi_get_D_cmd_mode(qspi_hw_t *hw)
{
	return (hw->CMD_D_CFG2.CMD_MODE);
}

static inline void qspi_enable(qspi_hw_t *hw, bool enable)
{
	hw->spi_config.spi_en = enable;
}

static inline bool qspi_is_enable(qspi_hw_t *hw)
{
	return (hw->spi_config.spi_en);
}

static inline void qspi_set_cpol(qspi_hw_t *hw, bool cpol)
{
	hw->spi_config.cpol = cpol;
}

static inline void qspi_set_cpha(qspi_hw_t *hw, bool cpha)
{
	hw->spi_config.cpha = cpha;
}

static inline void qspi_set_io2_io3_mode(qspi_hw_t *hw, bool mode)
{
	hw->spi_config.io2_io3_mode = mode;
}

static inline void qspi_set_io2(qspi_hw_t *hw, bool io2)
{
	hw->spi_config.io2 = io2;
}

static inline void qspi_set_io3(qspi_hw_t *hw, bool io3)
{
	hw->spi_config.io3 = io3;
}

static inline void qspi_force_spi_cs_low(qspi_hw_t *hw, bool value)
{
	hw->spi_config.force_spi_cs_low = value;
}

static inline void qspi_nss_high_gen_sck(qspi_hw_t *hw, bool enable)
{
	hw->spi_config.nss_h_gen_sck = enable;
}

static inline void qspi_set_clk_rate(qspi_hw_t *hw, uint8_t rate)
{
	hw->spi_config.clk_rate = rate;
}

static inline void qspi_disable_cmd_sck(qspi_hw_t *hw)
{
	hw->spi_config.dis_cmd_sck = 1;
}

static inline void qspi_set_first_bit_mode(qspi_hw_t *hw, bool mode)
{
	hw->spi_config.first_bit_mode = mode;
}

static inline void qspi_io_cpu_mem_sel(qspi_hw_t *hw, bool value)
{
	hw->spi_config.io_cpu_mem_sel = value;
}

static inline void qspi_cs_high_wait(qspi_hw_t *hw, uint8_t value)
{
	hw->spi_config.spi_cs_h_wait = value;
}

static inline void qspi_fifo_soft_reset(qspi_hw_t *hw)
{
	hw->config.sw_rst_fifo = 1;
}

static inline void qspi_enable_rx_done_intr(qspi_hw_t *hw)
{
	hw->spi_int_en.int_spi_rx_done = 1;
}

static inline void qspi_enable_tx_done_intr(qspi_hw_t *hw)
{
	hw->spi_int_en.int_spi_tx_done = 1;
}

static inline void qspi_enable_cmd_start_done_intr(qspi_hw_t *hw)
{
	hw->spi_int_en.int_cmd_start_done = 1;
}

static inline void qspi_enable_spi_done_intr(qspi_hw_t *hw)
{
	hw->spi_int_en.int_spi_done = 1;
}

static inline void qspi_enable_cmd_start_fail_intr(qspi_hw_t *hw)
{
	hw->spi_int_en.int_cmd_start_fail = 1;
}

static inline void qspi_enable_addr_cnt_intr(qspi_hw_t *hw)
{
	hw->spi_int_en.int_addr_cnt = 1;
}

static inline void qspi_clear_rx_done_intr(qspi_hw_t *hw)
{
	hw->spi_status_clr.clr_spi_rx_done = 1;
}

static inline void qspi_clear_tx_done_intr(qspi_hw_t *hw)
{
	hw->spi_status_clr.clr_spi_tx_done = 1;
}

static inline void qspi_clear_cmd_start_done_intr(qspi_hw_t *hw)
{
	hw->spi_status_clr.clr_cmd_start_done = 1;
}

static inline void qspi_clear_spi_done_intr(qspi_hw_t *hw)
{
	hw->spi_status_clr.clr_spi_done = 1;
}

static inline void qspi_clear_cmd_start_fail_intr(qspi_hw_t *hw)
{
	hw->spi_status_clr.clr_cmd_start_fail = 1;
}

static inline void qspi_clear_addr_cnt_intr(qspi_hw_t *hw)
{
	hw->spi_status_clr.int_addr_cnt = 1;
}

static inline bool qspi_is_rx_done(qspi_hw_t *hw)
{
	return (hw->spi_status.spi_rx_done);
}

static inline bool qspi_is_tx_done(qspi_hw_t *hw)
{
	return (hw->spi_status.spi_tx_done);
}

static inline bool qspi_is_cmd_start_done(qspi_hw_t *hw)
{
	return (hw->spi_status.cmd_start_done);
}

static inline bool qspi_is_spi_done(qspi_hw_t *hw)
{
	return (hw->spi_status.spi_done);
}

static inline bool qspi_is_cmd_start_fail(qspi_hw_t *hw)
{
	return (hw->spi_status.cmd_start_fail);
}

static inline bool qspi_get_spi_cs(qspi_hw_t *hw)
{
	return (hw->spi_status.spi_cs);
}

static inline bool qspi_is_rx_busy(qspi_hw_t *hw)
{
	return (hw->spi_status.spi_rx_busy);
}

static inline bool qspi_is_tx_busy(qspi_hw_t *hw)
{
	return (hw->spi_status.spi_tx_busy);
}

static inline bool qspi_is_fifo_empty(qspi_hw_t *hw)
{
	return (hw->spi_status.fifo_empty);
}

static inline void qspi_set_led_wr_cont_cmd(qspi_hw_t *hw, uint32_t value)
{
	hw->led_wr_cont_cmd = value;
}

static inline uint32_t qspi_get_led_wr_cont_cmd(qspi_hw_t *hw)
{
	return (hw->led_wr_cont_cmd);
}

static inline void qspi_write_data_to_fifo(qspi_hw_t *hw, uint8_t *buf, uint32_t buf_len)
{
	memcpy((uint8_t *)hw->fifo, buf, buf_len);
}

static inline void qspi_read_data_from_fifo(qspi_hw_t *hw, uint8_t *buf, uint32_t buf_len)
{
	memcpy(buf, (uint8_t *)hw->fifo, buf_len);
}

#ifdef __cplusplus
extern "C" }
#endif

#endif
