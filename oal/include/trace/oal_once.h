/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_ONCE_H
#define OAL_TRACE_ONCE_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_ExecuteOnce(...)                                                   \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_ExecuteOnce, __VA_ARGS__)

#endif
#endif
