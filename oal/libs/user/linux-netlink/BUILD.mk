##
# Copyright 2017-2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

# Enable native build
NATIVE_BUILD := 1

COMMON_DIR = $(DRIVER_DIR)/../common
STATIC_LIB_NAME := oal_user

SRCS = oal_comm.c

INCDIR = $(DEV_ROOT_DIR)/include        \
         $(DRIVER_DIR)/include          \

VPATH += $(DRIVER_DIR)/src/$(OS)        \

include $(COMMON_DIR)/Common.mk
