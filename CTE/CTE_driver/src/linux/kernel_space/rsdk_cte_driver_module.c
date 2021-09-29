/*
 * Copyright 2020-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <linux/types.h>
#include <linux/stddef.h>

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>

#include "rsdk_S32R45.h"
#include "rsdk_cte_driver_module.h"
#include "rsdk_cte_interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CTE_KERNEL_DEBUG_MODE_      // remove final _ for debug version, or compile with the appropriate specs

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define DEVICE_NAME "rsdk_cte" /* Dev name as it appears in /proc/devices   */


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("NXP Semiconductors");
MODULE_DESCRIPTION("NXP CTE Driver");
MODULE_VERSION("1.03");

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
static s32                       gsNumRsdkCteMajor;
static s32                       gsNumRsdkCteMinor;
static dev_t                     gsDevNum;
static struct class *            gspRsdkCteClass;
static const struct of_device_id gsRsdkCteMatch[] = {
    {
        .compatible = "nxp,cte",
    },
    {},
};

rsdkCteDevice_t *gpRsdkCteDevice = NULL;

/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/

/******************************************************************************/
/**
 * @brief   Open CTE device
 */
static int RsdkCteOpen(struct inode *pInode, struct file *fp)
{
    rsdkCteDevice_t *pRsdkCteDevice = container_of(pInode->i_cdev, rsdkCteDevice_t, cdevRef);
    int32_t           err = 0;

    if (pRsdkCteDevice != NULL)
    {
        fp->private_data = pRsdkCteDevice;
        pRsdkCteDevice->gUserPid = (uint32_t)current->pid;
#ifdef CTE_KERNEL_DEBUG_MODE
        (void)pr_alert("CTE_driver module: RsdkCteOpen: OK, PID = %d\n", pRsdkCteDevice->gUserPid);
#endif
    }
    else
    {
        (void)pr_err("RsdkCteOpen: No device context found for %s %d\n", DEVICE_NAME, iminor(pInode));
        err = -ENODEV;
    }

    return err;
}

/******************************************************************************/
/**
 * @brief   Release/close device
 */
static int RsdkCteRelease(struct inode *pInode, struct file *fp)
{
    rsdkCteDevice_t *pRsdkCteDevice = (rsdkCteDevice_t *)fp->private_data;
    int32_t           err = 0;

    if (pRsdkCteDevice == NULL)
    {
        (void)pr_err("RsdkCteRelease: No device context found for %s %d\n", DEVICE_NAME, iminor(pInode));
        err = -ENODEV;
    }
    else
    {
#ifdef CTE_KERNEL_DEBUG_MODE
        (void)pr_alert("RsdkCteRelease: OK.\n");
#endif
    }
    return err;
}

/**
 * @brief   Get dts properties
 */
static int32_t RsdkCteGetDtsProperties(struct device_node *pNode, rsdkCteDtsInfo_t *dtsInfo)
{
    int32_t err = 0;

    u32             prop = 0;
    struct resource res;

    if (pNode == NULL)
    {
        err = -EINVAL;
    }
    else
    {
#ifdef CTE_KERNEL_DEBUG_MODE
        (void)pr_alert("NODE ID : %s, %d, %d\n", pNode->properties->next->next->name,
                       pNode->properties->next->next->unique_id, *(int *)(pNode->properties->next->next->value));
#endif
        if (of_property_read_u32(pNode, "ctedev-id", &prop) < 0)
        {
            (void)pr_err("RsdkCteGetDtsProperties: ctedev-id attribute not found\n");
            err = -EINVAL;
        }
    }
    if (err == 0)
    {
        dtsInfo->devId = (int32_t)prop;
        if (of_get_property(pNode, "interrupts", NULL) != NULL)
        {
            dtsInfo->irqId = irq_of_parse_and_map(pNode, 0);  //DPHY errors interrupt

            if (dtsInfo->irqId == 0u)
            {
                (void)pr_err("RsdkCteGetDtsProperties: interrupt not found for %s_%d\n", DEVICE_NAME,
                             dtsInfo->devId);
                err = -EINVAL;
            }
            else
            {
#ifdef CTE_KERNEL_DEBUG_MODE
                (void)pr_alert("RsdkCteGetDtsProperties: all interrupt number for %s_%d found\n", DEVICE_NAME,
                               dtsInfo->devId);
#endif
            }
        }
        else
        {
            (void)pr_err("RsdkCteGetDtsProperties: Interrupt numbers not found for %s_%d\n", DEVICE_NAME,
                         dtsInfo->devId);
            err = -EINVAL;
        }
    }

    if (err == 0)
    {
        if (of_address_to_resource(pNode, 0, &res) != 0)
        {
            (void)pr_err("RsdkCteGetDtsProperties: Reg map not found for %s_%d\n", DEVICE_NAME, dtsInfo->devId);
            err = -EINVAL;
        }
        else
        {
            dtsInfo->pMemMapBaseAddr = (u32 __iomem *)res.start;
            dtsInfo->memSize = (uint32_t)resource_size(&res);
#ifdef CTE_KERNEL_DEBUG_MODE
            (void)pr_alert("RsdkCteGetDtsProperties: Phy reg map for %s%d: base addr=0x%lx, size=%d\n", DEVICE_NAME,
                           dtsInfo->devId, (unsigned long)dtsInfo->pMemMapBaseAddr, dtsInfo->memSize);
#endif
        }
    }

    return err;
}

/**
 * @brief   Device probe
 */
static int RsdkCteProbe(struct platform_device *pPlatDev)
{
    static const struct file_operations gRsdkCteFileOps = {
        .owner = THIS_MODULE,
        .open = RsdkCteOpen,
        .release = RsdkCteRelease,
    };

    int32_t             err;
    uint32_t            val;
    struct device *     pDevice = &pPlatDev->dev;
    struct device_node *pNode = pPlatDev->dev.of_node;
    struct device *     pSysFsDev;
    rsdkCteDevice_t *   pRsdkCteDev;
    dev_t               devNo;

#ifdef CTE_KERNEL_DEBUG_MODE
    (void)pr_alert("RsdkCteProbe: %s, %s / %d\n", DEVICE_NAME, pNode->full_name, pNode->properties->unique_id);
#endif
    BUG_ON((gsNumRsdkCteMajor == 0) || (gspRsdkCteClass == NULL));

    /* Allocate CTE device structure */
    pRsdkCteDev = kzalloc(sizeof(rsdkCteDevice_t), GFP_KERNEL);
    err = RsdkCteGetDtsProperties(pNode, &pRsdkCteDev->dtsInfo);
    if (err < 0)
    {
        kvfree(pRsdkCteDev);
        (void)pr_err("RsdkCteProbe: CTE DTS entry parse failed.\n");
        err = -EINVAL;
    }
    else
    {
#ifdef CTE_KERNEL_DEBUG_MODE
        (void)pr_alert("RsdkCteProbe: GetDtsProperties() OK -> id=%d\n", pRsdkCteDev->dtsInfo.devId);
#endif
        gpRsdkCteDevice = pRsdkCteDev;      // keep the device pointer for future use
        pRsdkCteDev->gUserPid = 0xffffffffu;                            // wrong pid
        devNo = MKDEV(gsNumRsdkCteMajor, gsNumRsdkCteMinor + pRsdkCteDev->dtsInfo.devId);
        dev_set_drvdata(pDevice, pRsdkCteDev);

        //initialize char device
        cdev_init(&pRsdkCteDev->cdevRef, &gRsdkCteFileOps);

        err = cdev_add(&(pRsdkCteDev->cdevRef), devNo, 1);
        if (err < 0)
        {
            (void)pr_err("RsdkCteProbe: Error %d in cdev_add() while adding %s", err, DEVICE_NAME);
        }
        else
        {
            // Create sysfs device
#ifdef CTE_KERNEL_DEBUG_MODE
            (void)pr_alert("RsdkCteProbe: cdev_add() OK.\n");
#endif
            pSysFsDev = device_create(gspRsdkCteClass, pDevice, devNo, NULL, DEVICE_NAME);
            if (IS_ERR(pSysFsDev))
            {
                err = (int32_t)PTR_ERR(pSysFsDev);
                (void)pr_err("RsdkCteProbe: Error %d in device_create() for %s", err, DEVICE_NAME);

                //revert previous actions:
                cdev_del(&pRsdkCteDev->cdevRef);
            }
            else
            {
#ifdef CTE_KERNEL_DEBUG_MODE
                (void)pr_alert("RsdkCteProbe: device_create() OK.\n");
#endif
            }
        }
        if (err < 0)
        {
            kvfree(pRsdkCteDev);
        }
    }

    //finished creating the device, now bind the specific hardware resources to it - register map, interrupts:
    if (err == 0)
    {
        pRsdkCteDev->pMemMapVirtAddr =
            (uint32_t *)ioremap((phys_addr_t)pRsdkCteDev->dtsInfo.pMemMapBaseAddr, pRsdkCteDev->dtsInfo.memSize);
        if (pRsdkCteDev->pMemMapVirtAddr == NULL)
        {
            err = -EFAULT;
            (void)pr_err("RsdkCteProbe: ioremap failed for CTE Registry Address!.\n");
        }
        if(err == 0)
        {
            //test the register access:
            pRsdkCteDev->pMemMapVirtAddr[CTE_REG_DBG_OFFSET_U32] = 0xffffffffu;  // write to debug registry
            val = pRsdkCteDev->pMemMapVirtAddr[CTE_REG_DBG_OFFSET_U32];
            pRsdkCteDev->pMemMapVirtAddr[CTE_REG_DBG_OFFSET_U32] = 0xffffffffu;  // write to debug registry
            val = pRsdkCteDev->pMemMapVirtAddr[CTE_REG_DBG_OFFSET_U32];
            if (val != 0u)
            {
                (void)pr_err("RsdkCteProbe: ioremap error - phys=%lx, virt=%lx, test val=%x.\n",
                             (uintptr_t)pRsdkCteDev->dtsInfo.pMemMapBaseAddr, 
                             (uintptr_t)pRsdkCteDev->pMemMapVirtAddr, val);
                err = -EFAULT;
                iounmap(pRsdkCteDev->pMemMapVirtAddr);
            }
            else
            {
#ifdef CTE_KERNEL_DEBUG_MODE
                (void)pr_alert("RsdkCteProbe: ioremap CTE -> phys=%lx, virt=%lx.\n",
                               (unsigned long)pRsdkCteDev->dtsInfo.pMemMapBaseAddr,
                               (unsigned long)pRsdkCteDev->pMemMapVirtAddr);
#endif
				pRsdkCteDev->pSrc_1 = (uint32_t *)ioremap((phys_addr_t)&SRC_1, 0x100); // get the SRC_1 mapping
#ifdef CTE_KERNEL_DEBUG_MODE
                (void)pr_alert("RsdkCteProbe: ioremap SRC_1 -> phys=%lx, virt=%lx.\n",
                               (unsigned long)&SRC_1,
                               (unsigned long)pRsdkCteDev->pSrc_1);
#endif
            }
        }
        if (err != 0)
        {
            //revert previous actions:
            device_destroy(gspRsdkCteClass, devNo);
            cdev_del(&pRsdkCteDev->cdevRef);
            kvfree(pRsdkCteDev);
        }
    }

    if (err == 0)
    {
        err = request_irq(pRsdkCteDev->dtsInfo.irqId, RsdkCteIrqHandlerLinux, 0, DEVICE_NAME, pRsdkCteDev);
        //request_irq returns negative values in case of errors
        if (err < 0)
        {
            (void)pr_err("RsdkCteProbe: %s - request_irq() has failed.\n", DEVICE_NAME);
            //revert previous actions:
            err = -EINTR;
        }
        else
        {
#ifdef CTE_KERNEL_DEBUG_MODE
            (void)pr_alert("RsdkCteProbe: Registered IRQ handler (irq.-%d).\n", pRsdkCteDev->dtsInfo.irqId);
#endif
            //initialize a single wait queue for sending interrupt events into user space
            err = OAL_InitWaitQueue(&(pRsdkCteDev->irqWaitQ));
            if (err < 0)
            {
                (void)pr_err("RsdkCteProbe: OAL_InitWaitQueue failed!.\n");
                (void)free_irq(pRsdkCteDev->dtsInfo.irqId, pRsdkCteDev);
            }
            else
            {
#ifdef CTE_KERNEL_DEBUG_MODE
                (void)pr_alert("RsdkCteProbe: OAL_InitWaitQueue ok.\n");
#endif
            }
        }
        if (err != 0)
        {
            iounmap(pRsdkCteDev->pMemMapVirtAddr);
            device_destroy(gspRsdkCteClass, devNo);
            cdev_del(&pRsdkCteDev->cdevRef);
            kvfree(pRsdkCteDev);
        }
    }

    if (err == 0)
    {
        err = RsdkCteRpcSrvInit();
        if (err != 0)
        {
            (void)pr_err("RsdkCteProbe: RsdkCteRpcSrvInit() failed, err = %d\n", err);
            pRsdkCteDev->gspRpcServ = NULL;
        }
        else
        {
            pRsdkCteDev->registeredEvents = 0;
#ifdef CTE_KERNEL_DEBUG_MODE
            (void)pr_alert("RsdkCteProbe: RsdkCteRpcSrvInit() ok.\n");
#endif
        }
    }

    return err;
}

/**
 * @brief   Device remove
 */
static int RsdkCteRemove(struct platform_device *ofpdev)
{
    rsdkCteDevice_t *pRsdkCteDev = dev_get_drvdata(&ofpdev->dev);

    (void)pr_alert("RsdkCteRemove: START \n");

    dev_set_drvdata(&ofpdev->dev, NULL);

    (void)CtePlatformUnregisterEvents();
    (void)RsdkCteRpcSrvExit();
    (void)OAL_DestroyWaitQueue(&(pRsdkCteDev->irqWaitQ));
    (void)free_irq(pRsdkCteDev->dtsInfo.irqId, pRsdkCteDev);
    iounmap(pRsdkCteDev->pMemMapVirtAddr);
    device_destroy(gspRsdkCteClass, MKDEV(gsNumRsdkCteMajor, gsNumRsdkCteMinor + pRsdkCteDev->dtsInfo.devId));
    cdev_del(&pRsdkCteDev->cdevRef);
    kvfree(pRsdkCteDev);

    return 0;
}

static struct platform_driver gsRsdkCteDriver = {
    .driver =
        {
            .name = "nxp-cte",
            .owner = THIS_MODULE,
            .of_match_table = gsRsdkCteMatch,
        },
    .probe = RsdkCteProbe,
    .remove = RsdkCteRemove,
};

/**
 * @brief   Module init
 */
static int __init RsdkCteModInit(void)
{
    int32_t err;

    /* Register our major, and accept a dynamic number. */
    err = alloc_chrdev_region(&gsDevNum, 0, 1, DEVICE_NAME);
    if (err < 0)
    {
        (void)pr_err("RsdkCteModInit: can't get major number = %d\n", err);
    }
    else  //alloc_chrdev_region() OK
    {
        gsNumRsdkCteMajor = (s32)MAJOR(gsDevNum);
        gsNumRsdkCteMinor = (s32)MINOR(gsDevNum);

        /* Create the device class if required */
        gspRsdkCteClass = class_create(THIS_MODULE, DEVICE_NAME);
        if (IS_ERR(gspRsdkCteClass))
        {
            unregister_chrdev_region((uint32_t)gsNumRsdkCteMajor, 0);
            err = (int32_t)PTR_ERR(gspRsdkCteClass);
            (void)pr_err("RsdkCteModInit: class_create() failed = %d\n", err);
        }
        else  //class_create() OK
        {
#ifdef CTE_KERNEL_DEBUG_MODE
            (void)pr_alert("RsdkCteModInit: class_create() OK.\n");
#endif
            err = platform_driver_register(&gsRsdkCteDriver);
            if (err != 0)
            {
                (void)pr_err("RsdkCteModInit: platform_driver_register() failed = %d\n", err);
                class_destroy(gspRsdkCteClass);
                unregister_chrdev_region(gsNumRsdkCteMajor, 1);
            }
            else  // platform_driver_register() OK
            {
                (void)pr_alert("RsdkCteModInit: CTE driver module init OK.\n");
            }
        }
    }

    return err;
}

/**
 * @brief   Module exit
 */
static void __exit RsdkCteModExit(void)
{
    platform_driver_unregister(&gsRsdkCteDriver);
    class_destroy(gspRsdkCteClass);
    unregister_chrdev_region(gsDevNum, 1);
    (void)pr_alert("RsdkCteModExit: CTE driver exit done. \n");
}

module_init(RsdkCteModInit);
module_exit(RsdkCteModExit);

#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
