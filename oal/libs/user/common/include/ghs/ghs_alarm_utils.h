/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_GHS_ALARM_UTILS_H
#define OAL_GHS_ALARM_UTILS_H

#include <oal_utils.h>

__BEGIN_DECLS

/**
 * @bfief Set alarm on a clock
 *
 * @param[in] aClock Integrity clock
 * @param[in] aNSec  Delay in nanoseconds
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
int32_t OAL_GHS_SetOneShotAlarm(Clock aClock, uint64_t aNSec);

/**
 * @bfief Disarm alarms clear overruns.
 *
 * @param[in] aClock Integrity clock
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
int32_t OAL_GHS_ClearAlarm(Clock aClock);

__END_DECLS
#endif
