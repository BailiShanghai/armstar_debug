#ifndef _BK_PSRAM_H_
#define _BK_PSRAM_H_

#include "reg_base.h"

#define PSRAM_DEBUG
#ifdef PSRAM_DEBUG
#define PSRAM_PRT      os_null_printf
#define PSRAM_WARN     os_null_printf
#define PSRAM_FATAL    os_null_printf
#else
#define PSRAM_PRT      os_null_printf
#define PSRAM_WARN     os_null_printf
#define PSRAM_FATAL    os_null_printf
#endif

#define PSRAM_DEV_NAME                ("psram")

#define PSRAM_FAILURE                (1)
#define PSRAM_SUCCESS                (0)

#define PSRAM_CMD_MAGIC              (0xa240000)

#define PSRAM_MEM_BASE_S             (0x60000000)
#define PSRAM_MEM_BASE_NS            (0x70000000)

enum
{
	PSRAM_STATE_SECURE = 0,
	PSRAM_STATE_NON_SECURE
};

enum {
	CMD_PSRAM_GET_VERSION = PSRAM_CMD_MAGIC + 1,
	CMD_PSRAM_SOFT_RESET,
	CMD_PSRAM_GET_TYPE,
	CMD_PSRAM_SET_TYPE,
	CMD_PSRAM_GET_REG_READ_LATENCY,
	CMD_PSRAM_SET_REG_READ_LATENCY,
	CMD_PSRAM_GET_REG_WRITE_LATENCY,
	CMD_PSRAM_SET_REG_WRITE_LATENCY,
	CMD_PSRAM_GET_DATA_READ_LATENCY,
	CMD_PSRAM_SET_DATA_READ_LATENCY,
	CMD_PSRAM_GET_DATA_WRITE_LATENCY,
	CMD_PSRAM_SET_DATA_WRITE_LATENCY,
	CMD_PSRAM_GET_SELECT_ADDR,
	CMD_PSRAM_SET_SELECT_ADDR
};

#define PSRAM_BASE                           (SOC_PSRAM_REG_BASE)

#define REG_PSRAM_DEVICE_ID                  (PSRAM_BASE + 0x0 * 4)

#define REG_PSRAM_VERSION_ID                 (PSRAM_BASE + 0x1 * 4)

#define REG_PSRAM_SOFT_RESET                 (PSRAM_BASE + 0x2 * 4)
#define PSRAM_SOFT_RESET_POSI                (0)
#define PSRAM_SOFT_RESET_MASK                (0x1)
#define PSRAM_CKG_BYPASS                     (0x1 << 1)

#define REG_PSRAM_CONFIG                     (PSRAM_BASE + 0x4 * 4)
#define PSRAM_TYPE_POSI                      (0)
#define PSRAM_TYPE_MASK                      (0xF)
#define PSRAM_REG_READ_LATENCY_POSI          (4)
#define PSRAM_REG_READ_LATENCY_MASK          (0xF)
#define PSRAM_REG_WRITE_LATENCY_POSI         (8)
#define PSRAM_REG_WRITE_LATENCY_MASK         (0xF)
#define PSRAM_DATA_READ_LATENCY_POSI         (12)
#define PSRAM_DATA_READ_LATENCY_MASK         (0xF)
#define PSRAM_DATA_WRITE_LATENCY_POSI        (16)
#define PSRAM_DATA_WRITE_LATENCY_MASK        (0xF)
#define PSRAM_ADDR_SELECT_POSI               (20)
#define PSRAM_ADDR_SELECT_MASK               (0xF)

#define REG_PSRAM_REQUEST                    (PSRAM_BASE + 0x8 * 4)
#define PSRAM_REQUEST_REG_WRITE              (0x1 << 0)
#define PSRAM_REQUEST_REG_READ               (0x1 << 1)
#define PSRAM_REQUEST_SOFT_RESET             (0x1 << 2)

#define REG_PSRAM_WR_ADDR                    (PSRAM_BASE + 0x9 * 4)

#define REG_PSRAM_WRITE_DATA                 (PSRAM_BASE + 0xA * 4)

#define REG_PSRAM_READ_DATA                  (PSRAM_BASE + 0xB * 4)

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void bk_psram_mpc_cofig(void);
void bk_psram_init(void);
UINT32 bk_psram_read(char *user_buf, UINT32 count, UINT32 address);
UINT32 bk_psram_write(char *user_buf, UINT32 count, UINT32 address);
UINT32 bk_psram_ctrl(UINT32 cmd, void *param);

#endif
