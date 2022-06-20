#ifndef _BK_QSPI_H_
#define _BK_QSPI_H_

#include "qspi_types.h"

int bk_qspi_enter_quad_mode(uint8_t cmd);
int bk_qspi_exit_quad_mode(uint8_t cmd);
uint32_t bk_qspi_read_id(uint8_t cmd);
int bk_qspi_cpu_write(qspi_config_t *qspi_config, uint8_t *buf, uint32_t buf_len);
int bk_qspi_cpu_read(qspi_config_t *qspi_config, uint8_t *buf, uint32_t buf_len);
int bk_qspi_io_write(qspi_config_t *qspi_config, uint8_t *buf, uint32_t buf_len);
int bk_qspi_io_read(qspi_config_t *qspi_config, uint8_t *buf, uint32_t buf_len);
void bk_qspi_mpc_config(void);
void bk_qspi_init(void);

#endif
