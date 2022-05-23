################################################################################
# Config values for build configurations
################################################################################

# Build verbosity
V						:= 0
# Debug build
DEBUG					:= 0
# Build platform
DEFAULT_PLAT			:= bk7236_v2022
PLAT					:= ${DEFAULT_PLAT}
# Testbed
TESTBED                 := 0
# Cross compile
DFT_CROSS_COMPILE		:= arm-none-eabi-
CROSS_COMPILE			:= ${TOOLCHAIN_PATH}/${DFT_CROSS_COMPILE}
# TEE SDK PATH
TEE_SDK_PATH			:= $(top-dir)/TEE_M


################################################################################
# spe config feature
################################################################################
CONFIG_SST_SUPPORT			:= Y
CONFIG_MBEDTLS_SUPPORT		:= Y
MBEDTLS_WITH_DUBHE			:= Y
CONFIG_FLASH_SP_SUPPORT		:= Y
CONFIG_CB_SUPPORT			:= Y
CONFIG_CB_SOCKET			:= N
CONFIG_STLS					:= Y

SECURE_BOOT					:= Y
SECURE_DBG					:= Y
PROVISIONING				:= Y
TRNG_CALIBRATION			:= Y

################################################################################
# PUF config
################################################################################
PUF             := N
PUF_SCRAMBLE    := N

################################################################################
# Process build options
################################################################################

# Verbose
ifneq ($V,1)
	Q := @
	CMD_ECHO := true
	CMD_ECHO_SILENT := echo
else
	Q :=
	CMD_ECHO := echo
	CMD_ECHO_SILENT := true
endif

# Debug
ifneq ($(DEBUG),0)
	BUILD_TYPE	:= debug
	CFLAGS		+= -g -DCONFIG_DEBUG=1
	ASFLAGS		+= -g -DCONFIG_DEBUG=1
else
	BUILD_TYPE	:= release
endif

# Output directory
ifeq ($O,)
	TOP_OUT	:= $(top-dir)/out/$(BUILD_TYPE)/$(PLAT)
	OUT_DIR	:= $(TOP_OUT)/$(COMPONENT_OUT)
else
	TOP_OUT	:= $(O)
	OUT_DIR	:= $(TOP_OUT)/$(COMPONENT_OUT)
endif

# Callback
ifeq ($(CONFIG_CB_SUPPORT),Y)
	CFLAGS		+= -DCONFIG_CB_SUPPORT=1
	ASFLAGS		+= -DCONFIG_CB_SUPPORT=1
else
	CFLAGS		+= -DCONFIG_CB_SUPPORT=0
	ASFLAGS		+= -DCONFIG_CB_SUPPORT=0
endif

# Callback
ifeq ($(CONFIG_CB_SUPPORT),Y)
ifeq ($(CONFIG_CB_SOCKET),Y)
	CFLAGS		+= -DCONFIG_CB_SOCKET=1
	ASFLAGS		+= -DCONFIG_CB_SOCKET=1
else
	CFLAGS		+= -DCONFIG_CB_SOCKET=0
	ASFLAGS		+= -DCONFIG_CB_SOCKET=0
endif
else
ifeq ($(CONFIG_CB_SOCKET),Y)
$(error CONFIG_CB_SOCKET depend on CONFIG_CB_SUPPORT)
endif
endif

ifeq ($(CONFIG_CB_SUPPORT),Y)
ifeq ($(CONFIG_STLS),Y)
	CFLAGS		+= -DCONFIG_STLS=1
	ASFLAGS		+= -DCONFIG_STLS=1
else
	CFLAGS		+= -DCONFIG_STLS=0
	ASFLAGS		+= -DCONFIG_STLS=0
endif
else
ifeq ($(CONFIG_STLS),Y)
$(error CONFIG_STLS depend on CONFIG_CB_SUPPORT)
endif
endif

# Secure Boot
ifeq ($(SECURE_BOOT),Y)
	CFLAGS		+= -DSECURE_BOOT=1
	ASFLAGS		+= -DSECURE_BOOT=1
else
	CFLAGS		+= -DSECURE_BOOT=0
	ASFLAGS		+= -DSECURE_BOOT=0
endif

# Secure Debug
ifeq ($(SECURE_DBG),Y)
	CFLAGS		+= -DSECURE_DBG=1
	ASFLAGS		+= -DSECURE_DBG=1
else
	CFLAGS		+= -DSECURE_DBG=0
	ASFLAGS		+= -DSECURE_DBG=0
endif

# Provisioning
ifeq ($(PROVISIONING),Y)
	CFLAGS		+= -DPROVISIONING=1
	ASFLAGS		+= -DPROVISIONING=1
else
	CFLAGS		+= -DPROVISIONING=0
	ASFLAGS		+= -DPROVISIONING=0
endif

# TRNG_CALIBRATION
ifeq ($(TRNG_CALIBRATION),Y)
ifneq ($(MBEDTLS_WITH_DUBHE), Y)
$(error TRNG_CALIBRATION depend on MBEDTLS_WITH_DUBHE)
endif
	CFLAGS		+= -DTRNG_CALIBRATION=1
	ASFLAGS		+= -DTRNG_CALIBRATION=1
else
	CFLAGS		+= -DTRNG_CALIBRATION=0
	ASFLAGS		+= -DTRNG_CALIBRATION=0
endif

# MBEDTLS_WITH_DUBHE
ifeq ($(MBEDTLS_WITH_DUBHE),Y)
	CFLAGS		+= -DMBEDTLS_WITH_DUBHE=1
	ASFLAGS		+= -DMBEDTLS_WITH_DUBHE=1
else
	CFLAGS		+= -DMBEDTLS_WITH_DUBHE=0
	ASFLAGS		+= -DMBEDTLS_WITH_DUBHE=0
endif

# PUF
ifeq ($(PUF),Y)
	CFLAGS		+= -DDBH_OTP_PUF_SUPPORT
	ASFLAGS		+= -DDBH_OTP_PUF_SUPPORT
ifeq ($(PUF_SCRAMBLE),Y)
	CFLAGS		+= -DDBH_OTP_SCRAMBLE
	ASFLAGS		+= -DDBH_OTP_SCRAMBLE
endif ### end PUF_SCRAMBLE
endif ### end PUF

################################################################################
# nspe lib config
################################################################################

TEE_CLIENT_LIB			:=
ifneq ($(TEE_SDK_PATH),)
	TEE_CLIENT_LIB		:=	$(TEE_SDK_PATH)/nspe/lib/libclient.a
else
	TEE_CLIENT_LIB		:=	$(TOP_OUT)/tee/libclient.a
endif

################################################################################
# Toolchain
################################################################################

CC			:=	${CROSS_COMPILE}gcc
CPP			:=	${CROSS_COMPILE}cpp
AS			:=	${CROSS_COMPILE}gcc
AR			:=	${CROSS_COMPILE}ar
LD			:=	${CROSS_COMPILE}ld
OC			:=	${CROSS_COMPILE}objcopy
OD			:=	${CROSS_COMPILE}objdump
NM			:=	${CROSS_COMPILE}nm
PP			:=	${CROSS_COMPILE}gcc -E

################################################################################
# Global helpers
################################################################################

#
# remove directory helper
#
RMDIR := rmdir --ignore-fail-on-non-empty


