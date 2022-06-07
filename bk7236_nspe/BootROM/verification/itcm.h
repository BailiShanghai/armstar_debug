#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "tcm.h"

#define TCM_BASE_ADDR   (0xE001E010)
#define ITGU_BASE_ADDR  (0xE001E500)

#define ITCM_S_MEM_BASE_ADDR   (0x0)
#define ITCM_NS_MEM_BASE_ADDR   (0x10000000)

typedef volatile tgu_hw_t itgu_hw_t;

extern uint32_t itcm_proof_main(void);

#ifdef __cplusplus
}
#endif
// eof

