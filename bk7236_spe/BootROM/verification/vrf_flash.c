#include "base.h"
#include "bk_uart.h"
#include "vrf_flash.h"
#include "mpc.h"

mpc_hw_t *mpc_flash;
uint32_t vrf_flash_config(void)
{
	mpc_flash = (mpc_hw_t *)MPC_FLASH_BASE_ADDR;
	
	mpc_config_ctrl(mpc_flash, 0, 0, 0, 0);
	mpc_flash->blk_lut[0] = 0x0C;
	
	return 0;
}
// eof
