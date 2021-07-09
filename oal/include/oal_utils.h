/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_UTILS_H
#define OAL_UTILS_H

#include <os_utils.h>

/**
 * @brief Put name between ""
 *
 * @param[in] s  String to put between ""
 */
#define OAL_STR(s) #s

/**
 * @brief Put expanded macro/define between ""
 *
 * @param[in] s The define to be expanded
 */
#define OAL_XSTR(s) OAL_STR(s)

/**
 * @brief Jump to \p LABEL if \p PARAM is NULL
 *
 * @param[in] PARAM The parameter to be checked for <tt>NULL</tt>
 * @param[in] LABEL Label to jump
 */
#define OAL_CHECK_NULL_PARAM(PARAM, LABEL)                                     \
	do {                                                                   \
		if (((PARAM) == NULL)) {                                       \
			OAL_LOG_ERROR(OAL_STR(PARAM) " is NULL\n");            \
			lRet = -EINVAL;                                        \
			goto LABEL;                                            \
		}                                                              \
	} while (1 == 0)

/**
 * @brief Get nearest multiple of \p ROUND_VAL starting from
 * \p OAL_SIZE.
 *
 * @param[in] OAL_SIZE      Size to round
 * @param[in] ROUND_VAL Rounding step
 *
 * @note \p ROUND_VAL must be power of 2
 */
#define OAL_ROUNDUP(OAL_SIZE, ROUND_VAL)                                           \
	(((OAL_SIZE) + (ROUND_VAL)-1U) & ~((ROUND_VAL)-1U))

/**
 * @brief Determines maximum unsigned value of a variable
 *
 * @param[in] VAR The variable to be checked
 *
 * @return Maximum value of the \p VAR
 */
#define OAL_MAX_VAR_VALUE(VAR)                                                 \
	OAL_COMP_EXTENSION({                                                   \
		uint64_t lMax =                                                \
		    ((((1UL << ((8UL * sizeof((VAR))) - 1UL)) - 1UL) << 1UL) + \
		     1UL);                                                     \
		lMax;                                                          \
	})

#define OAL_MIN_MACRO(A, B) (((A) < (B)) ? (A) : (B))
#define OAL_MAX_MACRO(A, B) (((A) > (B)) ? (A) : (B))

#ifndef OAL_ARRAY_SIZE
#define OAL_ARRAY_SIZE(ARRAY) ((sizeof((ARRAY))) / (sizeof((ARRAY)[0])))
#endif

#define OAL_UNUSED_ARG(ARG) (void)(ARG)

#define OAL_GET_PARENT_STRUCT(MEMBER_P, PARENT_TYPE, MEMBER_NAME)              \
	((PARENT_TYPE *)(uintptr_t)((uint8_t *)(uintptr_t)(MEMBER_P)-offsetof( \
	    PARENT_TYPE, MEMBER_NAME)))

#define OAL_NARG__(...) OAL_NARG_I_(__VA_ARGS__, OAL_RSEQ_NX())
#define OAL_NARG_I_(...) OAL_ARG_N(__VA_ARGS__)
#define OAL_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, \
                  _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26,  \
                  _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38,  \
                  _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
                  _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62,  \
                  _63, N, ...)                                                 \
	N

#define OAL_RSEQ_NX()                                                          \
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   \
	    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  \
	    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0

/* General definition for any function name */
#define OAL_XSTR_VFUNC(name, n) name##n
#define OAL_STR_VFUNC(name, n) OAL_XSTR_VFUNC(name, n)
#define OAL_VFUNC(PRINT_FUNC, PRINT_TAG, ...)                                  \
	OAL_STR_VFUNC(PRINT_FUNC, OAL_NARG__(__VA_ARGS__))                     \
	(PRINT_TAG, __VA_ARGS__)

#define OAL_ROUNDUP_PAGE(OAL_SIZE)                                             \
	((((OAL_SIZE) & (~(((uintptr_t)(OAL_PAGE_SIZE)) - (1U)))) < (OAL_SIZE))\
	     ? (((OAL_SIZE) & (~(((uintptr_t)(OAL_PAGE_SIZE)) - (1U)))) +      \
	        ((uintptr_t)(OAL_PAGE_SIZE)))                                  \
	     : (OAL_SIZE))

__BEGIN_DECLS

static inline uint8_t OAL_GetNumDigits(uint32_t aNumber)
{
	uint8_t lResult  = 0;
	uint32_t lNumber = aNumber;

	do {
		lResult++;
		lNumber /= 10U;
	} while (lNumber != 0U);

	return lResult;
}

static inline uint32_t OAL_Min32u(uint32_t aLeft, uint32_t aRight)
{
	return OAL_MIN_MACRO(aLeft, aRight);
}

static inline uint64_t OAL_Min64u(uint64_t aLeft, uint64_t aRight)
{
	return OAL_MIN_MACRO(aLeft, aRight);
}

static inline uintptr_t OAL_MinUptr(uintptr_t aLeft, uintptr_t aRight)
{
	return OAL_MIN_MACRO(aLeft, aRight);
}

static inline uint32_t OAL_Max32u(uint32_t aLeft, uint32_t aRight)
{
	return OAL_MAX_MACRO(aLeft, aRight);
}

static inline uint64_t OAL_Max64u(uint64_t aLeft, uint64_t aRight)
{
	return OAL_MAX_MACRO(aLeft, aRight);
}

static inline uintptr_t OAL_MaxUptr(uintptr_t aLeft, uintptr_t aRight)
{
	return OAL_MAX_MACRO(aLeft, aRight);
}



__END_DECLS

#endif
