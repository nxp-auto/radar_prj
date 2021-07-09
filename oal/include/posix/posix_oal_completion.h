/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_POSIX_COMPLETION_H
#define OAL_POSIX_COMPLETION_H

#include <oal_mutex.h>
#include <oal_utils.h>
#include <posix/posix_oal_condvar.h>

__BEGIN_DECLS

struct OAL_Completion {
	volatile uint32_t mDone;
	struct OAL_Mutex mMutex;
	struct OAL_CondVar mCondVar;
};

__END_DECLS

#endif /* OAL_POSIX_COMPLETION_H */
