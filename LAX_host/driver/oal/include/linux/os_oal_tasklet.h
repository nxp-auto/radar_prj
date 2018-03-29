/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OS_OAL_TASKLET_H
#define OS_OAL_TASKLET_H

#include <linux/interrupt.h>
#include "oal_utils.h"

/* Kernel space API */
#ifdef __KERNEL__

typedef struct tasklet_struct OAL_Tasklet_t;

#define OS_OAL_DECLARE_TASKLET(NAME, FUNC, DATA) \
	 DECLARE_TASKLET(NAME, FUNC, DATA)

static inline void OAL_tasklet_schedule(const OAL_Tasklet_t *tasklet)
{
	struct tasklet_struct *t = (struct tasklet_struct *)tasklet;
	tasklet_schedule(t);
}

#endif /* __KERNEL__ */
#endif /* OS_OAL_TASKLET_H */
