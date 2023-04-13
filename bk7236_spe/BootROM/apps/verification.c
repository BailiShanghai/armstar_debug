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

#if (CONFIG_ENABLE_VERIFY_PSRAM || CONFIG_ENABLE_VERIFY_PSRAM_CODE)
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

#if (CONFIG_ENABLE_VERIFY_DCACHE || CONFIG_ENABLE_VERIFY_MULTI_CORE1_DCACHE || CONFIG_ENABLE_VERIFY_MULTI_CORE0_DCACHE)
#include "data_cache.h"
#endif

#if CONFIG_ENABLE_POST_SIMULATION_VERIFICATION
#include "post_sim.h"
#endif

#if CONFIG_ENABLE_PRIVILEGE_PERIPHERAL_ACCESS
#include "privilege_peri.h"
#endif

#if CONFGI_ENABEL_SECURE_FLASH
#include "secure_flash.h"
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
	qspi_read_write_aps6404();
	#endif

	#if CONFIG_ENABLE_VERIFY_FLASH
	flash_data_access_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_DCACHE
	bk_printf("data_cache_verification_main\r\n");
	data_cache_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_MULTI_CORE0_DCACHE
	data_cache_multi_core0_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_MULTI_CORE1_DCACHE
	data_cache_multi_core1_verification_main();
	#endif

	#if CONFIG_ENABLE_VERIFY_PSRAM_CODE
	psram_code_test();
	#endif

	#if CONFIG_ENABLE_POST_SIMULATION_VERIFICATION
	post_sim_verification_main();
	#endif

	#if CONFIG_ENABLE_PRIVILEGE_PERIPHERAL_ACCESS
	privilege_verification_main();
	#endif
	
	#if CONFGI_ENABEL_SECURE_FLASH
	secure_flash_verification_main();
	#endif

	bk_printf("test end at the secure world\r\n");
}

uint32_t config_cpu1(void)
{
	/* 1, configure the interrupt vector*/
	/* 2, reset cpu1*/
	*((volatile uint32_t *)(0x44010014)) = (0x02010000);
	bk_printf("[CPU0]config cpu1 reg_addr:0x%x, val:0x%x\r\n", 0x44010014, *((volatile uint32_t *)(0x44010014)));

	*((volatile uint32_t *)(0x44010014)) = (*((volatile uint32_t *)(0x44010014))) | 0x01;
	bk_printf("[CPU0]config cpu1 reg_addr:0x%x, val:0x%x\r\n", 0x44010014, *((volatile uint32_t *)(0x44010014)));

	return 0;
}

void verification_main(void)
{
#if CONFIG_ENABLE_CO_CPU
	config_cpu1();
#endif
	
	#if CONFIG_ENABLE_VERIFICAION
	verification_secure();
	bk_printf("verification_secure over\r\n");
	#endif
	
	/* Boot the non-secure code. */	
	bk_printf("boot_non_secure\r\n");
	
	/* reg0x06 bit2=1*/
	*((volatile uint32_t *)(0x41040000 + 8 * 4)) = ((*((volatile uint32_t *)(0x41040000 + 8 * 4))) | 0x04);
	*((volatile uint32_t *)(0x41040000 + 5 * 4)) = ((*((volatile uint32_t *)(0x41040000 + 5 * 4))) | 0x06);
	
	boot_non_secure( NON_SECURE_START_ADDRESS );

	/* Non-secure software does not return, this code is not executed. */
	for( ; ; )
	{
		/* Should not reach here. */
	}
}

