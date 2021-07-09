/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_waitqueue.h>

int32_t OAL_WakeUpInterruptible(OAL_waitqueue_t *apWq)
{
	return OAL_WakeUp(apWq);
}

int32_t OAL_InitWaitQueue(OAL_waitqueue_t *apWq)
{
	int32_t lRet;

	if (apWq == NULL) {
		lRet = -1;
		goto exit_init_wq;
	}

	lRet = OAL_InitializeMutex(&apWq->mMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize mutex\n");
		goto exit_init_wq;
	}

	lRet = OAL_InitCondVar(&apWq->mCondVar);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize conditional variable\n");
		goto destroy_mutex;
	}

destroy_mutex:
	if (lRet != 0) {
		(void)OAL_DestroyMutex(&apWq->mMutex);
	}

exit_init_wq:
	return lRet;
}

int32_t OAL_DestroyWaitQueue(OAL_waitqueue_t *apWq)
{
	int32_t lRet = 0;
	int32_t lStatus;

	if (apWq == NULL) {
		lRet = -1;
		goto exit_destroy_wq;
	}

	lRet = OAL_DestroyCondVar(&apWq->mCondVar);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to destroy conditional variable\n");
	}

	lStatus = OAL_DestroyMutex(&apWq->mMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to destroy mutex\n");
		lRet = lStatus;
	}

exit_destroy_wq:
	return lRet;
}

int32_t OAL_WakeUp(OAL_waitqueue_t *apWq)
{
	int32_t lRet;

	if (apWq == NULL) {
		lRet = -1;
		goto wake_up_exit;
	}

	lRet = OAL_SignalCondVarWithMutex(&apWq->mCondVar, &apWq->mMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to signal conditional variable\n");
	}

wake_up_exit:
	return lRet;
}

int32_t OAL_OS_WaitQueue(OAL_waitqueue_t *apWq)
{
	int32_t lRet;
	if (apWq == NULL) {
		lRet = -1;
		goto os_wait_exit;
	}

	lRet = OAL_WaitCondVar(&apWq->mCondVar, &apWq->mMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to wait conditional variable\n");
	}

os_wait_exit:
	return lRet;
}

int32_t OAL_OS_WaitEventTimeout(OAL_waitqueue_t *apWq, uint64_t aUsec)
{
	int32_t lRet;

	if (apWq == NULL) {
		lRet = EINVAL;
		goto wait_event_timeout_exit;
	}

	lRet = OAL_TimedWaitCondVar(&apWq->mCondVar, &apWq->mMutex, aUsec);
	if (lRet != 0) {
		lRet = 0;
	}
wait_event_timeout_exit:
	return lRet;
}
