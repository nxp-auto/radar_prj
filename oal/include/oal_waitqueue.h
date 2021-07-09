/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_WAIT_QUEUE_H
#define OAL_WAIT_QUEUE_H

#include <oal_utils.h>
#include <os_oal_waitqueue.h>

__BEGIN_DECLS

/**
 * @defgroup OAL_WaitQueue OAL Wait Queue
 *
 * @{
 * @brief Manage threads waiting for a condition to become true
 * @details Very similar API and functionality as Linux Kernel wait queues.
 * This API is used to manage threads that are waiting for some condition to
 * become <tt>true</tt>.
 *
 * A thread waits a condition to become <tt>true</tt>
 * @code{.c}
 * lRet = OAL_InitWaitQueue(&gWaitQueue);
 * myFlag = 0;
 * ...
 * lRet = OAL_WaitEvent(&gWaitQueue, myFlag == 42);
 * @endcode
 *
 * and another one after processing or receiving an event sets the flag
 * and woke up above thread:
 * @code{.c}
 * myFlag = 42;
 * lRet = OAL_WakeUp(&gWaitQueue);
 * @endcode

 * @see https:/lwn.net/Articles/577370/
 */

/**
 * @brief Wait queue initialization function
 *
 * @param[in] apWq The queue to be initialized
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_InitWaitQueue(OAL_waitqueue_t *apWq);

/**
 * @brief Sleeps until the \p aCondition becomes true.
 * The function evaluates \p aCondition. The execution will be suspended
 * if it's evaluated to <tt>false</tt>. The thread sleeps until it will be woken
 * up by another thread by calling #OAL_WakeUp.
 * The condition will be re-evaluated after each wake-up.
 *
 * @param[in] apWq The wait queue
 * @param[in] aCondition The expression to be evaluated
 *
 * @note If putting to sleep the thread with this function it cannot be
 * interrupted.
 * @return 0 for success, a negative value otherwise
 */
#define OAL_WaitEvent(apWq, aCondition) OAL_OS_WAIT_EVENT(apWq, aCondition)

/**
 * @brief Sleeps until the condition becomes true or the timeout elapses.
 *
 * @param[in] apWq The wait queue
 * @param[in] aCondition The expression to be evaluated
 * @param[in] aTimeout The timeout in jiffies
 *
 * @return
 *         - 0, if the condition evaluated to false after the timeout elapsed,
 *         - 1, if the condition evaluated to true after the timeout elapsed,
 *         - the remaining jiffies,  (at least 1) if the condition evaluated
 *          to <tt>true</tt> before the timeout elapsed.
 * @note If putting to sleep the thread with this function it cannot be
 * interrupted before the timeout expires if the \p aCondition doesn't
 * become true.
 */
#define OAL_WaitEventTimeout(apWq, aCondition, aTimeout)                       \
	OAL_OS_WAIT_EVENT_TIMEOUT(apWq, aCondition, aTimeout)

/**
 * @brief Sleeps until the \c aCondition becomes true.
 *
 * @param[in] apWq The wait queue
 * @param[in] aCondition The expression to be evaluated
 *
 * @return 0 for success, a negative value otherwise
 * @see #OAL_WaitEvent
 */
#define OAL_WaitEventInterruptible(apWq, aCondition)                           \
	OAL_OS_WAIT_EVENT_INTERRUPTIBLE(apWq, aCondition)

/**
 * @brief Sleeps until the condition becomes true or the timeout elapses.
 *
 * @param[in] apWq The wait queue
 * @param[in] aCondition The expression to be evaluated
 * @param[in] aTimeout The timeout in jiffies
 *
 * @return
 *         - 0, if the condition evaluated to false after the timeout elapsed,
 *         - 1, if the condition evaluated to true after the timeout elapsed,
 *         - the remaining jiffies,  (at least 1) if the condition evaluated
 *          to <tt>true</tt> before the timeout elapsed.
 * @see #OAL_WaitEventTimeout
 */
#define OAL_WaitEventInterruptibleTimeout(apWq, aCondition, aTimeout)          \
	OAL_OS_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(apWq, aCondition, aTimeout)

/**
 * @brief Wake up a sleeping thread blocked in a #OAL_WaitEvent call
 *
 * @param[in] apWq The wait queue
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_WakeUp(OAL_waitqueue_t *apWq);

/**
 * @brief Wake up a process performing an interruptible sleep
 *
 * @param[in] apWq The wait queue
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_WakeUpInterruptible(OAL_waitqueue_t *apWq);

/**
 * @brief Destroy the wait queue and release all resources
 *
 * @param[in] apWq The wait queue
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_DestroyWaitQueue(OAL_waitqueue_t *apWq);

__END_DECLS
/** @} */

#include <legacy/oal_waitqueue_1_0.h>

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_waitqueue.h>
#endif
#endif
