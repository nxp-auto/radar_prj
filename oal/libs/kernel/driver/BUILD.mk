##############################################################################
#
# Copyright 2017-2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
##############################################################################

MODULE_NAME := oal_driver

SRCS += oal_driver_dispatcher.c    \
	os_kernel_module.c         \
	os_oal_allocation_kernel.c \
	os_oal_driver_dispatcher.c \

ifeq ($(OS), linux)
SRCS += cache.S                    \

endif

ifeq ($(OS), qnx)
LDFLAGS += -lfdt
endif

VPATH += $(DRIVER_DIR)/src/$(OS)   \
	 $(DRIVER_DIR)/src         \

INCDIR += $(DRIVER_DIR)/include    \
	  $(DEV_ROOT_DIR)/include  \
	  $(DEV_ROOT_DIR)/include/$(OS) \
	  $(DEV_ROOT_DIR)/libs/$(OS_WORLD)/$(OS)-$(OAL_COMM_IMPL)/include

DEP_STATIC_LIBS := $(DEV_ROOT_DIR)/libs/$(OS_WORLD)/$(OS)-$(OAL_COMM_IMPL)/$(ODIR)/liboal_$(OS_WORLD).a
