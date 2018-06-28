/*
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "vspa.h"
#include "lax_driver.h"
#include "lax_atu.h"

#include <linux/types.h>
#include <vspa_uapi.h>

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
/* Additional options for checking if Mailbox write to VSP completed */
#ifdef VSPA_DEBUG
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
static int gsCmdBufferBytes = AVI_CMD_BUF_SZ_WORDS_32 << 2;
module_param(gsCmdBufferBytes, int, 0644);
MODULE_PARM_DESC(gsCmdBufferBytes, "Size of cmdbuf(bytes), default: AVI_CMD_BUF_SZ_WORDS_32 as defined in lax_common.h");

static int gsSpmBufferBytes = 4096;
module_param(gsSpmBufferBytes, int, 0644);
MODULE_PARM_DESC(gsSpmBufferBytes, "Size of spm buf(bytes), default: 4096");

/* Number of VSPA devices probed on system */
static int gsNumVspaDevs;
static s32 gsNumVspaMajor;
static s32 gsNumVspaMinor;
static struct class *gspVspaClass;

extern const struct attribute_group gAttrGroup;

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

extern const struct attribute_group attrGroup;

/* Helper function to map the addr to AXI addr - needs access to vspaAtu */
uint32_t VspaGetAXIAddr(vspaControl_t *pVspaCtrl, dma_addr_t dmaAxiAddr) 
{
    vspaDevice_t *pVspaDev = container_of(pVspaCtrl, vspaDevice_t, vspaCtrl);
    return VspaMapAxiAddrAtuWin( &pVspaDev->atuCtrl, dmaAxiAddr);
}

/******************************************************************************/
/**
 * @brief   Send a signal to user space
 */
void Sig2UsrSend(vspaControl_t *pVspaCtrl, int32_t aValue)
{
    struct siginfo lInfo;
    struct pid *lpPidStruct;
    struct task_struct *lpTask;
    vspaDevice_t *pVspaDev =  container_of(pVspaCtrl, vspaDevice_t, vspaCtrl);

    // send the signal to user space
    memset(&lInfo, 0, sizeof(struct siginfo));
    lInfo.si_signo = RSDK_LAX_EVENT_SIGNAL;
    lInfo.si_code = SI_QUEUE;
    lInfo.si_int = (aValue);
    rcu_read_lock();
    lpPidStruct = find_get_pid(pVspaDev->gUserPid);
    lpTask = pid_task(lpPidStruct, PIDTYPE_PID);
    if (lpTask == NULL) 
    {
        pr_err("No such pid %u. Signal not sent.\n", (unsigned)pVspaDev->gUserPid);
        rcu_read_unlock();
    }           // if no such pidRegisterred
    else
    {
        rcu_read_unlock();

        //send the signal
        if ((send_sig_info(RSDK_LAX_EVENT_SIGNAL, &lInfo, lpTask)) < 0) 
        {
            pr_err("Error sending signal to user space.\n");
        }       // if sending signal failed
    }           // else if no such pid
}// Sig2UsrSend()


/******************************************************************************/
/**
 * @brief   Open vspa device
 */
static int VspaOpen(struct inode *inode, struct file *fp)
{
    vspaDevice_t *pVspaDev = NULL;
    vspaControl_t *pVspaCtrl;
    int rc = 0;
    pVspaDev = container_of(inode->i_cdev, vspaDevice_t, cdev);
    pVspaCtrl = &pVspaDev->vspaCtrl;
    gOalCommVspaCtrl = pVspaCtrl;

    if (pVspaDev != NULL) 
    {
        fp->private_data = pVspaDev;
        pVspaDev->gUserPid = current->pid;
    } 
    else
    {
        dev_err(pVspaDev->dev, "No device context found for %s %d\n", VSPA_DEVICE_NAME, iminor(inode));

        rc = -ENODEV;
    }

    return rc;
}// VspaOpen()


/******************************************************************************/
/**
 * @brief   release/close vspa device
 */
static int VspaRelease(struct inode *inode, struct file *fp)
{
    vspaDevice_t *pVspaDev = (vspaDevice_t *)fp->private_data;
    int rc = 0;

    if (pVspaDev != NULL) 
    {
    } 
    else 
    {
        pr_err("No device context found for %s %d\n", VSPA_DEVICE_NAME, iminor(inode));
        rc = -ENODEV;
    }

    return rc;
}// VspaRelease()


static const struct file_operations gsVspaFops = 
{
    .owner          = THIS_MODULE,
    .open           = VspaOpen,
    .release        = VspaRelease,
};

/************************* Probe / Remove **************************/

/******************************************************************************/
/**
 * @brief   Parse dts
 */
static int32_t ParseAtuDts(struct device_node *pNode, vspaDevice_t *pVspaDev)
{
    u32 numAtuWindws, winCnt;
    char atuWinStr[MAX_STRING_ATU_WIN];
    u64 atuWinProprts[3];
    struct device_node *pAtuNp;
    struct resource atuRes;
    atuControl_t * pAtuCtrl = & pVspaDev->atuCtrl;
    vspaControl_t *pVspaCtrl = &pVspaDev->vspaCtrl;
    int32_t err = 0;

    pAtuNp = of_parse_phandle(pNode, "atu", 0);
    if (NULL != pAtuNp) 
    {
        /* Find ATU Windows to be configured */
        if (of_property_read_u32(pAtuNp, "atu_win_cnt", &numAtuWindws) < 0) 
        {
            dev_err(pVspaDev->dev, "ATU windows count not found %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
            err = -EINVAL;
        } 
        else 
        {
            pAtuCtrl->numAtuWin = numAtuWindws;
            for (winCnt = 0; winCnt < pAtuCtrl->numAtuWin; winCnt++) 
            {
                snprintf(atuWinStr, MAX_STRING_ATU_WIN, "atu_win_%d", (winCnt + 1));
                if (of_property_read_u64_array(pAtuNp, atuWinStr, atuWinProprts, 3) < 0) 
                {
                    dev_err(pVspaDev->dev, "ATU WIN[%d] properties invalid %s%d\n",
                            winCnt, VSPA_DEVICE_NAME, pVspaCtrl->id);
                    err = -EINVAL;
                } 
                else 
                {
                    /* Update "pVspaDev" structure with ATU window
                     * information*/
                    pAtuCtrl->vspaAtuWin[winCnt].windwBaseAdd = atuWinProprts[0];
                    pAtuCtrl->vspaAtuWin[winCnt].windwSz = atuWinProprts[1] + VSPA_ATU_MIN_WIN_SZ;
                    pAtuCtrl->vspaAtuWin[winCnt].windwMappedAdd = atuWinProprts[2];
                }
            } /* end for */
        }
    } 
    else 
    { /* pAtuNp == NULL */
        dev_err(pVspaDev->dev, "ATU Node not found for %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
        err = -EINVAL;
    }

    if(err == 0)
    {
        if (of_address_to_resource(pAtuNp, 0, &atuRes)) 
        {
            dev_err(pVspaDev->dev, "ATU reg map not found for %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
            err = -EINVAL;
        } 
        else 
        {
            pAtuCtrl->pVspaAtuAddr = (u32 __iomem *)atuRes.start;
            pAtuCtrl->vspaAtuSize =  resource_size(&atuRes);
            for (winCnt = 0; winCnt < pAtuCtrl->numAtuWin; winCnt++) 
            {
                dev_info(pVspaDev->dev, "ATU Window[%d], Start 0x%llx, Size 0x%llx, Mapped Addr 0x%llx\n",
                        winCnt, pAtuCtrl->vspaAtuWin[winCnt].windwBaseAdd,
                        pAtuCtrl->vspaAtuWin[winCnt].windwSz, pAtuCtrl->vspaAtuWin[winCnt].windwMappedAdd);
            }
        }
    }

    return err;
}// ParseAtuDts()


/******************************************************************************/
/**
 * @brief   Get dts properties
 */
static int32_t VspaGetDtsProperties(struct device_node *pNode, vspaDevice_t *pVspaDev)
{
    u32 prop  = 0;
    dma_addr_t prop1 = 0;
    struct resource res;
    vspaControl_t *pVspaCtrl = &pVspaDev->vspaCtrl;
    int32_t err = 0;

    if (pNode == NULL)
    {
        err = -EINVAL;
    }
    else
    {
        if (of_property_read_u32(pNode, "vspadev-id", &prop) < 0) 
        {
            dev_err(pVspaDev->dev, "vspadev-id attribute not found\n");
            err = -EINVAL;
        }
        pVspaCtrl->id = prop;

        if (of_property_read_u64(pNode, "dbgregstart", &prop1) < 0) 
        {
            dev_err(pVspaDev->dev, "dbgregstart attribute not found for %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
            err = -EINVAL;
        }
    }
    if (err ==0)
    {
        pVspaCtrl->pDbgAddr = (u32 *)(dma_addr_t)prop1;

        if (of_property_read_u32(pNode, "dbgreglen", &prop) < 0) 
        {
            dev_err(pVspaDev->dev, "dbgreglen attribute not found for %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
            err = -EINVAL;
        }
    }
    if(err == 0)
    {
        pVspaCtrl->dbgSize = prop;

        pVspaCtrl->cmdBufferBytes = gsCmdBufferBytes;
        if (of_property_read_u32(pNode, "cmd_buf_size", &prop) >= 0)
            pVspaCtrl->cmdBufferBytes = prop;
        if (pVspaCtrl->cmdBufferBytes > (32*1024) || pVspaCtrl->cmdBufferBytes & 255) 
        {
            dev_err(pVspaDev->dev, "invalid cmdBufferSize %d for %s%d\n", pVspaCtrl->cmdBufferBytes, VSPA_DEVICE_NAME,
                pVspaCtrl->id);
            err = -EINVAL;
        }
    }
    if(err == 0)
    {
        if (of_get_property(pNode, "interrupts", NULL)) 
        {
            pVspaDev->vspa_irq_no    = irq_of_parse_and_map(pNode, 0);
            if (pVspaDev->vspa_irq_no == 0) 
            {
                dev_err(pVspaDev->dev, "Interrupt not found for %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
                err = -EINVAL;
            }
        } 
        else 
        {
            dev_err(pVspaDev->dev, "Interrupt numbers not found for %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
            err = -EINVAL;
        }
    }
    if(err == 0)
    {
        if (of_property_read_u64(pNode, "shbufstart", &prop1) < 0) 
        {
            dev_err(pVspaDev->dev, "shbufstart attribute not found for %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
            err = -EINVAL;
        }
    }
    if(err == 0)
    {
        pVspaCtrl->shbufstart = prop1;

        if (of_property_read_u32(pNode, "shbuflen", &prop) < 0) 
        {
            dev_err(pVspaDev->dev, "shbuflen attribute not found for %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
            err = -EINVAL;
        }
    }
    if(err == 0)
    {
        pVspaCtrl->shbuflen = prop;

        if (of_address_to_resource(pNode, 0, &res)) 
        {
            dev_err(pVspaDev->dev, "VSPA reg map not found for %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
            err = -EINVAL;
        } 
        else 
        {
            pVspaCtrl->pMemAddr = (u32 __iomem *)res.start;
            pVspaCtrl->memSize =  resource_size(&res);
            /* Parse ATU Node */
            if (LAX_PLATFORM_ATU != 0 && ParseAtuDts(pNode, pVspaDev)) 
            {
                dev_err(pVspaDev->dev, "VSPA-ATU DTS parsing failed %s%d\n", VSPA_DEVICE_NAME, pVspaCtrl->id);
                err = -EINVAL;
            }
        }
    }

    return err;
}// VspaGetDtsProperties()


/******************************************************************************/
/**
 * @brief   Declared vspa irq handler
 */
static irqreturn_t VspaDevIrqHandler(int irq, void *pDev) 
{
    vspaControl_t *pVspaCtrl = &(((vspaDevice_t *)pDev)->vspaCtrl);
    return VspaIrqHandler(irq, pVspaCtrl);
}// VspaDevIrqHandler()


/******************************************************************************/
/**
 * @brief   Device probe
 */
static int VspaProbe(struct platform_device *pdev)
{
    vspaDevice_t *pVspaDev = NULL;
    vspaControl_t *pVspaCtrl;
    struct device_node *pNode = pdev->dev.of_node;
    struct device *pDev = &pdev->dev;
    struct device *pSysFsDev;
    atuControl_t *pAtuCtrl;
    dev_t devNo;
    u8 deviceName[10];
    int err = 0;

    BUG_ON(gsNumVspaMajor == 0 || gspVspaClass == NULL);

    /* Allocate vspa device structure */
    pVspaDev = kzalloc(sizeof(vspaDevice_t), GFP_KERNEL);
    if (pVspaDev == NULL) 
    {
        pr_info(": failed to allocate vspa_dev\n");
        err = -ENOMEM;
    }
    else
    {
        pVspaDev->dev = pDev;

        if (VspaGetDtsProperties(pNode, pVspaDev) < 0) 
        {
            pr_info("VSPA DTS entry parse failed.\n");
            err = -EINVAL;
        }
        else
        {
            pVspaCtrl = &pVspaDev->vspaCtrl;

            devNo = MKDEV(gsNumVspaMajor, gsNumVspaMinor + pVspaCtrl->id);
            sprintf(deviceName, VSPA_DEVICE_NAME "%d", pVspaCtrl->id);

            dev_set_drvdata(&pdev->dev, pVspaDev);
            pAtuCtrl = &pVspaDev->atuCtrl;

            if (LAX_PLATFORM_ATU != 0)
            {
                pAtuCtrl->pVspaAtuRegs = ioremap((dma_addr_t)pAtuCtrl->pVspaAtuAddr, pAtuCtrl->vspaAtuSize);
            }

            VspaInit(pVspaCtrl);
            /*TODO move this into the VspaInit */
            err = request_irq(pVspaDev->vspa_irq_no, VspaDevIrqHandler,
                        0, deviceName, pVspaDev);
            if (err < 0) 
            {
                pr_info("%s: request_irq() err = %d\n", deviceName, err);
            }
            else
            {
                pVspaDev->eldFilename[0] = '\0';

                cdev_init(&pVspaDev->cdev, &gsVspaFops);
                pVspaDev->cdev.owner = THIS_MODULE;

                err = cdev_add(&pVspaDev->cdev, devNo, 1);
                if (err < 0) 
                {
                    pr_info("Error %d while adding %s", err, deviceName);
                }
                else
                {
                    /* Create sysfs device */
                    pSysFsDev = device_create(gspVspaClass, pDev, devNo, NULL, deviceName);
                    if (IS_ERR(pSysFsDev)) 
                    {
                        err = PTR_ERR(pSysFsDev);
                        pr_info("Error %d while creating %s", err, deviceName);
                    }
                    else
                    {

                        err = sysfs_create_group(&pDev->kobj, &gAttrGroup);
                        if (err < 0) 
                        {
                            pr_info("error %d while creating group %s", err, deviceName);
                            device_destroy(gspVspaClass, devNo);
                        }
                        else
                        {
                            VspaDisableAllIRQs(pVspaCtrl);

                            /* Enable ATU entries */
                            /* TODO: Can this be handled using ioctl()
                             * as ATU is not part of VSPA device */
                            if (LAX_PLATFORM_ATU != 0 && 0 == gsNumVspaDevs)
                            {
                               VspaAtuEnable(pAtuCtrl);
                            }
                            if (0 == gsNumVspaDevs)
                            {
                               err = VspaOalCommInit();
                            }

                            gsNumVspaDevs++;
                        }
                    }
                    if(err < 0)
                    {
                        cdev_del(&pVspaDev->cdev);
                    }
                }
                if(err < 0)
                {
                    free_irq(pVspaDev->vspa_irq_no, pVspaDev);
                }
            }
            if(err < 0)
            {
                VspaDeInit(pVspaCtrl);
                if (LAX_PLATFORM_ATU != 0)
                {
                    VspaAtuDisable(pAtuCtrl);
                    iounmap(pAtuCtrl->pVspaAtuRegs);
                }
            }
        }
        if(err < 0)
        {
            kfree(pVspaDev);
        }
    }
    return err;
}// VspaProbe()


/******************************************************************************/
/**
 * @brief   Device remove
 */
static int VspaRemove(struct platform_device *ofpdev)
{
    struct device *pDev = &ofpdev->dev;
    vspaDevice_t *pVspaDev = dev_get_drvdata(&ofpdev->dev);
    vspaControl_t *pVspaCtrl = &pVspaDev->vspaCtrl;

    BUG_ON(pVspaDev == NULL || gspVspaClass == NULL);

    VspaShutdownTimer(pVspaCtrl);
    VspaDisableAllIRQs(pVspaCtrl);

    free_irq(pVspaDev->vspa_irq_no, pVspaDev);

    dev_set_drvdata(&ofpdev->dev, NULL);

    VspaDeInit(pVspaCtrl);
    if (LAX_PLATFORM_ATU != 0)
    {
        VspaAtuDisable(&pVspaDev->atuCtrl);
        iounmap(pVspaDev->atuCtrl.pVspaAtuRegs);
    }

    sysfs_remove_group(&pDev->kobj, &gAttrGroup);
    device_destroy(gspVspaClass, MKDEV(gsNumVspaMajor, gsNumVspaMinor + pVspaCtrl->id));
    cdev_del(&pVspaDev->cdev);
    kfree(pVspaDev);

    if (gsNumVspaDevs > 0)
    {
        gsNumVspaDevs--;
    }
    if (gsNumVspaDevs == 0 && VspaOalCommExit() != 0) {
        pr_err("VspaOalCommExit failed \n");
    }
    return 0;
}// VspaRemove()

static const struct of_device_id gsVspaMatch[] = 
{
    {.compatible = "fsl,vspa",},
    {},
};

static struct platform_driver gsVspaDriver = 
{
    .driver = 
    {
        .name = "fsl-vspa",
        .owner = THIS_MODULE,
        .of_match_table = gsVspaMatch,
    },
    .probe = VspaProbe,
    .remove = VspaRemove,
};


/******************************************************************************/
/**
 * @brief   Driver/module init
 */
static int __init VspaModInit(void)
{
    int err;
    dev_t devNo;

    /* Register our major, and accept a dynamic number. */
    err = alloc_chrdev_region(&devNo, 0, MAX_VSPA, VSPA_DEVICE_NAME);
    if (err < 0) 
    {
        pr_err("vspa: can't get major number: %d\n", err);
    }
    else
    {
        gsNumVspaMajor = MAJOR(devNo);
        gsNumVspaMinor = MINOR(devNo);

        /* Create the device class if required */
        gspVspaClass = class_create(THIS_MODULE, VSPA_DEVICE_NAME);
        if (IS_ERR(gspVspaClass)) 
        {
            unregister_chrdev_region(gsNumVspaMajor, MAX_VSPA);
            err = PTR_ERR(gspVspaClass);
            pr_err("vspa: class_create() failed %d\n", err);
        }
        else
        {

            err = platform_driver_register(&gsVspaDriver);
            if (err != 0)
            {
                class_destroy(gspVspaClass);
                unregister_chrdev_region(gsNumVspaMajor, MAX_VSPA);
            }
        }
    }
    return err;
}// VspaModInit()


/******************************************************************************/
/**
 * @brief   Driver/module exit
 */
static void __exit VspaModExit(void)
{
    platform_driver_unregister(&gsVspaDriver);
    class_destroy(gspVspaClass);
    unregister_chrdev_region(gsNumVspaMajor, MAX_VSPA);
}// VspaModExit

module_init(VspaModInit);
module_exit(VspaModExit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("NXP Semiconductors");
MODULE_DESCRIPTION("NXP LAX Driver");
