/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <atomic.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <sys/iofunc.h>

#include <oal_log.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <oal_timespec.h>
#include <qnx_service.h>

#define OAL_MAX_NAME_LEN 50
#define EXIT (0xFFU)

struct qnx_service {
	char8_t mName[OAL_MAX_NAME_LEN];
	void *mpData;
	pthread_t mThread;
	qnx_serv_callback_t mCallback;
	qnx_serv_callback_t mNotifications;
	name_attach_t *mpAttach;
	uint32_t mFlags;
	volatile uint8_t mThreadFinished;
};

static qnx_service_t gsProcessServices[OAL_QNX_MAX_SERVICES_PER_PROCESS];
static OAL_DECLARE_STATIC_POOL(gsServicesPool,
                               OAL_ARRAY_SIZE(gsProcessServices));

static int32_t consume_message(struct _msg_info *apInfo, int32_t aRcvid,
                               serv_msg_t *apMsg, size_t *apOffset,
                               const qnx_service_t *acpQnxServ)
{
	int32_t lStatus = 0;
	int32_t lRet;

	if ((aRcvid == 0) && (apMsg->mPulse.type == _IO_CONNECT)) {
		lRet = MsgReply(aRcvid, EOK, NULL, 0);

		if (lRet == -1) {
			OAL_LOG_ERROR("MsgReply failed: %s\n", strerror(errno));
			lStatus = EINVAL;
		}
		goto consume_mesage_exit;
	}

	/* Pulse */
	if (aRcvid == 0) {
		if (acpQnxServ->mNotifications != NULL) {
			lStatus = acpQnxServ->mNotifications(
			    apInfo, aRcvid, apMsg, apOffset,
			    acpQnxServ->mpData);
		} else {
			lStatus = -EINVAL;
		}
		goto consume_mesage_exit;
	}

	if (apMsg->mMsgType == EXIT) {
		lStatus = (int32_t)EXIT;
	} else {
		lStatus = acpQnxServ->mCallback(apInfo, aRcvid, apMsg, apOffset,
		                                acpQnxServ->mpData);
	}

consume_mesage_exit:
	return lStatus;
}

static int32_t oal_name_detach(qnx_service_t *apQnxServ)
{
	int32_t lRet = 0;
	static uint32_t lsLock;

	/* Guard below block */
	while (atomic_add_value(&lsLock, 0U) != 0U) {
	};
	(void)atomic_set_value(&lsLock, 1U);

	if (apQnxServ->mpAttach != NULL) {
		lRet = name_detach(apQnxServ->mpAttach, 0);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to detach : %s\n",
			              strerror(errno));
		} else {
			apQnxServ->mpAttach = NULL;
		}
	}
	(void)atomic_clr_value(&lsLock, 1U);

	return lRet;
}

static int32_t oal_name_attach(qnx_service_t *apQnxServ, const char8_t *acpName)
{
	int32_t lRet      = 0, lStatus;
	dispatch_t *lpDpp = NULL;
	int32_t lChanID   = -1;

	apQnxServ->mpAttach = NULL;

	lChanID = ChannelCreate_r(_NTO_CHF_DISCONNECT);
	if (lChanID < 0) {
		OAL_LOG_ERROR("Failed to create a channel: %s\n",
		              strerror(-lChanID));
		lRet = -lChanID;
		goto name_attach_exit;
	}

	lpDpp = dispatch_create_channel(lChanID, 0);
	if (lpDpp == NULL) {
		OAL_LOG_ERROR("Failed to create dispatcher\n");
		lRet = ENOMEM;
		goto destroy_channel;
	}

	apQnxServ->mpAttach = name_attach(lpDpp, acpName, 0);
	if (apQnxServ->mpAttach == NULL) {
		OAL_LOG_ERROR("name_attach failed: %s\n", strerror(errno));
		lRet = -EINVAL;
	}

	if (lRet != 0) {
		lStatus = dispatch_destroy(lpDpp);
		if (lStatus != 0) {
			OAL_LOG_ERROR("Failed to destroy dispatch: %s\n",
			              strerror(errno));
			lRet = -errno;
		}
	}

destroy_channel:
	if ((lRet != 0) && (lChanID > 0)) {
		lStatus = ChannelDestroy_r(lChanID);
		if (lStatus != 0) {
			OAL_LOG_ERROR("Failed to destroy channel: %s\n",
			              strerror(-lStatus));
			lRet = lStatus;
		}
	}

name_attach_exit:
	return lRet;
}

static void *services_thread(void *apArg)
{
	qnx_service_t *lpQnxServ = apArg;
	serv_msg_t lMsg;
	struct _msg_info lInfo;
	int32_t lRcvid;
	int32_t lRet;
	int32_t lStatus;
	size_t lOffset;

	lpQnxServ->mThreadFinished = 0U;
	while (0U == 0U) {
		lRcvid = MsgReceive_r(lpQnxServ->mpAttach->chid, &lMsg,
		                      sizeof(lMsg), &lInfo);

		if (lRcvid < 0) {
			OAL_LOG_ERROR("MsgReceive failed: %s\n",
			              strerror(-lRcvid));
			if ((lRcvid == -EINTR) || (lRcvid == -ETIMEDOUT)) {
				continue;
			} else {
				lRet = lRcvid;
				break;
			}
		}

		lOffset = sizeof(lMsg);

		lStatus =
		    consume_message(&lInfo, lRcvid, &lMsg, &lOffset, lpQnxServ);
		// Pulses
		if (lRcvid == 0) {
			continue;
		}

		if (lStatus != 0) {
			if (lStatus == (int32_t)EXIT) {
				lRet = MsgReply(lRcvid, EOK, &lStatus,
				                sizeof(lStatus));
				if (lRet == 0) {
					lRet = lRcvid;
				}
				OAL_LOG_NOTE("Closing services thread\n");
				break;
			}

			lRet = MsgError(lRcvid, ENOSYS);
			if (lRet == -1) {
				OAL_LOG_ERROR("MsgError failed: %s\n",
				              strerror(errno));
			}
		} else {
			if ((lpQnxServ->mFlags &
			     OAL_QNX_SERV_NO_DEFAULT_REPLY) == 0U) {
				lRet = MsgReply(lRcvid, EOK, &lStatus,
				                sizeof(lStatus));
				if (lRet == -1) {
					OAL_LOG_ERROR("MsgReply failed: %s\n",
					              strerror(errno));
				}
			}
		}
	}

	lStatus = oal_name_detach(lpQnxServ);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to detach: %s\n", strerror(errno));
		lRet = lStatus;
	}
	lpQnxServ->mThreadFinished = 1U;
	pthread_exit((void *)(uintptr_t)lRet);
}

void OAL_QnxServAddNotifications(qnx_service_t *apServ,
                                 qnx_serv_callback_t aCallback)
{
	if (apServ != NULL) {
		apServ->mNotifications = aCallback;
	}
}

void OAL_QnxServAddData(qnx_service_t *apServ, void *apData)
{
	if (apServ != NULL) {
		apServ->mpData = apData;
	}
}

int32_t OAL_QnxServStart(qnx_service_t **apServ, const char8_t *acpName,
                         qnx_serv_callback_t aCallback, uint32_t aFlags)
{
	qnx_service_t *lpQnxServ;
	int32_t lRet = 0, lStatus;

	if ((apServ == NULL) || (acpName == NULL) || (aCallback == NULL)) {
		lRet = -EINVAL;
		goto serv_start_exit;
	}

	*apServ = NULL;
	lRet    = OAL_ALLOC_ELEM_FROM_POOL(&gsServicesPool, gsProcessServices,
                                        apServ);
	if ((lRet != 0) || (*apServ == NULL)) {
		lRet = -ENOMEM;
		OAL_LOG_ERROR(
		    "There are no more available positions "
		    "in QNX services pool. Please adjust its size.");
		goto serv_start_exit;
	}

	lpQnxServ = *apServ;

	/* Fill service's name */
	lpQnxServ->mName[sizeof(lpQnxServ->mName) - 1U] = (char8_t)(0);
	(void)strncpy(lpQnxServ->mName, acpName, sizeof(lpQnxServ->mName) - 1U);

	lpQnxServ->mCallback      = aCallback;
	lpQnxServ->mNotifications = NULL;
	lpQnxServ->mFlags         = aFlags;

	/*
	 * Below block is an equivalent of name_attach with
	 * _NTO_CHF_DISCONNECT flag
	 */
	lRet = oal_name_attach(lpQnxServ, acpName);
	if (lRet != 0) {
		OAL_LOG_ERROR("name_attach failed: %s\n", strerror(errno));
		lRet = -EINVAL;
		goto free_serv;
	}

	lRet = pthread_create(&lpQnxServ->mThread, NULL, services_thread,
	                      lpQnxServ);
	if (lRet != 0) {
		OAL_LOG_ERROR("pthread_create failed with %d code\n", lRet);
		lRet = -EIO;
		goto serv_detach;
	}

	lRet = 0;
	goto serv_start_exit;

serv_detach:
	lStatus = oal_name_detach(lpQnxServ);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to detach : %s\n", strerror(errno));
		lRet = lStatus;
		goto serv_start_exit;
	}
free_serv:
	lStatus = OAL_RELEASE_ELEM_FROM_POOL(&gsServicesPool, gsProcessServices,
	                                     lpQnxServ);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to release element from pool\n");
		lRet = lStatus;
	}

	*apServ = NULL;
serv_start_exit:
	return lRet;
}

int32_t OAL_QnxServStop(qnx_service_t *apServ)
{
	serv_msg_t lMsg;
	int64_t lStatus;
	int32_t lCoid;
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apServ, serv_stop_exit);

	if (apServ->mThreadFinished != 0U) {
		goto join_thread_label;
	}

	lCoid = name_open(apServ->mName, 0);
	if (lCoid == -1) {
		OAL_LOG_ERROR("name_open failed: %s\n", strerror(errno));
		lRet = -EINVAL;
		goto detach_file;
	} else {
		lMsg.mMsgType = EXIT;

		/*
		 * Interrupt MsgSend if the thread doesn't reply
		 * under a second
		 */
		lRet = OAL_QNX_TIMEOUT(_NTO_TIMEOUT_SEND | _NTO_TIMEOUT_REPLY,
		                OAL_NSEC_IN_SEC);
		if (lRet == -1) {
			OAL_LOG_ERROR("Failed to set timeout\n");
			goto detach_file;
		} else {
			lStatus = MsgSend(lCoid, &lMsg, sizeof(lMsg), NULL, 0);
			if (lStatus == -1) {
				OAL_LOG_ERROR("MsgSend failed: %s\n",
					      strerror(errno));
				lRet = (int32_t)lStatus;
			}
		}

		lStatus = name_close(lCoid);
		if (lStatus == -1) {
			lRet = (int32_t)lStatus;
			OAL_LOG_ERROR("name_close failed: %s\n",
			              strerror(errno));
		}
	}

join_thread_label:
	lStatus = pthread_join(apServ->mThread, NULL);
	if (lStatus != 0) {
		if (lRet == 0) {
			lRet = (int32_t)lStatus;
		}
		OAL_LOG_ERROR("pthread_join failed\n");
	}

detach_file:
	/* If detach from initialization failed */
	lStatus = oal_name_detach(apServ);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to detach : %s\n", strerror(errno));
		lRet = (int32_t)lStatus;
	}

	(void)OAL_RELEASE_ELEM_FROM_POOL(&gsServicesPool, gsProcessServices,
	                                 apServ);
serv_stop_exit:
	return lRet;
}
