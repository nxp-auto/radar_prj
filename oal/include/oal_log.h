/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_LOG_H
#define OAL_LOG_H

#include "oal_utils.h"

#ifndef __KERNEL__
#include <stdio.h>
#define PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define PRINT(fmt, ...) printk(KERN_ALERT  fmt, ##__VA_ARGS__)
#endif

#if 0
#define OAL_LOG_SUPPRESS_DEBUG
#define OAL_LOG_SUPPRESS_NOTE
#define OAL_LOG_SUPPRESS_WARNING
#define OAL_LOG_SUPPRESS_ERROR
#endif


#define TRACE1(tag, fmt)      PRINT(tag " [%s:%d] " fmt, __func__, __LINE__)
#define TRACE2(tag, fmt, ...) PRINT(tag " [%s:%d] " fmt, __func__, __LINE__, __VA_ARGS__)

#ifndef OAL_LOG_SUPPRESS_DEBUG
#define OAL_LOG_DEBUG(...) VFUNC(TRACE, "[DBG]", __VA_ARGS__)
#else
#define OAL_LOG_DEBUG(...)
#endif

#ifndef OAL_LOG_SUPPRESS_NOTE
#define OAL_LOG_NOTE(...) VFUNC(TRACE, "[NOTE]", __VA_ARGS__)
#else
#define OAL_LOG_NOTE(...)
#endif

#ifndef OAL_LOG_SUPPRESS_WARNING
#define OAL_LOG_WARNING(...) VFUNC(TRACE, "[WRN]", __VA_ARGS__)
#else
#define OAL_LOG_WARNING(...)
#endif

#ifndef OAL_LOG_SUPPRESS_ERROR
#define OAL_LOG_ERROR(...) VFUNC(TRACE, "[ERR]", __VA_ARGS__)
#else
#define OAL_LOG_ERROR(...)
#endif

/* Debugging utilities */
#define LOG() TRACE1("", "\n")

#define DUMP(MOD, VAR) TRACE2("", "%s = " MOD "\n", STR(VAR), VAR)
#define XDUMP(PTR, SIZE)                                             \
	do {                                                         \
			char *_ptr = (char *)(PTR);                  \
			int _i;                                      \
			TRACE2("", "%s = ", str(PTR));               \
			for (_i = 0; _i < (SIZE); _i++) {            \
				PRINT("0x%x ", *(_ptr + _i) & 0xFF); \
			}                                            \
			PRINT("%s", "\n");                           \
	}while(0)

#endif
