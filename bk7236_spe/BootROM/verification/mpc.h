#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    enum
    {
        MPC_STATE_SECURE = 0,
        MPC_STATE_NON_SECURE
    };

#define  MPC_PSRAM_BASE_ADDR   	(0x41100000)
#define  MPC_QSPI0_BASE_ADDR   	(0x41110000)
#define  MPC_QSPI1_BASE_ADDR   	(0x41120000)
#define  MPC_OTP2_BASE_ADDR     (0x41130000)
#define  MPC_FLASH_BASE_ADDR   	(0x41200000)
#define  MPC_SMEM0_BASE_ADDR   	(0x41210000)
#define  MPC_SMEM1_BASE_ADDR   	(0x41220000)
#define  MPC_SMEM2_BASE_ADDR   	(0x41230000)
#define  MPC_SMEM3_BASE_ADDR   	(0x41240000)
#define  MPC_SMEM4_BASE_ADDR   	(0x41250000)

#define MPC_REG_GAP_LOCATION_NUM   (3)
#define MPC_LUT_MAX_NUM  (64)

    typedef volatile struct
    {
        /* REG_0x00*/
        union
        {
            struct
            {
                uint32_t resv0: 4;
                uint32_t cfg_sec_rsp: 1;
                uint32_t rsv: 1;
                uint32_t gating_req: 1;
                uint32_t gating_ack: 1;
                uint32_t auto_inc: 1;
                uint32_t resv1: 22;
                uint32_t sec_lock: 1;
            };
            uint32_t v;
        } mpc_ctrl;

        uint32_t reg_gap[MPC_REG_GAP_LOCATION_NUM];

        /* REG_0x10 */
        uint32_t blk_max;

        /* REG_0x14 */
        uint32_t blk_size;

        /* REG_0x14 */
        uint32_t blk_idx;

        uint32_t blk_lut[MPC_LUT_MAX_NUM];
    } mpc_hw_t;

    extern void mpc_config_ctrl(mpc_hw_t *mpc_ptr, uint8_t sec_resp, uint8_t gating_req, uint8_t auto_increase, uint8_t sec_lock);
    extern void mpc_set_secure(mpc_hw_t *mpc_ptr, uint32_t psram_size, uint32_t psram_offset, uint32_t sec_state);
#ifdef __cplusplus
}
#endif
// eof


