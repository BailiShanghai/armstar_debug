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

#define CONFIG_ENABLE_PROOF    1
#define CONFIG_ENABLE_PROOF_SHARED_MEMORY        0
#define CONFIG_ENABLE_VERIFY_NON_SECURE_FIELD    0
#define CONFIG_ENABLE_PROOF_DTCM                 0
#define CONFIG_ENABLE_PROOF_ITCM                 0
#define CONFIG_ENABLE_PROOF_FLASH                0
#define CONFIG_ENABLE_PROOF_PSRAM                0
#define CONFIG_ENABLE_PROOF_NSC                  0
#define CONFIG_ENABLE_FLASH_DATA_SECURE          0

#if CONFIG_ENABLE_PROOF_SHARED_MEMORY
#include "shared_mem.h"
#endif

#if CONFIG_ENABLE_VERIFY_NON_SECURE_FIELD
#include "non_secure.h"
#endif

#if CONFIG_ENABLE_PROOF_DTCM
#include "dtcm.h"
#endif

#if CONFIG_ENABLE_PROOF_ITCM
#include "itcm.h"
#endif

#if CONFIG_ENABLE_PROOF_FLASH
#include "v_flash.h"
#endif

#if CONFIG_ENABLE_PROOF_PSRAM
#include "psram.h"
#endif

#if CONFIG_ENABLE_PROOF_NSC
#include "nsc_caller.h"
#endif

#if CONFIG_ENABLE_FLASH_DATA_SECURE
#include "secure_flash.h"
#endif

void proof_non_secure(void)
{
	#if CONFIG_ENABLE_PROOF_SHARED_MEMORY
	shared_memory_proof_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_NON_SECURE_FIELD
	non_secure_field_verification();
	#endif

	#if CONFIG_ENABLE_PROOF_DTCM
	dtcm_proof_main();
	#endif

	#if CONFIG_ENABLE_PROOF_ITCM
	itcm_proof_main();
	#endif

	#if CONFIG_ENABLE_PROOF_FLASH
	flash_data_access_proof_main();
	#endif

	#if CONFIG_ENABLE_PROOF_PSRAM
	psram_read_write_demo();
	#endif

	#if CONFIG_ENABLE_PROOF_NSC
	nsc_proof_main();
	#endif
	
	#if CONFIG_ENABLE_FLASH_DATA_SECURE
	secure_flash_proof_main();
	#endif
}

void proof_main(void)
{
	#if CONFIG_ENABLE_PROOF
	bk_printf("proof_non_secure\r\n");
	proof_non_secure();
	#endif

	*((volatile uint32_t *)(0x54010014)) = (0xbbbbbb00);
	bk_printf("test end at the non-secure world\r\n");

	/* Non-secure software does not return, this code is not executed. */
	for( ; ; )
	{
		//bk_printf("Non-secure idle dead while\r\n");
		/* Should not reach here. */
	}
}
// eof

