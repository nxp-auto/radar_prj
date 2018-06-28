/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_COMPLETION_H
#define OAL_COMPLETION_H

#include "oal_utils.h"
#include XSTR(OS/os_oal_completion.h)

/**
 * Completion API is very similar to Linux's completion API.
 * Please see https://www.kernel.org/doc/Documentation/scheduler/completion.txt
 * for more details.
 */

__BEGIN_DECLS

struct OAL_Completion;
typedef struct OAL_Completion OAL_Completion_t;

/**
 * @brief OAL_init_completion Initialize a completion
 *
 * @param c[in]        The completion structure to be initialized
 *
 * @return 0 for success, a negative value otherwise
 */
int OAL_init_completion(OAL_Completion_t *c);

/**
 * @brief OAL_wait_for_completion Wait for the completion of a concurrent work
 *
 * @param c[in] The completion structure
 *
 * @return 0 for success, a negative value otherwise
 */
int OAL_wait_for_completion(OAL_Completion_t *c);

/**
 * @brief OAL_wait_for_completion_timeout Wait for the completion of a
 * concurrent work with a timeout.
 *
 * @param c[in]        The completion structure
 * @param timeout[in]  Timeout, in ticks (jiffies)
 *
 * @return  0 if timed out, and positive (at least 1, or number of jiffies left
 * till timeout) if completed.
 */
unsigned long OAL_wait_for_completion_timeout(OAL_Completion_t *c, unsigned long timeout);

/**
 * @brief OAL_complete Signal to exactly one of the waiters on completion that
 * it can continue
 *
 * @param c[in]        The completion structure
 *
 * @return 0 for success, a negative value otherwise
 */
int OAL_complete(OAL_Completion_t *c);

/**
 * @brief OAL_complete_all Signal all current waiters on completions.
 *
 * @param c[in]        The completion structure
 *
 * @return 0 for success, a negative value otherwise
 */
int OAL_complete_all(OAL_Completion_t *c);

__END_DECLS

#endif /* OAL_COMPLETION_H */
