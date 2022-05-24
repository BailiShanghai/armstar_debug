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
#define CONFIG_ENABLE_VERIFY_NON_SECURE_FIELD    1

#if CONFIG_ENABLE_PROOF_SHARED_MEMORY
#include "shared_mem.h"
#endif

#if CONFIG_ENABLE_VERIFY_NON_SECURE_FIELD
#include "non_secure.h"
#endif

void proof_non_secure(void)
{
	#if CONFIG_ENABLE_PROOF_SHARED_MEMORY
	shared_memory_proof_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_NON_SECURE_FIELD
	non_secure_field_verification();
	#endif
}

void proof_main(void)
{
	#if CONFIG_ENABLE_PROOF
	proof_non_secure();
	#endif

	/* Non-secure software does not return, this code is not executed. */
	for( ; ; )
	{
		/* Should not reach here. */
	}
}
// eof

