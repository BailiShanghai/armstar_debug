#include "base.h"
#include "platform.h"
#include "partition_star.h"
#include "bk_common_types.h"
#include "bk_psram.h"
#include "psram.h"
#include "bk_uart.h"

#define BUFFER_SIZE         ((uint32_t) 0x1000)
#define TEST_VALUE_START    0x41

#define PSRAM_START_ADDRESS_NS   (PSRAM_MEM_BASE_NS)
#define TEST_PSRAM_SIZE_NS       (0x100000)
#define PSRAM_START_ADDRESS_S    (PSRAM_MEM_BASE_S + TEST_PSRAM_SIZE_NS)
#define TEST_PSRAM_SIZE_S        (0x100000)

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

	/*set 0x70000000~0x700FFFFF as non-secure*/
	//bk_psram_mpc_config(TEST_PSRAM_SIZE_NS, 0, PSRAM_STATE_NON_SECURE);

	/*set 0x60100000~601FFFFF as secure*/
	//bk_psram_mpc_config(TEST_PSRAM_SIZE_S, TEST_PSRAM_SIZE_NS, PSRAM_STATE_SECURE);

	bk_psram_mpc_cofig_II();

	bk_psram_init();

	/*demo to check psram of secure*/
	bk_psram_write((char *)psram_tx_buffer, BUFFER_SIZE, PSRAM_START_ADDRESS_S);
	bk_psram_read((char *)psram_rx_buffer, BUFFER_SIZE, PSRAM_START_ADDRESS_S);
	if (compare_buffer(psram_tx_buffer, psram_rx_buffer, BUFFER_SIZE)) {
		bk_printf("psram read or write s error\r\n");
	}

	/*demo to check psram of non-secure*/
	bk_psram_write((char *)psram_tx_buffer, BUFFER_SIZE, PSRAM_START_ADDRESS_NS);
	bk_psram_read((char *)psram_rx_buffer, BUFFER_SIZE, PSRAM_START_ADDRESS_NS);
	if (compare_buffer(psram_tx_buffer, psram_rx_buffer, BUFFER_SIZE)) {
		bk_printf("psram read or write ns error\r\n");
	}
}

