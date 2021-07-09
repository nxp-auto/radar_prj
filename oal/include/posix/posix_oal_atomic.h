/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_POSIX_ATOMIC_H
#define OAL_POSIX_ATOMIC_H

#include <oal_utils.h>

__BEGIN_DECLS

/**
 * Based on atomic operations offered by GCC built-ins:
 *
 * @see https:/gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
 * @see https:/gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html
 */

struct OAL_Atomic {
	volatile uint32_t mOSAtomic;
};

__END_DECLS

#endif
