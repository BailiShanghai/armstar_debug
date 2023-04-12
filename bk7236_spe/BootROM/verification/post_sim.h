#pragma once

/* address mapping, summary sheet
 * smem0	 0x28000000
 * smem1	 0x28010000
 * smem2	 0x28020000
 * smem3	 0x28040000
 * smem4	 0x28060000
 */

#define VERIFY_SHARED_MEM0_START_ADDR         (0x28008000)
#define VERIFY_SHARED_MEM0_END_ADDR           (0x28008000 + 20)

#define VERIFY_SHARED_MEM1_START_ADDR         (0x28018000)
#define VERIFY_SHARED_MEM1_END_ADDR           (0x28018000 + 20)

#define VERIFY_SHARED_MEM2_START_ADDR         (0x28038000)
#define VERIFY_SHARED_MEM2_END_ADDR           (0x28038000 + 20)

#define VERIFY_SHARED_MEM3_START_ADDR         (0x28048000)
#define VERIFY_SHARED_MEM3_END_ADDR           (0x28048000 + 20)

#define VERIFY_SHARED_MEM4_START_ADDR         (0x28068000)
#define VERIFY_SHARED_MEM4_END_ADDR           (0x28068000 + 20)

#define DTCM_S_BASE_ADDR                      (0x20000000)
#define DTCM_NS_BASE_ADDR                     (0x30000000)
#define ITCM_S_BASE_ADDR                      (0x0)
#define ITCM_NS_BASE_ADDR                     (0x10000000)

#define VERIFY_ITCM_START_ADDR               (ITCM_S_BASE_ADDR)
#define VERIFY_ITCM_END_ADDR                 (ITCM_S_BASE_ADDR + 20)

#define VERIFY_DTCM_START_ADDR               (DTCM_S_BASE_ADDR)
#define VERIFY_DTCM_END_ADDR                 (DTCM_S_BASE_ADDR + 20)

uint32_t post_sim_verification_main(void);
// eof

