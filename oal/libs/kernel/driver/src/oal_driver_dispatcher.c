/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_LOG_SUPPRESS_DEBUG
#define OAL_LOG_SUPPRESS_DEBUG
#endif
#include <oal_driver_dispatcher.h>
#include <oal_driver_functions.h>
#include <oal_log.h>
#include <oal_page_manager.h>

uint32_t OAL_DriverDispatcher(oal_dispatcher_t *apDisp, uint32_t aFuncId,
                             uintptr_t aInData, int32_t aDataLen)
{
	uint32_t lRetVal = 0;
	int32_t lIRet;
	OAL_UNUSED_ARG(aDataLen);

	switch (aFuncId) {
		case CMD_MEMORY_GET_SIZE: {
			uint64_t *lpMemIndex = (uint64_t *)aInData;
			uint64_t lSize       = 0;

			OAL_LOG_DEBUG("OAL: GET CHUNK OAL_SIZE\n");

			lIRet = OAL_GetChunkSize(*lpMemIndex, &lSize);
			if (lIRet != 0) {
				lRetVal = EINVAL;
				OAL_LOG_ERROR("Failed to get chunk's (%" PRIu64
				              ") size\n",
				              *lpMemIndex);
			}

			(void)OAL_RPCAppendReply(apDisp, (void *)&lSize,
			                         sizeof(lSize));
			break;
		}

		// Get base address
		case CMD_MEMORY_GET_BASE: {
			uint64_t *lpMemIndex = (uint64_t *)aInData;
			uint64_t lBaseAddr   = 0;

			OAL_LOG_DEBUG("OAL: GET BASE ADDRESS\n");

			lIRet = OAL_GetChunkBaseAddress(*lpMemIndex, &lBaseAddr);
			if (lIRet != 0) {
				lRetVal = EINVAL;
				OAL_LOG_ERROR("Failed to get chunk's (%" PRIu64
				              ") base address\n",
				              *lpMemIndex);
			}

			(void)OAL_RPCAppendReply(apDisp, (void *)&lBaseAddr,
			                         sizeof(lBaseAddr));

			break;
		}

		// Get available devices
		case CMD_MEMORY_GET_DEVICES: {
			uint32_t lNPools = OAL_GetNumberOfPools();
			OAL_LOG_DEBUG("OAL: GET LOADED DEVICES MASK\n");

			(void)OAL_RPCAppendReply(apDisp, (void *)&lNPools,
			                         sizeof(lNPools));
			break;
		}

		// Get autobalanced devices
		case CMD_MEMORY_GET_AUTOBALANCE: {
			uint32_t lIsBalanced = 0;
			uint32_t *lpMemIndex = (uint32_t *)aInData;

			OAL_LOG_DEBUG("OAL: GET AUTOBALANCED DEVICES MASK\n");

			lIRet = OAL_IsChunkAutobalanced(*lpMemIndex, &lIsBalanced);
			if (lIRet != 0) {
				lRetVal = EINVAL;
				OAL_LOG_ERROR("Failed to get chunk's (%" PRIu32
				              ") balance flag\n",
				              *lpMemIndex);
			}

			(void)OAL_RPCAppendReply(apDisp, (void *)&lIsBalanced,
			                         sizeof(lIsBalanced));
			break;
		}

		default: {
			// Pass down to OS layer
			lRetVal = OAL_OS_DriverDispatcher(apDisp, aFuncId,
			                                aInData, aDataLen);
			break;
		}
	}

	return lRetVal;
}
