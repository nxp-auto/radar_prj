/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OS_OAL_IRQ_UTILS_H
#define OS_OAL_IRQ_UTILS_H

#include "oal_utils.h"

/* Linux kernel functinality */
#ifdef __KERNEL__

#include <linux/interrupt.h>    // For interrupts
#include <linux/uaccess.h>

typedef irqreturn_t OAL_irqreturn_t;

#endif

#endif /* OS_OAL_IRQ_UTILS_H */
