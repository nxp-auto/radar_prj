##############################################################################
#
# Copyright 2017-2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
##############################################################################

ADDITIONAL_FILES = $(DEV_ROOT_DIR)/build/relative_to.sh

SAVE_VPATH    = $(addsuffix /, $(VPATH))
RESOLVED_SRCS = $(foreach file, $(SRCS), $(foreach dir, $(SAVE_VPATH), $(realpath $(wildcard $(dir)/$(file)))))
SAVE_DEPS     = $(addsuffix _save, $(DEP_STATIC_LIBS))

define save_artifacts
	@mkdir -p $(OAL_SOURCE_DROP)
	@for file in $(sort $(1)) ; do echo "	[SAVE] $$file"; done
	@cd $(DEV_ROOT_DIR) && find $(foreach file, $(sort $(1)), $(subst $(DEV_ROOT_DIR)/,, $(file))) -depth -maxdepth 1 -type f -exec cp -p --parents {} $(OAL_SOURCE_DROP) \;
endef

define save_libs
$(1):
	make -C $(dir $(1)) source_drop
endef


.PHONY: source_drop
source_drop: $(SAVE_DEPS)
ifeq ($(origin OAL_SOURCE_DROP),undefined)
	$(error error: OAL_SOURCE_DROP undefined)
endif
	$(if $(RESOLVED_SRCS), $(call save_artifacts, $(RESOLVED_SRCS)))
	$(if $(INCDIR), $(call save_artifacts, $(realpath $(INCDIR))))
	$(if $(MAKEFILE_LIST), $(call save_artifacts, $(realpath $(MAKEFILE_LIST))))
	$(if $(ADDITIONAL_FILES), $(call save_artifacts, $(realpath $(ADDITIONAL_FILES))))

$(foreach lib, $(SAVE_DEPS), $(eval $(call save_libs, $(lib))))
