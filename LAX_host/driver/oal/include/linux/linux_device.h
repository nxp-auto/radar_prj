/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LINUX_DEVICE_H
#define LINUX_DEVICE_H

#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define MAX_NAME_LEN       64

/**
 * @brief Holds device file specific information
 */
struct OAL_DevFile {
	char *mpName; ///< Device name
	struct class *mpClass; ///< Linux device class
	dev_t mDev; ///< Linux device
	struct device *mpDevice; ///< Basic device structure
	struct cdev mCDev; ///< Linux char device
};

typedef struct OAL_DevFile OAL_DevFile_t;

/**
 * @brief Creates a character device file
 *
 * @param[in] apDev   Device data structure corresponding to the new
 *                    created file
 * @param[in] apFops  File operations to be associated with the device file
 *                    created.
 * @param[in] apcName Device file name
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int OAL_initDevFile(OAL_DevFile_t *apDev,
		struct file_operations *apFops,
		const char *apcName);

/**
 * @brief Removes a character device file previously created with
 *        <code>OAL_initDevFile()</code> and free associated resources
 *
 * @param[in] apDev A <tt>OAL_DevFile_t</tt> structure pointer associated with
 *                  the device file
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int OAL_destroyDevFile(OAL_DevFile_t *apDev);

#endif /* LINUX_DEVICE_H */
