#ifndef _QSPI_TYPES_H_
#define _QSPI_TYPES_H_

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum {
	QSPI_STATE_SECURE = 0,
	QSPI_STATE_NON_SECURE
};

enum {
	QSPI_PORT_0 = 0,
	QSPI_PORT_1,
	QSPI_PORT_MAX
};

enum {
	QSPI_CMD_1 = 1,
	QSPI_CMD_2,
	QSPI_CMD_3,
	QSPI_CMD_4,
	QSPI_CMD_5,
	QSPI_CMD_6,
	QSPI_CMD_7,
	QSPI_CMD_8,
	QSPI_CMD_MAX
};

enum {
	QSPI_CMD_1_LINE = 1,
	QSPI_CMD_2_LINE,
	QSPI_CMD_3_LINE,
	QSPI_CMD_4_LINE,
	QSPI_CMD_5_LINE,
	QSPI_CMD_6_LINE,
	QSPI_CMD_7_LINE,
	QSPI_CMD_8_LINE,
	QSPI_CMD_MAX_LINE
};

enum {
	NO_INSERT_DUMMY_CLK = 0,
	CMD1_DUMMY_CMD2,
	CMD2_DUMMY_CMD3,
	CMD3_DUMMY_CMD4,
	CMD4_DUMMY_CMD5,
	CMD5_DUMMY_CMD6,
	CMD6_DUMMY_CMD7,
	CMD7_DUMMY_CMD8
};

typedef struct {
	uint32_t addr;
	uint8_t cmd;
	uint8_t cmd_line;
	uint8_t addr_line;
	uint8_t data_line;
	uint8_t line_num;
	uint8_t dummy_clk;
	uint8_t dummy_mode;
} qspi_config_t;

#ifdef __cplusplus
extern "C" }
#endif

#endif
