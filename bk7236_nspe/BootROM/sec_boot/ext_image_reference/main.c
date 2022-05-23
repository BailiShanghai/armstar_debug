/**
 * @if copyright_display
 *              Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *              All rights reserved
 *
 *          The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *              to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *              License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *              restricting among other things, the use, reproduction, distribution
 *              and transfer.  Each of the embodiments, including this information and
 *              any derivative work shall retain this copyright notice.
 * @endif
 */

#include <stdint.h>
/*this register is just for reference test*/
#define CM3DS_MPS2_FPGASYS_USERLED0 0x40028000
#define CM3DS_MPS2_FPGASYS_USERLED0_VALUE 0x00000001
int sp_main(void)
{
    /*here is just the example code, we just use the USERLED0 for reference*/
    *((volatile uint32_t*)CM3DS_MPS2_FPGASYS_USERLED0) |= CM3DS_MPS2_FPGASYS_USERLED0_VALUE;/*set to 1, the userled0 will on*/
    return 0;
}
