/*
 * Copyright 2016-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LAX_OS_H_
#define LAX_OS_H_

/*=================================================================================================
*                                        INCLUDE FILES
=================================================================================================*/
#include "lax_driver.h"

#include <linux/cdev.h>

#ifdef __cplusplus
extern "C" {
#endif


/*=================================================================================================
*                                      DEFINES AND MACROS
=================================================================================================*/
/* Debug and error reporting macros */
#define IF_DEBUG(x)     if (pLaxDev->debug & (x))                       \
    
#define ERR(...)        {                                               \
                            if (pLaxDev->debug & DEBUG_MESSAGES)        \
                            {                                           \
                                pr_err(LAX_DEVICE_NAME __VA_ARGS__);    \
                            }                                           \
                        }                                               \

#define LAX_DEVICE_NAME     "lax"


#define LAX_DMA_CHANNELS    (32)


/* mmap offset argument for LAX regsiters */
#define LAX_REG_OFFSET      (0)

/* mmap offset argument for dbg regsiter */
#define LAX_DBG_OFFSET      (4096)

#define BD_ENTRIES          (16)

#define LAX_IRQ_NUM         2u


/*=================================================================================================
*                                          CONSTANTS
=================================================================================================*/

/*=================================================================================================
*                                             ENUMS
=================================================================================================*/

/*=================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
=================================================================================================*/
/* The below structure contains all the information for the
* LAX device required by the kernel driver
*/
typedef struct {
    uint32_t        debug;
    struct device   *dev;
    /* Char device structure */
    struct cdev     chDev;
    /* Major minor information */
    dev_t           devT;
    /* IRQ numbers */
    unsigned int    laxIrqNo[LAX_IRQ_NUM];
    /* user pid */
    int32_t         gUserPid;

    lldLaxControl_t    laxCtrl;
} laxDevice_t;


/*=================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
=================================================================================================*/
extern const struct attribute_group gAttrGroup;

/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/
int  LaxModInit(void);
void LaxModExit(void);

#ifdef __cplusplus
}
#endif


#endif /* LAX_OS_H_ */
