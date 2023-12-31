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

#ifndef __TYPES_H__
#define __TYPES_H__

#ifndef __ASSEMBLY__

#ifdef __cplusplus
extern "C" {
}
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
////#include <unistd.h>  /* for off_t */

#ifndef __ULONG_T_DEFINED__
typedef unsigned long ulong_t;
#define __ULONG_T_DEFINED__
#endif

#ifndef __ERR_T_DEFINED__
typedef int32_t err_t;
#define __ERR_T_DEFINED__
#endif

#ifndef __ADDR_T_DEFINED__
typedef unsigned long addr_t;
#define __ADDR_T_DEFINED__
#endif

#ifdef __cplusplus
}
#endif

#endif /* !__ASSEMBLY__ */

#endif /* __TYPES_H__ */
