/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>

#include <oal_log.h>
#include <oal_utils.h>
#include <linux_device.h>

int32_t OAL_InitDevFile(OAL_DevFile_t *apDev, struct file_operations *apFops,
                        const char8_t *acpName)
{
	int32_t lRet = 0;
	size_t lLen;

	if ((apDev == NULL) || (acpName == NULL) || (apFops == NULL)) {
		lRet = -EINVAL;
		goto init_dev_file_exit;
	}

	apDev->mInit = 0U;

	lLen = strnlen(acpName, OAL_MAX_NAME_LEN);
	if (lLen == OAL_MAX_NAME_LEN) {
		OAL_LOG_WARNING(
		    "The name of the device is too long and will "
		    "be truncated to %zu chars (%s).\n",
		    sizeof(apDev->mName) - 1, acpName);
		lLen = OAL_MAX_NAME_LEN - 1U;
	}

	cdev_init(&apDev->mCDev, apFops);

	(void)snprintf(apDev->mName, sizeof(apDev->mName),
	               "%s" OAL_LINUX_CLASS_SUFFIX, acpName);
	apDev->mpClass = class_create(THIS_MODULE, apDev->mName);
	if (IS_ERR(apDev->mpClass)) {
		lRet           = (int32_t)PTR_ERR(apDev->mpClass);
		apDev->mpClass = NULL;
		goto init_dev_file_exit;
	}

	(void)snprintf(apDev->mName, sizeof(apDev->mName),
	               "%s" OAL_LINUX_REGION_SUFFIX, acpName);
	lRet = alloc_chrdev_region(&apDev->mDev, 0, 1, apDev->mName);
	if (lRet < 0) {
		goto fail_alloc_device;
	}

	apDev->mpDevice =
	    device_create(apDev->mpClass, NULL, apDev->mDev, NULL, acpName);
	if (IS_ERR(apDev->mpDevice)) {
		lRet            = (int32_t)PTR_ERR(apDev->mpDevice);
		apDev->mpDevice = NULL;
		goto fail_device_create;
	}

	lRet = cdev_add(&apDev->mCDev, apDev->mDev, 1);
	if (lRet == -1) {
		goto fail_cdev_add;
	}

	apDev->mInit = 1U;

	{
		void *lpDest       = (void *)apDev->mName;
		const void *lcpSrc = (const void *)acpName;
		(void)memcpy(lpDest, lcpSrc, lLen);
	}

	apDev->mName[lLen] = (char8_t)0;

	goto init_dev_file_exit;

fail_cdev_add:
	device_destroy(apDev->mpClass, apDev->mDev);
fail_device_create:
	unregister_chrdev_region(apDev->mDev, 1);
fail_alloc_device:
	class_destroy(apDev->mpClass);
	apDev->mpClass = NULL;
init_dev_file_exit:
	return lRet;
}

int32_t OAL_DestroyDevFile(OAL_DevFile_t *apDev)
{
	int32_t lRet = 0;

	if (apDev == NULL) {
		lRet = -EINVAL;
		goto destroy_exit;
	}

	if (apDev->mInit != 0U) {
		cdev_del(&apDev->mCDev);
		device_destroy(apDev->mpClass, apDev->mDev);
		unregister_chrdev_region(apDev->mDev, 1);
		class_destroy(apDev->mpClass);
	}
	apDev->mInit = 0U;

destroy_exit:
	return lRet;
}
