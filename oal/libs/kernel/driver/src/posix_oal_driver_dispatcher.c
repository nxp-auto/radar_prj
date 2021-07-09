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
#include <posix/posix_oal_driver_dispatcher.h>
#include <posix/posix_kern_oal_shared_memory.h>

uint32_t OAL_PosixOalDriverDispatcher(oal_dispatcher_t *apDisp,
                                      uint32_t aFuncId, uintptr_t aInData,
                                      int32_t aDataLen)
{
	uint32_t lRetVal = 0U;
	int32_t lIRet;
	OAL_UNUSED_ARG(aDataLen);

	switch (aFuncId) {
		///////////////////////////////////////////////////////////////////////////
		// Allocate a new contiguous region
		case CMD_ALLOC: {
			struct OALMemoryChunk *lpMemChunk;
			uint64_t lPagesIndex;
			uint64_t lNPages;

			CMD_ALLOC_TYPE *lpDrvCmd;
			pid_t lPid;
			OAL_LOG_DEBUG("OAL: ALLOCATE\n");

			(void)OAL_RPCGetClientPID(apDisp, &lPid);

			lpDrvCmd        = (CMD_ALLOC_TYPE *)aInData;
			lpDrvCmd->mSize = OAL_ROUNDUP_PAGE((lpDrvCmd->mSize));

			lIRet = OAL_GetFreePagesFromChunk(
			    lpDrvCmd->mChunkId, lpDrvCmd->mSize, &lPagesIndex,
			    &lNPages, &lpMemChunk);
			if (lIRet != 0) {
				lRetVal = ENOMEM;
				OAL_LOG_ERROR("Failed to allocate 0x%" PRIx64
				              " bytes from chunk %" PRIu32 "\n",
				              lpDrvCmd->mSize,
				              lpDrvCmd->mChunkId);
			} else {
				lpDrvCmd->mRetPhysPointer =
				    (lPagesIndex * OAL_PAGE_SIZE) +
				    lpMemChunk->mOSChunk.mStartPhysAddress;
				lpDrvCmd->mSize = lNPages * OAL_PAGE_SIZE;
				lpDrvCmd->mAlign =
				    lpMemChunk->mMemoryRegion.mAlign;
				(void)OAL_RPCAppendReply(apDisp,
				                         (void *)lpDrvCmd,
				                         sizeof(*lpDrvCmd));
			}

			break;
		}

		///////////////////////////////////////////////////////////////////////////
		// Free unused contiguous region
		case CMD_FREE: {
			CMD_FREE_TYPE *lpDrvCmd = (CMD_FREE_TYPE *)aInData;
			struct OALMemoryChunk *lpMemChunk;
			uint64_t lPagesIndex = 0U;

			OAL_LOG_DEBUG("OAL FREE\n");

			lIRet = OAL_GetMemoryChunkBasedOnAddr(
			    lpDrvCmd->mRetPhysPointer, lpDrvCmd->mSize,
			    &lpMemChunk);
			if (lIRet != 0) {
				lRetVal = ENOMEM;
				OAL_LOG_ERROR(
				    "Failed to identify memory "
				    "chunk for : 0x%" PRIx64 "\n",
				    lpDrvCmd->mRetPhysPointer);
			} else {
				if (lpMemChunk != NULL) {
					lPagesIndex =
					    (lpDrvCmd->mRetPhysPointer -
					     lpMemChunk->mMemoryRegion.mPhysAddr) /
					    (uint64_t)OAL_PAGE_SIZE;
				}
				lIRet = OAL_SetFreePages(
				    lpMemChunk, lPagesIndex,
				    lpDrvCmd->mSize / (uint64_t)OAL_PAGE_SIZE);
				if (lIRet != 0) {
					lRetVal = EINVAL;
					OAL_LOG_ERROR(
					    "Failed to mark unused: [0x%" PRIx64
					    " - 0x%" PRIx64 "]\n",
					    lpDrvCmd->mRetPhysPointer,
					    lpDrvCmd->mRetPhysPointer +
					        lpDrvCmd->mSize);
				}
			}

			break;
		}
		case CMD_CREATE_TOKEN: {
			OAL_TOKEN_TYPE *lpDrvCmd;
			lpDrvCmd = (OAL_TOKEN_TYPE *)aInData;

			if (lpDrvCmd == NULL) {
				lRetVal = EINVAL;
				break;
			}

			lIRet = OAL_DRV_CreateToken(lpDrvCmd->mPhysAddr,
			                            lpDrvCmd->mSize,
			                            &lpDrvCmd->mTokenID);
			if (lIRet != 0) {
				lRetVal = EINVAL;
				OAL_LOG_ERROR(
				    "Failed to create a token for : [0x%" PRIx64
				    " - 0x%" PRIx64 "]\n",
				    lpDrvCmd->mPhysAddr,
				    lpDrvCmd->mPhysAddr + lpDrvCmd->mSize);
			}

			(void)OAL_RPCAppendReply(apDisp, (void *)lpDrvCmd,
			                         sizeof(*lpDrvCmd));
			break;
		}
		case CMD_GET_TOKEN: {
			OAL_TOKEN_TYPE *lpDrvCmd;
			lpDrvCmd = (OAL_TOKEN_TYPE *)aInData;

			if (lpDrvCmd == NULL) {
				lRetVal = EINVAL;
				break;
			}

			lIRet = OAL_DRV_GetToken(lpDrvCmd->mTokenID,
			                         &lpDrvCmd->mPhysAddr,
			                         &lpDrvCmd->mSize);
			if (lIRet != 0) {
				lRetVal = EINVAL;
				OAL_LOG_ERROR(
				    "Failed to get info for token %" PRIxPTR
				    "\n",
				    lpDrvCmd->mTokenID);
			}

			(void)OAL_RPCAppendReply(apDisp, (void *)lpDrvCmd,
			                         sizeof(*lpDrvCmd));
			break;
		}
		case CMD_RELEASE_TOKEN: {
			uintptr_t *lpToken = (uintptr_t *)aInData;

			if (lpToken == NULL) {
				lRetVal = EINVAL;
				break;
			}

			lIRet = OAL_DRV_ReleaseToken(*lpToken);
			if (lIRet != 0) {
				lRetVal = EINVAL;
				OAL_LOG_ERROR(
				    "Failed to release token %" PRIxPTR "\n",
				    *lpToken);
			}

			break;
		}
		default: {
			OAL_LOG_ERROR("Invalid function ID: %d\n", aFuncId);
			lRetVal = EINVAL;
			break;
		}
	}

	return lRetVal;
}
