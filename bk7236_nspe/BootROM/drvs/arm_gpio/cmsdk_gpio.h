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

#ifndef __CMSDK_GPIO_H__
#define __CMSDK_GPIO_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------- General Purpose Input Output (GPIO) -------------------*/
typedef struct
{
  __IO   uint32_t  DATA;                     /* Offset: 0x000 (R/W) DATA Register */
  __IO   uint32_t  DATAOUT;                  /* Offset: 0x004 (R/W) Data Output Latch Register */
         uint32_t  RESERVED0[2];
  __IO   uint32_t  OUTENABLESET;             /* Offset: 0x010 (R/W) Output Enable Set Register */
  __IO   uint32_t  OUTENABLECLR;             /* Offset: 0x014 (R/W) Output Enable Clear Register */
  __IO   uint32_t  ALTFUNCSET;               /* Offset: 0x018 (R/W) Alternate Function Set Register */
  __IO   uint32_t  ALTFUNCCLR;               /* Offset: 0x01C (R/W) Alternate Function Clear Register */
  __IO   uint32_t  INTENSET;                 /* Offset: 0x020 (R/W) Interrupt Enable Set Register */
  __IO   uint32_t  INTENCLR;                 /* Offset: 0x024 (R/W) Interrupt Enable Clear Register */
  __IO   uint32_t  INTTYPESET;               /* Offset: 0x028 (R/W) Interrupt Type Set Register */
  __IO   uint32_t  INTTYPECLR;               /* Offset: 0x02C (R/W) Interrupt Type Clear Register */
  __IO   uint32_t  INTPOLSET;                /* Offset: 0x030 (R/W) Interrupt Polarity Set Register */
  __IO   uint32_t  INTPOLCLR;                /* Offset: 0x034 (R/W) Interrupt Polarity Clear Register */
  union {
    __I    uint32_t  INTSTATUS;              /* Offset: 0x038 (R/ ) Interrupt Status Register */
    __O    uint32_t  INTCLEAR;               /* Offset: 0x038 ( /W) Interrupt Clear Register */
    };
         uint32_t RESERVED1[241];
  __IO   uint32_t LB_MASKED[256];            /* Offset: 0x400 - 0x7FC Lower byte Masked Access Register (R/W) */
  __IO   uint32_t UB_MASKED[256];            /* Offset: 0x800 - 0xBFC Upper byte Masked Access Register (R/W) */
} CMSDK_GPIO_TypeDef;

/* CMSDK_GPIO DATA Register Definitions */
#define CMSDK_GPIO_DATA_Pos            0                                          /* CMSDK_GPIO DATA: DATA Position */
#define CMSDK_GPIO_DATA_Msk            (0xFFFFul << CMSDK_GPIO_DATA_Pos)          /* CMSDK_GPIO DATA: DATA Mask */

/* CMSDK_GPIO DATAOUT Register Definitions */
#define CMSDK_GPIO_DATAOUT_Pos         0                                          /* CMSDK_GPIO DATAOUT: DATAOUT Position */
#define CMSDK_GPIO_DATAOUT_Msk         (0xFFFFul << CMSDK_GPIO_DATAOUT_Pos)       /* CMSDK_GPIO DATAOUT: DATAOUT Mask */

/* CMSDK_GPIO OUTENSET Register Definitions */
#define CMSDK_GPIO_OUTENSET_Pos        0                                          /* CMSDK_GPIO OUTEN: OUTEN Position */
#define CMSDK_GPIO_OUTENSET_Msk        (0xFFFFul << CMSDK_GPIO_OUTEN_Pos)         /* CMSDK_GPIO OUTEN: OUTEN Mask */

/* CMSDK_GPIO OUTENCLR Register Definitions */
#define CMSDK_GPIO_OUTENCLR_Pos        0                                          /* CMSDK_GPIO OUTEN: OUTEN Position */
#define CMSDK_GPIO_OUTENCLR_Msk        (0xFFFFul << CMSDK_GPIO_OUTEN_Pos)         /* CMSDK_GPIO OUTEN: OUTEN Mask */

/* CMSDK_GPIO ALTFUNCSET Register Definitions */
#define CMSDK_GPIO_ALTFUNCSET_Pos      0                                          /* CMSDK_GPIO ALTFUNC: ALTFUNC Position */
#define CMSDK_GPIO_ALTFUNCSET_Msk      (0xFFFFul << CMSDK_GPIO_ALTFUNC_Pos)       /* CMSDK_GPIO ALTFUNC: ALTFUNC Mask */

/* CMSDK_GPIO ALTFUNCCLR Register Definitions */
#define CMSDK_GPIO_ALTFUNCCLR_Pos      0                                          /* CMSDK_GPIO ALTFUNC: ALTFUNC Position */
#define CMSDK_GPIO_ALTFUNCCLR_Msk      (0xFFFFul << CMSDK_GPIO_ALTFUNC_Pos)       /* CMSDK_GPIO ALTFUNC: ALTFUNC Mask */

/* CMSDK_GPIO INTENSET Register Definitions */
#define CMSDK_GPIO_INTENSET_Pos        0                                          /* CMSDK_GPIO INTEN: INTEN Position */
#define CMSDK_GPIO_INTENSET_Msk        (0xFFFFul << CMSDK_GPIO_INTEN_Pos)         /* CMSDK_GPIO INTEN: INTEN Mask */

/* CMSDK_GPIO INTENCLR Register Definitions */
#define CMSDK_GPIO_INTENCLR_Pos        0                                          /* CMSDK_GPIO INTEN: INTEN Position */
#define CMSDK_GPIO_INTENCLR_Msk        (0xFFFFul << CMSDK_GPIO_INTEN_Pos)         /* CMSDK_GPIO INTEN: INTEN Mask */

/* CMSDK_GPIO INTTYPESET Register Definitions */
#define CMSDK_GPIO_INTTYPESET_Pos      0                                          /* CMSDK_GPIO INTTYPE: INTTYPE Position */
#define CMSDK_GPIO_INTTYPESET_Msk      (0xFFFFul << CMSDK_GPIO_INTTYPE_Pos)       /* CMSDK_GPIO INTTYPE: INTTYPE Mask */

/* CMSDK_GPIO INTTYPECLR Register Definitions */
#define CMSDK_GPIO_INTTYPECLR_Pos      0                                          /* CMSDK_GPIO INTTYPE: INTTYPE Position */
#define CMSDK_GPIO_INTTYPECLR_Msk      (0xFFFFul << CMSDK_GPIO_INTTYPE_Pos)       /* CMSDK_GPIO INTTYPE: INTTYPE Mask */

/* CMSDK_GPIO INTPOLSET Register Definitions */
#define CMSDK_GPIO_INTPOLSET_Pos       0                                          /* CMSDK_GPIO INTPOL: INTPOL Position */
#define CMSDK_GPIO_INTPOLSET_Msk       (0xFFFFul << CMSDK_GPIO_INTPOL_Pos)        /* CMSDK_GPIO INTPOL: INTPOL Mask */

/* CMSDK_GPIO INTPOLCLR Register Definitions */
#define CMSDK_GPIO_INTPOLCLR_Pos       0                                          /* CMSDK_GPIO INTPOL: INTPOL Position */
#define CMSDK_GPIO_INTPOLCLR_Msk       (0xFFFFul << CMSDK_GPIO_INTPOL_Pos)        /* CMSDK_GPIO INTPOL: INTPOL Mask */

/* CMSDK_GPIO INTSTATUS Register Definitions */
#define CMSDK_GPIO_INTSTATUS_Pos       0                                          /* CMSDK_GPIO INTSTATUS: INTSTATUS Position */
#define CMSDK_GPIO_INTSTATUS_Msk       (0xFFul << CMSDK_GPIO_INTSTATUS_Pos)       /* CMSDK_GPIO INTSTATUS: INTSTATUS Mask */

/* CMSDK_GPIO INTCLEAR Register Definitions */
#define CMSDK_GPIO_INTCLEAR_Pos        0                                          /* CMSDK_GPIO INTCLEAR: INTCLEAR Position */
#define CMSDK_GPIO_INTCLEAR_Msk        (0xFFul << CMSDK_GPIO_INTCLEAR_Pos)        /* CMSDK_GPIO INTCLEAR: INTCLEAR Mask */

/* CMSDK_GPIO MASKLOWBYTE Register Definitions */
#define CMSDK_GPIO_MASKLOWBYTE_Pos     0                                          /* CMSDK_GPIO MASKLOWBYTE: MASKLOWBYTE Position */
#define CMSDK_GPIO_MASKLOWBYTE_Msk     (0x00FFul << CMSDK_GPIO_MASKLOWBYTE_Pos)   /* CMSDK_GPIO MASKLOWBYTE: MASKLOWBYTE Mask */

/* CMSDK_GPIO MASKHIGHBYTE Register Definitions */
#define CMSDK_GPIO_MASKHIGHBYTE_Pos    0                                          /* CMSDK_GPIO MASKHIGHBYTE: MASKHIGHBYTE Position */
#define CMSDK_GPIO_MASKHIGHBYTE_Msk    (0xFF00ul << CMSDK_GPIO_MASKHIGHBYTE_Pos)  /* CMSDK_GPIO MASKHIGHBYTE: MASKHIGHBYTE Mask */

#ifdef __cplusplus
}
#endif

#endif /* __CMSDK_GPIO_H__ */
