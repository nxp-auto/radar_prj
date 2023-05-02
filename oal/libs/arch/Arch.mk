##
# Copyright 2018-2019, 2021-2022 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

ARCH ?= arm64
VPATH += $(DEV_ROOT_DIR)/libs/arch/$(ARCH)/src      \
         $(DEV_ROOT_DIR)/libs/arch/src

INCDIR += $(DEV_ROOT_DIR)/libs/arch/$(ARCH)/include \
          $(DEV_ROOT_DIR)/libs/arch/include         \

ifeq ($(OS), sa)
	SRCS += arch_oal_spinlock.c                 \
		arch_oal_timespec.c                 \
		sa_oal_timespec.c                   \
		arch_oal_timer.c                    \
		sa_oal_timer.c                      \
		arch_oal_cache.c                    \

endif

ifeq ($(OS), zephyr)
	SRCS += arch_oal_cache.c                    \

endif

