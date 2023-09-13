/*
 * Copyright 2021-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_OS_SEMAPHORE_H
#define OAL_OS_SEMAPHORE_H

#include <oal_utils.h>
#include <semaphore.h>

__BEGIN_DECLS

struct OAL_Sema {
	sem_t mSem;
	uint32_t mValue;
	int32_t mSemVal;
};

__END_DECLS

#endif