#include "base.h"
#include "bk_uart.h"
#include "nsc_caller.h"

#if CONFIG_ENABLE_VERIFY_FLASH_DBUS
#define REG_READ(addr) (*((volatile uint32_t*)(addr)))
#define REG_WRITE(addr, v) (*((volatile uint32_t*)(addr))) = (v)

#define FLASH_0xd 0x54030034
#define FLASH_0xe 0x54030038

void v_flash_dbus(void)
{
        bk_printf("Read Flash reg 0x6/0xd in NS:\r\n");
        bk_printf("flash reg xd/%x: %x\r\n", FLASH_0xd, REG_READ(FLASH_0xd));
        bk_printf("flash reg xe/%x: %x\r\n", FLASH_0xe, REG_READ(FLASH_0xe));
        bk_printf("Write Flash reg 0x6/0xd in NS:\r\n");
        REG_WRITE(FLASH_0xd, 0x1300000);
        REG_WRITE(FLASH_0xe, 0x310000);
        bk_printf("After ns write, read Flash reg 0x6/0xd in NS:\r\n");
        bk_printf("flash reg xd/%x: %x\r\n", FLASH_0xd, REG_READ(FLASH_0xd));
        bk_printf("flash reg xe/%x: %x\r\n", FLASH_0xe, REG_READ(FLASH_0xe));
        bk_printf("Goto S to read flash reg:\r\n");
	NSC_v_flash_dbus();
}
#endif

uint32_t nsc_proof_main(void)
{
	int i;

	for(i = 0; i < 20; i ++)
	{
		bk_printf("[%d]:\r\n", i);
		NSCFunction();
	}

#if CONFIG_ENABLE_VERIFY_FLASH_DBUS
	v_flash_dbus();
#endif
	return 0;
}

// eof

