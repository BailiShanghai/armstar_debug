#ifndef _BK_PSRAM_H_
#define _BK_PSRAM_H_

#define PSRAM_SUCCESS                (0)
#define PSRAM_FAILURE                (1)

#define PSRAM_MEM_BASE_S             (0x60000000)
#define PSRAM_MEM_BASE_NS            (0x70000000)

/*******************************************************************************
* Function Declarations
*******************************************************************************/
UINT32 bk_psram_read(char *user_buf, UINT32 count, UINT32 address);
UINT32 bk_psram_write(char *user_buf, UINT32 count, UINT32 address);

#endif
