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

#ifndef __PLATFORM_H__
#define __PLATFROM_H__

#include "./inc/cmsis.h"
#include "./inc/platform_retarget.h"
#include "./inc/SMM_MPS2.h"
#include "./inc/freq_def.h"
#include "./inc/soc_devices.h"

#define GPIO0_BASE                  GPIO0_BASE_S
#define PLATFORM_SSP1_BASE          MPS2_SSP1_BASE_NS
#define PLATFROM_UART_BASE          UART0_BASE_NS

#endif /* __PLATFROM_H__ */
