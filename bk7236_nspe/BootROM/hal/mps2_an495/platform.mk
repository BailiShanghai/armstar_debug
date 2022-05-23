#
# Platform specific make configure file
# For mps2_an495
#

# Platform path
ARM_CPU 	:= cortex-m33
PLAT_PATH	:=	hal/$(PLAT)

PLAT_CPU_ARCH	:=	-mcpu=$(ARM_CPU)+nodsp
LIBC    = ${shell ${CC} -mthumb $(PLAT_CPU_ARCH) -print-file-name=libc.a}
LIBGCC  = ${shell ${CC} -mthumb $(PLAT_CPU_ARCH) -print-libgcc-file-name}

PLAT_INCS	:=	\
			-I$(PLAT_PATH)			\
			-I$(PLAT_PATH)/devices			\
			-Iboard/$(PLAT)	\
			-Iboard/$(PLAT)/layout	\

PLATFROM_SRCS	:=	\
			$(PLAT_PATH)/hal_platform.c		\
			$(PLAT_PATH)/irq_table.S	\

LINKSCRIPT	:=	$(PLAT_PATH)/link/link.ld.S
