/*
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_MEMORY_ALLOCATOR_H
#define OAL_MEMORY_ALLOCATOR_H

#include <oal_utils.h>

__BEGIN_DECLS

#define OAL_SUCCESS 0
#define OAL_FAILURE -1
typedef enum {
	OAL_SEM_ENOMEM     = 0x90006,
	OAL_SEM_EBADSEM    = 0x90004,
	OAL_SEM_ESEMLOCKED = 0x90002,
	OAL_SEM_ETIMEOUT   = 0x90003,
	OAL_SEM_EBADTYPE   = 0x90005
} OAL_RESULT;

/**
 * @defgroup OAL_MemoryAllocator OAL Memory Allocator
 * @brief Contiguous memory allocator
 *
 * @{
 * @name Memory functions
 * @details
 *
 *  This API allows dynamically allocating a block of memory described by a
 *  pointer whose address component points to a virtual address interpreted
 *  by the MMU. MMU configures various regions as cache-able,
 *  non-cacheable, etc. and handles the mapping of this virtual address
 *  to its physical location and other properties associated with the
 *  caching mechanisms that are available.
 *
 *  Application code running on the ARM should not care about the
 *  virtualness of any pointers and considers that property transparent to
 *  normal operation. There are certain low level operations that can
 *  optimized by accessing memory in different way. MMU regions allow the
 *  same physical memory location to be mapped with different
 *  properties. It is possible using the #OAL_GetReturnAddress() API to
 *  retrieve an address to have the appropriate properties that are handy
 *  for the current use case. If the subsequent are going to be executed by
 *  the hardware, one of the <I>Cache Control APIs</I> (e.g. #OAL_FlushMemory())
 *  can be used to make sure that the cache coherency is maintained.
 *
 *  One example is when writing to the registers of a device, we don't want
 *  to use a cacheable virtual pointer because data won't be written to the
 *  device. In this case using the non-cacheable virtual pointer is the only
 *  way.
 *
 *  Additionally, for devices that are able to communicate with each or
 *  read/write to memory on their own, the only possible value that they can
 *  be programmed with is the physical location of that memory. It is
 *  possible to retrieve the physical address associate with a virtual
 *  pointer to configure the parameters of a device. Physical addresses
 *  should only be used in such cases.
 *
 *  Mandatory function call ordering sequence for allocations:
 *
 *  <ol>
 *     <li>#OAL_AllocMemory</li>
 *     <li>optionally: #OAL_GetReturnAddress() [#OAL_ACCESS_PHY]</li>
 *     <li>manipulate data</li>
 *     <li>optionally: #OAL_FlushAndInvalidateMemory</li>
 *     <li>#OAL_FreeMemory</li>
 *  </ol>
 *
 * @par Differences between \ref OAL_AllocMemory and \ref OAL_AllocAndMapMemory
 *
 *   @anchor Differences
 *   Both functions allow memory allocation, but in a slighlty different way.
 *   \ref OAL_AllocAndMapMemory will not only allocate the memory, but will also
 *   map it using the selected cache policy.
 *
 *   \ref OAL_AllocMemory is targeted for allocations which are multiple
 *   of a page (\e PAGE_SIZE) in size. The size of a page is dependent on the
 *   underlying Operating System. It is usually set to 4KB of memory.
 *
 *   Using this function for small chunks, under a page or for buffers of which
 *   size isn't a multiple of a page will lead to fragmentation, due to the fact
 *   that it will round up the size to the next \e PAGE_SIZE multiple.
 *
 *   The fragmentation can be reduced if using \ref OAL_AllocAndMapMemory. It
 *   will use all the gaps from the previous allocations with the same cache
 *   policy.
 *
 *   Examples:
 *   * Below code will use two pages, one per each allocation, instead of
 *   reusing the leftover space from the first allocation.
 *   @code
 *     OAL_AllocMemory(8, OAL_MEMORY_FLAG_CONTIGUOUS);
 *     OAL_AllocMemory(8, OAL_MEMORY_FLAG_CONTIGUOUS);
 *   @endcode
 *   * Same applies in this case, because we are allocating with different cache
 *   policies
 *   @code
 *     OAL_AllocAndMapMemory(8, OAL_MEMORY_FLAG_CONTIGUOUS, OAL_ACCESS_CH_WB);
 *     OAL_AllocAndMapMemory(8, OAL_MEMORY_FLAG_CONTIGUOUS, OAL_ACCESS_NCH_NB);
 *   @endcode
 *   * Allocating in the same page:
 *   @code
 *     OAL_AllocAndMapMemory(8, OAL_MEMORY_FLAG_CONTIGUOUS, OAL_ACCESS_CH_WB);
 *     OAL_AllocAndMapMemory(8, OAL_MEMORY_FLAG_CONTIGUOUS, OAL_ACCESS_CH_WB);
 *     OAL_AllocAndMapMemory(8, OAL_MEMORY_FLAG_CONTIGUOUS, OAL_ACCESS_CH_WB);
 *     OAL_AllocAndMapMemory(8, OAL_MEMORY_FLAG_CONTIGUOUS, OAL_ACCESS_CH_WB);
 *     OAL_AllocAndMapMemory(8, OAL_MEMORY_FLAG_CONTIGUOUS, OAL_ACCESS_CH_WB);
 *   @endcode
 *
 **/

///< VISION assignment vs RSDK assignment for Linux
#define OAL_CHUNK_ID_DDR0 0x01  ///< memory region in DDR0 / DDR
#define OAL_CHUNK_ID_DDR1 0x02  ///< memory region in DDR1 / SRAM
#define OAL_CHUNK_ID_SRAM_SINGLE                                               \
	0x03  ///< memory region in SRAM 3MB section / specific SPT
#define OAL_CHUNK_ID_SRAM_MULTI                                                \
	0x04  ///< memory region in SRAM 1MB section / specific DSP/BBE
#define OAL_CHUNK_ID_CB_RAM                                                    \
	0x05  ///< memory region in not yet defined / specific CB-RAM memory
#define OAL_CHUNK_ID_EXTENSION                                                 \
	0x06  ///< memory region in not yet defined / reserved for possible future use
#define OAL_CHUNK_ID_AUTO 0x00  ///< up to OAL to decide

#define OAL_CHUNK_ID_MAXNR                                                     \
	OAL_CHUNK_ID_EXTENSION /* maximum CHUNK_ID number                     \
	                           */

/**
 * @brief After the memory is allocated, that memory can be accessed in various
 * ways. Depending on how the user is manipulating the data in the memory
 * space, at certain points in time different accesses may be required.
 * Use #OAL_GetReturnAddress() to get the address that is coupled with
 * the requested access type.
 *
 **/
///< Physical access
#define OAL_ACCESS_PHY (0U)
///< Cached access with Writeback policy
#define OAL_ACCESS_CH_WB (1U)
///< Cached access with Writethrough policy
#define OAL_ACCESS_CH_WT (2U)
///< Non-Cached Bufferable access
#define OAL_ACCESS_NCH_B (3U)
///< Non-Cached Non-Bufferable Access
#define OAL_ACCESS_NCH_NB (4U)
#define OAL_ACCESS_NUM (5U)

typedef uint32_t OAL_MEMORY_ACCESS;
typedef uint32_t OAL_MEMORY_FLAG;

///< Memory is aligned on a cache line
#define OAL_ALIGN2_CACHELINE (0x00000100U)
///< Memory is aligned on a page
#define OAL_ALIGN2_PAGE (0x00000200U)
#define OAL_BYTE_N (0x00000400U)
///< Memory is aligned on an N-byte boundary
#define OAL_ALIGN2_BYTE(N) (OAL_BYTE_N | (N << 16U))
///< Memory block is physically contiguous
#define OAL_MEMORY_FLAG_CONTIGUOUS (0x01U)
///< Memory allocated will never be swapped
#define OAL_MEMORY_FLAG_NONSWAPABLE (0x02U)
///< Memory is initialized with value zero
#define OAL_MEMORY_FLAG_ZERO (0x04U)
///< Memory is aligned as specified
#define OAL_MEMORY_FLAG_ALIGN(ALIGN2) (0x08U | (ALIGN2))

/**
 * @brief Memory will be allocated on DDR0 - fails if DDR0 allocations
 * are at maximum
 * @deprecated
 */
enum {
	///< Memory will be allocated on DDR0 - fails if DDR0 allocations are at
	/// maximum
	OAL_ALLOC_DDR0 = (OAL_CHUNK_ID_DDR0 << 24U),
	///< Memory will be allocated on DDR1 - fails if DDR1 allocations are at
	/// maximum
	OAL_ALLOC_DDR1 = (OAL_CHUNK_ID_DDR1 << 24U),
	///< Memory will be allocated on SRAM - fails if SRAM allocations are at
	/// maximum
	OAL_ALLOC_SRAM_SINGLE = (OAL_CHUNK_ID_SRAM_SINGLE << 24U),
	///< Memory will be allocated on SRAM - fails if SRAM allocations are at
	/// maximum
	OAL_ALLOC_SRAM_MULTI = (OAL_CHUNK_ID_SRAM_MULTI << 24U),
	///< Memory will be allocated on CB-RAM (RSDK) - fails if CB-RAM
	/// allocations are at maximum
	OAL_ALLOC_CB_RAM_SPECIFIC = (OAL_CHUNK_ID_CB_RAM << 24U),
	///< This is defined for an easy future extension - fails if allocations
	/// are at maximum
	OAL_ALLOC_EXTENSION_MEM = (OAL_CHUNK_ID_EXTENSION << 24U),
	///< Memory will be allcated on chunk with the least allocations
	///< fails when no suitable chunk is found
	OAL_ALLOC_AUTO = (OAL_CHUNK_ID_AUTO << 24U)
};

struct OAL_MemoryHandle;

/**
 * @brief Allocate a block of memory Athat meets certain criteria.
 * Memory will be allocated from the appropriate heap while satisfying all the
 * properties of flags. Memory allocation of specified size will allocate a
 * block of size and return a memory handle.
 *
 * @warning Suitable for allocations of which size is multiple of a
 * \e PAGE_SIZE. For the rest of the cases use \ref OAL_AllocAndMapMemory.
 *
 * @param[in] aSize  Amount of memory to allocate in bytes
 * @param[in] aFlags Memory flags
 *
 * @return
 *     - On Success: OAL Memory handle
 *     - On Failure: NULL
 * @see \ref Differences
 */
struct OAL_MemoryHandle *OAL_AllocMemory(uint32_t aSize,
                                         OAL_MEMORY_FLAG aFlags);

/**
 * @brief Allocate a block of memory and map it based on requested
 * cache policy.
 *
 * @param[in] aSize   Amount of memory to allocate in bytes
 * @param[in] aFlags  Memory flags
 * @param[in] aAccess Access type requested. Allowed values:
 *  the CPU will fail.
 *  - #OAL_ACCESS_CH_WB : Cacheable memory with a write back policy:
 *  - #OAL_ACCESS_CH_WT : Cacheable memory with a  write through policy
 *  - #OAL_ACCESS_NCH_B : Non-Cacheable memory but buffered
 *  - #OAL_ACCESS_NCH_NB: Non-Cacheable memory and not buffered
 * @return
 *     - On Success: OAL Memory handle
 *     - On Failure: NULL
 * @see \ref Differences
 * @see \ref OAL_AllocMemory
 * @see \ref OAL_GetReturnAddress
 */
struct OAL_MemoryHandle *OAL_AllocAndMapMemory(uint32_t aSize,
                                               OAL_MEMORY_FLAG aFlags,
                                               OAL_MEMORY_ACCESS aAccess);
/**
 * @brief Release the allocated memory and return it to memory manager.
 *
 * @param[in] apMemHandle Handle of the allocated memory block; it must be a
 * pointer returned by #OAL_AllocMemory() or variations.
 *
 * @return
 *     - On Success: 0
 *     - On Failure: negative value
 */
int32_t OAL_FreeMemory(struct OAL_MemoryHandle *apMemHandle);

/**
 * @brief Check if <tt>acpHandle</tt> is a valid OAL allocated memory handle
 *
 * @param[in] acpHandle A memory handle
 *
 * @return
 *     - <tt>0</tt> for invalid handle
 *     - <tt>1</tt> for valid handle
 */
uint8_t OAL_CheckAllocated(const void *acpHandle);

/**
 * @brief Get physical address or map a memory handle
 * @details
 *  Takes the given memory pointer and returns the pointer
 *  that allows proper access as identified in the access parameter
 *
 *  The value of \p apMemHandle passed has to be the value returned from one
 *  of the #OAL_AllocMemory functions.
 *
 *  - #OAL_ACCESS_PHY   : Returns the physical address of memory. Accessing this
 *via
 *  the CPU will fail.
 *  - #OAL_ACCESS_CH_WB : Returns the address that will be interpreted as cached
 *  with a  write back policy.
 *  - #OAL_ACCESS_CH_WT : Returns the address that will be interpreted as cached
 *  with a  write through policy.
 *  - #OAL_ACCESS_NCH_B : Returns the address that will be interpreted as
 *non-cached
 *  but buffered.
 *  - #OAL_ACCESS_NCH_NB: Returns the address that will be interpreted as
 *non-cached
 *  and not buffered.
 *
 * @param[in] apMemHandle Pointer to a memory buffer; it must be a pointer
 * returned by #OAL_AllocMemory() or variations.
 * @param[in] aAccess Access Type requested
 *
 * @return
 *         Address of buffer with appropriate access type
 *
 *  \warning User is responsible from appropriately handling
 *  coherency issues. This function is just going to generate an address.
 *  It will not attempt to resolve the coherency.
 **/
uintptr_t OAL_GetReturnAddress(struct OAL_MemoryHandle *apMemHandle,
                               OAL_MEMORY_ACCESS aAccess);

/**
 * @brief Unmap virtual space mapping of the allocated buffer.
 * @details The cache is not flushed when called, must be done outside this
 *  function if necessary.
 *
 * @param[in] apMemHandle Start address of the allocated memory block;
 * it must be a pointer returned by #OAL_AllocMemory() or variations.
 *
 * @return
 *     - On Success: Zero
 *     - On Failure: Negative
 */
int32_t OAL_UnmapMemory(struct OAL_MemoryHandle *apMemHandle);

/**
 * @name Cache Control Functions
 *
 *  These functions operate on a memory region specified by the user.
 *  There are no restrictions on the pointer that may be passed. They
 *  find if any part of the region specified by pAddr and size are
 *  currently held in the cache and operate only on those cache lines.
 *
 *  The pointer passed Must be returned by #OAL_AllocMemory() or
 *  its variations.
 *
 *  \warning The flush & invalidate functions are not thread safe
 *           for speed purposes. The user needs to ensure the thread
 *           & process safety outside those function.
 *
 *  Flush: If any region of this memory is currently held in
 *  the CPU cache, those contents will be written back to
 *  physical memory.
 *
 *  FlushAndInvalidate: On top of the flush operations, all the
 *  cache lines will be marked invalid. This will require
 *  a subsequent read operation to fetch data from physical memory.
 *
 *  Invalidate: Simply scratches out all the cache lines.
 *  Data kept in the cache is lost. Any subsequent read will fetch
 *  the data from physical memory
 *@{
 **/

/**
 * @brief Flush a memory region
 *
 * @param[in] aAddr  Start Address of the memory region to be flushed
 * @param[in] aSize Size of the memory region
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int32_t OAL_FlushMemory(uintptr_t aAddr, uint32_t aSize);

/**
 * @brief Invalidate caches for a memory region
 *
 * @param[in] aAddr Start Address of the memory region to be invalidated
 * @param[in] aSize  Size of the memory region
 *
 *  \warning On A53 core a flush operation may be performed before invalidation
 *           if the given range contains dirty data.
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int32_t OAL_InvalidateMemory(uintptr_t aAddr, uint32_t aSize);

/**
 * @brief Flush and invalidate caches for a memory region
 *
 * @param[in] aAddr Start Address of the memory region to be flushed and
 * invalidated
 * @param[in] aSize Size of the memory region
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int32_t OAL_FlushAndInvalidateMemory(uintptr_t aAddr, uint32_t aSize);

/**
 * @brief Invalidate all virtual ranges allocated using OAL. It will iterate
 * over all memory handles and perform and invalidate operations.
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int32_t OAL_FlushAndInvalidateAll(void);

/** @} */

/** @} */

__END_DECLS

#endif

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_memory_allocator.h>
#endif
