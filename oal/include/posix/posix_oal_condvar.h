/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_mutex.h>
#include <oal_utils.h>
#include <os_oal_condvar.h>

__BEGIN_DECLS

int32_t OAL_InitCondVar(struct OAL_CondVar *apCondVar);

int32_t OAL_DestroyCondVar(struct OAL_CondVar *apCondVar);

int32_t OAL_SignalCondVarWithMutex(struct OAL_CondVar *apCondVar,
                                   struct OAL_Mutex *apMutex);

int32_t OAL_SignalCondVar(struct OAL_CondVar *apCondVar);

int32_t OAL_WaitCondVar(struct OAL_CondVar *apCondVar,
                        struct OAL_Mutex *apMutex);

int32_t OAL_TimedWaitCondVar(struct OAL_CondVar *apCondVar,
                             struct OAL_Mutex *apMutex, uint64_t aUsec);

int32_t OAL_BroadcastCondVar(struct OAL_CondVar *apCondVar);

__END_DECLS
