/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_ARCH_WAITQUEUE_H
#define OAL_ARCH_WAITQUEUE_H
#include <oal_utils.h>

#define OAL_SA_ARCH_WAIT_EVENT()                                                \
	do {                                                                   \
		__asm__ volatile("wfe" ::: "memory");                          \
	} while (0 == 1)

#define OAL_SA_ARCH_GENERATE_EVENT()                                            \
	do {                                                                   \
		__asm__ volatile("sevl" ::: "memory");                         \
	} while (0 == 1)

#include <sa_oal_waitqueue.h>

#endif
