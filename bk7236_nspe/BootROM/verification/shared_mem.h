#pragma once

#define SECURE_SHAREDM_START_ADDR         (0x08010000)
#define NON_SECURE_SHAREDM_START_ADDR     (0x18010000)
#define VERIFY_DATA_LEN                   (1024)

extern void shared_memory_proof_main(void);
// eof
