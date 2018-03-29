/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_UTILS_H
#define OAL_UTILS_H

#include "common_stringify_macros.h"
#include XSTR(OS/os_utils.h)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))
#endif

#define UNUSED_ARG(ARG) (void)(ARG)

#define GET_PARENT_STRUCT(MEMBER_P, PARENT_TYPE, MEMBER_NAME) \
((PARENT_TYPE *)( (char *) (MEMBER_P) - offsetof(PARENT_TYPE, MEMBER_NAME)) )

#define __NARG__(...)  __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
		_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
		_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
		_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
		_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
		_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
		_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
		_61,_62,_63,N,...) N

#define __RSEQ_N() \
	2,2,2,2,             \
	2,2,2,2,2,2,2,2,2,2, \
	2,2,2,2,2,2,2,2,2,2, \
	2,2,2,2,2,2,2,2,2,2, \
	2,2,2,2,2,2,2,2,2,2, \
	2,2,2,2,2,2,2,2,2,2, \
	2,2,2,2,2,2,2,2,1,0

// General definition for any function name
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, tag, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (tag, __VA_ARGS__)

__BEGIN_DECLS

static inline uint8_t getNDigits(uint32_t number)
{
	uint8_t result = 0;

	do {
		result++;
		number /= 10U;
	} while(number != 0U);

	return result;
}

__END_DECLS

#endif
