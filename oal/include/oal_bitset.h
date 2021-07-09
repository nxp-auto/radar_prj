/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_BITSET_H
#define OAL_BITSET_H

#include <oal_utils.h>

/**
 * @defgroup OAL_Bitset OAL Bitset
 *
 * @{
 * @brief Bitset Data Structure
 * @details
 * The Bitset is preallocated at compile time and cannot be extended
 * dynamically at runtime.
 * It represents a compact way to track used and unused elements of an array
 * without adding a dedicated field to the element structure.
 *
 * @code
 * struct Word {
 *	char *mStart;
 *	site_t mLength;
 *	bool mUsed;
 * };
 *
 * static Word gWords[1000];
 * @endcode
 *
 * Could be rewritten using Bitset as follows
 * @code
 * struct Word {
 *	char *mStart;
 *	site_t mLength;
 * };
 *
 * static Word gWords[1000];
 * static OAL_DECLARE_BITSET(lBitset, 1000);
 * @endcode
 *
 * @see https:/en.wikipedia.org/wiki/Bit_array
 */

/** Mask of a Bitset chunk */
#define OAL_BITSET_CHUNK_MASK ((OALBitsetChunk_t)(-1LL))
/** Number of elements per chunk */
#define OAL_BITSET_POS_PER_CHUNK (sizeof(OALBitsetChunk_t) * 8U)

/** @cond DO_NOT_DOCUMENT **/

#define OAL_PRIV_BITSET_SIZE(OAL_SIZE, ELEM_SIZE)                              \
	((((OAL_SIZE) % (ELEM_SIZE)) != 0U)                                    \
	     ? (((OAL_SIZE) / (ELEM_SIZE)) + 1U)                               \
	     : ((OAL_SIZE) / (ELEM_SIZE)))

#define OAL_PRIV_GET_BITSET_CHUNK(BIT_ID)                                      \
	(OAL_PRIV_BITSET_SIZE(((BIT_ID) + 1U), OAL_BITSET_POS_PER_CHUNK) - 1U)

/** @endcond */

/**
 * @brief Declare a Bitset using a given size
 *
 * @param[in] NAME   Bitset's name
 * @param[in] OAL_SIZE   Size of the Bitset, number of elements contained
 * by the array.
 */
#define OAL_DECLARE_BITSET(NAME, OAL_SIZE)                                     \
	OALBitsetChunk_t                                                       \
	    NAME[OAL_PRIV_BITSET_SIZE((OAL_SIZE), OAL_BITSET_POS_PER_CHUNK)]

/**
 * @brief Initialize a Bitset
 *
 * @param[in] BITSET_PTR Bitset pointer
 * @param[in] OAL_SIZE       Number of elements to be initialized
 */
#define OAL_INIT_BITSET(BITSET_PTR, OAL_SIZE)                                  \
	OAL_InitBitset((BITSET_PTR), (OAL_SIZE))

/**
 * @brief Retrieve the bit position inside it's chunk.
 *
 * @param[in] BIT_ID The bit number
 *
 * @return Bit position inside the chunk
 */
#define OAL_GET_CHUNK_BIT(BIT_ID) ((BIT_ID) % OAL_BITSET_POS_PER_CHUNK)

/**
 * @brief Get chunk address that contains the given BIT_ID
 *
 * @param[out] BITSET_PTR   Pointer to the chunk containing BIT_ID
 * @param[in] BIT_ID The bit number
 *
 * @return Bit position inside the chunk
 */
#define OAL_GET_BITSET_CHUNK_PTR(BITSET_PTR, BIT_ID)                           \
	OAL_GetBitsetChunk((OALBitsetChunk_t *)(uintptr_t)(BITSET_PTR),        \
	                   (BIT_ID))

/**
 * @brief Check whether the a position from Bitset is available or used
 *
 * @param[out] BITSET_PTR   Pointer to the chunk containing BIT_ID
 * @param[in] BIT_ID        The bit number
 *
 * @return true if the BIT_ID is unused, false otherwise
 */
#define OAL_IS_BIT_SET(BITSET_PTR, BIT_ID)                                     \
	(((*OAL_GET_BITSET_CHUNK_PTR((BITSET_PTR), (BIT_ID))) &                \
	  (1ULL << OAL_GET_CHUNK_BIT((BIT_ID)))) != 0U)

/**
 * @brief Mark a position as used
 *
 * @param[out] BITSET_PTR   Pointer to the chunk containing BIT_ID
 * @param[in] BIT_ID        The bit number
 */
#define OAL_SET_BIT(BITSET_PTR, BIT_ID)                                        \
	(*OAL_GET_BITSET_CHUNK_PTR((BITSET_PTR), (BIT_ID)) |=                  \
	 (1ULL << OAL_GET_CHUNK_BIT((BIT_ID))))

/**
 * @brief Mark a position as unused
 *
 * @param[out] BITSET_PTR   Pointer to the chunk containing BIT_ID
 * @param[in] BIT_ID        The bit number
 */
#define OAL_CLEAR_BIT(BITSET_PTR, BIT_ID)                                      \
	OAL_COMP_EXTENSION({                                                   \
		OALBitsetChunk_t *lpChunk =                                    \
		    OAL_GET_BITSET_CHUNK_PTR((BITSET_PTR), (BIT_ID));          \
		*lpChunk &= ~(1ULL << OAL_GET_CHUNK_BIT((BIT_ID)));            \
	})

/**
 * @brief Retrieves an unused bit position from a given bitset
 *
 * @param[in] BITSET               The bitset
 * @param[out] FREE_POS_PTR        A pointer to an <tt>uint64_t</tt> value
 *                                 where will be stored the position of the free
 * bit.
 *                                 It will be set to
 * <tt>OAL_BITSET_CHUNK_MASK</tt> if
 *                                 the bitset is full.
 */
#define OAL_BITSET_GET_UNUSED_BIT(BITSET, FREE_POS_PTR)                        \
	OAL_BitsetGetUnusedBit(&(BITSET[0]), (sizeof((BITSET)) * 8U),          \
	                       (FREE_POS_PTR))

__BEGIN_DECLS

/** Bitset chunk */
typedef uint64_t OALBitsetChunk_t;

/** @cond DO_NOT_DOCUMENT **/

int32_t OAL_InitBitset(OALBitsetChunk_t *apBitset, size_t aSize);

int32_t OAL_BitsetGetUnusedBit(OALBitsetChunk_t *apBitset, size_t aBitsetSize,
                               uint64_t *apFreePos);

OALBitsetChunk_t *OAL_GetBitsetChunk(OALBitsetChunk_t *apBitset,
                                     uint64_t aBitPos);

/** @endcond */

__END_DECLS
/** @} */

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_bitset.h>
#endif
#endif /* OAL_BITSET_H */
