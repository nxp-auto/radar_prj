/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include "linux_device.h"
#include "oal_log.h"

#define REGION_SUFFIX      "_reg"
#define CLASS_SUFFIX       "_class"

int OAL_initDevFile(OAL_DevFile_t *apDev,
		struct file_operations *apFops,
		const char *apcName)
{
	int lRet = 0;
	size_t lLen;
	size_t lMaxSuffixSize = max(sizeof(REGION_SUFFIX),
					sizeof(CLASS_SUFFIX));
	size_t lMaxLen;

	if ((apDev == NULL) || (apcName == NULL) || (apFops == NULL)) {
		lRet = -EINVAL;
		goto init_dev_file_exit;
	}

	lLen = strnlen(apcName, MAX_NAME_LEN);
	lMaxLen = lLen + lMaxSuffixSize;

	apDev->mpName = kzalloc(lMaxLen, GFP_KERNEL);
	if (apDev->mpName == NULL) {
		lRet = -ENOMEM;
		goto init_dev_file_exit;
	}

	(void) snprintf(apDev->mpName, lMaxLen, "%s" CLASS_SUFFIX,
			apcName);
	apDev->mpClass = class_create(THIS_MODULE, apDev->mpName);
	if (IS_ERR(apDev->mpClass)) {
		goto free_name;
	}

	(void) snprintf(apDev->mpName, lMaxLen, "%s" REGION_SUFFIX,
			apcName);
	lRet = alloc_chrdev_region(&apDev->mDev, 0, 1, apDev->mpName);
	if (lRet < 0) {
		goto fail_alloc_device;
	}

	apDev->mpDevice = device_create(apDev->mpClass, NULL, apDev->mDev,
			NULL, apcName);
	if (IS_ERR(apDev->mpDevice)) {
		goto fail_device_create;
	}

	cdev_init(&apDev->mCDev, apFops);

	lRet = cdev_add(&apDev->mCDev, apDev->mDev, 1);
	if (lRet == -1) {
		goto fail_cdev_add;
	}

	memcpy(apDev->mpName, apcName, lLen);
	if (lLen < lMaxLen) {
		apDev->mpName[lLen] = '\0';
	}
	goto init_dev_file_exit;

fail_cdev_add:
	device_destroy(apDev->mpClass, apDev->mDev);
fail_device_create:
	unregister_chrdev_region(apDev->mDev, 1);
fail_alloc_device:
	class_destroy(apDev->mpClass);
free_name:
	kfree(apDev->mpName);
init_dev_file_exit:
	return lRet;
}

int OAL_destroyDevFile(OAL_DevFile_t *apDev)
{
	int lRet = 0;

	if (apDev == NULL) {
		lRet = -EINVAL;
		goto destroy_exit;
	}

	device_destroy(apDev->mpClass, apDev->mDev);
	class_destroy(apDev->mpClass);
	cdev_del(&apDev->mCDev);
	unregister_chrdev_region(apDev->mDev, 1);
	kfree(apDev->mpName);

destroy_exit:
	return lRet;
}

