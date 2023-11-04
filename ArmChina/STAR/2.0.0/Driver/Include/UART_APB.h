/**************************************************************************//**
 * @file     UART_APB.h
 * @brief    CMSIS ArmChina STAR APB UART Header File
 * @version  V1.1.0
 *           V1.0.0: The initial version to support STAR work with CMSDK UART on MPS2+.
 *           V1.1.0: Add support for MPS3.
 * @date     28. October 2020
 ******************************************************************************/
/*
 * Copyright (c) 2018-2021 ArmChina. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _UART_APB_H_
#define _UART_APB_H_

#ifdef _BOARD_MPS3
    #define _BOARD_CLK  32000000         /* MPS3 CPU Clock @Hz */
	#define _UART0_BAUD 115200           /* MPS3 FPGA_UART_0 BaudRate */
	#define _FPGA_UART0_BASE (0x41303000ul)   /* MPS3 FPGA_UART_0 Base Address */
#else
	#define _BOARD_CLK 20000000          /* MPS2 CPU Clock @Hz */
	#define _UART0_BAUD 38400            /* MPS2 UART0 BaudRate */
	#define _FPGA_UART0_BASE (0x40004000ul)   /* MPS2 UART0 Base Address */
#endif	

#define SYS_CLK _BOARD_CLK
#define BAUD_RATE _UART0_BAUD

#define BAUD_DIV (SYS_CLK / BAUD_RATE)

#define UART_BASE _FPGA_UART0_BASE

#define APB_UART_DATA   (UART_BASE + 0x0ul)
#define APB_UART_STATUS (UART_BASE + 0x4ul)
#define APB_UART_CTRL   (UART_BASE + 0x8ul)
#define APB_UART_BAUDIV (UART_BASE + 0x10ul)

#define APB_UART_CTRL_TXEN (1ul << 0)
#define APB_UART_CTRL_RXEN (1ul << 1)
#define APB_UART_STATUS_TXFULL (1ul << 0)
#define APB_UART_STATUS_RXEMPTY (1ul << 1)


#define WR_REG32(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define RD_REG32(addr) (*(volatile unsigned int *)(addr))


/* Functions for std inout mapped retargetted on UART */
/* The functions are implemented in UART_APB.c */
extern void APB_UART_Initialize(void);
extern unsigned char APB_UART_PutChar(unsigned char out_char);
extern unsigned char APB_UART_GetChar(void);

extern int stdout_putchar (int ch);


#endif
