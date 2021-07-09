/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_static_pool.h>

#define GET_FIRST_FREE_POS(BITSET_CHUNK)                                       \
	(~(BITSET_CHUNK) & ((BITSET_CHUNK) + 1U))

static inline uint64_t get64BitMask(uint64_t aMaskSize)
{
	uint64_t lMask;

	if (aMaskSize >= OAL_BITSET_POS_PER_CHUNK) {
		lMask = OAL_BITSET_CHUNK_MASK;
	} else {
		lMask = ((1ULL << aMaskSize) - 1U);
	}

	return lMask;
}

static inline uint64_t getLastChunkMask(uint64_t aSize)
{
	uint64_t lMask = 0U;
	uint64_t lPosToHide;

	if ((aSize % OAL_BITSET_POS_PER_CHUNK) != 0U) {
		lPosToHide = OAL_BITSET_POS_PER_CHUNK -
		             (aSize % OAL_BITSET_POS_PER_CHUNK);
		lMask = get64BitMask(lPosToHide);

		lMask <<= (OAL_BITSET_POS_PER_CHUNK - lPosToHide);
	}
	return lMask;
}

int32_t OAL_InitializePool(OALBitsetChunk_t *apBitset, uint64_t aSize,
                           size_t aBitsetChunks, uint64_t *apSearchHint)
{
	int32_t lRet = 0;
	uint64_t lMask;

	if (aSize > (((uint64_t)aBitsetChunks) * OAL_BITSET_POS_PER_CHUNK)) {
		OAL_LOG_ERROR(
		    "Trying to initiate more than allocated "
		    "0x%" PRIx64 " vs 0x%" PRIx64 "\n",
		    aSize, (uint64_t)aBitsetChunks * OAL_BITSET_POS_PER_CHUNK);
		lRet = -1;
	} else {
		lRet = OAL_INIT_BITSET(apBitset, (size_t)aSize);
		if (lRet == 0) {
			lMask = getLastChunkMask(aSize);
			if (lMask != 0ULL) {
				apBitset[aBitsetChunks - 1U] = lMask;
			}
			*apSearchHint = 0U;
		}
	}

	return lRet;
}

#ifndef OAL_LOG_SUPPRESS_DEBUG
void OAL_DumpPool(OALBitsetChunk_t *apBitset, uint64_t aSize)
{
	size_t lIdx = 0U;
	for (lIdx = 0; lIdx < aSize; lIdx++) {
		(void)OAL_PRINT("Chunk %zu - 0x%" PRIx64 "\n", lIdx,
		                apBitset[lIdx]);
	}
}
#endif

static inline int32_t getNumTrailingZeros(uint64_t aValue, uint64_t *apNumZeros)
{
	int32_t lRet = 0;

	if (aValue == 0ULL) {
		/* Full of zeros */
		*apNumZeros = 64UL;
		goto exit_get_zeros;
	}

	lRet = __builtin_ctzll(aValue);
	if (lRet < 0) {
		OAL_LOG_ERROR("__builtin_ctzll failed for 0x%" PRIu64 "\n",
		              aValue);
		goto exit_get_zeros;
	}

	*apNumZeros = (uint64_t)lRet;
	lRet        = 0;

exit_get_zeros:
	return lRet;
}

uint8_t OAL_IsPoolEmpty(OALBitsetChunk_t *apBitset, uint64_t aSize,
                        size_t aBitsetChunks)
{
	uint8_t lEmpty = 1U;
	uint64_t lIdx  = 0U;
	uint64_t lLastFreePos = 0U;
	uint64_t lNChunks = aSize / OAL_BITSET_POS_PER_CHUNK;

	for (lIdx = 0U; lIdx < lNChunks; lIdx++) {
		if (apBitset[lIdx] != 0U) {
			lEmpty = 0U;
			break;
		}
	}

	if ((lEmpty != 0U) && ((aSize % OAL_BITSET_POS_PER_CHUNK) != 0U)) {
		/* Count the number of trailing 0-bits in mask */
		if (getNumTrailingZeros(apBitset[aBitsetChunks - 1U],
		                        &lLastFreePos) == 0) {
			if (lLastFreePos !=
			    (aSize % OAL_BITSET_POS_PER_CHUNK)) {
				lEmpty = 0U;
			}
		}
	}

	return lEmpty;
}

uint8_t OAL_IsPoolFull(OALBitsetChunk_t *apBitset, size_t aBitsetChunks)
{
	uint8_t lFull = 1U;
	size_t lIdx;

	for (lIdx = 0U; lIdx < aBitsetChunks; lIdx++) {
		if (apBitset[lIdx] != OAL_BITSET_CHUNK_MASK) {
			lFull = 0U;
			break;
		}
	}

	return lFull;
}

int32_t OAL_GetFirstUnusedPosition(OALBitsetChunk_t *apBitset,
                                   uint64_t aBitsetChunks, uint64_t aSize,
                                   uint64_t *apSearchHint, uint64_t *apFreePos)
{
	int32_t lRet = OAL_END_OF_POOL;
	uint64_t lFirstZeroMask;
	uint64_t lIdx;

	OAL_CHECK_NULL_PARAM(apFreePos, get_first_unused_pos_exit);

	if (*apSearchHint >= aBitsetChunks) {
		*apSearchHint = 0U;
	}

	for (lIdx = *apSearchHint; lIdx < aBitsetChunks; lIdx++) {
		if (apBitset[lIdx] != OAL_BITSET_CHUNK_MASK) {
			lFirstZeroMask = GET_FIRST_FREE_POS(apBitset[lIdx]);
			/* Count the number of trailing 0-bits in mask */
			if (getNumTrailingZeros(lFirstZeroMask, apFreePos) !=
			    0) {
				break;
			}

			*apFreePos += (lIdx * OAL_BITSET_POS_PER_CHUNK);
			OAL_SET_BIT(apBitset, *apFreePos);
			*apSearchHint = lIdx;
			lRet          = 0;
			break;
		}
	}

	if ((lRet != 0) || (*apFreePos >= aSize)) {
		for (lIdx = 0U; lIdx < aBitsetChunks; lIdx++) {
			if (apBitset[lIdx] != OAL_BITSET_CHUNK_MASK) {
				lFirstZeroMask =
				    GET_FIRST_FREE_POS(apBitset[lIdx]);
				/* Count the number of trailing 0-bits in mask
				 */
				if (getNumTrailingZeros(lFirstZeroMask,
				                        apFreePos) != 0) {
					break;
				}
				*apFreePos += (lIdx * OAL_BITSET_POS_PER_CHUNK);
				OAL_SET_BIT(apBitset, *apFreePos);
				*apSearchHint = lIdx;
				lRet          = 0;
				break;
			}
		}
	}

	if (*apFreePos >= aSize) {
		lRet = OAL_END_OF_POOL;
	}

get_first_unused_pos_exit:
	return lRet;
}

int32_t OAL_SetUnusedPosition(OALBitsetChunk_t *apBitset, uint64_t aSize,
                              uint64_t aPos)
{
	int32_t lRet = -1;

	if (aPos >= aSize) {
		goto set_used_elem_exit;
	}

	OAL_CLEAR_BIT(apBitset, aPos);
	lRet = 0;

set_used_elem_exit:
	return lRet;
}

int32_t OAL_SetRangeUsage(OALBitsetChunk_t *apBitset, size_t aBitsetChunks,
                          uint64_t aSize, uint64_t aStartPos, uint64_t aNPos,
                          uint8_t aUsage)
{
	int32_t lRet = 0;
	size_t lChunkStartBit;
	uint64_t lMask;
	uint64_t lMaskSize;
	OALBitsetChunk_t *lpChunkPtr;
	uint64_t lOrigStartPos = aStartPos;
	uint64_t lStartPos     = aStartPos;
	uint64_t lOrigNPosPos  = aNPos;
	uint64_t lNPos         = aNPos;

	OAL_UNUSED_ARG(aBitsetChunks);

	/* Used for logging only */
	OAL_UNUSED_ARG(lOrigStartPos);
	OAL_UNUSED_ARG(lOrigNPosPos);

	if ((lStartPos + lNPos) > aSize) {
		lRet = -1;
		goto set_used_range_exit;
	}

	if (lNPos == 0U) {
		lRet = -1;
		goto set_used_range_exit;
	}

	if (lNPos == 1U) {
		if (aUsage == 0U) {
			if (!OAL_IS_BIT_SET(apBitset, lStartPos)) {
				lRet = -1;
			} else {
				OAL_CLEAR_BIT(apBitset, lStartPos);
			}
		} else {
			if (OAL_IS_BIT_SET(apBitset, lStartPos)) {
				lRet = -1;
			} else {
				OAL_SET_BIT(apBitset, lStartPos);
			}
		}
		goto set_used_range_exit;
	}

	while (lNPos != 0U) {
		lpChunkPtr     = OAL_GET_BITSET_CHUNK_PTR(apBitset, lStartPos);
		lChunkStartBit = (size_t)OAL_GET_CHUNK_BIT(lStartPos);

		/* Fill first part of the chunk */
		if (lChunkStartBit != 0U) {
			if ((lChunkStartBit + lNPos) <=
			    OAL_BITSET_POS_PER_CHUNK) {
				lMaskSize = lNPos;
			} else {
				size_t lPosPerChunk = OAL_BITSET_POS_PER_CHUNK;
				lMaskSize =
				    ((uint64_t)lPosPerChunk) - lChunkStartBit;
			}
		} else {
			/* Fill the entire chunk */
			if (lNPos >= OAL_BITSET_POS_PER_CHUNK) {
				lMaskSize = OAL_BITSET_POS_PER_CHUNK;
			} else {
				/* Fill the last part of the chunk */
				lMaskSize = lNPos;
			}
		}

		lMask = get64BitMask(lMaskSize) << lChunkStartBit;
		/* Set bits */
		if (aUsage == 1U) {
			if ((*lpChunkPtr & lMask) != 0ULL) {
				OAL_LOG_ERROR("The provided range [0x%" PRIx64
				              " - 0x%" PRIx64
				              "] is in "
				              "overlap with one of the "
				              "previous set operations\n",
				              lOrigStartPos,
				              lOrigStartPos + lOrigNPosPos);
				lRet = -1;
				goto set_used_range_exit;
			}

			*lpChunkPtr |= lMask;
		} else {
			/* Unset bits */
			if (((~(*lpChunkPtr)) & lMask) != 0U) {
				OAL_LOG_ERROR("The provided range [0x%" PRIx64
				              " - 0x%" PRIx64
				              "] is in "
				              "overlap with one of the "
				              "previous unset operations\n",
				              lOrigStartPos,
				              lOrigStartPos + lOrigNPosPos);
				lRet = -1;
				goto set_used_range_exit;
			}
			*lpChunkPtr &= ~(lMask);
		}

		if (lMaskSize <= lNPos) {
			lNPos -= lMaskSize;
		} else {
			OAL_LOG_ERROR(
			    "There is something wrong with the "
			    "algorithm from %s\n",
			    __func__);
			lRet = -1;
			goto set_used_range_exit;
		}
		lStartPos += lMaskSize;
	}

set_used_range_exit:
	return lRet;
}

static inline int32_t getNAvailablePos(OALBitsetChunk_t *apBitset,
                                       size_t aBitsetChunks, uint64_t aSize,
                                       uint64_t *apStartPos, uint64_t *apNPos)
{
	int32_t lRet = 0;
	size_t lChunkOffset;
	uint64_t lChunkOffset64;
	OALBitsetChunk_t *lpChunkPtr;
	uint64_t lZeros = 0U;
	size_t lPosPerChunk = OAL_BITSET_POS_PER_CHUNK;

	*apNPos    = 0U;
	lpChunkPtr = OAL_GET_BITSET_CHUNK_PTR(apBitset, *apStartPos);

	lChunkOffset64 = *apStartPos % OAL_BITSET_POS_PER_CHUNK;
	lChunkOffset   = (size_t)lChunkOffset64;
	if ((lChunkOffset != 0U) && ((*lpChunkPtr >> lChunkOffset) == 0U)) {
		*apNPos += ((uint64_t)(lPosPerChunk)-lChunkOffset64);
	} else {
		if (getNumTrailingZeros((*lpChunkPtr >> lChunkOffset),
		                        &lZeros) != 0) {
			goto exit_get_available_pos;
		}
		*apNPos += lZeros;
	}

	lpChunkPtr++;
	/* The chunk is filled partially */
	if (*apNPos == ((uint64_t)(lPosPerChunk) - ((uint64_t)lChunkOffset))) {
		while ((*lpChunkPtr == 0ULL) &&
		       ((ssize_t)(lpChunkPtr - apBitset) < ((ssize_t)aSize))) {
			lpChunkPtr++;
			*apNPos += OAL_BITSET_POS_PER_CHUNK;
		}

		if (((ssize_t)(lpChunkPtr - apBitset) !=
		     ((ssize_t)aBitsetChunks))) {
			if (getNumTrailingZeros(*lpChunkPtr, &lZeros) != 0) {
				goto exit_get_available_pos;
			}
			*apNPos += lZeros;
		}
	}

exit_get_available_pos:
	return lRet;
}

int32_t OAL_GetNextUnusedRange(OALBitsetChunk_t *apBitset, size_t aBitsetChunks,
                               uint64_t aSize, uint64_t *apStartPos,
                               uint64_t *apNPos)
{
	OALBitsetChunk_t *lpChunkPtr;
	OALBitsetChunk_t lChunkVal;
	uint64_t lChunkOffset;
	int32_t lRet            = 0;
	uint64_t lNextOneOffset = 0U;
	intptr_t lDiff;

	if ((apStartPos == NULL) || (apNPos == NULL)) {
		lRet = -1;
		goto get_next_unused_range;
	}

	if (*apStartPos > aSize) {
		lRet = -1;
		goto get_next_unused_range;
	}

	if (*apStartPos == aSize) {
		lRet = OAL_END_OF_POOL;
		goto get_next_unused_range;
	}

	while (OAL_IS_BIT_SET(apBitset, *apStartPos)) {
		lChunkOffset = *apStartPos % OAL_BITSET_POS_PER_CHUNK;
		lpChunkPtr   = OAL_GET_BITSET_CHUNK_PTR(apBitset, *apStartPos);

		if ((lChunkOffset == 0ULL) &&
		    (*lpChunkPtr == OAL_BITSET_CHUNK_MASK)) {
			/* Jump over full chunks */
			lpChunkPtr++;
			*apStartPos = 0U;
		} else {
			/* Mask positions before apStartPos */
			lChunkVal = *lpChunkPtr |
			            get64BitMask(*apStartPos %
			                         OAL_BITSET_POS_PER_CHUNK);
			if (getNumTrailingZeros(~lChunkVal, &lNextOneOffset) !=
			    0) {
				lRet = -1;
				goto get_next_unused_range;
			}

			/* Current chunk looks full */
			if (lNextOneOffset == OAL_BITSET_POS_PER_CHUNK) {
				*apStartPos = OAL_BITSET_POS_PER_CHUNK;
			} else {
				*apStartPos = lNextOneOffset;
			}
		}

		if ((lpChunkPtr - apBitset) < 0) {
			OAL_LOG_ERROR("Corrupted memory pool\n");
			goto get_next_unused_range;
		}

		lDiff = lpChunkPtr - apBitset;
		{
			/* MISRA Fix */
			size_t lPosPerChunk = OAL_BITSET_POS_PER_CHUNK;
			*apStartPos +=
			    ((uint64_t)lDiff) * ((uint64_t)lPosPerChunk);
		}

		if (*apStartPos >= aSize) {
			lRet = OAL_END_OF_POOL;
			goto get_next_unused_range;
		}
	}

	lRet = getNAvailablePos(apBitset, aBitsetChunks, aSize, apStartPos,
	                        apNPos);
get_next_unused_range:
	return lRet;
}

int32_t OAL_GetUnusedRange(OALBitsetChunk_t *apBitset, size_t aBitsetChunks,
                           uint64_t aSize, uint64_t *apStartPos,
                           uint64_t *apNPos)
{
	int32_t lRet              = 0;
	uint64_t lAvailableSpaces = 0U;

	if ((apStartPos == NULL) || (apNPos == NULL)) {
		lRet = -1;
		goto get_unused_range_exit;
	}

	if ((*apStartPos + *apNPos) > aSize) {
		lRet = OAL_END_OF_POOL;
		goto get_unused_range_exit;
	}

	do {
		lRet = OAL_GetNextUnusedRange(apBitset, aBitsetChunks, aSize,
		                              apStartPos, &lAvailableSpaces);
		if (lRet == OAL_END_OF_POOL) {
			break;
		}

		if (lAvailableSpaces >= *apNPos) {
			*apNPos = lAvailableSpaces;
			break;
		}

		*apStartPos += lAvailableSpaces;
	} while (lRet == 0);

get_unused_range_exit:
	return lRet;
}

int32_t OAL_AllocElemFromPool(OALBitsetChunk_t *apBitset, size_t aBitsetChunks,
                              uint64_t aSize, uint64_t *apSearchHint,
                              uint8_t *apAssocArrayPtr, uint8_t **apElemPtr,
                              size_t aElemSize)
{
	uint64_t lFreePos = 0U;
	int32_t lStatus   = 0;

	lStatus = OAL_GetFirstUnusedPosition(apBitset, aBitsetChunks, aSize,
	                                     apSearchHint, &lFreePos);
	if (lStatus != 0) {
		goto new_elem_exit;
	}

	*apElemPtr = apAssocArrayPtr + (aElemSize * lFreePos);

new_elem_exit:
	return lStatus;
}

int32_t OAL_ReleaseElemFromPool(OALBitsetChunk_t *apBitset, uint64_t aSize,
                                uint8_t *apAssocArrayPtr, uint8_t *apElemPtr,
                                size_t aElemSize)
{
	uintptr_t lDiff;
	int32_t lRet             = 0;
	uint8_t *lpAssecArrayEnd = apAssocArrayPtr + (aSize * aElemSize);

	/* Allocation cleanup */
	if (apElemPtr == NULL) {
		lRet = -1;
		goto set_unused_elem_exit;
	}

	if ((apElemPtr >= apAssocArrayPtr) && (apElemPtr < lpAssecArrayEnd)) {
		lDiff = (uintptr_t)apElemPtr;
		lDiff -= (uintptr_t)apAssocArrayPtr;
		lDiff /= aElemSize;

		lRet = OAL_SetUnusedPosition(apBitset, aSize, (uint64_t)lDiff);
	} else {
		lRet = -1;
		OAL_LOG_ERROR("Out of bounds access %p\n", (void *)apElemPtr);
	}

set_unused_elem_exit:
	return lRet;
}
