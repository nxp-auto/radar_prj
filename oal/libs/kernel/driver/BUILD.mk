##
# Copyright 2017-2021, 2023 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

ifneq (,$(filter sa zephyr, $(OS)))
    STATIC_LIB_NAME := oal_driver
else
    MODULE_NAME := oal_driver
endif

ifeq ($(OS), sa)
    SRCS = os_module_probe.c                                                   \
           os_kernel_module.c
endif

ifneq ($(OS), sa)
    SRCS += oal_driver_dispatcher.c                                            \

endif

ifneq (,$(filter linux qnx, $(OS)))
    SRCS += os_kernel_module.c                                                 \
            os_oal_driver_dispatcher.c                                         \
            posix_oal_driver_dispatcher.c                                      \
            driver_main.c                                                      \

    DEP_STATIC_LIBS := $(DEV_ROOT_DIR)/libs/$(OS_WORLD)/$(OS)-$(OAL_COMM_IMPL)/$(ODIR)/liboal_$(OS_WORLD).a

endif

ifeq ($(OS), linux)
    SRCS += cache.S                                                            \

endif

ifeq ($(OS), qnx)
    SRCS += os_module_probe.c                                                  \

    DEP_STATIC_LIBS += $(DEV_ROOT_DIR)/libs/user/qnx-$(OAL_COMM_IMPL)/build-qnx-user/liboal_user.a
endif

ifeq ($(OS), ghs)
    SRCS += os_oal_driver_dispatcher.c                                         \
            os_oal_driver_alloc_memory.c                                       \

    KERNEL_SRCS = os_kernel_module.c
    VAS_SRCS += $(SRCS) oal_memory_server.c

    INTEGRATION_FILE = oal_driver.int
    BSP_APPEND = oal_driver.bsp

    VAS_DEP_LIBS = $(DEV_ROOT_DIR)/libs/$(OS_WORLD)/$(OS)-$(OAL_COMM_IMPL)/build-ghs-user/liboal_$(OS_WORLD).a\
                   $(DEV_ROOT_DIR)/libs/user/$(OS)-$(OAL_COMM_IMPL)/build-ghs-user/liboal_user.a

    INCDIR += $(DEV_ROOT_DIR)/libs/user/common/include/$(OS)
endif

ifeq ($(OS), zephyr)
    SRCS += os_kernel_module.c                                                 \
            os_module_probe.c                                                  \

endif

VPATH += $(DRIVER_DIR)/src/$(OS)                                               \
         $(DRIVER_DIR)/src                                                     \

INCDIR += $(DRIVER_DIR)/include                                                \
          $(DRIVER_DIR)/include/$(OS)                                          \
          $(DEV_ROOT_DIR)/include                                              \
          $(DEV_ROOT_DIR)/include/$(OS)                                        \
          $(DEV_ROOT_DIR)/libs/kernel/common/include                           \
          $(DEV_ROOT_DIR)/libs/kernel/common/include/$(OS)                     \
          $(DEV_ROOT_DIR)/libs/kernel/common/include/posix                     \
          $(DEV_ROOT_DIR)/libs/$(OS_WORLD)/$(OS)-$(OAL_COMM_IMPL)/include

override CDEFS += -DOAL_FUNC_IMPLEMENTATION

