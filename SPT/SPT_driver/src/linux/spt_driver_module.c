/*
 * Copyright 2016-2020 NXP
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
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/clk.h>

#include "rsdk_S32R45.h"
#include "spt_oal.h"
#include "spt_driver_module.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("NXP Semiconductors");
MODULE_DESCRIPTION("NXP SPT Driver");
MODULE_VERSION("2.00");

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define DEVICE_NAME "spt_driver" /* Dev name as it appears in /proc/devices   */

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
static s32                       gNumSptMajor;
static s32                       gNumSptMinor;
static dev_t                     devNum = MKDEV(0, 0);
static struct class *            gspSptClass;
static const struct of_device_id gsSptMatch[] = {
    {
        .compatible = "nxp,spt",
    },
    {},
};

sptDevice_t sptDevice;
/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/

/******************************************************************************/
/**
 * @brief   Open device
 */
static int SptOpen(struct inode *pInode, struct file *fp)
{
    sptDevice_t *pSptDevice = container_of(pInode->i_cdev, sptDevice_t, cdevice);
    int          err = 0;

    if (pSptDevice != NULL)
    {
        fp->private_data = pSptDevice;
        pSptDevice->gUserPid = current->pid;
        PR_ALERT("spt_driver module: SptOpen: OK, PID = %d\n", pSptDevice->gUserPid);
    }
    else
    {
        PR_ERR("SptOpen: No device context found for %s %d\n", DEVICE_NAME, iminor(pInode));
        err = -ENODEV;
    }

    return err;
}

/******************************************************************************/
/**
 * @brief   release/close device
 */
static int SptRelease(struct inode *pInode, struct file *fp)
{
    sptDevice_t *pSptDevice = (sptDevice_t *)fp->private_data;
    int          err = 0;

    if (pSptDevice == NULL)
    {
        PR_ERR("SptRelease: No device context found for %s %d\n", DEVICE_NAME, iminor(pInode));
        err = -ENODEV;
    }
    else
    {
        PR_ALERT("spt_driver module: SptRelease: OK.\n");
    }
    return err;
}

static const struct file_operations gSptFileOps = {
    .owner = THIS_MODULE,
    .open = SptOpen,
    .release = SptRelease,
};

/**
 * @brief   Get dts properties
 */
static int32_t SptGetDtsProperties(struct device_node *pNode, sptDtsInfo_t *dtsInfo)
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
        if (of_property_read_u32(pNode, "sptdev-id", &prop) < 0)
        {
            PR_ALERT("spt_driver module: sptdev-id attribute not found\n");
            err = -EINVAL;
        }
    }
    if (err == 0)
    {
        dtsInfo->devId = prop;

        if (of_get_property(pNode, "interrupts", NULL) != NULL)
        {
            dtsInfo->irqId[SPT_DTS_IRQ_IDX_DSP] = irq_of_parse_and_map(pNode, 0);  //DSP error interrupt
            dtsInfo->irqId[SPT_DTS_IRQ_IDX_EVT] = irq_of_parse_and_map(pNode, 1);  //SPT Event IRQ
            dtsInfo->irqId[SPT_DTS_IRQ_IDX_ECS] = irq_of_parse_and_map(pNode, 2);  //ECS IRQ
            dtsInfo->irqId[SPT_DTS_IRQ_IDX_DMA] = irq_of_parse_and_map(pNode, 3);  //Interrupt on DMA completion

            if (dtsInfo->irqId[SPT_DTS_IRQ_IDX_DSP] == 0u)
            {
                PR_ERR("spt_driver module: DSP interrupt not found for %s%d\n", DEVICE_NAME, dtsInfo->devId);
                err = -EINVAL;
            }
            else
            {
                PR_ALERT("spt_driver module: DSP interrupt number for %s%d = %d \n", DEVICE_NAME, dtsInfo->devId,
                         dtsInfo->irqId[SPT_DTS_IRQ_IDX_DSP]);
            }

            if (dtsInfo->irqId[SPT_DTS_IRQ_IDX_EVT] == 0u)
            {
                PR_ERR("spt_driver module: EVT interrupt not found for %s%d\n", DEVICE_NAME, dtsInfo->devId);
                err = -EINVAL;
            }
            else
            {
                PR_ALERT("spt_driver module: EVT interrupt number for %s%d = %d \n", DEVICE_NAME, dtsInfo->devId,
                         dtsInfo->irqId[SPT_DTS_IRQ_IDX_EVT]);
            }

            if (dtsInfo->irqId[SPT_DTS_IRQ_IDX_ECS] == 0u)
            {
                PR_ERR("spt_driver module: ECS interrupt not found for %s%d\n", DEVICE_NAME, dtsInfo->devId);
                err = -EINVAL;
            }
            else
            {
                PR_ALERT("spt_driver module: ECS interrupt number for %s%d = %d \n", DEVICE_NAME, dtsInfo->devId,
                         dtsInfo->irqId[SPT_DTS_IRQ_IDX_ECS]);
            }
        }
        else
        {
            PR_ALERT("spt_driver module: Interrupt numbers not found for %s%d\n", DEVICE_NAME, dtsInfo->devId);
            err = -EINVAL;
        }
    }

    if (err == 0)
    {
        if (of_address_to_resource(pNode, 0, &res) != 0)
        {
            PR_ALERT("spt_driver module: Reg map not found for %s%d\n", DEVICE_NAME, dtsInfo->devId);
            err = -EINVAL;
        }
        else
        {
            dtsInfo->pMemMapBaseAddr = (u32 __iomem *)res.start;
            dtsInfo->memSize = resource_size(&res);
            PR_ALERT("spt_driver module: Phy reg map for %s%d: base addr=0x%x, size=%d\n", DEVICE_NAME, dtsInfo->devId,
                     dtsInfo->pMemMapBaseAddr, dtsInfo->memSize);
        }
    }

    return err;
}

/**
 * @brief   Device probe
 */
static int SptProbe(struct platform_device *pPlatDev)
{
    int                 err = 0, i;
    struct device *     pDevice = &pPlatDev->dev;
    struct device_node *pNode = pPlatDev->dev.of_node;
    struct device *     pSysFsDev;
    struct clk         *pClk;

    static const char   *sptClkNames[3] = { "spt_clk", "spt_module_clk", NULL};

    uint32_t regVal = 1;

    PR_ALERT("spt_driver module: SptProbe \n");
    BUG_ON((gNumSptMajor == 0) || (gspSptClass == NULL));

    // initialize the clocks
    i = 0;
    while((sptClkNames[i] != NULL) && (err == 0))
    {
        (void)pr_err("RsdkSptProbe: clock find %s.\n", sptClkNames[i]);
        pClk = devm_clk_get(pDevice, sptClkNames[i]);
        (void)pr_err("RsdkSptProbe: clock find result = %x.\n", pClk);
        if(IS_ERR(pClk))
        {   // clock not found
            (void)pr_err("RsdkSptProbe: clock find error for %s.\n", sptClkNames[i]);
            err = -EFAULT;
        }
        else
        {   // clock found
            if(clk_prepare_enable(pClk) != 0)
            {
                (void)pr_err("RsdkSptProbe: clock start error for %s.\n", sptClkNames[i]);
                err = -EFAULT;
            }
        }
        i++;
    }
    if (SptGetDtsProperties(pNode, &sptDevice.dtsInfo) != 0)
    {
        PR_ALERT("spt_driver module: SPT DTS entry parse failed.\n");
        err = -EINVAL;
    }
    if(err == 0)
    {
        PR_ALERT("spt_driver module: SptProbe: SptGetDtsProperties() OK.\n");

        dev_set_drvdata(pDevice, &sptDevice);

        //initialize char device
        cdev_init(&sptDevice.cdevice, &gSptFileOps);

        err = cdev_add(&sptDevice.cdevice, devNum, 1);
        if (err < 0)
        {
            PR_ALERT("spt_driver module: Error %d in cdev_add() while adding %s", err, DEVICE_NAME);
        }
        else
        {
            // Create sysfs device
            PR_ALERT("spt_driver module: SptProbe: cdev_add() OK.\n");
            pSysFsDev = device_create(gspSptClass, pDevice, devNum, NULL, DEVICE_NAME);
            if (IS_ERR(pSysFsDev))
            {
                err = PTR_ERR(pSysFsDev);
                PR_ALERT("spt_driver module: Error %d in device_create() for %s", err, DEVICE_NAME);

                //revert previous actions:
                cdev_del(&sptDevice.cdevice);
            }
            else
            {
                PR_ALERT("spt_driver module: SptProbe: device_create() OK.\n");
            }
        }
    }

    //finished creating the device, now bind the specific hardware resources to it - register map, interrupts:
    if (err == 0)
    {
        sptDevice.pSptRegs = (volatile struct SPT_tag *)ioremap((phys_addr_t)sptDevice.dtsInfo.pMemMapBaseAddr, (size_t)sptDevice.dtsInfo.memSize);
        if (sptDevice.pSptRegs == NULL)
        {
            err = EFAULT;
            PR_ALERT("spt_driver module: SptProbe: ioremap failed!.\n");

            //revert previous actions:
            device_destroy(gspSptClass, devNum);
            cdev_del(&sptDevice.cdevice);
        }
        else
        {
            //test the register access by doing a read-after-write:
            sptDevice.pSptRegs->CS_PG_ST_ADDR.R = SPT_REG_TESTVAL;
            regVal = sptDevice.pSptRegs->CS_PG_ST_ADDR.R;
			if(regVal != SPT_REG_TESTVAL)
			{
				err = EFAULT;
				PR_ALERT("spt_driver module: SptProbe: SPT register access failed!.\n");

				//revert previous actions:
				device_destroy(gspSptClass, devNum);
				cdev_del(&sptDevice.cdevice);
			}

            PR_ALERT("spt_driver module: SptProbe: ioremap output pSptRegs=%x, test value=%x.\n", sptDevice.pSptRegs, regVal);
        }
    }

    if (err == 0)
    {
        //register the same interrupt handler for all interrupt lines. The handler's first argument tells the type of interrupt.
        err = request_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_DSP], SptDevIrqHandler, 0, DEVICE_NAME, &sptDevice);
        
		if (err == 0)
        {
			err = request_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_EVT], SptDevIrqHandler, 0, DEVICE_NAME, &sptDevice);
		}
		else
		{
			(void*)free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_DSP], &sptDevice);
		}
        
		if (err == 0)
        {
			err = request_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_ECS], SptDevIrqHandler, 0, DEVICE_NAME, &sptDevice);
		}
		else
		{
			(void*)free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_DSP], &sptDevice);
			(void*)free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_EVT], &sptDevice);
		}
		
        if (err < 0)
        {
            PR_ALERT("spt_driver module: %s: a call to request_irq() has failed.\n", DEVICE_NAME);

            //revert previous actions:
            iounmap(sptDevice.pSptRegs);
            device_destroy(gspSptClass, devNum);
            cdev_del(&sptDevice.cdevice);
        }
        else
        {
            PR_ERR("spt_driver module: Registered SptDevIrqHandler.\n");

            //initialize a single wait queue for sending interrupt events into user space
            err = OAL_InitWaitQueue(&(sptDevice.irqWaitQ));
            if (err < 0)
            {
                PR_ERR("spt_driver module: SptProbe: OAL_InitWaitQueue failed!.\n");
				
				//revert previous actions:
				(void*)free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_DSP], &sptDevice);
				(void*)free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_EVT], &sptDevice);
				(void*)free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_ECS], &sptDevice);
				iounmap(sptDevice.pSptRegs);
				device_destroy(gspSptClass, devNum);
				cdev_del(&sptDevice.cdevice);

            }
            else
            {
                PR_ALERT("spt_driver module: SptProbe: OAL_InitWaitQueue ok.\n");
            }

            /* Initialize the wait queue condition */
            atomic_set(&(sptDevice.irqsNotServed), 0);

            /* Initialize the evtData queue */
            (void)memset(sptDevice.queueEvtData, 0, sizeof(sptDevice.queueEvtData));
            sptDevice.queueIdxRd = 0;
            atomic_set(&(sptDevice.queueIdxWr), 0);
        }
    }

    if (err == 0)
    {
        err = SptOalCommInit();
        if (err != 0)
        {
            PR_ERR("spt_driver module: SptOalCommInit() failed, err = %d\n", err);

			//revert previous actions:
			(void)OAL_DestroyWaitQueue(&(sptDevice.irqWaitQ));
			(void*)free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_DSP], &sptDevice);
			(void*)free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_EVT], &sptDevice);
			(void*)free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_ECS], &sptDevice);
			iounmap(sptDevice.pSptRegs);
			device_destroy(gspSptClass, devNum);
			cdev_del(&sptDevice.cdevice);
        }
        else
        {
            PR_ALERT("spt_driver module: SptOalCommInit() ok.\n");
        }
    }

    return err;
}

/**
 * @brief   Device remove
 */
static int SptRemove(struct platform_device *ofpdev)
{
    //struct device *pDev = &ofpdev->dev;
    int32_t retVal;
	const uint8_t* retPtr = NULL;

    sptDevice_t *pSptDev = dev_get_drvdata(&ofpdev->dev);

    dev_set_drvdata(&ofpdev->dev, NULL);

    retVal = SptOalCommExit();
    if (retVal != 0)
    {
        PR_ALERT("spt_driver module: SptRemove: OAL_RPCCleanup failed! \n");
    }
    else
    {
        retVal = OAL_DestroyWaitQueue(&(sptDevice.irqWaitQ));

        if (retVal != 0)
        {
            PR_ERR("spt_driver module: SptRemove: OAL_DestroyWaitQueue failed! \n");
        }
        else
        {
            retPtr = free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_DSP], &sptDevice);
			if (retPtr != NULL)
			{
				retPtr = free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_EVT], &sptDevice);
			}
			if (retPtr != NULL)
			{
				retPtr = free_irq(sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_ECS], &sptDevice);
			}

			if (retPtr != NULL)
			{
				iounmap(sptDevice.pSptRegs);
				device_destroy(gspSptClass, devNum);
				cdev_del(&pSptDev->cdevice);
			}
			else
			{
				PR_ERR("SptRemove: cannot free_irq!\n");
				retVal = ENODEV;
			}
        }
    }
    PR_ALERT("spt_driver module: SptRemove ok.\n");

    return (int)retVal;
}

static struct platform_driver gsSptDriver = {
    .driver =
        {
            .name = "nxp-spt",
            .owner = THIS_MODULE,
            .of_match_table = gsSptMatch,
        },
    .probe = SptProbe,
    .remove = SptRemove,
};

/**
 * @brief   Module init
 */
static int32_t __init SptModInit(void)
{
    int32_t err;

    /* Register our major, and accept a dynamic number. */
    err = alloc_chrdev_region(&devNum, 0, 1, DEVICE_NAME);
    if (err != 0)
    {
        PR_ERR("SptModInit: can't get major number: %d\n", err);
    }
    else  //alloc_chrdev_region() OK
    {
        gNumSptMajor = MAJOR(devNum);
        gNumSptMinor = MINOR(devNum);

        /* Create the device class if required */
        gspSptClass = class_create(THIS_MODULE, DEVICE_NAME);
        if (IS_ERR(gspSptClass))
        {
            unregister_chrdev_region(gNumSptMajor, 0);
            err = PTR_ERR(gspSptClass);
            PR_ERR("SptModInit: class_create() failed %d\n", err);
        }
        else  //class_create() OK
        {
            PR_ALERT("spt_driver module: SptModInit: class_create() OK.\n");

            err = platform_driver_register(&gsSptDriver);
            if (err != 0)
            {
                PR_ERR("SptModInit: platform_driver_register() failed! %d\n", err);
                class_destroy(gspSptClass);
                unregister_chrdev_region(gNumSptMajor, 1);
            }
            else  // platform_driver_register() OK
            {
                PR_ALERT("spt_driver module: SPT driver module init OK. Major number %d.\n", gNumSptMajor);
            }
        }
    }

    return err;
}

/**
 * @brief   Module exit
 */
static void __exit SptModExit(void)
{
    platform_driver_unregister(&gsSptDriver);
    class_destroy(gspSptClass);
    unregister_chrdev_region(devNum, 1);
    PR_ALERT("spt_driver module: SPT driver exit. \n");
}

module_init(SptModInit);
module_exit(SptModExit);

#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
