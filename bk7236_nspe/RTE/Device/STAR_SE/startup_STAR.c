/******************************************************************************
 * @file     startup_STAR.c
 * @brief    CMSIS Core Device Startup File for ArmChina-STAR Device
 * @version  V1.0.0
 * @date     24. April 2020
 ******************************************************************************/
/*
 * Copyright (c) 2009-2019 Arm Limited.
 * Copyright (c) 2019-2020 ArmChina. 
 * All rights reserved.
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

#if defined (STAR_CU)
  #include "STAR_CU.h"
#elif defined (STAR_SP)
  #include "STAR_SP.h"
#elif defined (STAR_SE)
  #include "STAR_SE.h"
#else
  #error device not specified!
#endif
#include "bk_uart.h"

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void( *pFunc )( void );

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern __NO_RETURN void __PROGRAM_START(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void __NO_RETURN Default_Handler(void);
void __NO_RETURN Reset_Handler  (void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
extern void NMI_Handler            (void);
extern void HardFault_Handler      (void);
extern void MemManage_Handler      (void);
extern void BusFault_Handler       (void);
extern void UsageFault_Handler     (void);
extern void SecureFault_Handler    (void);

void SVC_Handler            (void) __attribute__ ((weak, alias("Default_Handler_6")));
void DebugMon_Handler       (void) __attribute__ ((weak, alias("Default_Handler_7")));
void PendSV_Handler         (void) __attribute__ ((weak, alias("Default_Handler_8")));
void SysTick_Handler        (void) __attribute__ ((weak, alias("Default_Handler_9")));
void Interrupt0_Handler     (void) __attribute__ ((weak, alias("Default_Handler_10")));
void Interrupt1_Handler     (void) __attribute__ ((weak, alias("Default_Handler_11")));
void Interrupt2_Handler     (void) __attribute__ ((weak, alias("Default_Handler_12")));
void Interrupt3_Handler     (void) __attribute__ ((weak, alias("Default_Handler_13")));
void Interrupt4_Handler     (void) __attribute__ ((weak, alias("Default_Handler_14")));
void Interrupt5_Handler     (void) __attribute__ ((weak, alias("Default_Handler_15")));
void Interrupt6_Handler     (void) __attribute__ ((weak, alias("Default_Handler_16")));
void Interrupt7_Handler     (void) __attribute__ ((weak, alias("Default_Handler_17")));
void Interrupt8_Handler     (void) __attribute__ ((weak, alias("Default_Handler_18")));
void Interrupt9_Handler     (void) __attribute__ ((weak, alias("Default_Handler_19")));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const pFunc __VECTOR_TABLE[496];
       const pFunc __VECTOR_TABLE[496] __VECTOR_TABLE_ATTRIBUTE = {
  (pFunc)(&__INITIAL_SP),                   /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  MemManage_Handler,                        /* -12 MPU Fault Handler */
  BusFault_Handler,                         /* -11 Bus Fault Handler */
  UsageFault_Handler,                       /* -10 Usage Fault Handler */
  SecureFault_Handler,                      /*  -9 Secure Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  SVC_Handler,                              /*  -5 SVCall Handler */
  DebugMon_Handler,                         /*  -4 Debug Monitor Handler */
  0,                                        /*     Reserved */
  PendSV_Handler,                           /*  -2 PendSV Handler */
  SysTick_Handler,                          /*  -1 SysTick Handler */

  /* Interrupts */
  Interrupt0_Handler,                       /*   0 Interrupt 0 */
  Interrupt1_Handler,                       /*   1 Interrupt 1 */
  Interrupt2_Handler,                       /*   2 Interrupt 2 */
  Interrupt3_Handler,                       /*   3 Interrupt 3 */
  Interrupt4_Handler,                       /*   4 Interrupt 4 */
  Interrupt5_Handler,                       /*   5 Interrupt 5 */
  Interrupt6_Handler,                       /*   6 Interrupt 6 */
  Interrupt7_Handler,                       /*   7 Interrupt 7 */
  Interrupt8_Handler,                       /*   8 Interrupt 8 */
  Interrupt9_Handler                        /*   9 Interrupt 9 */
                                            /* Interrupts 10 .. 480 are left out */
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));

  SystemInit();                             /* CMSIS System Initialization */
  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}

void NMI_Handler(void)
{
	*((volatile UINT32 *)(0x54010014)) = (0xdead1100);
	bk_printf("NMI_Handler\r\n");
	while(0xdead1100);
}

void HardFault_Handler(void)
{
	*((volatile UINT32 *)(0x54010014)) = (0xdead1200);
	bk_printf("HardFault_Handler\r\n");
	while(0xdead1200);
}

void UsageFault_Handler(void)
{
	*((volatile UINT32 *)(0x54010014)) = (0xdead1300);
	bk_printf("UsageFault_Handler\r\n");
	while(0xdead1300);
}

void BusFault_Handler(void)
{
	*((volatile UINT32 *)(0x54010014)) = (0xdead1400);
	bk_printf("BusFault_Handler\r\n");
	while(0xdead1400);
}

void SecureFault_Handler(void)
{
	*((volatile UINT32 *)(0x54010014)) = (0xdead1500);
	bk_printf("SecureFault_Handler\r\n");
	while(0xdead1500);
}

void MemManage_Handler(void)
{
	*((volatile UINT32 *)(0x54010014)) = (0xdead1600);
	bk_printf("MemManage_Handler\r\n");
	while(0xdead1600);
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
  while(1);
}

void Default_Handler_0(void)
{
  while(1);
}
void Default_Handler_1(void)
{
  while(1);
}
void Default_Handler_2(void)
{
  while(1);
}
void Default_Handler_3(void)
{
  while(1);
}
void Default_Handler_4(void)
{
  while(1);
}
void Default_Handler_5(void)
{
  while(1);
}
void Default_Handler_6(void)
{
  while(1);
}
void Default_Handler_7(void)
{
  while(1);
}
void Default_Handler_8(void)
{
  while(1);
}
void Default_Handler_9(void)
{
  while(1);
}
void Default_Handler_10(void)
{
  while(1);
}
void Default_Handler_11(void)
{
  while(1);
}
void Default_Handler_12(void)
{
  while(1);
}
void Default_Handler_13(void)
{
  while(1);
}
void Default_Handler_14(void)
{
  while(1);
}
void Default_Handler_15(void)
{
  while(1);
}
void Default_Handler_16(void)
{
  while(1);
}
void Default_Handler_17(void)
{
  while(1);
}
void Default_Handler_18(void)
{
	while(1);
}
void Default_Handler_19(void)
{
	while(1);
}
// eof

