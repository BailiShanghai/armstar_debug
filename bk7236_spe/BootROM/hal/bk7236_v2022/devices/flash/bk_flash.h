#ifndef _BK_FLASH_H_
#define _BK_FLASH_H_

#include "types.h"
#include "bk_uart.h"

#define FLASH_DEBUG
#ifdef FLASH_DEBUG
#define FLASH_PRT      os_null_printf
#define FLASH_WARN     os_null_printf
#define FLASH_FATAL    os_null_printf
#else
#define FLASH_PRT      os_null_printf
#define FLASH_WARN     os_null_printf
#define FLASH_FATAL    os_null_printf
#endif

#define FLASH_DEV_NAME                ("flash")

#define FLASH_FAILURE                (1)
#define FLASH_SUCCESS                (0)

#define FLASH_CMD_MAGIC              (0xe240000)
enum
{
    CMD_FLASH_SET_CLK = FLASH_CMD_MAGIC + 1,
    CMD_FLASH_SET_DCO,
    CMD_FLASH_SET_DPLL,

    CMD_FLASH_WRITE_ENABLE,
    CMD_FLASH_WRITE_DISABLE,

    CMD_FLASH_READ_SR,
    CMD_FLASH_WRITE_SR,

    CMD_FLASH_READ_QE,
    CMD_FLASH_SET_QE,

    CMD_FLASH_SET_QWFR,
    CMD_FLASH_CLR_QWFR,

    CMD_FLASH_SET_WSR,
    CMD_FLASH_GET_ID,
    CMD_FLASH_GET_SIZE,
    CMD_FLASH_READ_MID,
    CMD_FLASH_ERASE_SECTOR,
	CMD_FLASH_SET_HPM,
	CMD_FLASH_GET_PROTECT,
    CMD_FLASH_SET_PROTECT
};

typedef enum
{
    FLASH_PROTECT_NONE,
    FLASH_PROTECT_ALL,
    FLASH_PROTECT_HALF,
    FLASH_UNPROTECT_LAST_BLOCK
} PROTECT_TYPE;

typedef enum
{
    FLASH_XTX_16M_SR_WRITE_DISABLE,
    FLASH_XTX_16M_SR_WRITE_ENABLE
} XTX_FLASH_MODE;

typedef struct
{
    UINT8 byte;
    UINT16 value;
} flash_sr_t;

#define MODE_STD              0
#define MODE_DUAL             1
#define MODE_QUAD             2

#define FLASH_BASE                           (0x44030000)

#define REG_FLASH_DEVICE_ID                  (FLASH_BASE + 0 * 4)
#define REG_FLASH_VERSION_ID                 (FLASH_BASE + 1 * 4)

#define REG_FLASH_CLK_RST_CFG                (FLASH_BASE + 2 * 4)
#define FIELD_SOFT_RESETN_POSI                   (0)
#define FIELD_SOFT_RESETN_MASK                   (0x1)
#define FIELD_CLKG_BPS_POSI                      (1)
#define FIELD_CLKG_BPS_MASK                      (0x1)

#define REG_FLASH_GLBL_STATUS                (FLASH_BASE + 3 * 4)

#define REG_FLASH_OP_CTRL                    (FLASH_BASE + 4 * 4)
#define FIELD_OP_SW                                (0x01UL << 29)

/* FIELD_WP_VALUE
	FIELD_WP_VALUE is the signal of wp pin;
	SRP0 bit is non-volatile read/write bit in the status register(sr register);
	FIELD_WP_VALUE and SRP0 control the write protection. the following is the detail:

	SRP0    WP       sr Register      description
	0       x        sw protected     sr can be written to after WEL=1(Write enable command)
	1       0        hw protected     sr can not be written by hw lock
	1       1        hw unprotected   sr can be written to after WEL=1
 */
#define FIELD_WP_VALUE                             (0x01UL << 30)
#define FIELD_BUSY_SW                              (0x01UL << 31)


#define REG_FLASH_OPERATE_SW                 (FLASH_BASE + 21 * 4)
#define ADDR_SW_REG_POSI                     (0)
#define ADDR_SW_REG_MASK                     (0x00FFFFFF)
#define OP_TYPE_SW_POSI                      (24)
#define OP_TYPE_SW_MASK                      (0x1F)

#define REG_FLASH_DATA_SW_FLASH              (FLASH_BASE + 5 * 4)

#define REG_FLASH_DATA_FLASH_SW              (FLASH_BASE + 6 * 4)

#define REG_FLASH_SR_CMD                     (FLASH_BASE + 7 * 4)
#define FIELD_RDSR_SEL_POSI                      (17)
#define FIELD_RDSR_SEL_MASK                      (0x1)
#define FIELD_WRSR_SEL_POSI                      (16)
#define FIELD_WRSR_SEL_MASK                      (0x1)
#define FIELD_RDSR_REG_POSI                      (8)
#define FIELD_RDSR_REG_MASK                      (0xFF)
#define FIELD_WRSR_REG_POSI                      (0)
#define FIELD_WRSR_REG_MASK                      (0xFF)

#define REG_FLASH_RDID_DATA_FLASH            (FLASH_BASE + 8 * 4)

#define REG_FLASH_SR_DATA_CRC_CNT            (FLASH_BASE + 9 * 4)
#define SR_DATA_FLASH_POSI                   (0)
#define SR_DATA_FLASH_MASK                   (0xFF)
#define CRC_ERROR_COUNT_POSI                 (8)
#define CRC_ERROR_COUNT_MASK                 (0xFF)
#define DATA_FLASH_SW_SEL_POSI               (16)
#define DATA_FLASH_SW_SEL_MASK               (0x07)
#define DATA_SW_FLASH_SEL_POSI               (19)
#define DATA_SW_FLASH_SEL_MASK               (0x07)

#define REG_FLASH_CONF                       (FLASH_BASE + 10 * 4)
#define FLASH_CLK_CONF_POSI                  (0)
#define FLASH_CLK_CONF_MASK                  (0x0F)
#define FIELD_MODE_SEL_POSI                  (4)
#define FIELD_MODE_SEL_MASK                  (0x1F)
#define FWREN_FLASH_CPU                      (0x01UL << 9)
#define WRSR_DATA_POSI                       (10)
#define WRSR_DATA_MASK                       (0x00FFFF)
#define CRC_EN                               (0x01UL << 26)

#define ALLFF_REJ                            (0x01UL << 27)
#define FIELD_ALLFF_REJ_POSI                      (27)
#define FIELD_ALLFF_REJ_MASK                      (0x1)

#define GATE_XAES                            (0x01UL << 28)
#define FIELD_FCLK_GATE_XAES_POSI                 (28)
#define FIELD_FCLK_GATE_XAES_MASK                 (0x1)


#define REG_FLASH_MISC_CONF                  (FLASH_BASE + 11 * 4)
#define DPD_STATUS                              (0x01UL << 31)
#define PREFETCH_VER                            (0x01UL << 25)
#define DPD_FBD                                 (0x01UL << 24)
#define FIELD_TDP_TDPDD_DELAY_POSI                      (12)
#define FIELD_TDP_TDPDD_DELAY_MASK                      (0xFFF)
#define FIELD_TREST1_TRDP_DELAY_POSI                    (12)
#define FIELD_TREST1_TRDP_DELAY_MASK                    (0xFFF)


#define REG_FLASH_PAGE_MEM                  (FLASH_BASE + 12 * 4)
#define FIELD_MEM_DATA_POSI                    (0)
#define FIELD_MEM_DATA_MASK                    (0xFF)
#define FIELD_MEM_DATA_CLR                     (0x01UL << 31)

#define REG_FLASH_SEC_START_ADDR0           (FLASH_BASE + 13 * 4)
#define REG_FLASH_SEC_END_ADDR0             (FLASH_BASE + 14 * 4)
#define REG_FLASH_SEC_START_ADDR1           (FLASH_BASE + 15 * 4)
#define REG_FLASH_SEC_END_ADDR1             (FLASH_BASE + 16 * 4)
#define REG_FLASH_SEC_START_ADDR2           (FLASH_BASE + 17 * 4)
#define REG_FLASH_SEC_END_ADDR2             (FLASH_BASE + 18 * 4)
#define REG_FLASH_SEC_START_ADDR3           (FLASH_BASE + 19 * 4)
#define REG_FLASH_SEC_END_ADDR3             (FLASH_BASE + 20 * 4)
#define FIELD_SEC_ADDR_POSI                    (0)
#define FIELD_SEC_ADDR_MASK                    (0xFFFFFF)
#define FIELD_SEC_ADDR_EN                      (0x01UL << 24)


#define CPU_DATA_WR_POSI                     (9)
#define CPU_DATA_WR_MASK                     (0x01)

#define M_VALUE_POST                         22
#define M_VALUE_MASK                         0xFF

#define LINE_MODE_TWO                        2
#define LINE_MODE_FOUR                       4

typedef enum
{
    FLASH_OPCODE_WREN    = 1,
    FLASH_OPCODE_WRDI    = 2,
    FLASH_OPCODE_RDSR    = 3,
    FLASH_OPCODE_WRSR    = 4,
    FLASH_OPCODE_READ    = 5,
    FLASH_OPCODE_RDSR2   = 6,
    FLASH_OPCODE_WRSR2   = 7,
    FLASH_OPCODE_PP      = 12,
    FLASH_OPCODE_SE      = 13,
    FLASH_OPCODE_BE1     = 14,
    FLASH_OPCODE_BE2     = 15,
    FLASH_OPCODE_CE      = 16,
    FLASH_OPCODE_DP      = 17,
    FLASH_OPCODE_RFDP    = 18,
    FLASH_OPCODE_RDID    = 20,
    FLASH_OPCODE_HPM     = 21,
    FLASH_OPCODE_CRMR    = 22,
    FLASH_OPCODE_CRMR2   = 23,
    FLASH_OPCODE_ENSO    = 24,
    FLASH_OPCODE_RDSCR   = 25,
} FLASH_OPCODE;

typedef struct
{
    uint32_t flash_id;
    uint8_t  sr_size;
    uint32_t  flash_size;
    uint8_t  line_mode;
    uint8_t  cmp_post;
    uint8_t  protect_post;
    uint8_t  protect_mask;
    uint16_t protect_all;
    uint16_t protect_none;
    uint16_t protect_half;
    uint16_t unprotect_last_block;
    uint8_t  qe_bit_post;
    uint8_t  qe_bit;
    uint8_t  m_value;
    uint8_t  mode_sel;
} flash_config_t;

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void bk_flash_init(void);
UINT32 bk_flash_read(char *user_buf, UINT32 count, UINT32 address);
UINT32 bk_flash_write(char *user_buf, UINT32 count, UINT32 address);
void bk_flash_erase_sector(UINT32 address);
void bk_flash_set_protect(PROTECT_TYPE type);
void bk_flash_disable_cpu_data_wr(void);
UINT32 bk_flash_ctrl(UINT32 cmd, void *parm);
#endif //_FLASH_H_
// eof

