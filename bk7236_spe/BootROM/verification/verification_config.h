#pragma once

#define CONFIG_ENABLE_PLATFORM_INIT               0  /* initialize te-200*/

#define CONFIG_ENABLE_VERIFICAION                 1
#define CONFIG_ENABLE_FLASH_MPC                   1

#define CONFIG_ENABLE_TT_IDAU                     0
#define CONFIG_ENABLE_VERIFICATION_HASH256        0
#define CONFIG_ENABLE_VERIFICATION_ACA_SRAM       0
#define CONFIG_ENABLE_VERIFY_TE200_RAM            0
#define CONFIG_ENABLE_VERIFY_SHARED_MEMORY        0
#define CONFIG_ENABLE_VERIFY_SECURE_FIELD         0  // ok
#define CONFIG_ENABLE_VERIFY_SIG_HASH256          0
#define CONFIG_ENABLE_VERIFY_TRNG                 0  // ok, internal/external entropy pool verification is ok
#define CONFIG_ENABLE_VERIFY_ALG_AES              0
#define CONFIG_ENABLE_VERIFY_DTCM                 0
#define CONFIG_ENABLE_VERIFY_ITCM                 0
#define CONFIG_ENABLE_VERIFY_PSRAM                0
#define CONFIG_ENABLE_VERIFY_QSPI                 0
#define CONFIG_ENABLE_VERIFY_FLASH                0
#define CONFIG_INVALIDATE_DCACHE_BEFORE_MPU_ENABLE 0
#define CONFIG_ENABLE_DCACHE_AFTER_MPU_ENABLE     1
#define CONFIG_ENABLE_VERIFY_DCACHE               0
#define CONFIG_ENABLE_VERIFY_PSRAM_CODE           0

#define CONFIG_ENABLE_VERIFY_MULTI_CORE0_DCACHE               0
#define CONFIG_ENABLE_VERIFY_MULTI_CORE1_DCACHE               0

#define CONFIG_ENABLE_POST_SIMULATION_VERIFICATION            0
#define CONFIG_ENABLE_PRIVILEGE_PERIPHERAL_ACCESS             0

#define CONFGI_ENABEL_SECURE_FLASH                1

// eof

