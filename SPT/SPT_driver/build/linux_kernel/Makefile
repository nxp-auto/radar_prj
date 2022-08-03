#  
#  Copyright 2019-2021 NXP
#  
#  SPDX-License-Identifier: BSD-3-Clause
#  

ifneq ($(RSDK_ROOTPATH), )
	_RSDK_ROOTPATH := ../$(RSDK_ROOTPATH)
else
	_RSDK_ROOTPATH := ../../../..
endif
	
RSDK_ABS_ROOTPATH := $(PWD)/$(_RSDK_ROOTPATH)

ifneq ($(SPT_ROOTPATH), )
	_SPT_ROOTPATH := ../$(SPT_ROOTPATH)
else
	_SPT_ROOTPATH := ../..
endif

ifeq ($(DBG_PRINT),true)
	PRINTK_EN := PRINTK_ENABLE
else
	PRINTK_EN := PRINTK_ENABLEnot
endif

SPT_ABS_ROOTPATH := $(RSDK_ABS_ROOTPATH)/SPT/SPT_driver

SRCDIR_LINUX = $(_SPT_ROOTPATH)/src/linux
SRCDIR_COMMON = $(_SPT_ROOTPATH)/src

SRCS := $(SRCDIR_LINUX)/spt_driver_module.c 		\
		$(SRCDIR_LINUX)/spt_interrupts_kernel.c 	\
		$(SRCDIR_LINUX)/spt_oal_comm_kernel.c 		\
		$(SRCDIR_COMMON)/common/Spt_Hw_Check.c
INCLUDES := $(SPT_ABS_ROOTPATH)/include 		  \
			$(SPT_ABS_ROOTPATH)/include/common	  \
			$(SPT_ABS_ROOTPATH)/include/specific  \
			$(RSDK_ABS_ROOTPATH)/api              \
			$(RSDK_ABS_ROOTPATH)/oal/include      \
			$(RSDK_ABS_ROOTPATH)/oal/include/linux \
			$(RSDK_ABS_ROOTPATH)/platform_setup/include/ARM/S32R45
LIBS = $(_RSDK_ROOTPATH)/oal/libs/kernel/linux-write/build-linux-kernel/liboal_kernel.o
OBJS := $(SRCS:.c=.o) $(LIBS)

#including compilerdefs.mak to get $(KERNEL_DIR). Using '-include' and a relative path which is only valid from current dir, 
#to avoid including it in linux source folder sub-makes which would cause clashing variables like CFLAGS
-include $(_RSDK_ROOTPATH)/compilerdefs.mak

CROSS_COMPILE ?= $(CPU_TOOLCHAIN_BINPATH)
export CROSS_COMPILE

#----- kbuild config: ----------------------------
MODULE_NAME := rsdk_spt_driver
ARCH ?= arm64
EXTRA_CFLAGS := -Wall -g $(addprefix -I,$(INCLUDES))
CFLGS := -std=gnu99 -Ulinux -Dlinux -DOS=linux -DS32R45 -D$(PRINTK_EN)

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-objs := $(OBJS)

.PHONY: all clean cleanall module modules_install clean_module

all: module

cleanall clean: clean_module

module:
	$(MAKE) -C $(KERNEL_DIR) KCPPFLAGS="$(CFLGS)" ARCH=$(ARCH) M="$(CURDIR)" modules -j1
modules_install:
	$ make -C $(KERNEL_DIR) M="$(CURDIR)" modules_install


clean_module:
	$(MAKE) -C $(KERNEL_DIR) M="$(CURDIR)" clean
	rm -f $(SRCDIR_LINUX)/*.o $(SRCDIR_LINUX)/.*.cmd $(SRCDIR_COMMON)/*.o $(SRCDIR_COMMON)/.*.cmd
	
print-%:
	@echo $* = $($*)

