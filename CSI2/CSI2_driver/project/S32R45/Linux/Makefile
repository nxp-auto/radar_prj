############################
# Copyright 2016-2023 NXP
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
EXTRA_SRCDIR = ../../../src/low_level
SRCS_DIR = ../../../src/linux/kernel_space
ifeq ($(origin CAPATH), undefined)
CAPATH:=$(CURDIR)/project/S32R45/Linux
endif
EXTRA_CFLAGS = -Wall -g 
EXTRA_CFLAGS += -I$(CAPATH)/../../../../../api 
EXTRA_CFLAGS += -I$(CAPATH)/../../../../../Tools/Trace/api 
EXTRA_CFLAGS += -I$(CAPATH)/../../../api
EXTRA_CFLAGS += -I$(CAPATH)/../../../include/linux
EXTRA_CFLAGS += -I$(CAPATH)/../../../include/low_level
EXTRA_CFLAGS += -I$(CAPATH)/../../../include/specific
EXTRA_CFLAGS += -I$(CAPATH)/../../../../../oal/include 
EXTRA_CFLAGS += -I$(CAPATH)/../../../../../oal/include/linux
EXTRA_CFLAGS += -I$(CAPATH)/../../../../../platform_setup/include/ARM/S32R45 
BINDIR := bin

COMPILE_MODULES := CDD_Csi2.o Csi2_Irq.o rsdk_csi2_interrupts.o rsdk_csi2_rpc_server.o rsdk_csi2_driver_module.o 

# module flags

CFLGS += -std=gnu99 -Ulinux -Dlinux -DS32R45 -DCONFIG_OF_PROMTREE -DS32R45_COMMON_H_ $(EXTRA_CFLAGS)
VPATH += . $(CAPATH)/$(SRCS_DIR) $(CAPATH)/$(EXTRA_SRCDIR) 
src = .
obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-objs := rsdk_csi2_driver_module.o \
  rsdk_csi2_interrupts.o \
  rsdk_csi2_rpc_server.o \
  CDD_Csi2.o \
  Csi2_Irq.o \
  Csi2_Irq_Handlers.o \
  ../../../../../oal/libs/kernel/linux-write/build-linux-kernel/liboal_kernel.o

#specific library definitions
CFLAGS_LIB = $(CFLAGS)
CFLAGS_LIB += -Iapi -Iinclude -I../../api -Iinclude/linux -I../../oal/include -I../../oal/include/linux -I$(KERNEL_DIR)/../recipe-sysroot/usr/include
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
	$(CC) -c src/linux/user_space/rsdk_csi2_driver_api.c -O0 -g3 $(CFLAGS_LIB) $(DEFINED_SYMBOLS) -o project/S32R45/Linux/rsdk_csi2_driver_api.o
	$(CC) -c src/linux/user_space/Csi2_Linux_Lib.c -O0 -g3 $(CFLAGS_LIB) $(DEFINED_SYMBOLS) -o project/S32R45/Linux/Csi2_Linux_Lib.o
	$(AR) rcs $(LIBNAMEDBG) project/S32R45/Linux/rsdk_csi2_driver_api.o project/S32R45/Linux/Csi2_Linux_Lib.o

lib_release:
	if [ ! -d "$(BINDIR)" ];then     \
		mkdir $(BINDIR);           \
	fi
	$(CC) -c src/linux/user_space/rsdk_csi2_driver_api.c -O3 -g0 $(CFLAGS_LIB) $(DEFINED_SYMBOLS) -o project/S32R45/Linux/rsdk_csi2_driver_api.o
	$(CC) -c src/linux/user_space/Csi2_Linux_Lib.c -O3 -g0 $(CFLAGS_LIB) $(DEFINED_SYMBOLS) -o project/S32R45/Linux/Csi2_Linux_Lib.o
	$(AR) rcs $(LIBNAME) project/S32R45/Linux/rsdk_csi2_driver_api.o project/S32R45/Linux/Csi2_Linux_Lib.o

module_cleantmp:
	make -C $(KERNEL_DIR) M=$(CAPATH)/ clean
module_clean clean: module_cleantmp
	rm -f $(BINDIR)/$(MODULE_NAME).ko

cleantmp_lib: 
	rm -f project/S32R45/Linux/*.o
	rm -f project/S32R45/Linux/*.su
	rm -f project/S32R45/Linux/*.dfinish

clean_lib:
	rm -f $(BINDIR)/librsdk_CSI2_driver_linux_gcc_S32R45_a53_debug.a
	rm -f $(BINDIR)/librsdk_CSI2_driver_linux_gcc_S32R45_a53.a

print-%:
	@echo $* = $($*)

