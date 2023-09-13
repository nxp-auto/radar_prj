/*
 * Copyright 2018-2019, 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <oal_byteorder.h>
#include <oal_fdt_utils.h>

/* OAL_FDT big endian values */
#define OAL_FDT_MAGIC 0xd00dfeedU
#define OAL_FDT_BEGIN_NODE 0x1U
#define OAL_FDT_END_NODE 0x2U
#define OAL_FDT_END 0x9U
#define OAL_FDT_PROP 0x3U
#define OAL_FDT_NOP 0x4U

/* OAL_FDT header specifics */
#define OAL_FDT_SUPPORTED_VER 17U

#define COMPATIBLE_PROP "compatible"
#define PHANDLE_PROP "phandle"
#define LINUX_PHANDLE_PROP "linux,phandle"

#define MAX_PROP_NAME_LEN ((size_t)30U)

struct DtbHeader {
	uint32_t mMagicNumber; /**< dtb header start */
	uint32_t mTotalSize;   /**< dtb total size */
	uint32_t mOffStruct;   /**< offset to structure block section */
	uint32_t mOffStrs;     /**< offset to strings block section */

	uint32_t mVersion; /**< current version */

	uint32_t mSizeStruct; /**< version >= 3 */
	uint32_t mSizeStrs;   /**< version >= 3 */
};

struct DtbPropOff {
	uint32_t mPropValLen;           /**< node property value length */
	const char8_t *mcpPropName;     /**< node property name */
	const char8_t *mcpPropValBlock; /**< node property value */
};

static int32_t checkDtbSanity(const uint32_t *acpDtb)
{
	int32_t lRet;

	if (acpDtb == NULL) {
		lRet = OAL_FDT_NULL_ERROR;
	} else if (OAL_GetBigEndian32(*acpDtb) != OAL_FDT_MAGIC) {
		lRet = OAL_FDT_MAGIC_ERROR;
	} else {
		lRet = 0;
	}

	return lRet;
}

static int32_t getDtbHeader(const uint32_t *acpDtb, struct DtbHeader *apDtbH)
{
	int32_t lRet = OAL_FDT_BAD_HEADER;
	const uint32_t *lcpStructEnd, *lcpStructEndNode, *lcpStructRsvMap;

	/* the device tree blob header format is the following
	(all values are 32-bit padded, big endian):

	   Offset | Contents
	   -------------------------------------------
	     0x00 | magic number (0xd00dfeed)
	     0x04 | totalsize
	     0x08 | offset to struct block
	     0x0C | offset to strings block
	     0x10 | offset to memory reserve map block
	     0x14 | version
	     0x18 | last compatible version
	     0x1C | boot cpu id
	     0x20 | strings block size >= version 3
	     0x24 | struct block size >= version 3
	     0x28 | mem rsv number
	  --------------------------------------------
	*/

	apDtbH->mMagicNumber = OAL_GetBigEndian32(acpDtb[0]);
	apDtbH->mTotalSize   = OAL_GetBigEndian32(acpDtb[1]);
	apDtbH->mOffStruct   = OAL_GetBigEndian32(acpDtb[2]);
	apDtbH->mOffStrs     = OAL_GetBigEndian32(acpDtb[3]);
	apDtbH->mVersion     = OAL_GetBigEndian32(acpDtb[5]);
	apDtbH->mSizeStrs    = OAL_GetBigEndian32(acpDtb[8]);
	apDtbH->mSizeStruct  = OAL_GetBigEndian32(acpDtb[9]);

	/* sanity checks */
	if (apDtbH->mVersion != OAL_FDT_SUPPORTED_VER) {
		OAL_LOG_WARNING(
		    "Device Tree version is different than the one"
		    "supported, please check there are no incompatibilities\n");
		goto ret_getheader;
	}

	/* end marker of memory reserve table must be 0 */
	lcpStructRsvMap =
	    acpDtb + (apDtbH->mOffStruct / ((uint32_t)sizeof(uint32_t))) - 1U;
	if (OAL_GetBigEndian32(*lcpStructRsvMap) != 0U) {
		OAL_LOG_ERROR("Failed to detect end marker\n");
		goto ret_getheader;
	}

	/* check end node and end tokens in structure block */
	/* OAL_FDT_END token must be the last token in the structure block */
	lcpStructEnd = acpDtb +
	               ((apDtbH->mOffStruct + apDtbH->mSizeStruct) /
	                ((uint32_t)sizeof(uint32_t))) -
	               1U;
	lcpStructEndNode = acpDtb +
	                   ((apDtbH->mOffStruct + apDtbH->mSizeStruct) /
	                    ((uint32_t)sizeof(uint32_t))) -
	                   2U;
	if ((OAL_GetBigEndian32(*lcpStructEnd) != OAL_FDT_END) ||
	    (OAL_GetBigEndian32(*lcpStructEndNode) != OAL_FDT_END_NODE)) {
		OAL_LOG_ERROR(
		    "Failed to detect OAL_FDT_END and "
		    "OAL_FDT_END_NODE markers\n");
		goto ret_getheader;
	}

	/* strings block must come after DT structure block*/
	if ((apDtbH->mOffStruct + apDtbH->mSizeStruct) != apDtbH->mOffStrs) {
		OAL_LOG_ERROR(
		    "Begin of the strings block doesn't match with "
		    "the end of DT structure\n");
		goto ret_getheader;
	}

	/*
	 * strings block must be the last block
	 * (except the case when U-Boot adds padding)
	 */
	if ((apDtbH->mOffStrs + apDtbH->mSizeStrs) > apDtbH->mTotalSize) {
		OAL_LOG_ERROR("Total size field is corrupted\n");
		goto ret_getheader;
	}

	lRet = 0;

ret_getheader:
	return lRet;
}

static void getDtbPropOff(const uint32_t *acpNodeAddr,
                          struct DtbPropOff *apDtbP,
                          const uint32_t *acpStrsAddr)
{
	/* each property has the following format:

	   -------------------------------------------------
	    property value block length (32 bit number)
	    property name (32 bit offset to Strings block)
	    property value block
	   -------------------------------------------------
	*/

	uint32_t lStructPropNameOff;

	apDtbP->mPropValLen = OAL_GetBigEndian32(acpNodeAddr[1]);

	lStructPropNameOff = OAL_GetBigEndian32(acpNodeAddr[2]);
	apDtbP->mcpPropName =
	    (const char8_t *)(acpStrsAddr) + lStructPropNameOff;

	apDtbP->mcpPropValBlock = (const char8_t *)&acpNodeAddr[3];
}

static int32_t getDtbStructOffset(struct DtbHeader *apDtbH,
                                  const uint32_t *acpDtb,
                                  const uint32_t **acpStructOff)
{
	int32_t lRet = 0;

	*acpStructOff = acpDtb + ((apDtbH->mOffStruct) / sizeof(uint32_t));

	if (OAL_GetBigEndian32(**acpStructOff) != OAL_FDT_BEGIN_NODE) {
		lRet = OAL_FDT_BAD_STRUCT_OFF;
	} else {
		lRet = 0;
	}

	return lRet;
}

static void getDtbStrsOffset(struct DtbHeader *apDtbH, const uint32_t *acpDtb,
                             const uint32_t **acpStrsOffset)
{
	*acpStrsOffset = (acpDtb + ((apDtbH->mOffStrs) / sizeof(uint32_t)));
}

static int32_t getStructLen(struct DtbHeader *apDtbH, uint32_t *apStructLen)
{
	int32_t lRet = 0;

	if ((apDtbH->mSizeStruct) == 0U) {
		lRet = OAL_FDT_BAD_STRUCT_LEN;
	}

	*apStructLen = (apDtbH->mSizeStruct) / ((uint32_t)sizeof(uint32_t));

	return lRet;
}

static int32_t setDtbBlocks(const uint32_t *acpDtb,
                            const uint32_t **acpStructOff,
                            const uint32_t **acpStrsOff, uint32_t *apStructLen)
{
	struct DtbHeader lDtbH;
	int32_t lRetStructLen, lRetStructOff, lRetGetHeader;
	int32_t lRetMagic, lRet;

	lRet = 0;

	lRetMagic = checkDtbSanity(acpDtb);
	if (lRetMagic != 0) {
		lRet = OAL_FDT_MAGIC_ERROR;
		goto ret_setdtbblocks;
	}

	lRetGetHeader = getDtbHeader(acpDtb, &lDtbH);
	if (lRetGetHeader != 0) {
		lRet = OAL_FDT_BAD_HEADER;
		goto ret_setdtbblocks;
	}

	getDtbStrsOffset(&lDtbH, acpDtb, acpStrsOff);

	lRetStructOff = getDtbStructOffset(&lDtbH, acpDtb, acpStructOff);
	if (lRetStructOff != 0) {
		lRet = OAL_FDT_BAD_STRUCT_OFF;
		goto ret_setdtbblocks;
	}

	lRetStructLen = getStructLen(&lDtbH, apStructLen);
	if (lRetStructLen != 0) {
		lRet = OAL_FDT_BAD_STRUCT_LEN;
	}

ret_setdtbblocks:
	return lRet;
}

static int32_t getNextFdtNode(uint32_t aOff, uint32_t aStructLen,
                              const uint32_t *acpStructOffset,
                              uint32_t *apNextOff)
{
	uint32_t lNextStructOff;
	int32_t lRet;
	uint32_t lOff = aOff;

	lRet = OAL_FDT_NEXT_NODE_NOT_FOUND;
	for (lOff += 1U; (lOff + 1U) < aStructLen; lOff++) {
		lNextStructOff = OAL_GetBigEndian32(acpStructOffset[lOff + 1U]);

		if ((OAL_GetBigEndian32(acpStructOffset[lOff]) ==
		     OAL_FDT_BEGIN_NODE) &&
		    (lNextStructOff != OAL_FDT_PROP) &&
		    (lNextStructOff != OAL_FDT_END_NODE)) {
			*apNextOff = lOff;
			lRet       = 0;
			break;
		}
	}

	return lRet;
}

int32_t OAL_GetName(const uint32_t *acpDtb, uint32_t aNodeOffset,
                    uint32_t *apLenN, char8_t **apName)
{
	uint32_t lStructLen, lI;
	const uint32_t *lcpStructOffset, *lcpStrsOffset;
	int32_t lRet, lRetBlocks;

	lRetBlocks =
	    setDtbBlocks(acpDtb, &lcpStructOffset, &lcpStrsOffset, &lStructLen);
	if (lRetBlocks != 0) {
		lRet = OAL_FDT_BLOCKS_ERROR;
		goto ret_getname;
	}

	lRet = OAL_FDT_NAME_ERROR;
	for (lI = 0U; (aNodeOffset + lI + 1U) < lStructLen; lI++) {
		if ((OAL_GetBigEndian32(lcpStructOffset[aNodeOffset + lI]) ==
		     OAL_FDT_BEGIN_NODE) &&
		    (OAL_GetBigEndian32(
		         lcpStructOffset[aNodeOffset + lI + 1U]) !=
		     OAL_FDT_PROP)) {
			*apName =
			    (char8_t *)(uintptr_t)&lcpStructOffset[aNodeOffset +
			                                           lI + 1U];
			if (apLenN != NULL) {
				*apLenN = (uint32_t)strlen(*apName);
			}

			lRet = 0;
			break;
		}
	}

ret_getname:
	return lRet;
}

int32_t OAL_GetProp(const uint32_t *acpDtb, uint32_t aNodeOff,
                    const char8_t *acpPropName, uint32_t *apLenPVal,
                    uint8_t **apPropValAddr)
{
	struct DtbPropOff lApDtbP;
	const uint32_t *lcpStrsOff, *lcpStructOff, *lcpNodeOffset;
	int32_t lRet, lRetBlocks;
	uint32_t lStructLen, lI;
	size_t lStrsLen;

	lRetBlocks =
	    setDtbBlocks(acpDtb, &lcpStructOff, &lcpStrsOff, &lStructLen);
	if (lRetBlocks != 0) {
		lRet = OAL_FDT_BLOCKS_ERROR;
		goto ret_getprop;
	}

	lcpNodeOffset = lcpStructOff + aNodeOff;

	lRet = OAL_FDT_PROP_OFFSET_ERROR;
	for (lI = 0; (lI + aNodeOff + 1U) < lStructLen; lI++) {
		if (OAL_GetBigEndian32(lcpNodeOffset[lI]) == OAL_FDT_PROP) {
			getDtbPropOff(lcpNodeOffset + lI, &lApDtbP, lcpStrsOff);

			if (OAL_strnlen(acpPropName, MAX_PROP_NAME_LEN) ==
			    MAX_PROP_NAME_LEN) {
				OAL_LOG_WARNING(
				    "Property name length is at"
				    "MAX_PROP_NAME_LEN limit \n");
			}

			lStrsLen = OAL_strnlen(acpPropName, MAX_PROP_NAME_LEN);

			if (strncmp(acpPropName, COMPATIBLE_PROP, lStrsLen) ==
			    0) {
				if ((uintptr_t)lApDtbP.mcpPropName >
				    (uintptr_t)(lcpStrsOff + lStrsLen)) {
					break;
				}
			}

			if (strncmp(lApDtbP.mcpPropName, acpPropName,
			            lStrsLen) == 0) {
				if (lApDtbP.mPropValLen == 0U) {
					/* Property without value */
					*apPropValAddr = NULL;
				} else {
					*apPropValAddr =
					    (uint8_t *)(uintptr_t)
					        lApDtbP.mcpPropValBlock;
				}

				if (apLenPVal != NULL) {
					*apLenPVal = lApDtbP.mPropValLen;
				}

				lRet = 0;
				break;
			}
		}

		if ((OAL_GetBigEndian32(lcpNodeOffset[lI]) ==
		     OAL_FDT_END_NODE) &&
		    (OAL_GetBigEndian32(lcpNodeOffset[lI + 1U]) !=
		     OAL_FDT_PROP)) {
			*apPropValAddr = NULL;
			lRet           = OAL_FDT_END_NODE_OFFSET;
			break;
		}
	}

ret_getprop:
	return lRet;
}

int32_t OAL_NodeOffsetByPhandle(const uint32_t *acpDtb, uint32_t aPhandle,
                                uint32_t *apNodeOffset)
{
	const uint32_t *lcpStrsOff, *lcpStructOff;
	uint32_t *lpPropPhandle;
	uint32_t lNodeOff, lStructVal, lStructLen, lOff = 0U;
	uint8_t *lpPropValAddr;
	int32_t lRet, lRetBlocks, lRetPhandle;

	if (apNodeOffset == NULL) {
		lRet = OAL_FDT_NULL_ERROR;
		goto ret_phandle;
	}

	lRetBlocks =
	    setDtbBlocks(acpDtb, &lcpStructOff, &lcpStrsOff, &lStructLen);
	if (lRetBlocks != 0) {
		lRet = OAL_FDT_BLOCKS_ERROR;
		goto ret_phandle;
	}

	lRet = OAL_FDT_PHANDLE_OFFSET_ERROR;

	for (lNodeOff = 0U; (lNodeOff + 1U) < lStructLen; lNodeOff++) {
		lStructVal = OAL_GetBigEndian32(lcpStructOff[lNodeOff]);

		if (lStructVal == OAL_FDT_BEGIN_NODE) {
			lOff = lNodeOff;
		}

		if ((lStructVal == OAL_FDT_PROP) &&
		    (OAL_GetBigEndian32(lcpStructOff[lNodeOff + 1U]) ==
		     OAL_FDT_NOP)) {
			lRetPhandle =
			    OAL_GetProp(acpDtb, lNodeOff, PHANDLE_PROP, NULL,
			                &lpPropValAddr);

			if (lRetPhandle != 0) {
				lRetPhandle = OAL_GetProp(acpDtb, lNodeOff,
				                          LINUX_PHANDLE_PROP,
				                          NULL, &lpPropValAddr);
			}

			if (lRetPhandle == 0) {
				uintptr_t lPropValAddr =
				    (uintptr_t)lpPropValAddr;
				lpPropPhandle = (uint32_t *)lPropValAddr;
				if (*lpPropPhandle ==
				    OAL_GetBigEndian32(aPhandle)) {
					*apNodeOffset = lOff;
					lRet          = 0;
					goto ret_phandle;
				}
			}
		}
	}

ret_phandle:
	return lRet;
}

int32_t OAL_NodeOffsetByCompatible(const uint32_t *acpDtb, uint32_t aStartOff,
                                   const char8_t *acpCompatible,
                                   uint32_t *apNodeOffset)
{
	uint32_t lI, lStructLen, lCompLen, lNextOff = 0U;
	const uint32_t *lcpStrsOff, *lcpStructOff;
	uint8_t *lpPropValAddr;
	int32_t lRet, lRetBlocks, lRetProp;

	if (apNodeOffset == NULL) {
		lRet = OAL_FDT_NULL_ERROR;
		goto ret_compatible;
	}

	*apNodeOffset = (uint32_t)(OAL_FDT_OFFSET_ERR);

	lRetBlocks =
	    setDtbBlocks(acpDtb, &lcpStructOff, &lcpStrsOff, &lStructLen);
	if (lRetBlocks != 0) {
		lRet = OAL_FDT_BLOCKS_ERROR;
		goto ret_compatible;
	}
	/**
	 * root node has offset 0;
	 * According to libfdt API, if starting offset (aStartOff) is -1,
	 * search includes root node
	 */
	else if (aStartOff == (uint32_t)(OAL_FDT_OFFSET_ERR)) {
		lI = 0U;
	}
	/**if starting offset (aStartOff) is 0, root node is ommited
	 * from the search
	 */
	else if (aStartOff == 0U) {
		lI = 1U;
	} else {
		lI = aStartOff;
	}

	lRet = OAL_FDT_COMPATIBLE_OFFSET_ERROR;

	while (lI < lStructLen) {
		if (lI > 0U) {
			if (getNextFdtNode(lI, lStructLen, lcpStructOff,
			                   &lNextOff) != 0) {
				lRet = OAL_FDT_NEXT_NODE_NOT_FOUND;
				goto ret_compatible;
			}
		}

		lRetProp = OAL_GetProp(acpDtb, lNextOff, COMPATIBLE_PROP,
		                       &lCompLen, &lpPropValAddr);

		if (lRetProp == 0) {
			if (strncmp((char8_t *)lpPropValAddr, acpCompatible,
			            (uint32_t)lCompLen) == 0) {
				*apNodeOffset = lNextOff;
				lRet          = 0;
				goto ret_compatible;
			}
		}

		lI++;
	}

ret_compatible:
	return lRet;
}
