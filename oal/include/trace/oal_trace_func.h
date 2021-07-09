/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_TRACE_FUNC_H
#define OAL_TRACE_FUNC_H

#include <oal_log.h>

#define OAL_FUNC_MAP_ARGS0(MACRO, ...)
#define OAL_FUNC_MAP_ARGS1(MACRO, ARG) MACRO(ARG)
#define OAL_FUNC_MAP_ARGS2(MACRO, ARG, ...)                                    \
	MACRO(ARG), OAL_FUNC_MAP_ARGS1(MACRO, __VA_ARGS__)
#define OAL_FUNC_MAP_ARGS3(MACRO, ARG, ...)                                    \
	MACRO(ARG), OAL_FUNC_MAP_ARGS2(MACRO, __VA_ARGS__)
#define OAL_FUNC_MAP_ARGS4(MACRO, ARG, ...)                                    \
	MACRO(ARG), OAL_FUNC_MAP_ARGS3(MACRO, __VA_ARGS__)
#define OAL_FUNC_MAP_ARGS5(MACRO, ARG, ...)                                    \
	MACRO(ARG), OAL_FUNC_MAP_ARGS4(MACRO, __VA_ARGS__)
#define OAL_FUNC_MAP_ARGS6(MACRO, ARG, ...)                                    \
	MACRO(ARG), OAL_FUNC_MAP_ARGS5(MACRO, __VA_ARGS__)
#define OAL_FUNC_MAP_ARGS7(MACRO, ARG, ...)                                    \
	MACRO(ARG), OAL_FUNC_MAP_ARGS6(MACRO, __VA_ARGS__)
#define OAL_FUNC_MAP_ARGS(N, MACRO, ...)                                       \
	OAL_FUNC_MAP_ARGS##N(MACRO, __VA_ARGS__)

#define OAL_TRACE_PRIXPTR_FMT0
#define OAL_TRACE_PRIXPTR_FMT1 "0x%" PRIxPTR
#define OAL_TRACE_PRIXPTR_FMT2                                                 \
	OAL_TRACE_PRIXPTR_FMT1 ", " OAL_TRACE_PRIXPTR_FMT1
#define OAL_TRACE_PRIXPTR_FMT3                                                 \
	OAL_TRACE_PRIXPTR_FMT2 ", " OAL_TRACE_PRIXPTR_FMT1
#define OAL_TRACE_PRIXPTR_FMT4                                                 \
	OAL_TRACE_PRIXPTR_FMT3 ", " OAL_TRACE_PRIXPTR_FMT1
#define OAL_TRACE_PRIXPTR_FMT5                                                 \
	OAL_TRACE_PRIXPTR_FMT4 ", " OAL_TRACE_PRIXPTR_FMT1
#define OAL_TRACE_PRIXPTR_FMT6                                                 \
	OAL_TRACE_PRIXPTR_FMT5 ", " OAL_TRACE_PRIXPTR_FMT1
#define OAL_TRACE_PRIXPTR_FMT7                                                 \
	OAL_TRACE_PRIXPTR_FMT6 ", " OAL_TRACE_PRIXPTR_FMT1
#define OAL_TRACE_PRIXPTR_FMT(N) OAL_TRACE_PRIXPTR_FMT##N

#define OAL_TRACE_COMMA0()
#define OAL_TRACE_COMMA1() ,
#define OAL_TRACE_COMMA2() ,
#define OAL_TRACE_COMMA3() ,
#define OAL_TRACE_COMMA4() ,
#define OAL_TRACE_COMMA5() ,
#define OAL_TRACE_COMMA6() ,
#define OAL_TRACE_COMMA7() ,
#define OAL_TRACE_COMMA(N) OAL_TRACE_COMMA##N()

#define OAL_ARG_FUNC_CALL(ARG) ARG
#define OAL_FUNC_ARG_CAST(ARG) ((uintptr_t)(ARG))

#define OAL_TRACE_VOID_FUNCTION(NUM_ARG, RET_TYPE, NAME, ...)                  \
	OAL_COMP_EXTENSION({                                                   \
		NAME(OAL_FUNC_MAP_ARGS(NUM_ARG, OAL_ARG_FUNC_CALL,             \
		                       __VA_ARGS__));                          \
		OAL_PRINT("%s(" OAL_TRACE_PRIXPTR_FMT(NUM_ARG) ")\n",          \
		          #NAME OAL_TRACE_COMMA(NUM_ARG) OAL_FUNC_MAP_ARGS(    \
		              NUM_ARG, OAL_FUNC_ARG_CAST, __VA_ARGS__));       \
	})

#define OAL_TRACE_FUNCTION(NUM_ARG, RET_TYPE, NAME, ...)                       \
	OAL_COMP_EXTENSION({                                                   \
		RET_TYPE _lRet = NAME(OAL_FUNC_MAP_ARGS(                       \
		    NUM_ARG, OAL_ARG_FUNC_CALL, __VA_ARGS__));                 \
		OAL_PRINT("0x%" PRIxPTR                                        \
		          " = %s(" OAL_TRACE_PRIXPTR_FMT(NUM_ARG) ")\n",       \
		          (uintptr_t)_lRet,                                    \
		          #NAME OAL_TRACE_COMMA(NUM_ARG) OAL_FUNC_MAP_ARGS(    \
		              NUM_ARG, OAL_FUNC_ARG_CAST, __VA_ARGS__));       \
		_lRet;                                                         \
	})

#endif
