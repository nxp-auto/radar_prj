#
# Copyright 2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Test directory relative to this file
MAKE_INC_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST) )))

include $(DEV_ROOT_DIR)/build/Makefile.inc
