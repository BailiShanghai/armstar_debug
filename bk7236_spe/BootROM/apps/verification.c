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
#include "verification_config.h"

#define DBG_PRE_PROMPT "APP"
#include "dbg.h"

#if CONFIG_ENABLE_TT_IDAU
#include "tt_idau.h"
#endif

#if CONFIG_ENABLE_VERIFICATION_HASH256
#include "hash256.h"
#endif

#if CONFIG_ENABLE_VERIFICATION_ACA_SRAM
#include "aca_sram.h"
#endif

#if CONFIG_ENABLE_VERIFY_TE200_RAM
#include "te_ram.h"
#endif

#if CONFIG_ENABLE_VERIFY_SHARED_MEMORY
#include "sharedm.h"
#endif

#if CONFIG_ENABLE_VERIFY_SECURE_FIELD
#include "secure.h"
#endif

#if CONFIG_ENABLE_VERIFY_SIG_HASH256
#include "sig_hash256.h"
#endif

#if CONFIG_ENABLE_VERIFY_TRNG
#include "trng.h"
#endif

#if CONFIG_ENABLE_VERIFY_ALG_AES
#include "alg_aes.h"
#endif

#if CONFIG_ENABLE_VERIFY_DTCM
#include "dtcm.h"
#endif

#if CONFIG_ENABLE_VERIFY_PSRAM
#include "psram.h"
#endif

#if CONFIG_ENABLE_FLASH_MPC
#include "vrf_flash.h"
#endif

#if CONFIG_ENABLE_VERIFY_ITCM
#include "itcm.h"
#endif

#if CONFIG_ENABLE_VERIFY_QSPI
#include "qspi.h"
#endif

#if CONFIG_ENABLE_VERIFY_FLASH
#include "vrf_flash.h"
#endif

#if CONFIG_ENABLE_VERIFY_DCACHE
#include "data_cache.h"
#endif

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

void verification_secure(void)
{
	#if CONFIG_ENABLE_FLASH_MPC
	vrf_flash_config();
	#endif
	
	#if CONFIG_ENABLE_TT_IDAU
	tt_verificaton_main();
	#endif
	
	#if CONFIG_ENABLE_VERIFICATION_HASH256
	hash256_verificaton_main();
	#endif

	#if CONFIG_ENABLE_VERIFICATION_ACA_SRAM
	aca_sram_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_TE200_RAM
	te200_access_ram_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_SHARED_MEMORY
	shared_memory_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_SECURE_FIELD
	secure_field_verification();
	#endif

	#if CONFIG_ENABLE_VERIFY_SIG_HASH256
	hash256_verificaton_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_TRNG
	trng_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_ALG_AES
	alg_aes_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_DTCM
	dtcm_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_PSRAM
	psram_read_write_demo();
	#endif

	#if CONFIG_ENABLE_VERIFY_ITCM
	itcm_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_QSPI
	qspi_read_write_demo();
	#endif

	#if CONFIG_ENABLE_VERIFY_FLASH
	flash_data_access_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_DCACHE
	data_cache_verification_main();
	#endif
}

void verification_main(void)
{
	#if CONFIG_ENABLE_VERIFICAION
	verification_secure();
	#endif
	
	/* Boot the non-secure code. */
	bk_printf("boot_non_secure\r\n");
	boot_non_secure( NON_SECURE_START_ADDRESS );

	/* Non-secure software does not return, this code is not executed. */
	for( ; ; )
	{
		/* Should not reach here. */
	}
}

