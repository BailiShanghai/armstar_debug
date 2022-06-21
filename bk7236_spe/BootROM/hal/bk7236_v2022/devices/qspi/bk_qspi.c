#include "base.h"
#include "reg.h"
#include "bk_common_types.h"
#include "platform.h"
#include "partition_star.h"
#include "mpc.h"
#include "bk_uart.h"
#include "bk_gpio.h"
#include "sys.h"

#include "qspi_reg.h"
#include "qspi_types.h"
#include "qspi_struct.h"
#include "qspi_ll.h"
#include "bk_qspi.h"

mpc_hw_t *g_qspi_mpc_ptr = (mpc_hw_t *)(MPC_QSPI0_BASE_ADDR);

static void bk_qspi_system_init(void)
{
	uint32_t value;

	/*enable qspi0 interrupt*/
	value = REG_READ(REG_SYS_CPU_0_INT_EN);
	value |= REG_SYS_QSPI_0_INT;
	REG_WRITE(REG_SYS_CPU_0_INT_EN, value);

	/*sel 480M PLL, div 4*/
	value = REG_READ(REG_SYS_CLK_CTRL);
	value &= ~(REG_SYS_CLK_DIV_CORE_MASK << REG_SYS_CLK_DIV_CORE_POSI);
	value &= ~(REG_SYS_CLK_SEL_CORE_MASK << REG_SYS_CLK_SEL_CORE_POSI);
	value |= (3 << REG_SYS_CLK_DIV_CORE_POSI);
	value |= (3 << REG_SYS_CLK_SEL_CORE_POSI);
	REG_WRITE(REG_SYS_CLK_CTRL, value);

	/*qspi0 clk source/4*/
	value = REG_READ(REG_SYS_CK_CTRL);
	value &= ~(REG_SYS_CKDIV_QSPI_0_MASK << REG_SYS_CKDIV_QSPI_0_POSI);
	value |= (3 << REG_SYS_CKDIV_QSPI_0_POSI);
	REG_WRITE(REG_SYS_CK_CTRL, value);

	/*qspi0 clk enable*/
	value = REG_READ(REG_SYS_CLK_EN);
	value |= REG_SYS_QSPI_0_CKEN;
	REG_WRITE(REG_SYS_CLK_EN, value);
}

static void bk_qspi_qpio_config(void)
{
	uint32_t value;

	/*gpio22&gpio23 to 4-func */
	value = REG_READ(REG_SYS_GPIO_CONFIG_2);
	value &= ~(REG_SYS_GPIO_22_MASK << REG_SYS_GPIO_22_POSI);
	value &= ~(REG_SYS_GPIO_23_MASK << REG_SYS_GPIO_23_POSI);
	value |= (3 << REG_SYS_GPIO_22_POSI);
	value |= (3 << REG_SYS_GPIO_23_POSI);
	REG_WRITE(REG_SYS_GPIO_CONFIG_2, value);

	/*gpoi24,25,26,27 to 4-func*/
	value = REG_READ(REG_SYS_GPIO_CONFIG_3);
	value &= ~(REG_SYS_GPIO_24_MASK << REG_SYS_GPIO_24_POSI);
	value &= ~(REG_SYS_GPIO_25_MASK << REG_SYS_GPIO_25_POSI);
	value &= ~(REG_SYS_GPIO_26_MASK << REG_SYS_GPIO_26_POSI);
	value &= ~(REG_SYS_GPIO_27_MASK << REG_SYS_GPIO_27_POSI);
	value |= (3 << REG_SYS_GPIO_24_POSI);
	value |= (3 << REG_SYS_GPIO_25_POSI);
	value |= (3 << REG_SYS_GPIO_26_POSI);
	value |= (3 << REG_SYS_GPIO_27_POSI);
	REG_WRITE(REG_SYS_GPIO_CONFIG_3, value);

	gpio_qspi_config();
}

static void bk_qspi_wait_cmd_done(void)
{
	while(!qspi_is_cmd_start_done(g_qspi_hw_cfg));
	qspi_clear_cmd_start_done_intr(g_qspi_hw_cfg, 1);
	qspi_clear_cmd_start_done_intr(g_qspi_hw_cfg, 0);
}

static void bk_qspi_direct_write(qspi_config_t *qspi_config)
{
	int i;

	/*cmd A is for CPU/DMA write data to device memory*/
	qspi_set_A_cmd(g_qspi_hw_cfg, QSPI_CMD_1, qspi_config->cmd);

	/*config command line*/
	qspi_set_A_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, qspi_config->cmd_line);

	/*config address line*/
	for (i = QSPI_CMD_2_LINE; i < qspi_config->line_num; i++) {
		qspi_set_A_line(g_qspi_hw_cfg, i, qspi_config->addr_line);
	}
	qspi_set_A_line(g_qspi_hw_cfg, i, QSPI_CMD_DISABLE);

	/*config dio and dummy*/
	qspi_set_A_data_line(g_qspi_hw_cfg, qspi_config->data_line);
	qspi_set_A_dummy_clk(g_qspi_hw_cfg, qspi_config->dummy_clk);
	qspi_set_A_dummy_mode(g_qspi_hw_cfg, qspi_config->dummy_mode);
	qspi_set_A_cmd_mode(g_qspi_hw_cfg, 0);
}

static void bk_qspi_direct_read(qspi_config_t *qspi_config)
{
	int i;

	/*cmd B is for CPU/DMA read data from device memory*/
	qspi_set_B_cmd(g_qspi_hw_cfg, QSPI_CMD_1, qspi_config->cmd);

	/*config command line*/
	qspi_set_B_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, qspi_config->cmd_line);

	/*config address line*/
	for (i = QSPI_CMD_2_LINE; i < qspi_config->line_num; i++) {
		qspi_set_B_line(g_qspi_hw_cfg, i, qspi_config->addr_line);
	}
	qspi_set_B_line(g_qspi_hw_cfg, i, QSPI_CMD_DISABLE);

	/*config dio and dummy*/
	qspi_set_B_data_line(g_qspi_hw_cfg, qspi_config->data_line);
	qspi_set_B_dummy_clk(g_qspi_hw_cfg, qspi_config->dummy_clk);
	qspi_set_B_dummy_mode(g_qspi_hw_cfg, qspi_config->dummy_mode);
	qspi_set_B_cmd_mode(g_qspi_hw_cfg, 0);
}

static void bk_qspi_indirect_write(qspi_config_t *qspi_config, uint16_t len)
{
	int i;
	uint8_t cmd_value;

	/*cmd C is for CPU write device register or IO mode,
	*write data to TX FIFO, cmd start will write FIFO to device memory*/
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_1, qspi_config->cmd);

	/*set write address*/
	cmd_value = (uint8_t)((qspi_config->addr & 0x00FF0000) >> 16);
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_2, cmd_value);
	cmd_value = (uint8_t)((qspi_config->addr & 0x0000FF00) >> 8);
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_3, cmd_value);
	cmd_value = (uint8_t)((qspi_config->addr & 0x000000FF) >> 0);
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_4, cmd_value);

	/*set cmd line*/
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, qspi_config->cmd_line);

	/*set address line*/
	for (i = QSPI_CMD_2_LINE; i < qspi_config->line_num; i++) {
		qspi_set_C_line(g_qspi_hw_cfg, i, qspi_config->addr_line);
	}
	qspi_set_C_line(g_qspi_hw_cfg, i, QSPI_CMD_DISABLE);

	/*set dio and dummy*/
	qspi_set_C_data_line(g_qspi_hw_cfg, qspi_config->data_line);
	qspi_set_C_dummy_clk(g_qspi_hw_cfg, qspi_config->dummy_clk);
	qspi_set_C_dummy_mode(g_qspi_hw_cfg, qspi_config->dummy_mode);

	/*write fifo to address*/
	qspi_set_C_data_len(g_qspi_hw_cfg, len);
	qspi_cmd_C_start(g_qspi_hw_cfg);
	bk_qspi_wait_cmd_done();
}

static void bk_qspi_indirect_read(qspi_config_t *qspi_config, uint16_t len)
{
	int i;
	uint8_t cmd_value;

	/*cmd D is for CPU read device register, read device memory data via FIFO*/
	qspi_set_D_cmd(g_qspi_hw_cfg, QSPI_CMD_1, qspi_config->cmd);

	/*set read address*/
	cmd_value = (uint8_t)((qspi_config->addr & 0x00FF0000) >> 16);
	qspi_set_D_cmd(g_qspi_hw_cfg, QSPI_CMD_2, cmd_value);
	cmd_value = (uint8_t)((qspi_config->addr & 0x0000FF00) >> 8);
	qspi_set_D_cmd(g_qspi_hw_cfg, QSPI_CMD_4, cmd_value);
	cmd_value = (uint8_t)((qspi_config->addr & 0x000000FF) >> 0);
	qspi_set_D_cmd(g_qspi_hw_cfg, QSPI_CMD_5, cmd_value);

	/*set cmd line*/
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, qspi_config->cmd_line);

	/*set address line*/
	for (i = QSPI_CMD_2_LINE; i < qspi_config->line_num; i++) {
		qspi_set_D_line(g_qspi_hw_cfg, i, qspi_config->addr_line);
	}
	qspi_set_D_line(g_qspi_hw_cfg, i, QSPI_CMD_DISABLE);

	/*set dio and dummy*/
	qspi_set_D_data_line(g_qspi_hw_cfg, qspi_config->data_line);
	qspi_set_D_dummy_clk(g_qspi_hw_cfg, qspi_config->dummy_clk);
	qspi_set_D_dummy_mode(g_qspi_hw_cfg, qspi_config->dummy_mode);

	/*read data from fifo*/
	qspi_set_D_data_len(g_qspi_hw_cfg, len);
	qspi_cmd_D_start(g_qspi_hw_cfg);
	bk_qspi_wait_cmd_done();
}

int bk_qspi_enter_quad_mode(uint8_t cmd)
{
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_1, cmd);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, QSPI_1_LINE);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_2_LINE, QSPI_CMD_DISABLE);
	qspi_cmd_C_start(g_qspi_hw_cfg);
	bk_qspi_wait_cmd_done();

	return 0;
}

int bk_qspi_exit_quad_mode(uint8_t cmd)
{
	qspi_set_C_cmd(g_qspi_hw_cfg, QSPI_CMD_1, cmd);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, QSPI_4_LINE);
	qspi_set_C_line(g_qspi_hw_cfg, QSPI_CMD_2_LINE, QSPI_CMD_DISABLE);
	qspi_cmd_C_start(g_qspi_hw_cfg);
	bk_qspi_wait_cmd_done();

	return 0;
}

uint32_t bk_qspi_read_id(uint8_t cmd)
{
	qspi_set_D_cmd(g_qspi_hw_cfg, QSPI_CMD_1, cmd);
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_1_LINE, QSPI_1_LINE);
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_2_LINE, QSPI_1_LINE);
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_3_LINE, QSPI_1_LINE);
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_4_LINE, QSPI_1_LINE);
	qspi_set_D_line(g_qspi_hw_cfg, QSPI_CMD_5_LINE, QSPI_CMD_DISABLE);

	qspi_set_D_data_len(g_qspi_hw_cfg, 6);
	qspi_cmd_D_start(g_qspi_hw_cfg);
	bk_qspi_wait_cmd_done();

	return g_qspi_hw_cfg->fifo[0];
}

int bk_qspi_cpu_write(qspi_config_t *qspi_config, uint8_t *buf, uint32_t buf_len)
{
	int i, write_cnt;
	uint32_t value;
	uint8_t *pb = (uint8_t *)&value;

	if (buf == NULL || buf_len == 0) {
		return 0;
	}

	bk_qspi_direct_write(qspi_config);

	qspi_io_cpu_mem_sel(g_qspi_hw_cfg, QSPI_MEM_FOR_CPU);
	qspi_clear_addr_cnt_intr(g_qspi_hw_cfg, 1);
	qspi_clear_addr_cnt_intr(g_qspi_hw_cfg, 0);

	while (buf_len) {
		if (buf_len < 4) {
			value = REG_READ(qspi_config->addr);
			write_cnt = buf_len;
		} else {
			write_cnt = 4;
		}

		for (i = 0; i < write_cnt; i++) {
			pb[i] = *buf++;
		}

		bk_printf("cpu write: %x:%x\r\n", qspi_config->addr, value);
		REG_WRITE(qspi_config->addr, value);
		buf_len -= write_cnt;
		qspi_config->addr += write_cnt;
	}

	qspi_clear_addr_cnt_intr(g_qspi_hw_cfg, 1);
	qspi_clear_addr_cnt_intr(g_qspi_hw_cfg, 0);
	qspi_io_cpu_mem_sel(g_qspi_hw_cfg, QSPI_MEM_FOR_IO);

	return 0;
}

int bk_qspi_cpu_read(qspi_config_t *qspi_config, uint8_t *buf, uint32_t buf_len)
{
	int i, read_cnt;
	uint32_t value;
	uint8_t *pb = (uint8_t *)&value;

	if (buf == NULL || buf_len == 0) {
		return 0;
	}

	bk_qspi_direct_read(qspi_config);

	qspi_io_cpu_mem_sel(g_qspi_hw_cfg, QSPI_MEM_FOR_CPU);

	while (buf_len) {
		value = REG_READ(qspi_config->addr);
		bk_printf("cpu read: %x:%x\r\n", qspi_config->addr, value);
		read_cnt = buf_len < 4 ? buf_len : 4;

		for (i = 0; i < read_cnt; i++) {
			*buf++ = pb[i];
		}

		buf_len -= read_cnt;
		qspi_config->addr += read_cnt;
	}

	qspi_io_cpu_mem_sel(g_qspi_hw_cfg, QSPI_MEM_FOR_IO);

	return 0;
}

int bk_qspi_io_write(qspi_config_t *qspi_config, uint8_t *buf, uint32_t buf_len)
{
	uint32_t value = 0;
	uint32_t index = 0;

	if (buf == NULL || buf_len == 0) {
		return 0;
	}

	while (buf_len) {
		value = REG_RD32(buf, 0);
		bk_printf("io write %d:%x\r\n", index, value);
		qspi_write_data_to_fifo_II(g_qspi_hw_cfg, value, index);
		buf += 4;
		buf_len -= 4;
		index++;

		if (index == 64) {
			bk_qspi_indirect_write(qspi_config, 256);
			qspi_config->addr += 256;
			index = 0;
		}
	}

	if (index) {
		bk_printf("io wr addr %x:%d\r\n", qspi_config->addr, index);
		bk_qspi_indirect_write(qspi_config, index*4);
		qspi_config->addr += index*4;
		index = 0;
	}

	return 0;
}

int bk_qspi_io_read(qspi_config_t *qspi_config, uint8_t *buf, uint32_t buf_len)
{
	int i, j;
	uint16_t read_cnt;
	uint32_t value;
	uint8_t *pb = (uint8_t *)&value;

	if (buf == NULL || buf_len == 0) {
		return 0;
	}

	while (buf_len) {
		read_cnt = buf_len > 256 ? 256 : buf_len;
		bk_printf("rd addr %x:%d\r\n", qspi_config->addr, read_cnt);
		bk_qspi_indirect_read(qspi_config, read_cnt);
		for (i = 0; i < read_cnt/4; i++) {
			value = qspi_read_data_from_fifo_II(g_qspi_hw_cfg, i);
			bk_printf("io read %d:%x\r\n", i, value);
			for (j = 0; j < 4; j++) {
				*buf++ = pb[j];
			}
		}

		buf_len -= read_cnt;
		qspi_config->addr += read_cnt;
	}

	return 0;
}

void bk_qspi_mpc_config(void)
{
	mpc_config_ctrl(g_qspi_mpc_ptr, 0, 0, 0, 0);
	g_qspi_mpc_ptr->blk_lut[0] = 0x1;
}

void bk_qspi_init(void)
{
	bk_qspi_system_init();

	/*config qspi gpio to second function*/
	bk_qspi_qpio_config();

	/*config qspi ll to base address*/
	qspi_ll_init();

	/*enable qspi*/
	qspi_cs_high_wait(g_qspi_hw_cfg, 1);
	qspi_set_clk_rate(g_qspi_hw_cfg, 0);
	qspi_set_io2_io3_mode(g_qspi_hw_cfg, 1);
	qspi_enable(g_qspi_hw_cfg, 1);
}

