/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TIMESPEC_H
#define OAL_TIMESPEC_H

#include "oal_utils.h"
#include "oal_log.h"

__BEGIN_DECLS

#define NSEC_IN_USEC (1000LL)
#define MSEC_IN_SEC  (1000LL)
#define USEC_IN_MSEC (1000LL)
#define NSEC_IN_MSEC (1000000LL)
#define USEC_IN_SEC  (1000000LL)
#define NSEC_IN_SEC  (1000000000LL)

/**
 * @brief Structure to define time in seconds and nanoseconds
 */
struct OAL_Timespec {
	int64_t  sec; ///< The number of seconds
	int64_t nsec; ///< The number of nanoseconds
};

typedef struct OAL_Timespec OAL_Timespec_t;

/**
 * @brief OAL_GetTime Retrieve the time
 *
 * @param tm[in] The timespec structure
 *
 * @return 0 for success, a negative value otherwise
 */
int OAL_GetTime(OAL_Timespec_t *tm);

/**
 * @brief OAL_Sleep Suspend the execution of the calling thread
 * based on value passed via <code>tm<code>.
 *
 * @param tm[in] Sleep duration
 *
 * @return 0 for success, a negative value otherwise
 */
int OAL_Sleep(const OAL_Timespec_t *tm);

/**
 * @brief OAL_nsleep Suspend the execution of the calling thread
 * for nanoseconds intervals
 *
 * @param nsec[in] Sleep interval
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_nsleep(int64_t nsec)
{
	OAL_Timespec_t tm;
	int ret = 0;

	if (nsec < 0) {
		ret = -EINVAL;
	} else {
		tm.sec = nsec / NSEC_IN_SEC;
		tm.nsec = nsec % NSEC_IN_SEC;
		ret = OAL_Sleep(&tm);
	}

	return ret;
}

/**
 * @brief OAL_usleep Suspend the execution of the calling thread
 * for microseconds intervals
 *
 * @param usec[in] Sleep interval
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_usleep(int64_t usec)
{
	int ret = 0;
	OAL_Timespec_t tm;

	if (usec < 0) {
		ret = -EINVAL;
	} else {
		tm.sec = usec / USEC_IN_SEC;
		tm.nsec = (usec % USEC_IN_SEC) * NSEC_IN_USEC;
		ret = OAL_Sleep(&tm);
	}

	return ret;
}

/**
 * @brief OAL_GetTimeFromNs Convert nanoseconds to OAL_Timespec_t
 *
 * @param tm[out] Time in OAL_Timespec_t format
 * @param ns[in]  Time in nanoseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_GetTimeFromNs(OAL_Timespec_t *tm, int64_t ns)
{
	int ret = 0;
	if (tm == NULL) {
		ret = -EINVAL;
	} else {
		tm->sec = ns / NSEC_IN_SEC;
		tm->nsec = ns % NSEC_IN_SEC;
	}
	return ret;
}

/**
 * @brief OAL_GetTimeFromUs Convert microseconds to OAL_Timespec_t
 *
 * @param tm[out] Time in OAL_Timespec_t format
 * @param us[in]  Time in microseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_GetTimeFromUs(OAL_Timespec_t *tm, int64_t us)
{
	int ret = 0;
	if (tm == NULL) {
		ret = -EINVAL;
	} else {
		ret = OAL_GetTimeFromNs(tm, us * NSEC_IN_USEC);
	}
	return ret;
}

/**
 * @brief OAL_GetTimeFromMs Convert milliseconds to OAL_Timespec_t
 *
 * @param tm[out] Time in OAL_Timespec_t format
 * @param ms[in]  Time in milliseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_GetTimeFromMs(OAL_Timespec_t *tm, int64_t ms)
{
	int ret = 0;
	if (tm == NULL) {
		ret = -EINVAL;
	} else {
		ret = OAL_GetTimeFromNs(tm, ms * NSEC_IN_MSEC);
	}
	return ret;
}

/**
 * @brief OAL_TimeToUs Convert OAL_Timespec_t to microseconds
 *
 * @param tm[in]  Time in OAL_Timespec_t format
 * @param us[out] Time in microseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_TimeToUs(const OAL_Timespec_t *tm, int64_t *us)
{
	int ret = 0;
	if ((tm == NULL) || (us == NULL)) {
		ret = -EINVAL;
	} else {
		*us = (tm->sec * USEC_IN_SEC) + (tm->nsec / NSEC_IN_USEC);
	}

	return ret;
}

/**
 * @brief OAL_TimeToUs Convert OAL_Timespec_t to milliseconds
 *
 * @param tm[in]  Time in OAL_Timespec_t format
 * @param us[out] Time in milliseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_TimeToMs(const OAL_Timespec_t *tm, int64_t *ms)
{
	int ret = 0;
	if ((tm == NULL) || (ms == NULL)) {
		ret = -EINVAL;
	} else {
		if (OAL_TimeToUs(tm, ms) == 0) {
			*ms /= 1000;
		}
	}

	return ret;
}

/**
 * @brief OAL_TimeDiff Compute the elapsed time between two OAL_Timespec_t
 * structures. Equivalent of stop - start.
 *
 * @param start[in]  The starting point
 * @param stop[in]   The endpoint
 * @param diff[out]  The difference between start and stop
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_TimeDiff(const OAL_Timespec_t *start,
			       const OAL_Timespec_t *stop,
			       OAL_Timespec_t *diff)
{
	int64_t nsec;
	int ret = 0;

	if ((start == NULL) || (stop == NULL) || (diff == NULL)) {
		ret = -EINVAL;
	} else {
		OAL_Timespec_t pstart = *start;
		if (stop->nsec < pstart.nsec) {
			nsec = ((pstart.nsec - stop->nsec) / NSEC_IN_SEC) + 1;
			pstart.nsec -= (NSEC_IN_SEC * nsec);
			pstart.sec += nsec;
		}

		if ((stop->nsec - pstart.nsec) > NSEC_IN_SEC) {
			nsec = (stop->nsec - pstart.nsec) / NSEC_IN_SEC;
			pstart.nsec += (NSEC_IN_SEC * nsec);
			pstart.sec -= nsec;
		}

		diff->sec = stop->sec - pstart.sec;
		diff->nsec = stop->nsec - pstart.nsec;
	}

	return ret;
}

/**
 * @brief OAL_TimeDiffNs Compute the elapsed time between two OAL_Timespec_t
 * structures in nanoseconds.
 *
 * @param start[in]  The starting point
 * @param stop[in]   The endpoint
 * @param ns[out]    The elapsed time in nanoseconds between start and stop
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_TimeDiffNs(const OAL_Timespec_t *start,
				const OAL_Timespec_t *stop,
				int64_t *ns)
{
	OAL_Timespec_t diff;
	int ret = 0;

	if (ns != NULL) {
		ret = OAL_TimeDiff(start, stop, &diff);
		if (ret == 0) {
			*ns = (diff.sec * NSEC_IN_SEC) + diff.nsec;
		}
	} else {
		ret = -EINVAL;
	}

	return ret;
}

/**
 * @brief OAL_TimeDiffNs Compute the elapsed time between two OAL_Timespec_t
 * structures in microseconds.
 *
 * @param start[in]  The starting point
 * @param stop[in]   The endpoint
 * @param us[out]    The elapsed time in microseconds between start and stop
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_TimeDiffUs(const OAL_Timespec_t *start,
				const OAL_Timespec_t *stop,
				int64_t *us)
{
	int ret = 0;

	if (us != NULL) {
		ret = OAL_TimeDiffNs(start, stop, us);
		if (ret == 0) {
			*us /= NSEC_IN_USEC;
		}
	} else {
		ret = -EINVAL;
	}

	return ret;
}

/**
 * @brief OAL_TimeDiffNs Compute the elapsed time between two OAL_Timespec_t
 * structures in miliseconds.
 *
 * @param start[in]  The starting point
 * @param stop[in]   The endpoint
 * @param us[out]    The elapsed time in miliseconds between start and stop
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_TimeDiffMs(const OAL_Timespec_t *start,
				const OAL_Timespec_t *stop,
				int64_t *ms)
{
	int ret = OAL_TimeDiffNs(start, stop, ms);
	if (ret == 0) {
		*ms /= NSEC_IN_MSEC;
	}

	return ret;
}

/**
 * @brief OAL_GetElapsedNs Compute the elapsed time from <code>start</code>
 * until now in nanoseconds.
 *
 * @param start[in] The starting point
 * @param ns[out]   The elapsed time in nanoseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_GetElapsedNs(const OAL_Timespec_t *start, int64_t *ns)
{
	OAL_Timespec_t now;
	int ret = 0;

	if ((start == NULL) || (ns == NULL)) {
		ret = -EINVAL;
	} else {
		(void) OAL_GetTime(&now);
		ret = OAL_TimeDiffNs(start, &now, ns);
	}

	return ret;
}

/**
 * @brief OAL_GetElapsedUs Compute the elapsed time from <code>start</code>
 * until now in microseconds.
 *
 * @param start[in] The starting point
 * @param us[out]   The elapsed time in microseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_GetElapsedUs(const OAL_Timespec_t *start, int64_t *us)
{
	OAL_Timespec_t now;
	int ret = 0;

	if ((start == NULL) || (us == NULL)) {
		ret = -EINVAL;
	} else {

		ret = OAL_GetTime(&now);
		if (ret == 0) {
			ret = OAL_TimeDiffUs(start, &now, us);
		}
	}
	return ret;
}

/**
 * @brief OAL_GetElapsedMs Compute the elapsed time from <code>start</code>
 * until now in milliseconds.
 *
 * @param start[in] The starting point
 * @param us[out]   The elapsed time in microseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int OAL_GetElapsedMs(const OAL_Timespec_t *start, int64_t *ms)
{
	OAL_Timespec_t now;
	int ret = 0;

	if ((start == NULL) || (ms == NULL)) {
		ret = -EINVAL;
	} else {
		ret = OAL_GetTime(&now);
		if (ret == 0) {
			ret = OAL_TimeDiffMs(start, &now, ms);
		}
	}

	return ret;
}

__END_DECLS

#endif /* OAL_TIMESPEC_H */
