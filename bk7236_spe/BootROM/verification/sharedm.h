#pragma once

#define SECURE_SHAREDM_START_ADDR         (0x08000000)
#define NON_SECURE_SHAREDM_START_ADDR     (0x18000000)
#define VERIFY_DATA_LEN                   (1024)

extern void shared_memory_verification_main(void);
// eof
