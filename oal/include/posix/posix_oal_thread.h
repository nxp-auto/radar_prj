/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_POSIX_THREAD_H
#define OAL_POSIX_THREAD_H

#include <oal_utils.h>
#include <pthread.h>

__BEGIN_DECLS

enum OAL_ThreadState {
	OAL_NOT_JOINED_YET,
	OAL_JOINED_THREAD,
};

struct OAL_Thread {
	pthread_t mOSThread;
	enum OAL_ThreadState mState;
};

__END_DECLS

#endif /* ifndef OAL_POSIX_THREAD_H */
