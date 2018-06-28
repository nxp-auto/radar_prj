/*
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APEXALLOCATORPRIVATE_H
#define APEXALLOCATORPRIVATE_H

#include "oal_allocator.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct AddrNode
{
   uint64_t    addr;
   uint64_t    size;
   uint32_t    allocated_addr;
   struct AddrNode* pPrev;
   struct AddrNode* pNext;
} AddrNode_t;

typedef struct ChunkNode
{
   uint64_t     addr;
   uint64_t     size;
   bool         is_free;
   AddrNode_t   addr_sentinel;
   struct ChunkNode* pPrev;
   struct ChunkNode* pNext;
} ChunkNode_t;

typedef struct
{
   ChunkNode_t chunk_sentinel;
} Allocator_t;


/* Chunk helpers*/
static void        helper_list_init(ChunkNode_t* pSentinel);
static void        helper_list_addHead(ChunkNode_t* pSentinel, ChunkNode_t* pNodeToBeAdded);
static void        helper_list_addTail(ChunkNode_t* pSentinel, ChunkNode_t* pNodeToBeAdded);
static void        helper_list_remove(ChunkNode_t* pSentinel, ChunkNode_t* pNodeToBeRemoved);
static ChunkNode_t*  helper_list_getHead(ChunkNode_t* pSentinel, int32_t isRemove);
static ChunkNode_t*  helper_chunk_node_alloc(uint8_t space_id);
static void        helper_chunk_node_free(uint8_t space_id, ChunkNode_t* pChunkNode);


/* Address helpers */
static void        helper_list_init_addr(AddrNode_t* pSentinel);
static void        helper_list_addHead_addr(AddrNode_t* pSentinel, AddrNode_t* pNodeToBeAdded);
static void        helper_list_addTail_addr(AddrNode_t* pSentinel, AddrNode_t* pNodeToBeAdded);
static void        helper_list_remove_addr(AddrNode_t* pSentinel, AddrNode_t* pNodeToBeRemoved);
static AddrNode_t* helper_list_find(AddrNode_t* pSentinel, void* pAddress);
static void        helper_list_split(AddrNode_t* pSentinelLeft, AddrNode_t* pSentinelRight, AddrNode_t* pAddrNode);
static void        helper_list_append(AddrNode_t* pSentinel0, AddrNode_t* pSentinel1);
static AddrNode_t* helper_list_getHead_addr(AddrNode_t* pSentinel, int32_t isRemove);
static AddrNode_t* helper_addr_node_alloc_addr(uint8_t space_id);
static void        helper_addr_node_free_addr(uint8_t space_id, AddrNode_t* pAddrNode);
static int32_t     helper_addr_compare(void* key, AddrNode_t* pAddrNode);

#ifdef __cplusplus
}
#endif

#endif /* APEXALLOCATORPRIVATE_H */
