/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 * 
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __SPI_PL022_H__
#define __SPI_PL022_H__

#include <stdint.h>
#include <stdbool.h>
#include "spi_drv.h"
#include "types.h"

typedef void (*cs_select_func_t)(uint32_t port, bool is_select);
extern int spi_pl022_init(uint32_t port,
                          addr_t base_addr,
                          uint32_t clk_freq,
                          cs_select_func_t cs_select);
extern int spi_pl022_transceive(uint32_t port,
                                const struct spi_config *config,
                                const struct spi_buf_set *tx_bufs,
                                const struct spi_buf_set *rx_bufs);

#endif /* __SPI_PL022_H__ */
