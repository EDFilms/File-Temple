// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_LINEAR_ALLOCATOR_H
#define ST_LINEAR_ALLOCATOR_H

#include "stPlatform.h"

#define ST_FIXED_ALLOCATOR_SINGLE_ALLOCATION 1

typedef struct stFixedLinearAllocatorT
{
  u32   size;                             // Total size of the allocated memory
  u8*  mem;                              // Allocated memory and end of allocated memory.
  struct stFixedLinearAllocatorT *next, *prev; // If used with a dynamic allocator, then these are the siblings
  u8    *current, *end;                   // If current pointer, and end bounds. Current increases per allocation.
  u32  flags;
} stFixedLinearAllocator;

typedef struct stDynamicLinearAllocatorT
{
  u32 size;
  stFixedLinearAllocator *first, *last;
} stDynamicLinearAllocator;

void  stFixedLinearAllocator_Initialise(stFixedLinearAllocator* allocator);

void  stFixedLinearAllocator_Shutdown(stFixedLinearAllocator* allocator);

u8*  stFixedLinearAllocator_MaybeObtain(stFixedLinearAllocator* allocator, u32 nbBytes);

void stFixedLinearAllocator_Clear(stFixedLinearAllocator* allocator);

size_t stFixedLinearAllocator_Used(stFixedLinearAllocator* buffer);

void stDynamicLinearAllocator_Initialise(stDynamicLinearAllocator* allocator);

void stDynamicLinearAllocator_Shutdown(stDynamicLinearAllocator* allocator);

u8*  stDynamicLinearAllocator_Obtain(stDynamicLinearAllocator* allocator, u32 nbBytes);

u8*  stDynamicLinearAllocator_Obtain2(stDynamicLinearAllocator* allocator, u32 nbBytes);

void stDynamicLinearAllocator_Clear(stDynamicLinearAllocator* allocator);

void stDynamicLinearAllocator_Reserve(stDynamicLinearAllocator* allocator, u32 minPoolCount);

void stDynamicLinearAllocator_PrintStats(stDynamicLinearAllocator* mem);

void stDynamicLinearAllocator_Move(stDynamicLinearAllocator* dst, stDynamicLinearAllocator* src);

#define ST_DYNAMIC_LINEAR_ALLOCATOR_INITIALISE(ALLOCATOR, TYPE, SIZE_PER_POOL) do { \
                                                                                (ALLOCATOR).size  = sizeof(TYPE) * SIZE_PER_POOL; \
                                                                                (ALLOCATOR).first = NULL; \
                                                                                (ALLOCATOR).last  = NULL; \
                                                                               } while(0)

#define ST_DYNAMIC_LINEAR_ALLOCATOR_OBTAIN(TYPE, ALLOCATOR)  ((TYPE*) stDynamicLinearAllocator_Obtain((ALLOCATOR), sizeof(TYPE)))

#endif
