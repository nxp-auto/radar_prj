/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_POSIX_BOTTOM_HALF_H
#define OAL_POSIX_BOTTOM_HALF_H

#include <oal_bottom_half.h>
#include <oal_utils.h>

__BEGIN_DECLS

struct OAL_BottomHalf {
	OAL_BottomHalfFunction_t mFunc;
	uintptr_t mData;
};

__END_DECLS
#endif /* OAL_POSIX_BOTTOM_HALF_H */
