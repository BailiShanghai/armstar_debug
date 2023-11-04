/**************************************************************************//**
 * @file     main.c
 * @brief    main file of the coremark example. 
 *           invoke core_main to run the benchmark testing.
 *           use system tick to count the cycles.  
 * @version  V1.0.0
 * @date     29. June 2021
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

#include <stdio.h>
#include "lowlevel_retarget.h"
#include "UART_APB.h"
#include "coremark.h"

#if defined (STAR_CU)
  #include "STAR_CU.h"
#elif defined (STAR_SP)
  #include "STAR_SP.h"
#elif defined (STAR_SE)
  #include "STAR_SE.h"
#endif	

/***********************************Macro Definition********************************************/
#define CLKB_MSK                            2UL
#define INTB_MSK                            1UL
#define ENAB_MSK                            0UL
#define NVIC_SYSTICK_CTRL                   ((volatile unsigned int *) 0xE000E010)
#define NVIC_SYSTICK_LOAD                   ((volatile unsigned int *) 0xE000E014)
#define NVIC_SYSTICK_CURRENT_VALUE          ((volatile unsigned int *) 0xE000E018)
#define NVIC_SYSTICK_CLK                    (1UL << CLKB_MSK)
#define NVIC_SYSTICK_INT                    (1UL << INTB_MSK)
#define NVIC_SYSTICK_ENABLE                 (1UL << ENAB_MSK)

#define TICK_FREQ                            1000      /*TickFreq	is 1000 times per second*/
#define SYSTEM_CLOCK                         20000000  /*SYSTEM_CLOCK must be replaced with the actual CPU clock*/


/***********************************Function Definition*******************************************/

/*Setup SystemTick with defined SystemFreq and TickFreq
** Note that: SYSTEM_CLOCK must be the actual CPU clcok frequency on the platform you are using**
*/
void SetUp_SysTick(void) 
{
	  /*Stop and reset the SysTick*/
	  *( NVIC_SYSTICK_CTRL ) = 0UL;
	  *( NVIC_SYSTICK_CURRENT_VALUE ) = 0UL;
	
	  /* Configure SysTick to interrupt at the requested rate. */
	  *( NVIC_SYSTICK_LOAD ) = ( SYSTEM_CLOCK / TICK_FREQ ) - 1UL;
	  *( NVIC_SYSTICK_CTRL ) = NVIC_SYSTICK_CLK | NVIC_SYSTICK_INT | NVIC_SYSTICK_ENABLE;
}

/*definition of SysTickCounter for Time calculation*/
volatile ee_u32 SysTickCounter = 0;

/*SysTick_Handler used for time calculation
*/
void SysTick_Handler(void) 
{
    SysTickCounter ++;
}

/* main function: to setup Systick and run CoreMark tests
** Round 1: Caches enabled by default **
** Round 2: Caches disabled in the main() **
*/
MAIN_RETURN_TYPE coremark_main(int argc, char *argv[]);

int main()
{	
	int argc=0;
	char *argv[1];

	UartStdOutInit();
	SetUp_SysTick();

    printf("\n---- Round 1 : Cache enabled ----\n");
	printf("STAR: CoreMark is running! ...\n");
	coremark_main(argc, argv);
	printf("\nRound 1 Completed.\n");

#if 1
	__NOP();
	SCB_DisableICache();
	__NOP();
	SCB_DisableDCache();
  __NOP();
	printf("\n---- Round 2 : Cache disabled ----\n");
	printf("STAR: CoreMark is running! ...\n");
	coremark_main(argc, argv);
	printf("\nRound 2 Completed.\n");
#endif	

	for(;;)
	{
		;
	}
	
}	
