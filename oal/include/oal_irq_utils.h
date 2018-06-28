/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_IRQ_UTILS_H
#define OAL_IRQ_UTILS_H

#include "oal_utils.h"
#include XSTR(OS/os_oal_irq_utils.h)

#define IRQ_NONE    0
#define IRQ_HANDLED 1

typedef OAL_irqreturn_t (*OAL_irq_handler_t)(const int32_t irq_no,
		const void* irq_data);

#define IRQ_HANDLER(NAME) OAL_irqreturn_t NAME(const int32_t irq_no, \
					   const void* irq_data)


#endif /* OAL_IRQ_UTILS_H */
