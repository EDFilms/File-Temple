// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stPool.c
 *
 * Memory pool allocator classes and functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stPool.h"
#include "stList.h"

typedef union
{
  stByte*      byte;
  stPoolBlock* block;
} stBlockedListNode;

void stFixedPool_Initialize(stFixedPool* pool)
{
  ST_NULL_CHECK(pool);
  ST_ASSERT(pool->nbBlocks > 0, "Number of blocks must be non-zero");
  ST_ASSERT(pool->size >= sizeof(stPoolBlock), "Block size is to small");
  
  pool->head = NULL;
  pool->mem  = ST_ALLOCATE_MEM(pool->size * pool->nbBlocks, ST_DEFAULT_ALIGNMENT);
  stFixedPool_Clear(pool, true);
}

void stFixedPool_Shutdown(stFixedPool* pool)
{
  ST_NULL_CHECK(pool);
  
  pool->head = NULL;
  ST_RELEASE_MEM(pool->mem);
  pool->mem = NULL;
  pool->mem  = NULL;
}

void* stFixedPool_MaybeAllocateMemory(stFixedPool* pool)
{
  ST_NULL_CHECK(pool);
  
  if (pool->head == NULL)
    return NULL;

  stPoolBlock* head = pool->head;
  pool->head = head->next;

  return (void*) head;
}

void  stFixedPool_ReleaseMemory(stFixedPool* pool, void* mem)
{
  ST_NULL_CHECK(pool);

  stPoolBlock* newHead = (stPoolBlock*) mem;
  newHead->next = pool->head;
  pool->head = newHead->next;
}

stBool  stFixedPool_ContainsMemory(stFixedPool* pool, void* mem)
{
  void* memEnd = (void*) ((stByte*) pool->mem + (pool->nbBlocks * pool->size));
  return mem >= pool->mem && mem < memEnd;
}

void stFixedPool_Clear(stFixedPool* pool, stBool zeroMem)
{
  stBlockedListNode n; 

  if (zeroMem)
  {
    memset(pool->mem, 0, pool->size * pool->nbBlocks);
  }

  // Fill out pool memory as a single linked list.
  n.byte = pool->mem;
  pool->head = n.block;
  n.byte += pool->size;

  stPoolBlock* runner = pool->head;
  for(u32 i=1;i < pool->nbBlocks;i++)
  {
    runner->next = n.block;
    runner = n.block;
    n.byte += pool->size;
  }

  runner->next = NULL;
}

void stDynamicPool_Initialize(stDynamicPool* pool)
{
  ST_NULL_CHECK(pool);

  ST_ASSERT(pool->nbBlocks > 0, "Number of blocks must be non-zero");
  ST_ASSERT(pool->size >= sizeof(stPoolBlock), "Block size is to small");

  pool->first = NULL;
  pool->last  = NULL;
}

static void shutDownFixedPoolFromDynamicPool(stFixedPool* p)
{
  stFixedPool_Shutdown(p);
  ST_DELETE_FROM_HEAP(p);
}

void stDynamicPool_Shutdown(stDynamicPool* pool)
{
  ST_LIST_CLEAR(stFixedPool, pool, shutDownFixedPoolFromDynamicPool);
}

void* stDynamicPool_MaybeAllocateMemory(stDynamicPool* pool)
{
  ST_LIST_FOR_BEGIN(stFixedPool, pool, p);
    void* mem = stFixedPool_MaybeAllocateMemory(p);
    if (mem)
      return mem;
  ST_LIST_FOR_END(stFixedPool, pool, p);

  stFixedPool* p = ST_NEW_FROM_HEAP(stFixedPool);

  if (p == NULL)
  {
    return NULL;
  }

  p->size = pool->size;
  p->nbBlocks = pool->nbBlocks;
  stFixedPool_Initialize(p);
  ST_LIST_ADD(pool, p);

  return stFixedPool_MaybeAllocateMemory(p);
}

void  stDynamicPool_ReleaseMemory(stDynamicPool* pool, void* mem)
{
  ST_LIST_FOR_BEGIN(stFixedPool, pool, p);
    if (stFixedPool_ContainsMemory(p, mem))
    {
      stFixedPool_ReleaseMemory(p, mem);
      return;
    }
  ST_LIST_FOR_END(stFixedPool, pool, p);

  ST_ALWAYS_ASSERT("Unknown memory to pool");
}

void  stDynamicPool_Clear(stDynamicPool* pool, stBool zeroMem)
{ 
  ST_LIST_FOR_BEGIN(stFixedPool, pool, p);
    stFixedPool_Clear(p, zeroMem);
  ST_LIST_FOR_END(stFixedPool, pool, p);
}

void stDynamicPool_PrintStats(stDynamicPool* allocator)
{
  u32 count;
  ST_LIST_COUNT(count, stFixedPool, allocator);

  u32 memSize = count * allocator->size;

  printf("%i Pools, %ib used", count, memSize);
}

