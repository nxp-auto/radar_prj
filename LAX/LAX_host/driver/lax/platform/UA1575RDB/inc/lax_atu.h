/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef LAX_ATU_H
#define LAX_ATU_H

/* This doxy template is to be used by files visible to customers.
 * Mind the @} ending comment, too.
 * Remove this explaining comment when specializing this template file.
 */

/** @addtogroup <name_of_doxygen_module>
* @{
*/

/*=================================================================================================
*                                        INCLUDE FILES
=================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif


/*=================================================================================================
*                                      DEFINES AND MACROS
=================================================================================================*/
#define MAX_STRING_ATU_WIN  256
#define LAX_ATU_MIN_WIN_SZ  16
#define MAX_LAX_ATU_WIN     8


/*=================================================================================================
*                                          CONSTANTS
=================================================================================================*/

/*=================================================================================================
*                                             ENUMS
=================================================================================================*/

/*=================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
=================================================================================================*/
typedef struct {
    uint64_t windwBaseAdd;
    uint64_t windwSz;
    uint64_t windwMappedAdd;
} atuWindow_t;

typedef struct {
    u32 __iomem *pLaxAtuRegs; /* Virtual address */
    u32 __iomem *pLaxAtuAddr; /* Physical address */
    uint32_t    laxAtuSize;
    uint32_t    numAtuWin;
    atuWindow_t laxAtuWin[MAX_LAX_ATU_WIN];
} atuControl_t;

/*=================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
=================================================================================================*/


/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/
void        LaxAtuEnable(atuControl_t *pAtuCtrl);
void        LaxAtuDisable(atuControl_t *pAtuCtrl);
uint32_t    LaxMapAxiAddrAtuWin(atuControl_t *pAtuCtrl, dma_addr_t);


#ifdef __cplusplus
}
#endif

/** @} */ /*doxygen module*/

#endif /*RSDK_LAXATU_H*/

