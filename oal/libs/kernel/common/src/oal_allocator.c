/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/** Linux kernel version change by Rostislav Hulik (rostislav.hulik@nxp.com) **/

#include "oal_allocator.h"
#include "oal_allocator_private.h"
#include "oal_log.h"
#include "oal_kalloc.h"

#ifndef INT64_MAX
#define INT64_MAX 0x7fffffffffffffffLL
#endif

#define OAL_LOG_NOTE_OFF

static void __apex_free_addrListElement_init(uint8_t space_id);
static void __apex_free_chunkListElement_init(uint8_t space_id);
static void* apex_allocator_malloc_internal(uint8_t space_id, uint64_t size, const uint64_t align, int32_t special);

static uint64_t gs__apex_pmem_size[OAL_MAX_ALLOCATOR_NUM];
static uint64_t gs__apex_base_pmem_addr[OAL_MAX_ALLOCATOR_NUM];

static int32_t apex_allocator_init(uint8_t space_id);

static void apex_allocator_lib_init(uint8_t space_id);
#if 0
void apex_allocator_destroy(void);
#endif

/*
 *  Definitions
 */
#define __HEAD_LIST(pSentinel)             ((pSentinel)->pNext)
#define __TAIL(pSentinel)             ((pSentinel)->pPrev)
#define __IS_EMPTY(pSentinel)         ((pSentinel)->pNext == (pSentinel))
#define __IS_HEAD(pSentinel, pNode)   ((pNode)->pPrev == (pSentinel))
#define __IS_TAIL(pSentinel, pNode)   ((pNode)->pNext == (pSentinel))

static int32_t       gs_num_allocations[OAL_MAX_ALLOCATOR_NUM] = {0};
static AddrNode_t*   gs_AddrNodes[OAL_MAX_ALLOCATOR_NUM] = {0};
static AddrNode_t    gs_FreeAddrNodeList[OAL_MAX_ALLOCATOR_NUM];
static AddrNode_t*   gsp_FreeAddrNodeList[OAL_MAX_ALLOCATOR_NUM];

static ChunkNode_t*  gs_ChunkNodes[OAL_MAX_ALLOCATOR_NUM] = {0};
static ChunkNode_t   gs_FreeChunkNodeList[OAL_MAX_ALLOCATOR_NUM];
static ChunkNode_t*  gsp_FreeChunkNodeList[OAL_MAX_ALLOCATOR_NUM];

static Allocator_t  gs_Allocator[OAL_MAX_ALLOCATOR_NUM];
static Allocator_t* gsp_Allocator[OAL_MAX_ALLOCATOR_NUM];

static int gs_num_chunks[OAL_MAX_ALLOCATOR_NUM] = {0};
/*
 * PUBLIC
 * Initialization function
 */
void apex_allocator_mmap_init(uint8_t space_id, uint64_t base, uint64_t size)
{
  gs__apex_pmem_size[space_id] = size;
  gs__apex_base_pmem_addr[space_id] = base;

  gsp_FreeAddrNodeList[space_id]  = &(gs_FreeAddrNodeList[space_id]);
  gsp_FreeChunkNodeList[space_id] = &(gs_FreeChunkNodeList[space_id]);
  gsp_Allocator[space_id] = &gs_Allocator[space_id];

  gs_AddrNodes[space_id] = OAL_Kalloc(sizeof(AddrNode_t) * MAX_ADDRSLIST, VIRT_MEM);
  gs_ChunkNodes[space_id] = OAL_Kalloc(sizeof(ChunkNode_t) * MAX_CHUNKLIST, VIRT_MEM);

  apex_allocator_lib_init(space_id);
}

void apex_allocator_mmap_deinit(uint8_t space_id)
{
    if (gs_AddrNodes[space_id] != 0)
    {
        OAL_Kfree(gs_AddrNodes[space_id], VIRT_MEM);
        gs_AddrNodes[space_id] = 0;
    }
    if (gs_ChunkNodes[space_id] != 0)    
    {
        OAL_Kfree(gs_ChunkNodes[space_id], VIRT_MEM);
        gs_ChunkNodes[space_id] = 0;
    }
}

/*
 * PRIVATE
 * Utility function of "apex_allocator_mmap_init"
 * Stack :
 * - apex_allocator_mmap_init
 * -- apex_allocator_lib_init
 */
static void apex_allocator_lib_init(uint8_t space_id)
{
  if ( apex_allocator_init(space_id) != 0 ) {
    OAL_LOG_ERROR("failed to init apex allocator id: %d\n", space_id);
  }
}

/*
 * PRIVATE
 * Utility function of "apex_allocator_mmap_init"
 * Stack :
 * - apex_allocator_mmap_init
 * -- apex_allocator_lib_init
 * --- apex_allocator_init
 *
 */
static int32_t apex_allocator_init(uint8_t space_id)
{
   ChunkNode_t* pChunkNode = NULL;
   uint64_t addr;
   uint64_t size;
   int32_t created = 1;

   addr = gs__apex_base_pmem_addr[space_id];
   size = gs__apex_pmem_size[space_id];

   if (created)
   {
      __apex_free_addrListElement_init(space_id);
      __apex_free_chunkListElement_init(space_id);

      pChunkNode = helper_chunk_node_alloc(space_id);

      helper_list_init(&(gsp_Allocator[space_id]->chunk_sentinel));

      pChunkNode->addr = addr;
      pChunkNode->size = size;
      pChunkNode->is_free = true;

      helper_list_addHead(&(gsp_Allocator[space_id]->chunk_sentinel),
                          pChunkNode);
   }

   return 0;
}

/*
 * PUBLIC
 * Memory allocation function
 */
void* apex_allocator_malloc(uint8_t space_id, uint64_t size, const uint64_t align)
{
    #if 0
    GAIN_LOCK;
    #endif

    void *temp = apex_allocator_malloc_internal(space_id, size, align, 0);
    #if 0
    RELEASE_LOCK;
    #endif
    return temp;
}

/*
 * PRIVATE
 * Utility function of "apex_allocator_malloc"
 * Stack :
 * - apex_allocator_malloc
 * -- apex_allocator_malloc_internal
 */
static void* apex_allocator_malloc_internal(uint8_t space_id, uint64_t size,
                                     const uint64_t align, int32_t special)
{
   uint64_t  addr = 0;
   uint64_t  allocated_addr = 0;
   ChunkNode_t* pChunkNode = __HEAD_LIST(&(gsp_Allocator[space_id]->chunk_sentinel));
   void *   retval;

   UNUSED_ARG(special);

   if ((size == 0) || (gs__apex_pmem_size[space_id] < size) ||
       (gs_num_allocations[space_id] == MAX_ADDRSLIST) ||
       (gs_num_chunks[space_id] == MAX_CHUNKLIST-2))
   {
      retval = (void *)0;
   }

   else
   {
      size = (size + 3) & ~3U;

      while (pChunkNode != &(gsp_Allocator[space_id]->chunk_sentinel))
      {
         allocated_addr = (pChunkNode->addr + (align-1)) & ~(align-1);
         if ((pChunkNode->is_free == true) &&                                     // Chunk must be free
             ((allocated_addr + size) <= (pChunkNode->addr + pChunkNode->size)))  // And have enough space for the allocation
         {
            break;
         }

         pChunkNode = (ChunkNode_t*)pChunkNode->pNext;
      }

      /* check if finding free chunk */
      if (pChunkNode != &(gsp_Allocator[space_id]->chunk_sentinel))   // We found chunk with enough free space
      {
         /* --------------------------------------------------------------------------*/
         /* Process new allocated area                                                */
         /* --------------------------------------------------------------------------*/

         /* get new allocated address */
         addr = pChunkNode->addr;
         size += allocated_addr - addr;

         if (__IS_HEAD(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNode))
         {
            /* In the case that chunk is first chunk, insert new  ALLOC chunk */
            /* to the head of chunk list and link new allocated area to the   */
            /* new ALLOC chunk                                                */
            ChunkNode_t* pChunkNode2 = helper_chunk_node_alloc(space_id);
            AddrNode_t*  pAddrNode2  = helper_addr_node_alloc_addr(space_id);

            pChunkNode2->addr          = pAddrNode2->addr = addr;
            pChunkNode2->size          = pAddrNode2->size = size;
            pAddrNode2->allocated_addr = allocated_addr;
            pChunkNode2->is_free       = false;

            helper_list_addHead_addr(&(pChunkNode2->addr_sentinel), pAddrNode2);
            helper_list_addHead(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNode2);
         }
         else
         {
            /* In the case that chunk is not first chunk, add new allocated area */
            /* to the tail of the previous ALLOC chunk                           */
            ChunkNode_t* pChunkNode2 = pChunkNode->pPrev;
            AddrNode_t*  pAddrNode2  = helper_addr_node_alloc_addr(space_id);

            pAddrNode2->size            = size;
            pAddrNode2->addr            = addr;
            pAddrNode2->allocated_addr  = allocated_addr;
            pChunkNode2->size          += size;

            helper_list_addTail_addr(&(pChunkNode2->addr_sentinel), pAddrNode2);
         }

         /* --------------------------------------------------------------------------*/
         /* Process FREE chunk which provides new allocated area                      */
         /* --------------------------------------------------------------------------*/

         /* Get new alloc area from the first size bytes of FREE chunk */
         pChunkNode->addr += size;
         pChunkNode->size -= size;

         /* Check if the FREE chunk area consume all of it */
         if (pChunkNode->size == 0)
         {
            if (__IS_TAIL(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNode))
            {
               /* In the case of no chunk after FREE_CHUNK, remove and delete FREE chunk */
               helper_list_remove(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNode);
               helper_chunk_node_free(space_id, pChunkNode);
               pChunkNode = 0;
            }
            else
            {
               /* In the case of having ALLOC chunk after FREE_CHUNK, merge previous and next ALLOC chunk */
               ChunkNode_t* pChunkNodeNext = (ChunkNode_t*)pChunkNode->pPrev;
               ChunkNode_t* pChunkNodePrev = (ChunkNode_t*)pChunkNode->pNext;

               helper_list_append(&(pChunkNodeNext->addr_sentinel), &(pChunkNodePrev->addr_sentinel));
               pChunkNodeNext->size += pChunkNodePrev->size;

               helper_list_remove(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNodePrev);
               helper_list_remove(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNode);
               helper_chunk_node_free(space_id, pChunkNodePrev);
               helper_chunk_node_free(space_id, pChunkNode);
               pChunkNodePrev = 0;
               pChunkNode = 0;
            }
         }
      }
      else
      {
         allocated_addr = 0;
         /* No enough free space */
         //OAL_LOG_ERROR("NO ENOUGH FREE AREA ID: %d\n", space_id);
      }

      if (allocated_addr != 0) {
          ++gs_num_allocations[space_id];
      }
      retval = (void *)allocated_addr;
   }
   return retval;
}


/*
 * PUBLIC
 * Memory deallocation function
 */
void apex_allocator_free(uint8_t space_id, void *addr)
{
    #if 0
    GAIN_LOCK;
    #endif
    ChunkNode_t* pChunkNode = __HEAD_LIST(&(gsp_Allocator[space_id]->chunk_sentinel));

   /* Find corresponding ALLOC chunk */
   while (pChunkNode != &(gsp_Allocator[space_id]->chunk_sentinel))
   {
      if ((pChunkNode->is_free                == false) &&
          (pChunkNode->addr                   <= (uint64_t)addr) &&
          (pChunkNode->addr + pChunkNode->size > (uint64_t)addr))
      {
         break;
      }

      pChunkNode = (ChunkNode_t*)pChunkNode->pNext;
   }
   /* Check if finding corresponding ALLOC chunk */
   if (pChunkNode != &(gsp_Allocator[space_id]->chunk_sentinel))
   {
      AddrNode_t* pAddrNodeFree = helper_list_find(&(pChunkNode->addr_sentinel), addr);

      if (pAddrNodeFree)
      {
         if (!__IS_HEAD(&(pChunkNode->addr_sentinel), pAddrNodeFree) &&
             !__IS_TAIL(&(pChunkNode->addr_sentinel), pAddrNodeFree))   // Cx{A0,...,Ay,...,An}, where Ay is neither the first or the last AddrNode on that ChunkNode.
         {
            /* The deallocated area is in the middle of the chunk, create new chunk and split current ALLOC chunk*/
            /* Cx{A0,...}, Cy, Cz{...,An} */
            ChunkNode_t* pChunkNodeFree  = 0;
            ChunkNode_t* pChunkNodeAlloc = 0;

            /* Create FREE chunk */
            pChunkNodeFree          = helper_chunk_node_alloc(space_id);
            pChunkNodeFree->addr    = pAddrNodeFree->addr;
            pChunkNodeFree->size    = pAddrNodeFree->size;
            pChunkNodeFree->is_free = true;

            /* Create new ALLOC chunk */
            pChunkNodeAlloc          = helper_chunk_node_alloc(space_id);
            pChunkNodeAlloc->addr    = pAddrNodeFree->addr + pAddrNodeFree->size;
            pChunkNodeAlloc->size    = pChunkNode->size - (pChunkNodeAlloc->addr - pChunkNode->addr);
            pChunkNodeAlloc->is_free = false;

            /* Change original ALLOC chunk size */
            pChunkNode->size = pAddrNodeFree->addr - pChunkNode->addr;

            helper_list_split(&(pChunkNode->addr_sentinel),
                                 &(pChunkNodeAlloc->addr_sentinel),
                                 pAddrNodeFree);

            helper_list_remove_addr(&(pChunkNodeAlloc->addr_sentinel),
                               pAddrNodeFree);

            helper_addr_node_free_addr(space_id, pAddrNodeFree);
            pAddrNodeFree = 0;

            /* Link the chunk                                         */
            /* OriginalChunk(chunk) -> newFreeChunk -> nextAllocChunk */
            pChunkNodeAlloc->pNext   = pChunkNode->pNext;
            pChunkNode->pNext->pPrev = pChunkNodeAlloc;
            pChunkNode->pNext        = pChunkNodeFree;
            pChunkNodeFree->pPrev    = pChunkNode;
            pChunkNodeFree->pNext    = pChunkNodeAlloc;
            pChunkNodeAlloc->pPrev   = pChunkNodeFree;
         }
         else
         {
            /* Check if the allocated area is the head of ALLOC chunk */
            if (__IS_HEAD(&(pChunkNode->addr_sentinel), pAddrNodeFree)) // Cx{A0,...}, where A0 is the first AddrNode on that ChunkNode.
            {
               if (__IS_HEAD(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNode)) // And it is the first ChunkNode
               {
                  /* In the case that this ALLOC chunk is first chunk, create new FREE */
                  /* chunk for new FREE area                                           */
                  ChunkNode_t* pChunkNodeFree = 0;

                  pChunkNodeFree          = helper_chunk_node_alloc(space_id);
                  pChunkNodeFree->addr    = pAddrNodeFree->addr;
                  pChunkNodeFree->size    = pAddrNodeFree->size;
                  pChunkNodeFree->is_free = true;

                  helper_list_addHead(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNodeFree);
               }
               else
               {
                  /* In the case that chunk is not the 1st chunk, increase the size of previous FREE chunk */
                  /* Here it is safe to assume that the previous chunk is already a free chunk */
                  pChunkNode->pPrev->size += pAddrNodeFree->size;
               }
               /* Remove free area from ALLOC chunk */
               pChunkNode->addr += pAddrNodeFree->size;
               pChunkNode->size -= pAddrNodeFree->size;
            }
            /* Check if the allocated area is the tail of ALLOC chunk */
            else
            {
                if (__IS_TAIL(&(pChunkNode->addr_sentinel), pAddrNodeFree)) // Cx{...,An}, where An is the last AddrNode on that ChunkNode.
                {
                   if (!__IS_TAIL(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNode)) // And it is the not the last ChunkNode
                   {
                      /* Here it is safe to assume that the next chunk is already a free chunk */
                      pChunkNode->pNext->addr  = pAddrNodeFree->addr;
                      pChunkNode->pNext->size += pAddrNodeFree->size;
                   }
                   else
                   {
                      /* In the case that there is no tail FREE chunk, create new FREE */
                      ChunkNode_t* pChunkNodeFree = 0;

                      pChunkNodeFree          = helper_chunk_node_alloc(space_id);
                      pChunkNodeFree->addr    = pAddrNodeFree->addr;
                      pChunkNodeFree->size    = pAddrNodeFree->size;
                      pChunkNodeFree->is_free = true;

                      helper_list_addTail(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNodeFree);
                   }
                   pChunkNode->size -= pAddrNodeFree->size;
                }
            }
            #if 0
            else
            {
                APEXALLOC_ASSERT(0);
            }
            #endif

            /* Remove address element for the address chunk */
            helper_list_remove_addr(&(pChunkNode->addr_sentinel), pAddrNodeFree);
            helper_addr_node_free_addr(space_id, pAddrNodeFree);
            pAddrNodeFree = 0;


            if (pChunkNode->size == 0)
            {
               /* In the case that this ALLOC chunk do not have any allocated area */

               if (__IS_TAIL(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNode))
               {
                  /* In the case that this ALLOC chunk has no following chunk, just */
                  /* remove this ALLOC chunk                                        */
                  helper_list_remove(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNode);
                  helper_chunk_node_free(space_id, pChunkNode);
                  pChunkNode = 0;
               }
               else
               {
                  /* In the case that this ALLOC chunk has next chunk, merge previous */
                  /* and next FREE chunk                                              */
                  ChunkNode_t* pChunkNodePrev = (ChunkNode_t*)pChunkNode->pPrev;
                  ChunkNode_t* pChunkNodeNext = (ChunkNode_t*)pChunkNode->pNext;

                  pChunkNodePrev->size += pChunkNodeNext->size;
                  helper_list_remove(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNode);
                  helper_list_remove(&(gsp_Allocator[space_id]->chunk_sentinel), pChunkNodeNext);
                  helper_chunk_node_free(space_id, pChunkNode);
                  helper_chunk_node_free(space_id, pChunkNodeNext);
                  pChunkNode = 0;
                  pChunkNodeNext = 0;
               }
            }
         }
      }
      else  {
        OAL_LOG_ERROR("free fail\n");
      }
   }
   else {
     OAL_LOG_ERROR("free fail\n");
   }
   if (gs_num_allocations[space_id] > 0) {
       --gs_num_allocations[space_id];
   }
   return;
}



/*
 * PRIVATE
 * Utility function of "ApexList"
 * Used to initialize the sentinel node named "dummy"
 */
static void helper_list_init(ChunkNode_t* pSentinel)
{
   pSentinel->pPrev = pSentinel->pNext = pSentinel;
}
static void helper_list_init_addr(AddrNode_t* pSentinel)
{
   pSentinel->pPrev = pSentinel->pNext = pSentinel;
}



/*
 * PRIVATE
 * Utility function of "ApexList"
 * Used to add a node to the head of the list
 */
static void helper_list_addHead(ChunkNode_t* pSentinel, ChunkNode_t* pNodeToBeAdded)
{
    #if 0
    APEXALLOC_ASSERT(pSentinel && pNodeToBeAdded);
    #endif

    /* addr_sentinel <-> pNodeToBeAdded <-> Old_Head */
   ChunkNode_t* const cpOldHead  = __HEAD_LIST(pSentinel);

   pSentinel->pNext      = pNodeToBeAdded;
   pNodeToBeAdded->pPrev = pSentinel;
   pNodeToBeAdded->pNext = cpOldHead;
   cpOldHead->pPrev      = pNodeToBeAdded;

   return;
}
static void helper_list_addHead_addr(AddrNode_t* pSentinel, AddrNode_t* pNodeToBeAdded)
{
    #if 0
    APEXALLOC_ASSERT(pSentinel && pNodeToBeAdded);
    #endif

   /* addr_sentinel <-> pNodeToBeAdded <-> Old_Head */
   AddrNode_t* const cpOldHead  = __HEAD_LIST(pSentinel);

   pSentinel->pNext      = pNodeToBeAdded;
   pNodeToBeAdded->pPrev = pSentinel;
   pNodeToBeAdded->pNext = cpOldHead;
   cpOldHead->pPrev      = pNodeToBeAdded;

   return;
}



/*
 * PRIVATE
 * Utility function of "ApexList"
 * Used to add a node to the tail of the list
 */
static void helper_list_addTail(ChunkNode_t* pSentinel, ChunkNode_t* pNodeToBeAdded)
{
    #if 0
    APEXALLOC_ASSERT(pSentinel && pNodeToBeAdded);
    #endif

   /* Old_Tail <-> pNodeToBeAdded <-> addr_sentinel */
   ChunkNode_t* const cpOldTail  = __TAIL(pSentinel);

   cpOldTail->pNext      = pNodeToBeAdded;
   pNodeToBeAdded->pPrev = cpOldTail;
   pNodeToBeAdded->pNext = pSentinel;
   pSentinel->pPrev      = pNodeToBeAdded;

   return;
}
static void helper_list_addTail_addr(AddrNode_t* pSentinel, AddrNode_t* pNodeToBeAdded)
{
    #if 0
    APEXALLOC_ASSERT(pSentinel && pNodeToBeAdded);
    #endif

   /* Old_Tail <-> pNodeToBeAdded <-> addr_sentinel */
   AddrNode_t* const cpOldTail  = __TAIL(pSentinel);

   cpOldTail->pNext      = pNodeToBeAdded;
   pNodeToBeAdded->pPrev = cpOldTail;
   pNodeToBeAdded->pNext = pSentinel;
   pSentinel->pPrev      = pNodeToBeAdded;

   return;
}



/*
 * PRIVATE
 * Utility function of "ApexList"
 * Used to remove a node in the list
 */
static void helper_list_remove(ChunkNode_t* pSentinel, ChunkNode_t* pNodeToBeRemoved)
{
    #if 0
    APEXALLOC_ASSERT(pSentinel && pNodeToBeRemoved);
    #endif

   UNUSED_ARG(pSentinel);

   /* Set previous and next pointer to element */
   pNodeToBeRemoved->pPrev->pNext = pNodeToBeRemoved->pNext;
   pNodeToBeRemoved->pNext->pPrev = pNodeToBeRemoved->pPrev;

   return;
}
static void helper_list_remove_addr(AddrNode_t* pSentinel, AddrNode_t* pNodeToBeRemoved)
{
    #if 0
    APEXALLOC_ASSERT(pSentinel && pNodeToBeRemoved);
    #endif

   UNUSED_ARG(pSentinel);

   /* Set previous and next pointer to element */
   pNodeToBeRemoved->pPrev->pNext = pNodeToBeRemoved->pNext;
   pNodeToBeRemoved->pNext->pPrev = pNodeToBeRemoved->pPrev;

   return;
}


/*
 * PRIVATE
 *    Utility function of "ApexList"
 *    Used to find a node in the list
 *
 * Only user:
 * - apex_allocator_free
 */
static AddrNode_t *helper_list_find(AddrNode_t* pSentinel, void* pAddress)
{
   AddrNode_t * retval = 0;

   AddrNode_t* pAddrNode = __HEAD_LIST(pSentinel);
   while (pAddrNode != pSentinel)
   {
      if ( helper_addr_compare(pAddress, pAddrNode) == 0)
      {
         retval = pAddrNode;
         break;
      }
      pAddrNode = pAddrNode->pNext;
   }
   return retval;
}



/*
 * PRIVATE
 *    Utility function of "ApexList"
 *    Used to split the original list in two.
 *    First with everything to the left of pAddrNode.
 *    Second pAddrNode and everything to its right.
 *    There is an assumed precondition that pAddrNode is neither the tail nor the head of the original list!
 *
 * Only user:
 * - apex_allocator_free
 */
static void helper_list_split(AddrNode_t* pSentinelLeft,  // Original list which retains everything to the left of pAddrNode
                              AddrNode_t* pSentinelRight, // New list which gets pAddrNode and everything to its right
                              AddrNode_t* pAddrNode)
{
   AddrNode_t* pOldTail = __TAIL(pSentinelLeft);

   __TAIL(pSentinelLeft)   = pAddrNode->pPrev;
   pAddrNode->pPrev->pNext = pSentinelLeft;

   __HEAD_LIST(pSentinelRight) = pAddrNode;
   pAddrNode->pPrev       = pSentinelRight;
   pOldTail->pNext        = pSentinelRight;
   __TAIL(pSentinelRight) = pOldTail;


   return;
}



/*
 * PRIVATE
 *    Utility function of "ApexList"
 *    Used for random insertion?
 *
 * Only user:
 * - apex_allocator_free
 */
static void helper_list_append(AddrNode_t* pSentinel0, AddrNode_t* pSentinel1)
{
   __TAIL(pSentinel0)->pNext = __HEAD_LIST(pSentinel1);
   __HEAD_LIST(pSentinel1)->pPrev = __TAIL(pSentinel0);
   __TAIL(pSentinel0)        = __TAIL(pSentinel1);
   __TAIL(pSentinel0)->pNext = pSentinel0;

   return;
}


static ChunkNode_t* helper_list_getHead(ChunkNode_t* pSentinel, int32_t isRemove)
{
   ChunkNode_t* pChunkNode = 0;

    #if 0
    APEXALLOC_ASSERT(pSentinel);
    #endif

    if (!__IS_EMPTY(pSentinel))
    {
        pChunkNode = __HEAD_LIST(pSentinel);

        if (isRemove)
        {
            helper_list_remove(pSentinel, pChunkNode);
        }
    }
    return pChunkNode;
}

static AddrNode_t* helper_list_getHead_addr(AddrNode_t* pSentinel, int32_t isRemove)
{
    AddrNode_t* pAddrNode = 0;

    #if 0
    APEXALLOC_ASSERT(pSentinel);
    #endif

    if (!__IS_EMPTY(pSentinel))
    {
        pAddrNode = __HEAD_LIST(pSentinel);

        if (isRemove)
        {
            helper_list_remove_addr(pSentinel, pAddrNode);
        }
    }
    return pAddrNode;
}



static void __apex_free_addrListElement_init(uint8_t space_id)
{
   int32_t i;

   helper_list_init_addr(gsp_FreeAddrNodeList[space_id]);
   for (i = 0; i < MAX_ADDRSLIST; i++)
   {
      helper_list_addTail_addr(gsp_FreeAddrNodeList[space_id], &(gs_AddrNodes[space_id][i]));
   }
}



static void __apex_free_chunkListElement_init(uint8_t space_id)
{
   int32_t i;

   helper_list_init(gsp_FreeChunkNodeList[space_id]);
   for (i = 0; i < MAX_CHUNKLIST; i++)
   {
      helper_list_addTail(gsp_FreeChunkNodeList[space_id], &(gs_ChunkNodes[space_id][i]));
   }
}



static AddrNode_t *helper_addr_node_alloc_addr(uint8_t space_id)
{
   AddrNode_t* pAddrNode = NULL;

   pAddrNode = (AddrNode_t*)helper_list_getHead_addr(gsp_FreeAddrNodeList[space_id], 1);
    #if 0
    APEXALLOC_ASSERT(pAddrNode);
    #endif

   return pAddrNode;
}



static void helper_addr_node_free_addr(uint8_t space_id, AddrNode_t* pAddrNode)
{
   helper_list_addTail_addr(gsp_FreeAddrNodeList[space_id], pAddrNode);
}



static ChunkNode_t *helper_chunk_node_alloc(uint8_t space_id)
{
   ChunkNode_t* pChunkNode = (ChunkNode_t*)helper_list_getHead(gsp_FreeChunkNodeList[space_id], 1);

   ++gs_num_chunks[space_id];
    #if 0
    APEXALLOC_ASSERT(pChunkNode);
    #endif

   if (pChunkNode) {
      helper_list_init_addr(&(pChunkNode->addr_sentinel));
   }

   return pChunkNode;
}



static void helper_chunk_node_free(uint8_t space_id, ChunkNode_t* pChunkNode)
{
   --gs_num_chunks[space_id];
   helper_list_addTail(gsp_FreeChunkNodeList[space_id], pChunkNode);
}



static int32_t helper_addr_compare(void* key, AddrNode_t* pAddrNode)
{
   return ((int32_t)(int64_t)key) - (int32_t)(int64_t)(pAddrNode->allocated_addr);
}


int64_t apex_allocator_get_total_size(uint8_t space_id)
{
   return (gs__apex_pmem_size[space_id] > INT64_MAX) ? -1 : (int64_t)gs__apex_pmem_size[space_id];
}

int64_t apex_allocator_get_free_size(uint8_t space_id)
{
    #if 0
    GAIN_LOCK;
    #endif

   int64_t freeSizeInBytes = 0;

   const ChunkNode_t* const cpcChunkSentinel = &(gsp_Allocator[space_id]->chunk_sentinel);
   const ChunkNode_t* const cpcChunkBegin    = &(gs_ChunkNodes[space_id][0]);
   const ChunkNode_t* const cpcChunkEnd      = &(gs_ChunkNodes[space_id][MAX_CHUNKLIST]);

   if((cpcChunkSentinel->pNext >= cpcChunkBegin) &&
      (cpcChunkSentinel->pNext <  cpcChunkEnd)   &&
      (cpcChunkSentinel->pPrev >= cpcChunkBegin) &&
      (cpcChunkSentinel->pPrev <  cpcChunkEnd)) // Sentinel should point to valid ChunkNodes
   {
      const ChunkNode_t* pcChunkNode = cpcChunkSentinel->pNext;

      while(pcChunkNode != cpcChunkSentinel)
      {
         if(((pcChunkNode->pNext == cpcChunkSentinel) ||
             ((pcChunkNode->pNext >= cpcChunkBegin) &&
              (pcChunkNode->pNext <  cpcChunkEnd))) &&
            ((pcChunkNode->pPrev == cpcChunkSentinel) ||
             ((pcChunkNode->pPrev >= cpcChunkBegin) &&
              (pcChunkNode->pPrev <  cpcChunkEnd))))
         {
            if(pcChunkNode->is_free == 1)
            {
               freeSizeInBytes += pcChunkNode->size;
            }
            pcChunkNode = pcChunkNode->pNext;
         }
         else
         {
            freeSizeInBytes = -1;
            #if 0
            SHOW_ERROR(0);
            #endif
            break;
         }
      }
   }
   else
   {
      freeSizeInBytes = -1;
        #if 0
        SHOW_ERROR(0);
        #endif
   }

    #if 0
    RELEASE_LOCK;
    #endif

   return freeSizeInBytes;
}

uint64_t apex_allocator_get_physical_base(uint8_t space_id)
{
  return gs__apex_base_pmem_addr[space_id];
}
