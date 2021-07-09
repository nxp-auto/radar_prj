/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_LINUX_DEVICE_H
#define OAL_LINUX_DEVICE_H

#include <oal_utils.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>

#define OAL_MAX_NAME_LEN 64U
#define OAL_LINUX_REGION_SUFFIX "_reg"
#define OAL_LINUX_CLASS_SUFFIX "_class"

#define OAL_LINUX_DEV_NAME_MAX_LEN (OAL_MAX_NAME_LEN + \
				    sizeof(OAL_LINUX_CLASS_SUFFIX))

/**
 * @brief Holds device file specific information
 */
struct OAL_DevFile {
	char8_t mName[OAL_LINUX_DEV_NAME_MAX_LEN];          ///< Device name
	struct class *mpClass;    ///< Linux device class
	dev_t mDev;               ///< Linux device
	struct device *mpDevice;  ///< Basic device structure
	struct cdev mCDev;        ///< Linux char device
	uint8_t mInit;            ///< Initialization flag
};

typedef struct OAL_DevFile OAL_DevFile_t;

/**
 * @brief Creates a character device file
 *
 * @param[in] apDev   Device data structure corresponding to the new
 *                    created file
 * @param[in] apFops  File operations to be associated with the device file
 *                    created.
 * @param[in] acpName Device file name
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int32_t OAL_InitDevFile(OAL_DevFile_t *apDev, struct file_operations *apFops,
                        const char8_t *acpName);

/**
 * @brief Removes a character device file previously created with
 *        <code>OAL_InitDevFile()</code> and free associated resources
 *
 * @param[in] apDev A <tt>OAL_DevFile_t</tt> structure pointer associated with
 *                  the device file
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int32_t OAL_DestroyDevFile(OAL_DevFile_t *apDev);

#endif /* OAL_LINUX_DEVICE_H */
