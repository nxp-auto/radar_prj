##
# Copyright 2017-2018, 2022 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

ifndef INSTALLKERNEL
	DRIVER_BUILD_DIR := $(CURDIR)/
endif

# Test directory relative to this file
DEV_MAKE_INC_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST) )))

# Development
DEV_ROOT_DIR ?= $(realpath $(DEV_MAKE_INC_DIR)/../ )

# Output Directory
ODIR := $(lastword $(subst \, ,$(subst /, ,$(DRIVER_BUILD_DIR))))

_DIR_SPLIT := $(subst -,/, $(ODIR))
# OS name
OS := $(lastword $(subst /, , $(dir $(_DIR_SPLIT))))

# OS space [User / kernel]
OS_WORLD := $(lastword  $(subst /, , $(_DIR_SPLIT)))

DRIVER_DIR ?= $(realpath $(DRIVER_BUILD_DIR)/../ )

VPATH := $(wildcard $(DRIVER_DIR)/src)

OAL_SOURCE_DROP ?= $(abspath $(DEV_ROOT_DIR)/../oal_sources)
# Install location
OAL_INSTALL ?= $(abspath $(DEV_ROOT_DIR)/../oal_install)

-include $(DRIVER_DIR)/BUILD.mk

ifneq (,$(findstring graph,$(MAKECMDGOALS)))
ARCH ?= arm64
-include $(DEV_ROOT_DIR)/build/CallGraph.mk
else
-include $(DEV_ROOT_DIR)/build/$(ODIR)/Makefile.inc
endif
