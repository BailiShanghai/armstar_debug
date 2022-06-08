#include "base.h"
#include "reg.h"
#include "bk_common_types.h"
#include "platform.h"
#include "partition_star.h"
#include "mpc.h"
#include "bk_uart.h"
#include "bk_gpio.h"

#include "qspi_reg.h"
#include "qspi_types.h"
#include "qspi_struct.h"
#include "qspi_ll.h"
#include "bk_qspi.h"

mpc_hw_t *g_qspi_mpc_ptr = (mpc_hw_t *)(MPC_QSPI0_BASE_ADDR);

static int bk_qspi_write(uint8_t *buf, uint32_t buf_len)
{
	uint32_t write_len;

	while (buf_len) {
		write_len = buf_len > 256 ? 256 : buf_len;
		qspi_set_C_data_len(g_qspi_hw_cfg, write_len);
		qspi_write_data_to_fifo(g_qspi_hw_cfg, buf, write_len);

		buf_len -= write_len;
		buf += write_len;

		/*start write cmd & wait for cmd done*/
		qspi_cmd_C_start(g_qspi_hw_cfg);
		while(!qspi_is_cmd_start_done(g_qspi_hw_cfg));

		/*wait tx done*/
		while (!qspi_is_tx_done(g_qspi_hw_cfg));
	}

	return 0;
}

static int bk_qspi_read(uint8_t *buf, uint32_t buf_len)
{
	uint32_t read_len;

	while (buf_len) {
		read_len = buf_len > 256 ? 256 : buf_len;
		qspi_set_D_data_len(g_qspi_hw_cfg, read_len);
		qspi_read_data_from_fifo(g_qspi_hw_cfg, buf, read_len);

		buf_len -= read_len;
		buf += read_len;

		/*start write cmd & wait for cmd done*/
		qspi_cmd_D_start(g_qspi_hw_cfg);
		while(!qspi_is_cmd_start_done(g_qspi_hw_cfg));

		/*wait rx done*/
		while (qspi_is_rx_done(g_qspi_hw_cfg));
	}

	return 0;
}

void bk_qspi_mpc_config(uint32_t qspi_size, uint32_t qspi_offset, uint32_t sec_state)
{
	mpc_config_ctrl(g_qspi_mpc_ptr, 0, 0, 0, 0);
	mpc_set_secure(g_qspi_mpc_ptr, qspi_size, qspi_offset, sec_state);
}

void bk_qspi_init(void)
{
	/*config qspi gpio to second function*/
	gpio_qspi_config();

	/*config qspi ll to base address*/
	qspi_ll_init();

	/*enable tx&rx done interrupt*/
	qspi_enable_tx_done_intr(g_qspi_hw_cfg);
	qspi_enable_rx_done_intr(g_qspi_hw_cfg);

	qspi_enable(g_qspi_hw_cfg, 1);

	bk_printf("qspi device ID: %s\r\n", qspi_get_device_id(g_qspi_hw_cfg));
	bk_printf("qspi version ID: %s\r\n", qspi_get_version_id(g_qspi_hw_cfg));
}

int bk_qspi_quad_direct_write(uint32_t addr, uint8_t *buf, uint32_t buf_len)
{
	if (buf == NULL || buf_len == 0) {
		return -1;
	}

	/*cmd A is for CPU/DMA write data to device memory*/
	qspi_set_A_cmd(g_qspi_hw_cfg, QSPI_CMD_1, QSPI_CMD_QUAD_WRITE);

	/*config command line*/
	qspi_set_A_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, QSPI_1_LINE);

	/*config address line*/
	qspi_set_A_line(g_qspi_hw_cfg, QSPI_CMD_2_LINE, QSPI_4_LINE);
	qspi_set_A_line(g_qspi_hw_cfg, QSPI_CMD_3_LINE, QSPI_4_LINE);
	qspi_set_A_line(g_qspi_hw_cfg, QSPI_CMD_4_LINE, QSPI_4_LINE);
	qspi_set_A_line(g_qspi_hw_cfg, QSPI_CMD_5_LINE, QSPI_CMD_DISABLE);

	/*config dio and dummy*/
	qspi_set_A_data_line(g_qspi_hw_cfg, QSPI_4_LINE);
	qspi_set_A_dummy_clk(g_qspi_hw_cfg, 0);
	qspi_set_A_dummy_mode(g_qspi_hw_cfg, NO_INSERT_DUMMY_CLK);
	qspi_set_A_cmd_mode(g_qspi_hw_cfg, 0);

	/*wait spi status*/
	while (qspi_is_tx_busy(g_qspi_hw_cfg));
	while (qspi_is_rx_busy(g_qspi_hw_cfg));
	while (!qspi_get_spi_cs(g_qspi_hw_cfg));

	/*write data*/
	memcpy((uint8_t *)addr, buf, buf_len);

	return 0;
}

int bk_qspi_quad_direct_read(uint32_t addr, uint8_t *buf, uint32_t buf_len)
{
	if (buf == NULL || buf_len == 0) {
		return -1;
	}

	/*cmd B is for CPU/DMA read data from device memory*/
	qspi_set_B_cmd(g_qspi_hw_cfg, QSPI_CMD_1, QSPI_CMD_FAST_READ_QUAD);

	/*config command line*/
	qspi_set_B_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, QSPI_1_LINE);

	/*config address line*/
	qspi_set_B_line(g_qspi_hw_cfg, QSPI_CMD_2_LINE, QSPI_4_LINE);
	qspi_set_B_line(g_qspi_hw_cfg, QSPI_CMD_3_LINE, QSPI_4_LINE);
	qspi_set_B_line(g_qspi_hw_cfg, QSPI_CMD_4_LINE, QSPI_4_LINE);
	qspi_set_B_line(g_qspi_hw_cfg, QSPI_CMD_5_LINE, QSPI_CMD_DISABLE);

	/*config dio and dummy*/
	qspi_set_B_data_line(g_qspi_hw_cfg, QSPI_4_LINE);
	qspi_set_B_dummy_clk(g_qspi_hw_cfg, 6);
	qspi_set_B_dummy_mode(g_qspi_hw_cfg, CMD4_DUMMY_CMD5);
	qspi_set_B_cmd_mode(g_qspi_hw_cfg, 0);

	/*wait spi status*/
	while (qspi_is_tx_busy(g_qspi_hw_cfg));
	while (qspi_is_rx_busy(g_qspi_hw_cfg));
	while (!qspi_get_spi_cs(g_qspi_hw_cfg));

	memcpy(buf, (uint8_t *)addr, buf_len);

	return 0;
}

int bk_qspi_quad_indirect_write(uint32_t addr, uint8_t *buf, uint32_t buf_len)
{
	uint8_t cmd_value;

	if (buf == NULL || buf_len == 0) {
		return -1;
	}

	/*cmd C is for CPU write device register or IO mode,
	*write data to TX FIFO, cmd start will write FIFO to device memory*/
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_1, QSPI_CMD_QUAD_WRITE);

	/*set write address*/
	cmd_value = (uint8_t)((addr & 0x00FF0000) >> 16);
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_2, cmd_value);
	cmd_value = (uint8_t)((addr & 0x0000FF00) >> 8);
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_3, cmd_value);
	cmd_value = (uint8_t)((addr & 0x000000FF) >> 0);
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_4, cmd_value);

	/*set cmd line*/
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, QSPI_1_LINE);

	/*set address line*/
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_2_LINE, QSPI_4_LINE);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_3_LINE, QSPI_4_LINE);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_4_LINE, QSPI_4_LINE);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_5_LINE, QSPI_CMD_DISABLE);

	/*set dio and dummy*/
	qspi_set_C_data_line(g_qspi_hw_cfg, QSPI_4_LINE);
	qspi_set_C_dummy_clk(g_qspi_hw_cfg, 0);
	qspi_set_C_dummy_mode(g_qspi_hw_cfg, NO_INSERT_DUMMY_CLK);

	/*write data*/
	bk_qspi_write(buf, buf_len);

	return 0;
}

int bk_qspi_quad_indirect_read(uint32_t addr, uint8_t *buf, uint32_t buf_len)
{
	uint8_t cmd_value;

	if (buf == NULL || buf_len == 0) {
		return -1;
	}

	/*cmd D is for CPU read device register, read device memory data via FIFO*/
	qspi_set_D_cmd(g_qspi_hw_cfg, QSPI_CMD_1, QSPI_CMD_FAST_READ_QUAD);

	/*set read address*/
	cmd_value = (uint8_t)((addr & 0x00FF0000) >> 16);
	qspi_set_D_cmd(g_qspi_hw_cfg, QSPI_CMD_2, cmd_value);
	cmd_value = (uint8_t)((addr & 0x0000FF00) >> 8);
	qspi_set_D_cmd(g_qspi_hw_cfg, QSPI_CMD_4, cmd_value);
	cmd_value = (uint8_t)((addr & 0x000000FF) >> 0);
	qspi_set_D_cmd(g_qspi_hw_cfg, QSPI_CMD_5, cmd_value);

	/*set cmd line*/
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, QSPI_1_LINE);

	/*set address line*/
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_2_LINE, QSPI_4_LINE);
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_3_LINE, QSPI_4_LINE);
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_4_LINE, QSPI_4_LINE);
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_5_LINE, QSPI_CMD_DISABLE);

	/*set dio and dummy*/
	qspi_set_D_data_line(g_qspi_hw_cfg, QSPI_4_LINE);
	qspi_set_D_dummy_clk(g_qspi_hw_cfg, 6);
	qspi_set_D_dummy_mode(g_qspi_hw_cfg, CMD4_DUMMY_CMD5);

	/*read data*/
	bk_qspi_read(buf, buf_len);

	return 0;
}

int bk_qspi_enter_quad_mode(void)
{
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_1, QSPI_CMD_ENTER_QUAD_MODE);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, QSPI_1_LINE);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_2_LINE, QSPI_CMD_DISABLE);
	qspi_cmd_C_start(g_qspi_hw_cfg);

	return 0;
}

int bk_qspi_exit_quad_mode(void)
{
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_1, QSPI_CMD_EXIT_QUAD_MODE);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, QSPI_4_LINE);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_2_LINE, QSPI_CMD_DISABLE);
	qspi_cmd_C_start(g_qspi_hw_cfg);

	return 0;
}

