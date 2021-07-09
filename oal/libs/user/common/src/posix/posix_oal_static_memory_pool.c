/*
 * Copyright 2019-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_mem_constants.h>
#include <oal_mutex.h>
#include <oal_once.h>
#include <oal_static_pool.h>
#include <posix_oal_static_memory_pool.h>

#define OAL_MEM_PER_CHUNK (100U)
#define OAL_GET_NCHUNKS(SIZE)	                                               \
	((((SIZE) % (OAL_MEM_PER_CHUNK)) != 0U)                                    \
	     	? (((SIZE) / (OAL_MEM_PER_CHUNK)) + 1U)                            \
	     	: ((SIZE) / (OAL_MEM_PER_CHUNK)))

static uint8_t gsMemoryBuffer[OAL_MAX_PROCESS_COMM_SHARED_BUFFER];
static OAL_DECLARE_STATIC_POOL(gsMemoryPool,
                               OAL_GET_NCHUNKS(sizeof(gsMemoryBuffer)));
static struct OAL_Mutex gsMemoryPoolMutex;

static int32_t gsMemStaticPoolError;

static void initMemoryPool(void)
{
	if (OAL_InitializeMutex(&gsMemoryPoolMutex) != 0) {
		OAL_LOG_ERROR("Failed to initialize mutex\n");
		gsMemStaticPoolError = -EINVAL;
		goto init_exit;
	}

	if (OAL_INITIALIZE_POOL(&gsMemoryPool) != 0) {
		OAL_LOG_ERROR("Failed to initialize memory pool\n");
		gsMemStaticPoolError = -EINVAL;
	}

init_exit:
	return;
}

static uint64_t getNumChunks(size_t aSize)
{
	uint64_t lSize      = (uint64_t)aSize;
	uint64_t lRemainder = lSize % (uint64_t)OAL_MEM_PER_CHUNK;
	uint64_t lChunks    = lSize / (uint64_t)OAL_MEM_PER_CHUNK;

	if (lRemainder != 0UL) {
		lChunks++;
	}

	return lChunks;
}

int32_t OAL_AllocFromMemoryPool(size_t aSize, uint8_t **apPtr)
{
	int32_t lStatus, lRet = 0;
	uint64_t lStartIndex, lAvailNElems, lNElems;
	static struct OAL_OnceControl lsMemPoolInit = OAL_ONCE_INIT;

	OAL_CHECK_NULL_PARAM(apPtr, alloc_exit);
	lNElems      = getNumChunks(aSize);
	lAvailNElems = lNElems;
	lStartIndex  = 0U;

	/* Nothing to do */
	if (aSize == 0U) {
		lRet = 0;
		goto alloc_exit;
	}

	lRet = OAL_ExecuteOnce(&lsMemPoolInit, initMemoryPool);
	if ((lRet != 0) || (gsMemStaticPoolError != 0)) {
		OAL_LOG_ERROR("Failed to initialize memory pool\n");
		gsMemStaticPoolError = 0;
		lRet                 = -EINVAL;
		goto alloc_exit;
	}

	lRet = OAL_LockMutex(&gsMemoryPoolMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to take lock\n");
		goto alloc_exit;
	}

	lRet = OAL_POOL_GET_UNUSED_RANGE(&gsMemoryPool, &lStartIndex,
	                                 &lAvailNElems);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "Failed to allocate memory from pool\n"
		    "Please adjust its size\n");
		lRet = -ENOMEM;
		goto release_mutex;
	}

	lRet = OAL_POOL_SET_USED_RANGE(&gsMemoryPool, lStartIndex, lNElems);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to set the allocated pages used\n");
		goto release_mutex;
	}

	*apPtr = &gsMemoryBuffer[OAL_MEM_PER_CHUNK * lStartIndex];

release_mutex:
	lStatus = OAL_UnlockMutex(&gsMemoryPoolMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to release lock\n");
		lRet = lStatus;
	}
alloc_exit:
	return lRet;
}

int32_t OAL_ReleasePoolMemory(size_t aSize, uint8_t *apPtr)
{
	int32_t lStatus, lRet;
	uintptr_t lPtrDiff, lPtr = (uintptr_t)apPtr;
	static uintptr_t lsBeginPtr = (uintptr_t)&gsMemoryBuffer;
	static uintptr_t lsEndPtr =
	    (uintptr_t)&gsMemoryBuffer[OAL_ARRAY_SIZE(gsMemoryBuffer) - 1U];
	uint64_t lNElems, lStartIndex;

	/* Nothing to do */
	if (aSize == 0U) {
		OAL_LOG_WARNING("Releasing an empty pool\n");
		lRet = 0;
		goto release_exit;
	}

	lNElems = getNumChunks(aSize);

	if ((lPtr < lsBeginPtr) || (lPtr > lsEndPtr)) {
		OAL_LOG_ERROR("Invalid argument\n");
		lRet = -EINVAL;
		goto release_exit;
	}

	lPtrDiff = lPtr - lsBeginPtr;
	if ((lPtrDiff % (uintptr_t)OAL_MEM_PER_CHUNK) != 0U) {
		OAL_LOG_ERROR("Given pointer is in the middle of a chunk\n");
		lRet = -EINVAL;
		goto release_exit;
	}

	lStartIndex = (uint64_t)lPtrDiff / (uint64_t)OAL_MEM_PER_CHUNK;

	lRet = OAL_LockMutex(&gsMemoryPoolMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to take lock\n");
		goto release_exit;
	}

	lRet = OAL_POOL_SET_UNUSED_RANGE(&gsMemoryPool, lStartIndex, lNElems);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to mark given positions as unused\n");
	}

	lStatus = OAL_UnlockMutex(&gsMemoryPoolMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to release lock\n");
		lRet = lStatus;
	}
release_exit:
	return lRet;
}
