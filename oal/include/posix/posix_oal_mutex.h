/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_POSIX_MUTEX_H
#define OAL_POSIX_MUTEX_H

#include <oal_log.h>
#include <pthread.h>

__BEGIN_DECLS

struct OAL_Mutex {
	pthread_mutex_t mOSMutex;
};

__END_DECLS

#endif
