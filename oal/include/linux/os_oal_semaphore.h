/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OS_OAL_SEMAPHORE_H
#define OS_OAL_SEMAPHORE_H

#include <oal_utils.h>
#include <semaphore.h>

__BEGIN_DECLS

struct OAL_Sema {
	sem_t mSem;
	unsigned int mValue;
	int mSemVal;
};

__END_DECLS

#endif