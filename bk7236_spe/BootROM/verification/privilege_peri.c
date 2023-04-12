#include "base.h"
#include "cmsis_armclang.h"
#include "bk_uart.h"
#include "privilege_peri.h"

uint32_t privilege_verification_main(void)
{
    bk_printf("privilege_verification_main enter\r\n");

    bk_printf("0x4104001c reg:0x%x\r\n", *((volatile uint32_t *)(0x41040000 + 7 * 4)));
    bk_printf("control reg:0x%x\r\n", __get_CONTROL());

    /* configure privilege register in the prro, and uart */
    *((volatile uint32_t *)(0x41040000 + 7 * 4)) = ((*((volatile uint32_t *)(0x41040000 + 7 * 4))) | 0x04);

    bk_printf("enter unprivilege mode\r\n");
    /*enter unprivilege level*/
    __set_CONTROL(0x01);

    bk_printf("privilege_verification_main over\r\n");

    return 0;
}
// eof

