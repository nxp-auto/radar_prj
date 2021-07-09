/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_BYTEORDER_H
#define OAL_TRACE_BYTEORDER_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_GetLittleEndian16(...)                                             \
	OAL_TRACE_FUNCTION(1, uint16_t, OAL_GetLittleEndian16, __VA_ARGS__)

#define OAL_GetLittleEndian32(...)                                             \
	OAL_TRACE_FUNCTION(1, uint32_t, OAL_GetLittleEndian32, __VA_ARGS__)

#define OAL_GetLittleEndian64(...)                                             \
	OAL_TRACE_FUNCTION(1, uint64_t, OAL_GetLittleEndian64, __VA_ARGS__)

#define OAL_GetBigEndian16(...)                                                \
	OAL_TRACE_FUNCTION(1, uint16_t, OAL_GetBigEndian16, __VA_ARGS__)

#define OAL_GetBigEndian32(...)                                                \
	OAL_TRACE_FUNCTION(1, uint32_t, OAL_GetBigEndian32, __VA_ARGS__)

#define OAL_GetBigEndian64(...)                                                \
	OAL_TRACE_FUNCTION(1, uint64_t, OAL_GetBigEndian64, __VA_ARGS__)
#endif
#endif
