#ifndef _BK_QSPI_H_
#define _BK_QSPI_H_

void bk_qspi_mpc_config(uint32_t qspi_size, uint32_t qspi_offset, uint32_t sec_state);
void bk_qspi_init(void);
int bk_qspi_quad_direct_write(uint32_t addr, uint8_t *buf, uint32_t buf_len);
int bk_qspi_quad_direct_read(uint32_t addr, uint8_t *buf, uint32_t buf_len);
int bk_qspi_quad_indirect_write(uint32_t addr, uint8_t *buf, uint32_t buf_len);
int bk_qspi_quad_indirect_read(uint32_t addr, uint8_t *buf, uint32_t buf_len);

#endif
