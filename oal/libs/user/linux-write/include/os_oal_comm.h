/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_COMM_H
#define OAL_OS_COMM_H

#include <oal_comm.h>

__BEGIN_DECLS

static inline size_t OAL_GetArgumentsBufferSize(OAL_FuncArgs_t *apArgs,
                                            size_t aNumArgs)
{
	size_t lIdx;
	size_t lRet;
	lRet = 0;

	for (lIdx = 0; lIdx < aNumArgs; lIdx++) {
		lRet += apArgs[lIdx].mSize;
	}

	return lRet;
}

__END_DECLS

#endif /* OAL_OS_COMM_H */
