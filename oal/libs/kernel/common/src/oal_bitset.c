/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <oal_bitset.h>

int32_t OAL_InitBitset(OALBitsetChunk_t *apBitset, size_t aSize)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apBitset, init_bitset_exit);

	(void)memset(
	    apBitset, 0,
	    (size_t)(OAL_PRIV_BITSET_SIZE(aSize, OAL_BITSET_POS_PER_CHUNK) *
	             sizeof(OALBitsetChunk_t)));
init_bitset_exit:
	return lRet;
}

int32_t OAL_BitsetGetUnusedBit(OALBitsetChunk_t *apBitset, size_t aBitsetSize,
                               uint64_t *apFreePos)
{
	uint64_t lIdx = 0U;
	int32_t lRet  = 0;

	OAL_CHECK_NULL_PARAM(apBitset, get_unused_exit);
	OAL_CHECK_NULL_PARAM(apFreePos, get_unused_exit);

	*apFreePos = OAL_BITSET_CHUNK_MASK;
	for (lIdx = 0U; lIdx < aBitsetSize; lIdx++) {
		if (!OAL_IS_BIT_SET(apBitset, lIdx)) {
			*apFreePos = lIdx;
			break;
		}
	}

get_unused_exit:
	return lRet;
}

OALBitsetChunk_t *OAL_GetBitsetChunk(OALBitsetChunk_t *apBitset,
                                     uint64_t aBitPos)
{
	OALBitsetChunk_t *lpRet = NULL;
	if (apBitset != NULL) {
		lpRet = apBitset + OAL_PRIV_GET_BITSET_CHUNK(aBitPos);
	}

	return lpRet;
}
