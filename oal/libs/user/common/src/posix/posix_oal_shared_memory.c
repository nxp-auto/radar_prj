/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_common.h>
#include <oal_driver_functions.h>
#include <oal_log.h>
#include <oal_mem_constants.h>
#include <oal_memory_allocator.h>
#include <oal_mutex.h>
#include <oal_shared_memory.h>
#include <oal_static_pool.h>
#include <oal_virtmem_manager.h>
#include <os_oal_shared_memory.h>
#include <posix_oal_memory_allocator.h>
#include <oal_once.h>

struct TokenMapping {
	uintptr_t mToken;
	uintptr_t mVirtAddr;
	uint64_t mSize;
};

static struct TokenMapping gsTokenMap[OAL_MAX_SHARED_TOKENS_PER_PROCESS];
static OAL_DECLARE_STATIC_POOL(gsTokenMapPool, OAL_ARRAY_SIZE(gsTokenMap));
static struct OAL_Mutex gsTokensMutex;

static int32_t gsSharedMemoryError;
static void initializeSharedMemMutex(void)
{
	int32_t lRet = OAL_InitializeMutex(&gsTokensMutex);
	if (lRet != 0) {
		gsSharedMemoryError = lRet;
		OAL_LOG_ERROR("Failed to initialize tokens' lock\n");
		goto init_exit;
	}

	lRet = OAL_INITIALIZE_POOL(&gsTokenMapPool);
	if (lRet != 0) {
		gsSharedMemoryError = lRet;
		OAL_LOG_ERROR("Failed to initialize memory pool\n");
	}

init_exit:
	return;
}

int32_t OAL_CreateMemoryToken(struct OAL_MemoryHandle *apMemHandle,
                              uintptr_t *apToken)
{
	int32_t lRet = 0;
	uint32_t lStatus;
	uint8_t lAllocated;
	OAL_TOKEN_TYPE lCmdData;
	static struct OAL_OnceControl lsInitTokensMutex = OAL_ONCE_INIT;

	OAL_CHECK_NULL_PARAM(apToken, create_exit);

	(void)OAL_ExecuteOnce(&lsInitTokensMutex, initializeSharedMemMutex);
	if (gsSharedMemoryError != 0) {
		OAL_LOG_ERROR("Failed to initialize shared buffers mutex\n");
		lRet = gsSharedMemoryError;
		gsSharedMemoryError = 0;
		goto create_exit;
	}

	lAllocated = OAL_CheckAllocated(apMemHandle);
	if (lAllocated == 0U) {
		OAL_LOG_ERROR("The passed memory handle "
			      "wasn't allocated using OAL: %p\n",
			      (void *)apMemHandle);
		lRet = -EINVAL;
		goto create_exit;
	}

	if (apMemHandle->mpPhysAlloc == NULL) {
		OAL_LOG_ERROR("Corrupted data\n");
		lRet = -EINVAL;
		goto create_exit;
	}

	lCmdData.mPhysAddr =
	    apMemHandle->mpPhysAlloc->mPhysAddr + apMemHandle->mOffset;
	lCmdData.mSize    = apMemHandle->mSize;
	lCmdData.mTokenID = 0U;

	lStatus = OAL_SimpleInOutCall(gOalH, (uint32_t)CMD_CREATE_TOKEN, lCmdData);
	if (lStatus != 0U) {
		lRet = (int32_t) lStatus;
		OAL_LOG_ERROR("Failed to communicate with OAL driver\n");
		goto create_exit;
	} else {
		*apToken = lCmdData.mTokenID;
	}

create_exit:
	return lRet;
}

static int32_t findMappedToken(uintptr_t aTokenID,
                               struct TokenMapping **apToken)
{
	uint64_t lIdx;
	int32_t lRet = -1;

	for (lIdx = 0U; lIdx < OAL_ARRAY_SIZE(gsTokenMap); lIdx++) {
		if (OAL_IS_BIT_SET(&gsTokenMapPool.mBitset, lIdx)) {
			if (aTokenID == gsTokenMap[lIdx].mToken) {
				lRet = 0;
				if (apToken != NULL) {
					*apToken = &gsTokenMap[lIdx];
				}
			}
			break;
		}
	}
	return lRet;
}

static int32_t mapToken(OAL_TOKEN_TYPE *apCmdData, OAL_MEMORY_ACCESS aAccess,
                        uintptr_t *apMappedAddress)
{
	int32_t lRet, lStatus;
	uint64_t lFreePos = 0ULL;
	enum OAL_MemoryMapping lVirtType;
	uint8_t *lpVirtAddr;

	lRet = OAL_LockMutex(&gsTokensMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to take mutex\n");
		goto map_exit;
	}

	/* Check if already mapped */
	lRet = findMappedToken(apCmdData->mTokenID, NULL);
	if (lRet == 0) {
		OAL_LOG_ERROR("Token %" PRIxPTR " is already mapped\n",
		              apCmdData->mTokenID);
		lRet = -EINVAL;
		goto release_mutex;
	}

	lRet = OAL_POOL_GET_FIRST_UNUSED(&gsTokenMapPool, &lFreePos);
	if (lRet != 0) {
		OAL_LOG_ERROR("Tokens pool is full, please adjust its size\n");
		lRet = -ENOMEM;
		goto release_mutex;
	}

	if (aAccess == OAL_ACCESS_CH_WB) {
		lVirtType = OAL_VIRT_CACHEABLE;
	} else {
		lVirtType = OAL_VIRT_NON_CACHEABLE;
	}

	lRet = OAL_MapPhysMem(apCmdData->mPhysAddr, apCmdData->mSize,
	                      &lpVirtAddr, lVirtType);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to map token %" PRIxPTR "\n",
		              apCmdData->mTokenID);
		goto release_position;
	}

	gsTokenMap[lFreePos].mToken    = apCmdData->mTokenID;
	gsTokenMap[lFreePos].mVirtAddr = (uintptr_t)lpVirtAddr;
	gsTokenMap[lFreePos].mSize     = apCmdData->mSize;

	*apMappedAddress = (uintptr_t)lpVirtAddr;

release_position:
	if (lRet != 0) {
		lStatus = OAL_POOL_SET_UNUSED(&gsTokenMapPool, lFreePos);
		if (lStatus != 0) {
			OAL_LOG_ERROR("Failed to release position %" PRIx64
			              "\n",
			              lFreePos);
			lRet = lStatus;
		}
	}
release_mutex:
	lStatus = OAL_UnlockMutex(&gsTokensMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to release mutex\n");
		lRet = lStatus;
		goto map_exit;
	}

map_exit:
	return lRet;
}

static int32_t unmapToken(uintptr_t aToken)
{
	int32_t lRet = 0, lStatus;
	uintptr_t lPos;
	struct TokenMapping *lpToken;

	lRet = OAL_LockMutex(&gsTokensMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to take mutex\n");
		goto map_exit;
	}

	/* Check if already mapped */
	lRet = findMappedToken(aToken, &lpToken);
	if (lRet != 0) {
		OAL_LOG_ERROR("Token %" PRIxPTR " isn't mapped\n", aToken);
		lRet = -EINVAL;
		goto release_mutex;
	}

	lPos    = ((uintptr_t)lpToken) - ((uintptr_t)&gsTokenMap[0]);
	lPos   /= sizeof(gsTokenMap[0]);
	lStatus = OAL_POOL_SET_UNUSED(&gsTokenMapPool, (uint64_t)lPos);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to release position %" PRIxPTR "\n", lPos);
		lRet = lStatus;
	}

	lStatus =
	    OAL_UnmapVirtAddress((uint8_t *)lpToken->mVirtAddr, lpToken->mSize);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to unmap token %" PRIxPTR "\n", aToken);
		lRet = lStatus;
	}

release_mutex:
	lStatus = OAL_UnlockMutex(&gsTokensMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to release mutex\n");
		lRet = lStatus;
		goto map_exit;
	}

map_exit:
	return lRet;
}

int32_t OAL_MapMemoryToken(uintptr_t aToken, OAL_MEMORY_ACCESS aAccess,
                           uintptr_t *apMappedAddress)
{
	int32_t lRet = 0;
	uint32_t lStatus;
	OAL_TOKEN_TYPE lCmdData = {
	    .mPhysAddr = 0UL, .mSize = 0UL,
	};

	OAL_CHECK_NULL_PARAM(apMappedAddress, map_exit);
	*apMappedAddress = 0U;

	if (gOalH == NULL) {
		lRet = OAL_Initialize();
		if (lRet != 0) {
			goto map_exit;
		}
	}

	lCmdData.mTokenID = aToken;

	lStatus = OAL_SimpleInOutCall(gOalH, (uint32_t)CMD_GET_TOKEN, lCmdData);
	if (lStatus != 0U) {
		lRet = (int32_t) lStatus;
		OAL_LOG_ERROR("Failed to communicate with OAL driver\n");
		goto map_exit;
	}

	if (aAccess == OAL_ACCESS_PHY) {
		*apMappedAddress = lCmdData.mPhysAddr;
	} else {
		lRet = mapToken(&lCmdData, aAccess, apMappedAddress);
	}
map_exit:
	return lRet;
}

int32_t OAL_UnmapMemoryToken(uintptr_t aToken) { return unmapToken(aToken); }
int32_t OAL_ReleaseToken(uintptr_t aToken)
{
	int32_t lRet;
	uint32_t lStatus;

	/* Is it mapped ? */
	lRet = findMappedToken(aToken, NULL);
	if (lRet == 0) {
		lRet = OAL_UnmapMemoryToken(aToken);
	} else {
		lRet = 0;
	}

	lStatus = OAL_SimpleInCall(gOalH, (uint32_t)CMD_RELEASE_TOKEN, aToken);
	if (lStatus != 0U) {
		lRet = (int32_t)lStatus;
		OAL_LOG_ERROR("Failed to communicate with OAL driver\n");
	}

	return lRet;
}
