/*
 * Copyright 2016-2023 NXP
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
#include <linux/clk.h>


/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/* Additional options for checking if Mailbox write to LAX completed */
#ifdef LAX_DEBUG
#define MB_CHECK_ON_WRITE    /* Check when queuing a new mailbox write */
#define MB_CHECK_IN_IRQ    /* Check during Mailbox IRQ processing    */
#define MB_CHECK_TIMER    /* Use timer to keep checking after MB OUT*/
#endif

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
static int gsCmdBufferBytes = (int)(RSDK_LAX_CMD_BUF_SZ_WORDS_32 << 2u);
module_param(gsCmdBufferBytes, int, 0644);
MODULE_PARM_DESC(gsCmdBufferBytes, 
                "Size of cmdbuf(bytes), default: RSDK_LAX_CMD_BUF_SZ_WORDS_32 as defined in rsdk_lax_config.h");

static int gsSpmBufferBytes = 4096;
module_param(gsSpmBufferBytes, int, 0644);
MODULE_PARM_DESC(gsSpmBufferBytes, "Size of spm buf(bytes), default: 4096");

/* Number of LAX devices probed on system */
static int32_t gsNumLaxDevs;
static s32 gsNumLaxMajor;
static s32 gsNumLaxMinor;
static struct class *gspLaxClass;
static dev_t gsDevNo;


/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/
static int LaxOpen(struct inode *pInode, struct file *fp);
static int LaxRelease(struct inode *pInode, struct file *fp);
static int LaxProbe(struct platform_device *pdev);
static int LaxRemove(struct platform_device *ofpdev);


static const struct of_device_id gsLaxMatch[] = 
{
    {.compatible = "nxp,lax",},
    {},
};

static struct platform_driver gsLaxDriver = 
{
    .driver = 
    {
        .name = "nxp-lax",
        .owner = THIS_MODULE,
        .of_match_table = gsLaxMatch,
    },
    .probe = LaxProbe,
    .remove = LaxRemove,
};



/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 ==================================================================================================*/
/******************************************************************************/
/**
 * @brief   Open LAX device
 */
static int LaxOpen(struct inode *pInode, struct file *fp)
{
    laxDevice_t     *pLaxDev;
    lldLaxControl_t *pLaxCtrl;
    int32_t         rc;
    
    pLaxDev = container_of(pInode->i_cdev, laxDevice_t, chDev);
    
    if (pLaxDev != NULL) 
    {
        fp->private_data = pLaxDev;
        pLaxDev->gUserPid = current->pid;
        pLaxCtrl = &pLaxDev->laxCtrl;
        gOalCommLaxCtrl[iminor(pInode)] = pLaxCtrl;
        rc = 0;
    } 
    else
    {
        LAX_LOG_INFO("No device context found for %s %d\n", LAX_DEVICE_NAME, iminor(pInode));
        rc = -ENODEV;
    }
    return rc;
}// LaxOpen()


/******************************************************************************/
/**
 * @brief   release/close LAX device
 */
static int LaxRelease(struct inode *pInode, struct file *fp)
{
    laxDevice_t *pLaxDev;
    int32_t     rc;

    rc = 0;
    pLaxDev = (laxDevice_t *)fp->private_data;
    
    if (pLaxDev != NULL) 
    {
        fp->private_data = pLaxDev;
        pLaxDev->gUserPid = current->pid;
    } 
    else
    {
        LAX_LOG_INFO("No device context found for %s %d\n", LAX_DEVICE_NAME, iminor(pInode));
        rc = -ENODEV;
    }
    return rc;
}// LaxRelease()

/******************************************************************************/
/**
 * @brief   Get dts properties
 */
static int32_t LaxGetDtsProperties(struct device_node *pNode, laxDevice_t *pLaxDev)
{
    u32 prop  = 0;
    dma_addr_t prop1 = 0;
    struct resource res;
    lldLaxControl_t *pLaxCtrl = &pLaxDev->laxCtrl;
    int32_t err = 0;

    if (pNode == NULL)
    {
        err = -EINVAL;
    }
    else
    {
        if (of_property_read_u32(pNode, "laxdev-id", &prop) < 0) 
        {
            dev_err(pLaxDev->dev, "laxdev-id attribute not found\n");
            err = -EINVAL;
        }
        pLaxCtrl->id = (int32_t)prop;

        if (of_property_read_u64(pNode, "dbgregstart", &prop1) < 0) 
        {
            dev_err(pLaxDev->dev, "dbgregstart attribute not found for %s%d\n", LAX_DEVICE_NAME, pLaxCtrl->id);
            err = -EINVAL;
        }
    }
    if (err == 0)
    {
        pLaxCtrl->pDbgAddr = (u32 *)(dma_addr_t)prop1;

        if (of_property_read_u32(pNode, "dbgreglen", &prop) < 0) 
        {
            dev_err(pLaxDev->dev, "dbgreglen attribute not found for %s%d\n", LAX_DEVICE_NAME, pLaxCtrl->id);
            err = -EINVAL;
        }
    }
    if(err == 0)
    {
        if (of_get_property(pNode, "interrupts", NULL) != NULL) 
        {
            pLaxDev->laxIrqNo[0]    = irq_of_parse_and_map(pNode, 0); // ORed Functional Interrupt.
            pLaxDev->laxIrqNo[1]    = irq_of_parse_and_map(pNode, 1); //LAX_DMA_ERR or VCPU_IIT interrupt.
            if ((pLaxDev->laxIrqNo[0] == 0u) || (pLaxDev->laxIrqNo[1] == 0u)) 
            {
                dev_err(pLaxDev->dev, "Interrupt not found for %s%d\n", LAX_DEVICE_NAME, pLaxCtrl->id);
                err = -EINVAL;
            }
        } 
        else 
        {
            dev_err(pLaxDev->dev, "Interrupt numbers not found for %s%d\n", LAX_DEVICE_NAME, pLaxCtrl->id);
            err = -EINVAL;
        }
    }
    if(err == 0)
    {
        if (of_address_to_resource(pNode, 0, &res) != 0) 
        {
            dev_err(pLaxDev->dev, "LAX reg map not found for %s%d\n", LAX_DEVICE_NAME, pLaxCtrl->id);
            err = -EINVAL;
        } 
        else 
        {
            pLaxCtrl->pMemAddr = (u32 __iomem *)res.start;
            pLaxCtrl->memSize =  (uint32_t)resource_size(&res);
        }
    }

    return err;
}// LaxGetDtsProperties()


/******************************************************************************/
/**
 * @brief   Declared LAX irq handler
 */
static irqreturn_t LaxDevIrqHandler(int irq, void *pDev) 
{
    lldLaxControl_t *pLaxCtrl;
    
    pLaxCtrl = &(((laxDevice_t *)pDev)->laxCtrl);
    return LaxIrqHandler(irq, pLaxCtrl);
}// LaxDevIrqHandler()


/******************************************************************************/
/**
 * @brief   Device probe
 */
static int LaxProbe(struct platform_device *pdev)
{
    static const struct file_operations gsLaxFops = 
    {
        .owner          = THIS_MODULE,
        .open           = LaxOpen,
        .release        = LaxRelease,
    };
    laxDevice_t        *pLaxDev;
    lldLaxControl_t    *pLaxCtrl;
    struct device_node *pNode;
    struct device      *pToDev;
    struct device      *pSysFsDev;
    struct clk         *pClk;
    dev_t                devNo;
    u8                 *deviceName;
    int32_t             err;
    
    BUG_ON((gsNumLaxMajor == 0) || (gspLaxClass == NULL));

    pNode = pdev->dev.of_node;
    pToDev = &pdev->dev;
    err = 0;

    // initialize the clock
    LAX_LOG_INFO("RsdkLaxProbe: clock find - lax_module_clk.\n");
    printk("RsdkLaxProbe: clock find - lax_module_clk.\n");
    pClk = devm_clk_get(pToDev, "lax_module_clk");
    LAX_LOG_INFO("RsdkLaxProbe: clock find result - %x.\n", pClk);
    printk("RsdkLaxProbe: clock find result - %x.\n", pClk);
    if(IS_ERR(pClk))
    {   // clock not found
        LAX_LOG_ERROR("RsdkLaxProbe: clock find error for lax_module_clk.\n");
        err = -EFAULT;
    }
    else
    {   // clock found
        if(clk_prepare_enable(pClk) != 0)
        {
            LAX_LOG_ERROR("RsdkLaxProbe: clock start error for lax_module_clk.\n");
            err = -EFAULT;
        }
    }

    if(err == 0)
    {
        deviceName = devm_kmalloc(&pdev->dev, 10, GFP_KERNEL );
        /* Allocate LAX device structure */
        pLaxDev = devm_kzalloc(&pdev->dev, sizeof(laxDevice_t), GFP_KERNEL);
        if ((pLaxDev == NULL) || (deviceName == NULL)) 
        {
            LAX_LOG_INFO(": failed to allocate lax_dev or deviceName\n");
            err = -ENOMEM;
        }
    }
    if(err == 0)
    {
        pLaxDev->dev = pToDev;

        err = LaxGetDtsProperties(pNode, pLaxDev);
        if (err < 0) 
        {
            LAX_LOG_INFO("LAX DTS entry parse failed.\n");
            err = -EINVAL;
        }
        else
        {
            pLaxCtrl = &pLaxDev->laxCtrl;

            devNo = MKDEV(gsNumLaxMajor, gsNumLaxMinor + pLaxCtrl->id);
            (void)sprintf(deviceName, LAX_DEVICE_NAME "%d", pLaxCtrl->id);
            dev_set_drvdata(&pdev->dev, pLaxDev);

            if(RSDK_SUCCESS != LaxLowLevelDriverInit(pLaxCtrl))
            {
                err = -EINVAL;
            }
            else
            {
                /* register the same interrupt handler for all interrupt lines */
                err = request_irq(pLaxDev->laxIrqNo[0], LaxDevIrqHandler, 0, deviceName, pLaxDev);
                err += request_irq(pLaxDev->laxIrqNo[1], LaxDevIrqHandler, 0, deviceName, pLaxDev);
                if (err < 0) 
                {
                    LAX_LOG_INFO("%s: request_irq() err = %d\n", deviceName, err);
                }
                else
                {
                    cdev_init(&pLaxDev->chDev, &gsLaxFops);
                    pLaxDev->chDev.owner = THIS_MODULE;

                    err = cdev_add(&pLaxDev->chDev, devNo, 1);
                    if (err < 0)
                    {
                        LAX_LOG_INFO("Error %d while adding %s", err, deviceName);
                    }
                    else
                    {
                        /* Create sysfs device */
                        pSysFsDev = device_create(gspLaxClass, pToDev, devNo, NULL, deviceName);
                        if (IS_ERR(pSysFsDev))
                        {
                            err = (int32_t)PTR_ERR(pSysFsDev);
                            LAX_LOG_INFO("Error %d while creating %s", err, deviceName);
                        }
                        else
                        {
                            if (0 == gsNumLaxDevs)
                            {
                               if(LaxOalCommInit() == RSDK_SUCCESS)
                               {
                                    gsNumLaxDevs++;
                               }
                            }
                        }
                        if(err < 0)
                        {
                            cdev_del(&pLaxDev->chDev);
                        }
                        else
                        {
                            LAX_LOG_INFO("Lax driver %d initialized.", devNo);
                        }
                    }
                    if(err < 0)
                    {
                        (void)free_irq(pLaxDev->laxIrqNo[0], pLaxDev);
                        (void)free_irq(pLaxDev->laxIrqNo[1], pLaxDev);
                    }
                }
                if(err < 0)
                {
                    if(RSDK_SUCCESS != LaxDeInit(pLaxCtrl))
                    {
                        LAX_LOG_ERROR ("Error in LaxDeInit");
                    }
                }
            }
        }
    }
    return err;
}// LaxProbe()


/******************************************************************************/
/**
 * @brief   Device remove
 */
static int LaxRemove(struct platform_device *ofpdev)
{
    struct device   *pDev;
    laxDevice_t     *pLaxDev;
    lldLaxControl_t    *pLaxCtrl;
    pLaxDev = dev_get_drvdata(&ofpdev->dev);
    pDev = &ofpdev->dev;
    pLaxCtrl = &pLaxDev->laxCtrl;

    BUG_ON((pLaxDev == NULL) || (gspLaxClass == NULL));
    
    (void)free_irq(pLaxDev->laxIrqNo[0], pLaxDev);
    (void)free_irq(pLaxDev->laxIrqNo[1], pLaxDev);

    dev_set_drvdata(&ofpdev->dev, NULL);

    if(RSDK_SUCCESS != LaxDeInit(pLaxCtrl))
    {
        LAX_LOG_ERROR ("Error in LaxDeInit");
    }
    
    device_destroy(gspLaxClass, MKDEV(gsNumLaxMajor, gsNumLaxMinor + pLaxCtrl->id));
    cdev_del(&pLaxDev->chDev);

    if (gsNumLaxDevs > 0)
    {
        gsNumLaxDevs--;
        if (gsNumLaxDevs == 0)
        {
            if(LaxOalCommExit() != RSDK_SUCCESS)
            {
                LAX_LOG_ERROR("LaxOalCommExit failed \n");
            }
        }
    }
    return 0;
}// LaxRemove()


/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 ==================================================================================================*/

/******************************************************************************/
/**
 * @brief   Driver/module init
 */
int LaxModInit(void)
{
    int32_t err;

    /* Register our major, and accept a dynamic number. */
    err = alloc_chrdev_region(&gsDevNo, 0, RSDK_LAX_CORES_NUM, LAX_DEVICE_NAME);
    if (err < 0) 
    {
        LAX_LOG_ERROR("lax: can't get major number: %d\n", err);
    }
    else
    {
        gsNumLaxMajor = MAJOR(gsDevNo);
        gsNumLaxMinor = MINOR(gsDevNo);

        /* Create the device class if required */
        gspLaxClass = class_create(THIS_MODULE, LAX_DEVICE_NAME);
        if (IS_ERR(gspLaxClass)) 
        {
            unregister_chrdev_region(gsDevNo, RSDK_LAX_CORES_NUM);
            err = (int32_t)PTR_ERR(gspLaxClass);
            LAX_LOG_ERROR("lax: class_create() failed %d\n", err);
        }
        else
        {
            err = platform_driver_register(&gsLaxDriver);
            if (err != 0)
            {
                class_destroy(gspLaxClass);
                unregister_chrdev_region(gsDevNo, RSDK_LAX_CORES_NUM);
            }
        }
    }
    return err;
}// LaxModInit()


/******************************************************************************/
/**
 * @brief   Driver/module exit
 */
void LaxModExit(void)
{
    platform_driver_unregister(&gsLaxDriver);
    class_destroy(gspLaxClass);
    unregister_chrdev_region(gsDevNo, RSDK_LAX_CORES_NUM);
}// LaxModExit
