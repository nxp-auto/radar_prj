/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_OS_SPINLOCK_H
#define OAL_OS_SPINLOCK_H

#include <oal_utils.h>

__BEGIN_DECLS

#ifdef __KERNEL__

typedef spinlock_t OAL_spinlock_t;
typedef spinlock_t OAL_irqspinlock_t;

#endif

__END_DECLS

#endif
