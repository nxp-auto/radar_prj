##
# Copyright 2017-2021 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

VPATH += $(COMMON_DIR)/src                                        \
         $(COMMON_DIR)/src/$(OS)                                  \
         $(COMMON_DIR)/../../kernel/common/src/                   \

INCDIR += $(DEV_ROOT_DIR)/include/$(OS)                           \
          $(COMMON_DIR)/include                                   \
          $(COMMON_DIR)/include/posix                             \
          $(COMMON_DIR)/include/$(OS)                             \
          $(DEV_ROOT_DIR)/libs/kernel/driver/include              \
          $(DEV_ROOT_DIR)/libs/kernel/common/include              \

ifneq ($(OS), zephyr)
SRCS += oal_free_mem_tree.c                                       \
        oal_memory_allocator.c                                    \
        oal_virtmem_manager.c                                     \
        os_oal_memory_allocator.c                                 \

endif

SRCS += oal_static_pool.c                                         \
		oal_bitset.c                                              \
		oal_byteorder.c                                           \
		oal_endianness.c                                          \

# Exclude FDT utilities for Integrity, Linux Kernel, Zephyr because
# they have their own interface
ifeq (,$(filter ghs zephyr, $(OS)))
    SRCS += oal_fdt_utils.c
endif

ifneq (,$(filter linux qnx, $(OS)))
    SRCS +=                                                       \
            posix_oal_timespec.c                                  \
            posix_oal_once.c                                      \
            posix_oal_memory_allocator.c                          \
            posix_extra_oal_allocator.c                           \
            posix_oal_thread.c                                    \
            posix_oal_atomic.c                                    \
            posix_oal_bottom_half.c                               \
            posix_oal_mutex.c                                     \
            posix_oal_shared_memory.c                             \
            posix_oal_static_memory_pool.c                        \

    INCDIR += $(COMMON_DIR)/include/posix

    VPATH += $(COMMON_DIR)/src/posix
endif

ifeq ($(OS), ghs)

    VPATH += $(COMMON_DIR)/src/posix

    SRCS +=                                                       \
            ghs_memory_region.c                                   \
            ghs_alarm_utils.c                                     \
            ghs_task_utils.c                                      \
            posix_oal_waitqueue.c                                 \
            posix_oal_completion.c                                \
            os_oal_once.c                                         \
            os_oal_uptime.c                                       \
            os_oal_timespec.c                                     \
            os_oal_condvar.c                                      \
            os_oal_timer.c                                        \
            posix_extra_oal_allocator.c                           \
            posix_oal_bottom_half.c                               \
            os_oal_thread.c                                       \
            os_oal_atomic.c                                       \
            os_oal_mutex.c                                        \
            os_oal_spinlock.c                                     \

endif

ifeq ($(OS), linux)
    VPATH += $(DEV_ROOT_DIR)/libs/kernel/common/src/posix

    SRCS += devmem_map.c                                          \
            os_oal_timespec.c                                     \
            posix_oal_uptime.c                                    \
            os_oal_memmap.c                                       \

    LDFLAGS += -lpthread
endif

ifeq ($(OS), sa)

    VPATH += $(COMMON_DIR)/src/posix

    SRCS +=                                                       \
            os_oal_once.c                                         \
            posix_oal_bottom_half.c                               \

    INCDIR += $(DEV_ROOT_DIR)/libs/kernel/common/include          \
              $(DEV_ROOT_DIR)/libs/kernel/common/include/$(OS)    \
              $(DEV_ROOT_DIR)/libs/kernel/driver/include/$(OS)    \

endif

ifeq ($(OS), zephyr)
    VPATH += $(COMMON_DIR)/src/posix                              \
			 $(COMMON_DIR)/src/linux                              \
			 $(COMMON_DIR)/../../kernel/common/src/$(OS)          \

    INCDIR += $(COMMON_DIR)/include/posix

    SRCS += posix_oal_thread.c                                    \
			posix_oal_timespec.c                                  \
			os_oal_timespec.c                                     \
			posix_oal_mutex.c                                     \
			posix_oal_once.c                                      \
			os_oal_spinlock.c                                     \

endif

override CDEFS += -DOAL_FUNC_IMPLEMENTATION

