#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "tcm.h"

#define TCM_BASE_ADDR   (0xE001E010)
#define ITGU_BASE_ADDR  (0xE001E500)
#define ITCM_MEM_BASE_ADDR   (0xE001E010)

typedef volatile tgu_hw_t itgu_hw_t;
#ifdef __cplusplus
}
#endif
// eof

