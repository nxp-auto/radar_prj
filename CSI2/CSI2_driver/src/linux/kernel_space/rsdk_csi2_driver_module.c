/*
 * Copyright 2019-2023 NXP
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
#include <linux/clk.h>

#include "Csi2_Driver_Module.h"
#include "Csi2_Interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_MODE

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define DEVICE_NAME "rsdk_csi2" /* Dev name as it appears in /proc/devices   */
#define DFS_PHY_ADDRESS 0x40054000u
#define DFS_MEM_LEN 0x40u

// masks for irq installation
#define CSI2_LX_IRQ0_MASK (1LU << 0u)
#define CSI2_LX_IRQ1_MASK (1LU << 1u)
#define CSI2_LX_IRQ2_MASK (1LU << 2u)

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("NXP Semiconductors");
MODULE_DESCRIPTION("NXP MIPICSI2 Driver");
MODULE_VERSION("4.1");


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
static s32                       gsNumRsdkCsi2Major;
static s32                       gsNumRsdkCsi2Minor;
static dev_t                     gsDevNum;
static struct class             *gspRsdkCsi2Class;
static const struct of_device_id gsRsdkCsi2Match[] = {
    {
        .compatible = "nxp,mipicsi2",
    },
    {},
};

rsdkCsi2Device_t *gpRsdkCsi2Device[RSDK_MIPICSI2_INTERFACES] = {NULL, NULL, NULL, NULL};

/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/

/******************************************************************************/
/**
 * @brief   Open CSI2 device
 */
static int RsdkCsi2Open(struct inode *pInode, struct file *pFile)
{
    rsdkCsi2Device_t *pRsdkCsi2Device = container_of(pInode->i_cdev, rsdkCsi2Device_t, cdevRef);
    int32_t           err = 0;

    if (pRsdkCsi2Device != NULL)
    {
        pFile->private_data = pRsdkCsi2Device;
        pRsdkCsi2Device->gUserPid = (uint32_t)current->pid;
#ifdef DEBUG_MODE
        (void)pr_alert("CSI2_driver module: RsdkCsi2Open: OK, PID = %d\n", pRsdkCsi2Device->gUserPid);
#endif
    }
    else
    {
        (void)pr_err("RsdkCsi2Open: No device context found for %s %d\n", DEVICE_NAME, iminor(pInode));
        err = -ENODEV;
    }

    return err;
}

/******************************************************************************/
/**
 * @brief   Release/close device
 */
static int RsdkCsi2Release(struct inode *pInode, struct file *pFile)
{
    rsdkCsi2Device_t *pRsdkCsi2Device = (rsdkCsi2Device_t *)pFile->private_data;
    int32_t           err = 0;

    if (pRsdkCsi2Device == NULL)
    {
        (void)pr_err("RsdkCsi2Release: No device context found for %s %d\n", DEVICE_NAME, iminor(pInode));
        err = -ENODEV;
    }
    else
    {
#ifdef DEBUG_MODE
        (void)pr_alert("RsdkCsi2Release: OK.\n");
#endif
    }
    return err;
}

/******************************************************************************/
/**
 * @brief   Get dts properties
 */
static int32_t RsdkCsi2GetDtsProperties(struct device_node *pNode, rsdkCsi2DtsInfo_t *dtsInfo)
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
#ifdef DEBUG_MODE
        (void)pr_alert("NODE ID : %s, %d, %d\n", pNode->properties->next->next->name,
                       pNode->properties->next->next->unique_id, *(uint32_t *)(pNode->properties->next->next->value));
#endif
        if (of_property_read_u32(pNode, "mipicsi2dev-id", &prop) < 0)
        {
            (void)pr_err("RsdkCsi2GetDtsProperties: mipicsi2dev-id attribute not found\n");
            err = -EINVAL;
        }
    }
    if (err == 0)
    {
        dtsInfo->devId = (int32_t)prop;
        if (of_get_property(pNode, "interrupts", NULL) != NULL)
        {
            dtsInfo->irqId[0] = irq_of_parse_and_map(pNode, 0);  //DPHY errors interrupt
            dtsInfo->irqId[1] = irq_of_parse_and_map(pNode, 1);  //Rx errors interrupt
            dtsInfo->irqId[2] = irq_of_parse_and_map(pNode, 2);  //Events interrupt

            if ((dtsInfo->irqId[0] == 0u) || (dtsInfo->irqId[1] == 0u) || (dtsInfo->irqId[2] == 0u))
            {
                (void)pr_err("RsdkCsi2GetDtsProperties: at least one interrupt not found for %s_%d\n", DEVICE_NAME,
                             dtsInfo->devId);
                err = -EINVAL;
            }
            else
            {
#ifdef DEBUG_MODE
                (void)pr_alert("RsdkCsi2GetDtsProperties: all interrupt number for %s_%d found\n", DEVICE_NAME,
                               dtsInfo->devId);
#endif
            }
        }
        else
        {
            (void)pr_err("RsdkCsi2GetDtsProperties: Interrupt numbers not found for %s_%d\n", DEVICE_NAME,
                         dtsInfo->devId);
            err = -EINVAL;
        }
    }

    if (err == 0)
    {
        if (of_address_to_resource(pNode, 0, &res) != 0)
        {
            (void)pr_err("RsdkCsi2GetDtsProperties: Reg map not found for %s_%d\n", DEVICE_NAME, dtsInfo->devId);
            err = -EINVAL;
        }
        else
        {
            dtsInfo->pMemMapBaseAddr = (u32 __iomem *)res.start;
            dtsInfo->memSize = (uint32_t)resource_size(&res);
#ifdef DEBUG_MODE
            (void)pr_alert("RsdkCsi2GetDtsProperties: Phy reg map for %s%d: base addr=0x%lx, size=%d\n", DEVICE_NAME,
                           dtsInfo->devId, (uint64_t)dtsInfo->pMemMapBaseAddr, dtsInfo->memSize);
#endif
        }
    }

    return err;
}

/******************************************************************************/
/**
 * @brief   Device probe
 */
static int RsdkCsi2Probe(struct platform_device *pPlatDev)
{
    static char         deviceName[RSDK_MIPICSI2_INTERFACES][20];
    static const struct file_operations gRsdkCsi2FileOps = {
        .owner = THIS_MODULE,
        .open = RsdkCsi2Open,
        .release = RsdkCsi2Release,
    };
    
    int32_t             err = 0;
    struct device      *pDevice = &pPlatDev->dev;
    struct device_node *pNode = pPlatDev->dev.of_node;
    struct device      *pSysFsDev;
    rsdkCsi2Device_t   *pRsdkCsi2Dev;
    dev_t               devNo;

    //volatile uint32_t __iomem *pRsdkCsi2Regs; // Virtual address
    uint32_t val;

#ifdef DEBUG_MODE
    (void)pr_alert("RsdkCsi2Probe: %s, %s / %d\n", DEVICE_NAME, pNode->full_name, pNode->properties->unique_id);
#endif
    BUG_ON((gsNumRsdkCsi2Major == 0) || (gspRsdkCsi2Class == NULL));

    // initialize the necessary clocks
    /*  To be done when the clocks support will be implemented
    struct clk          *pClk;
    static const char   *csi2ClkNames[5] = { "csi2_dphy_clk", "csi2_pll_clk", "csi2_module_clk", "csi2_ctrl_clk", NULL};
    int32_t             i;
    i = 0;
    while((csi2ClkNames[i] != NULL) && (err == 0))
    {
        (void)pr_err("RsdkCsi2Probe: clock find %s.\n", csi2ClkNames[i]);
        pClk = devm_clk_get(pDevice, csi2ClkNames[i]);
        (void)pr_err("RsdkCsi2Probe: clock find rez %x.\n", pClk);
        if(IS_ERR(pClk))
        {   /-> clock not found
            (void)pr_err("RsdkCsi2Probe: clock find error for %s.\n", csi2ClkNames[i]);
            err = -EFAULT;
        }
        else
        {   /-> clock found
            if(clk_prepare_enable(pClk) != 0)
            {
                (void)pr_err("RsdkCsi2Probe: clock start error for %s.\n", csi2ClkNames[i]);
                err = -EFAULT;
            }
        }
        i++;
    }
    /-> Allocate CSI2 device structure 
    if(err == 0)
    {
        */
        pRsdkCsi2Dev = kzalloc(sizeof(rsdkCsi2Device_t), GFP_KERNEL);
        err = RsdkCsi2GetDtsProperties(pNode, &pRsdkCsi2Dev->dtsInfo);
        if (err < 0)
        {
            kvfree(pRsdkCsi2Dev);
            (void)pr_err("RsdkCsi2Probe: MIPICSI2 DTS entry parse failed.\n");
            err = -EINVAL;
        }
    /*  necessary to be commented, to keep the correct match for brackets, as err is not changed at clocks setup
    }
    */
    if(err == 0)
    {
#ifdef DEBUG_MODE
        (void)pr_alert("RsdkCsi2Probe: GetDtsProperties() OK -> id=%d\n", pRsdkCsi2Dev->dtsInfo.devId);
#endif
        gpRsdkCsi2Device[pRsdkCsi2Dev->dtsInfo.devId] = pRsdkCsi2Dev;  // keep the device pointer for future use
        pRsdkCsi2Dev->gUserPid = 0xffffffffu;                          // wrong pid
        devNo = MKDEV(gsNumRsdkCsi2Major, gsNumRsdkCsi2Minor + pRsdkCsi2Dev->dtsInfo.devId);
        (void)sprintf(deviceName[pRsdkCsi2Dev->dtsInfo.devId],"%s_%d", DEVICE_NAME, pRsdkCsi2Dev->dtsInfo.devId);
        dev_set_drvdata(pDevice, pRsdkCsi2Dev);

        //initialize char device
        cdev_init(&pRsdkCsi2Dev->cdevRef, &gRsdkCsi2FileOps);

        err = cdev_add(&(pRsdkCsi2Dev->cdevRef), devNo, 1);
        if (err < 0)
        {
            (void)pr_err("RsdkCsi2Probe: Error %d in cdev_add() while adding %s", err, 
                                                        deviceName[pRsdkCsi2Dev->dtsInfo.devId]);
        }
        else
        {
            // Create sysfs device
#ifdef DEBUG_MODE
            (void)pr_alert("RsdkCsi2Probe: cdev_add() OK.\n");
#endif
            pSysFsDev = device_create(gspRsdkCsi2Class, pDevice, devNo, NULL, deviceName[pRsdkCsi2Dev->dtsInfo.devId]);
            if (IS_ERR(pSysFsDev))
            {
                err = (int32_t)PTR_ERR(pSysFsDev);
                (void)pr_err("RsdkCsi2Probe: Error %d in device_create() for %s", err, 
                                                        deviceName[pRsdkCsi2Dev->dtsInfo.devId]);

                //revert previous actions:
                cdev_del(&pRsdkCsi2Dev->cdevRef);
            }
            else
            {
#ifdef DEBUG_MODE
                (void)pr_alert("RsdkCsi2Probe: device_create() OK.\n");
#endif
            }
        }
        if (err < 0)
        {
            kvfree(pRsdkCsi2Dev);
        }
    } // if (err < 0)

    //finished creating the device, now bind the specific hardware resources to it - register map, interrupts:
    if (err == 0)
    {
        pRsdkCsi2Dev->pMemMapVirtAddr =
            (uint32_t *)ioremap((phys_addr_t)pRsdkCsi2Dev->dtsInfo.pMemMapBaseAddr, pRsdkCsi2Dev->dtsInfo.memSize);
        if (pRsdkCsi2Dev->pMemMapVirtAddr == NULL)
        {
            err = -EFAULT;
            (void)pr_err("RsdkCsi2Probe: ioremap failed for CSI2 Registry Address!.\n");
        }
        else
        {
            //test the register access:
            pRsdkCsi2Dev->pMemMapVirtAddr[3] = 0xffffffffu;
            val = pRsdkCsi2Dev->pMemMapVirtAddr[3];
            if (val != 0x7fffu)
            {
                (void)pr_err("RsdkCsi2Probe: ioremap error - phys=%lx, virt=%lx, test val=%x.\n",
                         (uint64_t)pRsdkCsi2Dev->dtsInfo.pMemMapBaseAddr, (uint64_t)pRsdkCsi2Dev->pMemMapVirtAddr, val);
                err = -EFAULT;
                iounmap(pRsdkCsi2Dev->pMemMapVirtAddr);
            }
            else
            {
#ifdef DEBUG_MODE
                (void)pr_alert("RsdkCsi2Probe: ioremap -> phys=%lx, virt=%llx.\n",
                               (uint32_t)pRsdkCsi2Dev->dtsInfo.pMemMapBaseAddr,
                               (uint64_t)pRsdkCsi2Dev->pMemMapVirtAddr);
#endif
            }
        }
        if (err == 0)
        {
#if (CSI2_DFS_USAGE != CSI2_DFS_NOT_USED)
            if (pRsdkCsi2Dev->dtsInfo.devId == 0)
            {  // map the memory only for the first unit
#ifdef DEBUG_MODE
                (void)pr_alert("RsdkCsi2Probe: ioremap for DFS -> phys=%lx, virt=%lx.\n",
                               (uint64_t)DFS_PHY_ADDRESS, (uint64_t)DFS_MEM_LEN);
#endif
                pRsdkCsi2Dev->pMemMapVirtDfs = (uint32_t *)ioremap((phys_addr_t)DFS_PHY_ADDRESS, DFS_MEM_LEN);
            }
            else
            {  // for all other units only copy the address (to have it)
                pRsdkCsi2Dev->pMemMapVirtDfs = gpRsdkCsi2Device[0]->pMemMapVirtDfs;
            }
            if (pRsdkCsi2Dev->pMemMapVirtDfs == NULL)
            {
                err = -EFAULT;
                (void)pr_err("RsdkCsi2Probe: ioremap failed for DFS Registry !.\n");
                iounmap(pRsdkCsi2Dev->pMemMapVirtAddr);
            }
#endif
        }
        if (err != 0)
        {
            //revert previous actions:
            device_destroy(gspRsdkCsi2Class, devNo);
            cdev_del(&pRsdkCsi2Dev->cdevRef);
            kvfree(pRsdkCsi2Dev);
        }
    } // if (err == 0)

    if (err == 0)
    {
        val = 0u;  // installed irq handlers mask
                   //register the interrupt handlers
        err = request_irq(pRsdkCsi2Dev->dtsInfo.irqId[0], RsdkCsi2RxIrqHandlerLinux, 0, 
                                                                deviceName[pRsdkCsi2Dev->dtsInfo.devId], pRsdkCsi2Dev);
        if (err == 0)
        {
            val |= (uint32_t)CSI2_LX_IRQ0_MASK;
            err = request_irq(pRsdkCsi2Dev->dtsInfo.irqId[1], RsdkCsi2PhyIrqHandlerLinux, 0, 
                                                                deviceName[pRsdkCsi2Dev->dtsInfo.devId], pRsdkCsi2Dev);
        }
        if (err == 0)
        {
            val |= (uint32_t)CSI2_LX_IRQ1_MASK;
            err =
                request_irq(pRsdkCsi2Dev->dtsInfo.irqId[2], RsdkCsi2EventsIrqHandlerLinux, 0, 
                                                                deviceName[pRsdkCsi2Dev->dtsInfo.devId], pRsdkCsi2Dev);
        }
        // request_irq returns negative values in case of errors, so I'm reusing err, 
        // because there's no need to know which of these calls fails
        if (err < 0)
        {
            (void)pr_err("RsdkCsi2Probe: %s - at least a call to request_irq() has failed.\n", 
                                                                deviceName[pRsdkCsi2Dev->dtsInfo.devId]);
            //revert previous actions:
            if ((val & ((uint32_t)CSI2_LX_IRQ0_MASK)) != 0u)
            {
                (void)free_irq(pRsdkCsi2Dev->dtsInfo.irqId[0], pRsdkCsi2Dev);
            }
            if ((val & ((uint32_t)CSI2_LX_IRQ1_MASK)) != 0u)
            {
                (void)free_irq(pRsdkCsi2Dev->dtsInfo.irqId[1], pRsdkCsi2Dev);
            }
            err = -EINTR;
        }
        else
        {
#ifdef DEBUG_MODE
            (void)pr_alert("RsdkCsi2Probe: Registered all IRQ handlers.\n");
#endif
            //initialize the wait queues for sending interrupt events into user space
            err = OAL_InitWaitQueue(&(pRsdkCsi2Dev->irqWaitQ));
            if (err < 0)
            {
                (void)pr_err("RsdkCsi2Probe: OAL_InitWaitQueues failed!.\n");
                (void)free_irq(pRsdkCsi2Dev->dtsInfo.irqId[0], pRsdkCsi2Dev);
                (void)free_irq(pRsdkCsi2Dev->dtsInfo.irqId[1], pRsdkCsi2Dev);
                (void)free_irq(pRsdkCsi2Dev->dtsInfo.irqId[2], pRsdkCsi2Dev);
            }
            else
            {
#ifdef DEBUG_MODE
                (void)pr_alert("RsdkCsi2Probe: OAL_InitWaitQueue ok.\n");
#endif
            }
        }
        if (err != 0)
        {
            iounmap(pRsdkCsi2Dev->pMemMapVirtAddr);
            device_destroy(gspRsdkCsi2Class, devNo);
            cdev_del(&pRsdkCsi2Dev->cdevRef);
            kvfree(pRsdkCsi2Dev);
        }
    } // if (err == 0)

    if (err == 0)
    {
       // init the RPC server, but only for first unit (only one RPC for all units)
        err = RsdkCsi2RpcSrvInit((uint32_t)pRsdkCsi2Dev->dtsInfo.devId);
        if (err != 0)
        {
            (void)pr_err("RsdkCsi2Probe: RsdkCsi2RpcSrvInit() failed, err = %d\n", err);
        }
        else
        {
#ifdef DEBUG_MODE
            (void)pr_alert("RsdkCsi2Probe: RsdkCsi2RpcSrvInit() ok.\n");
#endif
        }
    }

    return err;
}

/******************************************************************************/
/**
 * @brief   Device remove
 */
static int RsdkCsi2Remove(struct platform_device *pOfpDev)
{

    rsdkCsi2Device_t *pRsdkCsi2Dev = dev_get_drvdata(&pOfpDev->dev);

    (void)pr_alert("RsdkCsi2Remove: START \n");

    dev_set_drvdata(&pOfpDev->dev, NULL);

    (void)RsdkCsi2RpcSrvExit((uint32_t)pRsdkCsi2Dev->dtsInfo.devId);
    (void)OAL_DestroyWaitQueue(&(pRsdkCsi2Dev->irqWaitQ));
    (void)free_irq(pRsdkCsi2Dev->dtsInfo.irqId[0], pRsdkCsi2Dev);
    (void)free_irq(pRsdkCsi2Dev->dtsInfo.irqId[1], pRsdkCsi2Dev);
    (void)free_irq(pRsdkCsi2Dev->dtsInfo.irqId[2], pRsdkCsi2Dev);
    iounmap(pRsdkCsi2Dev->pMemMapVirtAddr);
    device_destroy(gspRsdkCsi2Class, MKDEV(gsNumRsdkCsi2Major, gsNumRsdkCsi2Minor + pRsdkCsi2Dev->dtsInfo.devId));
    cdev_del(&pRsdkCsi2Dev->cdevRef);
    kvfree(pRsdkCsi2Dev);

    return 0;
}

static struct platform_driver gsRsdkCsi2Driver = {
    .driver =
        {
            .name = "fsl-mipicsi2",
            .owner = THIS_MODULE,
            .of_match_table = gsRsdkCsi2Match,
        },
    .probe = RsdkCsi2Probe,
    .remove = RsdkCsi2Remove,
};

/******************************************************************************/
/**
 * @brief   Module init
 */
static int __init RsdkCsi2ModInit(void)
{
    int32_t err, i;

    /* Register our major, and accept a dynamic number. */
    err = alloc_chrdev_region(&gsDevNum, 0, 1, DEVICE_NAME);
    if (err < 0)
    {
        (void)pr_err("RsdkCsi2ModInit: can't get major number = %d\n", err);
    }
    else  //alloc_chrdev_region() OK
    {
        gsNumRsdkCsi2Major = (s32)MAJOR(gsDevNum);
        gsNumRsdkCsi2Minor = (s32)MINOR(gsDevNum);

        /* Create the device class if required */
        gspRsdkCsi2Class = class_create(THIS_MODULE, DEVICE_NAME);
        if (IS_ERR(gspRsdkCsi2Class))
        {
            unregister_chrdev_region((uint32_t)gsNumRsdkCsi2Major, 0);
            err = (int32_t)PTR_ERR(gspRsdkCsi2Class);
            (void)pr_err("RsdkCsi2ModInit: class_create() failed = %d\n", err);
        }
        else  //class_create() OK
        {
#ifdef DEBUG_MODE
            (void)pr_alert("RsdkCsi2ModInit: class_create() OK.\n");
#endif
            err = platform_driver_register(&gsRsdkCsi2Driver);
            if (err != 0)
            {
                (void)pr_err("RsdkCsi2ModInit: platform_driver_register() failed = %d\n", err);
                class_destroy(gspRsdkCsi2Class);
                unregister_chrdev_region(gsNumRsdkCsi2Major, 1);
            }
        }
    }
    if(err == 0)
    {                   // platform_driver_register() OK, so only count the available units
        i = 0;
        while (((uint32_t)i < RSDK_MIPICSI2_INTERFACES) && (gpRsdkCsi2Device[i] != NULL))
        {
            if (gpRsdkCsi2Device[i]->pMemMapVirtDfs != NULL)
            {
                i++;
            }
            else
            {
                break;
            }
        }
        (void)pr_alert("RsdkCsi2ModInit: CSI2 driver module init OK. Available units 0...%d.\n", i - 1);
    }

    return err;
}

/******************************************************************************/
/**
 * @brief   Module exit
 */
static void __exit RsdkCsi2ModExit(void)
{
    platform_driver_unregister(&gsRsdkCsi2Driver);
    class_destroy(gspRsdkCsi2Class);
    unregister_chrdev_region(gsDevNum, 1);
    (void)pr_alert("RsdkCsi2ModExit: CSI2 driver exit done. \n");
}

module_init(RsdkCsi2ModInit);
module_exit(RsdkCsi2ModExit);

#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
