#include <stdint.h>

#define CORTEX_M33_REG_DAUTHCTRL 0xE000EE04
/*
 31:8 Reserved – Reserved
 7:6 SNID RO Secure non-invasive debug
	 00—TrustZone security extension not implemented
	 01—Reserved
	 10—Secure non-invasive debug disabled (TrustZone implemented)
	 11—Secure non-invasive debug allowed (TrustZone implemented)
 5:4 SID RO Secure invasive debug
	 00—TrustZone security extension not implemented
	 01—Reserved
	 10—Secure invasive debug disabled (TrustZone implemented)
	 11—Secure invasive debug allowed (TrustZone implemented)
 3:2 NSNID RO Non-secure non-invasive debug
	 0x—Reserved
	 10—Non-invasive debug disabled
	 11—Non-invasive debug allowed
 1:0 NSID RO Non-secure invasive debug
	 0x—Reserved
	 10—Invasive debug disabled
	 11—Invasive debug allowed
 */
#define CORTEX_M33_CPU_DEBUG_ENABLE_VALUE 0x0000000F
#define CORTEX_M33_CPU_DEBUG_DISABLE_VALUE 0x00000005

void hal_cpu_debug_enable(void)
{
    *((volatile uint32_t *)CORTEX_M33_REG_DAUTHCTRL) =
        CORTEX_M33_CPU_DEBUG_ENABLE_VALUE;/*DAUTHCTRL register, jtag debug  enable*/
}

void hal_cpu_debug_disable(void)
{
    *((volatile uint32_t *)CORTEX_M33_REG_DAUTHCTRL) =
        CORTEX_M33_CPU_DEBUG_DISABLE_VALUE;/*DAUTHCTRL register, jtag  disable*/
}
//eof

