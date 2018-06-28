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

#include "oal_log.h"
#include "oal_comm.h"
#include "oal_comm_kernel.h"
#include "os_oal_comm_kernel.h"
#include "linux_device.h"

struct oal_dispatcher {
	OAL_FuncArgs_t* mpArgsBuff;
	struct file* mpFile;
	size_t mFillLevel;
	size_t mMaxSize;
	OAL_RPCService_t mService;
};

struct OAL_RPCService {
	OAL_dispatch_t mDispatch;
	OAL_DevFile_t mDevData;
	struct file_operations mFops;
	void* mpInBuffer;
	size_t mpInBufferSize;
	struct mutex mLock;
	OAL_ServiceData_t mData;
};


static long consumeMessage(struct comm_args aInArgs, struct file* apFile,
			struct OAL_RPCService *apService)
{
	long lRet = 0;
	uint32_t lFret;
	struct oal_dispatcher lDispatcher;

	lDispatcher.mpArgsBuff = aInArgs.out_buff;
	lDispatcher.mFillLevel = (size_t)0;
	lDispatcher.mMaxSize = aInArgs.n_out_args;
	lDispatcher.mpFile = apFile;
	lDispatcher.mService = apService;


	if (apService->mpInBufferSize < aInArgs.in_arg_buff_len) {
		apService->mpInBufferSize = aInArgs.in_arg_buff_len;

		if (aInArgs.in_arg_buff_len > KMALLOC_MAX_SIZE) {
			OAL_LOG_ERROR("Input payload too long." \
				      "Maximum size should be %lu\n",
				      KMALLOC_MAX_SIZE);
			lRet = -EIO;
			goto end_consumeMessage;
		}

		apService->mpInBuffer = krealloc(apService->mpInBuffer,
						apService->mpInBufferSize,
						GFP_KERNEL);

		if (apService->mpInBuffer == NULL) {
			OAL_LOG_ERROR("Failed to allocate memory !\n");
			lRet = -EIO;
			goto end_consumeMessage;
		}
	}

	// Copy serialized input arguments from user-space
	if (copy_from_user(apService->mpInBuffer, (void*)aInArgs.in_arg_buff,
			   aInArgs.in_arg_buff_len) != 0) {
		OAL_LOG_ERROR("copy_from_user failed!\n");
		lRet = -1;
		goto end_consumeMessage;
	}

	// Call the dispatcher
	lFret = apService->mDispatch(&lDispatcher, aInArgs.func_id,
				 apService->mpInBuffer,
				 aInArgs.in_arg_buff_len);

	if (copy_to_user((void*)aInArgs.pRet, &lFret, sizeof(lFret)) != 0) {
		OAL_LOG_ERROR("copy_to_user failed!\n");
		lRet = -1;
	}

end_consumeMessage:
	return lRet;
}

static ssize_t OAL_WriteCallback(struct file* apFile,
                      const char*  apcData,
                      size_t       aSize,
                      loff_t*      apOffset)
{
	struct comm_args lInArgs;
	struct cdev *lCdev;
	struct OAL_DevFile *lDevData;
	struct OAL_RPCService *lpService;
	ssize_t lRet = aSize;

	if ((apFile == NULL) || (apFile->private_data == NULL)) {
		lRet = -EIO;
		goto write_exit;
	}

	// Get cdev from private data
	lCdev = apFile->private_data;

	// Get device data
	lDevData = GET_PARENT_STRUCT(lCdev, struct OAL_DevFile, mCDev);

	// Get service
	lpService = GET_PARENT_STRUCT(lDevData, struct OAL_RPCService, mDevData);

	if (lpService == NULL) {
		OAL_LOG_ERROR("Invalid service\n");
		lRet = -EIO;
		goto write_exit;
	}

	if (aSize != sizeof(lInArgs)) {
		OAL_LOG_ERROR("Incomplete input arguments\n");
		lRet = (ssize_t)(-EOPNOTSUPP);
		goto write_exit;
	}

	// Guard the access to service, in consequnence all RPC calls
	// are serialized.
	mutex_lock(&lpService->mLock);
	if (copy_from_user(&lInArgs, (void*)apcData, sizeof(lInArgs)) != 0) {
		OAL_LOG_ERROR("copy_from_user failed!\n");
		lRet = (ssize_t)-EIO;
	} else {
		consumeMessage(lInArgs, apFile, lpService);
	}
	mutex_unlock(&lpService->mLock);

write_exit:
	return lRet;
}

struct file* OAL_WriteGetFile(oal_dispatcher_t *apDispatcher)
{
	return apDispatcher->mpFile;
}

int OAL_WriteOpen(struct inode *apInode, struct file *apFile)
{
	if ((apInode != NULL) && (apFile != NULL)) {
		// Set cdev as private data
		apFile->private_data = apInode->i_cdev;
	}

	return 0;
}

static int defaultClose(struct inode *apInode, struct file *apFile)
{
	UNUSED_ARG(apInode);
	UNUSED_ARG(apFile);
	return 0;
}

OAL_RPCService_t OAL_RPCRegister(char *apName, OAL_dispatch_t aDispatch)
{
	OAL_RPCService_t lpService = NULL;
	int lRet;

	lpService = (OAL_RPCService_t) kzalloc(sizeof(*lpService), GFP_KERNEL);
	if (lpService == NULL) {
		OAL_LOG_ERROR("OAL_RPCRegister failed\n");
	} else {
		lpService->mDispatch = aDispatch;

		lpService->mFops.owner = THIS_MODULE;
		lpService->mFops.open = OAL_WriteOpen;
		lpService->mFops.release = defaultClose;
		lpService->mFops.write = OAL_WriteCallback;

		mutex_init(&lpService->mLock);
		lRet = OAL_initDevFile(&lpService->mDevData,
				&lpService->mFops, apName);
		if (lRet != 0) {
			kfree(lpService);
			lpService = NULL;
		}
	}

	return lpService;
}

int OAL_SetCustomFileOperations(OAL_RPCService_t aService,
		struct file_operations* apFileOps)
{
	int lRet = 0;
	size_t lLen;
	char *lpName;

	if ((aService == NULL) || (apFileOps == NULL)) {
		lRet = -EINVAL;
		goto set_fops_exit;
	}

	mutex_lock(&aService->mLock);

	// Save device name
	lLen = strnlen(aService->mDevData.mpName, MAX_NAME_LEN) + 1;
	lpName = kzalloc(lLen, GFP_KERNEL);
	if (lpName == NULL) {
		lRet = -ENOMEM;
		goto set_fops_exit_lock;
	}

	memcpy(lpName, aService->mDevData.mpName, lLen);

	// Release previously registered device
	lRet = OAL_destroyDevFile(&aService->mDevData);
	if (lRet != 0) {
		goto free_name;
	}

	memcpy(&aService->mFops, apFileOps, sizeof(aService->mFops));
	memset(&aService->mDevData, 0, sizeof(aService->mDevData));

	aService->mFops.owner = THIS_MODULE;
	aService->mFops.open = OAL_WriteOpen;
	aService->mFops.write = OAL_WriteCallback;

	// Register updated operations
	lRet = OAL_initDevFile(&aService->mDevData,
			&aService->mFops,
			lpName);

free_name:
	kfree(lpName);
set_fops_exit_lock:
	mutex_unlock(&aService->mLock);
set_fops_exit:
	return lRet;
}

int OAL_RPCSetPrivateData(OAL_RPCService_t aServ, OAL_ServiceData_t aData)
{
	int ret = 0;
	if (aServ == NULL) {
		ret = -EINVAL;
	} else {
		aServ->mData = aData;
	}

	return ret;
}

int OAL_RPCGetPrivateData(OAL_RPCService_t aServ, OAL_ServiceData_t *apData)
{
	int ret = 0;
	if ((aServ == NULL) || (apData == NULL)) {
		ret = -EINVAL;
	} else {
		*apData = aServ->mData;
	}

	return ret;
}

int OAL_RPCCleanup(const OAL_RPCService_t aService)
{
	struct OAL_RPCService *lpServ = aService;

	if (lpServ != NULL) {
		(void) OAL_destroyDevFile(&aService->mDevData);
		mutex_destroy(&lpServ->mLock);

		if (lpServ->mpInBuffer != NULL) {
			kfree(lpServ->mpInBuffer);
		}

		kfree(lpServ);
	}


	return 0;
}

int OAL_RPCAppendReply(oal_dispatcher_t *apDispatcher, char *data, size_t size)
{
	int lRet = 0;
	OAL_FuncArgs_t* lCurrnetArg;
	OAL_FuncArgs_t lUsArg;

	// Append datasize and data buffer to be sent to userspace
	if (apDispatcher->mpArgsBuff == NULL){
		lRet = -1;
		OAL_LOG_WARNING("NULL output buffer\n");
		goto rpc_append_end;
	}

	if (apDispatcher->mpArgsBuff->data == NULL){
		lRet = -1;
		OAL_LOG_WARNING("NULL output buffer\n");
		goto rpc_append_end;
	}

	if (apDispatcher->mFillLevel >= apDispatcher->mMaxSize ) {
		OAL_LOG_WARNING("Output buffer overflow!\n");
		lRet = -1;
		goto rpc_append_end;
	}

	lCurrnetArg = &(apDispatcher->mpArgsBuff[apDispatcher->mFillLevel]);
	if (copy_from_user(&lUsArg, lCurrnetArg, sizeof(lUsArg)) != 0) {
		OAL_LOG_ERROR("copy_from_user failed!\n");
		lRet = -1;
		goto rpc_append_end;
	}

	// Check if argument size from userspace corresponds with the
	// one received
	if (lUsArg.size != size){
		OAL_LOG_WARNING("Argument size don't match! (%lu != %lu)\n",
				lUsArg.size, size);
		lRet = -1;
		goto rpc_append_end;
	}

	if (copy_to_user(lUsArg.data, data, size) != 0) {
		OAL_LOG_ERROR("copy_to_user failed!\n");
		lRet = -1;
		goto rpc_append_end;
	}

	apDispatcher->mFillLevel++;

rpc_append_end:
	return lRet;
}

OAL_RPCService_t OAL_RPCGetService(oal_dispatcher_t *apDisp)
{
	OAL_RPCService_t lServ = NULL;
	if (apDisp != NULL) {
		lServ = apDisp->mService;
	}

	return lServ;
}

int OAL_RPCGetClientPID(oal_dispatcher_t *apDispatch, pid_t *apClientPID)

{
	int lRet;

	if ((apDispatch == NULL) || (apClientPID == NULL)) {
		lRet = -EINVAL;
	} else {
		*apClientPID = task_pgrp_nr(current);
	}

	return lRet;
}
