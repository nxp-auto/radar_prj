/*
 * Copyright 2020-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr_service.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <oal_comm_kernel.h>
#include <oal_log.h>

static K_THREAD_STACK_ARRAY_DEFINE(gsZephyrStacks,
								   OAL_MAX_SERVICES_PER_DRIVER,
                				   OAL_ZEPHYR_THREAD_STACK_SIZE);
static OAL_DECLARE_STATIC_POOL(gsZephyrStacksPool,
							   OAL_ARRAY_SIZE(gsZephyrStacks));

static struct k_thread gsZephyrThreads[OAL_MAX_SERVICES_PER_DRIVER];
static OAL_DECLARE_STATIC_POOL(gsZephyrThreadsPool,
							   OAL_ARRAY_SIZE(gsZephyrThreads));

static struct k_mbox gsZephyrMboxes[OAL_MAX_SERVICES_PER_DRIVER];
static OAL_DECLARE_STATIC_POOL(gsZephyrMboxesPool,
							   OAL_ARRAY_SIZE(gsZephyrMboxes));

static int32_t consume_msg(struct k_mbox_msg aMsg, struct comm_args aMsgData,
						   zephyr_service_t aZServ)
{
	int32_t lRet;

	if (OAL_RPC_CANCEL == aMsg.info) {
		lRet = OAL_RPC_CANCEL;
	}
	else if (OAL_RPC_MSG == aMsg.info) {
		lRet = aZServ->mCallback(aMsg, aZServ->mpMailbox, aMsgData,
								 aMsg.rx_source_thread, aZServ->mServ);
	}
	else {
		OAL_LOG_ERROR("Unknown message info: %d\n", aMsg.info);
		lRet = -1;
	}

	return lRet;
}

static void k_thread_service(void *apArg1, void *apArg2, void *apArg3)
{
	OAL_UNUSED_ARG(apArg3);
	zephyr_service_t lZServ = (zephyr_service_t)apArg1;
	int32_t lStatus 		= 0;
	struct k_mbox_msg lRecvMsg;
	struct comm_args lMsgData;

    while (1) {
		lRecvMsg.size = OAL_MSG_DATA_MAX_SIZE;
		lRecvMsg.rx_source_thread = K_ANY;

		k_mbox_get(lZServ->mpMailbox, &lRecvMsg, &lMsgData, K_FOREVER);

		if (lRecvMsg.size > OAL_MSG_DATA_MAX_SIZE) {
			OAL_LOG_ERROR(
				"Please check the sending message buffer size, "
				"exceeded %ld bytes\n", lRecvMsg.size - OAL_MSG_DATA_MAX_SIZE);
			break;
        }

        lStatus = consume_msg(lRecvMsg, lMsgData, lZServ);
        if (-1 == lStatus) {
        	OAL_LOG_ERROR("Failed to consume unknown message\n");
		}
		else if (OAL_RPC_CANCEL == lStatus) {
			OAL_LOG_ERROR("Service is canceled\n");
			break;
		}
	}

	k_thread_abort(k_current_get());
}

int32_t OAL_ZephyrMboxRegister(zephyr_service_t aZServ, 
							   zephyr_serv_callback_t aCallback, 
							   OAL_RPCService_t aServ)
{
	int32_t lRet = 0;
	k_thread_stack_t *lpStack = NULL;
	struct k_thread *lpThread = NULL;
	struct k_mbox *lpMBox 	  = NULL;
	zephyr_service_t lZserv   = NULL;

	if (NULL == aZServ || NULL == aCallback || NULL == aServ) {
		lRet = -EINVAL;
		goto mbox_register_exit;
	}

	lZserv = aZServ;

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&gsZephyrMboxesPool, gsZephyrMboxes,
									&lpMBox);
	if ((0 != lRet) || (NULL == lpMBox)) {
		OAL_LOG_ERROR(
		    "There are no more available positions "
		    "in zephyr stack pool. Please adjust its size.");
		goto mbox_register_exit;
	}
	k_mbox_init(lpMBox);

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&gsZephyrStacksPool, gsZephyrStacks,
									&lpStack);
	if ((0 != lRet) || (NULL == lpStack)) {
		OAL_LOG_ERROR(
		    "There are no more available positions "
		    "in zephyr stack pool. Please adjust its size.");
		goto mbox_register_exit;
	}
	lZserv->mpStack = lpStack;

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&gsZephyrThreadsPool, gsZephyrThreads,
									&lpThread);
	if ((0 != lRet) || (NULL == lpThread)) {
		OAL_LOG_ERROR(
		    "There are no more available positions "
		    "in zephyr thread pool. Please adjust its size.\n");
		goto mbox_register_exit;
	}

	lZserv->mpMailbox = lpMBox;
	lZserv->mpThread  = lpThread;
	lZserv->mCallback = aCallback;
	lZserv->mServ 	  = aServ;

	/* Create a RPC thread service */
	lZserv->mServerID = k_thread_create(lpThread, lpStack,
                                        OAL_ZEPHYR_THREAD_STACK_SIZE,
                                        k_thread_service,
                                        lZserv, NULL, NULL,
                                        OAL_ZEPHYR_THREAD_PRIORITY,
                                        OAL_ZEPHYR_THREAD_OPTIONS,
                                        OAL_ZEPHYR_THREAD_TIME_OUT);

mbox_register_exit:
	if (NULL != lpMBox) {
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsZephyrMboxesPool,
									     gsZephyrMboxes, lpMBox);
	}

	if (NULL != lpStack) {
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsZephyrStacksPool,
									     gsZephyrStacks, lpStack);
	}

	if (NULL != lpThread) {
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsZephyrThreadsPool,
									     gsZephyrThreads, lpThread);
	}

	return lRet;
}

int32_t OAL_ZephyrAbortServer(zephyr_service_t aZServ)
{
	int32_t lRet = 0;
	
	if (NULL == aZServ) {
		lRet = -EINVAL;
	}
	else {
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsZephyrStacksPool, 
									     gsZephyrStacks, aZServ->mpStack);
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsZephyrThreadsPool, 
									     gsZephyrThreads, aZServ->mpThread);
		k_thread_abort(aZServ->mServerID);
	}

	return lRet;
}
