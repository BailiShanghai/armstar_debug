#include "base.h"
#include "platform.h"
#include "partition_star.h"
#include "bk_common_types.h"
#include "bk_uart.h"
#include "bk_qspi.h"
#include "qspi_reg.h"
#include "qspi_types.h"
#include "qspi.h"

#define APS6404_CMD_READ             (0x03)
#define APS6404_CMD_FAST_READ        (0x0B)
#define APS6404_CMD_FAST_READ_QUAD   (0xEB)
#define APS6404_CMD_WRITE            (0x02)
#define APS6404_CMD_QUAD_WRITE       (0x38)
#define APS6404_CMD_ENTER_QUAD_MODE  (0x35)
#define APS6404_CMD_EXIT_QUAD_MODE   (0xF5)
#define APS6404_CMD_READ_ID          (0x9F)

#define GD25Q32B_CMD_WRITE           (0x32)
#define GD25Q32B_CMD_READ            (0xEB)

#define BUFFER_SIZE         (32)
#define TEST_VALUE_START    0x71

#define QSPI_BLOCK_SIZE         (256 * 1024)
#define QSPI_START_ADDRESS_NS   (QSPI_0_MEM_BASE_S)
#define TEST_QSPI_SIZE_NS       (QSPI_BLOCK_SIZE)
#define QSPI_START_ADDRESS_S    (QSPI_START_ADDRESS_NS + TEST_QSPI_SIZE_NS)
#define TEST_QSPI_SIZE_S        (QSPI_BLOCK_SIZE)

#define QSPI_IO_ADDRESS         (0x112238)

uint8_t qspi_tx_buffer[BUFFER_SIZE];
uint8_t qspi_rx_buffer[BUFFER_SIZE];

static void fill_buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
	uint32_t tmpIndex = 0;

	/* Put in global buffer different values */
	for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ ) {
		pBuffer[tmpIndex] = tmpIndex + uwOffset;
	}
}

static uint8_t compare_buffer(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
	while (BufferLength--) {
		if (*pBuffer1 != *pBuffer2) {
			return 1;
		}

		pBuffer1++;
		pBuffer2++;
	}

	return 0;
}

void qspi_read_write_aps6404(void)
{
	qspi_config_t qspi_config;

	fill_buffer(qspi_tx_buffer, BUFFER_SIZE, TEST_VALUE_START);

	bk_printf("qspi s world\r\n");

	bk_qspi_mpc_config();

	bk_qspi_init();

	bk_printf("qspi aps6404 id %x\r\n", bk_qspi_read_id(APS6404_CMD_READ_ID));

	/*enter quad mode*/
	bk_qspi_enter_quad_mode(APS6404_CMD_ENTER_QUAD_MODE);

	/*write data to secure address with cpu mode*/
	qspi_config.addr = QSPI_START_ADDRESS_S;
	qspi_config.cmd = APS6404_CMD_QUAD_WRITE;
	qspi_config.cmd_line = QSPI_4_LINE;
	qspi_config.addr_line = QSPI_4_LINE;
	qspi_config.data_line = QSPI_4_LINE;
	qspi_config.line_num = QSPI_CMD_5_LINE;
	qspi_config.dummy_clk = 0;
	qspi_config.dummy_mode = NO_INSERT_DUMMY_CLK;
	bk_qspi_cpu_write(&qspi_config, qspi_tx_buffer, BUFFER_SIZE);

	/*read data from secure address with cpu mode*/
	qspi_config.addr = QSPI_START_ADDRESS_S;
	qspi_config.cmd = APS6404_CMD_FAST_READ_QUAD;
	qspi_config.cmd_line = QSPI_4_LINE;
	qspi_config.addr_line = QSPI_4_LINE;
	qspi_config.data_line = QSPI_4_LINE;
	qspi_config.line_num = QSPI_CMD_5_LINE;
	qspi_config.dummy_clk = 6;
	qspi_config.dummy_mode = CMD4_DUMMY_CMD5;
	bk_qspi_cpu_read(&qspi_config, qspi_rx_buffer, BUFFER_SIZE);

	/*compare data*/
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("cpu quad qspi read or write s error\r\n");
	} else {
		bk_printf("cpu quad qspi read or write s success\r\n");
	}

	/*write data to secure address with io mode*/
	qspi_config.addr = QSPI_IO_ADDRESS;
	qspi_config.cmd = APS6404_CMD_QUAD_WRITE;
	qspi_config.cmd_line = QSPI_4_LINE;
	qspi_config.addr_line = QSPI_4_LINE;
	qspi_config.data_line = QSPI_4_LINE;
	qspi_config.line_num = QSPI_CMD_5_LINE;
	qspi_config.dummy_clk = 0;
	qspi_config.dummy_mode = NO_INSERT_DUMMY_CLK;
	bk_qspi_io_write(&qspi_config, qspi_tx_buffer, BUFFER_SIZE);

	/*read data from secure address with io mode*/
	qspi_config.addr = QSPI_IO_ADDRESS;
	qspi_config.cmd = APS6404_CMD_FAST_READ_QUAD;
	qspi_config.cmd_line = QSPI_4_LINE;
	qspi_config.addr_line = QSPI_4_LINE;
	qspi_config.data_line = QSPI_4_LINE;
	qspi_config.line_num = QSPI_CMD_5_LINE;
	qspi_config.dummy_clk = 6;
	qspi_config.dummy_mode = CMD4_DUMMY_CMD5;
	bk_qspi_io_read(&qspi_config, qspi_rx_buffer, BUFFER_SIZE);

	/*compare data*/
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("io quad qspi read or write s error\r\n");
	} else {
		bk_printf("io quad qspi read or write s success\r\n");
	}

	/*exit quad mode*/
	bk_qspi_exit_quad_mode(APS6404_CMD_EXIT_QUAD_MODE);

	/*write data to secure address with cpu mode*/
	qspi_config.addr = QSPI_START_ADDRESS_S;
	qspi_config.cmd = APS6404_CMD_WRITE;
	qspi_config.cmd_line = QSPI_1_LINE;
	qspi_config.addr_line = QSPI_1_LINE;
	qspi_config.data_line = QSPI_1_LINE;
	qspi_config.line_num = QSPI_CMD_5_LINE;
	qspi_config.dummy_clk = 0;
	qspi_config.dummy_mode = NO_INSERT_DUMMY_CLK;
	bk_qspi_cpu_write(&qspi_config, qspi_tx_buffer, BUFFER_SIZE);

	/*read data from secure address with cpu mode*/
	qspi_config.addr = QSPI_START_ADDRESS_S;
	qspi_config.cmd = APS6404_CMD_FAST_READ_QUAD;
	qspi_config.cmd_line = QSPI_1_LINE;
	qspi_config.addr_line = QSPI_1_LINE;
	qspi_config.data_line = QSPI_1_LINE;
	qspi_config.line_num = QSPI_CMD_5_LINE;
	qspi_config.dummy_clk = 6;
	qspi_config.dummy_mode = CMD4_DUMMY_CMD5;
	bk_qspi_cpu_read(&qspi_config, qspi_rx_buffer, BUFFER_SIZE);

	/*compare data*/
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("cpu single qspi read or write s error\r\n");
	} else {
		bk_printf("cpu single qspi read or write s success\r\n");
	}

	/*write data to secure address with io mode*/
	qspi_config.addr = QSPI_IO_ADDRESS;
	qspi_config.cmd = APS6404_CMD_WRITE;
	qspi_config.cmd_line = QSPI_1_LINE;
	qspi_config.addr_line = QSPI_1_LINE;
	qspi_config.data_line = QSPI_1_LINE;
	qspi_config.line_num = QSPI_CMD_5_LINE;
	qspi_config.dummy_clk = 0;
	qspi_config.dummy_mode = NO_INSERT_DUMMY_CLK;
	bk_qspi_io_write(&qspi_config, qspi_tx_buffer, BUFFER_SIZE);

	/*read data from secure address with io mode*/
	qspi_config.addr = QSPI_IO_ADDRESS;
	qspi_config.cmd = APS6404_CMD_FAST_READ;
	qspi_config.cmd_line = QSPI_1_LINE;
	qspi_config.addr_line = QSPI_1_LINE;
	qspi_config.data_line = QSPI_1_LINE;
	qspi_config.line_num = QSPI_CMD_5_LINE;
	qspi_config.dummy_clk = 8;
	qspi_config.dummy_mode = CMD4_DUMMY_CMD5;
	bk_qspi_io_read(&qspi_config, qspi_rx_buffer, BUFFER_SIZE);

	/*compare data*/
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("io single qspi read or write s error\r\n");
	} else {
		bk_printf("io single qspi read or write s success\r\n");
	}
}

void qspi_read_write_gd25q32b(void)
{
	qspi_config_t qspi_config;

	fill_buffer(qspi_tx_buffer, BUFFER_SIZE, TEST_VALUE_START);

	bk_printf("qspi gd25q32b s world\r\n");

	bk_qspi_mpc_config();

	bk_qspi_init();

	/*write data to secure address with cpu mode*/
	qspi_config.addr = QSPI_START_ADDRESS_S;
	qspi_config.cmd = GD25Q32B_CMD_WRITE;
	qspi_config.cmd_line = QSPI_1_LINE;
	qspi_config.addr_line = QSPI_1_LINE;
	qspi_config.data_line = QSPI_4_LINE;
	qspi_config.line_num = QSPI_CMD_5_LINE;
	qspi_config.dummy_clk = 0;
	qspi_config.dummy_mode = NO_INSERT_DUMMY_CLK;
	bk_qspi_cpu_write(&qspi_config, qspi_tx_buffer, BUFFER_SIZE);

	/*read data from secure address with cpu mode*/
	qspi_config.addr = QSPI_START_ADDRESS_S;
	qspi_config.cmd = GD25Q32B_CMD_READ;
	qspi_config.cmd_line = QSPI_1_LINE;
	qspi_config.addr_line = QSPI_4_LINE;
	qspi_config.data_line = QSPI_4_LINE;
	qspi_config.line_num = QSPI_CMD_6_LINE;
	qspi_config.dummy_clk = 4;
	qspi_config.dummy_mode = CMD5_DUMMY_CMD6;
	bk_qspi_cpu_read(&qspi_config, qspi_rx_buffer, BUFFER_SIZE);

	/*compare data*/
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("cpu single qspi read or write s error\r\n");
	} else {
		bk_printf("cpu single qspi read or write s success\r\n");
	}

	/*write data to secure address with io mode*/
	qspi_config.addr = QSPI_IO_ADDRESS;
	qspi_config.cmd = GD25Q32B_CMD_WRITE;
	qspi_config.cmd_line = QSPI_1_LINE;
	qspi_config.addr_line = QSPI_1_LINE;
	qspi_config.data_line = QSPI_4_LINE;
	qspi_config.line_num = QSPI_CMD_5_LINE;
	qspi_config.dummy_clk = 0;
	qspi_config.dummy_mode = NO_INSERT_DUMMY_CLK;
	bk_qspi_io_write(&qspi_config, qspi_tx_buffer, BUFFER_SIZE);

	/*read data from secure address with io mode*/
	qspi_config.addr = QSPI_IO_ADDRESS;
	qspi_config.cmd = GD25Q32B_CMD_READ;
	qspi_config.cmd_line = QSPI_1_LINE;
	qspi_config.addr_line = QSPI_4_LINE;
	qspi_config.data_line = QSPI_4_LINE;
	qspi_config.line_num = QSPI_CMD_6_LINE;
	qspi_config.dummy_clk = 4;
	qspi_config.dummy_mode = CMD5_DUMMY_CMD6;
	bk_qspi_io_read(&qspi_config, qspi_rx_buffer, BUFFER_SIZE);

	/*compare data*/
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("io single qspi read or write s error\r\n");
	} else {
		bk_printf("io single qspi read or write s success\r\n");
	}
}

