/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_comm.h>
#include <oal_log.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <oal_utils.h>
#include <os_oal_comm.h>

#define MAX_LEN_FAMILY (15U)

static struct OS_OAL_DriverHandle
    gsDriverHandles[OAL_MAX_DRIVER_HANDLES_PER_PROCESS];
static OAL_DECLARE_STATIC_POOL(gsDriverHandlesPool,
                               OAL_ARRAY_SIZE(gsDriverHandles));

static inline int32_t socket_send(int32_t aSockfd, const uint8_t *acpBuf,
                                  size_t aLen, int32_t aFlags)
{
	int32_t lRet = 0;
	struct sockaddr_nl lNlAddress;

	(void)memset(&lNlAddress, 0, sizeof(lNlAddress));
	lNlAddress.nl_family = AF_NETLINK;

	do {
		lRet =
		    sendto(aSockfd, acpBuf, aLen, aFlags,
		           (struct sockaddr *)&lNlAddress, sizeof(lNlAddress));
		if ((lRet < 0) && (errno != EAGAIN)) {
			break;
		} else {
			acpBuf += lRet;
			aLen -= (size_t)lRet;
			lRet = 0;
		}
	} while (((size_t)lRet) < aLen);

	return lRet;
}

OAL_DriverHandle_t OAL_OpenDriver(const char8_t *acpChannelName)
{
	OAL_DriverHandle_t lHandle = NULL;
	struct nlattr *lpNlattrs   = NULL;
	// Family lName length can be upto 16 chars including \0
	char8_t lName[MAX_LEN_FAMILY];
	int32_t lRet;
	size_t lNameLen;
	ssize_t lSendRecvSize;
	struct sockaddr_nl lNlAddress;

	if (acpChannelName == NULL) {
		goto open_driver_exit;
	}

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&gsDriverHandlesPool, gsDriverHandles,
	                                &lHandle);
	if ((lRet != 0) || (lHandle == NULL)) {
		OAL_LOG_ERROR(
		    "There are no more available positions "
		    "in driver handles pool. Please adjust its size.\n");
		goto open_driver_exit;
	}

	(void)memset(lHandle, 0, sizeof(*lHandle));
	(void)memcpy(lName, acpChannelName,
	             strnlen(acpChannelName, MAX_LEN_FAMILY - 1U) + 1U);
	lName[MAX_LEN_FAMILY - 1] = (char8_t)0;

	lNameLen     = strlen(lName);
	lHandle->mFd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
	if (lHandle->mFd < 0) {
		OAL_LOG_ERROR("socket(): %s\n", strerror(errno));
		goto free_mem;
	}

	// Bind the socket.
	(void)memset(&lNlAddress, 0, sizeof(lNlAddress));
	lNlAddress.nl_family = AF_NETLINK;
	lNlAddress.nl_groups = 0U;

	lRet = connect(lHandle->mFd, (struct sockaddr *)&lNlAddress,
	               sizeof(lNlAddress));
	if (lRet < 0) {
		OAL_LOG_ERROR("Connection to kernel driver failed: %s\n",
		              strerror(errno));
		goto close_socket;
	}

	// Populate the netlink header
	lHandle->mSendBuffer.mNlmHdr.nlmsg_type  = GENL_ID_CTRL;
	lHandle->mSendBuffer.mNlmHdr.nlmsg_flags = NLM_F_REQUEST;
	lHandle->mSendBuffer.mNlmHdr.nlmsg_seq   = 0U;
	lHandle->mSendBuffer.mNlmHdr.nlmsg_pid   = getpid();
	lHandle->mSendBuffer.mNlmHdr.nlmsg_len   = NLMSG_LENGTH(GENL_HDRLEN);

	// Populate the payload's "family header" : which in our case is
	// genlmsghdr
	lHandle->mSendBuffer.mGenHdr.cmd     = CTRL_CMD_GETFAMILY;
	lHandle->mSendBuffer.mGenHdr.version = 0x1;

	// Populate the payload's "netlink attributes"
	lpNlattrs           = OAL_GetNlattr(&lHandle->mSendBuffer);
	lpNlattrs->nla_type = CTRL_ATTR_FAMILY_NAME;
	lpNlattrs->nla_len  = lNameLen + 1U + NLA_HDRLEN;
	strncpy(OAL_GetNlaData(lpNlattrs), lName, lNameLen);

	lHandle->mSendBuffer.mNlmHdr.nlmsg_len +=
	    NLMSG_ALIGN(lpNlattrs->nla_len);

	// Send the family ID request message to the netlink controller
	lRet = socket_send(lHandle->mFd, (uint8_t *)&lHandle->mSendBuffer,
	                   lHandle->mSendBuffer.mNlmHdr.nlmsg_len, 0);

	if (lRet != 0) {
		OAL_LOG_ERROR("sendto(): %s\n", strerror(errno));
		close(lHandle->mFd);
		goto close_socket;
	}

	// Wait for the response message
	lSendRecvSize = recv(lHandle->mFd, &lHandle->mRecvBuffer,
	                     sizeof(lHandle->mRecvBuffer), 0);
	if (lSendRecvSize < 0) {
		OAL_LOG_ERROR("recv(): %s\n", strerror(errno));
		goto close_socket;
	}

	// Validate response message
	if (!NLMSG_OK((&lHandle->mRecvBuffer.mNlmHdr), lSendRecvSize)) {
		OAL_LOG_ERROR("family ID request : receive error\n");
		goto close_socket;
	}

	if (lHandle->mRecvBuffer.mNlmHdr.nlmsg_type == NLMSG_ERROR) {
		struct nlmsgerr *lpErr = NLMSG_DATA(&lHandle->mRecvBuffer);
		OAL_LOG_ERROR("family ID request : invalid message (%s)\n",
		              strerror(-lpErr->error));
		goto close_socket;
	}

	// Extract family ID
	lpNlattrs = OAL_GetNlattr(&lHandle->mRecvBuffer);
	lpNlattrs = (struct nlattr *)((uint8_t *)lpNlattrs +
	                              NLA_ALIGN(lpNlattrs->nla_len));
	if (lpNlattrs->nla_type == CTRL_ATTR_FAMILY_ID) {
		lHandle->mFamilyId = *(uint16_t *)OAL_GetNlaData(lpNlattrs);
	} else {
		lHandle->mFamilyId = 0;
	}

	goto open_driver_exit;

close_socket:
	close(lHandle->mFd);
free_mem:
	(void)OAL_RELEASE_ELEM_FROM_POOL(&gsDriverHandlesPool, gsDriverHandles,
	                                 lHandle);
	lHandle = NULL;
open_driver_exit:
	return lHandle;
}

int32_t OAL_CloseDriver(OAL_DriverHandle_t *apHandle)
{
	int32_t lRet = -1;

	if (apHandle != NULL) {
		struct OS_OAL_DriverHandle *lpHandle = *apHandle;
		if (lpHandle != NULL) {
			(void)close(lpHandle->mFd);
			(void)OAL_RELEASE_ELEM_FROM_POOL(
			    &gsDriverHandlesPool, gsDriverHandles,
			    lpHandle);
		}
		*apHandle = NULL;
		lRet      = 0;
	}

	return lRet;
}

static inline size_t get_size(OAL_FuncArgs_t *apArgs, size_t aLen)
{
	size_t lIdx;
	size_t lSize = 0U;
	if (apArgs != NULL) {
		for (lIdx = 0U; lIdx < aLen; lIdx++) {
			lSize += apArgs[lIdx].mSize;
		}
	}

	return lSize;
}

static inline void reset_buffer(struct OS_OAL_SocketBuffer *apBuff)
{
	(void)memset(&apBuff->mNlmHdr, 0, sizeof(apBuff->mNlmHdr));
	(void)memset(&apBuff->mGenHdr, 0, sizeof(apBuff->mGenHdr));
}

#define SEQ_NUMBER (33U)

static inline void init_buffers(OAL_DriverHandle_t aHandle)
{
	reset_buffer(&aHandle->mSendBuffer);
	reset_buffer(&aHandle->mRecvBuffer);

	aHandle->mSendBuffer.mNlmHdr.nlmsg_len   = NLMSG_LENGTH(GENL_HDRLEN);
	aHandle->mSendBuffer.mNlmHdr.nlmsg_type  = (uint16_t)aHandle->mFamilyId;
	aHandle->mSendBuffer.mNlmHdr.nlmsg_flags = NLM_F_REQUEST;
	aHandle->mSendBuffer.mNlmHdr.nlmsg_seq   = SEQ_NUMBER;
	aHandle->mSendBuffer.mNlmHdr.nlmsg_pid   = (uint32_t)getpid();
	aHandle->mSendBuffer.mGenHdr.cmd         = OAL_RPC_CMD_EXEC;
}

/* Only if logging is enabled */
#ifndef OAL_LOG_SUPPRESS_DEBUG
static inline void dump_nlmsghdr(const struct nlmsghdr *acpHandle)
{
	OAL_DUMP("%d", acpHandle->nlmsg_len);
	OAL_DUMP("%d", acpHandle->nlmsg_type);
	OAL_DUMP("%d", acpHandle->nlmsg_flags);
	OAL_DUMP("%d", acpHandle->nlmsg_seq);
	OAL_DUMP("%d", acpHandle->nlmsg_pid);
}

static inline void dump_nlattrs(const struct nlattr *acpAttr)
{
	OAL_DUMP("%d", acpAttr->nla_len);
	OAL_DUMP("%d", acpAttr->nla_type);
}
#endif

static inline int32_t unmarshalling(struct nlattr *apAttr,
                                    OAL_FuncArgs_t *apOutArg)
{
	int32_t lRet = 0;

	if (OAL_GetNlaLen(apAttr) != (int32_t)apOutArg->mSize) {
		OAL_LOG_ERROR(
		    "Incompatibility : expected %zu and received %d\n",
		    apOutArg->mSize, OAL_GetNlaLen(apAttr));
		lRet = -1;
	} else {
		(void)memcpy(apOutArg->mpData, OAL_GetNlaData(apAttr),
		             apOutArg->mSize);
	}

	return lRet;
}

static uint32_t receive_data(OAL_DriverHandle_t aHandle,
                             OAL_FuncArgs_t *apOutArgs, size_t aNumOutArgs)
{
	int32_t lRet;
	uint32_t lFret           = (uint32_t)-1;
	struct nlattr *lpNlattrs = NULL;
	intptr_t lPayload;
	ssize_t lNlattrLen;
	int32_t lArgId = 0;

	// Receive reply from kernel
	ssize_t lRecvSize = recv(aHandle->mFd, &aHandle->mRecvBuffer,
	                         sizeof(aHandle->mRecvBuffer), 0);
	if (lRecvSize < 0) {
		OAL_LOG_ERROR("recv(): %s\n", strerror(errno));
		goto receive_data_end;
	}

	// Validate response message
	if (aHandle->mRecvBuffer.mNlmHdr.nlmsg_type == NLMSG_ERROR) {
		OAL_LOG_ERROR("NACK Received\n");
		goto receive_data_end;
	}

	if (!NLMSG_OK((&aHandle->mRecvBuffer.mNlmHdr), lRecvSize)) {
		OAL_LOG_ERROR("Invalid Message\n");
		goto receive_data_end;
	}

	lRecvSize = OAL_GetNlMsgPayload(&aHandle->mRecvBuffer.mNlmHdr);
	lPayload  = (intptr_t)OAL_GetNlattr(&aHandle->mRecvBuffer);

	if (lPayload == 0) {
		OAL_LOG_ERROR("Failed to get nl attributes\n");
		goto receive_data_end;
	}

	/* At least the return value + corresponding attributes */
	if ((ssize_t)lRecvSize < ((ssize_t)sizeof(lFret)) +
	    ((ssize_t)sizeof(*lpNlattrs))) {
		    OAL_LOG_ERROR(
		    "Failed to receive function return "
		    "lRecvSize = %zd\n",
		    lRecvSize);
		goto receive_data_end;
	}

	while (lRecvSize > 0) {
		lpNlattrs = (struct nlattr *)lPayload;

		lNlattrLen = (ssize_t)sizeof(*lpNlattrs);
		lNlattrLen += (ssize_t)OAL_GetNlaLen(lpNlattrs);

		lRecvSize -= lNlattrLen;
		lPayload += lNlattrLen;

		if (lpNlattrs->nla_type == OAL_RPC_FUNC_RET) {
			lFret = *(uint32_t *)OAL_GetNlaData(lpNlattrs);
			continue;
		}
		if ((lpNlattrs->nla_type == OAL_RPC_REPLY) &&
		    (apOutArgs != NULL) && (aNumOutArgs != 0U) &&
		    ((ssize_t)lArgId < (ssize_t)aNumOutArgs)) {
			lRet = unmarshalling(lpNlattrs, &apOutArgs[lArgId]);
			if (lRet != 0) {
				OAL_LOG_ERROR(
				    "Failed to unmarshalling "
				    "received packets\n");
			}

			lArgId++;
			continue;
		}
	}

receive_data_end:
	return lFret;
}

static inline void marshalling(uint8_t *apDest, uint32_t aFuncID,
                               OAL_FuncArgs_t *apInArgs, size_t aNumInArgs)
{
	size_t lIdx;
	uint8_t *lpDest  = apDest;
	void *lpVoidDest = (void *)lpDest;
	void *lpVoidSrc  = (void *)&aFuncID;

	(void)memcpy(lpVoidDest, lpVoidSrc, sizeof(aFuncID));
	lpDest += sizeof(aFuncID);

	if ((apInArgs != NULL) && (aNumInArgs != 0U)) {
		for (lIdx = 0U; lIdx < aNumInArgs; lIdx++) {
			lpVoidDest = (void *)lpDest;
			lpVoidSrc  = (void *)apInArgs[lIdx].mpData;
			(void)memcpy(lpVoidDest, lpVoidSrc,
			             apInArgs[lIdx].mSize);
			lpDest += apInArgs[lIdx].mSize;
		}
	}
}

uint32_t OAL_DriverCall(OAL_DriverHandle_t aHandle, uint32_t aFuncID,
                        OAL_FuncArgs_t *apInArgs, size_t aNumInArgs,
                        OAL_FuncArgs_t *apOutArgs, size_t aNumOutArgs)
{
	struct nlattr *lpNlattrs = NULL;
	size_t lInSize           = 0U;
	uint32_t lRet            = 0U;
	int32_t lIRet;
	size_t lIdx;

	if (aHandle == NULL) {
		lRet = EINVAL;
		goto driver_call_exit;
	}

	for (lIdx = 0U; lIdx < aNumInArgs; lIdx++) {
		if (apInArgs[lIdx].mpData == NULL) {
			OAL_LOG_ERROR("Invalid argument\n");
			lRet = EINVAL;
			break;
		}
	}

	if (lRet == (uint32_t)EINVAL) {
		goto driver_call_exit;
	}

	init_buffers(aHandle);

	lpNlattrs           = OAL_GetNlattr(&aHandle->mSendBuffer);
	lpNlattrs->nla_type = OAL_RPC_CALL;

	// Message length
	lInSize = get_size(apInArgs, aNumInArgs) + sizeof(aFuncID) + NLA_HDRLEN;

	if (lInSize > OAL_MAX_VAR_VALUE(lpNlattrs->nla_len)) {
		lRet = EINVAL;
		goto driver_call_exit;
	}

	lpNlattrs->nla_len =
	    (uint16_t)(lInSize & OAL_MAX_VAR_VALUE(lpNlattrs->nla_len));

	marshalling(OAL_GetNlaData(lpNlattrs), aFuncID, apInArgs, aNumInArgs);

	aHandle->mSendBuffer.mNlmHdr.nlmsg_len +=
	    NLMSG_ALIGN(lpNlattrs->nla_len);

	lIRet = socket_send(aHandle->mFd, (uint8_t *)&aHandle->mSendBuffer,
	                    aHandle->mSendBuffer.mNlmHdr.nlmsg_len, 0);
	if (lIRet != 0) {
		lRet = (uint32_t)-1;
	} else {
		lRet = receive_data(aHandle, apOutArgs, aNumOutArgs);
	}

driver_call_exit:
	return lRet;
}
