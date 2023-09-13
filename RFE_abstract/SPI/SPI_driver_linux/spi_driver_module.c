/*
 * Copyright 2021 NXP
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
#include <linux/mm.h>

#include "spi_driver_module.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define DEVICE_NAME "rsdk_spi" /* Dev name as it appears in /proc/devices   */

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("NXP Semiconductors");
MODULE_DESCRIPTION("NXP RSDK SPI Driver");
MODULE_VERSION("1.00");
MODULE_ALIAS("RSDK_SPI");

// User space access to SPI registers
#define SPI_IP_PROT_MEM_U32         4U
#define REGPROT_GCR_UAA_MASK_U32    0x00800000UL
#define GCR_OFFSET_U32              0x900UL

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
static s32                       gNumSpiMajor;
static s32                       gNumSpiMinor;
static dev_t                     devNum = MKDEV(0, 0);
static struct class *            gspSpiClass;
static const struct of_device_id gsSpiMatch[] = {
    {
        .compatible = "nxp,rsdk_spi",
    },
    {},
};

spiDevice_t spiDevice;
/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/
static int32_t SpiMemMmap(struct file *filp, struct vm_area_struct *vma)
{
    size_t size = vma->vm_end - vma->vm_start;

    if ((vma->vm_pgoff > spiDevice.dtsInfo.memSize) || (size > (spiDevice.dtsInfo.memSize - vma->vm_pgoff)))
    {
        PR_ERR("rsdk_spi_driver: SpiMemMmap: offset: %lu memSize %lld size %lu", vma->vm_pgoff, spiDevice.dtsInfo.memSize, size);
        return -EINVAL;
    }

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	vma->vm_pgoff += ((uintptr_t)spiDevice.dtsInfo.pMemMapBaseAddr >> PAGE_SHIFT);
	PR_ALERT("rsdk_spi_driver: SpiMemMmap: Map %lx bytes: phys 0x%lx to virt 0x%lx\n", size, (uintptr_t)spiDevice.dtsInfo.pMemMapBaseAddr, vma->vm_start);

	if (io_remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot))
    {
        PR_ERR("rsdk_spi_driver: SpiMemMmap: io_remap_pfn_range");
		return -EAGAIN;
    }
	return 0;
}

/**
 * @brief   Open device
 */
static int SpiOpen(struct inode *pInode, struct file *fp)
{
    spiDevice_t *pSpiDevice = container_of(pInode->i_cdev, spiDevice_t, cdevice);
    int          err = 0;

    if (pSpiDevice != NULL)
    {
        fp->private_data = pSpiDevice;
        pSpiDevice->gUserPid = current->pid;
        PR_ALERT("rsdk_spi_driver: SpiOpen done, PID = %d\n", pSpiDevice->gUserPid);
    }
    else
    {
        PR_ERR("rsdk_spi_driver: SpiOpen: No device context found for %s %d\n", DEVICE_NAME, iminor(pInode));
        err = -ENODEV;
    }
    return err;
}

/**
 * @brief   release/close device
 */
static int SpiRelease(struct inode *pInode, struct file *fp)
{
    spiDevice_t *pSpiDevice = (spiDevice_t *)fp->private_data;
    int          err = 0;

    if (pSpiDevice == NULL)
    {
        PR_ERR("rsdk_spi_driver: No device context found for %s %d\n", DEVICE_NAME, iminor(pInode));
        err = -ENODEV;
    }
    PR_ALERT("rsdk_spi_driver: SpiRelease done, PID = %d\n", pSpiDevice->gUserPid);
    return err;
}

static const struct file_operations gSpiFileOps = {
    .owner = THIS_MODULE,
    .open = SpiOpen,
    .release = SpiRelease,
    .mmap = SpiMemMmap
};

/**
 * @brief   Get dts properties
 */
static int RsdkSpiGetDtsProperties(struct device_node *pNode, spiDtsInfo_t *dtsInfo)
{
    int             err = 0;
    u32             prop = 0;
    struct resource res;

    if (pNode == NULL)
    {
        PR_ERR("rsdk_spi_driver: RsdkSpiGetDtsProperties: device_node is null");
        return -EINVAL;
    }
    if (of_property_read_u32(pNode, "spidev-id", &prop) < 0)
    {
        PR_ERR("rsdk_spi_driver: RsdkSpiGetDtsProperties: spidev-id attribute not found\n");
        return -EINVAL;
    }
    dtsInfo->devId = prop;
 
    if (of_address_to_resource(pNode, 0, &res) != 0)
    {
        PR_ERR("rsdk_spi_driver: RsdkSpiGetDtsProperties: Reg map not found for %s%d\n", DEVICE_NAME, dtsInfo->devId);
        return -EINVAL;
    }
    dtsInfo->pMemMapBaseAddr = (u32 __iomem *)res.start;
    dtsInfo->memSize = resource_size(&res);
    return err;
}

/**
 * @brief   Device probe
 */
static int SpiProbe(struct platform_device *pPlatDev)
{
    int                 err = 0;
    struct device *     pDevice = &pPlatDev->dev;
    struct device_node *pNode = pPlatDev->dev.of_node;
    struct device *     pSysFsDev;
    uint32_t *          regProt;

    PR_ALERT("rsdk_spi_driver: SpiProbe: start");
    BUG_ON((gNumSpiMajor == 0) || (gspSpiClass == NULL));

    if (RsdkSpiGetDtsProperties(pNode, &spiDevice.dtsInfo) != 0)
    {
        PR_ERR("rsdk_spi_driver: SpiProbe: RsdkSpiGetDtsProperties failed.\n");
        return -EINVAL;
    }
        
    dev_set_drvdata(pDevice, &spiDevice);
    cdev_init(&spiDevice.cdevice, &gSpiFileOps);
    err = cdev_add(&spiDevice.cdevice, devNum, 1);
    if (err < 0)
    {
        PR_ERR("rsdk_spi_driver: SpiProbe: cdev_add: %d", err);
        return err;
    }
    pSysFsDev = device_create(gspSpiClass, pDevice, devNum, NULL, DEVICE_NAME);
    if (IS_ERR(pSysFsDev))
    {
        err = PTR_ERR(pSysFsDev);
        PR_ERR("rsdk_spi_driver: SpiProbe: device_create: %d", err);
        cdev_del(&spiDevice.cdevice);
        return err;
    }
  
    // Allow register access from user space
    regProt = (uint32_t *)ioremap((phys_addr_t)(spiDevice.dtsInfo.pMemMapBaseAddr + GCR_OFFSET_U32), (size_t)SPI_IP_PROT_MEM_U32);
    if (regProt == NULL)
    {
        PR_ERR("rsdk_spi_driver: SpiProbe: ioremap");
        device_destroy(gspSpiClass, devNum);
        cdev_del(&spiDevice.cdevice);
        return EFAULT;
    }
    *(regProt) |= REGPROT_GCR_UAA_MASK_U32;
    iounmap(regProt);
    
    return err;
}

/**
 * @brief   Device remove
 */
static int SpiRemove(struct platform_device *ofpdev)
{
    spiDevice_t *pSpiDev = dev_get_drvdata(&ofpdev->dev);
    dev_set_drvdata(&ofpdev->dev, NULL);
    device_destroy(gspSpiClass, devNum);
    cdev_del(&pSpiDev->cdevice);
    PR_ALERT("rsdk_spi_driver: SpiRemove done.\n");
    return 0;
}

static struct platform_driver gsSpiDriver = {
    .driver =
        {
            .name = "nxp-resdk_spi",
            .owner = THIS_MODULE,
            .of_match_table = gsSpiMatch,
        },
    .probe = SpiProbe,
    .remove = SpiRemove,
};

/**
 * @brief   Module init
 */
static int __init SPIModuleInit(void)
{
    int err = 0;

    err = alloc_chrdev_region(&devNum, 0, 1, DEVICE_NAME);
    if (err != 0)
    {
        PR_ERR("rsdk_spi_driver: SPIModuleInit: alloc_chrdev_region: %d\n", err);
        return err;
    }
    gNumSpiMajor = MAJOR(devNum);
    gNumSpiMinor = MINOR(devNum);

    gspSpiClass = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(gspSpiClass))
    {
        unregister_chrdev_region(gNumSpiMajor, 0);
        err = PTR_ERR(gspSpiClass);
        PR_ERR("rsdk_spi_driver: SPIModuleInit: class_create: %d\n", err);
        return err;
    }

    err = platform_driver_register(&gsSpiDriver);
    if (err != 0)
    {
        class_destroy(gspSpiClass);
        unregister_chrdev_region(gNumSpiMajor, 1);        
        PR_ERR("rsdk_spi_driver: SPIModuleInit: platform_driver_register: %d\n", err);
        return err;
    }
    PR_ALERT("rsdk_spi_driver: SPIModuleInit: RSDK SPI driver init OK. Major number %d.\n", gNumSpiMajor);

    return err;
}

/**
 * @brief   Module exit
 */
static void __exit SPIModuleExit(void)
{
    platform_driver_unregister(&gsSpiDriver);
    class_destroy(gspSpiClass);
    unregister_chrdev_region(devNum, 1);
    PR_ALERT("rsdk_spi_driver: SPIModuleExit: SPI driver exit. \n");
}

module_init(SPIModuleInit);
module_exit(SPIModuleExit);

#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
