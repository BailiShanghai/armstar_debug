/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */
#include <arm_cmse.h>
#include "RTE_Components.h"
#include CMSIS_device_header

#if defined (STAR_CU)
#include "STAR_CU.h"
#elif defined (STAR_SP)
#include "STAR_SP.h"
#elif defined (STAR_SE)
#include "STAR_SE.h"
#endif

#include "base.h"
#include "cmsis_armclang.h"

#define DBG_PRE_PROMPT "APP"
#include "dbg.h"

/* typedef for non-secure Reset Handler. */
typedef void ( *NonSecureResetHandler_t )	( void ) __attribute__( ( cmse_nonsecure_call ) );

#define NON_SECURE_START_ADDRESS		( 0x12020000U )

void boot_non_secure( uint32_t ulNonSecureStartAddress )
{
	NonSecureResetHandler_t pxNonSecureResetHandler;

	/* Main Stack Pointer value for the non-secure side is the first entry in
	 * the non-secure vector table. Read the first entry and assign the same to
	 * the non-secure main stack pointer(MSP_NS). */
	__TZ_set_MSP_NS( *( ( uint32_t * )( ulNonSecureStartAddress ) ) );

	/* Non secure Reset Handler is the second entry in the non-secure vector
	 * table. Read the non-secure reset handler.
	 */
	pxNonSecureResetHandler = ( NonSecureResetHandler_t )( * ( ( uint32_t * ) ( ( ulNonSecureStartAddress ) + 4U ) ) );

	/* Start non-secure software application by jumping to the non-secure Reset
	 * Handler. */
	pxNonSecureResetHandler();
}

void verification_main(void)
{
	/* Boot the non-secure code. */
	boot_non_secure( NON_SECURE_START_ADDRESS );

	/* Non-secure software does not return, this code is not executed. */
	for( ; ; )
	{
		/* Should not reach here. */
	}
}
