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

void bk_qspi_init(void)
{
	/*config qspi gpio to second function*/
	gpio_qspi_config();

	/*config qspi ll to base address*/
	qspi_ll_init();

	/*enable tx&rx done interrupt*/
	qspi_enable_tx_done_intr(g_qspi_hw_cfg);
	qspi_enable_rx_done_intr(g_qspi_hw_cfg);

	bk_printf("qspi device ID: %s\r\n", qspi_get_device_id(g_qspi_hw_cfg));
	bk_printf("qspi version ID: %s\r\n", qspi_get_version_id(g_qspi_hw_cfg));
}

int bk_qspi_write(uint8_t *buf, uint32_t buf_len)
{
	uint32_t write_len;

	if (buf == NULL || buf_len == 0) {
		return -1;
	}

	/*reset fifo*/
	qspi_fifo_soft_reset(g_qspi_hw_cfg);

	while (buf_len) {
		/*wait tx free*/
		while (qspi_is_tx_busy(g_qspi_hw_cfg));

		write_len = buf_len > 256 ? 256 : buf_len;
		qspi_write_data_to_fifo(g_qspi_hw_cfg, buf, write_len);

		buf_len -= write_len;
		buf += write_len;

		/*wait tx done*/
		while (qspi_is_tx_done(g_qspi_hw_cfg));
	}

	return 0;
}

int bk_qspi_read(uint8_t *buf, uint32_t buf_len)
{
	uint32_t read_len;

	if (buf == NULL || buf_len == 0) {
		return -1;
	}

	/*reset fifo*/
	qspi_fifo_soft_reset(g_qspi_hw_cfg);

	while (buf_len) {
		/*wait rx free*/
		while (qspi_is_rx_busy(g_qspi_hw_cfg));

		read_len = buf_len > 256 ? 256 : buf_len;
		qspi_read_data_from_fifo(g_qspi_hw_cfg, buf, read_len);

		buf_len -= read_len;
		buf += read_len;

		/*wait rx done*/
		while (qspi_is_rx_done(g_qspi_hw_cfg));
	}

	return 0;
}

