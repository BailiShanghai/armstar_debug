#include "base.h"
#include "bk_uart.h"
#include "vrf_flash.h"
#include "mpc.h"

mpc_hw_t *mpc_flash;
uint32_t vrf_flash_config(void)
{
	mpc_flash = (mpc_hw_t *)MPC_FLASH_BASE_ADDR;
	
	mpc_config_ctrl(mpc_flash, 0, 0, 1, 0);
	mpc_set_secure(mpc_flash, FLASH_SECURE_SIZE, 0, MPC_STATE_SECURE);
	mpc_set_secure(mpc_flash, FLASH_NON_SECURE_SIZE, FLASH_SECURE_SIZE, MPC_STATE_NON_SECURE);
	
	return 0;
}
// eof
