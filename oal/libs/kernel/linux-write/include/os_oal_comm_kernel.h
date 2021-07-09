/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_COMM_KERNEL_H
#define OAL_OS_COMM_KERNEL_H

#include <oal_comm.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <oal_utils.h>

__BEGIN_DECLS

struct comm_args {
	uint32_t* mpRet;    ///< Pointer to userspace return value
	uint32_t mFuncId;   ///< Id of the function to be executed
	void* mpInArgBuff;  ///< Userspace address of the input arguments buffer
	size_t mInArgBuffLen;       ///< Input arguments buffer length
	OAL_FuncArgs_t* mpOutBuff;  ///< Userspace address of the output buffer
	size_t mNumOutArgs;         ///< Out arguments number
};

#ifdef __KERNEL__

/**
 * @brief Obtain file from oal dispatcher
 *
 * @param[in] apDisp  The dispatcher
 *
 * @return The file associated to \p apDisp
 */
struct file* OAL_WriteGetFile(oal_dispatcher_t* apDisp);

/**
 * @brief Set / add custom file operation to service file. This call
 * will not take into account the value set for open and write calls
 * since both of them are used internally by the framework.
 *
 * @param[in] aService      The service
 * @param[in] apFileOps     File operations to be added / updated
 *
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_SetCustomFileOperations(OAL_RPCService_t aService,
                                    struct file_operations* apFileOps);

/**
 * @brief Obtain RPC service based on a Linux Kernel file structure
 *
 * @param[in] apFile      The file resulted after a RPC registration
 * @param[out] apService  OAL RPC service associated to \p apFile
 *
 * @warning \p apFile must be a file resulted after RPC registration
 * @return 0 for success, a negative value otherwise
 */
int32_t OAL_WriteGetServiceFromFile(struct file* apFile,
                                    OAL_RPCService_t* apService);
#else

#define OAL_LINUX_DEV_PREFIX "/dev/"

#ifndef OAL_MAX_DRIVER_PATH_LEN
/* 100 chars + 3 digits + '\0' */
#define OAL_MAX_DRIVER_PATH_LEN (100U + 3U + 1U)
#endif

#define OAL_LINUX_DRIVER_PATH_SIZE (OAL_MAX_DRIVER_PATH_LEN + \
				    sizeof(OAL_LINUX_DEV_PREFIX))

/* Driver's path + 3 digits */
#define OAL_LINUX_EVENT_PATH_SIZE (OAL_LINUX_DRIVER_PATH_SIZE + 3U)

struct OAL_DriverEvent {
	int32_t mFd;
	uint32_t mID;
	char8_t mPath[OAL_LINUX_EVENT_PATH_SIZE];
	uint8_t mStatus;
	struct OS_OAL_DriverHandle *mpHandle;
};

struct OS_OAL_DriverHandle {
	int32_t mFd;      ///< File descriptor to the opened device file
	char8_t mPath[OAL_LINUX_DRIVER_PATH_SIZE];  ///< File path
	OAL_DECLARE_STATIC_POOL_UNINITIALIZED(mEventsPool,
		OAL_MAX_EVENTS_PER_SERVICE);
	struct OAL_DriverEvent mEvents[OAL_MAX_EVENTS_PER_SERVICE];
};

#endif

__END_DECLS
#endif /* OAL_OS_COMM_KERNEL_H */
