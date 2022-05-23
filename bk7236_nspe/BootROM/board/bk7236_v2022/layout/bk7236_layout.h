/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __BK7236_2022_LAYOUT_H__
#define __BK7236_2022_LAYOUT_H__

/* ***************** README ******************
 *
 * The memory layout macros are in the format of:
 *      LAYOUT_[name_abbreviation]_[FLASH/ROM/RAM]_[OFFSET/SIZE/START]
 * For example:
 *      LAYOUT_PRIM_BL_FLASH_OFFSET is the primary bootloader offset in spi flash
 * Here the size is the maximum available space.
 * 
 * This is the Asymmetric mode layout, for using Symmetric mode layout, please reference to
 * bk7236_layout_symmetric.h
 */
/*
 * Internal definations, don't use them
 */
#define __LAYOUT_FLASH_BASE_BLOCK_SIZE  (0x00040000) //TODO fix me, is the size enough??
#define __LAYOUT_ROM_BASE_BLOCK_SIZE    (__LAYOUT_FLASH_BASE_BLOCK_SIZE)

/*
 * Defines the SoC memory map
 */
#define LAYOUT_ROM_NS_BASE          (0x06000000)
#define LAYOUT_ROM_S_BASE           (0x16000000)
#define LAYOUT_TOTAL_ROM_SIZE       (0x00200000)
#define LAYOUT_ROM_SIZE_S           (__LAYOUT_ROM_BASE_BLOCK_SIZE * 9) /* bootrom, bootloader, TEE, recovery bootloader */
#define LAYOUT_ROM_SIZE_NS          (__LAYOUT_ROM_BASE_BLOCK_SIZE * 7) /* REE */

#define LAYOUT_RAM_NS_BASE          (0x28000000)
#define LAYOUT_RAM_S_BASE           (0x38000000)
#define LAYOUT_TOTAL_RAM_SIZE       (0x00200000)  //TODO fix me
#define LAYOUT_RAM_SIZE_S           (LAYOUT_TOTAL_RAM_SIZE / 2)
#define LAYOUT_RAM_SIZE_NS          (LAYOUT_TOTAL_RAM_SIZE / 2)

#define LAYOUT_SPI_FLASH_START      (0x12000000)    /*secure flash memory address, beken*/

/*
 * Defines the SPI Flash internal layout
 */
/* primary bootloader */
#define LAYOUT_PRIM_BL_FLASH_OFFSET         (0x00040000)
#define LAYOUT_PRIM_BL_FLASH_SIZE           (__LAYOUT_FLASH_BASE_BLOCK_SIZE)

/* tee */
#define LAYOUT_TEE_FLASH_OFFSET             (0x00080000)
#define LAYOUT_TEE_FLASH_SIZE               (__LAYOUT_FLASH_BASE_BLOCK_SIZE * 6)

/* ree, zephyr or FreeRTOS */
#define LAYOUT_REE_FLASH_OFFSET             (0x00200000)
#define LAYOUT_REE_FLASH_SIZE               (__LAYOUT_FLASH_BASE_BLOCK_SIZE * 7)

/* primary manifest */
#define LAYOUT_PRIM_MANIFEST_FLASH_OFFSET   (0x003C0000) //TODO fix me
#define LAYOUT_PRIM_MANIFEST_FLASH_SIZE     (__LAYOUT_FLASH_BASE_BLOCK_SIZE)

/* recovery bootloader */
#define LAYOUT_RECV_BL_FLASH_OFFSET         (0x00400000)
#define LAYOUT_RECV_BL_FLASH_SIZE           (__LAYOUT_FLASH_BASE_BLOCK_SIZE)

/* recovery manifest */
#define LAYOUT_RECV_MANIFEST_FLASH_OFFSET   (0x00440000) //TODO fix me
#define LAYOUT_RECV_MANIFEST_FLASH_SIZE     (__LAYOUT_FLASH_BASE_BLOCK_SIZE)

/* cache partition */
#define LAYOUT_CACHE_PARTITION_FLASH_OFFSET (0x00480000)
#define LAYOUT_CACHE_PARTITION_FLASH_SIZE   (__LAYOUT_FLASH_BASE_BLOCK_SIZE * 2)

/* control partition */
#define LAYOUT_CTRL_PARTITION_FLASH_OFFSET  (0x00500000) //TODO fix me
#define LAYOUT_CTRL_PARTITION_FLASH_SIZE    (__LAYOUT_FLASH_BASE_BLOCK_SIZE)

/* secure storage */
#define LAYOUT_SST_FLASH_OFFSET             (0x00540000)
#define LAYOUT_SST_FLASH_SIZE               (0x00100000)

/* application storage/non-secure storage */
#define LAYOUT_APP_ST_FLASH_OFFSET          (0x00640000)
#define LAYOUT_APP_ST_FLASH_SIZE            (0x00400000)

/* the simulated OTP */
#define LAYOUT_SIMU_OTP_FLASH_OFFSET        (0x00A40000) //TODO fix me
#define LAYOUT_SIMU_OTP_FLASH_SIZE          (__LAYOUT_FLASH_BASE_BLOCK_SIZE)

/*
 * Defines the ROM internal layout
 */
/* BOOTROM */
#define LAYOUT_BOOTROM_ROM_START            (LAYOUT_ROM_S_BASE + 0x00000000)
#define LAYOUT_BOOTROM_ROM_SIZE             (__LAYOUT_ROM_BASE_BLOCK_SIZE)

/* primary bootloader */
#define LAYOUT_PRIM_BL_ROM_OFFSET           (0x00040000)
#define LAYOUT_PRIM_BL_ROM_START            (LAYOUT_ROM_S_BASE + LAYOUT_PRIM_BL_ROM_OFFSET)
#define LAYOUT_PRIM_BL_ROM_SIZE             (LAYOUT_PRIM_BL_FLASH_SIZE)

/* tee */
#define LAYOUT_TEE_ROM_OFFSET               (0x00080000)
#define LAYOUT_TEE_ROM_START                (LAYOUT_ROM_S_BASE + LAYOUT_TEE_ROM_OFFSET)
#define LAYOUT_TEE_ROM_SIZE                 (LAYOUT_TEE_FLASH_SIZE)

/* ree */
#define LAYOUT_REE_ROM_OFFSET               (0x00200000)
#define LAYOUT_REE_ROM_START                (LAYOUT_ROM_NS_BASE + LAYOUT_REE_ROM_OFFSET)
#define LAYOUT_REE_ROM_SIZE                 (LAYOUT_REE_FLASH_SIZE)

/* recovery bootloader */
#define LAYOUT_RECV_BL_ROM_OFFSET           (0x003C0000)
#define LAYOUT_RECV_BL_ROM_START            (LAYOUT_ROM_S_BASE + LAYOUT_RECV_BL_ROM_OFFSET)
#define LAYOUT_RECV_BL_ROM_SIZE             (LAYOUT_RECV_BL_FLASH_SIZE)

/*
 * Defines the RAM internal layout
 */
/* BOOTROM */
#define LAYOUT_BOOTROM_RAM_START            (LAYOUT_RAM_S_BASE)
#define LAYOUT_BOOTROM_RAM_SIZE             (LAYOUT_TOTAL_RAM_SIZE)

/* primary bootloader */
#define LAYOUT_PRIM_BL_RAM_START            (LAYOUT_RAM_S_BASE)
#define LAYOUT_PRIM_BL_RAM_SIZE             (LAYOUT_TOTAL_RAM_SIZE)

/* recovery bootloader */
#define LAYOUT_RECV_BL_RAM_START            (LAYOUT_RAM_S_BASE)
#define LAYOUT_RECV_BL_RAM_SIZE             (LAYOUT_TOTAL_RAM_SIZE)

/* ree */
#define LAYOUT_REE_RAM_START                (LAYOUT_RAM_NS_BASE)
#define LAYOUT_REE_RAM_SIZE                 (LAYOUT_RAM_SIZE_NS)

/* tee */
#define LAYOUT_TEE_RAM_START                (LAYOUT_RAM_S_BASE + (LAYOUT_TOTAL_RAM_SIZE / 2))
#define LAYOUT_TEE_RAM_SIZE                 (LAYOUT_RAM_SIZE_S)

#endif /* __BK7236_2022_LAYOUT_H__ */
