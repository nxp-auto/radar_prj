/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_OS_BOTTOM_HALF_H
#define OAL_OS_BOTTOM_HALF_H

#include <oal_utils.h>

/* Kernel space API */
#ifdef __KERNEL__
#include <linux/interrupt.h>

__BEGIN_DECLS

struct OAL_BottomHalf {
	struct tasklet_struct mTasklet;
};

__END_DECLS

#else
#include <posix/posix_oal_bottom_half.h>
#endif /* __KERNEL__ */
#endif /* OAL_OS_BOTTOM_HALF_H */
