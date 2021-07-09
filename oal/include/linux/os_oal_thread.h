/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_OS_THREAD_H
#define OAL_OS_THREAD_H

#include <oal_log.h>

#ifdef __KERNEL__
#include <linux/kthread.h>

struct OAL_Thread {
	struct task_struct *mpOSThread;
	void *(*mpCallback)(void *apArg);
	void *mpFuncArg;
	void *mpFuncRet;
};

#else /* ifdef __KERNEL__ */
#include <posix/posix_oal_thread.h>
#endif /* ifdef __KERNEL__ */
#endif /* ifndef OAL_OS_THREAD_H */
