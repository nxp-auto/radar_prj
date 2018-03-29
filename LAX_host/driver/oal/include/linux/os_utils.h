/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OS_UTILS_H
#define OS_UTILS_H

/* Linux kernel functinality */
#ifdef __KERNEL__
	#include <linux/kernel.h>
	#include <linux/bug.h>
	#include <linux/types.h>
	#include <linux/delay.h>
	#define __BEGIN_DECLS
	#define __END_DECLS

	#define PRIx8  "x"
	#define PRIx16 "x"
	#define PRIx32 "x"
	#define PRIx64 "llx"

#else

	#ifdef POSIX_C_SOURCE
	#	undef POSIX_C_SOURCE
	#endif
	#define POSIX_C_SOURCE 200809L
	#ifndef _GNU_SOURCE
	#	define _GNU_SOURCE
	#endif
	#include <sys/cdefs.h>
	#include <string.h>
	#include <stdint.h>
	#include <stdlib.h>
	#include <signal.h>
	#include <errno.h>
	#include <stdbool.h>
	#include <inttypes.h>
	#include "posix/posix_delay.h"

#endif

#endif /* OS_UTILS_H */
