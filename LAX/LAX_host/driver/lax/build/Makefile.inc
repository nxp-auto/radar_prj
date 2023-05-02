#
# Copyright 2018-2023 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Test directory relative to this file
MAKE_INC_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST) )))

# OAL module root directory
DEV_ROOT_DIR := $(MAKE_INC_DIR)/../../../../../oal

include $(DEV_ROOT_DIR)/build/Makefile.inc
