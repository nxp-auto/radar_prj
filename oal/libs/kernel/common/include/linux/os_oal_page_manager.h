/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_PAGE_MANAGER_H
#define OAL_OS_PAGE_MANAGER_H
#include <oal_utils.h>

#ifndef OAL_PAGE_SIZE
#define OAL_PAGE_SIZE (PAGE_SIZE)
#endif

__BEGIN_DECLS

struct OSOALMemoryChunk {
	uintptr_t mStartPhysAddress;
};

__END_DECLS

#endif /* OAL_OS_PAGE_MANAGER_H */
