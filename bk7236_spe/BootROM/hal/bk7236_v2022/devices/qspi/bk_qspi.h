#ifndef _BK_QSPI_H_
#define _BK_QSPI_H_

void bk_qspi_init(void);
int bk_qspi_write(uint8_t *buf, uint32_t buf_len);
int bk_qspi_read(uint8_t *buf, uint32_t buf_len);

#endif
