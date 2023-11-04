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

#include "base.h"
#include "hal_platform.h"
#include "app.h"
#include "bk_uart.h"
#include "verification_config.h"
#include "reg_base.h"
#include "soc.h"
#include "STAR_SE.h"

#define DBG_PRE_PROMPT "BS"
#include "dbg.h"

extern int32_t sys_device_do_config_level(int32_t level);
extern void verification_main(void);

const char build_message[] =
    "Build Time: " __DATE__ " " __TIME__ " " BUILD_TYPE;

#if CONFIG_ENABLE_PLL
#define addSYSTEM_Reg0x40                 *((volatile unsigned long *) (0x44010000+0x40*4))
#define addSYSTEM_Reg0x45                 *((volatile unsigned long *) (0x44010000+0x45*4))
#define addSYSTEM_Reg0x46                 *((volatile unsigned long *) (0x44010000+0x46*4))
#define addSYSTEM_Reg0x8                  *((volatile unsigned long *) (0x44010000+0x8*4))
#define addSYSTEM_Reg0x9                  *((volatile unsigned long *) (0x44010000+0x9*4))

void hal_delay(int count)
{
    volatile int i = count;
    while (i > 0)
    {
        i --;
    }
}

/* From Wangjian, method to enable 7236 DPLL:
  * ana_reg5 set bit5
  * ana_reg0 set to 0x81185B57
  * ana_reg0 clear bit19
  * ana_reg0 set bit19
  */
void hal_enable_pll_120mhz(void)
{
    addSYSTEM_Reg0x45 = addSYSTEM_Reg0x45 | 0x20;
    hal_delay(2000);

    addSYSTEM_Reg0x40 = 0x81185B57;

    hal_delay(2000);

    addSYSTEM_Reg0x40 &= 0xfff7ffff;
    addSYSTEM_Reg0x40 |= 0x00080000;

    addSYSTEM_Reg0x8 |= 0x033; 	// sel 480M PLL ; div4
}

void hal_enable_pll_240mhz(void)
{
    addSYSTEM_Reg0x45 = addSYSTEM_Reg0x45 | 0x20;
    hal_delay(2000);

    addSYSTEM_Reg0x40 = 0x81185B57;

    hal_delay(2000);

    addSYSTEM_Reg0x40 &= 0xfff7ffff;
    addSYSTEM_Reg0x40 |= 0x00080000;

    addSYSTEM_Reg0x8 |= 0x031; 	// sel 480M PLL ; div2
}

void hal_printf_pll_registers(void)
{
    bk_printf("addSYSTEM_Reg0x45:0x%x\r\n", addSYSTEM_Reg0x45);
    bk_printf("addSYSTEM_Reg0x40:0x%x\r\n", addSYSTEM_Reg0x40);
    bk_printf("addSYSTEM_Reg0x8:0x%x\r\n", addSYSTEM_Reg0x8);
}
#endif /* CONFIG_ENABLE_PLL*/

void close_wdt(void)
{
    REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5A0000);
    REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA50000);
    REG_SET(SOC_WDT_REG_BASE + 4 * 2, 1, 1, 1);
    REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0x5A0000);
    REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0xA50000);
}

#if 1
#define portSTACK_TYPE	  uint32_t
typedef portSTACK_TYPE StackType_t;
#define portFIRST_CONFIGURABLE_REGION				  ( 5UL )
#define portLAST_CONFIGURABLE_REGION				  ( 7UL )
#define portNUM_CONFIGURABLE_REGIONS				  ( ( portLAST_CONFIGURABLE_REGION - portFIRST_CONFIGURABLE_REGION ) + 1 )
#define portTOTAL_NUM_REGIONS						  ( portNUM_CONFIGURABLE_REGIONS + 1 )	 /* Plus one to make space for the stack region. */

typedef struct MPURegionSettings
{
    uint32_t ulRBAR; /**< RBAR for the region. */
    uint32_t ulRLAR; /**< RLAR for the region. */
} MPURegionSettings_t;

typedef struct MPU_SETTINGS
{
    uint32_t ulMAIR0;											   /**< MAIR0 for the task containing attributes for all the 4 per task regions. */
    MPURegionSettings_t xRegionsSettings[ portTOTAL_NUM_REGIONS ]; /**< Settings for 4 per task regions. */
} xMPU_SETTINGS;

typedef struct _test_control_block_       /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    volatile StackType_t *pxTopOfStack;  /*< Points to the location of the last item placed on the tasks stack.  THIS MUST BE THE FIRST MEMBER OF THE TCB STRUCT. */

    xMPU_SETTINGS xMPUSettings; /*< The MPU settings are defined as part of the port layer.  THIS MUST BE THE SECOND MEMBER OF THE TCB STRUCT. */
} TEST_TCB;



TEST_TCB g_test_tcb =
{
    NULL,
    {
        0,
        {
            {0x28001200, (0x28001200 + 64) | 1},
            {0x28001300, (0x28001300 + 64) | 1},
            {0x28001400, (0x28001400 + 64) | 1}
        }
    }
};
TEST_TCB *pxTestTcb;


const ARM_MPU_Region_t t_mpu_table[1][7] =
{
    {
        //                     BASE          SH              RO   NP   XN                         LIMIT         ATTR
        { .RBAR = ARM_MPU_RBAR(0x00000000UL, ARM_MPU_SH_NON, 0UL, 0UL, 0UL), .RLAR = ARM_MPU_RLAR(0x1FFFFFFFUL, 0UL) },
        { .RBAR = ARM_MPU_RBAR(0x20000000UL, ARM_MPU_SH_NON, 0UL, 0UL, 0UL), .RLAR = ARM_MPU_RLAR(0x2803FFFFUL, 0UL) },
        { .RBAR = ARM_MPU_RBAR(0x28040000UL, ARM_MPU_SH_NON, 0UL, 0UL, 0UL), .RLAR = ARM_MPU_RLAR(0x2805FFFFUL, 0UL) },
        { .RBAR = ARM_MPU_RBAR(0x28060000UL, ARM_MPU_SH_NON, 0UL, 0UL, 0UL), .RLAR = ARM_MPU_RLAR(0x2807FFFFUL, 1UL) }, /* sram 4*/
        { .RBAR = ARM_MPU_RBAR(0x28080000UL, ARM_MPU_SH_NON, 0UL, 0UL, 0UL), .RLAR = ARM_MPU_RLAR(0x3FFFFFFFUL, 0UL) },
        { .RBAR = ARM_MPU_RBAR(0x40000000UL, ARM_MPU_SH_NON, 0UL, 0UL, 0UL), .RLAR = ARM_MPU_RLAR(0x6bFFFFFFUL, 2UL) },
        { .RBAR = ARM_MPU_RBAR(0x70000000UL, ARM_MPU_SH_NON, 0UL, 0UL, 1UL), .RLAR = ARM_MPU_RLAR(0xEFFFFFFFUL, 2UL) },
    }
};

void test_cat_mpu_config(void)
{
    pxTestTcb = &g_test_tcb;

    __asm volatile
    (
        "	.syntax unified 								\n"
        "													\n"
        "	ldr  r2, pxCurrentTCBConst2 					\n"/* Read the location of pxCurrentTCB i.e. &( pxCurrentTCB ). */
        "	ldr  r1, [r2]									\n"/* Read pxCurrentTCB. */
        "	ldr  r0, [r1]									\n"/* Read top of stack from TCB - The first item in pxCurrentTCB is the task top of stack. */
        "													\n"
        "	dmb 											\n"/* Complete outstanding transfers before disabling MPU. */
        "	ldr r2, xMPUCTRLConst2							\n"/* r2 = 0xe000ed94 [Location of MPU_CTRL]. */
        "	ldr r4, [r2]									\n"/* Read the value of MPU_CTRL. */
        "	bic r4, #1										\n"/* r4 = r4 & ~1 i.e. Clear the bit 0 in r4. */
        "	str r4, [r2]									\n"/* Disable MPU. */
        "													\n"
        "	adds r1, #4 									\n"/* r1 = r1 + 4. r1 now points to MAIR0 in TCB. */
        "	ldr  r3, [r1]									\n"/* r3 = *r1 i.e. r3 = MAIR0. */
        "	ldr  r2, xMAIR0Const2							\n"/* r2 = 0xe000edc0 [Location of MAIR0]. */
        //"	str  r3, [r2]									\n"/* Program MAIR0. */
        "	ldr  r2, xRNRConst2 							\n"/* r2 = 0xe000ed98 [Location of RNR]. */
        "	movs r3, #4 									\n"/* r3 = 4. */
        "	str  r3, [r2]									\n"/* Program RNR = 4. */
        "	adds r1, #4 									\n"/* r1 = r1 + 4. r1 now points to first RBAR in TCB. */
        "	ldr  r2, xRBARConst2							\n"/* r2 = 0xe000ed9c [Location of RBAR]. */
        "	ldmia r1!, {r4-r11} 							\n"/* Read 4 set of RBAR/RLAR registers from TCB. */
        "	stmia r2!, {r4-r11} 							\n"/* Write 4 set of RBAR/RLAR registers using alias registers. */
        "													\n"
        "	ldr r2, xMPUCTRLConst2							\n"/* r2 = 0xe000ed94 [Location of MPU_CTRL]. */
        "	ldr r4, [r2]									\n"/* Read the value of MPU_CTRL. */
        "	orr r4, #1										\n"/* r4 = r4 | 1 i.e. Set the bit 0 in r4. */
        "	str r4, [r2]									\n"/* Enable MPU. */
        "	dsb 											\n"/* Force memory writes before continuing. */
        "													\n"

        "													\n"
        "	.align 4										\n"
        "pxCurrentTCBConst2: .word pxTestTcb    			\n"
        "xMPUCTRLConst2: .word 0xe000ed94					\n"
        "xMAIR0Const2: .word 0xe000edc0 					\n"
        "xRNRConst2: .word 0xe000ed98						\n"
        "xRBARConst2: .word 0xe000ed9c						\n"
    );
}


uint32_t test_mpu_config(void)
{
    bk_printf("dc_mpu_config 20220625 0924\r\n");;
    bk_printf("address 0x28040000 wt cache policy\r\n");
    bk_printf("address 0x28060000 normal memory without cacheable\r\n");

    __DMB();
    ARM_MPU_Disable();
    do
    {
        MPU->RNR = 0;
        MPU->RBAR = 0;
        MPU->RLAR = 0;
        MPU->RBAR_A1 = 0;
        MPU->RLAR_A1 = 0;
        MPU->RBAR_A2 = 0;
        MPU->RLAR_A2 = 0;
        MPU->RBAR_A3 = 0;
        MPU->RLAR_A3 = 0;
        MPU->MAIR0 = 0x44444444;
        MPU->MAIR1 = 0x44444444;
    }
    while(0);

    ARM_MPU_SetMemAttr(0UL, ARM_MPU_ATTR(       /* Normal memory */
                           ARM_MPU_ATTR_MEMORY_(0UL, 0UL, 1UL, 1UL), /* Outer Write-Back transient with read and write allocate */
                           ARM_MPU_ATTR_MEMORY_(0UL, 0UL, 1UL, 1UL)  /* Inner Write-Through transient with read and write allocate */
                       ));
    ARM_MPU_SetMemAttr(1UL, ARM_MPU_ATTR(       /* Normal memory */
                           ARM_MPU_ATTR_MEMORY_(0UL, 1UL, 0UL, 0UL), /* Outer Write-Back transient with read and write allocate */
                           ARM_MPU_ATTR_MEMORY_(0UL, 1UL, 0UL, 0UL)  /* Inner Write-Through transient with read and write allocate */
                       ));
    ARM_MPU_SetMemAttr(2UL, ARM_MPU_ATTR(       /* Device memory */
                           ARM_MPU_ATTR_MEMORY_(0UL, 0UL, 0UL, 0UL), /* Outer Write-Back transient with read and write allocate */
                           ARM_MPU_ATTR_MEMORY_(0UL, 0UL, 0UL, 0UL)  /* Inner Write-Through transient with read and write allocate */
                       ));

    ARM_MPU_Load(0, t_mpu_table[0], 4);

    bk_printf("revoke the function-ARM_MPU_NS_Enable\r\n");
    ARM_MPU_Enable_NS(4);

    bk_printf("revoke the function-ARM_MPU_Enable\r\n");

    ARM_MPU_Enable(4);

    bk_printf("SCB_EnableDCache\r\n");
    if (SCB->CLIDR & SCB_CLIDR_IC_Msk)
        SCB_EnableDCache();

    MPU->RNR = 0;
    bk_printf("MPU->TYPE:0x%x\r\n", MPU->TYPE);
    bk_printf("MPU->CTRL:0x%x\r\n", MPU->CTRL);
    bk_printf("MPU->MAIR0:0x%x\r\n", MPU->MAIR0);
    bk_printf("MPU->MAIR1:0x%x\r\n", MPU->MAIR1);
    bk_printf("MPU->RBAR:0x%x\r\n", MPU->RBAR);
    bk_printf("MPU->RLAR:0x%x\r\n", MPU->RLAR);
    bk_printf("MPU->RBAR1:0x%x\r\n", MPU->RBAR_A1);
    bk_printf("MPU->RLAR1:0x%x\r\n", MPU->RLAR_A1);
    bk_printf("MPU->RBAR2:0x%x\r\n", MPU->RBAR_A2);
    bk_printf("MPU->RLAR2:0x%x\r\n", MPU->RLAR_A2);
    bk_printf("MPU->RBAR3:0x%x\r\n", MPU->RBAR_A3);
    bk_printf("MPU->RLAR3:0x%x\r\n", MPU->RLAR_A3);

    bk_printf("MPU->CTRL:0x%x\r\n", MPU->CTRL);

    __DSB();
    __ISB();


    return 0;
}

void verification_mpu_main(void)
{
}
#endif

int32_t uart_init(struct device *dev);

int main(void)
{
    int32_t ret = 0;

    close_wdt();
	#if CONFIG_DIABLE_DEBUG_FEATURE
    hal_cpu_debug_disable();
	#endif

    uart_init(0);

#if CONFIG_ENABLE_PLL
    hal_enable_pll_120mhz();
    hal_printf_pll_registers();
#endif

    bk_printf("welcome to the secure world\r\n");

    ret = hal_platform_early_init();
    CHECK_RET("platform_early_init failed!\n");

#if CONFIG_MEM_LEAK
    extern int32_t mem_leak_check_init(void);
    ret = mem_leak_check_init();
    CHECK_RET("mem_leak_check_init failed!\n");
#endif

    bk_printf("DEVICE_LEVEL0\r\n");
    ret = sys_device_do_config_level(DEVICE_LEVEL0);
    CHECK_RET("device level0 init fail 0x%08x\n", ret);

    bk_printf("==================================================================="
              "=======\n");
    bk_printf("| %-70s |\n",
              "version 1.0");
    bk_printf("| %-70s |\n", __DATE__);
    bk_printf("| %-70s |\n", __TIME__);
    bk_printf("==================================================================="
              "=======\n");

#if CONFIG_ENABLE_PLATFORM_INIT
    ret = hal_platform_init();
    CHECK_RET("platform_init failed!\n");
#else
    bk_printf("hal_platform_init bypass\r\n");
#endif

    bk_printf("DEVICE_LEVEL1\r\n");
    ret = sys_device_do_config_level(DEVICE_LEVEL1);
    CHECK_RET("device level1 init fail 0x%08x\n", ret);

#if CONFIG_ENABLE_PLATFORM_INIT
    ret = hal_platform_post_init();
    CHECK_RET("hal_platform_post_init failed!\n");
#else
    bk_printf("hal_platform_post_init bypass\r\n");
#endif

    bk_printf("DEVICE_LEVEL2\r\n");
    ret = sys_device_do_config_level(DEVICE_LEVEL2);
    CHECK_RET("device level2 init fail 0x%08x\n", ret);

    bk_printf("DEVICE_LEVEL3\r\n");
    ret = sys_device_do_config_level(DEVICE_LEVEL3);
    CHECK_RET("device level2 init fail 0x%08x\n", ret);

    apps_init();

    verification_main();

finish:
    /* should not be here */
    while (1)
    {
        ;
    }

    return 0;
}
// eof

