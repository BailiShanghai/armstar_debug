#pragma once

#define FLASH_SECURE_SIZE       (0x20000) // please check linker script
#define FLASH_NON_SECURE_SIZE   (0x20000)

extern uint32_t vrf_flash_config(void);
// eof

