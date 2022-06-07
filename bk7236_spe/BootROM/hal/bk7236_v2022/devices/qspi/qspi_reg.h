#ifndef _QSPI_REG_H_
#define _QSPI_REG_H_

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define QSPI_DEBUG
#ifdef QSPI_DEBUG
#define QSPI_PRT      os_null_printf
#define QSPI_WARN     os_null_printf
#define QSPI_FATAL    os_null_printf
#else
#define QSPI_PRT      os_null_printf
#define QSPI_WARN     os_null_printf
#define QSPI_FATAL    os_null_printf
#endif

#define QSPI_0_DEV_NAME                ("qspi0")
#define QSPI_1_DEV_NAME                ("qspi1")

#define QSPI_FAILURE                (1)
#define QSPI_SUCCESS                (0)

#define QSPI_CMD_MAGIC              (0xa250000)

#define QSPI_0_MEM_BASE_S             (0x64000000)
#define QSPI_0_MEM_BASE_NS            (0x74000000)
#define QSPI_1_MEM_BASE_S             (0x68000000)
#define QSPI_1_MEM_BASE_NS            (0x78000000)

#define QSPI_CORE_STATUS_IDLE         (0x0)
#define QSPI_CORE_STATUS_SEND_CMD     (0x2)
#define QSPI_CORE_STATUS_DUMMY_CYCLE  (0x4)
#define QSPI_CORE_STATUS_WRITE_DATA   (0x8)
#define QSPI_CORE_STATUS_READ_DATA    (0x10)
#define QSPI_CORE_STATUS_DONE         (0x20)
#define QSPI_CORE_STATUS_SPI_CSN_H    (0x40)

#define QSPI_1_LINE         (0)
#define QSPI_2_LINE         (1)
#define QSPI_4_LINE         (2)
#define QSPI_CMD_DISABLE    (3)

#define QSPI_0_BASE      (0x46040000)
#define QSPI_1_BASE      (0x46060000)

#ifdef __cplusplus
}
#endif

#endif
