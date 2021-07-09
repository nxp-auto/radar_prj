/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_LOG_H
#define OAL_LOG_H

#include "oal_utils.h"

#if 0
#define OAL_LOG_SUPPRESS_DEBUG
#define OAL_LOG_SUPPRESS_NOTE
#define OAL_LOG_SUPPRESS_WARNING
#define OAL_LOG_SUPPRESS_ERROR
#endif

#define OAL_TRACE1(PRINT_TAG, PRINT_FMT)                                       \
	(void)OAL_PRINT(PRINT_TAG " [%s:%d] " PRINT_FMT, __func__, __LINE__)
#define OAL_TRACE2(PRINT_TAG, PRINT_FMT, ...)                                  \
	(void)OAL_PRINT(PRINT_TAG " [%s:%d] " PRINT_FMT, __func__, __LINE__,       \
	            __VA_ARGS__)

#ifndef OAL_LOG_SUPPRESS_DEBUG
#define OAL_LOG_DEBUG(...) OAL_VFUNC(OAL_TRACE, "[DBG]", __VA_ARGS__)
#else
#define OAL_LOG_DEBUG(...)
#endif

#ifndef OAL_LOG_SUPPRESS_NOTE
#define OAL_LOG_NOTE(...) OAL_VFUNC(OAL_TRACE, "[NOTE]", __VA_ARGS__)
#else
#define OAL_LOG_NOTE(...)
#endif

#ifndef OAL_LOG_SUPPRESS_WARNING
#define OAL_LOG_WARNING(...) OAL_VFUNC(OAL_TRACE, "[WRN]", __VA_ARGS__)
#else
#define OAL_LOG_WARNING(...)
#endif

#ifndef OAL_LOG_SUPPRESS_ERROR
#define OAL_LOG_ERROR(...) OAL_VFUNC(OAL_TRACE, "[ERR]", __VA_ARGS__)
#else
#define OAL_LOG_ERROR(...)
#endif

/* Debugging utilities */
#define OAL_LOG() OAL_TRACE1("", "\n")

#define OAL_NOT_IMPLEMENTED() OAL_LOG_ERROR("%s isn't implemented\n", __func__)

#define OAL_DUMP(MOD, VAR) OAL_TRACE2("", "%s = " MOD "\n", OAL_STR(VAR), VAR)
#define OAL_XDUMP(PTR, OAL_SIZE)                                                   \
	do {                                                                   \
		char8_t *_ptr = (char8_t *)(PTR);                              \
		int32_t _i;                                                    \
		OAL_TRACE2("", "%s = ", OAL_STR(PTR));                         \
		for (_i = 0; _i < (OAL_SIZE); _i++) {                              \
			OAL_PRINT("0x%x ", *(_ptr + _i) & 0xFF);                   \
		}                                                              \
		OAL_PRINT("%s", "\n");                                             \
	} while (1 == 0)

#endif
