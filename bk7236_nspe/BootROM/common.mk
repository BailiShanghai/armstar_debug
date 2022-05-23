COMMON_INCS	:=	\
			-I./device	\
			-I./drvs	\
			-I./drvs/inc	\
			-I./arch/arm-m	\
			-I./inc	\
			-I./inc/hal	\
			-I./inc/pal	\
			-I./hal/inc	\
			-I./hal	\
			-I./utils	\
			-I./utils/prng	\
			-I./utils/stdout	\
			-I./utils/string	\
			-I./utils/inc	\
			-I./utils/mutex	\
			-I./utils/oal	\
			-I./apps/inc	\

ifeq ($(MBEDTLS_WITH_DUBHE),Y)
COMMON_INCS +=  \
            -I./utils/ce_lite/include \
            -I./utils/ce_lite/library/ce_lite/inc
else
COMMON_INCS +=  \
            -I./utils/mbedtls/mbedtls/include
endif


COMMON_SRCS	:=	\
			$(call swildcard,bs,.c .S)	\
			$(call swildcard,utils,.c .S)	\
			$(call swildcard,utils/heap,.c .S)	\
			$(call swildcard,utils/stdout,.c .S)	\
			$(call swildcard,utils/string,.c .S)	\
			$(call swildcard,utils/oal,.c .S)	\
			$(call swildcard,hal,.c .S)	\
			$(call swildcard,hal/crypto,.c .S)	\
			$(call swildcard,pal,.c .S)	\
			device/device.c	\
			drvs/arm_uart/uart_cmsdk_apb.c	\
			drvs/arm_uart/uart_drv_to_dev.c	\
			hal/$(PLAT)/devices/uart/uart_cmsdk_device.c	\
			$(call swildcard,drvs/spi/spi_pl022,.c .S)	\
			drvs/spi/spi_drv_to_dev.c	\
			hal/$(PLAT)/devices/spi/spi_device.c	\
			$(call swildcard,drvs/flash/spi_flash_w25qxxdv,.c .S)	\
			drvs/flash/flash_drv_to_dev.c	\
			hal/$(PLAT)/devices/flash/flash_device.c	\
			./utils/memmgmt/heap_4.c	\
			apps/app.c	\

ifeq ($(MBEDTLS_WITH_DUBHE),Y)
    #for bootrom,bootloader and tee we need to
    # enable DUBHE_SECURE flag
    COMMON_DBH_CFLAGS       += -DARM_CE_DUBHE
    COMMON_DBH_CFLAGS       += -DARM_CE_DUBHE_OTP
    # if OTP PUF enabled, need to open this flag
ifeq ($(PUF),Y)
    COMMON_DBH_CFLAGS       += -DDBH_OTP_PUF_SUPPORT
    COMMON_DBH_CFLAGS       += -DCE_LITE_OTP_PUF
endif
    COMMON_DBH_ASFLAGS      += -DARM_CE_DUBHE
    COMMON_DBH_ASFLAGS      += -DARM_CE_DUBHE_OTP
ifeq ($(PUF),Y)
     COMMON_DBH_ASFLAGS     += -DDBH_OTP_PUF_SUPPORT
     COMMON_DBH_ASFLAGS     += -DCE_LITE_OTP_PUF
endif
endif

ifeq ($(MBEDTLS_WITH_DUBHE),Y)
COMMON_SRCS += \
      $(call swildcard,./utils/ce_lite/library/,.c .S) \
      $(call swildcard,./utils/ce_lite/library/ce_lite/src,.c .S)
else
COMMON_SRCS += \
      $(call swildcard,./utils/mbedtls/mbedtls/library,.c .S)   \
      $(call swildcard,./utils/mbedtls/mbedtls/library/original_crypto_lib,.c .S)
endif

ifeq ($(CONFIG_MEM_LEAK),Y)
COMMON_SRCS	+=	\
			$(call swildcard, ./utils/mem_leak/,.c)
endif
