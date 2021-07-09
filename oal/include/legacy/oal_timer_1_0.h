/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_LEGACY_TIMER_1_0_H
#define OAL_LEGACY_TIMER_1_0_H

#include <os_oal_timer.h>

__BEGIN_DECLS

static inline int32_t OAL_setup_timer(OAL_Timer_t *apTimer,
                                      OAL_Timer_callback_t aCallback,
                                      uintptr_t aData, uint32_t aFlags)
{
	return OAL_SetupTimer(apTimer, aCallback, aData, aFlags);
}

static inline int32_t OAL_set_timer_timeout(OAL_Timer_t *apTimer,
                                            uint64_t aNSec)
{
	return OAL_SetTimerTimeout(apTimer, aNSec);
}

static inline int32_t OAL_add_timer(OAL_Timer_t *apTimer)
{
	return OAL_AddTimer(apTimer);
}

static inline int32_t OAL_del_timer(OAL_Timer_t *apTimer)
{
	return OAL_DelTimer(apTimer);
}

static inline void OAL_destroy_timer(OAL_Timer_t *apTimer)
{
	(void)OAL_DestroyTimer(apTimer);
}

__END_DECLS

#endif /* OAL_LEGACY_TIMER_1_0_H */
