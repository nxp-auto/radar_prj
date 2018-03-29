#
# Copyright 2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

MODULE_NAME := rsdk_lax_driver

ifeq ($(OS), linux)
	CDEFS += -D_BSD_SOURCE
endif

SRCS =  lax_driver.c                                    \
	event.c                                    \
	cbuffer.c                                    \
	lax_atu.c                                    \
	sys.c                                        \
	vspa_core.c                                  \

VPATH += $(DRIVER_DIR)/os_agnostic/src
VPATH += $(DRIVER_DIR)/platform/UA1575RDB/src
VPATH += $(DRIVER_DIR)/linux

ifeq ($(OS)-$(OS_WORLD), linux-user)
	VPATH += $(DRIVER_DIR)/src/posix
else
	VPATH += $(DRIVER_DIR)/src/$(OS)
endif

ifeq ($(OS), qnx)
	VPATH += $(DRIVER_DIR)/src/posix
endif

INCDIR = $(DRIVER_DIR)/include                           \
         $(TEST_ROOT_DIR)/include                        \
         $(DEV_ROOT_DIR)/include                         \
         $(DRIVER_DIR)/os_agnostic/inc                   \
         $(DRIVER_DIR)/platform/UA1575RDB/inc            \
         $(DRIVER_DIR)/linux                             \
         $(DRIVER_DIR)/include/uapi/linux                \
         $(DRIVER_DIR)/../../../LAX_dispatcher/common


DEP_STATIC_LIBS := $(DEV_ROOT_DIR)/libs/$(OS_WORLD)/$(OS)-$(OAL_COMM_IMPL)/$(ODIR)/liboal_$(OS_WORLD).a
