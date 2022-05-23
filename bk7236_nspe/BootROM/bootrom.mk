
#
# BOOTROM Version
#
VERSION_MAJOR				:=	1
VERSION_MINOR				:=	0
CONFIG_MSP_STACK_SIZE		:=	10240

GLOBAL_INCLUDES	:=	\
			$(COMMON_INCS)	\
			$(PLAT_INCS)	\
			-Iboard/$(PLAT)/layout/bootrom	\
			-Iinc/secure_boot	\

GLOBAL_INCLUDES	+=	\
			$(SEC_BOOT_INCS)	\

BOOTROM_SOURCES	:=	\
			$(COMMON_SRCS)	\
			$(ARCH_SRCS)	\
			$(PLATFROM_SRCS)	\
			examples/bootrom/main.c	\
			examples/bootrom/secure_boot_sample.c	\

BOOTROM_SOURCES	+=	\
			$(SEC_BOOT_SRCS)	\

BINARY		:=	bootrom
TARGET_OUT	:=	bootrom
SRCS		:=	$(sort $(BOOTROM_SOURCES))
LDSCRIPT	:=	$(LINKSCRIPT)

VERSION_STRING		:=	BOOTROM_v${VERSION_MAJOR}.${VERSION_MINOR}
CFLAGS_$(BINARY)	:=	$(PLAT_CPU_ARCH) $(GLOBAL_INCLUDES) -DVERSION_STRING=\"$(VERSION_STRING)\" -DBUILD_TYPE=\"$(BUILD_TYPE)\" -DMBEDTLS_CONFIG_FILE=\"mbedtls_config.h\" $(SEC_BOOT_CFLAGS) $(COMMON_DBH_CFLAGS)
ASFLAGS_$(BINARY)	:=	$(PLAT_CPU_ARCH) $(GLOBAL_INCLUDES) -DCONFIG_MSP_STACK_SIZE=$(CONFIG_MSP_STACK_SIZE) $(SEC_BOOT_ASFLAGS) $(COMMON_DBH_ASFLAGS)
LDFLAGS_$(BINARY)	:=	-static	--print-memory-usage $(SEC_BOOT_LDFLAGS)
ifeq ($(CONFIG_MEM_LEAK),Y)
DEPLIB 				:= $(LIBGCC)
else
DEPLIB 				:=
endif

include ${mk-dir}/link.mk


