/*
 * Copyright 2016-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "lax_os.h"
#include "lax_driver.h"

#include "rsdk_lax_common.h"
#include "oal_log.h"

#include <linux/types.h>
#include <lax_uapi.h>

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/fs.h>


/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 ==================================================================================================*/
static int __init LaxDriverInit(void) 
{ 
    return LaxModInit(); 
}

static void __exit LaxDriverRelease(void) 
{ 
    LaxModExit(); 
}

module_init(LaxDriverInit);
module_exit(LaxDriverRelease);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("NXP Semiconductors");
MODULE_DESCRIPTION("NXP LAX Driver");
MODULE_VERSION("3.00");