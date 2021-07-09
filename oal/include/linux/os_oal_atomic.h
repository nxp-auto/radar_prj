/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_OS_ATOMIC_H
#define OAL_OS_ATOMIC_H

#include <oal_utils.h>

#ifndef __KERNEL__
#include <posix/posix_oal_atomic.h>
#else

__BEGIN_DECLS

struct OAL_Atomic {
	atomic_t mOSAtomic;
};

__END_DECLS

#endif /* __KERNEL__ */

#endif
