/*
 * Copyright 2017-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <qnx_dev_resource.h>
#include <sys/iofunc.h>
#include <sys/queue.h>

#include <oal_log.h>
#include <oal_memmap.h>
#include <oal_static_pool.h>

#define DESTRUCTOR_ATTRIBUTE(PRIORITY) __attribute__((destructor(PRIORITY)))
#define OAL_QNX_MAX_RESOURCES 100U

struct dev_resource {
	TAILQ_ENTRY(dev_resource) mTail;

	resource_release_t mCallback;
	void *mpData;
};

// Stack of resources with their release callbacks
static TAILQ_HEAD(, dev_resource) gQNXResources;

static struct dev_resource gsQnxResources[OAL_QNX_MAX_RESOURCES];
static OAL_DECLARE_STATIC_POOL(gsQnxResourcesPool,
                               OAL_ARRAY_SIZE(gsQnxResources));

static void init_resources_queue(void) { TAILQ_INIT(&gQNXResources); }

int32_t OAL_RegisterResource(resource_release_t aCallback, void *apData)
{
	int32_t lRet;
	struct dev_resource *lpRes            = NULL;
	static pthread_once_t lsResourcesOnce = PTHREAD_ONCE_INIT;

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&gsQnxResourcesPool, gsQnxResources,
	                                &lpRes);
	if ((lRet != 0) || (lpRes == NULL)) {
		OAL_LOG_ERROR(
		    "QNX resources pool is full, please adjust "
		    "its size.\n");
		lRet = -ENOMEM;
		goto reg_resource_exit;
	}

	lRet = pthread_once(&lsResourcesOnce, init_resources_queue);
	if (lRet != 0) {
		OAL_LOG_ERROR("pthread_once failed (%s)\n", strerror(errno));
		goto release_mem;
	}

	lpRes->mCallback = aCallback;
	lpRes->mpData    = apData;
	TAILQ_INSERT_HEAD(&gQNXResources, lpRes, mTail);

release_mem:
	if (lRet != 0) {
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsQnxResourcesPool,
		                                 gsQnxResources, lpRes);
	}

reg_resource_exit:
	return lRet;
}

void DESTRUCTOR_ATTRIBUTE(256) OAL_ReleaseResources(void)
{
	struct dev_resource *lpRes;
	TAILQ_FOREACH (lpRes, &gQNXResources, mTail) {
		lpRes->mCallback(lpRes->mpData);
		TAILQ_REMOVE(&gQNXResources, lpRes, mTail);
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsQnxResourcesPool,
		                                 gsQnxResources, lpRes);
	}
}

struct _rel_map_iomem_reg {
	uintptr_t mAddr;
	uint64_t mLen;
};

static struct _rel_map_iomem_reg gsRelMapIoMem[OAL_QNX_MAX_RESOURCES];
static OAL_DECLARE_STATIC_POOL(gsRelMapIoMemPool,
                               OAL_ARRAY_SIZE(gsRelMapIoMem));

static void rel_map_iomem_reg(void *apData)
{
	const struct _rel_map_iomem_reg *lcpRegMap = apData;
	if (apData != NULL) {
		(void)OAL_kmemunmap(lcpRegMap->mAddr, lcpRegMap->mLen);
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsRelMapIoMemPool,
		                                 gsRelMapIoMem, lcpRegMap);
	}
}

int32_t OAL_DevFdtMapIomemReg(const struct fdt_node *acpNode, int32_t aIndex,
                              uint8_t **apVirt, uint64_t *apLen)
{
	struct _rel_map_iomem_reg *lpRegMap = NULL;
	int32_t lRet                        = -ENODEV;
	int32_t lStatus;

	OAL_CHECK_NULL_PARAM(apVirt, dev_fdt_map_iomem_reg_exit);
	OAL_CHECK_NULL_PARAM(apLen, dev_fdt_map_iomem_reg_exit);

	lRet = OAL_FdtMapIomemReg(acpNode, aIndex, apVirt, apLen);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "Failed to map registers of a "
		    "device tree node\n");
		goto dev_fdt_map_iomem_reg_exit;
	}

	lStatus = OAL_ALLOC_ELEM_FROM_POOL(&gsRelMapIoMemPool, gsRelMapIoMem,
	                                   &lpRegMap);
	if ((lStatus != 0) || (lpRegMap == NULL)) {
		OAL_LOG_ERROR(
		    "QNX FDT map  pool is full, please "
		    "adjust its size.\n");
		lRet = -ENOMEM;
		goto ummap_mem;
	}

	lpRegMap->mAddr = (uintptr_t)*apVirt;
	lpRegMap->mLen  = *apLen;

	lRet = OAL_RegisterResource(rel_map_iomem_reg, lpRegMap);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to register resource\n");
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsRelMapIoMemPool,
		                                 gsRelMapIoMem, lpRegMap);
	}
ummap_mem:
	if (lRet != 0) {
		(void)OAL_kmemunmap((uintptr_t)*apVirt, *apLen);
	}
dev_fdt_map_iomem_reg_exit:
	return lRet;
}

static void rel_fdt_get_nmatches_by_compatible(void *apData)
{
	OAL_UNUSED_ARG(apData);
	(void)OAL_ReleaseFdt();
}

int32_t OAL_DevFdtGetNmatchesByCompatible(const char8_t *const *acpCompatible)
{
	int32_t lRet = OAL_FdtGetNmatchesByCompatible(acpCompatible);
	if (lRet >= 0) {
		(void)OAL_RegisterResource(rel_fdt_get_nmatches_by_compatible,
		                           NULL);
	}

	return lRet;
}

static void rel_dispatch_create(void *apData)
{
	(void)dispatch_destroy((dispatch_t *)apData);
}

dispatch_t *OAL_DevDispatchCreate(void)
{
	int32_t lRet;
	dispatch_t *lpRet = dispatch_create();
	if (lpRet != NULL) {
		lRet = OAL_RegisterResource(rel_dispatch_create, lpRet);
		if (lRet != 0) {
			rel_dispatch_create(lpRet);
			lpRet = NULL;
		}
	}

	return lpRet;
}

struct _rel_resmgr_attach {
	dispatch_t *mpDpp;
	int32_t mDppID;
};

static struct _rel_resmgr_attach gsResMgrAttach[OAL_QNX_MAX_RESOURCES];
static OAL_DECLARE_STATIC_POOL(gsResMgrAttachPool,
                               OAL_ARRAY_SIZE(gsResMgrAttach));

static void rel_resmgr_attach(void *apData)
{
	const struct _rel_resmgr_attach *lcpRegMap = apData;
	if (lcpRegMap != NULL) {
		(void)resmgr_detach(lcpRegMap->mpDpp, lcpRegMap->mDppID, 0);
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsResMgrAttachPool,
		                                 gsResMgrAttach, lcpRegMap);
	}
}

int32_t OAL_DevResmgrAttach(dispatch_t *apDpp, resmgr_attr_t *apAttr,
                            const char8_t *acpPath, enum _file_type aFileType,
                            uint32_t aFlags,
                            const resmgr_connect_funcs_t *acpConnectFuncs,
                            const resmgr_io_funcs_t *acpIoFuncs,
                            RESMGR_HANDLE_T *apHandle)
{
	struct _rel_resmgr_attach *lpData = NULL;
	int32_t lStatus;
	int32_t lDppId =
	    resmgr_attach(apDpp, apAttr, acpPath, aFileType, aFlags,
	                  acpConnectFuncs, acpIoFuncs, apHandle);
	if (lDppId == -1) {
		goto dev_res_attach_exit;
	}

	lStatus = OAL_ALLOC_ELEM_FROM_POOL(&gsResMgrAttachPool, gsResMgrAttach,
	                                   &lpData);
	if ((lStatus != 0) || (lpData == NULL)) {
		OAL_LOG_ERROR(
		    "QNX ResMgr map pool is full, please "
		    "adjust its size.\n");
		lStatus = -ENOMEM;
		goto detach_resmgr;
	}

	lpData->mpDpp  = apDpp;
	lpData->mDppID = lDppId;

	lStatus = OAL_RegisterResource(rel_resmgr_attach, lpData);
	if (lStatus != 0) {
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsResMgrAttachPool,
		                                 gsResMgrAttach, lpData);
		lDppId = -1;
	}

detach_resmgr:
	if (lStatus != 0) {
		(void)resmgr_detach(apDpp, lDppId, 0);
		lDppId = -1;
	}

dev_res_attach_exit:
	return lDppId;
}

static void rel_resmgr_context_alloc(void *apData)
{
	resmgr_context_free((resmgr_context_t *)apData);
}

resmgr_context_t *OAL_DevResmgrContextAlloc(dispatch_t *apDpp)
{
	int32_t lRet;
	resmgr_context_t *lpRet = resmgr_context_alloc(apDpp);
	if (lpRet == NULL) {
		goto context_alloc_exit;
	}

	lRet = OAL_RegisterResource(rel_resmgr_context_alloc, lpRet);
	if (lRet != 0) {
		resmgr_context_free(lpRet);
		lpRet = NULL;
	}

context_alloc_exit:
	return lpRet;
}

struct _rel_fdt_match_call {
	const char8_t *const *mcpCompatible;
	fdt_callback_t *mpRemoveCallback;
	void **mpData;
};

static struct _rel_fdt_match_call gsFdtMatchCalls[OAL_QNX_MAX_RESOURCES];
static OAL_DECLARE_STATIC_POOL(gsFdtMatchCallsPool,
                               OAL_ARRAY_SIZE(gsFdtMatchCalls));
static void rel_fdt_match_call(void *apData)
{
	const struct _rel_fdt_match_call *lcpRegMap = apData;
	if (lcpRegMap != NULL) {
		if (lcpRegMap->mpRemoveCallback != NULL) {
			(void)OAL_FdtMatchCall(lcpRegMap->mcpCompatible,
			                       lcpRegMap->mpRemoveCallback,
			                       lcpRegMap->mpData);
			(void)OAL_RELEASE_ELEM_FROM_POOL(
			    &gsFdtMatchCallsPool, gsFdtMatchCalls, lcpRegMap);
		}
	}
}

int32_t OAL_DevFdtMatchCall(const char8_t *const *acpCompatible,
                            fdt_callback_t aProbeCallback,
                            fdt_callback_t aRemoveCallback, void **apData)
{
	struct _rel_fdt_match_call *lpRegMap = NULL;
	int32_t lStatus;
	int32_t lRet = OAL_FdtMatchCall(acpCompatible, aProbeCallback, apData);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to match string in fdt\n");
		goto fdt_match_exit;
	}

	lStatus = OAL_ALLOC_ELEM_FROM_POOL(&gsFdtMatchCallsPool,
	                                   gsFdtMatchCalls, &lpRegMap);
	if ((lStatus != 0) || (lpRegMap == NULL)) {
		OAL_LOG_ERROR(
		    "QNX FDT match pool is full, please "
		    "adjust its size.\n");
		lRet = -ENOMEM;
		goto fdt_match_exit;
	}

	lpRegMap->mcpCompatible    = acpCompatible;
	lpRegMap->mpRemoveCallback = aRemoveCallback;
	lpRegMap->mpData           = apData;

	lStatus = OAL_RegisterResource(rel_fdt_match_call, lpRegMap);
	if (lStatus != 0) {
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsFdtMatchCallsPool,
		                                 gsFdtMatchCalls, lpRegMap);
		lRet = -ENOMEM;
	}

fdt_match_exit:
	return lRet;
}

static void rel_thread_pool_create(void *apData)
{
	thread_pool_t *lpPool = apData;
	if (lpPool != NULL) {
		(void)thread_pool_destroy(lpPool);
	}
}

thread_pool_t *OAL_DevThreadPoolCreate(thread_pool_attr_t *apAttr,
                                       uint32_t aFlags)
{
	thread_pool_t *lpPool;
	int32_t lStatus;

	lpPool = thread_pool_create(apAttr, aFlags);
	if (lpPool == NULL) {
		OAL_LOG_ERROR("Failed to create thread pool\n");
		goto create_pool_exit;
	}

	lStatus = OAL_RegisterResource(rel_thread_pool_create, lpPool);
	if (lStatus != 0) {
		rel_thread_pool_create(lpPool);
		lpPool = NULL;
	}
create_pool_exit:
	return lpPool;
}
