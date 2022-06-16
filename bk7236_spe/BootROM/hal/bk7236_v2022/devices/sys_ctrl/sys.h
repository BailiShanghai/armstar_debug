#pragma once

#define SYS_BASE_ADDR  (0x44010000)
#define REG_SYS_CLK_CTRL                     (SYS_BASE_ADDR + 8 * 4)
#define REG_SYS_CK_CTRL                      (SYS_BASE_ADDR + 9 * 4)
#define REG_SYS_CKDIV_PSRAM                  (0x1 << 4)
#define ERG_SYS_CKSEL_PSRAM                  (0x1 << 5)
#define REG_SYS_CKSEL_FLASH_POSI             (24)
#define REG_SYS_CKSEL_FLASH_MASK             (0x3)
#define REG_SYS_CLK_EN                       (SYS_BASE_ADDR + 12 * 4)
#define REG_SYS_PSRAM_CKEN                   (0x1 << 19)
#define REG_SYS_DISCKG_CTRL                  (SYS_BASE_ADDR + 0x0E * 4)

// eof
