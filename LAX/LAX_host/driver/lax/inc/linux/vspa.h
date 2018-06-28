/*
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __VSPA_H_
#define __VSPA_H_

#include "lax_driver.h"
#include "lax_atu.h"

#include <linux/cdev.h>

/* Debug and error reporting macros */
#define IF_DEBUG(x)    if (pVspaDev->debug & (x))
#define ERR(...)    { if (pVspaDev->debug & DEBUG_MESSAGES) \
                pr_err(VSPA_DEVICE_NAME __VA_ARGS__); }
#define MAX_VSPA 4
#define VSPA_DEVICE_NAME "vspa"

/* Debug bit assignments */
#define DEBUG_MESSAGES      (1<<0)
#define DEBUG_STARTUP       (1<<1)
#define DEBUG_CMD           (1<<2)
#define DEBUG_REPLY         (1<<3)
#define DEBUG_SPM           (1<<4)
#define DEBUG_DMA           (1<<5)
#define DEBUG_EVENT         (1<<6)
#define DEBUG_WATCHDOG      (1<<7)
#define DEBUG_MBOX64_OUT    (1<<8)
#define DEBUG_MBOX32_OUT    (1<<9)
#define DEBUG_MBOX64_IN     (1<<10)
#define DEBUG_MBOX32_IN     (1<<11)
#define DEBUG_DMA_IRQ       (1<<12)
#define DEBUG_FLAGS0_IRQ    (1<<13)
#define DEBUG_FLAGS1_IRQ    (1<<14)
#define DEBUG_IOCTL         (1<<15)
#define DEBUG_SEQID         (1<<16)
#define DEBUG_CMD_BD        (1<<17)
#define DEBUG_REPLY_BD      (1<<18)
#define DEBUG_TEST_SPM      (1<<24)



#define VSPA_DMA_CHANNELS       (32)


/* mmap offset argument for vspa regsiters */
#define VSPA_REG_OFFSET         (0)

/* mmap offset argument for dbg regsiter */
#define VSPA_DBG_OFFSET         (4096)

#define BD_ENTRIES              (16)



/* The below structure contains all the information for the
* vspa device required by the kernel driver
*/
typedef struct {
    uint32_t    debug;
    struct device   *dev;
    /* Char device structure */
    struct cdev cdev;
    /* Major minor information */
    dev_t       dev_t;
    /* IRQ numbers */
    uint32_t    vspa_irq_no;
    /* user pid */
    int gUserPid;

    vspaControl_t      vspaCtrl;
    atuControl_t       atuCtrl;

    char        eldFilename[VSPA_MAX_ELD_FILENAME];
} vspaDevice_t;



#endif /* _VSPA_H */
