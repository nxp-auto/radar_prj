/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_THREAD_H
#define OAL_THREAD_H

#include <oal_utils.h>
#include <os_oal_thread.h>

/**
 * @defgroup OAL_Thread OAL Thread
 *
 * @{
 * @brief Simple thread API
 * @details
 *
 * Threads are the simplest way to delegate a part of the current tasks
 * to another execution unit, thus reducing the overall execution time.
 *
 */

__BEGIN_DECLS

struct OAL_Thread;

/**
 * @brief Callback function passed to #OAL_CreateThread
 *
 * @param[in] apTaskArg Function argument
 *
 * @return A value set by the function
 */
typedef void *(*OAL_TaskCallback_t)(void *apTaskArg);

/**
 * @brief Thread creation mechanism. The created thread will call \p aTaskFunc
 * and pass \p aTaskArg to it.
 *
 * @param[in,out] apThread    The thread
 * @param[in] acpTaskName      Task name, can be <tt>NULL</tt>
 * @param[in] aTaskFunc       The function to run in the thread
 * @param[in] apTaskArg       Argument passed to \p aTaskFunc
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 */
int32_t OAL_CreateThread(struct OAL_Thread *apThread,
                         const char8_t *acpTaskName,
                         OAL_TaskCallback_t aTaskFunc, void *apTaskArg);

/**
 * @brief Wait thread's termination
 *
 * @param[in] apThread The thread
 * @param[in] apThreadReturn Pointer to value returned by #OAL_TaskCallback_t,
 *                           unset it <tt>NULL</tt>.
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 */
int32_t OAL_JoinTask(struct OAL_Thread *apThread, void **apThreadReturn);

__END_DECLS

/* @} */

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_thread.h>
#endif
#endif
