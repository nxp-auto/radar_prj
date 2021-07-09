/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/queue.h>

#include <oal_irq_utils.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <qnx_irq.h>

#define IRQ_THREAD_PRIO 200

// IRQ pulses
#define IRQ_PULSE_CODE (_PULSE_CODE_MINAVAIL + 1)
#define IRQ_EXIT_CODE (IRQ_PULSE_CODE + 1)

struct _qnx_irq {
	TAILQ_ENTRY(_qnx_irq) mTail;

	int32_t mIrqNum;
	int32_t mChannId;
	int32_t mIrqId;
	OAL_irq_handler_t mHandler;
	void *mpData;

	pthread_t mThread;
	int32_t mConid;
};

static int32_t channel_init(int32_t *apIntChannel, int32_t *apIntConId)
{
	int32_t lRet  = 0;
	*apIntChannel = ChannelCreate(_NTO_CHF_UNBLOCK);
	if (*apIntChannel == -1) {
		OAL_LOG_ERROR("ChannelCreate failed: %s\n", strerror(errno));
		lRet = -1;
		goto channel_init_exit;
	}

	*apIntConId = ConnectAttach(0, 0, *apIntChannel, _NTO_SIDE_CHANNEL, 0);
	if (*apIntConId == -1) {
		OAL_LOG_ERROR("ConnectAttach failed: %s\n", strerror(errno));
		(void)ChannelDestroy(*apIntChannel);
		*apIntChannel = -1;
		lRet          = -1;
	}

channel_init_exit:
	return lRet;
}

static void *qnx_irq_thread(void *apArg)
{
	int32_t lRcvId;
	struct _pulse lPulse;
	struct _qnx_irq *lpIrqData = (struct _qnx_irq *)apArg;

	if (lpIrqData == NULL) {
		OAL_LOG_ERROR("Invalid argument\n");
		goto irq_thread_exit;
	}

	OAL_LOG_NOTE("Waiting interrupts ...\n");
	do {
		lRcvId =
		    MsgReceivePulse_r(lpIrqData->mChannId, &lPulse, sizeof(lPulse), NULL);

		/* Interrupted by a signal */
		if (lRcvId == -EINTR) {
			break;
		}

		/* Channel ID doesn't exists */
		if (lRcvId == -ESRCH) {
			break;
		}

		if (lRcvId != EOK) {
			OAL_LOG_ERROR("MsgReceivePulse_r returned %d\n",
			              lRcvId);
			continue;
		}

		/* IRQ thread shutdown */
		if (lPulse.code == IRQ_EXIT_CODE) {
			OAL_LOG_NOTE("Detaching interrupt %d\n",
			             lpIrqData->mIrqNum);
			break;
		}

		/* Call registered interrupt handler */
		if (lPulse.code == IRQ_PULSE_CODE) {
			(void)lpIrqData->mHandler(lpIrqData->mIrqNum,
			                          lpIrqData->mpData);
			(void)InterruptUnmask(lpIrqData->mIrqNum, lpIrqData->mIrqId);
		}

	} while (true);

irq_thread_exit:
	return NULL;
}

static TAILQ_HEAD(, _qnx_irq) gQnxIRQQueue;

static struct _qnx_irq gsProcessIRQS[OAL_QNX_MAX_IRQS_PER_PROCESS];
static OAL_DECLARE_STATIC_POOL(gsIRQPool, OAL_ARRAY_SIZE(gsProcessIRQS));

static void init_queue(void) { TAILQ_INIT(&gQnxIRQQueue); }

int32_t OAL_SetIrq(int32_t aIrqNum, OAL_irq_handler_t aHandler, void *apData)
{
	struct _qnx_irq *lpIrq = NULL;
	int32_t lRet                      = 0;
	static pthread_once_t lsQueueOnce = PTHREAD_ONCE_INIT;
	struct sigevent lEvent;

	if (ThreadCtl(_NTO_TCTL_IO, NULL) == -1) {
		OAL_LOG_ERROR("Failed to get root permissions: %s\n",
		              strerror(errno));
		lRet = -EPERM;
		goto set_irq_exit;
	}

	lRet = pthread_once(&lsQueueOnce, init_queue);
	if (lRet != EOK) {
		OAL_LOG_ERROR("pthread_once failed\n");
		goto set_irq_exit;
	}

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&gsIRQPool, gsProcessIRQS, &lpIrq);
	if ((lRet != 0) || (lpIrq == NULL)) {
		OAL_LOG_ERROR("There are no more available positions "
			"in QNX IRQ pool. Please adjust its size.\n");
		lRet = -ENOMEM;
		goto set_irq_exit;
	}

	lpIrq->mIrqNum  = aIrqNum;
	lpIrq->mHandler = aHandler;
	lpIrq->mpData   = apData;

	lRet = channel_init(&lpIrq->mChannId, &lpIrq->mConid);
	if (lRet == -1) {
		goto release_pool_elem;
	}

	/* Trigger a pulse for each interrupt */
	SIGEV_PULSE_INIT(&lEvent, lpIrq->mConid, IRQ_THREAD_PRIO,
	                 IRQ_PULSE_CODE, NULL);

	/* Attach the event to interrupt source */
	lpIrq->mIrqId = InterruptAttachEvent(lpIrq->mIrqNum, &lEvent,
	                              _NTO_INTR_FLAGS_TRK_MSK);
	if (lpIrq->mIrqId == -1) {
		OAL_LOG_ERROR("InterrruptAttachEvent (irq 0x%x) - %s\n",
		              lpIrq->mIrqNum, strerror(errno));
		lRet = -1;
		goto destroy_conn;
	}

	lRet = pthread_create(&lpIrq->mThread, NULL, qnx_irq_thread, lpIrq);
	if (lRet != 0) {
		OAL_LOG_ERROR("pthread_create failed with %d code\n", lRet);
		lRet = -1;
		goto detach_irq;
	}

	TAILQ_INSERT_TAIL(&gQnxIRQQueue, lpIrq, mTail);

detach_irq:
	if (lRet != 0) {
		(void) InterruptDetach(lpIrq->mIrqId);
	}

destroy_conn:
	if (lRet != 0) {
		(void)ConnectDetach(lpIrq->mConid);
		(void)ChannelDestroy(lpIrq->mChannId);
	}

release_pool_elem:
	if (lRet != 0) {
		(void) OAL_RELEASE_ELEM_FROM_POOL(&gsIRQPool, gsProcessIRQS, lpIrq);
	}

set_irq_exit:
	return lRet;
}

int32_t OAL_FreeIrq(int32_t aIrqNum)
{
	int32_t lRet = -EINVAL, lStatus;
	struct _qnx_irq *lpIrq;

	TAILQ_FOREACH (lpIrq, &gQnxIRQQueue, mTail) {
		if (lpIrq->mIrqNum != aIrqNum) {
			continue;
		}

		lRet = MsgSendPulse(lpIrq->mConid, IRQ_THREAD_PRIO,
		                    IRQ_EXIT_CODE, 0);
		if (lRet == -1) {
			OAL_LOG_ERROR("MsgSendPulse failed: %s\n",
			              strerror(errno));
		} else {
			lRet = pthread_join(lpIrq->mThread, NULL);
			if (lRet != 0) {
				OAL_LOG_ERROR("pthread_join failed with %d code\n",
					      lRet);
			}
		}

		TAILQ_REMOVE(&gQnxIRQQueue, lpIrq, mTail);
		lStatus = OAL_RELEASE_ELEM_FROM_POOL(&gsIRQPool, gsProcessIRQS,
						  lpIrq);
		if (lStatus != 0) {
			lRet = -1;
		}
		break;
	}

	return lRet;
}
