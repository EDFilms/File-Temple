// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stPool.h
 *
 * Memory pool allocator classes and functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_POOL_H
#define ST_POOL_H

#include "stPlatform.h"

struct stFixedPoolT;
struct stPoolBlockT;

/**
 * Next linked-list block for an allocated block of memory
 */
typedef struct stPoolBlockT
{
  struct stPoolBlockT* next;
} stPoolBlock;

/**
 * Fixed size memory pool divided up equally, and managed through linked lists
 */
typedef struct stFixedPoolT
{
  u32   nbBlocks;                   // Number of blocks of size
  u32   size;                       // Size of a block in bytes
  void* mem;                        // Allocated memory and end of allocated memory.
  struct stPoolBlockT* head;        // First available block.
  struct stFixedPoolT* next, *prev; // If used with a dynamic pool, then these are the siblings
  u8*   offset, *offsetEnd;         // If a linear allocator is used with this, then the linearAllocator will use offset.
} stFixedPool;

/**
 * Dynamic memory pool implemented as multiples of stFixedPools
 */
typedef struct stDynamicPoolT
{
  u32          size;
  u32          nbBlocks;
  stFixedPool *first, *last;
} stDynamicPool;

/**
 * Initializes a pool and allocated memory. size and memory must be filled out before initializing
 */
ST_PRIVATE_FUNCTION(Named="stFixedPool_Initialize", Text="Initializes a pool and allocated memory. size and mem must be filled out before initializing", MaybeThreadSafe)
void stFixedPool_Initialize(stFixedPool* pool);

/**
 * Shutdowns a pool and releases allocated memory
 */
ST_PRIVATE_FUNCTION(Named="stFixedPool_Shutdown", Text="Shutdowns a pool and releases allocated memory", MaybeThreadSafe)
void stFixedPool_Shutdown(stFixedPool* pool);

/**
 * Maybe allocate memory based upon the size of the block in the pool, or return NULL
 */
ST_PRIVATE_FUNCTION(Named="stFixedPool_MaybeAllocateMemory", Text="Maybe allocate memory based upon the size of the block in the pool, or return NULL", MaybeNull, MaybeThreadSafe)
void* stFixedPool_MaybeAllocateMemory(stFixedPool* pool);

/**
 * Allocate memory based upon the size of the block in the pool
 */
ST_PRIVATE_FUNCTION(Named="stFixedPool_AllocateMemory", Text="Allocate memory based upon the size of the block in the pool", MaybeNull, MaybeThreadSafe)
void  stFixedPool_ReleaseMemory(stFixedPool* pool, void* mem);

/**
 * Is pointer contains memory in the block of the pool?
 */
ST_PRIVATE_FUNCTION(Named="stFixedPool_ContainsMemory", Text="Is pointer contains memory in the block of the pool", MaybeNull, MaybeThreadSafe)
stBool  stFixedPool_ContainsMemory(stFixedPool* pool, void* mem);

/**
 * Resets the pool but keep the memory
 */
ST_PRIVATE_FUNCTION(Named="stFixedPool_Clear", Text="Resets the pool keeping the memory", MaybeThreadSafe)
void  stFixedPool_Clear(stFixedPool* pool, stBool zeroMem);



/**
 * Initializes a pool and allocated memory. size and memory must be filled out before initializing
 */
ST_PRIVATE_FUNCTION(Named="stDynamicPool_Initialize", Text="Initializes a pool and allocated memory. size and mem must be filled out before initializing", MaybeThreadSafe)
void stDynamicPool_Initialize(stDynamicPool* pool);

/**
 * Shutdowns a pool and releases allocated memory
 */
ST_PRIVATE_FUNCTION(Named="stDynamicPool_Shutdown", Text="Shutdowns a pool and releases allocated memory", MaybeThreadSafe)
void stDynamicPool_Shutdown(stDynamicPool* pool);

/**
 * Maybe allocate memory based upon the size of the block in the pool, or return NULL
 */
ST_PRIVATE_FUNCTION(Named="stDynamicPool_MaybeAllocateMemory", Text="Maybe allocate memory based upon the size of the block in the pool, or return NULL", MaybeNull, MaybeThreadSafe)
void* stDynamicPool_MaybeAllocateMemory(stDynamicPool* pool);

/**
 * Allocate memory based upon the size of the block in the pool
 */
ST_PRIVATE_FUNCTION(Named="stDynamicPool_AllocateMemory", Text="Allocate memory based upon the size of the block in the pool", MaybeNull, MaybeThreadSafe)
void  stDynamicPool_ReleaseMemory(stDynamicPool* pool, void* mem);

/**
 * Allocate memory based upon the size of the block in the pool
 */
ST_PRIVATE_FUNCTION(Named="stDynamicPool_AllocateMemory", Text="Allocate memory based upon the size of the block in the pool", MaybeNull, MaybeThreadSafe)
void  stDynamic(stDynamicPool* pool, void* mem);

/**
 * Resets the pool but keep the memory
 */
ST_PRIVATE_FUNCTION(Named="stDynamicPool_Clear", Text="Resets the pool keeping the memory", MaybeThreadSafe)
void  stDynamicPool_Clear(stDynamicPool* pool, stBool zeroMem);

void stDynamicPool_PrintStats(stDynamicPool* mem);

#endif
