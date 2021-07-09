/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_OS_COMPLETION_H
#define OAL_OS_COMPLETION_H

#include <oal_utils.h>

#ifdef __KERNEL__
#include <linux/completion.h>

__BEGIN_DECLS

struct OAL_Completion {
	struct completion mComp;
};

__END_DECLS
#endif

#endif /* OAL_OS_COMPLETION_H */
