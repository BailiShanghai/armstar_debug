#include "base.h"
#include "reg.h"
#include "bk_common_types.h"
#include "platform.h"
#include "bk_psram.h"
#include "bk_uart.h"

UINT32 bk_psram_read(char *user_buf, UINT32 count, UINT32 address)
{
	UINT32 i, read_cnt;
	UINT32 value;
	UINT8 *pb = (UINT8 *)&value;

	if(count == 0 || user_buf == NULL) {
		return 0;
	}

	while (count) {
		value = REG_READ(address);
		bk_printf("psram ns read: %x:%x\r\n", address, value);
		read_cnt = count < 4 ? count : 4;

		for (i = 0; i < read_cnt; i++) {
			*user_buf++ = pb[i];
		}

		count -= read_cnt;
		address += read_cnt;
	}

	return 0;
}


UINT32 bk_psram_write(char *user_buf, UINT32 count, UINT32 address)
{
	UINT32 i, write_cnt;
	UINT32 value;
	UINT8 *pb = (UINT8 *)&value;

	if (count == 0 || user_buf == NULL) {
		return 0;
	}

	while (count) {
		if (count < 4) {
			value = REG_READ(address);
			write_cnt = count;
		} else {
			write_cnt = 4;
		}

		for (i = 0; i < write_cnt; i++) {
			pb[i] = *user_buf++;
		}

		bk_printf("psram ns write: %x:%x\r\n", address, value);
		REG_WRITE(address, value);
		count -= write_cnt;
		address += write_cnt;
	}

	return 0;
}

