#include "base.h"
#include "mpc.h"
#include "bk_uart.h"

static void mpc_config_lut(mpc_hw_t *mpc_ptr, UINT32 blk_num, UINT32 blk_offset, UINT32 sec_state)
{
    UINT32 i;
    UINT32 lut_idx_offset, lut_mod_offset;
    UINT32 lut_tail_len;
    UINT32 lut_idx, lut_mod;

    /*get offset idx and mod*/
    lut_idx_offset = (blk_offset / 32);
    lut_mod_offset = (blk_offset % 32);

    /*set lut offset tail*/
    lut_tail_len = (blk_num > (32 - lut_mod_offset)) ? (32 - lut_mod_offset) : blk_num;
    lut_mod = lut_mod_offset + lut_tail_len;
    for (i = lut_mod_offset; i < lut_mod; i++)
    {
        if (sec_state == MPC_STATE_NON_SECURE)
        {
            mpc_ptr->blk_lut[lut_idx_offset] |= (0x1 << i);
        }
        else
        {
            mpc_ptr->blk_lut[lut_idx_offset] &= ~(0x1 << i);
        }
    }

    /*set left block num of lut*/
    lut_idx_offset++;
    lut_idx = ((blk_num - lut_tail_len) / 32) + lut_idx_offset;
    lut_mod = ((blk_num - lut_tail_len) % 32);
    for (i = lut_idx_offset; i < lut_idx; i++)
    {
        if (sec_state == MPC_STATE_NON_SECURE)
        {
            mpc_ptr->blk_lut[i] = (UINT32)(~0x0);
        }
        else
        {
            mpc_ptr->blk_lut[i] = (UINT32)(0x0);
        }
    }

    for (i = 0; i < lut_mod; i++)
    {
        if (sec_state == MPC_STATE_NON_SECURE)
        {
            mpc_ptr->blk_lut[lut_idx] |= (0x1 << i);
        }
        else
        {
            mpc_ptr->blk_lut[lut_idx] &= ~(0x1 << i);
        }
    }
}

void mpc_config_ctrl(mpc_hw_t *mpc_ptr, uint8_t sec_resp, uint8_t gating_req, uint8_t auto_increase, uint8_t sec_lock)
{
    /*config mpc to auto increase*/
    mpc_ptr->mpc_ctrl.cfg_sec_rsp = 1; /* error debug*/
    mpc_ptr->mpc_ctrl.gating_req = gating_req;
    mpc_ptr->mpc_ctrl.auto_inc = auto_increase;
    mpc_ptr->mpc_ctrl.sec_lock = sec_lock;
}

void mpc_set_secure(mpc_hw_t *mpc_ptr, uint32_t total_size, uint32_t offset_size, uint32_t secure_state)
{
    uint32_t blk_size, blk_idx;
    uint32_t blk_num, blk_offset;

    /*get block number & block offset*/
    blk_size = (32 << mpc_ptr->blk_size);   // 64k Bytes
    blk_num = (total_size / blk_size);      //TODO, integer multpules? if total size is 0x20000, blk_num is 2
    blk_offset = (offset_size / blk_size);  //TODO, integer multpules?
    blk_idx = blk_offset + blk_num;

    /*check block idx*/
    if (blk_idx > mpc_ptr->blk_max)
    {
        bk_printf("idx error, idx:%d, max:%d\r\n", blk_idx, mpc_ptr->blk_max);
        return;
    }
    else
    {
        mpc_ptr->blk_idx = blk_idx;
    }

    /*config mpc lut*/
    mpc_config_lut(mpc_ptr, blk_num, blk_offset, secure_state);
}
// eof
