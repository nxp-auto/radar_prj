/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_COMPLETION_H
#define OAL_COMPLETION_H

#include <oal_utils.h>
#include <os_oal_completion.h>

/**
 * @defgroup OAL_Completion OAL Completion
 *
 * @{
 * @brief Wait for a condition mechanism
 * @details
 * Completion API is very similar to Linux's completion API.
 *
 * Completions are a simple synchronization mechanism that is preferable to
 * sleeping and waking up in some situations. If you have a task that must
 * simply sleep until some process has run its course, completions can do it
 * easily and without race conditions.
 *
 * A completion is, essentially, a one-shot flag that says
 * <em>things may proceed</em>.
 *
 * @see https:/www.kernel.org/doc/Documentation/scheduler/completion.txt
 * @see https:/lwn.net/Articles/23993/
 */

__BEGIN_DECLS

struct OAL_Completion;
typedef struct OAL_Completion OAL_Completion_t;

/**
 * @brief Initialize a completion
 *
 * @param[in] apComp  The completion structure to be initialized
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_InitCompletion(OAL_Completion_t *apComp);

/**
 * @brief Wait for the completion of a concurrent work
 *
 * @param[in]  apComp The completion structure
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_WaitForCompletion(OAL_Completion_t *apComp);

/**
 * @brief Wait for the completion of a concurrent work with a timeout.
 *
 * @param[in] apComp    The completion structure
 * @param[in] aTimeout  Timeout, in ticks (<tt>jiffies</tt>)
 *
 * @return  0 if timed out, and positive (at least 1, or number of
 * <tt>jiffies</tt> left till timeout) if completed.
 * @see #OAL_Uptime
 */
uint64_t OAL_WaitForCompletionTimeout(OAL_Completion_t *apComp,
                                      uint64_t aTimeout);

/**
 * @brief Signal to exactly one of the waiters on completion that it can
 * continue
 *
 * @param[in] apComp The completion structure
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_Complete(OAL_Completion_t *apComp);

/**
 * @brief OAL_complete_all Signal all current waiters on completions.
 *
 * @param[in] apComp  The completion structure
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_CompleteAll(OAL_Completion_t *apComp);

/**
 * @brief Destroy completion
 *
 * @param[in] apComp  The completion to be released
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_DestroyCompletion(OAL_Completion_t *apComp);

__END_DECLS
/* @} */

#include <legacy/oal_completion_1_0.h>

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_comm.h>
#endif
#endif /* OAL_COMPLETION_H */
