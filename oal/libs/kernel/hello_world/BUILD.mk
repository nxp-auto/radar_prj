##
# Copyright 2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

# Enable native build
NATIVE_BUILD := 1

STATIC_LIB_NAME := test

SRCS = printhello.c                                   \

VPATH += $(DEV_ROOT_DIR)/libs/kernel/common/src/sa

INCDIR = $(DRIVER_DIR)/include                        \
         $(DEV_ROOT_DIR)/include                      \
         $(DEV_ROOT_DIR)/include/$(OS)                \
