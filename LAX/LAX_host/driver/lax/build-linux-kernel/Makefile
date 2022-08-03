############################
# Copyright 2016-2020 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
############################


PLATFORM ?= S32R45
TARGET   ?= a53
COMPILER ?= gcc
OSENV    ?= linux
ARCH     := arm64
#including compilerdefs.mak in here with '-include' and a relative path which is only valid from current dir, 
#to avoid including it in linux source folder sub-makes, to avoid clashing variables like CFLAGS
-include $(CURDIR)/../../compilerdefs.mak


MODULE_NAME := rsdk_lax_driver
ifeq ($(origin CAPATH), undefined)
CAPATH:=$(CURDIR)
endif
BINDIR := $(CAPATH)/../../../../LAX_bin

VPATH += . $(CAPATH)/../src/linux $(CAPATH)/../src $(CAPATH)/../platform/RRM/src
src = .
obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-objs :=  lax_os_core.o   \
    lax_driver.o                        \
    lax_os_core_main.o                  \
  ../../../../../oal/libs/kernel/linux-write/build-linux-kernel/liboal_kernel.o

#specific library definitions
CFLAGS_MOD = $(CFLAGS)
CFLAGS_MOD += -I$(CAPATH)/../inc \
    -I$(CAPATH)/../inc/linux \
    -I$(CAPATH)/../platform/RRM/inc \
    -I$(CAPATH)/../../../../LAX_common \
    -I$(CAPATH)/../../../../../oal/include \
    -I$(CAPATH)/../../../../../oal/include/linux \
    -I$(CAPATH)/../../../../../api \
    -I$(KERNEL_DIR)/../recipe-sysroot/usr/include \
    -DLAX_OS_linux -D$(PLATFORM) -Ulinux -Dlinux

ifeq ($(KERNEL_DIR), )
    $(info "***** Error : KERNEL_DIR not defined !")
endif
ifeq ($(CROSS_COMPILE), )
    $(info "***** Error : CROSS_COMPILE not defined !")
endif


.PHONY: all module clean cleanall cleantmp modules_install

all: clean module

module:
	if [ ! -d "$(BINDIR)" ];then     \
		mkdir $(BINDIR);           \
	fi
	make KCPPFLAGS="$(CFLAGS_MOD)" ARCH=$(ARCH) -C $(KERNEL_DIR) CAPATH="$(CAPATH)" KBUILD_EXTMOD="$(CAPATH)"  modules
	cp -f $(CAPATH)/$(MODULE_NAME).ko $(BINDIR)/$(MODULE_NAME).ko
modules_install:
	$ make -C $(KERNEL_DIR) M="$(CAPATH)" modules_install

cleanall: clean

clean: cleantmp
	rm -f $(BINDIR)/$(MODULE_NAME).ko

cleantmp:
	make -C $(KERNEL_DIR) M=$(CAPATH)/ clean

print-%:
	@echo $* = $($*)

