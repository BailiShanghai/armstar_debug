/**
 * @file	test_doxygen.h
 * @brief	This file contains examples for doxygen.
 * @author 	xxx (xxx@armchina.com)
 *
 * @date    	2018/04/09
 *
 * @if copyright_display
 *		Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *		All rights reserved
 *
 *	 	The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *		to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *		License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *		restricting among other things, the use, reproduction, distribution
 *		and transfer.  Each of the embodiments, including this information and
 *		any derivative work shall retain this copyright notice.
 * @endif
 *
 *
 * @if modification_History
 *
 *		<b>Modification History:\n</b>
 *		Date				Name			Comment \n\n
 *		2018/04/09               	xxx			initial verison
 *
 * @endif
 */

#ifndef __CMSIS_H__
#define __CMSIS_H__

#ifndef __ASSEMBLY__
/* --------  Configuration of the Cortex-M33 Processor and Core Peripherals  ------ */
#define __CM33_REV                0x0000U   /* Core revision r0p1 */
#define __MPU_PRESENT             1U        /* MPU present */
#define __SAUREGION_PRESENT       1U        /* SAU regions present */
#define __VTOR_PRESENT            1U        /* VTOR present */
#define __NVIC_PRIO_BITS          3U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             1U        /* no FPU present */
#define __DSP_PRESENT             1U        /* no DSP extension present */

#include "cmsis_platform_irq.h"
#include "cmsis/Include/core_cm33.h"

#endif /* __ASSEMBLY__ */
#endif /* __CMSIS_H__ */
