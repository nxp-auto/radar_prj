/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_COMPLETION_1_0_H
#define OAL_COMPLETION_1_0_H

#include <oal_completion.h>
#include <oal_utils.h>

__BEGIN_DECLS

static inline int32_t OAL_init_completion(OAL_Completion_t *apComp)
{
	return OAL_InitCompletion(apComp);
}

static inline int32_t OAL_wait_for_completion(OAL_Completion_t *apComp)
{
	return OAL_WaitForCompletion(apComp);
}

static inline uint64_t OAL_wait_for_completion_timeout(OAL_Completion_t *apComp,
                                                       uint64_t aTimeout)
{
	return OAL_WaitForCompletionTimeout(apComp, aTimeout);
}

static inline int32_t OAL_complete(OAL_Completion_t *apComp)
{
	return OAL_Complete(apComp);
}

static inline int32_t OAL_complete_all(OAL_Completion_t *apComp)
{
	return OAL_CompleteAll(apComp);
}

__END_DECLS

#endif /* OAL_COMPLETION_1_0_H */
