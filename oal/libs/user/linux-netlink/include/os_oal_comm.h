/*
 * Copyright 2014-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_OS_COMM_H
#define OAL_OS_COMM_H

#ifndef __KERNEL__
#include <linux/genetlink.h>
#include <linux/netlink.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#endif

#include "oal_utils.h"

#define OAL_SOCKET_BUFFER_SIZE (50 * 1024)
#define OAL_RPC_VERSION 1

__BEGIN_DECLS

/**
 * @brief Attributes types
 */
///< Invalid type
#define OAL_RPC_UNSPEC (0U)
///< RPC call
#define OAL_RPC_CALL (1U)
///< RCP reply with out arguments
#define OAL_RPC_REPLY (2U)
///< RPC call return value
#define OAL_RPC_FUNC_RET (3U)
#define OAL_RPC_MAX (OAL_RPC_FUNC_RET)

/**
 * @brief Commands
 */
#define OAL_RPC_CMD_UNSPEC (0U)
#define OAL_RPC_CMD_EXEC (1U)
#define OAL_RPC_CMD_MAX (OAL_RPC_CMD_EXEC)

#ifndef __KERNEL__
struct OS_OAL_SocketBuffer {
	struct nlmsghdr mNlmHdr;
	struct genlmsghdr mGenHdr;
	uint8_t mData[OAL_SOCKET_BUFFER_SIZE];
};

struct OS_OAL_DriverHandle {
	struct OS_OAL_SocketBuffer mSendBuffer;
	struct OS_OAL_SocketBuffer mRecvBuffer;
	int32_t mFd;
	int32_t mFamilyId;
};

static inline struct nlattr *OAL_GetNlattr(struct OS_OAL_SocketBuffer *apBuff)
{
	return (struct nlattr *)(((uint8_t *)NLMSG_DATA(apBuff)) + GENL_HDRLEN);
}

static inline void *OAL_GetNlaData(struct nlattr *apAttrs)
{
	return ((void *)((uint8_t *)(apAttrs) + NLA_HDRLEN));
}

static inline int32_t OAL_GetNlMsgPayload(struct nlmsghdr *apMsg)
{
	return (NLMSG_PAYLOAD(apMsg, 0) - GENL_HDRLEN);
}

/**
 * OAL_GetNlaLen - length of payload
 * @apNAttr: netlink attribute
 */
static inline int32_t OAL_GetNlaLen(const struct nlattr *acpNAttr)
{
	uint16_t lRes = acpNAttr->nla_len - (uint16_t)NLA_HDRLEN;
	return (int32_t) lRes;
}
#endif

__END_DECLS

#endif /* OAL_OS_COMM_H */
