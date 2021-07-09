/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_IRQ_UTILS_H
#define OAL_IRQ_UTILS_H

#include <oal_utils.h>
#include <os_oal_irq_utils.h>

#define OAL_IRQ_NONE 0
#define OAL_IRQ_HANDLED 1

__BEGIN_DECLS

typedef OAL_irqreturn_t (*OAL_irq_handler_t)(const int32_t irq_no,
                                             const void *irq_data);
__END_DECLS

#define OAL_IRQ_HANDLER(NAME)                                                      \
	OAL_irqreturn_t NAME(const int32_t irq_no, const void *irq_data)

#endif /* OAL_IRQ_UTILS_H */
