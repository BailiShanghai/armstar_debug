#ifndef _BK_QSPI_H_
#define _BK_QSPI_H_

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
	/* REG_0x00 */
	uint32_t device_id;

	/* REG_0x01 */
	uint32_t version_id;

	/* REG_0x02 */
	union {
		struct {
			uint32_t soft_reset: 1; 
			uint32_t bps_clk_gate: 1;
			uint32_t rsv: 30;
		};
		uint32_t v;
	} glb_ctrl;

	/* REG_0x03 */
	uint32_t core_status;

	uint32_t reg_gap_0[5];
	
	/* REG_0x08 */
	union {
		struct {
			uint32_t CMD_5: 8;
			uint32_t CMD_6: 8;
			uint32_t CMD_7: 8;
			uint32_t CMD_8: 8;
		};
		uint32_t v;
	} CMD_A_L;

	/* REG_0x09 */
	union {
		struct {
			uint32_t CMD_1: 8;
			uint32_t CMD_2: 8;
			uint32_t CMD_3: 8;
			uint32_t CMD_4: 8;
		};
		uint32_t v;
	} CMD_A_H;

	/* REG_0x0A */
	union {
		struct {
			uint32_t CMD1_LINE: 2;
			uint32_t CMD2_LINE: 2;
			uint32_t CMD3_LINE: 2;
			uint32_t CMD4_LINE: 2;
			uint32_t CMD5_LINE: 2;
			uint32_t CMD6_LINE: 2;
			uint32_t CMD7_LINE: 2;
			uint32_t CMD8_LINE: 2;
			uint32_t rsv: 16;
		};
		uint32_t v;
	} CMD_A_CFG1;

	/* REG_0x0B */
	union {
		struct {
			uint32_t rsv0: 14;
			uint32_t DATA_LINE: 2;
			uint32_t DUMMY_CLK: 7;
			uint32_t rsv1: 1;
			uint32_t DUMMY_MODE: 3;
			uint32_t rsv2: 3;
			uint32_t CMD_MODE: 2;
		};
		uint32_t v;
	} CMD_A_CFG2;

	/* REG_0x0C */
	union {
		struct {
			uint32_t CMD_5: 8;
			uint32_t CMD_6: 8;
			uint32_t CMD_7: 8;
			uint32_t CMD_8: 8;
		};
		uint32_t v;
	} CMD_B_L;

	/* REG_0x0D */
	union {
		struct {
			uint32_t CMD_1: 8;
			uint32_t CMD_2: 8;
			uint32_t CMD_3: 8;
			uint32_t CMD_4: 8;
		};
		uint32_t v;
	} CMD_B_H;

	/* REG_0x0E */
	union {
		struct {
			uint32_t CMD1_LINE: 2;
			uint32_t CMD2_LINE: 2;
			uint32_t CMD3_LINE: 2;
			uint32_t CMD4_LINE: 2;
			uint32_t CMD5_LINE: 2;
			uint32_t CMD6_LINE: 2;
			uint32_t CMD7_LINE: 2;
			uint32_t CMD8_LINE: 2;
			uint32_t rsv: 16;
		};
		uint32_t v;
	} CMD_B_CFG1;

	/* REG_0x0F */
	union {
		struct {
			uint32_t rsv0: 14;
			uint32_t DATA_LINE: 2;
			uint32_t DUMMY_CLK: 7;
			uint32_t rsv1: 1;
			uint32_t DUMMY_MODE: 3;
			uint32_t rsv2: 3;
			uint32_t CMD_MODE: 2;
		};
		uint32_t v;
	} CMD_B_CFG2;

	
	/* REG_0x10 */
	union {
		struct {
			uint32_t CMD_5: 8;
			uint32_t CMD_6: 8;
			uint32_t CMD_7: 8;
			uint32_t CMD_8: 8;
		};
		uint32_t v;
	} CMD_C_L;

	/* REG_0x11 */
	union {
		struct {
			uint32_t CMD_1: 8;
			uint32_t CMD_2: 8;
			uint32_t CMD_3: 8;
			uint32_t CMD_4: 8;
		};
		uint32_t v;
	} CMD_C_H;

	/* REG_0x12 */
	union {
		struct {
			uint32_t CMD1_LINE: 2;
			uint32_t CMD2_LINE: 2;
			uint32_t CMD3_LINE: 2;
			uint32_t CMD4_LINE: 2;
			uint32_t CMD5_LINE: 2;
			uint32_t CMD6_LINE: 2;
			uint32_t CMD7_LINE: 2;
			uint32_t CMD8_LINE: 2;
			uint32_t rsv: 16;
		};
		uint32_t v;
	} CMD_C_CFG1;

	/* REG_0x13 */
	union {
		struct {
			uint32_t CMD_START: 1;
			uint32_t rsv0: 1;
			uint32_t DATA_LEN: 10;
			uint32_t rsv1: 2;
			uint32_t DATA_LINE: 2;
			uint32_t DUMMY_CLK: 7;
			uint32_t rsv2: 1;
			uint32_t DUMMY_MODE: 3;
			uint32_t rsv3: 5;
		};
		uint32_t v;
	}CMD_C_CFG2;

	/* REG_0x14 */
	union {
		struct {
			uint32_t CMD_5: 8;
			uint32_t CMD_6: 8;
			uint32_t CMD_7: 8;
			uint32_t CMD_8: 8;
		};
		uint32_t v;
	} CMD_D_L;

	/* REG_0x15 */
	union {
		struct {
			uint32_t CMD_1: 8;
			uint32_t CMD_2: 8;
			uint32_t CMD_3: 8;
			uint32_t CMD_4: 8;
		};
		uint32_t v;
	} CMD_D_H;

	/* REG_0x16 */
	union {
		struct {
			uint32_t CMD1_LINE: 2;
			uint32_t CMD2_LINE: 2;
			uint32_t CMD3_LINE: 2;
			uint32_t CMD4_LINE: 2;
			uint32_t CMD5_LINE: 2;
			uint32_t CMD6_LINE: 2;
			uint32_t CMD7_LINE: 2;
			uint32_t CMD8_LINE: 2;
			uint32_t rsv: 16;
		};
		uint32_t v;
	} CMD_D_CFG1;

	/* REG_0x17 */
	union {
		struct {
			uint32_t CMD_START: 1;
			uint32_t rsv0: 1;
			uint32_t DATA_LEN: 10;
			uint32_t rsv1: 2;
			uint32_t DATA_LINE: 2;
			uint32_t DUMMY_CLK: 7;
			uint32_t rsv2: 1;
			uint32_t DUMMY_MODE: 3;
			uint32_t rsv3: 5;
		};
		uint32_t v;
	} CMD_D_CFG2;

	/* REG_0x18 */
	union {
		struct {
			uint32_t spi_en: 1;
			uint32_t cpol: 1;
			uint32_t cpha: 1;
			uint32_t io2_io3_mode: 1;
			uint32_t io2: 1;
			uint32_t io3: 1;
			uint32_t force_spi_cs_low: 1;
			uint32_t nss_h_gen_sck: 1;
			uint32_t clk_rate: 8;
			uint32_t dis_cmd_sck: 1;
			uint32_t rsv0: 1;
			uint32_t first_bit_mode: 1;
			uint32_t seq_wr_len_sel: 2;
			uint32_t mem_wait_start_sel: 1;
			uint32_t io_cpu_mem_sel: 1;
			uint32_t rsv2: 1;
			uint32_t spi_cs_h_wait: 8;
		};
		uint32_t v;
	} spi_config;

	/* REG_0x19 */
	union {
		struct {
			uint32_t rsv0: 1;
			uint32_t sw_rst_fifo: 1;
			uint32_t rsv1: 30;
		};
		uint32_t v;
	} config;

	/* REG_0x1A */
	union {
		struct {
			uint32_t int_spi_rx_done: 1;
			uint32_t int_spi_tx_done: 1;
			uint32_t int_cmd_start_done: 1;
			uint32_t int_spi_done: 1;
			uint32_t int_cmd_start_fail: 1;
			uint32_t int_addr_cnt: 1;
			uint32_t rsv: 26;
		};
		uint32_t v;
	} spi_int_en;

	/* REG_0x1B */
	union {
		struct {
			uint32_t clr_spi_rx_done: 1;
			uint32_t clr_spi_tx_done: 1;
			uint32_t clr_cmd_start_done: 1;
			uint32_t clr_spi_done: 1;
			uint32_t clr_cmd_start_fail: 1;
			uint32_t int_addr_cnt: 1;
			uint32_t rsv: 26;
		};
		uint32_t v;
	} spi_status_clr;

	/* REG_0x1C */
	union {
		struct {
			uint32_t spi_rx_done: 1;
			uint32_t spi_tx_done: 1;
			uint32_t cmd_start_done: 1;
			uint32_t spi_done: 1;
			uint32_t cmd_start_fail: 1;
			uint32_t rsv0: 7;
			uint32_t spi_cs: 1;
			uint32_t spi_rx_busy: 1;
			uint32_t spi_tx_busy: 1;
			uint32_t rsv1: 1;
			uint32_t fifo_empty: 1;
			uint32_t rsv2: 15;
		};
		uint32_t v;
	} spi_status;

	/* REG_0x1D */
	uint32_t led_wr_cont_cmd;

	uint32_t fifo[64];
} qspi_hw_t;

#ifdef __cplusplus
 }
#endif

#endif
