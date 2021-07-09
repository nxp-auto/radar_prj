/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_SA_WAITQUEUE_H
#define OAL_SA_WAITQUEUE_H

#include <oal_utils.h>
#include <oal_waitqueue.h>
#include <arch_oal_timespec.h>
#include <arch_oal_waitqueue.h>

#define OAL_OS_WAIT_EVENT(apWq, aCondition)                                    \
	OAL_COMP_EXTENSION({                                                   \
		int _ret = 0;                                                  \
		while ((aCondition) == 0) {                                    \
			OAL_SA_ARCH_WAIT_EVENT();                               \
		}                                                              \
		_ret;                                                          \
	})

#define OAL_OS_WAIT_EVENT_TIMEOUT(apWq, aCondition, aTimeout)                  \
	OAL_COMP_EXTENSION({                                                   \
		uint64_t _final = OAL_SA_GetTime() + aTimeout;                        \
		uint64_t _ret   = 0;                                           \
                                                                               \
		while (((aCondition) == 0) && (OAL_SA_GetTime() < _final))            \
			;                                                      \
                                                                               \
		if ((OAL_SA_GetTime() < _final))                                      \
			_ret = (_final - OAL_SA_GetTime());                           \
		else                                                           \
			_ret = aCondition;                                     \
                                                                               \
		_ret;                                                          \
	})

#endif
