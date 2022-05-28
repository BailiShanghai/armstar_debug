#include "base.h"
#include "platform.h"
#include "partition_star.h"
#include "bk_common_types.h"
#include "bk_psram.h"
#include "psram.h"

#define BUFFER_SIZE         ((uint32_t) 0x1000)
#define TEST_VALUE_START    0x41
#define PSRAM_START_ADDRESS   SAU_INIT_START6

uint8_t psram_tx_buffer[BUFFER_SIZE];
uint8_t psram_rx_buffer[BUFFER_SIZE];

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

void psram_read_write_demo(void)
{
	fill_buffer(psram_tx_buffer, BUFFER_SIZE, TEST_VALUE_START);

	bk_psram_init();
	bk_psram_write((char *)psram_tx_buffer, BUFFER_SIZE, PSRAM_START_ADDRESS);
	bk_psram_read((char *)psram_rx_buffer, BUFFER_SIZE, PSRAM_START_ADDRESS);

	if (compare_buffer(psram_tx_buffer, psram_rx_buffer, BUFFER_SIZE)) {
		bk_printf("psram read or write error\r\n");
	}
}

