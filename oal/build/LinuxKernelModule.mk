##
# Copyright 2017-2021 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

# Prerequisites
#
# SRCS - Sources to be included in compilation
# KERNEL_DIR - Kernel directory
# MODULE_NAME - The name of the module
# STATIC_LIB_NAME - The name of the static library

ifeq ($(origin KERNEL_DIR),undefined)
    $(error error: KERNEL_DIR undefined. Export it before using the build system. \
    E.g. export KERNEL_DIR=/path/to/linux/kernel/dir)
endif

ifeq ($(strip $(SRCS)),)
    $(error "There are no source files")
endif

include $(DEV_ROOT_DIR)/build/Dependencies.mk
include $(DEV_ROOT_DIR)/build/LinuxBuildConfig.mk
include $(DEV_ROOT_DIR)/build/DeviceTree.mk

define call_kernel_makefile
	$(MAKE) -C $(KERNEL_DIR) \
		KBUILD_EXTMOD="$(DRIVER_BUILD_DIR)/$(OBJDIR)/" \
		KCPPFLAGS="$(CFLGS)" \
		ARCH=$(ARCH) \
		DRIVER_BUILD_DIR="$(CURDIR)/" \
		ENABLE_CODE_COVERAGE=$(ENABLE_CODE_COVERAGE) \
		OAL_LDFLAGS="$(LDFLAGS)" \
		OAL_ROOT="$(DEV_ROOT_DIR)" $(1)

endef

# Current dir
CDIR                := $(dir $(realpath $(lastword $(MAKEFILE_LIST) )))
VPATH               := $(addsuffix /, $(VPATH))
ARCH                ?= arm64
src                  = .
_OBJS               := $(SRCS:.S=.o)
CFLGS               += $(foreach includedir, $(INCDIR),-I$(includedir)) $(CDEFS)
PYTHON              ?= python

# Linux Kernel accepts dependencies with relative paths only
_DEP_OBJECTS        := $(if $(_DEP_STATIC_LIBS),\
                          $(shell $(PYTHON) $(CDIR)/relative_to.py \
                                  $(abspath $(DRIVER_BUILD_DIR)/$(OBJDIR)) \
                                  $(abspath $(_DEP_STATIC_LIBS)) \
                                  $(abspath $(_INJECT_LIBRARY))))
OBJS                := $(_OBJS:.c=.o) $(_DEP_OBJECTS:.a=.o)

ifdef STATIC_LIB_NAME # Kernel library
    VPATH               += $(DEV_ROOT_DIR)/build/
    SRCS                += __dummy.c
    MODULE_NAME         := _dummy
    obj-m               += $(MODULE_NAME).o
    $(MODULE_NAME)-objs := lib.a __dummy.o
    lib-y               := $(OBJS)
else # Kernel Module
    obj-m               += $(MODULE_NAME).o
    $(MODULE_NAME)-objs := $(OBJS) /../../linux-write/build-linux-kernel/liboal_kernel.o
    ldflags-y           := $(OAL_LDFLAGS)
endif


ifeq ($(ENABLE_CODE_COVERAGE),1)
    GCOV_PROFILE := y
endif

# Out of kernel context
ifeq ($(abspath $(CURDIR)),$(abspath $(DRIVER_BUILD_DIR)))
    OAL_STATIC_LIB := $(OBJDIR)/lib$(STATIC_LIB_NAME).a
    OAL_KERNEL_MODULE := $(OBJDIR)/$(MODULE_NAME).ko

    .DEFAULT_GOAL = all
    CLEAN_OBJS    = $(filter-out Makefile,$(wildcard *))

    # Create targets for each dependency
    $(foreach lib, $(_DEP_STATIC_LIBS) $(_INJECT_LIBRARY), $(eval $(call generate_lib, $(lib))))

    # Guard current build folder
    LOCAL_LOCK_FILE := $(OBJDIR)/.kern_local_lock

ifdef STATIC_LIB_NAME # Kernel library
all: $(OAL_STATIC_LIB) | $(OBJDIR)
lib: all
app:
else
all: $(OAL_KERNEL_MODULE) | $(OBJDIR)
lib:
app: all
endif
endif

.NOTPARALLEL:

ifndef OAL_NOINCREMENTAL_BUILD
$(OAL_STATIC_LIB): FORCE | $(OBJDIR_MAKEFILE)
else
$(OAL_STATIC_LIB): $(OBJDIR_MAKEFILE)
endif
	$(call call_kernel_makefile)
	@echo "	[MV] lib.a -> lib$(STATIC_LIB_NAME).a"
	@cp $(OBJDIR)/lib.a $(OBJDIR)/lib$(STATIC_LIB_NAME).o
	@mv $(OBJDIR)/lib.a $(OBJDIR)/lib$(STATIC_LIB_NAME).a

$(OAL_KERNEL_MODULE): FORCE $(_DEP_STATIC_LIBS) $(_INJECT_LIBRARY) | $(OBJDIR_MAKEFILE)
	$(call call_kernel_makefile)

clean:
	$(call clean_objdir)
	@echo "	[CLN] $(CLEAN_OBJS)"
	@rm -f $(CLEAN_OBJS) $(LOCAL_LOCK_FILE) || true

modules_install:
	$(call call_kernel_makefile, modules_install)
    
-include $(DEV_ROOT_DIR)/build/SourceDrop.mk
-include $(DEV_ROOT_DIR)/build/Install.mk


