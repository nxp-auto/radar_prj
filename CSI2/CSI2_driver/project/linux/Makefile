############################
# Copyright 2016-2018 NXP
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


MODULE_NAME := rsdk_csi2_driver
EXTRA_SRCDIR = ../../src/GEN-2
SRCS_DIR = ../../src/GEN-2/linux/kernel_space
ifeq ($(origin CAPATH), undefined)
CAPATH:=$(CURDIR)/project/linux
endif
EXTRA_CFLAGS = -Wall -g 
EXTRA_CFLAGS += -I$(CAPATH)/../../api
EXTRA_CFLAGS += -I$(CAPATH)/../../include
EXTRA_CFLAGS += -I$(CAPATH)/../../include/GEN-2
EXTRA_CFLAGS += -I$(CAPATH)/../../include/GEN-2/linux 
EXTRA_CFLAGS += -I$(CAPATH)/../../../../api 
EXTRA_CFLAGS += -I$(CAPATH)/../../../../oal/include 
EXTRA_CFLAGS += -I$(CAPATH)/../../../../oal/include/linux
BINDIR := bin

COMPILE_MODULES := csi2_driver_irq_management.o csi2_driver_platform_specific.o rsdk_csi2_interrupts.o rsdk_csi2_rpc_server.o rsdk_csi2_driver_module.o 

# module flags

CFLGS += -std=gnu99 -Ulinux -Dlinux -DS32R45 -DCONFIG_OF_PROMTREE $(EXTRA_CFLAGS)
VPATH += . $(CAPATH)/$(SRCS_DIR) $(CAPATH)/$(EXTRA_SRCDIR) 
src = .
obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-objs := rsdk_csi2_driver_module.o \
  rsdk_csi2_interrupts.o \
  rsdk_csi2_rpc_server.o \
  csi2_driver_platform_specific.o \
  csi2_driver_irq_management.o \
  ../../../../oal/libs/kernel/linux-write/build-linux-kernel/liboal_kernel.o

#specific library definitions
CFLAGS_LIB = $(CFLAGS)
CFLAGS_LIB += -Iapi -Iinclude -I../../api -Iinclude/GEN-2/linux -I../../oal/include -I../../oal/include/linux -Isrc/GEN-2/linux/user_space -I$(KERNEL_DIR)/../recipe-sysroot/usr/include
DEFINED_SYMBOLS = -std=gnu99 -Ulinux -Dlinux -DS32R45 -Wall -pthread
LIB_PREFIX ?= librsdk_CSI2_driver
LIBNAME := $(BINDIR)/$(LIB_PREFIX)_$(ARTIFACT_SUFFIX).a
LIBNAMEDBG := $(BINDIR)/$(LIB_PREFIX)_$(ARTIFACT_SUFFIX)_debug.a

#
# all, module, clean_module cleantmp_module = only for Linux space
# all others can be used on both OS, Windows & Linux
#
.PHONY: all debug release clean cleanall cleanalltmp module_clean module_cleantmp module modules_install lib cleantmp_lib clean_lib lib_debug lib_release

all: lib module

lib: lib_debug lib_release
debug: lib_debug 
release: lib_release 

cleanall: cleantmp_lib clean_lib
cleanalltmp: cleantmp_lib

module:
	if [ ! -d "$(BINDIR)" ];then     \
		mkdir $(BINDIR);           \
	fi
	make KCPPFLAGS="$(CFLGS)" ARCH=$(ARCH) -C $(KERNEL_DIR) CAPATH="$(CAPATH)" KBUILD_EXTMOD="$(CAPATH)"  modules
	cp -f $(CAPATH)/$(MODULE_NAME).ko $(BINDIR)/$(MODULE_NAME).ko
modules_install:
	$ make -C $(KERNEL_DIR) M="$(CAPATH)" modules_install

lib_debug:
	if [ ! -d "$(BINDIR)" ];then     \
		mkdir $(BINDIR);           \
	fi
	$(CC) -c src/rsdk_csi2_driver_api.c -O0 -g3 $(CFLAGS_LIB) $(DEFINED_SYMBOLS) -o project/linux/rsdk_csi2_driver_api.o
	$(AR) rcs $(LIBNAMEDBG) project/linux/rsdk_csi2_driver_api.o 

lib_release:
	if [ ! -d "$(BINDIR)" ];then     \
		mkdir $(BINDIR);           \
	fi
	$(CC) -c src/rsdk_csi2_driver_api.c -O3 -g0 $(CFLAGS_LIB) $(DEFINED_SYMBOLS) -o project/linux/rsdk_csi2_driver_api.o
	$(AR) rcs $(LIBNAME) project/linux/rsdk_csi2_driver_api.o

module_cleantmp:
	make -C $(KERNEL_DIR) M=$(CAPATH)/ clean
module_clean clean: module_cleantmp
	rm -f $(BINDIR)/$(MODULE_NAME).ko

cleantmp_lib: 
	rm -f project/linux/*.o
	rm -f project/linux/*.su
	rm -f project/linux/*.dfinish

clean_lib:
	rm -f $(BINDIR)/librsdk_CSI2_driver_linux_gcc_S32R45_a53_debug.a
	rm -f $(BINDIR)/librsdk_CSI2_driver_linux_gcc_S32R45_a53.a

print-%:
	@echo $* = $($*)

