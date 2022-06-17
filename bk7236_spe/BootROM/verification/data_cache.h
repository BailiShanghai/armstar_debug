#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if CONFIG_ENABLE_VERIFY_DCACHE
uint32_t data_cache_verification_main(void);
#endif

#if CONFIG_ENABLE_VERIFY_MULTI_CORE0_DCACHE
uint32_t data_cache_multi_core0_verification_main(void);
#endif

#if CONFIG_ENABLE_VERIFY_MULTI_CORE1_DCACHE
uint32_t data_cache_multi_core1_verification_main(void);
#endif

#ifdef __cplusplus
}
#endif
// eof


