/*
 * Copyright 2017-2019, 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_comm_kernel.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <os_oal_comm.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <net/genetlink.h>

#define OAL_SERVER_MAX_NAME 15U

struct oal_dispatcher {
	struct sk_buff *mpRskb;
	OAL_RPCService_t mService;
};

struct OAL_RPCService {
	struct genl_family mFam;
	struct genl_ops mOps;
	OAL_dispatch_t mDispatch;
	OAL_ServiceData_t mData;
};

struct OAL_ReferDispatcher {
	uint8_t mUsedDispatch;
	char8_t mName[OAL_SERVER_MAX_NAME];
	struct OAL_RPCService *mSaveMem;
};

static int32_t oal_rpc_exec(struct sk_buff *apSkb2, struct genl_info *apInfo);

static int32_t oal_rpc_pre_exec(const struct genl_ops *acpOps,
                                struct sk_buff *apSkb,
                                struct genl_info *apInfo);

static struct nla_policy gsOalRpcPolicy[OAL_RPC_MAX + 1] = {
    [OAL_RPC_CALL]     = {.type = NLA_NUL_STRING},
    [OAL_RPC_REPLY]    = {.type = NLA_U32},
    [OAL_RPC_FUNC_RET] = {.type = NLA_U32},
};

// Family definition
static struct genl_family gsOalRpcFamily = {
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 10, 0)
    .id = GENL_ID_GENERATE,
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 2, 0)
    .policy = gsOalRpcPolicy,
#endif
    .hdrsize  = 0,
    .name     = "<serv_name>",
    .version  = OAL_RPC_VERSION,
    .maxattr  = OAL_RPC_MAX,
    .pre_doit = oal_rpc_pre_exec,
};

static struct OAL_ReferDispatcher gsDispatch[OAL_MAX_SERVICES_PER_DRIVER];
static struct OAL_RPCService gsDriverServices[OAL_MAX_SERVICES_PER_DRIVER];
static OAL_DECLARE_STATIC_POOL(gsRPCNetServicesPool,
                               OAL_ARRAY_SIZE(gsDriverServices));

int32_t OAL_RPCAppendReply(oal_dispatcher_t *apDisp, uint8_t *apData,
                           size_t aDataSize)
{
	return nla_put(apDisp->mpRskb, (int32_t)OAL_RPC_REPLY,
	               (int32_t)aDataSize, apData);
}

static int32_t process_message(struct nlattr *apNa, struct genl_info *apInfo)
{
	int32_t lRet = 0;
	size_t lLen;
	uint8_t *lpData;
	void *lpMsgHead;
	uint32_t lFunc;
	struct oal_dispatcher lDisp;
	uint32_t lFret;
	struct OAL_RPCService *lpServ = apInfo->user_ptr[0];

	lDisp.mpRskb = NULL;
	lpData       = (uint8_t *)nla_data(apNa);

	if ((lpData == NULL) || (lpServ == NULL) ||
	    (lpServ->mDispatch == NULL)) {
		OAL_LOG_ERROR("error while receiving lpData\n");
		lRet = -EINVAL;
		goto process_message_end;
	}

	// Send a message back
	// Allocate some memory, since the size is not yet known use
	// NLMSG_GOODSIZE
	lDisp.mpRskb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (lDisp.mpRskb == NULL) {
		lRet = -ENOMEM;
		OAL_LOG_ERROR("Failed to allocated a new skb\n");
		goto process_message_end;
	}

	lDisp.mService = lpServ;

	// Create the message headers
	lpMsgHead = genlmsg_put(lDisp.mpRskb, 0, apInfo->snd_seq + 1U,
	                        &gsOalRpcFamily, 0, OAL_RPC_CMD_EXEC);
	if (lpMsgHead == NULL) {
		lRet = -ENOMEM;
		goto free_skb;
	}

	lFunc = nla_get_u32(apNa);
	lLen  = (size_t)nla_len(apNa);

	lFret = lpServ->mDispatch(
	    &lDisp, lFunc,
	    (uintptr_t)(lpData + sizeof(lFunc) + OAL_SERVER_MAX_NAME),
	    (lLen - sizeof(lFunc) - OAL_SERVER_MAX_NAME -
	     strlen(lpServ->mFam.name) - 1U));

	// Send back the return code
	lRet = nla_put_u32(lDisp.mpRskb, (int32_t)OAL_RPC_FUNC_RET, lFret);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to append return code\n");
		goto free_skb;
	}

	// Finalize the message
	genlmsg_end(lDisp.mpRskb, lpMsgHead);

	// Send the message back
	lRet = genlmsg_unicast(genl_info_net(apInfo), lDisp.mpRskb,
	                       apInfo->snd_portid);

free_skb:
	if (lRet != 0) {
		nlmsg_free(lDisp.mpRskb);
	}

process_message_end:
	return lRet;
}

static int32_t oal_rpc_pre_exec(const struct genl_ops *acpOps,
                                struct sk_buff *apSkb, struct genl_info *apInfo)
{
	int32_t lRet                  = EINVAL;
	struct OAL_RPCService *lpServ = NULL;
	struct nlattr *lpNa;
	uint32_t lFunc;
	uint8_t lI;
	uint8_t *lpData;
	const char8_t *lName;

	OAL_UNUSED_ARG(apSkb);
	if ((acpOps != NULL) && (apInfo != NULL)) {
		lpNa = apInfo->attrs[OAL_RPC_CALL];

		if (lpNa != NULL) {
			lFunc  = nla_get_u32(lpNa);
			lpData = (uint8_t *)nla_data(lpNa);
			lName  = (char *)(lpData + sizeof(lFunc));

			for (lI = 0; lI < OAL_MAX_SERVICES_PER_DRIVER; lI++) {
				if (gsDispatch[lI].mUsedDispatch !=
				        (uint8_t)0 &&
				    (strncmp(gsDispatch[lI].mName, lName,
				             (strlen(gsDispatch[lI].mName) +
				              1U))) == 0) {
					lpServ = gsDispatch[lI].mSaveMem;
					break;
				}
			}

			apInfo->user_ptr[0] = lpServ;
			lRet                = 0;
		}
	}
	return lRet;
}

static int32_t oal_rpc_exec(struct sk_buff *apSkb2, struct genl_info *apInfo)
{
	struct nlattr *lpNa;
	int32_t lRet = 0;

	OAL_UNUSED_ARG(apSkb2);
	if (apInfo == NULL) {
		lRet = -EINVAL;
		goto out;
	}

	lpNa = apInfo->attrs[OAL_RPC_CALL];
	if (lpNa != NULL) {
		lRet = process_message(lpNa, apInfo);
	} else {
		(void)printk("no apInfo->attrs %i\n", OAL_RPC_CALL);
	}

out:
	return lRet;
}

OAL_RPCService_t OAL_RPCRegister(const char8_t *acpName, OAL_dispatch_t aDisp)
{
	int32_t lRet;
	struct OAL_RPCService *lpServ = NULL;
	size_t lLen, lI;

	static struct genl_ops lsOalRpcOps = {
		.cmd   = OAL_RPC_CMD_EXEC,
		.flags = 0,
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 2, 0)
		.policy = gsOalRpcPolicy,
#endif
		.doit   = oal_rpc_exec,
		.dumpit = NULL,
	};

	if (aDisp == NULL) {
		goto register_end;
	}

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&gsRPCNetServicesPool, gsDriverServices,
	                                &lpServ);
	if ((lRet != 0) || (lpServ == NULL)) {
		OAL_LOG_ERROR(
		    "There are no more available positions "
		    "in driver's services pool. Please adjust its size.");
		goto register_end;
	}

	lpServ->mOps      = lsOalRpcOps;
	lpServ->mDispatch = aDisp;
	lpServ->mFam      = gsOalRpcFamily;

	lLen = (size_t)OAL_Min64u((uint64_t)GENL_NAMSIZ,
	                          (uint64_t)(strlen(acpName) + 1U));

	(void)memcpy(lpServ->mFam.name, acpName, lLen);
	lpServ->mFam.ops   = &lpServ->mOps;
	lpServ->mFam.n_ops = 1;

	for (lI = 0; lI < OAL_MAX_SERVICES_PER_DRIVER; lI++) {
		if (gsDispatch[lI].mUsedDispatch == (uint8_t)0) {
			gsDispatch[lI].mUsedDispatch = 1;
			gsDispatch[lI].mSaveMem      = lpServ;
			(void)strncpy(gsDispatch[lI].mName, acpName, lLen);
			break;
		}
	}

	// Register the new family
	lRet = genl_register_family(&lpServ->mFam);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to register family with code: %d\n",
		              lRet);
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsRPCNetServicesPool,
		                                 gsDriverServices, lpServ);
		lpServ = NULL;
	}

register_end:
	return lpServ;
}

int32_t OAL_RPCSetPrivateData(OAL_RPCService_t aServ, OAL_ServiceData_t aData)
{
	int32_t lRet = 0;
	if (aServ == NULL) {
		lRet = -EINVAL;
	} else {
		aServ->mData = aData;
	}

	return lRet;
}

int32_t OAL_RPCGetPrivateData(OAL_RPCService_t aServ, OAL_ServiceData_t *apData)
{
	int32_t lRet = 0;
	if ((aServ == NULL) || (apData == NULL)) {
		lRet = -EINVAL;
	} else {
		*apData = aServ->mData;
	}

	return lRet;
}

OAL_RPCService_t OAL_RPCGetService(oal_dispatcher_t *apDisp)
{
	OAL_RPCService_t lServ = NULL;
	if (apDisp != NULL) {
		lServ = apDisp->mService;
	}

	return lServ;
}

int32_t OAL_RPCCleanup(const OAL_RPCService_t acServ)
{
	struct OAL_RPCService *lpServ = acServ;
	int32_t lRet                  = 0;

	if (lpServ != NULL) {
		lRet = genl_unregister_family(&lpServ->mFam);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to unregister family %i\n", lRet);
		}

		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsRPCNetServicesPool,
		                                 gsDriverServices, lpServ);
	} else {
		lRet = -1;
	}

	return lRet;
}

int32_t OAL_RPCGetClientPID(oal_dispatcher_t *apDispatch, pid_t *apClientPID)
{
	int32_t lRet = 0;

	if ((apDispatch == NULL) || (apClientPID == NULL)) {
		lRet = -EINVAL;
	} else {
		*apClientPID = task_pgrp_nr(current);
	}

	return lRet;
}
