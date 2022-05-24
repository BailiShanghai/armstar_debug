#ifndef _VERIFICATION_TE200_ACCESS_SYSTEM_RAM_H_
#define _VERIFICATION_TE200_ACCESS_SYSTEM_RAM_H_

#define spe_ram0_start_addr   (0x08000000)
#define spe_ram1_start_addr   (0x08010000)
#define spe_ram2_start_addr   (0x08020000)
#define spe_ram3_start_addr   (0x08040000)
#define spe_ram4_start_addr   (0x08060000)

extern int32_t te200_access_ram_verification_main(void);

#endif // _VERIFICATION_TE200_ACCESS_SYSTEM_RAM_H_
// eof
