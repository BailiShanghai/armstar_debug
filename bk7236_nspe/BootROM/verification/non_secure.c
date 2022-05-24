#include "base.h"
#include "bk_uart.h"
#include "cmsis_armclang.h"

void non_secure_field_verification(void)
{
	bk_printf("non_secure world!! control register:0x%x\r\n", __get_CONTROL());
	
	/*enter unprivilege level*/
	__set_CONTROL(0x01);
	
	bk_printf("non_secure world at the unprivilege mode!! control register:0x%x\r\n", __get_CONTROL());
}
// eof

