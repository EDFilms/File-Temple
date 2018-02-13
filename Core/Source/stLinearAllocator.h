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

#ifndef ST_LINEAR_ALLOCATOR_H
#define ST_LINEAR_ALLOCATOR_H

#include "stPlatform.h"

#define ST_FIXED_ALLOCATOR_SINGLE_ALLOCATION 1

/**
 * Represents the state of an pre-allocated chunk of (large) memory. Memory
 * is reserved using a stack based system.
 */
typedef struct stFixedLinearAllocatorT
{
  u32   size;                             // Total size of the allocated memory
  u8*  mem;                              // Allocated memory and end of allocated memory.
  struct stFixedLinearAllocatorT *next, *prev; // If used with a dynamic allocator, then these are the siblings
  u8    *current, *end;                   // If current pointer, and end bounds. Current increases per allocation.
  u32  flags;
} stFixedLinearAllocator;

/**
 * Linked-list style Container object for the stFixedLinearAllocator class, to allow an
 * expanding pool of fixed-sized memory blocks.
 */
typedef struct stDynamicLinearAllocatorT
{
  u32 size;
  stFixedLinearAllocator *first, *last;
} stDynamicLinearAllocator;


/**
 * Initialise a fixed linear allocator
 *
 * Preset size in allocator pointer to set memory size.
 */
void  stFixedLinearAllocator_Initialise(stFixedLinearAllocator* allocator);

/**
 * Shutdown a fixed linear allocator
 */
void  stFixedLinearAllocator_Shutdown(stFixedLinearAllocator* allocator);

/**
 * Try and obtain nbBytes from the linear allocator.
 *
 * If the allocator is full or nbBytes to too large, NULL is returned.
 * @return a u8 pointer to the allocated memory or NULL.
 */
u8*  stFixedLinearAllocator_MaybeObtain(stFixedLinearAllocator* allocator, u32 nbBytes);

/**
 * Reset the state of the linear allocator, but do not free or clear the memory.
 */
void stFixedLinearAllocator_Clear(stFixedLinearAllocator* allocator);

/**
 * Get the number of bytes used by this linear allocator
 */
size_t stFixedLinearAllocator_Used(stFixedLinearAllocator* buffer);

/**
 * Initialises a dynamic linear allocator
 *
 * Preset size in allocator pointer to set linear allocator memory size.
 */
void stDynamicLinearAllocator_Initialise(stDynamicLinearAllocator* allocator);

/**
 * Shuts down a dynamic linear allocator. All linear allocators are deleted and all memory is freed.
 */
void stDynamicLinearAllocator_Shutdown(stDynamicLinearAllocator* allocator);


/**
 * Try and obtain nbBytes from the linear allocator.
 *
 * If the allocator is full then a new allocator (therefore chunk of memory is allocator) is created.
 * 
 * NULL is only returned if the nbBytes is larger than the maximum size of allocations
 * 
 * @return a u8 pointer to the allocated memory or NULL.
 */
u8*  stDynamicLinearAllocator_Obtain(stDynamicLinearAllocator* allocator, u32 nbBytes);

/**
 * Like stDynamicLinearAllocator_Obtain but with console output debugging
 * @deprecated
 */
u8*  stDynamicLinearAllocator_Obtain2(stDynamicLinearAllocator* allocator, u32 nbBytes);

/**
 * Clear all allocations but does not delete linear allocators or memory.
 */
void stDynamicLinearAllocator_Clear(stDynamicLinearAllocator* allocator);

/**
 * Tries to pre-allocates number of allocators to the meet the minimum pool count
 */
void stDynamicLinearAllocator_Reserve(stDynamicLinearAllocator* allocator, u32 minPoolCount);

/**
 * Prints to the log - the number of linear allocators and total memory used by them.
 */
void stDynamicLinearAllocator_PrintStats(stDynamicLinearAllocator* mem);

/**
 * Transfers ownership and state from linear allocators to another
 */
void stDynamicLinearAllocator_Move(stDynamicLinearAllocator* dst, stDynamicLinearAllocator* src);

#define ST_DYNAMIC_LINEAR_ALLOCATOR_INITIALISE(ALLOCATOR, TYPE, SIZE_PER_POOL) do { \
                                                                                (ALLOCATOR).size  = sizeof(TYPE) * SIZE_PER_POOL; \
                                                                                (ALLOCATOR).first = NULL; \
                                                                                (ALLOCATOR).last  = NULL; \
                                                                               } while(0)

#define ST_DYNAMIC_LINEAR_ALLOCATOR_OBTAIN(TYPE, ALLOCATOR)  ((TYPE*) stDynamicLinearAllocator_Obtain((ALLOCATOR), sizeof(TYPE)))

#endif
