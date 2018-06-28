##############################################################################
#
# Copyright 2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
##############################################################################

.PHONY: install

define install_artifact
	@echo "	[INSTALL] $(subst $(DEV_ROOT_DIR),,$(subst $(TEST_ROOT_DIR)/,,$(1)))"
	@mkdir -p $(dir $(OAL_INSTALL)/$(OS)/$(subst $(ODIR)/,,$(subst $(DEV_ROOT_DIR)/,,$(subst $(TEST_ROOT_DIR)/,,$(1))))) && cp -p $(1) "$$_"
endef

install: all
ifeq ($(origin OAL_INSTALL),undefined)
	$(error error: OAL_INSTALL undefined)
endif
	$(if $(wildcard $(EXEC)), $(call install_artifact,$(realpath $(DRIVER_BUILD_DIR)/$(EXEC))))
	$(if $(wildcard $(ST_LIB)), $(call install_artifact,$(realpath $(DRIVER_BUILD_DIR)/$(ST_LIB))))
	$(if $(wildcard $(MODULE_NAME).ko), $(call install_artifact,$(realpath $(DRIVER_BUILD_DIR)/$(MODULE_NAME).ko)))
	$(if $(wildcard $(EXEC).bin), $(call install_artifact,$(realpath $(DRIVER_BUILD_DIR)/$(EXEC).bin)))

