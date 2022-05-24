#include "base.h"
#include "sig_hash256.h"
#include "bk_uart.h"

static const unsigned char hash_msg_sample[] = {
    0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x31, 0x32, 0x33, 0x0a
};

static const unsigned char sha256_hash_ref[] = {
	0xf0, 0x10, 0x9d, 0xb9, 0xf3, 0x61, 0x9a, 0xa5, 0xfb, 0x0f, 0x53, 0x3e, 0x1c, 0xa1, 0x45, 0x61,
	0x47, 0xb0, 0x1b, 0x43, 0xa4, 0xb8, 0x07, 0x64, 0x46, 0x14, 0x1e, 0x2a, 0x85, 0x95, 0xb1, 0xc9
};

extern int32_t sha256_verify(const uint8_t *message,
								 size_t message_size,
								 const uint8_t *ref_hash,
								 uint8_t is_224);

void hash256_verificaton_main(void)
{
    int32_t ret;
    bk_printf("SHA256 example...\r\n");
    ret = sha256_verify(hash_msg_sample,
                        sizeof(hash_msg_sample),
                        sha256_hash_ref,
                        0);

	(void)ret;
	
    return;
	
}
// eof

