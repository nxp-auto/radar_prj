/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_PAGE_MANAGER_H
#define OAL_OS_PAGE_MANAGER_H
#include <oal_utils.h>

#define OAL_PAGE_SIZE ((uintptr_t)(__PAGESIZE))

__BEGIN_DECLS

struct OSOALMemoryChunk {
	uint64_t mStartPhysAddress;
};

__END_DECLS

#endif /* OAL_OS_PAGE_MANAGER_H */
