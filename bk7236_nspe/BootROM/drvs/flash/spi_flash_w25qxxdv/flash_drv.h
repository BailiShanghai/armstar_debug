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

#ifndef __FLASH_DRV_H__
#define __FLASH_DRV_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t flash_drv_init(size_t max_flash_size, uint32_t spi_freq,
                       uint32_t spi_slave_num, uint32_t spi_dev_id_value);
int32_t flash_drv_read(uint32_t offset, uint8_t *data, size_t len);
int32_t flash_drv_write(uint32_t offset, const uint8_t *data, size_t len);
int32_t flash_drv_erase(uint32_t offset, size_t len);
int32_t flash_drv_write_protection_set(bool enable);

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_DRV_H__ */
