/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_SPINLOCK_H
#define OAL_TRACE_SPINLOCK_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_InitSpinLock(...)                                                  \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_InitSpinLock, __VA_ARGS__)

#define OAL_LockSpin(...)                                                      \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_LockSpin, __VA_ARGS__)

#define OAL_UnlockSpin(...)                                                    \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_UnlockSpin, __VA_ARGS__)

#define OAL_InitIRQSpinLock(...)                                               \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_InitIRQSpinLock, __VA_ARGS__)

#define OAL_LockIRQSpin(...)                                                   \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_LockIRQSpin, __VA_ARGS__)

#define OAL_UnlockIRQSpin(...)                                                 \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_UnlockIRQSpin, __VA_ARGS__)

#endif
#endif
