#include "base.h"
#include "te_ram.h"
#include "pal_string.h"
#include "bk_uart.h"

static const unsigned char hash_msg_sample[] =
{
    0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x31, 0x32, 0x33, 0x0a
};

static const unsigned char sha256_hash_ref[] =
{
    0xf0, 0x10, 0x9d, 0xb9, 0xf3, 0x61, 0x9a, 0xa5, 0xfb, 0x0f, 0x53, 0x3e, 0x1c, 0xa1, 0x45, 0x61,
    0x47, 0xb0, 0x1b, 0x43, 0xa4, 0xb8, 0x07, 0x64, 0x46, 0x14, 0x1e, 0x2a, 0x85, 0x95, 0xb1, 0xc9
};


int32_t sha256_verify(const uint8_t *message,
                      size_t message_size,
                      const uint8_t *ref_hash,
                      uint8_t is_224);

int32_t test_sha256_ram(uint32_t ram_addr)
{
    int msg_len;
    int hash_len;
    int32_t ret = 0;
    uint8_t *ram_ptr = (uint8_t *)ram_addr;
    uint8_t *msg_ptr;
    uint8_t *hash_ptr;

    msg_len = sizeof(hash_msg_sample);
    msg_ptr = ram_ptr;
    pal_memcpy(msg_ptr, hash_msg_sample, msg_len);

    hash_len = sizeof(sha256_hash_ref);
    hash_ptr = &msg_ptr[msg_len];
    pal_memcpy(hash_ptr, sha256_hash_ref, hash_len);

    bk_printf("SHA256 example...\r\n");
    ret = sha256_verify((uint8_t *)msg_ptr,
                        msg_len,
                        (uint8_t *)hash_ptr,
                        0);
    return ret;
}

int32_t te200_access_ram_verification_main(void)
{
    test_sha256_ram(spe_ram0_start_addr);
    test_sha256_ram(spe_ram1_start_addr);
    test_sha256_ram(spe_ram2_start_addr);
    test_sha256_ram(spe_ram3_start_addr);
    test_sha256_ram(spe_ram4_start_addr);

    return 0;
}

// eof

