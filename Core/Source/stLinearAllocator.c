// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stLinearAllocator.h
 *
 * Stack-like memory pool functions and objects
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stLinearAllocator.h"
#include "stList.h"

#define ST_CHECK_LINEAR_ALLOCATOR_SIZES

// stFixedLinearAllocator when used with dynamic allocators. Either
//  (0) Allocate two sets of mem (stFixedAllocator and bytes of size)
//  (1) Allocate one set of mem (stFixedAllocator + bytes of size), where they are shared.

#define ST_FIXED_DYNAMIC_ALLOCATORS_SHARE_MEM 1

stFixedLinearAllocator* allocateFixedAllocatorForDynamicAllocator(u32 size)
{
  size_t totalSize = sizeof(stFixedLinearAllocator) + size;
  u8* mem = (u8*) ST_ALLOCATE_MEM(totalSize, ST_DEFAULT_ALIGNMENT);
  
  if (mem == NULL)
  {
    return NULL;
  }

  memset(mem, 0, totalSize);
  stFixedLinearAllocator* allocator = (stFixedLinearAllocator*) mem;
  allocator->mem = mem + sizeof(stFixedLinearAllocator);

  allocator->flags = ST_FIXED_ALLOCATOR_SINGLE_ALLOCATION;
  allocator->size  = size;
  allocator->next  = NULL;
  allocator->prev  = NULL;

  stFixedLinearAllocator_Clear(allocator);

  return allocator;
}

void  stFixedLinearAllocator_Initialise(stFixedLinearAllocator* allocator)
{
  ST_NULL_CHECK(allocator);
  ST_ASSERT(allocator->size >= 4, "Allocator size to small");

  allocator->mem   = (u8*) ST_ALLOCATE_MEM(allocator->size, ST_DEFAULT_ALIGNMENT);

  if (allocator->mem != NULL)
  {
    memset(allocator->mem, 0, allocator->size);
    allocator->next  = NULL;
    allocator->prev  = NULL;
    allocator->flags = 0;

    stFixedLinearAllocator_Clear(allocator);
  }
}

void  stFixedLinearAllocator_Shutdown(stFixedLinearAllocator* allocator)
{
  ST_NULL_CHECK(allocator);
  ST_NULL_CHECK(allocator->mem);
  ST_RELEASE_MEM(allocator->mem);
  allocator->mem = NULL;
}

u8*  stFixedLinearAllocator_MaybeObtain(stFixedLinearAllocator* allocator, u32 nbBytes)
{
  ST_NULL_CHECK(allocator);
  ST_NULL_CHECK(allocator->current);

  u8* end = allocator->current + nbBytes;
  if (end >= allocator->end)
      return NULL;
  
  u8* mem = allocator->current;
  allocator->current += nbBytes;
  
  return mem;
}

void stFixedLinearAllocator_Clear(stFixedLinearAllocator* allocator)
{
  ST_NULL_CHECK(allocator);
  ST_NULL_CHECK(allocator->mem);

  allocator->current = allocator->mem;
  allocator->end     = allocator->current + allocator->size;
}

size_t stFixedLinearAllocator_Used(stFixedLinearAllocator* allocator)
{

  ST_NULL_CHECK(allocator);
  ST_NULL_CHECK(allocator->mem);

  return (allocator->current - allocator->mem);
}
void  stDynamicLinearAllocator_Initialise(stDynamicLinearAllocator* allocator)
{
  ST_NULL_CHECK(allocator);
  ST_ASSERT(allocator->size >= 4, "Allocator size to small");

  allocator->first  = NULL;
  allocator->last   = NULL;
}

void  stDynamicLinearAllocator_Shutdown(stDynamicLinearAllocator* allocator)
{
  ST_LIST_FOR_BEGIN(stFixedLinearAllocator, allocator, a);

  if (ST_HAS_FLAG(a->flags, ST_FIXED_ALLOCATOR_SINGLE_ALLOCATION))
  {
    ST_RELEASE_MEM(a);
  }
  else
  {
    stFixedLinearAllocator_Shutdown(a);
    ST_DELETE_FROM_HEAP(a);
  }

  ST_LIST_FOR_END(stFixedLinearAllocator, allocator, a);

  allocator->first  = NULL;
  allocator->last   = NULL;
}

u8*  stDynamicLinearAllocator_Obtain(stDynamicLinearAllocator* allocator, u32 nbBytes)
{

  ST_LIST_FOR_BEGIN(stFixedLinearAllocator, allocator, a);
    u8* mem = stFixedLinearAllocator_MaybeObtain(a, nbBytes);
    if (mem)
        return mem;
  ST_LIST_FOR_END(stFixedLinearAllocator, allocator, a);


#if (ST_FIXED_DYNAMIC_ALLOCATORS_SHARE_MEM == 0)

  stFixedLinearAllocator* newAllocator = ST_NEW_FROM_HEAP(stFixedLinearAllocator);
  newAllocator->size = allocator->size;
  stFixedLinearAllocator_Initialise(newAllocator);
  ST_LIST_ADD(allocator, newAllocator);

  return stFixedLinearAllocator_MaybeObtain(newAllocator, nbBytes);
#else

  stFixedLinearAllocator* newAllocator = allocateFixedAllocatorForDynamicAllocator(allocator->size);
  
  if (newAllocator != NULL)
  {
    ST_LIST_ADD(allocator, newAllocator);
    return stFixedLinearAllocator_MaybeObtain(newAllocator, nbBytes);
  }

  return NULL;

#endif
}

u8*  stDynamicLinearAllocator_Obtain2(stDynamicLinearAllocator* allocator, u32 nbBytes)
{

  ST_LIST_FOR_BEGIN(stFixedLinearAllocator, allocator, a);
  u8* mem = stFixedLinearAllocator_MaybeObtain(a, nbBytes);
  if (mem)
  {
    printf("Got Allocator: need %i\n", nbBytes);
    return mem;
  }
  ST_LIST_FOR_END(stFixedLinearAllocator, allocator, a);


#if (ST_FIXED_DYNAMIC_ALLOCATORS_SHARE_MEM == 0)

  stFixedLinearAllocator* newAllocator = ST_NEW_FROM_HEAP(stFixedLinearAllocator);
  newAllocator->size = allocator->size;
  stFixedLinearAllocator_Initialise(newAllocator);
  ST_LIST_ADD(allocator, newAllocator);

  return stFixedLinearAllocator_MaybeObtain(newAllocator, nbBytes);
#else

  stFixedLinearAllocator* newAllocator = allocateFixedAllocatorForDynamicAllocator(allocator->size);

  if (newAllocator != NULL)
  {

    printf("New Allocator: need %i\n", nbBytes);
    ST_LIST_ADD(allocator, newAllocator);
    return stFixedLinearAllocator_MaybeObtain(newAllocator, nbBytes);
  }

  return NULL;

#endif
}
void stDynamicLinearAllocator_Clear(stDynamicLinearAllocator* allocator)
{
  ST_LIST_FOR_BEGIN(stFixedLinearAllocator, allocator, a);
    stFixedLinearAllocator_Clear(a);
  ST_LIST_FOR_END(stFixedLinearAllocator, allocator, a);
}

#if ST_CFG_DATA_PROBE == 1

#include "stProbe.h"

ST_PUBLIC_PROBE void stProbe_FixedLinearAllocator(const char* name, stFixedLinearAllocator* allocator)
{
  if (name != NULL)
    stProbe_BeginWithName(name);
  else
    stProbe_Begin();

  stProbe_KeyPointer("address", allocator);

  if (allocator != NULL)
  {
    stProbe_KeyInt("size", allocator->size);
    stProbe_KeyPointer("mem", allocator->mem);
    stProbe_KeyPointer("current", allocator->current);
    stProbe_KeyPointer("end", allocator->end);
    stProbe_KeyInt("used",allocator->size - (allocator->end - allocator->current));
    stProbe_KeyInt("free", (allocator->end - allocator->current));
    stProbe_KeyPointer("next", allocator->next);
    stProbe_KeyPointer("prev", allocator->prev);
  }

  stProbe_End();
}

ST_PUBLIC_PROBE void stProbe_DynamicLinearAllocator(const char* name, stDynamicLinearAllocator* allocator)
{
  if (name != NULL)
    stProbe_BeginWithName(name);
  else
    stProbe_Begin();

  stProbe_KeyPointer("address", allocator);

  if (allocator != NULL)
  {
    stProbe_KeyInt("size", allocator->size);
    u32 count;
    ST_LIST_COUNT(count, stFixedLinearAllocator, allocator);
    stProbe_KeyInt("count", count);

    stProbe_KeyPointer("first", allocator->first);
    stProbe_KeyPointer("last", allocator->last);

    stProbe_BeginListWithName("allocators");
      ST_LIST_FOR_BEGIN(stFixedLinearAllocator, allocator, a);
        stProbe_FixedLinearAllocator(NULL, a);
      ST_LIST_FOR_END(stFixedLinearAllocator, allocator, a);
    stProbe_EndList();
  }

  stProbe_End();
}

void stDynamicLinearAllocator_Reserve(stDynamicLinearAllocator* allocator, u32 minPoolCount)
{
  if (minPoolCount == 0)
    return;

  u32 count;
  ST_LIST_COUNT(count, stFixedLinearAllocator, allocator);

  if (count >= minPoolCount)
    return;

  u32 diff = minPoolCount - count;

  for(u32 i=0;i < diff;i++)
  {
    stFixedLinearAllocator* newAllocator = ST_NEW_FROM_HEAP(stFixedLinearAllocator);

    if (newAllocator == NULL)
    {
      return;
    }

    newAllocator->size = allocator->size;
    stFixedLinearAllocator_Initialise(newAllocator);
    ST_LIST_ADD(allocator, newAllocator);
  }

}

void stDynamicLinearAllocator_PrintStats(stDynamicLinearAllocator* allocator)
{
  u32 count;
  ST_LIST_COUNT(count, stFixedLinearAllocator, allocator);

  u32 memSize = count * allocator->size;
  ST_UNUSED(memSize);
  ST_PRINT_CHATTERF("%i Allocators, %ib/%iKb used", count, memSize, count / 1024);
}

void stDynamicLinearAllocator_Move(stDynamicLinearAllocator* dst, stDynamicLinearAllocator* src)
{
  dst->first = src->first;
  dst->last  = src->last;
  dst->size  = src->size;

  src->first = NULL;
  src->last  = NULL;
  src->size  = 0;
}

#endif
