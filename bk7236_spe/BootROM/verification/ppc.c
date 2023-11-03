#include "base.h"
#include "bk_uart.h"

void config_ppc(void)
{
        *((volatile uint32_t *)(0x45020000 + 2 * 4)) = 0;
        *((volatile uint32_t *)(0x45020000 + 2 * 4)) = 1; /* soft reset dma0 module */
        *((volatile uint32_t *)(0x45020000 + 5 * 4)) = 0xFFF;
        *((volatile uint32_t *)(0x45020000 + 4 * 4)) = 0;

        *((volatile uint32_t *)(0x45030000 + 2 * 4)) = 0;
        *((volatile uint32_t *)(0x45030000 + 2 * 4)) = 1; /* soft reset dma1 module */
        *((volatile uint32_t *)(0x45030000 + 5 * 4)) = 0xFFF;
        *((volatile uint32_t *)(0x45030000 + 4 * 4)) = 0;

        *((volatile uint32_t *)(0x41040000 + 2 * 4)) = 1; /* soft reset ppro module */


        *((volatile uint32_t *)(0x41040000 + 4 * 4)) = 0xFFFFFFFF; /* 0xFFFFFFFC gpio_nonsec0  gpio0/1:uart1*/
        *((volatile uint32_t *)(0x41040000 + 5 * 4)) = 0xFFFFFFFF; /* gpio_nonsec1 */

        *((volatile uint32_t *)(0x41040000 + 6 * 4)) = 0x080FFF6F;//0x080F7F6F;//0x080FFF6F;
        *((volatile uint32_t *)(0x41040000 + 7 * 4)) = 0x187;
        *((volatile uint32_t *)(0x41040000 + 8 * 4)) = 0x10003FFF;//0x10003FFB
        *((volatile uint32_t *)(0x41040000 + 9 * 4)) = 0x9E03F;
        *((volatile uint32_t *)(0x41040000 + 10 * 4)) = 0x21F9;
        *((volatile uint32_t *)(0x41040000 + 11 * 4)) = 0x2E00FF;
        *((volatile uint32_t *)(0x41040000 + 12 * 4)) = 0x13;
        *((volatile uint32_t *)(0x41040000 + 13 * 4)) = 0xB9;
        *((volatile uint32_t *)(0x41040000 + 14 * 4)) = 0x1;
        *((volatile uint32_t *)(0x41040000 + 15 * 4)) = 0x1FFF;
}


