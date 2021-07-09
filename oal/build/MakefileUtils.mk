##
# Copyright 2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

##################################
# Get parent of a file / directory, equivalent of dirname utility
# @param $(1) The file
# @return The parent
define dirname
    $(abspath $(subst $(lastword $(subst \, ,$(subst /, ,$(1)))),,$(strip $(1))))
endef

##################################
# Double dirname
# @param $(1) The file
# @return parent of the parent
define dirdirname
    $(call dirname,$(call dirname,$(1)))
endef

ifneq ($(OBJDIR),$(DEFAULT_OBJDIR))
    define clean_objdir
        @echo "	[RM] $(OBJDIR)"
        @rm -rf $(OBJDIR) || true
    endef
else
    define clean_objdir
    endef
endif

# Build configuration
DEFAULT_OAL_BUILD_CONFIG = undefined
OAL_BUILD_CONFIG ?= $(DEFAULT_OAL_BUILD_CONFIG)

# Custom output folder
DEFAULT_OBJDIR     := .
OBJDIR_MAKEFILE    ?= $(OBJDIR)/Makefile

ifeq ($(OAL_BUILD_CONFIG),debug)
    OBJDIR ?= debug
endif

ifeq ($(OAL_BUILD_CONFIG),release)
    OBJDIR ?= release
endif

ifeq (,$(OAL_BUILD_CONFIG))
    OBJDIR ?= .
endif

ifeq ($(OAL_BUILD_CONFIG),$(DEFAULT_OAL_BUILD_CONFIG))
    OBJDIR ?= .
endif

ifndef OBJDIR
    $(error Invalid Build configuration : '$(OAL_BUILD_CONFIG)')
endif

$(OBJDIR):
	@echo "	[MKDIR]  $@"
	@mkdir -p $(OBJDIR)

$(OBJDIR_MAKEFILE): $(CURDIR)/Makefile | $(OBJDIR)
	@echo "	[LN] $^ <- $@"
	@ln -s $^ $@
