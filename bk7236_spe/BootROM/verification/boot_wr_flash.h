#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"

    enum {
        BRET_OK = 0,
        BRET_ERR,
        BRET_INVALID,
        BRET_NO_MEMORY,
        BRET_MAX
    };

#define RW_DATA_SIZE         (64)
#define RW_DATA_CHUNK_SIZE   (128) /*2s multiply*/
#define ERASE_SECTOR_SIZE    (4096)
#define SPECIFIED_SEC_ADDR   (0x02000000)
#define FLASH_ADDR_FROM_ID(obj, id)  (obj->base_addr + id * RW_DATA_CHUNK_SIZE)

    /*index 0 for data content count(management field), and other index for data content*/
#define DATA_CHUNK_MAX_IDX  (ERASE_SECTOR_SIZE / RW_DATA_CHUNK_SIZE)
#define DATA_CHUNK_MAX_CNT  (DATA_CHUNK_MAX_IDX - 1)

#if ((0 == ERASE_SECTOR_SIZE) || (0 == DATA_CHUNK_MAX_IDX) || (0 == RW_DATA_CHUNK_SIZE) || (ERASE_SECTOR_SIZE & (RW_DATA_CHUNK_SIZE)))
#error 
    "check the macro setting of RW_DATA_CHUNK_SIZE and ERASE_SECTOR_SIZE"
#endif

    typedef struct _sec_obj_
    {
        uint32_t base_addr;
        uint32_t total_size;
        uint32_t chunk_size;
        void *resv;
    } SEC_OBJ_T, *SEC_OBJ_PTR;

    extern hal_ret_t boot_write_chunk(SEC_OBJ_T *sec_obj, void *chunk_ptr, uint32_t chunk_size);
    extern hal_ret_t boot_read_chunk(SEC_OBJ_T *sec_obj, void *chunk_ptr, uint32_t chunk_size);

#ifdef __cplusplus
}
#endif
// eof



