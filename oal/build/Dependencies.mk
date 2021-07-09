##
# Copyright 2018-2020 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

include $(DEV_ROOT_DIR)/build/MakefileUtils.mk


# Add OBJDIR to all libraries
_DEP_STATIC_LIBS = $(foreach deplib, $(DEP_STATIC_LIBS), \
	$(dir $(deplib))$(OBJDIR)/$(notdir $(deplib)))

ifneq ($(INJECT_LIBRARY),)
   _INJECT_LIBRARY := $(dir $(INJECT_LIBRARY))$(OBJDIR)/$(notdir $(INJECT_LIBRARY))
endif

# Lock mechanism
LOCK_FD      := 9
LOCK_TIMEOUT := 2000

define get_lib_build_dir
    $(if $(subst $(abspath $(OBJDIR)),,$(CURDIR)),\
        $(call dirdirname,$(1)),\
        $(call dirname,$(1)))
endef

FORCE:

# Force library generation at each run
define generate_lib
$(1): FORCE
	@echo "	[DEP] $1"
	@$(MAKE) -C $(call get_lib_build_dir,$(1)) OBJDIR=$(OBJDIR)
endef

