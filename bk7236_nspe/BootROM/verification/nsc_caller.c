#include "base.h"
#include "bk_uart.h"
#include "nsc_caller.h"

uint32_t nsc_proof_main(void)
{
	int i;

	for(i = 0; i < 20; i ++)
	{
		bk_printf("[%d]:\r\n", i);
		NSCFunction();
	}
	
	return 0;
}

// eof

