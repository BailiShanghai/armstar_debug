#ifndef __TT_IDAU_H_
#define __TT_IDAU_H_

#define ADDR_SPACE_START          (0x02000000)
#define ADDR_SPACE_END            (0x06000000) // FIXME TODO
#define TT_INVALID_VAL            (0xFFFFFFFF)

#ifdef ARM_CMSE_CMSIS_PACK
typedef union {
  struct cmse_address_info {
    unsigned mpu_region:8;
#if __ARM_FEATURE_CMSE & 2
    unsigned sau_region:8;
#else
    unsigned :8;
#endif
    unsigned mpu_region_valid:1;
#if __ARM_FEATURE_CMSE & 2
    unsigned sau_region_valid:1;
#else
    unsigned :1;
#endif
    unsigned read_ok:1;
    unsigned readwrite_ok:1;
#if __ARM_FEATURE_CMSE & 2
    unsigned nonsecure_read_ok:1;
    unsigned nonsecure_readwrite_ok:1;
    unsigned secure:1;
    unsigned idau_region_valid:1;
    unsigned idau_region:8;
#else
    unsigned :12;
#endif
  } flags;
  unsigned value;
} cmse_address_info_t;
#endif

extern void tt_verificaton_main(void);
#endif // __TT_IDAU_H_
// eof
