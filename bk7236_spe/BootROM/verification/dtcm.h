#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "tcm.h"

#define TCM_BASE_ADDR   (0xE001E010)
#define DTGU_BASE_ADDR  (0xE001E600)
#define DTCM_MEM_BASE_ADDR   (0xE001E010)

typedef volatile tgu_hw_t dtgu_hw_t;
#ifdef __cplusplus
}
#endif
// eof

