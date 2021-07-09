/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_FREE_MEM_TREE
#define OAL_FREE_MEM_TREE

#include <oal_utils.h>

__BEGIN_DECLS

#define OAL_MAX_DEPTH_ERROR (-5)

/**
 * @brief Utility to cast any address to a <tt>struct TreeNode</tt>
 *
 * @param[in] ADDR     The address
 * @param[in] OFFSET   The offset added to the <tt>ADDR</tt>
 *
 * @return A pointer to the resulted <tt>struct TreeNode</tt>
 */
#define OAL_GET_TREE_NODE(ADDR, OFFSET)                                        \
	OAL_COMP_EXTENSION({                                                   \
		uintptr_t lTreeNodeAddr = ((uintptr_t)(ADDR));                 \
		lTreeNodeAddr += (OFFSET);                                     \
		(struct TreeNode *)(lTreeNodeAddr);                            \
	})

/**
 * Availability Tree
 *
 * The availability tree is used to keep the track of all free memory regions.
 * It is very similar to an classic AVL tree with a few exceptions:
 *    * Each node represents a free memory area.
 *@code
 *      struct TreeNode *lNode = 0x8000;
 *      lNode->mSize = 0x200;
 *@endcode
 *      describes a node that starts at <tt>0x8000</tt> and has <tt>0x200</tt>
 *      bytes
 *    * The node value is represented by node address (<tt>0x800</tt> in above
 *      example).
 *    * Two nodes are coalesced when forming a contiguous memory area
 *      @verbatim [0x800, 0x900), [0x900, 0xa000) -> [0x800, 0xa000)
 *@endverbatim
 *      This operation might occur after an insert or a delete.
 *    * Since the tree is used to keep the track of free memory areas it has to
 *      be preallocated prior to tree insertion
 */

/**
 * @brief Availability Tree node
 */
struct TreeNode {
	uintptr_t mPhysTag; /**< Physical range cookie */
	uintptr_t mSize;    /**< Node length (bytes)*/
	uintptr_t mMinSize; /**< The smallest amount that can be allocated */
	uintptr_t mMaxSize; /**< The biggest free area contained by the tree */
	struct TreeNode *mpLeft;  /**< Left child (AVL specific)*/
	struct TreeNode *mpRight; /**< Right child (AVL specific) */
	uint32_t mHeight;         /**< Node height (AVL specific) */
};

/**
 * @brief Add a free area to the tree
 *
 * @param[in]  apNewNode The address of the free region
 * @param[in]  aPhysTag  Physical tag address associated to \a apNewNode
 * @param[in]  aSize     The size of the region
 * @param[in,out] apRootRef The reference to the root node of the tree
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_InsertTreeNode(struct TreeNode *apNewNode, uintptr_t aPhysTag,
                           uintptr_t aSize, struct TreeNode **apRootRef);

/**
 * @brief Removes an interval from a tree
 *
 * @param[in] aAddr Start address of the interval
 * @param[in] aSize Size of the interval
 * @param[in,out] apRootRef The reference to the root node of the tree
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_RemoveIntervalFromTree(uintptr_t aAddr, uintptr_t aSize,
                                   struct TreeNode **apRootRef);

/**
 * @brief Delete a node from free.
 *
 * <tt>apDelNode</tt> must be part of the tree, otherwise the operation will
 * fail.
 *
 * @param[in] apDelNode  The address of the node to be deleted
 * @param[in,out] apRootRef The reference to the root node of the tree
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_DeleteTreeNode(struct TreeNode *apDelNode,
                           struct TreeNode **apRootRef);

/**
 * @brief Request a free area from the tree
 *
 * @param[in] aSize           The size of the requested area
 * @param[in,out] apRootRef   The reference to the root node of the tree
 * @param[out] apStartAddress Start address of the allocated area
 * @param[out] apPhysTag      Physical address of the allocated area
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_AllocateFromTree(uintptr_t aSize, struct TreeNode **apRootRef,
                             void **apStartAddress, uintptr_t *apPhysTag);

#ifndef OAL_LOG_SUPPRESS_DEBUG
/**
 * @brief Dumps the tree at <tt>stdout</tt>
 *
 * @param[in] apRoot The root node of the tree
 */
void OAL_DumpTree(struct TreeNode *apRoot);
#endif

__END_DECLS

#endif
