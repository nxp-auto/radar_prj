/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <posix/posix_kern_oal_shared_memory.h>

struct AllocationToken {
	uint64_t mPhysAddr;
	uint64_t mSize;
};

static struct AllocationToken gsTokens[OAL_MAX_SHARED_TOKENS_PER_DRIVER];
static OAL_DECLARE_STATIC_POOL(gsTokensPool, OAL_ARRAY_SIZE(gsTokens));

static uint8_t isOverlapping(uint64_t aStart1, uint64_t aEnd1,
			     uint64_t aStart2, uint64_t aEnd2)
{
	uint8_t lRet = 0U;
	uint64_t lMaxStart = OAL_Max64u(aStart1, aStart2);
	uint64_t lMinEnd = OAL_Min64u(aEnd1, aEnd2);

	if (lMaxStart <= lMinEnd) {
		lRet = 1U;
	}

	return lRet;
}

static int32_t findToken(uint64_t aPhysAddr, uint64_t aSize)
{
	uint64_t lIdx, lIntEnd;
	int32_t lRet = -1;
	uint64_t lArgEnd = aPhysAddr + aSize - 1U;

	for (lIdx = 0U; lIdx < OAL_ARRAY_SIZE(gsTokens); lIdx++) {
		if (OAL_IS_BIT_SET(&gsTokensPool.mBitset, lIdx)) {
			/* is it part of an existing interval */
			lIntEnd = gsTokens[lIdx].mPhysAddr + gsTokens[lIdx].mSize - 1U;
			if (isOverlapping(aPhysAddr, lArgEnd,
					  gsTokens[lIdx].mPhysAddr, lIntEnd) != 0U) {
				lRet = 0;
			}
			break;
		}
	}
	return lRet;
}

static uintptr_t positionToToken(uint64_t aPos)
{
	uintptr_t lElemAddr  = (uintptr_t)&gsTokens[aPos];
	uintptr_t lTokenMask = OAL_MAX_VAR_VALUE(lTokenMask);

	return lTokenMask - lElemAddr;
}

static int32_t tokenToPosition(uintptr_t aToken, uint64_t *apPos)
{
	int32_t lRet           = 0;
	uintptr_t lTokensBegin = ((uintptr_t)&gsTokens);
	uintptr_t lTokensEnd   = lTokensBegin + sizeof(gsTokens);
	uintptr_t lTokenMask   = OAL_MAX_VAR_VALUE(lTokenMask);
	uintptr_t lElemAddr    = lTokenMask - aToken;

	if ((lElemAddr < lTokensBegin) || (lElemAddr >= lTokensEnd)) {
		lRet = -EINVAL;
		OAL_LOG_ERROR("Corrupted token: %" PRIxPTR "\n", aToken);
		goto token_exit;
	}

	if (((lElemAddr - lTokensBegin) % sizeof(gsTokens[0])) != 0U) {
		lRet = -EINVAL;
		OAL_LOG_ERROR("Corrupted token: %" PRIxPTR "\n", aToken);
		goto token_exit;
	}

	*apPos = (lElemAddr - lTokensBegin) / sizeof(gsTokens[0]);

token_exit:
	return lRet;
}

int32_t OAL_DRV_CreateToken(uint64_t aPhysAddr, uint64_t aSize,
                            uintptr_t *apToken)
{
	int32_t lRet;
	uint64_t lFreePos = 0ULL;

	lRet = findToken(aPhysAddr, aSize);
	if (lRet == 0) {
		OAL_LOG_ERROR("Address 0x%" PRIx64 " is in overlap with "
			      "one of the existing tokens\n", aPhysAddr);
		lRet = -EACCES;
		goto create_exit;
	}

	lRet = OAL_POOL_GET_FIRST_UNUSED(&gsTokensPool, &lFreePos);
	if (lRet != 0) {
		OAL_LOG_ERROR("Tokens pool is full, please adjust its size\n");
		lRet = -ENOMEM;
		goto create_exit;
	}

	gsTokens[lFreePos].mPhysAddr = aPhysAddr;
	gsTokens[lFreePos].mSize     = aSize;

	*apToken = positionToToken(lFreePos);

create_exit:
	return lRet;
}

int32_t OAL_DRV_GetToken(uintptr_t aToken, uint64_t *apPhysAddr,
                         uint64_t *apSize)
{
	int32_t lRet;
	uint64_t lPos;

	lRet = tokenToPosition(aToken, &lPos);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to interpret token\n");
		goto get_token_exit;
	}

	if (!OAL_IS_BIT_SET(&gsTokensPool.mBitset, lPos)) {
		lRet = -EINVAL;
		OAL_LOG_ERROR("The token has been released\n");
		goto get_token_exit;
	}

	*apPhysAddr = gsTokens[lPos].mPhysAddr;
	*apSize     = gsTokens[lPos].mSize;

get_token_exit:
	return lRet;
}

int32_t OAL_DRV_ReleaseToken(uintptr_t aToken)
{
	int32_t lRet;
	uint64_t lPos;

	lRet = tokenToPosition(aToken, &lPos);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to interpret token\n");
		goto release_exit;
	}

	if (OAL_IS_BIT_SET(&gsTokensPool.mBitset, lPos)) {
		lRet = OAL_POOL_SET_UNUSED(&gsTokensPool, lPos);
	} else {
		lRet = -EINVAL;
	}

release_exit:
	return lRet;
}
