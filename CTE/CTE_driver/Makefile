#
# Copyright 2019-2021 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

.PHONY: all lib module debug release cleanall cleanalltmp module_clean module_cleantmp modules_install clean preproc


all lib module debug release cleanall cleanalltmp module_clean module_cleantmp preproc modules_install:
	@echo "#### Building CTE driver ####"
	@if [ "$(OSENV)" = "linux" ]; then                                                              \
        if [ "$(PLATFORM)" = "S32R45" ]; then                                                       \
            echo "#### Building CTE driver $@ for linux environment ####" ;                         \
            make -f project/linux/Makefile $@;                                                      \
        else                                                                                        \
            echo "#### Can not make CTE driver $@ for linux environment and $(PLATFORM) ####" ;     \
        fi                                                                                          \
	else                                                                                            \
        if [ "$(PLATFORM)" = "S32R294" ]; then                                                      \
            echo "Building CTE driver $@ for sa environment" ;                                      \
            make -f Makefile_sa $@;                                                                 \
        else                                                                                        \
            if [ "$(PLATFORM)" = "S32R45" ]; then                                                   \
                echo "Building CTE driver $@ for sa environment" ;                                  \
                make -f Makefile_sa $@;                                                             \
            else                                                                                    \
                echo "#### Can not make CTE driver $@ for linux environment and $(PLATFORM) ####" ; \
            fi                                                                                      \
        fi                                                                                          \
    fi
