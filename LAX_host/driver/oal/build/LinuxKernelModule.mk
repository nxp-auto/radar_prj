##############################################################################
#
# Copyright 2017-2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
##############################################################################

# Prerequisites
#
# SRCS - Sources to be included in compilation
# KERNEL_DIR - Kernel directory
# MODULE_NAME - The name of the module
# STATIC_LIB_NAME - The name of the static library

ifdef STATIC_LIB_NAME
MODULE_NAME := _dummy
endif

define generate_lib
$(1):
	@echo "	[DEP] $1"
	make -C $(dir $(1))
endef

# Current dir
CDIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST) )))

# Linux kernel Makefile works with relative paths
VPATH := $(addsuffix /, $(VPATH))
SRCS := $(foreach file, $(SRCS), $(foreach dir, $(VPATH), $(wildcard $(dir)/$(file))))
SRCS := $(if $(SRCS),$(shell $(CDIR)/relative_to.sh $(DRIVER_BUILD_DIR) $(SRCS)))

# Lock mechanism
LOCK_FILE    := /tmp/$(notdir $(lastword $(MAKEFILE_LIST)))
LOCK_FD      := 9
LOCK_TIMEOUT := 2000

obj-m += $(MODULE_NAME).o

_OBJS := $(SRCS:.S=.o)
OBJS := $(_OBJS:.c=.o)                                                                 \
	$(if $(DEP_STATIC_LIBS),                                                      \
		$(shell $(CDIR)/relative_to.sh $(DRIVER_BUILD_DIR) $(DEP_STATIC_LIBS) \
	))

CFLGS = -Ulinux -DOS=linux
CFLGS += $(foreach includedir, $(INCDIR),-I$(includedir))

ifdef STATIC_LIB_NAME
$(MODULE_NAME)-objs := lib.a
lib-y := $(OBJS)
else
$(MODULE_NAME)-objs := $(OBJS)
endif

.PHONY: clean all $(DEP_STATIC_LIBS)

# Make sure that there is only one instance of Linux Kernel makefile
# LOCK_FILE guards Linux Kernel makefile
all: $(DEP_STATIC_LIBS)
	@exec $(LOCK_FD)>$(LOCK_FILE) && flock -w $(LOCK_TIMEOUT) $(LOCK_FD) && \
		make -C $(KERNEL_DIR) \
			M=`pwd` \
			KCPPFLAGS="$(CFLGS)" \
			ARCH=arm64 \
			DRIVER_BUILD_DIR="$(CURDIR)/" \
			-j1
ifdef STATIC_LIB_NAME
	mv lib.a lib$(STATIC_LIB_NAME).a
endif

# Create targets for each dependency
ifndef STATIC_LIB_NAME
$(foreach lib, $(DEP_STATIC_LIBS), $(eval $(call generate_lib, $(lib))))
endif

clean:
	make -C $(KERNEL_DIR) M=`pwd` ARCH=arm64 clean DRIVER_BUILD_DIR="$(CURDIR)/"
	(rm -f $(CURDIR)/*~ $(CURDIR)/Module.symvers $(CURDIR)/Module.markers $(CURDIR)/modules.order $(OBJS)) || true

-include $(DEV_ROOT_DIR)/build/SourceDrop.mk
