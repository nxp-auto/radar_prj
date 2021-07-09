/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_STATIC_POOL_H
#define OAL_STATIC_POOL_H

#include <oal_utils.h>
#include <oal_bitset.h>

/**
 * @defgroup OAL_Pool OAL Static Pool
 *
 * @{
 * @brief Statically allocated pool
 * @details
 * This data structure represents a fancy wrapper over @ref OAL_Bitset.
 * It allows to control the Bitset at high level without getting dirty with
 * low-level commands and checks.
 *
 * Additionally to @ref OAL_Bitset, it offers the possibility to set / unset
 * multiple elements using one single call in a performant manner. Finding
 * a free region is also possible with the API exported by OAL memory pools.
 *
 */

/** End of the pool */
#define OAL_END_OF_POOL 1

/** @cond DO_NOT_DOCUMENT **/
#define OAL_POOL_STATIC_INIT(SIZE)                                             \
	{                                                                      \
		.mBitset = {0}, .mSize = (SIZE), .mSearchHint = 0,             \
	}
/** @endcond */

/**
 * @brief Declare and initialize a memory pool
 *
 * @param[in] NAME   Pool's name
 * @param[in] SIZE   Size of the pool, number of elements contained
 * @note Cannot be used as a member of a type, for this case use
 *       \ref OAL_DECLARE_STATIC_POOL_UNINITIALIZED
 */
#define OAL_DECLARE_STATIC_POOL(NAME, SIZE)                                    \
	struct OAL_Pool##NAME {                                                \
		OAL_DECLARE_BITSET(mBitset, (SIZE));                           \
		uint64_t mSize;                                                \
		uint64_t mSearchHint;                                          \
	} NAME = OAL_POOL_STATIC_INIT(SIZE)

/**
 * @brief Declare a memory pool without initializing it. This is suitable for
 * member declarations followed by a #OAL_SET_POOL_SIZE and #OAL_INITIALIZE_POOL
 * call.
 *
 * @code{.c}
 * struct MyContainer {
 * 		uint32_t mArray[MAX_ELEMENTS];
 *		OAL_DECLARE_STATIC_POOL_UNINITIALIZED(mArrayPool, MAX_ELEMENTS);
 * };
 *
 * static struct MyContainer gContainer;
 *	...
 * int32_t module_initialization()
 * {
 * 		...
 *		OAL_SET_POOL_SIZE(&gContainer.mArrayPool,
 *OAL_ARRAY_SIZE(gContainer.mArray));
 *		OAL_INITIALIZE_POOL(&gContainer.mArrayPool);
 * 		...
 * }
 * @endcode
 *
 * @param[in] NAME   Pool's name
 * @param[in] SIZE   Size of the pool, number of elements contained
 */
#define OAL_DECLARE_STATIC_POOL_UNINITIALIZED(NAME, SIZE)                      \
	struct OAL_Pool##NAME {                                                \
		OAL_DECLARE_BITSET(mBitset, SIZE);                             \
		uint64_t mSize;                                                \
		uint64_t mSearchHint;                                          \
	} NAME

/**
 * @brief Set the size of the pool. This function is usually called on an
 * uninitialised pool.
 *
 * @param[in] POOL_PTR   Pool reference
 * @param[in] SIZE       Size of the pool, number of elements contained
 * @see OAL_DECLARE_STATIC_POOL_UNINITIALIZED
 */
#define OAL_SET_POOL_SIZE(POOL_PTR, SIZE)                                      \
	OAL_COMP_EXTENSION({                                                   \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                 \
		lpPoolPtr->mSize                 = (SIZE);                     \
	})

/**
 * @brief Initialize an uninitialized pool
 *
 * @param[in] POOL_PTR   A pointer to pool
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 * @see OAL_DECLARE_STATIC_POOL_UNINITIALIZED
 */
#define OAL_INITIALIZE_POOL(POOL_PTR)                                                 \
	OAL_COMP_EXTENSION({                                                          \
		int32_t lInitRet                 = 0;                                 \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                        \
		lInitRet                         = OAL_InitializePool(                \
                    &lpPoolPtr->mBitset[0], lpPoolPtr->mSize, \
                    OAL_ARRAY_SIZE(lpPoolPtr->mBitset),       \
                    &lpPoolPtr->mSearchHint);                 \
		lInitRet;                                                             \
	})

/**
 * @brief Dump the content of the pool with used and unused position at
 * standard output
 *
 * @param[in] POOL_PTR   A pointer to pool
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
#define OAL_DUMP_POOL(POOL_PTR)                                                \
	OAL_COMP_EXTENSION({                                                   \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                 \
		OAL_DumpPool(&lpPoolPtr->mBitset[0],                           \
		             OAL_ARRAY_SIZE(lpPoolPtr->mBitset));              \
	})

/**
 * @brief Check whether the pool is empty or not
 *
 * @param[in] POOL_PTR   A pointer to pool
 *
 * @return true if the pool is empty, false otherwise
 */
#define OAL_IS_POOL_EMPTY(POOL_PTR)                                            \
	OAL_COMP_EXTENSION({                                                   \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                 \
		(OAL_IsPoolEmpty(&lpPoolPtr->mBitset[0], lpPoolPtr->mSize,     \
		                 OAL_ARRAY_SIZE(lpPoolPtr->mBitset)) == 1U);   \
	})

/**
 * @brief Check whether the pool is full or not
 *
 * @param[in] POOL_PTR   A pointer to pool
 *
 * @return true if the pool is full, false otherwise
 */
#define OAL_IS_POOL_FULL(POOL_PTR)                                             \
	OAL_COMP_EXTENSION({                                                   \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                 \
		(OAL_IsPoolFull(&lpPoolPtr->mBitset[0],                        \
		                OAL_ARRAY_SIZE(lpPoolPtr->mBitset)) == 1U);    \
	})

/**
 * @brief Get first unused position from the pool and mark
 * \a FREE_POS_PTR position as used.
 *
 * @param[in] POOL_PTR       A pointer to pool
 * @param[out] FREE_POS_PTR  A \a uint64_t pointer to the first
 * available position
 *
 * @return
 * 	* 0 is the operation succeeded
 * 	* #OAL_END_OF_POOL if the pool is full
 */
#define OAL_POOL_GET_FIRST_UNUSED(POOL_PTR, FREE_POS_PTR)                                        \
	OAL_COMP_EXTENSION({                                                                     \
		int32_t lFirstRet                = 0;                                            \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                                   \
		lFirstRet                        = OAL_GetFirstUnusedPosition(                   \
                    &lpPoolPtr->mBitset[0],                               \
                    OAL_ARRAY_SIZE(lpPoolPtr->mBitset), lpPoolPtr->mSize, \
                    &lpPoolPtr->mSearchHint, (FREE_POS_PTR));             \
		lFirstRet;                                                                       \
	})

/**
 * @brief Mark a position as unused
 *
 * @param[in] POOL_PTR       A pointer to pool
 * @param[in] POS            The position to mark as unused
 * available position
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
#define OAL_POOL_SET_UNUSED(POOL_PTR, POS)                                     \
	OAL_COMP_EXTENSION({                                                   \
		int32_t lUnusedRet               = 0;                          \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                 \
		lUnusedRet = OAL_SetUnusedPosition(&lpPoolPtr->mBitset[0],     \
		                                   lpPoolPtr->mSize, (POS));   \
		lUnusedRet;                                                    \
	})

/**
 * @brief Mark elements contained by the interval [START_POS, START_POS + N_POS)
 * as used.
 *
 * @note Make sure the provided range is a valid one, otherwise the call will
 * fail. It's desirable to use an interval obtained after a
 * #OAL_POOL_GET_NEXT_UNUSED_RANGE or #OAL_POOL_GET_UNUSED_RANGE call.
 *
 * @param[in] POOL_PTR       A pointer to pool
 * @param[in] START_POS      The beginning of the interval
 * @param[in] N_POS          Number of position to be marked as used including
 * \p START_POS
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
#define OAL_POOL_SET_USED_RANGE(POOL_PTR, START_POS, N_POS)                                       \
	OAL_COMP_EXTENSION({                                                                      \
		int32_t lUsedRet                 = 0;                                             \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                                    \
		lUsedRet                         = OAL_SetRangeUsage(                             \
                    &lpPoolPtr->mBitset[0],                               \
                    OAL_ARRAY_SIZE(lpPoolPtr->mBitset), lpPoolPtr->mSize, \
                    (START_POS), (N_POS), 1);                             \
		lUsedRet;                                                                         \
	})

/**
 * @brief Mark elements contained by the interval [START_POS, START_POS + N_POS)
 * as unused. Reverse operation of #OAL_POOL_SET_USED_RANGE
 *
 * @param[in] POOL_PTR       A pointer to pool
 * @param[in] START_POS      The beginning of the interval
 * @param[in] N_POS          Number of position to be marked as unused including
 * \p START_POS
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
#define OAL_POOL_SET_UNUSED_RANGE(POOL_PTR, START_POS, N_POS)                              \
	OAL_COMP_EXTENSION({                                                               \
		int32_t lUnusedRangeRet          = 0;                                      \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                             \
		lUnusedRangeRet                  = OAL_SetRangeUsage(                      \
                    &lpPoolPtr->mBitset[0],                               \
                    OAL_ARRAY_SIZE(lpPoolPtr->mBitset), lpPoolPtr->mSize, \
                    (START_POS), (N_POS), 0);                             \
		lUnusedRangeRet;                                                           \
	})

/**
 * @brief Get first interval of contiguous unused positions starting from
 * <tt>*START_POS_PTR</tt>. In a loop this function allows to iterate over
 * all unused intervals from a pool.
 *
 * @code{.c}
 * int32_t lRet;
 * uint64_t lStart, lSize;
 * do {
 * 	lRet = OAL_POOL_GET_NEXT_UNUSED_RANGE(&lPool, &lStart,
 * 			&lSize);

 * 	if (lRet != OAL_END_OF_POOL) {
 * 		lStart += lSize;
 * 	}
 * 	... do something with lStart and lSize ....
 * } while (lRet != OAL_END_OF_POOL);
 * @endcode
 *
 * @param[in] POOL_PTR            A pointer to pool
 * @param[in,out] START_POS_PTR   The beginning of the interval
 * @param[out] N_POS_PTR          Number of position to be marked as unused
 * including \p START_POS
 *
 * @return 0 is the operation succeeded, #OAL_END_OF_POOL if the
 * iteration reached the end of the pool or a negative value otherwise
 */
#define OAL_POOL_GET_NEXT_UNUSED_RANGE(POOL_PTR, START_POS_PTR, N_POS_PTR)                \
	OAL_COMP_EXTENSION({                                                              \
		int32_t lNUnusedRangeRet         = 0;                                     \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                            \
		lNUnusedRangeRet                 = OAL_GetNextUnusedRange(                \
                    &lpPoolPtr->mBitset[0],                               \
                    OAL_ARRAY_SIZE(lpPoolPtr->mBitset), lpPoolPtr->mSize, \
                    (START_POS_PTR), (N_POS_PTR));                        \
		lNUnusedRangeRet;                                                         \
	})

/**
 * @brief Get first interval of contiguous unused positions starting from
 * <tt>*START_POS_PTR</tt> and having at least <tt>*N_POS_PTR</tt>.
 *
 * @param[in] POOL_PTR            A pointer to pool
 * @param[in,out] START_POS_PTR   The beginning of the interval
 * @param[out] N_POS_PTR          Number of position to be marked as unused
 * including \p START_POS
 *
 * @return 0 is the operation succeeded, #OAL_END_OF_POOL if the
 * iteration reached the end of the pool or a negative value otherwise
 */

#define OAL_POOL_GET_UNUSED_RANGE(POOL_PTR, START_POS_PTR, N_POS_PTR)                   \
	OAL_COMP_EXTENSION({                                                            \
		int32_t lGetUnusedRangeRet       = 0;                                   \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                          \
		lGetUnusedRangeRet               = OAL_GetUnusedRange(                  \
                    &lpPoolPtr->mBitset[0],                               \
                    OAL_ARRAY_SIZE(lpPoolPtr->mBitset), lpPoolPtr->mSize, \
                    (START_POS_PTR), (N_POS_PTR));                        \
		lGetUnusedRangeRet;                                                     \
	})

/**
 * @brief Get the address of an unused element from \p ASSOC_ARRAY_PTR
 * and mark its position as _used_ in \p POOL_PTR.
 *
 * @param[in] POOL_PTR             A pointer to pool
 * @param[in] ASSOC_ARRAY_PTR      A pointer to associated array
 * @param[in,out] ELEM_PTR_PTR     Pointer to element's pointer.
 *
 * @warning The function will not check the boundary of \p ASSOC_ARRAY_PTR. It
 * assumes that the pool and the passed array have exactly the same number of
 * elements.
 *
 * @return 0 is the operation succeeded, #OAL_END_OF_POOL if the
 * iteration reached the end of the pool or a negative value otherwise
 */
#define OAL_ALLOC_ELEM_FROM_POOL(POOL_PTR, ASSOC_ARRAY_PTR, ELEM_PTR_PTR)                           \
	OAL_COMP_EXTENSION({                                                                        \
		int32_t lAllocElemRet                   = 0;                                        \
		__typeof__((POOL_PTR)) lpPoolPtr        = (POOL_PTR);                               \
		__typeof__(*(ELEM_PTR_PTR)) lpAsocArray = (ASSOC_ARRAY_PTR);                        \
		__typeof__((ELEM_PTR_PTR)) lpElemPtrPtr = (ELEM_PTR_PTR);                           \
		lAllocElemRet                           = OAL_AllocElemFromPool(                    \
                    &lpPoolPtr->mBitset[0],                               \
                    OAL_ARRAY_SIZE(lpPoolPtr->mBitset), lpPoolPtr->mSize, \
                    &lpPoolPtr->mSearchHint,                              \
                    (uint8_t *)(uintptr_t)(lpAsocArray),                  \
                    (uint8_t **)(uintptr_t)(lpElemPtrPtr),                \
                    sizeof(**(lpElemPtrPtr)));                            \
		lAllocElemRet;                                                                      \
	})

/**
 * @brief Release an element obtained after an \ref OAL_ALLOC_ELEM_FROM_POOL
 * call.
 *
 * @param[in] POOL_PTR             A pointer to pool
 * @param[in] ASSOC_ARRAY_PTR      A pointer to associated array
 * @param[in] ELEM_PTR             Element obtained using
 *                                 \ref OAL_ALLOC_ELEM_FROM_POOL
 *
 * @return 0 is the operation succeeded, or a negative value otherwise
 */
#define OAL_RELEASE_ELEM_FROM_POOL(POOL_PTR, ASSOC_ARRAY_PTR, ELEM_PTR)                        \
	OAL_COMP_EXTENSION({                                                                   \
		int32_t lReleaseElemRet          = 0;                                          \
		__typeof__((POOL_PTR)) lpPoolPtr = (POOL_PTR);                                 \
		__typeof__((ELEM_PTR)) lpArray   = (ASSOC_ARRAY_PTR);                          \
		__typeof__((ELEM_PTR)) lpElemPtr = (ELEM_PTR);                                 \
		lReleaseElemRet                  = OAL_ReleaseElemFromPool(                    \
                    &lpPoolPtr->mBitset[0], lpPoolPtr->mSize,                 \
                    (uint8_t *)(uintptr_t)(lpArray),                          \
                    (uint8_t *)(uintptr_t)(lpElemPtr), sizeof(*(lpElemPtr))); \
		lReleaseElemRet;                                                               \
	})

/** @} */

__BEGIN_DECLS

/** @cond DO_NOT_DOCUMENT **/
int32_t OAL_InitializePool(OALBitsetChunk_t *apBitset, uint64_t aSize,
                           size_t aBitsetChunks, uint64_t *apSearchHint);

#ifndef OAL_LOG_SUPPRESS_DEBUG
void OAL_DumpPool(OALBitsetChunk_t *apBitset, uint64_t aSize);
#endif

uint8_t OAL_IsPoolEmpty(OALBitsetChunk_t *apBitset, uint64_t aSize,
                        size_t aBitsetChunks);

uint8_t OAL_IsPoolFull(OALBitsetChunk_t *apBitset, size_t aBitsetChunks);

int32_t OAL_GetFirstUnusedPosition(OALBitsetChunk_t *apBitset,
                                   uint64_t aBitsetChunks, uint64_t aSize,
                                   uint64_t *apSearchHint, uint64_t *apFreePos);

int32_t OAL_SetUnusedPosition(OALBitsetChunk_t *apBitset, uint64_t aSize,
                              uint64_t aPos);

int32_t OAL_SetRangeUsage(OALBitsetChunk_t *apBitset, size_t aBitsetChunks,
                          uint64_t aSize, uint64_t aStartPos, uint64_t aNPos,
                          uint8_t aUsage);

int32_t OAL_GetNextUnusedRange(OALBitsetChunk_t *apBitset, size_t aBitsetChunks,
                               uint64_t aSize, uint64_t *apStartPos,
                               uint64_t *apNPos);

int32_t OAL_GetUnusedRange(OALBitsetChunk_t *apBitset, size_t aBitsetChunks,
                           uint64_t aSize, uint64_t *apStartPos,
                           uint64_t *apNPos);

int32_t OAL_AllocElemFromPool(OALBitsetChunk_t *apBitset, size_t aBitsetChunks,
                              uint64_t aSize, uint64_t *apSearchHint,
                              uint8_t *apAssocArrayPtr, uint8_t **apElemPtr,
                              size_t aElemSize);

int32_t OAL_ReleaseElemFromPool(OALBitsetChunk_t *apBitset, uint64_t aSize,
                                uint8_t *apAssocArrayPtr, uint8_t *apElemPtr,
                                size_t aElemSize);

/** @endcond */

__END_DECLS

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_static_pool.h>
#endif
#endif /* OAL_STATIC_POOL_H */
