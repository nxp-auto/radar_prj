/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OS_OAL_COMM_KERNEL_H
#define OS_OAL_COMM_KERNEL_H


#include "oal_utils.h"
#include "oal_comm.h"

__BEGIN_DECLS

struct comm_args {
	uint32_t* pRet; ///< Pointer to userspace return value
	uint32_t func_id; ///< Id of the function to be executed
	void* in_arg_buff; ///< Userspace address of the input arguments buffer
	size_t in_arg_buff_len; ///< Input arguments buffer length
	OAL_FuncArgs_t* out_buff; ///< Userspace address of the output buffer
	size_t n_out_args; ///< Out arguments number
};

#ifdef __KERNEL__

/**
 * @brief Obtain file from oal dispatcher
 *
 * @param[in] d  The dispatcher
 *
 * @return The file associated to <code>d</code>
 */
struct file* OAL_WriteGetFile(oal_dispatcher_t *d);

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
int OAL_SetCustomFileOperations(OAL_RPCService_t aService,
		struct file_operations* apFileOps);

#else

struct OS_OAL_DriverHandle {
	int fd; ///< File descriptor to the opened device file
};

#endif

__END_DECLS
#endif /* OS_OAL_COMM_KERNEL_H */
