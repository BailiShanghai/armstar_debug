#include "base.h"
#include "platform.h"
#include "partition_star.h"
#include "bk_common_types.h"
#include "bk_uart.h"
#include "bk_qspi.h"
#include "qspi_reg.h"
#include "qspi_types.h"
#include "qspi.h"

#define BUFFER_SIZE         ((uint32_t) 0x1000)
#define TEST_VALUE_START    0x41

#define QSPI_START_ADDRESS_NS   (0)
#define TEST_QSPI_SIZE_NS       (0x100000)
#define QSPI_START_ADDRESS_S    (TEST_QSPI_SIZE_NS)
#define TEST_QSPI_SIZE_S        (0x100000)

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

void qspi_read_write_demo(void)
{
	fill_buffer(qspi_tx_buffer, BUFFER_SIZE, TEST_VALUE_START);

	/*set 0x74000000~0x740FFFFF as non-secure*/
	bk_qspi_mpc_config(TEST_QSPI_SIZE_NS, 0, QSPI_STATE_NON_SECURE);

	/*set 0x64100000~641FFFFF as secure*/
	bk_qspi_mpc_config(TEST_QSPI_SIZE_S, TEST_QSPI_SIZE_NS, QSPI_STATE_SECURE);

	bk_qspi_init();

	/*demo to check psram of secure*/
	bk_qspi_quad_direct_write(QSPI_START_ADDRESS_S, qspi_tx_buffer, BUFFER_SIZE);
	bk_qspi_quad_direct_read(QSPI_START_ADDRESS_S, qspi_rx_buffer, BUFFER_SIZE);
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("qspi direct read or write s error\r\n");
	}

	bk_qspi_quad_indirect_write(QSPI_START_ADDRESS_S, qspi_tx_buffer, BUFFER_SIZE);
	bk_qspi_quad_indirect_read(QSPI_START_ADDRESS_S, qspi_rx_buffer, BUFFER_SIZE);
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("qspi indirect read or write s error\r\n");
	}

	bk_qspi_quad_direct_write(QSPI_START_ADDRESS_NS, qspi_tx_buffer, BUFFER_SIZE);
	bk_qspi_quad_direct_read(QSPI_START_ADDRESS_NS, qspi_rx_buffer, BUFFER_SIZE);
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("qspi direct read or write ns error\r\n");
	}

	bk_qspi_quad_indirect_write(QSPI_START_ADDRESS_NS, qspi_tx_buffer, BUFFER_SIZE);
	bk_qspi_quad_indirect_read(QSPI_START_ADDRESS_NS, qspi_rx_buffer, BUFFER_SIZE);
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("qspi indirect read or write ns error\r\n");
	}
}

