##
# Copyright 2018,2020 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

.PHONY: install

define install_artifact
	$(eval install_path=$(dir $(OAL_INSTALL)/$(OS)/$(subst $(ODIR)/,,$(subst $(DEV_ROOT_DIR)/,,$(subst $(TEST_ROOT_DIR)/,,$(1))))))
	@echo "	[INSTALL] $(subst $(DEV_ROOT_DIR),,$(subst $(TEST_ROOT_DIR)/,,$(1)))"
	@mkdir -p $(install_path) && cp -p $(1) $(install_path)
endef

ARTIFACTS_SOURCE = $(DRIVER_BUILD_DIR)

install: all
ifeq ($(origin OAL_INSTALL),undefined)
	$(error error: OAL_INSTALL undefined)
endif
	$(if $(wildcard $(EXEC)), $(call install_artifact,$(realpath $(ARTIFACTS_SOURCE)/$(EXEC))))
	$(if $(wildcard $(ST_LIB)), $(call install_artifact,$(realpath $(ARTIFACTS_SOURCE)/$(ST_LIB))))
	$(if $(wildcard $(OAL_KERNEL_MODULE)), $(call install_artifact,$(realpath $(ARTIFACTS_SOURCE)/$(OAL_KERNEL_MODULE))))
	$(if $(wildcard $(OAL_STATIC_LIB)), $(call install_artifact,$(realpath $(ARTIFACTS_SOURCE)/$(OAL_STATIC_LIB))))
	$(if $(wildcard $(OAL_SA_BIN)), $(call install_artifact,$(realpath $(ARTIFACTS_SOURCE)/$(OAL_SA_BIN))))
	$(if $(wildcard $(OAL_SA_LIB)), $(call install_artifact,$(realpath $(ARTIFACTS_SOURCE)/$(OAL_SA_LIB))))
	$(if $(wildcard $(MODULE_APP)), $(call install_artifact,$(realpath $(ARTIFACTS_SOURCE)/$(MODULE_APP))))
	$(if $(wildcard $(MODULE_APP_BIN)), $(call install_artifact,$(realpath $(ARTIFACTS_SOURCE)/$(MODULE_APP_BIN))))
