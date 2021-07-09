##
# Copyright 2018-2020 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

ifeq ($(OAL_BUILD_CONFIG),debug)
    override CDEFS += -g3
endif

ifeq ($(OAL_BUILD_CONFIG),release)
    override CDEFS += -O3
endif
