/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RSDK_CBUFFER_H
#define RSDK_CBUFFER_H

/* This doxy template is to be used by files visible to customers.
 * Mind the @} ending comment, too.
 * Remove this explaining comment when specializing this template file.
 */

/** @addtogroup <name_of_doxygen_module>
* @{
*/

/*=================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
=================================================================================================*/
#include <oal_utils.h>

#ifdef __cplusplus
/*do this after the #includes*/
extern "C" {
#endif


/*=================================================================================================
*                                      DEFINES AND MACROS
=================================================================================================*/
/**
* @brief          A brief text description (one line).
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/



/*=================================================================================================
*                                          CONSTANTS
=================================================================================================*/
/**
* @brief          A brief text description (one line).
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/



/*=================================================================================================
*                                             ENUMS
=================================================================================================*/
/**
* @brief          A brief text description (one line).
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
* @pre            Preconditions as text description. Optional tag.
* @post           Postconditions as text description. Optional tag.
*
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/

/*=================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
=================================================================================================*/
/**
* @brief          A brief text description (one line).
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/
typedef struct {
    uint32_t    writeIdx;
    uint32_t    readIdx;
    uint32_t    size;
    uint32_t    used;
    uint32_t    *paddr;
    uint32_t    *vaddr;
    int         cbSizePrev;
    int         cbIdxPrev;
} circularBuffer_t;


/*=================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
=================================================================================================*/


/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/

/**
* @brief          A brief text description (one line).
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
*
* @param[in]      number      Describes a parameter that is input to the function or macro.
*                             Must be omitted if the function does not have parameters.
* @param[out]     object_ptr  Describes a parameter that is output of the function or macro.
*                             Must be omitted if the function does not have parameters.
* @param[in,out]  config_ptr  Describes a parameter that is both input and output of the function or
*                             macro.
*                             Must be omitted if the function does not have parameters.
*
* @return         Description of the returned value.
*                 One can use @ref to point to a regularly used type defined in an @addtogroup.
* @retval RETURNED_VALUE_1    Describes specific return value RETURNED_VALUE_1 and its meaning. There
*                             can be more than one @retval tags for each function.
*                             It must be omitted if the function does not return specific values.
* @retval RETURNED_VALUE_2    Describes specific return value RETURNED_VALUE_2 and its meaning. There
*                             can be more than one @retval tags for each function.
*                             It must be omitted if the function does not return specific values.
*
* @pre            Preconditions as text description. Optional tag.
* @post           Postconditions as text description. Optional tag.
*
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/

void CbufferReset(circularBuffer_t *cbuf);
void CbufferInit(circularBuffer_t *cbuf, uint32_t size, uint32_t *pAddr, uint32_t *pVaddr);
int CbufferAdd(circularBuffer_t *cbuf, uint32_t size, uint32_t nextPtrSize);
void CbufferFree(circularBuffer_t *cbuf, uint32_t index, uint32_t size);


#ifdef __cplusplus
}
#endif

/** @} */ /*doxygen module*/

#endif /*RSDK_CBUFFER_H*/

