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

#include <stdint.h>
#include <stddef.h>
#include "base.h"
#include "err.h"
#include "device.h"
#include "flash_dev.h"
#include "bk_flash.h"

#define DBG_PRE_PROMPT  "dd_flash"
#include "dbg.h"

#define DRV_PROMPT "FLASH DRV"
#include "drv_dep.h"

size_t g_flash_max_size = 0;

#define FLASH_MAX_MEM_SIZE (g_flash_max_size)
#define FLASH_OFFSET_TO_ADDR(offset)    (offset + LAYOUT_SPI_FLASH_START)

#define __MASK(x, align) ((x) & ((align)-1))
#define __IS_ALIGN(x, align) (0 == __MASK(x, align))

int32_t flash_init(struct device *dev)
{
    struct flash_dev_data *data = dev->driver_data;

    if (NULL == data) {
        return ERR_NOT_FOUND;
    }

    g_flash_max_size = data->max_flash_size;
	bk_flash_init();
	
    return 0;
}

static int32_t _flash_read(struct device *dev, uint32_t offset, uint8_t *data,
                           size_t len)
{
    int32_t ret = NO_ERROR;
	uint32_t address = FLASH_OFFSET_TO_ADDR(offset);

    DBG("Flash Read offset: %d size: %d\n", offset, len);
    (void)dev;
    DRV_CHECK_CONDITION(offset < FLASH_MAX_MEM_SIZE, ERR_INVALID_ARGS,
                    "Invalid offset: 0x%x\n", offset);
    DRV_CHECK_CONDITION(len < FLASH_MAX_MEM_SIZE, ERR_INVALID_ARGS,
                    "Invalid len: 0x%x\n", len);
    DRV_CHECK_CONDITION((offset + len) < FLASH_MAX_MEM_SIZE, ERR_INVALID_ARGS,
                    "Invalid offset + len: 0x%x + 0x%x\n", offset, len);

    ret = bk_flash_read((char *)data, len, address);
    DRV_CHECK_RET("flash_drv_read[read] failed: 0x%x \n", ret);

finish:
    return ret;
}

static int32_t _flash_write(struct device *dev, uint32_t offset,
                            const uint8_t *data, size_t len)
{
	uint32_t address = FLASH_OFFSET_TO_ADDR(offset);
	
    (void)dev;
    return bk_flash_write((char *)data, len, address);
}

static int32_t _flash_erase(struct device *dev, uint32_t offset, size_t len)
{
	uint32_t count = len;
	uint32_t current_addr;
	uint32_t address = FLASH_OFFSET_TO_ADDR(offset);

    (void)dev;

	current_addr = address;	
	while(count >= FLASH_SECTOR_SIZE)
	{
		bk_flash_erase_sector(current_addr);

		count -= FLASH_SECTOR_SIZE;
		current_addr += FLASH_SECTOR_SIZE;
	}
	
    return 0;
}

static int32_t _flash_write_protection_set(struct device *dev, bool enable)
{
    (void)dev;
	
	if(enable){
		bk_flash_set_protect(FLASH_PROTECT_ALL);
	}else{
		bk_flash_set_protect(FLASH_PROTECT_NONE);
	}

    return 0;
}

const struct flash_driver_api flash_api_funcs = {
    .read                 = _flash_read,
    .write                = _flash_write,
    .erase                = _flash_erase,
    .write_protection_set = _flash_write_protection_set,
};

int32_t flash_read(struct device *dev, uint32_t offset, uint8_t *data,
                   size_t len)
{
    const struct flash_driver_api *drv_api = dev->driver_api;

    if (NULL == drv_api) {
        return ERR_NOT_FOUND;
    }
    return drv_api->read(dev, offset, data, len);
}

int32_t flash_write(struct device *dev, uint32_t offset, const uint8_t *data,
                    size_t len)
{
    const struct flash_driver_api *drv_api = dev->driver_api;

    if (NULL == drv_api) {
        return ERR_NOT_FOUND;
    }
    return drv_api->write(dev, offset, data, len);
}

int32_t flash_erase(struct device *dev, uint32_t offset, size_t size)
{
    const struct flash_driver_api *drv_api = dev->driver_api;

    if (NULL == drv_api) {
        return ERR_NOT_FOUND;
    }
    return drv_api->erase(dev, offset, size);
}

int32_t flash_write_protection_set(struct device *dev, bool enable)
{
    const struct flash_driver_api *drv_api = dev->driver_api;

    if (NULL == drv_api) {
        return ERR_NOT_FOUND;
    }
    return drv_api->write_protection_set(dev, enable);
}
// eof

