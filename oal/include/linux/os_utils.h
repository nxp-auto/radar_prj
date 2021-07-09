/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_UTILS_H
#define OAL_OS_UTILS_H

/* Linux kernel functinality */
#ifdef __KERNEL__
#include <linux/bug.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/types.h>
#define __BEGIN_DECLS
#define __END_DECLS

#define PRIx8 "x"
#define PRIx16 "x"
#define PRIx32 "x"
#define PRIx64 "llx"
#define PRId64 "lld"
#define PRIu64 "lld"
#define PRIu32 "u"
#define PRIu16 "u"
#define PRIu8 "u"

#ifndef OAL_PRINT
#define OAL_PRINT(fmt, ...) printk(KERN_ALERT fmt, ##__VA_ARGS__)
#endif

#ifndef OAL_PAGE_SIZE
#define OAL_PAGE_SIZE (PAGE_SIZE)
#endif

#ifndef __HAVE_ARCH_STRLEN
static inline __kernel_size_t strlen(const char *acpString)
{
	__kernel_size_t lLen = 0;
	while (*acpString != '\0') {
		acpString++;
		lLen++;
	}

	return lLen;
}
#endif

typedef long intptr_t;
#define PRIxPTR "lx"
#else
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
#define _POSIX_C_SOURCE 200809L
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include "posix/posix_delay.h"

#define OAL_L1_CACHE_LINE_SIZE 0x40

#ifndef OAL_PRINT
#define OAL_PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#endif
#endif

#ifndef OAL_COMP_EXTENSION
#define OAL_COMP_EXTENSION __extension__
#endif

__BEGIN_DECLS

typedef char char8_t;

__END_DECLS

#endif /* OAL_OS_UTILS_H */
