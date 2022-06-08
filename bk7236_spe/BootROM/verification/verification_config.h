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
#define CONFIG_ENABLE_VERIFY_FLASH                1

// eof

