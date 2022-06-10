#pragma once

#define FLASH_SECURE_SIZE       (0x20000) // please check linker script
#define FLASH_NON_SECURE_SIZE   (0x20000)

#define TEST_S_START_ADDR        (0x02040100)
#define TEST_S_END_ADDR          (0x02040200)
#define TEST_NS_START_ADDR       (0x12040300)
#define TEST_NS_END_ADDR         (0x12040400)

extern uint32_t vrf_flash_config(void);
// eof

