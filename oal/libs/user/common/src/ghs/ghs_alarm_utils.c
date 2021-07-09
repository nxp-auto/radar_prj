/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_timespec.h>
#include <os_oal_uptime.h>
#include <ghs_alarm_utils.h>

int32_t OAL_GHS_ClearAlarm(Clock aClock)
{
	Error lError;
	Value lOverruns;
	int32_t lRet = 0;

	lError = SetClockAlarm(aClock, false, NULL, NULL);
	if (lError != Success) {
		lRet = -1;
		OAL_LOG_ERROR("Failed to disarm (%s)\n", ErrorString(lError));
	}

	lError = ClearClockAlarmOverruns(aClock, &lOverruns);
	if (lError != Success) {
		lRet = -1;
		OAL_LOG_ERROR("Failed clear overruns (%s)\n",
		              ErrorString(lError));
	}

	return lRet;
}

int32_t OAL_GHS_SetOneShotAlarm(Clock aClock, uint64_t aNSec)
{
	Error lError;
	Time lGhsTime, lNow;
	int32_t lRet   = 0;
	uint64_t lSec  = aNSec / (uint64_t)OAL_NSEC_IN_SEC;
	uint64_t lNsec = aNSec - (lSec * (uint64_t)OAL_NSEC_IN_SEC);

	lGhsTime.Seconds  = (int64_t)lSec;
	lGhsTime.Fraction = OAL_GHS_NsecToFraction(lNsec);

	lError = GetClockTime(aClock, &lNow);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get clock time (%s)\n",
		              ErrorString(lError));
		lRet = -1;
		goto set_clock_alarm_exit;
	}

	lError = AddTime(&lGhsTime, &lNow);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get clock time (%s)\n",
		              ErrorString(lError));
		lRet = -1;
		goto set_clock_alarm_exit;
	}

	/* Single shot */
	lError = SetClockAlarm(aClock, false, &lGhsTime, NULLTime);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to set alarm (%s)\n",
		              ErrorString(lError));
		lRet = -1;
		goto set_clock_alarm_exit;
	}

set_clock_alarm_exit:
	return lRet;
}
