/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_free_mem_tree.h>

#ifndef OAL_MAX_RECURSIVE_CALLS
#define OAL_MAX_RECURSIVE_CALLS 42U
#endif

enum TreeDirection { LEFT, RIGHT };

static int32_t internalInsertTreeNode(struct TreeNode *apNewNode,
                                      uintptr_t aPhysTag, uintptr_t aSize,
                                      struct TreeNode **apRootRef,
                                      uint32_t aMaxDepth);

static int32_t internalDeleteTreeNode(struct TreeNode *apDelNode,
                                      struct TreeNode **apRootRef,
                                      uint32_t aMaxDepth);

static int32_t internalAllocateFromTree(uintptr_t aSize,
                                        struct TreeNode **apRootRef,
                                        void **apStartAddress,
                                        uintptr_t *apPhysTag,
                                        uint32_t aMaxDepth);

static int32_t internalRemoveInterval(uintptr_t aAddr, uintptr_t aSize,
                                      struct TreeNode **apRootRef,
                                      uint32_t aMaxDepth);

int32_t OAL_InsertTreeNode(struct TreeNode *apNewNode, uintptr_t aPhysTag,
                           uintptr_t aSize, struct TreeNode **apRootRef)
{
	return internalInsertTreeNode(apNewNode, aPhysTag, aSize, apRootRef,
	                              OAL_MAX_RECURSIVE_CALLS);
}

int32_t OAL_RemoveIntervalFromTree(uintptr_t aAddr, uintptr_t aSize,
                                   struct TreeNode **apRootRef)
{
	return internalRemoveInterval(aAddr, aSize, apRootRef,
	                              OAL_MAX_RECURSIVE_CALLS);
}

int32_t OAL_DeleteTreeNode(struct TreeNode *apDelNode,
                           struct TreeNode **apRootRef)
{
	return internalDeleteTreeNode(apDelNode, apRootRef,
	                              OAL_MAX_RECURSIVE_CALLS);
}

int32_t OAL_AllocateFromTree(uintptr_t aSize, struct TreeNode **apRootRef,
                             void **apStartAddress, uintptr_t *apPhysTag)
{
	return internalAllocateFromTree(aSize, apRootRef, apStartAddress,
	                                apPhysTag, OAL_MAX_RECURSIVE_CALLS);
}

static int32_t initTreeNode(struct TreeNode *apRoot, uintptr_t aPhysTag,
                            uintptr_t aSize)
{
	int32_t lRet = -1;
	if (apRoot != NULL) {
		(void)memset(apRoot, 0, sizeof(*apRoot));
		apRoot->mPhysTag = aPhysTag;
		apRoot->mSize    = aSize;
		apRoot->mHeight  = 1U;
		apRoot->mMinSize = aSize;
		apRoot->mMaxSize = aSize;
		lRet             = 0;
	}
	return lRet;
}

static void adjustNodeSizes(struct TreeNode *apRoot)
{
	uintptr_t lMinLeft, lMinRight, lMaxLeft, lMaxRight;

	if (apRoot->mpLeft != NULL) {
		lMaxLeft = apRoot->mpLeft->mMaxSize;
		lMinLeft = apRoot->mpLeft->mMinSize;
	} else {
		lMaxLeft = 0U;
		lMinLeft = 0U;
	}

	if (apRoot->mpRight != NULL) {
		lMaxRight = apRoot->mpRight->mMaxSize;
		lMinRight = apRoot->mpRight->mMinSize;
	} else {
		lMaxRight = 0U;
		lMinRight = 0U;
	}

	apRoot->mMaxSize = OAL_MaxUptr(lMaxLeft, lMaxRight);
	apRoot->mMaxSize = OAL_MaxUptr(apRoot->mSize, apRoot->mMaxSize);

	/* Exclude NULL nodes */
	apRoot->mMinSize = apRoot->mSize;
	if (lMinLeft != 0U) {
		apRoot->mMinSize = OAL_MinUptr(apRoot->mMinSize, lMinLeft);
	}
	if (lMinRight != 0U) {
		apRoot->mMinSize = OAL_MinUptr(apRoot->mMinSize, lMinRight);
	}
}

static void getLeftLeaf(struct TreeNode *apRoot, struct TreeNode **apLeaf)
{
	struct TreeNode *lpChild;
	struct TreeNode *lpRoot = apRoot;

	*apLeaf = lpRoot;
	while (lpRoot != NULL) {
		lpChild = lpRoot->mpLeft;
		if (lpChild != NULL) {
			*apLeaf = lpChild;
		}
		lpRoot = lpChild;
	}
}

static inline void adjustNodeHeight(struct TreeNode *apRoot)
{
	uint32_t lLeftHeight = 0U, lRightHeight = 0U;

	if (apRoot->mpLeft != NULL) {
		lLeftHeight = apRoot->mpLeft->mHeight;
	}
	if (apRoot->mpRight != NULL) {
		lRightHeight = apRoot->mpRight->mHeight;
	}
	apRoot->mHeight = 1U + OAL_Max32u(lLeftHeight, lRightHeight);
}

static uintptr_t getNodeAddress(struct TreeNode *apNode)
{
	return (uintptr_t)apNode;
}

static uintptr_t getNodeEndAddress(struct TreeNode *apNode)
{
	return getNodeAddress(apNode) + apNode->mSize;
}

static uint8_t canBeCoalescedAfter(struct TreeNode *apNode, uintptr_t aAddr,
                                   uintptr_t aPhysTag)
{
	uint8_t lRet = 0U;

	if ((getNodeEndAddress(apNode) == aAddr) &&
	    (apNode->mPhysTag == aPhysTag)) {
		lRet = 1U;
	}

	return lRet;
}

static uint8_t canBeCoalescedBefore(struct TreeNode *apNode, uintptr_t aAddr,
                                    uintptr_t aPhysTag, uintptr_t aSize)
{
	uint8_t lRet = 0U;

	if ((getNodeAddress(apNode) == (aAddr + aSize)) &&
	    (apNode->mPhysTag == aPhysTag)) {
		lRet = 1U;
	}

	return lRet;
}

static uint8_t canBeCoalesced(struct TreeNode *apNode, uintptr_t aAddr,
                              uintptr_t aPhysTag, uintptr_t aSize)
{
	uint8_t lBefore = canBeCoalescedBefore(apNode, aAddr, aPhysTag, aSize);
	uint8_t lAfter = canBeCoalescedAfter(apNode, aAddr, aPhysTag);
	return lBefore + lAfter;
}

static int32_t coalesceRightNodes(struct TreeNode **apNode1Ref,
                                  struct TreeNode **apNode2Ref,
                                  uint32_t aMaxDepth)
{
	int32_t lRet = 0;
	struct TreeNode *lpNode1, *lpNode2;

	if (aMaxDepth == 0U) {
		OAL_LOG_ERROR("Reached max allowed depth of the tree\n");
		lRet = OAL_MAX_DEPTH_ERROR;
		goto coalesce_nodes_exit;
	}

	lpNode1 = *apNode1Ref;
	lpNode2 = *apNode2Ref;

	if (canBeCoalescedAfter(lpNode1, (uintptr_t)lpNode2,
	                        lpNode2->mPhysTag) != 0U) {
		lpNode1->mSize += lpNode2->mSize;

		/* Decouple the node from tree */
		lRet =
		    internalDeleteTreeNode(lpNode2, apNode1Ref, aMaxDepth - 1U);
	} else {
		/* The nodes cannot be coalesced */
		lRet = 0;
	}

coalesce_nodes_exit:
	return lRet;
}

static int32_t coalesceRightIntervals(struct TreeNode *apNewNode,
                                      uintptr_t aPhysTag, uintptr_t aSize,
                                      struct TreeNode **apRootRef,
                                      uint32_t aMaxDepth)
{
	int32_t lRet = 0;
	struct TreeNode *lpLeaf;
	struct TreeNode *lpRoot;

	if (aMaxDepth == 0U) {
		OAL_LOG_ERROR("Reached max allowed depth of the tree\n");
		lRet = OAL_MAX_DEPTH_ERROR;
		goto coalesce_intervals_exit;
	}

	lpRoot = *apRootRef;

	if (canBeCoalescedAfter(lpRoot, (uintptr_t)apNewNode, aPhysTag) != 0U) {
		lpRoot->mSize += aSize;

		/* Check if right node can be coalesced */
		if (lpRoot->mpRight != NULL) {
			lRet = coalesceRightNodes(apRootRef, &lpRoot->mpRight,
			                          aMaxDepth - 1U);

			/*
			 * Check if leftmost node of right child can be
			 * coalesced. The leftmost node of the right child will
			 * will be always > lpRoot.
		         */
			if ((lpRoot->mpRight != NULL) &&
			    (lpRoot->mpRight->mpLeft != NULL)) {
				getLeftLeaf(lpRoot->mpRight, &lpLeaf);
				lRet = coalesceRightNodes(apRootRef, &lpLeaf,
				                          aMaxDepth - 1U);
			}
		} else {
			adjustNodeSizes(lpRoot);
			adjustNodeHeight(lpRoot);
		}

	} else if (canBeCoalescedBefore(lpRoot, (uintptr_t)apNewNode, aPhysTag,
	                                aSize) != 0U) {
		lRet =
		    internalDeleteTreeNode(lpRoot, apRootRef, aMaxDepth - 1U);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to delete : %p\n",
			              (void *)lpRoot);
			goto coalesce_intervals_exit;
		}

		lRet = internalInsertTreeNode(apNewNode, aPhysTag,
		                              aSize + lpRoot->mSize, apRootRef,
		                              aMaxDepth - 1U);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to insert node : %p\n",
			              (void *)apNewNode);
		}
	} else {
		lRet = -1;
		OAL_LOG_ERROR("Corrupted tree node ...\n");
	}

coalesce_intervals_exit:
	return lRet;
}

static inline uint8_t checkOverlap(uintptr_t aBegin1, uintptr_t aSize1,
                                   uintptr_t aPhysTag1, struct TreeNode *apNode)
{
	uint8_t lRet      = 0U;
	uintptr_t lBegin2 = (uintptr_t)apNode;
	uintptr_t lEnd2   = lBegin2 + apNode->mSize;
	uintptr_t lEnd1   = aBegin1 + aSize1;

	/* Interval overlap of the same physical allocation */
	if (aPhysTag1 == apNode->mPhysTag) {
		if ((lEnd1 >= lBegin2) && (lEnd2 >= aBegin1)) {
			lRet = 1U;
		}
	} else {
		if ((lEnd1 > lBegin2) && (lEnd2 > aBegin1)) {
			lRet = 1U;
		}
	}

	return lRet;
}

static inline int32_t getNodeBalance(struct TreeNode *apNode)
{
	int32_t lLeftHeight = 0, lRightHeight = 0;
	int32_t lDiff;

	if (apNode->mpLeft != NULL) {
		lLeftHeight = (int32_t)apNode->mpLeft->mHeight;
	}

	if (apNode->mpRight != NULL) {
		lRightHeight = (int32_t)apNode->mpRight->mHeight;
	}

	lDiff = lLeftHeight - lRightHeight;
	return lDiff;
}

static inline void rotateLeftLeft(struct TreeNode **apRootRef)
{
	struct TreeNode *lpRoot;
	struct TreeNode *lpLeft;

	lpRoot = *apRootRef;
	lpLeft = lpRoot->mpLeft;
	if (lpLeft != NULL) {
		lpRoot->mpLeft  = lpLeft->mpRight;
		lpLeft->mpRight = lpRoot;
		*apRootRef      = lpLeft;
	}
}

static inline void rotateLeftRight(struct TreeNode **apRootRef)
{
	struct TreeNode *lpRoot;
	struct TreeNode *lpLeft;
	struct TreeNode *lpLeftRight;

	lpRoot = *apRootRef;
	lpLeft = lpRoot->mpLeft;
	if (lpLeft != NULL) {
		lpLeftRight = lpLeft->mpRight;
		if (lpLeftRight != NULL) {
			lpRoot->mpLeft       = lpLeftRight->mpRight;
			lpLeft->mpRight      = lpLeftRight->mpLeft;
			lpLeftRight->mpLeft  = lpLeft;
			lpLeftRight->mpRight = lpRoot;

			*apRootRef = lpLeftRight;
		}
	}
}

static inline void rotateRightLeft(struct TreeNode **apRootRef)
{
	struct TreeNode *lpRoot;
	struct TreeNode *lpRight;
	struct TreeNode *lpRightLeft;

	lpRoot  = *apRootRef;
	lpRight = lpRoot->mpRight;
	if (lpRight != NULL) {
		lpRightLeft = lpRight->mpLeft;
		if (lpRightLeft != NULL) {
			lpRoot->mpRight      = lpRightLeft->mpLeft;
			lpRight->mpLeft      = lpRightLeft->mpRight;
			lpRightLeft->mpRight = lpRight;
			lpRightLeft->mpLeft  = lpRoot;

			*apRootRef = lpRightLeft;
		}
	}
}

static inline void rotateRightRight(struct TreeNode **apRootRef)
{
	struct TreeNode *lpRoot;
	struct TreeNode *lpRight;

	lpRoot  = *apRootRef;
	lpRight = lpRoot->mpRight;
	if (lpRight != NULL) {
		lpRoot->mpRight = lpRight->mpLeft;
		lpRight->mpLeft = lpRoot;

		*apRootRef = lpRight;
	}
}

static void adjustTreeBalance(struct TreeNode **apRootRef)
{
	struct TreeNode *lpRoot;
	int32_t lRootBalance;

	lpRoot = *apRootRef;

	lRootBalance = getNodeBalance(lpRoot);
	if (lRootBalance >= 2) {
		/* Unbalanced left */
		if (getNodeBalance(lpRoot->mpLeft) <= -1) {
			rotateLeftRight(apRootRef);
		} else {
			rotateLeftLeft(apRootRef);
		}
	}

	if (lRootBalance <= -2) {
		/* Unbalanced right */
		if (getNodeBalance(lpRoot->mpRight) >= 1) {
			rotateRightLeft(apRootRef);
		} else {
			rotateRightRight(apRootRef);
		}
	}

	if ((*apRootRef)->mpRight != NULL) {
		adjustNodeHeight((*apRootRef)->mpRight);
		adjustNodeSizes((*apRootRef)->mpRight);
	}

	if ((*apRootRef)->mpLeft != NULL) {
		adjustNodeHeight((*apRootRef)->mpLeft);
		adjustNodeSizes((*apRootRef)->mpLeft);
	}

	adjustNodeHeight(*apRootRef);
	adjustNodeSizes(*apRootRef);

	return;
}

/**
 * The algorithm is based on pseudocode from: A New Implementation Technique for
 * Memory Management, Mehran Rezaei and Krishna M. Kavi
 */
static int32_t internalInsertTreeNode(struct TreeNode *apNewNode,
                                      uintptr_t aPhysTag, uintptr_t aSize,
                                      struct TreeNode **apRootRef,
                                      uint32_t aMaxDepth)
{
	int32_t lRet         = 0;
	uint8_t *lpNewNodeCP = (uint8_t *)apNewNode;
	uint8_t *lpRootCP;
	struct TreeNode *lpRoot;

	if (apRootRef == NULL) {
		lRet = -1;
		goto insert_exit;
	}

	if (aSize < sizeof(*apNewNode)) {
		OAL_LOG_ERROR("Trying to add a node smaller (0x%" PRIxPTR
		              "  bytes) than "
		              "size of the node (0x%" PRIx64 " bytes).\n",
		              aSize, (uint64_t)sizeof(*apNewNode));
		lRet = -1;
		goto insert_exit;
	}

	if (*apRootRef == NULL) {
		lRet       = initTreeNode(apNewNode, aPhysTag, aSize);
		*apRootRef = apNewNode;
		goto insert_exit;
	}

	if (aMaxDepth == 0U) {
		OAL_LOG_ERROR("Reached max allowed depth of the tree\n");
		lRet = OAL_MAX_DEPTH_ERROR;
		goto insert_exit;
	}

	lpRoot   = *apRootRef;
	lpRootCP = (uint8_t *)lpRoot;

	if (canBeCoalesced(lpRoot, (uintptr_t)apNewNode, aPhysTag, aSize) !=
	    0U) {
		lRet = coalesceRightIntervals(apNewNode, aPhysTag, aSize,
		                              apRootRef, aMaxDepth - 1U);
		goto insert_exit;
	} else {
		if (checkOverlap((uintptr_t)apNewNode, aSize, aPhysTag,
		                 lpRoot) != 0U) {
			OAL_LOG_ERROR("[0x%" PRIxPTR " - 0x%" PRIxPTR
			              "] (tag = 0x%" PRIxPTR
			              ") overlaps with "
			              "node [0x%" PRIxPTR " - 0x%" PRIxPTR
			              "] (tag = 0x%" PRIxPTR ")\n",
			              (uintptr_t)apNewNode,
			              (uintptr_t)apNewNode + aSize, aPhysTag,
			              (uintptr_t)lpRoot,
			              (uintptr_t)lpRoot + lpRoot->mSize,
			              lpRoot->mPhysTag);
			lRet = -1;
			goto insert_exit;
		}
		if (lpNewNodeCP < lpRootCP) {
			lRet = internalInsertTreeNode(apNewNode, aPhysTag,
			                              aSize, &lpRoot->mpLeft,
			                              aMaxDepth - 1U);
		} else {
			lRet = internalInsertTreeNode(apNewNode, aPhysTag,
			                              aSize, &lpRoot->mpRight,
			                              aMaxDepth - 1U);
		}
	}

	if (lRet == 0) {
		adjustTreeBalance(apRootRef);
		adjustNodeSizes(*apRootRef);
	}
insert_exit:
	return lRet;
}

static int32_t internalDeleteLeafNode(struct TreeNode **apRootRef,
                                      uint32_t aMaxDepth)
{
	int32_t lRet = -1;
	struct TreeNode *lpRoot;
	struct TreeNode *lpLeaf = NULL;

	if (aMaxDepth == 0U) {
		OAL_LOG_ERROR("Reached max allowed depth of the tree\n");
		lRet = OAL_MAX_DEPTH_ERROR;
		goto delete_tree_node_exit;
	}

	lpRoot = *apRootRef;

	if ((lpRoot->mpLeft == NULL) || (lpRoot->mpRight == NULL)) {
		if (lpRoot->mpLeft != NULL) {
			*apRootRef = lpRoot->mpLeft;
		} else if (lpRoot->mpRight != NULL) {
			*apRootRef = lpRoot->mpRight;
		} else {
			*apRootRef = NULL;
		}
		lRet = 0;
	} else {
		getLeftLeaf(lpRoot->mpRight, &lpLeaf);

		lRet = internalDeleteTreeNode(lpLeaf, &(*apRootRef)->mpRight,
		                              aMaxDepth - 1U);
		if (lRet != 0) {
			goto delete_tree_node_exit;
		}

		lpLeaf->mpRight = (*apRootRef)->mpRight;
		lpLeaf->mpLeft  = (*apRootRef)->mpLeft;
		*apRootRef      = lpLeaf;
		lRet            = 0;
	}

delete_tree_node_exit:
	return lRet;
}

static int32_t internalDeleteTreeNode(struct TreeNode *apDelNode,
                                      struct TreeNode **apRootRef,
                                      uint32_t aMaxDepth)
{
	int32_t lRet = -1;
	struct TreeNode *lpRoot;

	if (apRootRef == NULL) {
		goto delete_tree_node_exit;
	}

	if (aMaxDepth == 0U) {
		OAL_LOG_ERROR("Reached max allowed depth of the tree\n");
		lRet = OAL_MAX_DEPTH_ERROR;
		goto delete_tree_node_exit;
	}

	lpRoot = *apRootRef;

	if ((lpRoot->mpLeft != NULL) && (lpRoot > apDelNode)) {
		lRet = internalDeleteTreeNode(apDelNode, &lpRoot->mpLeft,
		                              aMaxDepth - 1U);
	} else if ((lpRoot->mpRight != NULL) && (lpRoot < apDelNode)) {
		lRet = internalDeleteTreeNode(apDelNode, &lpRoot->mpRight,
		                              aMaxDepth - 1U);
	} else {
		lRet = internalDeleteLeafNode(apRootRef, aMaxDepth - 1U);
	}

	if ((*apRootRef != NULL) && (lRet == 0)) {
		adjustTreeBalance(apRootRef);
		adjustNodeSizes(*apRootRef);
	}

delete_tree_node_exit:
	return lRet;
}

static uint8_t isContainedByNode(uintptr_t aAddr, uintptr_t aSize,
                                 struct TreeNode *apNode)
{
	uint8_t lRet       = 0U;
	uintptr_t lNode    = (uintptr_t)apNode;
	uintptr_t lNodeEnd = lNode + apNode->mSize;
	uintptr_t lAddrEnd = aAddr + aSize;

	if ((lNode <= aAddr) && (lNodeEnd >= lAddrEnd)) {
		lRet = 1U;
	}

	return lRet;
}

static void getNodeContaining(uintptr_t aAddr, uintptr_t aSize,
                              struct TreeNode *apRootRef,
                              struct TreeNode **apMatchedNode)
{
	struct TreeNode *lpNode = apRootRef;
	*apMatchedNode          = NULL;

	while (lpNode != NULL) {
		if (isContainedByNode(aAddr, aSize, lpNode) != 0U) {
			*apMatchedNode = lpNode;
			break;
		}

		if (aAddr < ((uintptr_t)lpNode)) {
			lpNode = lpNode->mpLeft;
			continue;
		}

		if (aAddr > ((uintptr_t)lpNode)) {
			lpNode = lpNode->mpRight;
			continue;
		}

		OAL_LOG_ERROR("The interval (0x%" PRIxPTR " - 0x%" PRIxPTR
		              ") doesn't match perfectly the node (0x%" PRIxPTR
		              " - 0x%" PRIxPTR ")\n",
		              aAddr, aSize, (uintptr_t)lpNode, lpNode->mSize);

		break;
	}
}

static int32_t internalRemoveInterval(uintptr_t aAddr, uintptr_t aSize,
                                      struct TreeNode **apRootRef,
                                      uint32_t aMaxDepth)
{
	int32_t lRet                   = -1;
	struct TreeNode *lpMatchedNode = NULL;
	struct TreeNode *lpNewNode;
	uintptr_t lAddr, lSize, lEnd, lIntervalEnd;

	if (apRootRef == NULL) {
		goto remove_interval_exit;
	}

	/* Identify the node */
	getNodeContaining(aAddr, aSize, *apRootRef, &lpMatchedNode);
	if (lpMatchedNode == NULL) {
		OAL_LOG_ERROR("The node (0x%" PRIxPTR " - 0x%" PRIxPTR
		              ") "
		              "isn't part of the tree\n",
		              aAddr, aAddr + aSize);
		lRet = -EINVAL;
		goto remove_interval_exit;
	}

	lAddr = (uintptr_t)lpMatchedNode;
	lSize = lpMatchedNode->mSize;

	/* Remove node from tree prior to any other operations */
	lRet = internalDeleteTreeNode(lpMatchedNode, apRootRef, aMaxDepth - 1U);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to delete node\n");
		goto remove_interval_exit;
	}

	/* Same start address */
	if (lAddr == aAddr) {
		/* Remove only first part of the node */
		if (lSize != aSize) {
			lpNewNode = (struct TreeNode *)(lAddr + aSize);
			lRet      = internalInsertTreeNode(
                            lpNewNode, lpMatchedNode->mPhysTag, lSize - aSize,
                            apRootRef, aMaxDepth - 1U);
			if (lRet != 0) {
				OAL_LOG_ERROR("Failed to add a new node\n");
				goto restore_node;
			}
		}
	}

	lEnd         = lAddr + lSize;
	lIntervalEnd = aAddr + aSize;

	/* Remove middle or end of the node */
	if (lAddr < aAddr) {
		lpNewNode = (struct TreeNode *)lAddr;
		lRet      = internalInsertTreeNode(
                    lpNewNode, lpMatchedNode->mPhysTag, aAddr - lAddr,
                    apRootRef, aMaxDepth - 1U);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to add a new node\n");
			goto restore_node;
		}

		/* Middle cut */
		if (lEnd > lIntervalEnd) {
			lpNewNode = (struct TreeNode *)lIntervalEnd;
			lRet      = internalInsertTreeNode(
                            lpNewNode, lpMatchedNode->mPhysTag,
                            lEnd - lIntervalEnd, apRootRef, aMaxDepth - 1U);
			if (lRet != 0) {
				OAL_LOG_ERROR("Failed to add a new node\n");
				goto restore_node;
			}
		}
	}

restore_node:
	if (lRet != 0) {
		(void)internalInsertTreeNode((struct TreeNode *)lAddr,
		                             lpMatchedNode->mPhysTag, lSize,
		                             apRootRef, aMaxDepth - 1U);
	}

remove_interval_exit:
	return lRet;
}

static int32_t internalAllocateFromTree(uintptr_t aSize,
                                        struct TreeNode **apRootRef,
                                        void **apStartAddress,
                                        uintptr_t *apPhysTag,
                                        uint32_t aMaxDepth)
{
	int32_t lRet = -1;
	struct TreeNode *lpRoot;
	uint8_t *lpRootCP;

	if ((apRootRef == NULL) || (*apRootRef == NULL) ||
	    (apStartAddress == NULL) || (apPhysTag == NULL)) {
		goto allocate_exit;
	}

	if (aMaxDepth == 0U) {
		OAL_LOG_ERROR("Reached max allowed depth of the tree\n");
		lRet = OAL_MAX_DEPTH_ERROR;
		goto allocate_exit;
	}

	if (aSize < sizeof(**apRootRef)) {
		OAL_LOG_ERROR("Cannot allocate area less than 0x%" PRIx64
		              " bytes (tree node size)\n",
		              (uint64_t)sizeof(**apRootRef));
		goto allocate_exit;
	}

	lpRoot   = *apRootRef;
	lpRootCP = (uint8_t *)lpRoot;
	if (lpRoot->mMaxSize < aSize) {
		goto allocate_exit;
	}

	if (lpRoot->mSize == aSize) {
		*apStartAddress = lpRoot;
		*apPhysTag      = lpRoot->mPhysTag;
		lRet =
		    internalDeleteTreeNode(lpRoot, apRootRef, aMaxDepth - 1U);
		goto allocate_exit;
	}

	if ((lpRoot->mpLeft != NULL) || (lpRoot->mpRight != NULL)) {
		if (lpRoot->mpLeft != NULL) {
			lRet = internalAllocateFromTree(
			    aSize, &lpRoot->mpLeft, apStartAddress, apPhysTag,
			    aMaxDepth - 1U);
		}

		/* Failed to allocate from left child */
		if ((lRet != 0) && (lpRoot->mpRight != NULL)) {
			lRet = internalAllocateFromTree(
			    aSize, &lpRoot->mpRight, apStartAddress, apPhysTag,
			    aMaxDepth - 1U);
		}
	}

	/* Failed to allocate from children */
	if (lRet != 0) {
		if (lpRoot->mSize >= (aSize + sizeof(*lpRoot))) {
			/* Split the node */
			lpRoot->mSize -= aSize;
			*apStartAddress = lpRootCP + lpRoot->mSize;
			*apPhysTag      = lpRoot->mPhysTag;
			lRet            = 0;
		}
	}

	if (lRet == 0) {
		adjustNodeSizes(lpRoot);
	}

allocate_exit:
	return lRet;
}

#ifndef OAL_LOG_SUPPRESS_DEBUG
static void priv_dumpTree(struct TreeNode *apRoot, uint32_t aLevel)
{
	uint32_t lIndex = 0U;
	for (lIndex = 0U; lIndex < aLevel; lIndex++) {
		(void)OAL_PRINT("%s",
		                (lIndex == (aLevel - 1U)) ? "   " : "   |");
	}
	if (apRoot != NULL) {
		(void)OAL_PRINT(
		    "+- %p - %p (h%" PRIu32 ", s0x%" PRIxPTR ", p0x%" PRIxPTR
		    " [0x%" PRIxPTR " - 0x%" PRIxPTR "])\n",
		    (uint8_t *)apRoot, (uint8_t *)apRoot + apRoot->mSize,
		    apRoot->mHeight, apRoot->mSize, apRoot->mPhysTag,
		    apRoot->mMinSize, apRoot->mMaxSize);
		priv_dumpTree(apRoot->mpLeft, aLevel + 1U);
		priv_dumpTree(apRoot->mpRight, aLevel + 1U);
	} else {
		(void)OAL_PRINT("+- %p\n", NULL);
	}
}

void OAL_DumpTree(struct TreeNode *apRoot) { priv_dumpTree(apRoot, 0U); }
#endif
