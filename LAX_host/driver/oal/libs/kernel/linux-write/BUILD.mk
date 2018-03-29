##############################################################################
#
# Copyright 2017-2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
##############################################################################


COMMON_DIR = $(DRIVER_DIR)/../common

STATIC_LIB_NAME := oal_kernel

SRCS = oal_comm.c

INCDIR = $(DEV_ROOT_DIR)/include                         \
	 $(DRIVER_DIR)/include                           \

include $(COMMON_DIR)/Common.mk
