############################
# Copyright 2020-2023 NXP
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


MODULE_NAME := rsdk_cte_driver
EXTRA_SRCDIR = ../../../src/low_level
SRCS_DIR = ../../../src/linux/kernel_space
ifeq ($(origin CAPATH), undefined)
CAPATH:=$(CURDIR)/project/S32R45/Linux
endif
EXTRA_CFLAGS = -Wall -g 
EXTRA_CFLAGS += -I$(CAPATH)/../../../api
EXTRA_CFLAGS += -I$(CAPATH)/../../../include
EXTRA_CFLAGS += -I$(CAPATH)/../../../include/low_level
EXTRA_CFLAGS += -I$(CAPATH)/../../../include/linux 
EXTRA_CFLAGS += -I$(CAPATH)/../../../../../api 
EXTRA_CFLAGS += -I$(CAPATH)/../../../../../platform_setup/include/ARM/S32R45
EXTRA_CFLAGS += -I$(CAPATH)/../../../../../oal/include 
EXTRA_CFLAGS += -I$(CAPATH)/../../../../../oal/include/linux
BINDIR := bin

COMPILE_MODULES := CDD_Cte.o Cte_Irq.o rsdk_cte_interrupt.o rsdk_cte_rpc_server.o rsdk_cte_driver_module.o 

# module flags

CFLGS += -std=gnu99 -Ulinux -Dlinux -DS32R45 -DCONFIG_OF_PROMTREE $(EXTRA_CFLAGS)
VPATH += . $(CAPATH)/$(SRCS_DIR) $(CAPATH)/$(EXTRA_SRCDIR) 
src = .
obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-objs := rsdk_cte_driver_module.o \
  rsdk_cte_interrupt.o \
  rsdk_cte_rpc_server.o \
  CDD_Cte.o \
  Cte_Irq.o \
  ../../../../../oal/libs/kernel/linux-write/build-linux-kernel/liboal_kernel.o

#specific library definitions
CFLAGS_LIB = $(CFLAGS)
CFLAGS_LIB += -Iapi -Iinclude -I../../api -Iinclude/linux -Iinclude/low_level -I../../oal/include -I../../oal/include/linux -Isrc/linux/user_space -I$(KERNEL_DIR)/../recipe-sysroot/usr/include -I../../platform_setup/include/ARM/S32R45
DEFINED_SYMBOLS = -std=gnu99 -Ulinux -Dlinux -DS32R45 -Wall -pthread
LIB_PREFIX ?= librsdk_CTE_driver
LIBNAME := $(BINDIR)/$(LIB_PREFIX)_$(ARTIFACT_SUFFIX).a
LIBNAMEDBG := $(BINDIR)/$(LIB_PREFIX)_$(ARTIFACT_SUFFIX)_debug.a


#
# all, module, clean_module cleantmp_module = only for Linux space
# all others can be used on both OS, Windows & Linux
#
.PHONY: all debug release cleanall cleanalltmp module_clean module_cleantmp module modules_install lib clean preproc

all: lib module

lib: lib_debug lib_release
debug: lib_debug 
release: lib_release 

cleanall: cleantmp_lib clean_lib module_clean
cleanalltmp: cleantmp_lib

module:
	if [ ! -d "$(BINDIR)" ];then     \
		mkdir $(BINDIR);           \
	fi
	make KCPPFLAGS="$(CFLGS)" ARCH=$(ARCH) -C $(KERNEL_DIR) CAPATH="$(CAPATH)" KBUILD_EXTMOD="$(CAPATH)"  modules
	cp -f $(CAPATH)/$(MODULE_NAME).ko $(BINDIR)/$(MODULE_NAME).ko
modules_install:
	$ make -C $(KERNEL_DIR) M="$(CAPATH)" modules_install

preproc lib_debug:
	if [ ! -d "$(BINDIR)" ];then     \
		mkdir $(BINDIR);           \
	fi
	$(CC) -c src/linux/user_space/rsdk_cte_linux_lib.c -O0 -g3 $(CFLAGS_LIB) $(DEFINED_SYMBOLS) -o project/S32R45/Linux/rsdk_cte_linux_lib.o
	$(AR) rcs $(LIBNAMEDBG) project/S32R45/Linux/rsdk_cte_linux_lib.o

lib_release:
	if [ ! -d "$(BINDIR)" ];then     \
		mkdir $(BINDIR);           \
	fi
	$(CC) -c src/linux/user_space/rsdk_cte_linux_lib.c -O3 -g0 $(CFLAGS_LIB) $(DEFINED_SYMBOLS) -o project/S32R45/Linux/rsdk_cte_linux_lib.o
	$(AR) rcs $(LIBNAME) project/S32R45/Linux/rsdk_cte_linux_lib.o

module_cleantmp:
	make -C $(KERNEL_DIR) M=$(CAPATH)/ clean
module_clean clean: module_cleantmp
	rm -f $(BINDIR)/$(MODULE_NAME).ko

cleantmp_lib: 
	rm -f project/S32R45/Linux/*.o
	rm -f project/S32R45/Linux/*.su
	rm -f project/S32R45/Linux/*.dfinish

clean_lib:
	rm -f $(LIBNAME)
	rm -f $(LIBNAMEDBG)

print-%:
	@echo $* = $($*)

