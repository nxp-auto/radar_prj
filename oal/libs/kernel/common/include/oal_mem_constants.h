/*
 * Copyright 2018-2019, 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_MEM_CONSTANTS_H
#define OAL_MEM_CONSTANTS_H

#include <oal_utils.h>

/* Allow constants updates using OAL_CONSTANTS_FILE define */
#ifdef OAL_CONSTANTS_FILE
#include OAL_XSTR(OAL_CONSTANTS_FILE)
#endif

/* Number of memory reservations from device tree */
#ifndef OAL_MAX_RESERVED_REGIONS
#define OAL_MAX_RESERVED_REGIONS 6U
#endif

/* Max allowed size of the buffer transferred over RPC */
#ifndef OAL_RPC_MAX_DATA_BUFFER_SIZE
#define OAL_RPC_MAX_DATA_BUFFER_SIZE 1000U
#endif

/* Number of services pe kernel driver */
#ifndef OAL_MAX_SERVICES_PER_DRIVER
#define OAL_MAX_SERVICES_PER_DRIVER 42U
#endif

/* Number of subscribers in kernel space, per driver */
#ifndef OAL_MAX_SUBSCRIBERS_PER_EVENT
#define OAL_MAX_SUBSCRIBERS_PER_EVENT 42U
#endif

/* Number of open drivers from a process' context */
#ifndef OAL_MAX_DRIVER_HANDLES_PER_PROCESS
#define OAL_MAX_DRIVER_HANDLES_PER_PROCESS 42U
#endif

/* Size of memory reservations in bytes */
#ifndef OAL_MAX_REGION_SIZE
#define OAL_MAX_REGION_SIZE 0x20000000U
#endif

/* Allowed allocations from a Virtual Address Space */
#ifndef OAL_VAS_MAX_ALLOCATION
#define OAL_VAS_MAX_ALLOCATION 8000U
#endif

/* Allowed allocations per reserved memory chunk */
#ifndef OAL_MAX_PHYS_ALLOCATION_PER_CHUNK
#define OAL_MAX_PHYS_ALLOCATION_PER_CHUNK 8000U
#endif

/* [min - max] range of the reserved areas */
#ifndef OAL_MIN_RESERVED_ID
#define OAL_MIN_RESERVED_ID 0U
#endif

#ifndef OAL_MAX_RESERVED_ID
#define OAL_MAX_RESERVED_ID 9U
#endif

/* GHS constants */
/* Number of connections per VAS */
#ifndef OAL_MAX_CONNECTIONS_PER_VAS
#define OAL_MAX_CONNECTIONS_PER_VAS 50U
#endif

/* Number of attempts for service discovery */
#ifndef OAL_MAX_DISCOVERY_ATTEMPTS
#define OAL_MAX_DISCOVERY_ATTEMPTS 5U
#endif

/* Number of events per RPC connection */
#ifndef OAL_MAX_EVENTS_PER_SERVICE
#define OAL_MAX_EVENTS_PER_SERVICE 32U
#endif

/* QNX: Number of allocated QNX services per process, user space */
#ifndef OAL_QNX_MAX_SERVICES_PER_PROCESS
#define OAL_QNX_MAX_SERVICES_PER_PROCESS 40U
#endif

/* QNX: Number of interrupt instances per process */
#ifndef OAL_QNX_MAX_IRQS_PER_PROCESS
#define OAL_QNX_MAX_IRQS_PER_PROCESS 40U
#endif

/* Linux & QNX: Number of memory tokens per process */
#ifndef OAL_MAX_SHARED_TOKENS_PER_PROCESS
#define OAL_MAX_SHARED_TOKENS_PER_PROCESS 40U
#endif

/* Linux & QNX: Number of memory tokens per driver */
#ifndef OAL_MAX_SHARED_TOKENS_PER_DRIVER
#define OAL_MAX_SHARED_TOKENS_PER_DRIVER 400U
#endif

/* Linux & QNX: Memory allocated for communication serialization (bytes) */
#ifndef OAL_MAX_PROCESS_COMM_SHARED_BUFFER
#define OAL_MAX_PROCESS_COMM_SHARED_BUFFER (4U * 1024U)
#endif

#endif /* OAL_MEM_CONSTANTS_H */
