#pragma once

#define SYS_BASE_ADDR  (0x44010000)
#define REG_SYS_CLK_CTRL                     (SYS_BASE_ADDR + 8 * 4)
#define REG_SYS_CLK_DIV_CORE_POSI            (0)
#define REG_SYS_CLK_DIV_CORE_MASK            (0xF)
#define REG_SYS_CLK_SEL_CORE_POSI            (4)
#define REG_SYS_CLK_SEL_CORE_MASK            (0x3)

#define REG_SYS_CK_CTRL                      (SYS_BASE_ADDR + 9 * 4)
#define REG_SYS_CKDIV_PSRAM                  (0x1 << 4)
#define ERG_SYS_CKSEL_PSRAM                  (0x1 << 5)
#define REG_SYS_CKDIV_QSPI_0_POSI            (6)
#define REG_SYS_CKDIV_QSPI_0_MASK            (0xF)
#define REG_SYS_CKSEL_FLASH_POSI             (24)
#define REG_SYS_CKSEL_FLASH_MASK             (0x3)

#define REG_SYS_CLK_EN                       (SYS_BASE_ADDR + 12 * 4)
#define REG_SYS_PSRAM_CKEN                   (0x1 << 19)
#define REG_SYS_QSPI_0_CKEN                  (0x1 << 20)

#define REG_SYS_DISCKG_CTRL                  (SYS_BASE_ADDR + 0x0E * 4)

#define REG_SYS_CPU_0_INT_EN                 (SYS_BASE_ADDR + 0x20 * 4)
#define REG_SYS_QSPI_0_INT                   (0x1 << 20)

#define REG_SYS_GPIO_CONFIG_2                (SYS_BASE_ADDR + 0x32 * 4)
#define REG_SYS_GPIO_22_POSI                 (24)
#define REG_SYS_GPIO_22_MASK                 (0xF)
#define REG_SYS_GPIO_23_POSI                 (28)
#define REG_SYS_GPIO_23_MASK                 (0xF)

#define REG_SYS_GPIO_CONFIG_3                (SYS_BASE_ADDR + 0x33 * 4)
#define REG_SYS_GPIO_24_POSI                 (0)
#define REG_SYS_GPIO_24_MASK                 (0xF)
#define REG_SYS_GPIO_25_POSI                 (4)
#define REG_SYS_GPIO_25_MASK                 (0xF)
#define REG_SYS_GPIO_26_POSI                 (8)
#define REG_SYS_GPIO_26_MASK                 (0xF)
#define REG_SYS_GPIO_27_POSI                 (12)
#define REG_SYS_GPIO_27_MASK                 (0xF)

// eof
