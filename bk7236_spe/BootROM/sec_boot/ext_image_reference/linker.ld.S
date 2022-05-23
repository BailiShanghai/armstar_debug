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

#include "mem_layout.h"

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

MEMORY
{
  RAM (rwx)             : ORIGIN = CONFIG_RAM_ADDR + 0x100000, LENGTH = 0x1000
}


SECTIONS
{
    .__EXT_IMG_HEAD :
    {
        . = ALIGN(4);
        LONG(0x53704864);           /* 0x53704864: magic */
        LONG(0x00000001);           /* 0x00000001: version */

        LONG(__text_vma_start);
        LONG(__text_vma_end);
        LONG(__text_lma_entry);
        LONG(0x00000400);/*ext binary stack size*/
        LONG(0x00000000);/*reserve 0*/
        LONG(0x00000000);/*reserve 1*/
        . = ALIGN(4);
    } > RAM

    __text_vma_start = .;
    __text_lma_entry = .;
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.text*))
        KEEP(*(".text.*"))
        . = ALIGN(4);
    } > RAM

    __text_vma_end = .;

}   /* End of sections */
