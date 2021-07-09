/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_POSIX_WAITQUEUE
#define OAL_POSIX_WAITQUEUE

#include <oal_log.h>
#include <oal_mutex.h>
#include <oal_uptime.h>
#include <oal_waitqueue.h>
#include <posix/posix_oal_condvar.h>

__BEGIN_DECLS

struct OAL_PosixWaitqueue {
	struct OAL_CondVar mCondVar;
	struct OAL_Mutex mMutex;
};

typedef struct OAL_PosixWaitqueue OAL_waitqueue_t;

int32_t OAL_OS_WaitQueue(OAL_waitqueue_t *apWq);

#define OAL_POSIX_WAIT_EVENT(wq, condition)                                    \
	OAL_COMP_EXTENSION({                                                   \
		int _ret;                                                      \
		OAL_waitqueue_t *_wq = (OAL_waitqueue_t *)(&(wq));             \
		_ret                 = OAL_LockMutex(&_wq->mMutex);            \
		if (_ret != 0) {                                               \
			OAL_LOG_ERROR("Failed to take mutex\n");               \
		}                                                              \
		while ((_ret == 0) && (!(condition))) {                        \
			_ret = OAL_OS_WaitQueue(_wq);                          \
		}                                                              \
		if (_ret == 0) {                                               \
			_ret = OAL_UnlockMutex(&_wq->mMutex);                  \
			if (_ret != 0) {                                       \
				OAL_LOG_ERROR("Failed to release mutex\n");    \
			}                                                      \
		} else {                                                       \
			OAL_LOG_ERROR("OAL_OS_WaitQueue failed\n");            \
		}                                                              \
		_ret;                                                          \
	})

int32_t OAL_OS_WaitEventTimeout(OAL_waitqueue_t *apWq, uint64_t aUsec);

#ifndef EOK
#define EOK 0
#endif

#define OAL_POSIX_WAIT_EVENT_TIMEOUT(wq, condition, timeout)                   \
	OAL_COMP_EXTENSION({                                                   \
		int _ret = timeout;                                            \
		int _status;                                                   \
		OAL_waitqueue_t *_wq = (OAL_waitqueue_t *)(&(wq));             \
		_status              = OAL_LockMutex(&_wq->mMutex);            \
		if (_status != 0) {                                            \
			OAL_LOG_ERROR("Failed to take mutex\n");               \
		}                                                              \
		if (!(condition)) {                                            \
			_ret = OAL_OS_WaitEventTimeout(                        \
			    _wq, OAL_TicksToUsec(timeout));                    \
			if (_ret == ETIMEDOUT) {                               \
				if ((condition)) {                             \
					_ret = 1;                              \
				} else {                                       \
					_ret = 0;                              \
				}                                              \
			} else {                                               \
				if (_ret == EOK) {                             \
					if ((condition)) {                     \
						_ret = 2;                      \
					} else {                               \
						_ret = 0;                      \
					}                                      \
				}                                              \
			}                                                      \
		}                                                              \
		_status = OAL_UnlockMutex(&_wq->mMutex);                       \
		if (_status != 0) {                                            \
			OAL_LOG_ERROR("Failed to release mutex\n");            \
		}                                                              \
		_ret;                                                          \
	})

__END_DECLS

#endif
