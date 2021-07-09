/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_UPTIME_H
#define OAL_TRACE_UPTIME_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_GetUptime(...)                                                     \
	OAL_TRACE_FUNCTION(0, int32_t, OAL_GetUptime, __VA_ARGS__)

#endif
#endif
