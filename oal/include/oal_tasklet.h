/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TASKLET_H
#define OAL_TASKLET_H

#include "common_stringify_macros.h"
#include XSTR(OS/os_oal_tasklet.h)

__BEGIN_DECLS

/* Each platform must have its own implementation of the OAL_Tasklet_t.
 * OAL_Tasklet_t describes a tasklet and  must be defined in os_oal_tasklet.h
 * for that specific OS.
 */

/**
 * @brief OAL_DECLARE_TASKLET Declare and initialize a tasklet
 *
 * @param NAME The name of the tasklet
 * @param FUNC The callback called by the tasklet
 * @param DATA The argument to be passed to FUNC
 *
 */
#define OAL_DECLARE_TASKLET(NAME, FUNC, DATA) \
	OS_OAL_DECLARE_TASKLET(NAME, FUNC, DATA)

/**
 * @brief tasklet_schedule Schedule a tasklet
 *
 * @param tasklet The tasklet to be scheduled
 */
void OAL_tasklet_schedule(const OAL_Tasklet_t *tasklet);

__END_DECLS
#endif /* OAL_TASKLET_H */
