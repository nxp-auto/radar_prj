##
# Copyright 2017-2022 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

VPATH += $(COMMON_DIR)/src/$(OS)                           \
         $(COMMON_DIR)/src

INCDIR += $(DEV_ROOT_DIR)/include/$(OS)                    \
          $(DEV_ROOT_DIR)/include/                         \
          $(COMMON_DIR)/include                            \
          $(COMMON_DIR)/include/$(OS)                      \
          $(DEV_ROOT_DIR)/libs/kernel/common/include       \


ifneq ($(OS), zephyr)
SRCS += oal_page_manager.c
endif
		  
SRCS += oal_static_pool.c                                  \
        oal_bitset.c                                       \
        oal_byteorder.c                                    \
        oal_endianness.c                                   \
        oal_page_manager.c                                 \


ifeq ($(OAL_DONT_USE_FDT),)
    # Exclude FDT utilities for Integrity because
    # it has its own interface
    ifneq (,$(filter sa qnx linux zephyr, $(OS)))
        SRCS += oal_fdt_utils.c
    endif
endif

ifeq ($(OS), qnx)
    VPATH += $(DEV_ROOT_DIR)/libs/user/common/src/posix    \
             $(COMMON_DIR)/src/posix                       \

    SRCS += qnx_service.c                                  \
            oal_timer.c                                    \
            oal_memmap.c                                   \
            qnx_fdt_utils.c                                \
            qnx_fdt_addr.c                                 \
            qnx_dev.c                                      \
            qnx_dev_resource.c                             \
            qnx_resolve_reg.c                              \
            posix_oal_timespec.c                           \
            posix_oal_waitqueue.c                          \
            posix_oal_completion.c                         \
            posix_oal_uptime.c                             \
            os_oal_timespec.c                              \
            oal_irq_utils.c                                \
            oal_devtree_utils.c                            \
            qnx_posix_typed_mem_utils.c                    \
            posix_oal_thread.c                             \
            os_oal_condvar.c                               \
            os_oal_memmap.c                                \
            os_oal_spinlock.c                              \
            oal_add_reserved_mem.c                         \
            posix_kern_oal_shared_memory.c                 \

endif

ifeq ($(OS), linux)
    SRCS += os_oal_timespec.c                              \
            linux_device.c                                 \
            os_oal_thread.c                                \
            os_oal_atomic.c                                \
            os_oal_bottom_half.c                           \
            os_oal_completion.c                            \
            os_oal_memmap.c                                \
            os_oal_spinlock.c                              \
            os_oal_timer.c                                 \
            os_oal_uptime.c                                \
            os_oal_waitqueue.c                             \
            oal_add_reserved_mem.c                         \
            posix_kern_oal_shared_memory.c                 \


    ifeq ($(OAL_DONT_USE_FDT),)
        SRCS += oal_devtree_utils.c
    endif
endif

ifeq ($(OS), sa)
    VPATH += $(DEV_ROOT_DIR)/libs/user/common/src/$(OS)

    SRCS += os_oal_timespec.c                              \
            os_oal_waitqueue.c                             \
            os_oal_utils.c                                 \
            os_oal_once.c                                  \
            os_oal_atomic.c                                \
            os_oal_timer.c                                 \
            os_oal_completion.c                            \
            os_oal_memmap.c                                \
            os_oal_mutex.c                                 \
            os_oal_spinlock.c                              \
            os_oal_thread.c                                \
            os_oal_uptime.c                                \
            oal_add_reserved_mem.c                         \

    ifeq ($(OAL_DONT_USE_FDT),)
        SRCS += oal_devtree_utils.c
    endif

endif

ifeq ($(OS), zephyr)
    VPATH += $(COMMON_DIR)/src/posix                              \
             $(COMMON_DIR)/../../kernel/common/src/$(OS)          \
             $(DEV_ROOT_DIR)/libs/user/common/src/posix           \

    INCDIR += $(DEV_ROOT_DIR)/include                             \
              $(COMMON_DIR)/include/posix                         \
              $(DEV_ROOT_DIR)/libs/kernel/common/include          \
              $(DEV_ROOT_DIR)/libs/kernel/common/include/$(OS)    \
              $(DEV_ROOT_DIR)/libs/kernel/driver/include/$(OS)    \

    SRCS += zephyr_service.c                                \
            oal_add_reserved_mem.c                          \
            os_oal_memmap.c                                 \
            os_oal_condvar.c                                \
            posix_oal_completion.c                          \
            os_oal_timespec.c                               \
            os_oal_mutex.c                                  \
            os_oal_thread.c                                 \
            os_oal_spinlock.c                               \
            posix_oal_uptime.c                              \
            os_oal_atomic.c                                 \
            os_oal_timer.c                                  \
            posix_oal_waitqueue.c                           \
            os_oal_bottomhalf.c                             \
            os_oal_irq_utils.c                              \
            posix_kern_oal_shared_memory.c                  \

    ifeq ($(OAL_DONT_USE_FDT),)
        SRCS += oal_devtree_utils.c
    endif
endif

override CDEFS += -DOAL_FUNC_IMPLEMENTATION

