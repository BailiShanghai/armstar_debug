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

#ifndef __FREQ_DEF_H__
#define __FREQ_DEF_H__

/*
  * MPS2 AN521 has different frequencies for system core clock (20MHz) and
  * peripherals clock (25MHz).
  */
 #define  XTAL             (40000000UL)
 #define  PERIPHERAL_XTAL  (50000000UL)

 #define  SYSTEM_CLOCK     (XTAL/2)
 #define  PERIPHERAL_CLOCK (PERIPHERAL_XTAL/2)

 #define  UART_CLOCK    (20000000UL)

#endif /* __FREQ_DEF_H__ */
