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

	bk_qspi_init();

	/*demo to check psram of secure*/
	bk_qspi_write(qspi_tx_buffer, BUFFER_SIZE);
	bk_qspi_read(qspi_rx_buffer, BUFFER_SIZE);
	if (compare_buffer(qspi_tx_buffer, qspi_rx_buffer, BUFFER_SIZE)) {
		bk_printf("qspi read or write error\r\n");
	}
}

