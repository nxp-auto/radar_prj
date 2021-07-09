/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_IRQ_UTILS_H
#define OAL_OS_IRQ_UTILS_H

#include "oal_utils.h"

/* Linux kernel functinality */
#ifdef __KERNEL__

#include <linux/interrupt.h>  // For interrupts
#include <linux/uaccess.h>

typedef irqreturn_t OAL_irqreturn_t;

#endif

#endif /* OAL_OS_IRQ_UTILS_H */
