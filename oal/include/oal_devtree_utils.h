/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_DEVTREE_UTILS_H
#define OAL_DEVTREE_UTILS_H

#include <oal_log.h>
#include <oal_utils.h>
#include <oal_byteorder.h>
#include <oal_fdt_utils.h>

// FDT properties utils
#define OAL_PROP_SKIP_N_VALUES(P, N, TYPE) ((P) += (sizeof((TYPE)0) * (N)))

#define OAL_PROP_GET_VAL(P, TYPE) (*(__typeof__((TYPE)) *)(uintptr_t)(P))
#define OAL_PROP_CONSUME_VAL(P, TYPE)                                          \
	do {                                                                   \
		uintptr_t lUPtrTemp = (uintptr_t)(P);                          \
		lUPtrTemp += sizeof((TYPE));                                   \
		(P) = (__typeof__((P)))lUPtrTemp;                              \
	} while (1 == 0)

#define OAL_PROP_GET_NEXT_VALUE(DEST, PTR, TYPE, F)                            \
	do {                                                                   \
		(DEST) =                                                       \
		    (__typeof__(DEST))(F)(OAL_PROP_GET_VAL((PTR), (TYPE)));    \
		OAL_PROP_CONSUME_VAL((PTR), TYPE);                             \
	} while (1 == 0)

#define OAL_PROP_GET_NEXT_UINT8(DEST, PTR)                                     \
	OAL_PROP_GET_NEXT_VALUE((DEST), (PTR), ((uint8_t)0), (uint8_t))
#define OAL_PROP_GET_NEXT_UINT16(DEST, PTR)                                    \
	OAL_PROP_GET_NEXT_VALUE((DEST), (PTR), ((uint16_t)0),                  \
	                        OAL_GetBigEndian16)
#define OAL_PROP_GET_NEXT_UINT32(DEST, PTR)                                    \
	OAL_PROP_GET_NEXT_VALUE((DEST), (PTR), ((uint32_t)0),                  \
	                        OAL_GetBigEndian32)
#define OAL_PROP_GET_NEXT_UINT64(DEST, PTR)                                    \
	OAL_PROP_GET_NEXT_VALUE((DEST), (PTR), ((uint64_t)0),                  \
	                        OAL_GetBigEndian64)

#define OAL_REG_PROPERTY "reg"

__BEGIN_DECLS

/**
 * @brief Describes a device tree node
 */
struct fdt_node {
	uint32_t mOffset; /**< Node offset */
	char8_t *mpName;  /**< Node name */
};

/**
 * @brief OAL_FdtNodeByCompatible Identify a node in device tree
 * using its compatible string
 *
 * @param[in]  acpCompatible Compatible string of the node
 * @param[out] apNode        Reference to the node
 *
 * @return 0 on success, a negative value otherwise
 */
int32_t OAL_FdtNodeByCompatible(const char8_t *acpCompatible,
                                struct fdt_node *apNode);

/**
 * @brief OAL_FdtGetNmatchesByCompatible Get number of matches based
 * on provided acpCompatible string
 *
 * @param[in] acpCompatible The acpCompatible string
 *
 * @return Number of matches on success, a negative value otherwise
 */
int32_t OAL_FdtGetNmatchesByCompatible(const char8_t *const *acpCompatible);

/**
 * @brief Callback to be passed to OAL_FdtMatchCall
 *
 * @param[in] apNode  FDT node
 * @param[out] apcData Data associated to <tt>apNode</tt>
 *
 * @return 0 on success, a negative value otherwise
 */
typedef int32_t(fdt_callback_t)(const struct fdt_node *apNode,
                                const void **apcData);

/**
 * @brief  Call the provided callback on each apNode that  matches one of the
 * <tt>acpCompatible</tt> strings
 *
 * @param[in] acpCompatible An array of acpCompatible strings ending with NULL
 * @param[in] aCallback     The callback to be called
 * @param[in] apFdata       An array of apcData pointers. The size of the array
 *                          must be equal with the number of matches.
 *
 * @return 0 on success, a negative value otherwise
 */
int32_t OAL_FdtMatchCall(const char8_t *const *acpCompatible,
                         fdt_callback_t aCallback, void **apFdata);

/**
 * @brief Retrieves the address of Device Tree.
 *
 * @param[out] apFdtAddr The address of the device tree
 *
 * @return 0 on success, a negative value otherwise
 */
int32_t OAL_GetFdtAddress(uintptr_t *apFdtAddr);

/**
 * @brief Resolve the phandle referenced by a property to a device tree apNode.
 *
 * @param[in]  acpSnode     Node containing the phandle
 * @param[out] apDestNode   The apNode referenced by phandle
 * @param[in]  acpPropName  Name of the property that references
 *                          <tt>apDestNode</tt>
 *
 * @return 0 on success, a negative value otherwise
 */
int32_t OAL_FdtParsePhandle(const struct fdt_node *acpSnode,
                            struct fdt_node *apDestNode,
                            const char8_t *acpPropName);

/**
 * @brief Read data from "reg" property
 *
 * @param[in]  acpNode   A device tree apNode
 * @param[in]  aIndex    The index of the area within the apNode
 * @param[out] apRegBase The base address of the registers area
 * @param[out] apLen     The length of the registers area area
 *
 * @return 0 on success, a negative value otherwise
 */
int32_t OAL_FdtGetReg(const struct fdt_node *acpNode, int32_t aIndex,
                      uintptr_t *apRegBase, uintptr_t *apLen);

/**
 * @brief OAL_FdtGetNodeProperty Gets the pointer to a apNode property
 *
 * @param[in] acpNode        A device tree apNode
 * @param[in] acpPropName   Property mcpName
 * @param[in] acpPropValue  Pointer to property's value
 *
 * @return 0 on success, a negative value otherwise
 */
int32_t OAL_FdtGetNodeProperty(const struct fdt_node *acpNode,
                               const char8_t *acpPropName,
                               const char8_t **acpPropValue);

__END_DECLS

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_devtree_utils.h>
#endif
#endif
