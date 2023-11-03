/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#include <arm_cmse.h>
#include "nsc_functions.h"
#include "secure_port_macros.h"

/**
 * @brief Counter returned from NSCFunction.
 */
static uint32_t ulSecureCounter = 0;

/**
 * @brief typedef for non-secure callback.
 */
typedef void ( *NonSecureCallback_t )( void ) __attribute__( ( cmse_nonsecure_call ) );
/*-----------------------------------------------------------*/

secureportNON_SECURE_CALLABLE uint32_t NSCFunction( Callback_t pxCallback )
{
    /* Increment the secure side counter. */
    ulSecureCounter += 1;
	
	bk_printf("ulSecureCounter:0x%x\r\n", ulSecureCounter);
	
    /* Return the secure side counter. */
    return ulSecureCounter;
}

#if CONFIG_ENABLE_VERIFY_FLASH_DBUS
#define REG_READ(addr) (*((volatile uint32_t*)(addr)))
#define REG_WRITE(addr, v) (*((volatile uint32_t*)(addr))) = (v)

#define PPRO_0x6  0x41040024
#define FLASH_0xd 0x44030034
#define FLASH_0xe 0x44030038

secureportNON_SECURE_CALLABLE uint32_t NSC_v_flash_dbus( Callback_t pxCallback )
{
        *((volatile uint32_t *)(0x41040000 + 6 * 4)) = 0x080F7F2F;
        *((volatile uint32_t *)(0x41040000 + 8 * 4)) = 0x10003FFB;

	bk_printf("Back to secure world\r\n");
        bk_printf("flash reg xd/%x: %x\r\n", FLASH_0xd, REG_READ(FLASH_0xd));
        bk_printf("flash reg xe/%x: %x\r\n", FLASH_0xe, REG_READ(FLASH_0xe));
        *((volatile uint32_t *)(0x41040000 + 6 * 4)) = 0x080FFF6F;
        *((volatile uint32_t *)(0x41040000 + 8 * 4)) = 0x10003FFF;
	return ulSecureCounter;
}
#endif


/*-----------------------------------------------------------*/
