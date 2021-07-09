/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_MUTEX_H
#define OAL_TRACE_MUTEX_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_InitializeMutex(...)                                               \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_InitializeMutex, __VA_ARGS__)

#define OAL_LockMutex(...)                                                     \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_LockMutex, __VA_ARGS__)

#define OAL_UnlockMutex(...)                                                   \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_UnlockMutex, __VA_ARGS__)

#define OAL_DestroyMutex(...)                                                  \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_DestroyMutex, __VA_ARGS__)

#endif
#endif
