#include "base.h"
#include "bk_uart.h"
#include "bk_flash.h"
#include "boot_wr_flash.h"
#include "hal_common.h"

SEC_OBJ_T sec_object =
{
    .base_addr = SPECIFIED_SEC_ADDR,
    .total_size = ERASE_SECTOR_SIZE,
    .chunk_size = RW_DATA_SIZE,
    .resv = 0,
};

void bk_flash_set_protect(PROTECT_TYPE type);

typedef struct _control_flag_
{
    uint32_t magic_word;
    uint32_t boot_path;
    uint32_t primary_addr;
} control_flag_t;

control_flag_t control_struct =
{
    0xdeadbeaf,
    1,
    0x3000
};

void bwf_read(void)
{
    hal_ret_t ret;
    control_flag_t control = {0};

    ret = boot_read_chunk(&sec_object, (void *)&control, sizeof(control));
    if(HAL_OK != ret)
    {
        bk_printf("read failed\r\n");
        return;
    }

    bk_printf("read0:0x%x:0x%x:0x%x\r\n", control.magic_word, control.boot_path, control.primary_addr);
    (void)ret;
}

void bwf_write(void)
{
    bwf_read();
    bk_flash_set_protect(0);

    control_struct.primary_addr += 4;
    boot_write_chunk(&sec_object, (void *)&control_struct, sizeof(control_struct));
    bwf_read();
}
// eof

