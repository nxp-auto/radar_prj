##############################################################################
#
# Copyright 2017-2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
##############################################################################

VPATH += $(COMMON_DIR)/src/$(OS) \
	 $(COMMON_DIR)/src

INCDIR += $(DEV_ROOT_DIR)/include/$(OS)

ifneq (,$(filter linux qnx, $(OS)))
SRCS += oal_allocation_kernel.c      \
	oal_allocator.c              \
	oal_cma_list.c               \

endif

ifeq ($(OS), qnx)
	VPATH += $(DEV_ROOT_DIR)/libs/user/common/src/posix

	SRCS += qnx_service.c        \
		oal_uptime.c         \
		oal_timer.c          \
		oal_memmap.c         \
		os_oal_timespec.c    \
		posix_oal_timespec.c \
		oal_wait_queue.c     \
		oal_completion.c     \
		oal_irq_utils.c      \
		qnx_fdt_utils.c      \
		os_kernel_module.c   \
		os_oal_allocation_kernel.c \

endif

ifeq ($(OS), linux)
	SRCS += os_oal_timespec.c    \
		linux_device.c       \
		os_kernel_module.c   \
		os_oal_allocation_kernel.c \

endif

ifeq ($(OS), sa)
	SRCS += os_oal_timespec.c

endif
