/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_POSIX_ONCE_H
#define OAL_POSIX_ONCE_H

#include <oal_once.h>
#include <oal_utils.h>
#include <pthread.h>

#define OAL_ONCE_INIT                                                          \
	{                                                                      \
		.mPosixOnce = PTHREAD_ONCE_INIT                                \
	}

__BEGIN_DECLS

struct OAL_OnceControl {
	pthread_once_t mPosixOnce;
};

__END_DECLS

#endif
