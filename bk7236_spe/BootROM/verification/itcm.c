#include "base.h"
#include "bk_uart.h"
#include "itcm.h"
#include <math.h>

itgu_hw_t *itgu_hw_ptr;
tcm_hw_t *tcm_hw_ptr;

void itcm_head_test(void) __attribute__((section("ITCM_HEAD")));
void itcm_head_test(void)
{
    bk_printf("itcm_head_test\r\n");
}

void itcm_tail_test(void) __attribute__((section("ITCM_TAIL")));
void itcm_tail_test(void)
{
    bk_printf("itcm_tail_test\r\n");
}

uint32_t itcm_lut_config(uint32_t total_blk_num, uint32_t previous_blk_num, uint32_t sec_state)
{
    uint32_t sec_mapping_bit = 0;
    uint32_t prv_reg_val = 0;
    uint32_t is_mix_flag = 0;
    uint32_t back_reg_val, mix_reg_val;
    uint32_t i, int_val, mod_val;

    if(STATE_NON_SECURE == sec_state)
    {
        sec_mapping_bit = 1;
        prv_reg_val = 0xFFFFFFFF;
    }
    back_reg_val = ~prv_reg_val;

    int_val = previous_blk_num / 32;
    mod_val = previous_blk_num % 32;
    if(mod_val)
        is_mix_flag = 1;

    for(i = 0; i < ((total_blk_num + 31) / 32); i ++)
    {
        if(i < int_val)
            itgu_hw_ptr->sec_lookup_tab[i] = prv_reg_val;
        else
            itgu_hw_ptr->sec_lookup_tab[i] = back_reg_val;
    }

    mix_reg_val = back_reg_val;
    for(i = 0; i < mod_val; i ++)
    {
        mix_reg_val &= (~(1 << i));
        mix_reg_val |= ((sec_mapping_bit & 0x01) << i);
    }

    if(is_mix_flag)
    {
        itgu_hw_ptr->sec_lookup_tab[int_val] = mix_reg_val;
    }

    return 0;
}

uint32_t itcm_config(void)
{
    uint32_t ret;
    uint32_t tcm_size, blk_num, blk_size;

    itgu_hw_ptr->tgu_ctrl.data_busfault_en = 1;
    itgu_hw_ptr->tgu_ctrl.ahbt_err_resp = 1;
    tcm_hw_ptr->itcm_ctrl.en = 1;

    tcm_size = tcm_hw_ptr->itcm_ctrl.size;
    blk_num = pow(2, itgu_hw_ptr->tgu_cfg.num_of_blks);
    blk_size = pow(2, itgu_hw_ptr->tgu_cfg.blk_size + 5);
    bk_printf("itcm total size:0x%x blk_num:0x%x, blk_size:0x%x\r\n",
              blk_num * blk_size,
              blk_num,
              blk_size);

    /* previous section is secure, and the other is non-secure*/
    ret = itcm_lut_config(blk_num, blk_num >> 1, STATE_SECURE);

    return ret;
}

uint32_t itcm_init(void)
{
    uint32_t ret;

    tcm_hw_ptr = (tcm_hw_t *)TCM_BASE_ADDR;
    itgu_hw_ptr = (itgu_hw_t *)ITGU_BASE_ADDR;

    ret = itcm_config();

    return ret;
}

uint32_t itcm_verification_main(void)
{
    itcm_init();

    itcm_head_test();
    itcm_tail_test();

    return 0;
}

// eof

