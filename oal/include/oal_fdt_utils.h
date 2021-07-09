/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_FDT_UTILS_H
#define OAL_FDT_UTILS_H

#include <oal_utils.h>

__BEGIN_DECLS

/**
 * @defgroup OAL_Device_Tree_Utilities  OAL Device Tree
 *
 * @{
 * @brief OAL Device Tree Utilities
 * @details
 * Parses device tree blobs in order to provide info about:
 *   * node names
 *   * node properties
 *   * node offsets for corresponding phandle properties
 *   * node offsets for corresponding compatible properties
 */

/** The requested node or property does not exist */
#define OAL_FDT_OFFSET_ERR -1

/** Not a valid device tree, missing magic number */
#define OAL_FDT_MAGIC_ERROR -2

/** Structure block offset is not valid, begins without OAL_FDT_BEGIN_NODE tag
 */
#define OAL_FDT_BAD_STRUCT_OFF -3

/** Strings block offset is not valid */
#define OAL_FDT_BAD_STRINGS_OFF -4

/** OAL_FDT_NULL_STRINGS_OFF : Strings block address is NULL */
#define OAL_FDT_NULL_STRINGS_OFF -5

/** OAL_FDT_BAD_STRUCT_LEN : Structure block length is 0 */
#define OAL_FDT_BAD_STRUCT_LEN -6

/** Structure block adress is NULL */
#define OAL_FDT_NULL_STRUCT_OFF -7

/** Device Tree property could not be found */
#define OAL_FDT_PROP_OFFSET_ERROR -8

/** struct DtbPropOff could not be set */
#define OAL_FDT_PROP_OFF_ERR -9

/** Phandle property could not be found */
#define OAL_FDT_PHANDLE_OFFSET_ERROR -10

/** Node name could not be found */
#define OAL_FDT_NAME_ERROR -11

/** compatible' property could not be found */
#define OAL_FDT_COMPATIBLE_OFFSET_ERROR -12

/** Structure, Strings blocks and Structure Block length could not be set */
#define OAL_FDT_BLOCKS_ERROR -13

/** struct DtbHeader could not be set */
#define OAL_FDT_BAD_HEADER -14

/** Device tree blob adress is NULL */
#define OAL_FDT_NULL_ERROR -15

/** Starting node offset is out of bounds */
#define OAL_FDT_BAD_START_OFF -16

/** OAL_FDT_END_NODE tag was reached while searching for device tree
 * properties
 */
#define OAL_FDT_END_NODE_OFFSET -17

/** Could not find next node offset */
#define OAL_FDT_NEXT_NODE_NOT_FOUND -19

/**
 * @brief      Retrieve node name at given node offset
 *
 *
 * @param[in]  acpDtb Offset to device tree blob memory location
 * @param[in]  aNodeOffset Struct block offset (node offset)
 * @param[in]  apLenN Length of found name, if \p apLenN not <tt>NULL</tt>
 *
 * @param[out] apName Node name
 *
 * @return
 *        * 0, if successful
 *        * \ref OAL_FDT_NAME_ERROR, if no name is found.
 *        * \ref OAL_FDT_BLOCKS_ERROR
 */
int32_t OAL_GetName(const uint32_t *acpDtb, uint32_t aNodeOffset,
                    uint32_t *apLenN, char8_t **apName);

/**
 * @brief      Retrieve a pointer to the property value at given
 *			  node offset and at given property name.
 *
 *
 * @param[in]  acpDtb Offset to device tree blob memory location
 * @param[in]  aNodeOff Node offset whose property value is needed
 * @param[in]  acpPropName Property name whose property value is needed
 * @param[in]  apLenPVal Length of found proprty value, if \p apLenPVal not
 *                       <tt>NULL</tt>
 *
 * @param[out] apPropValAddr Pointer to property value. If property contains no
 *                           value, (e.g: init;), \p apPropValAddr is
 *                           <tt>NULL</tt>
 *
 * @return
 *         * 0, if successful
 *         * \ref OAL_FDT_PROP_OFFSET_ERROR, if no matched property name is
 *         found
 *         * \ref OAL_FDT_BLOCKS_ERROR
 *         * \ref OAL_FDT_PROP_OFF_ERR
 *         * \ref OAL_FDT_END_NODE_OFFSET
 */
int32_t OAL_GetProp(const uint32_t *acpDtb, uint32_t aNodeOff,
                    const char8_t *acpPropName, uint32_t *apLenPVal,
                    uint8_t **apPropValAddr);

/**
 *@brief      Retrieve node offset for given phandle property value
 *
 *
 *@param[in]  acpDtb  Offset to device tree blob memory location
 *@param[in]  aPhandle  Phandle property value
 *
 *@param[out] apNodeOffset  Node offset found
 *
 *@return
 *         * 0, if successful
 *         * \ref OAL_FDT_PHANDLE_OFFSET_ERROR, if no matched phandle value is
 *           found
 *         * \ref OAL_FDT_NULL_ERROR
 *         * \ref OAL_FDT_BLOCKS_ERROR
 */
int32_t OAL_NodeOffsetByPhandle(const uint32_t *acpDtb, uint32_t aPhandle,
                                uint32_t *apNodeOffset);

/**
 * @brief     Retrieve node offset for given 'compatible' property value
 *
 *
 * @param[in]  acpDtb  Offset to device tree blob memory location
 * @param[in]  aStartOff  Starting node offset
 *                        * \ref OAL_FDT_OFFSET_ERR - search includes root node
 *                        * <tt>0</tt>  - search does not include root node
 * @param[in]  acpCompatible  Compatible property value string
 *
 * @param[out] apNodeOffset Node offset found
 *
 * @return
 *         * 0, if successful
 *         * \ref OAL_FDT_COMPATIBLE_OFFSET_ERROR, if given 'compatible'
 *           property not found
 *         * \ref OAL_FDT_NULL_ERROR
 *         * \ref OAL_FDT_BLOCKS_ERROR
 *         * \ref OAL_FDT_BAD_START_OFF
 *         * \ref OAL_FDT_NEXT_NODE_NOT_FOUND
 *         * \ref OAL_FDT_COMPATIBLE_OFFSET_ERROR
 */
int32_t OAL_NodeOffsetByCompatible(const uint32_t *acpDtb, uint32_t aStartOff,
                                   const char8_t *acpCompatible,
                                   uint32_t *apNodeOffset);

__END_DECLS

/** @} */

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_fdt_utils.h>
#endif
#endif /* OAL_FDT_UTILS_H */
