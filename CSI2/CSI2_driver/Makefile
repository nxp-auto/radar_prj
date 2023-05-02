#
# Copyright 2019-2023 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

.PHONY: all lib module debug release preproc clean cleanall cleanalltmp clean_lib module_clean module_cleantmp cleantmp_lib lib_debug lib_release modules_install


all lib module debug release preproc clean cleanall cleanalltmp clean_lib module_clean module_cleantmp cleantmp_lib lib_debug lib_release modules_install:
	@echo "#### Building CSI2 driver ####"
	@if [ "$(OSENV)" = "linux" ]; then                                                               \
        if [ "$(PLATFORM)" = "S32R45" ]; then                                                       \
            echo "#### Building CSI2 driver $@ for linux environment ####" ;                        \
            make -f project/S32R45/Linux/Makefile $@;                                                      \
        else                                                                                        \
            echo "#### Can not make CSI2 driver $@ for linux environment and $(PLATFORM) ####" ;    \
        fi                                                                                          \
	else                                                                                            \
        echo "Building CSI2 driver $@ for sa environment" ;                                         \
        make -f Makefile_sa $@;                                                                     \
    fi
