/*
 * Copyright 2017-2019, 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TIMESPEC_H
#define OAL_TIMESPEC_H

#include "oal_log.h"
#include "oal_utils.h"

/**
 * @defgroup OAL_Timespec OAL Time specification
 *
 * @{
 * @brief Time related structures
 * @details
 * This API contains time related structures and a few time utilities like:
 *   - Getting time (#OAL_GetTime)
 *   - Sleeping (not safe) (#OAL_Sleep)
 *   - Time subtraction (#OAL_TimeDiff)
 */

__BEGIN_DECLS

/** Number of nanoseconds in a microsecond */
#define OAL_NSEC_IN_USEC (1000LL)
/** Number of milliseconds in a second */
#define OAL_MSEC_IN_SEC (1000LL)
/** Number of microseconds in a millisecond */
#define OAL_USEC_IN_MSEC (1000LL)
/** Number of nanoseconds in a millisecond */
#define OAL_NSEC_IN_MSEC (1000000LL)
/** Number of microseconds in a second */
#define OAL_USEC_IN_SEC (1000000LL)
/** Number of nanoseconds in a second */
#define OAL_NSEC_IN_SEC (1000000000LL)

/**
 * @brief Structure to define time in seconds and nanoseconds
 */
struct OAL_Timespec {
	int64_t mSec;   ///< The number of seconds
	int64_t mNsec;  ///< The number of nanoseconds
};

typedef struct OAL_Timespec OAL_Timespec_t;

/**
 * @brief Retrieve the time
 *
 * @param[in] apTm The timespec structure
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_GetTime(OAL_Timespec_t *apTm);

/**
 * @brief Suspend the execution of the calling thread
 * based on value passed via <code>acpTm</code>.
 *
 * @param[in] acpTm Sleep duration
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_Sleep(const OAL_Timespec_t *acpTm);

/**
 * @brief Suspend the execution of the calling thread
 * for nanoseconds intervals
 *
 * @param[in] aNsec Sleep interval
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_nsleep(int64_t aNsec)
{
	OAL_Timespec_t lTm;
	int32_t lRet = 0;

	if (aNsec < 0) {
		lRet = -EINVAL;
	} else {
		lTm.mSec  = aNsec / OAL_NSEC_IN_SEC;
		lTm.mNsec = aNsec % OAL_NSEC_IN_SEC;
		lRet      = OAL_Sleep(&lTm);
	}

	return lRet;
}

/**
 * @brief Suspend the execution of the calling thread
 * for microseconds intervals
 *
 * @param[in] aUsec Sleep interval
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_usleep(int64_t aUsec)
{
	int32_t lRet = 0;
	OAL_Timespec_t lTm;

	if (aUsec < 0) {
		lRet = -EINVAL;
	} else {
		lTm.mSec  = aUsec / OAL_USEC_IN_SEC;
		lTm.mNsec = (aUsec % OAL_USEC_IN_SEC) * OAL_NSEC_IN_USEC;
		lRet      = OAL_Sleep(&lTm);
	}

	return lRet;
}

/**
 * @brief Convert nanoseconds to #OAL_Timespec_t
 *
 * @param[out] apTm Time in #OAL_Timespec_t format
 * @param[in] aNs   Time in nanoseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_GetTimeFromNs(OAL_Timespec_t *apTm, int64_t aNs)
{
	int32_t lRet = 0;
	if (apTm == NULL) {
		lRet = -EINVAL;
	} else {
		apTm->mSec  = aNs / OAL_NSEC_IN_SEC;
		apTm->mNsec = aNs % OAL_NSEC_IN_SEC;
	}
	return lRet;
}

/**
 * @brief Convert microseconds to #OAL_Timespec_t
 *
 * @param[out] apTm Time in #OAL_Timespec_t format
 * @param[in] aUs  Time in microseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_GetTimeFromUs(OAL_Timespec_t *apTm, int64_t aUs)
{
	int32_t lRet = 0;
	if (apTm == NULL) {
		lRet = -EINVAL;
	} else {
		lRet = OAL_GetTimeFromNs(apTm, aUs * OAL_NSEC_IN_USEC);
	}
	return lRet;
}

/**
 * @brief Convert milliseconds to #OAL_Timespec_t
 *
 * @param[out] apTm Time in #OAL_Timespec_t format
 * @param[in] aMs   Time in milliseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_GetTimeFromMs(OAL_Timespec_t *apTm, int64_t aMs)
{
	int32_t lRet = 0;
	if (apTm == NULL) {
		lRet = -EINVAL;
	} else {
		lRet = OAL_GetTimeFromNs(apTm, aMs * OAL_NSEC_IN_MSEC);
	}
	return lRet;
}

/**
 * @brief Convert #OAL_Timespec_t to microseconds
 *
 * @param[in] acpTm  Time in #OAL_Timespec_t format
 * @param[out] apUs  Time in microseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_TimeToUs(const OAL_Timespec_t *acpTm, int64_t *apUs)
{
	int32_t lRet = 0;
	if ((acpTm == NULL) || (apUs == NULL)) {
		lRet = -EINVAL;
	} else {
		*apUs = (acpTm->mSec * OAL_USEC_IN_SEC) +
		        (acpTm->mNsec / OAL_NSEC_IN_USEC);
	}

	return lRet;
}

/**
 * @brief Convert #OAL_Timespec_t to milliseconds
 *
 * @param[in] acpTm  Time in #OAL_Timespec_t format
 * @param[out] apMs Time in milliseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_TimeToMs(const OAL_Timespec_t *acpTm, int64_t *apMs)
{
	int32_t lRet = 0;
	lRet         = OAL_TimeToUs(acpTm, apMs);
	if (lRet == 0) {
		*apMs /= 1000;
	}
	return lRet;
}

/**
 * @brief Compute the elapsed time between two #OAL_Timespec_t
 * structures. Equivalent of <tt>stop - start</tt>.
 *
 * @param[in] acpStart  The starting point
 * @param[in] acpStop   The endpoint
 * @param[out] apDiff   The difference between \p acpStop and \p acpStart
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_TimeDiff(const OAL_Timespec_t *acpStart,
                                   const OAL_Timespec_t *acpStop,
                                   OAL_Timespec_t *apDiff)
{
	int64_t lNsec;
	int32_t lRet = 0;

	if ((acpStart == NULL) || (acpStop == NULL) || (apDiff == NULL)) {
		lRet = -EINVAL;
	} else {
		OAL_Timespec_t lPstart = *acpStart;
		if (acpStop->mNsec < lPstart.mNsec) {
			lNsec = ((lPstart.mNsec - acpStop->mNsec) /
			         OAL_NSEC_IN_SEC) +
			        1;
			lPstart.mNsec -= (OAL_NSEC_IN_SEC * lNsec);
			lPstart.mSec += lNsec;
		}

		if ((acpStop->mNsec - lPstart.mNsec) > OAL_NSEC_IN_SEC) {
			lNsec =
			    (acpStop->mNsec - lPstart.mNsec) / OAL_NSEC_IN_SEC;
			lPstart.mNsec += (OAL_NSEC_IN_SEC * lNsec);
			lPstart.mSec -= lNsec;
		}

		apDiff->mSec  = acpStop->mSec - lPstart.mSec;
		apDiff->mNsec = acpStop->mNsec - lPstart.mNsec;
	}

	return lRet;
}

/**
 * @brief Compute the elapsed time between two #OAL_Timespec_t
 * structures in nanoseconds.
 *
 * @param[in] acpStart  The starting point
 * @param[in] acpStop   The endpoint
 * @param[out] apNs     The difference between \p acpStart and
 * \p acpStop
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_TimeDiffNs(const OAL_Timespec_t *acpStart,
                                     const OAL_Timespec_t *acpStop,
                                     int64_t *apNs)
{
	OAL_Timespec_t lDiff;
	int32_t lRet = 0;

	if (apNs != NULL) {
		lRet = OAL_TimeDiff(acpStart, acpStop, &lDiff);
		if (lRet == 0) {
			*apNs = (lDiff.mSec * OAL_NSEC_IN_SEC) + lDiff.mNsec;
		}
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

/**
 * @brief Compute the elapsed time between two #OAL_Timespec_t
 * structures in microseconds.
 *
 * @param[in] acpStart  The starting point
 * @param[in] acpStop   The endpoint
 * @param[out] apUs     The elapsed time in microseconds between
 * \p acpStart and \p acpStop
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_TimeDiffUs(const OAL_Timespec_t *acpStart,
                                     const OAL_Timespec_t *acpStop,
                                     int64_t *apUs)
{
	int32_t lRet = 0;

	if (apUs != NULL) {
		lRet = OAL_TimeDiffNs(acpStart, acpStop, apUs);
		if (lRet == 0) {
			*apUs /= OAL_NSEC_IN_USEC;
		}
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

/**
 * @brief Compute the elapsed time between two #OAL_Timespec_t
 * structures in milliseconds.
 *
 * @param[in] acpStart  The starting point
 * @param[in] acpStop   The endpoint
 * @param[out] apMs     The elapsed time in milliseconds between
 * \p acpStart and \p acpStop
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_TimeDiffMs(const OAL_Timespec_t *acpStart,
                                     const OAL_Timespec_t *acpStop,
                                     int64_t *apMs)
{
	int32_t lRet = OAL_TimeDiffNs(acpStart, acpStop, apMs);
	if (lRet == 0) {
		*apMs /= OAL_NSEC_IN_MSEC;
	}

	return lRet;
}

/**
 * @brief Compute the elapsed time from \p acpStart
 * until now in nanoseconds.
 *
 * @param[in] acpStart The starting point
 * @param[out] apNs    The elapsed time in nanoseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_GetElapsedNs(const OAL_Timespec_t *acpStart,
                                       int64_t *apNs)
{
	int32_t lRet = 0;
	OAL_Timespec_t lNow;
	lNow.mSec  = 0;
	lNow.mNsec = 0;

	if ((acpStart == NULL) || (apNs == NULL)) {
		lRet = -EINVAL;
	} else {
		lRet = OAL_GetTime(&lNow);
		if (lRet == 0) {
			lRet = OAL_TimeDiffNs(acpStart, &lNow, apNs);
		}
	}

	return lRet;
}

/**
 * @brief Compute the elapsed time from \p acpStart
 * until now in microseconds.
 *
 * @param[in] acpStart The starting point
 * @param[out] apUs    The elapsed time in microseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_GetElapsedUs(const OAL_Timespec_t *acpStart,
                                       int64_t *apUs)
{
	OAL_Timespec_t lNow;
	int32_t lRet = 0;

	if ((acpStart == NULL) || (apUs == NULL)) {
		lRet = -EINVAL;
	} else {
		lRet = OAL_GetTime(&lNow);
		if (lRet == 0) {
			lRet = OAL_TimeDiffUs(acpStart, &lNow, apUs);
		}
	}
	return lRet;
}

/**
 * @brief Compute the elapsed time from \p acpStart
 * until lNow in milliseconds.
 *
 * @param[in] acpStart The starting point
 * @param[out] apMs    The elapsed time in microseconds
 *
 * @return 0 for success, a negative value otherwise
 */
static inline int32_t OAL_GetElapsedMs(const OAL_Timespec_t *acpStart,
                                       int64_t *apMs)
{
	OAL_Timespec_t lNow;
	int32_t lRet = 0;

	if ((acpStart == NULL) || (apMs == NULL)) {
		lRet = -EINVAL;
	} else {
		lRet = OAL_GetTime(&lNow);
		if (lRet == 0) {
			lRet = OAL_TimeDiffMs(acpStart, &lNow, apMs);
		}
	}

	return lRet;
}

__END_DECLS

/* @} */

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_timespec.h>
#endif
#endif /* OAL_TIMESPEC_H */
