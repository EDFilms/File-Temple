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

#include "stPlatform.h"
#include "stThreads.h"
#include "stList.h"

#include <stdlib.h>
#include "stError.h"

#if ST_CFG_USE_NEDMALLOC == 1

#include "nedmalloc.h"

#define ST_PLATFORM_MALLOC nedmalloc
#define ST_PLATFORM_FREE   nedfree

static s32 nbAllocations = 0;

void  stInitialiseHeapMemory()
{
}

void  stShutdownHeapMemory()
{
}

#if ST_CFG_TRACK_MEMORY == 0
void* stAllocateMemoryFromHeapImpl(size_t nbBytes, size_t alignment)
#else
void* stAllocateMemoryFromHeapImpl(size_t nbBytes, size_t alignment, const char* file, int line, int unused)
#endif
{
#if ST_CFG_TRACK_MEMORY == 1
  ST_UNUSED(file);
  ST_UNUSED(line);
  ST_UNUSED(unused);
  nbAllocations++;
#endif
  //printf("Heap Obtain: %i\n", nbAllocations);
  return nedmalloc2(nbBytes, alignment, 0);
}

void  stReleaseMemoryFromHeapImpl(void* user)
{
  ST_ASSERT(user != NULL, "Cannot free a NULL pointer!");
  nedfree(user);
  // printf("Heap Release: %i\n", nbAllocations);
#if ST_CFG_TRACK_MEMORY == 1
  nbAllocations--;
#endif
}

#if ST_CFG_TRACK_MEMORY == 0
void* stReallocateMemoryFromHeapImpl(void* user, size_t nbBytes)
#else
void* stReallocateMemoryFromHeapImpl(void* user, size_t nbBytes, const char* file, int line, int unused)
#endif
{
#if ST_CFG_TRACK_MEMORY == 1
  ST_UNUSED(file);
  ST_UNUSED(line);
  ST_UNUSED(unused);
#endif
  return nedrealloc(user, nbBytes);
}

ST_PUBLIC_PROBE int stInternal_GetNbMemoryAllocations() 
{ 
  return 0; //nbAllocations;
}

#else

#define ST_PLATFORM_MALLOC malloc
#define ST_PLATFORM_FREE   free

static stMutex sHeapMutex;

struct stAllocationHeaderT
{
  void*       mem;
#if ST_CFG_TRACK_MEMORY == 1
  struct stAllocationHeaderT *next, *prev;
  const char* file;
  int         line;
  int         ref;
  size_t      size;
#endif
};

typedef struct stAllocationHeaderT stAllocationHeader;

#if ST_CFG_TRACK_MEMORY == 1

struct stAllocationsT
{
  stAllocationHeader *first, *last;
  u32     nbAllocations;
  size_t  used, highest;
};

struct stAllocationsT allocations;

#endif

#if (ST_CFG_MALLOC_CONTROLLED_NULL == 1) && (ST_CFG_TRACK_MEMORY == 1)
static size_t stMallocReturnNullWhenTotalSize = ST_MEGABYTES(64);
#endif

ST_PUBLIC_PROBE int stInternal_SetMallocMaxSizeMb(int megaBytes)
{
#if (ST_CFG_MALLOC_CONTROLLED_NULL == 1) && (ST_CFG_TRACK_MEMORY == 1)
  stMallocReturnNullWhenTotalSize = (size_t) ST_MEGABYTES(megaBytes);
#else
  ST_UNUSED(megaBytes);
#endif
  return 0;
}

#if (ST_CFG_MALLOC_CONTROLLED_NULL == 1) && (ST_CFG_TRACK_MEMORY == 1)
void* ST_MALLOC(size_t size)
{
  if (allocations.used >= stMallocReturnNullWhenTotalSize)
    return NULL;
  return ST_PLATFORM_MALLOC(size); // TODO
}
#else
#define ST_MALLOC(SIZE) ST_PLATFORM_MALLOC(SIZE)
#endif

void  stInitialiseHeapMemory()
{
  stMutex_Initialize(&sHeapMutex);
#if ST_CFG_TRACK_MEMORY == 1
  allocations.nbAllocations = 0;
#endif
}
ST_PUBLIC_PROBE int stInternal_PrintMemoryAllocations();

void  stShutdownHeapMemory()
{
  stInternal_PrintMemoryAllocations();
  stMutex_Shutdown(&sHeapMutex);
}

#define ST_PADDING_SIZE(ALIGNMENT) ((ALIGNMENT - 1) + sizeof(stAllocationHeader))

#if ST_CFG_TRACK_MEMORY == 0
void* stAllocateMemoryFromHeapImpl(size_t nbBytes, size_t alignment)
#else
void* stAllocateMemoryFromHeapImpl(size_t nbBytes, size_t alignment, const char* file, int line, int ref)
#endif
{

  u8* mem, *user;

  stMutex_Lock(&sHeapMutex);
  size_t padding = ST_PADDING_SIZE(alignment);
  mem = ST_MALLOC(nbBytes + padding);

  if (mem == NULL)
  {
    stRaiseError(ST_ERROR_MEMORY);
    return NULL;
  }

  user = (u8*)(((size_t) (mem + padding)) & ~(alignment - 1));    // Align forward, taking allowances for padding header.
  stAllocationHeader* header = &((stAllocationHeader*)user)[-1];  // Get pointer to 'header' which is backwards from the user pointer.
  header->mem  = mem;
#if ST_CFG_TRACK_MEMORY == 1
  header->file = file;
  header->line = line;
  header->size = nbBytes;
  header->ref  = ref;
  header->next = NULL;
  header->prev = NULL;

  ST_LIST_ADD((&allocations), header);
  
  allocations.used += (size_t) (nbBytes + padding);
  
  if (allocations.highest < allocations.used)
    allocations.highest = allocations.used;
  
  allocations.nbAllocations++;

#endif

  stMutex_Unlock(&sHeapMutex);

  return user;
}

void  stReleaseMemoryFromHeapImpl(void* user)
{
  ST_ASSERT(user != NULL, "Cannot free a NULL pointer!");

  stMutex_Lock(&sHeapMutex);

  stAllocationHeader* header = &((stAllocationHeader*)user)[-1];  // Get pointer to 'header' which is backwards from the user pointer.

#if ST_CFG_TRACK_MEMORY == 1
  allocations.used -= header->size;
  allocations.nbAllocations--;
  ST_LIST_REMOVE(stAllocationHeader, (&allocations), header);
#endif

  ST_PLATFORM_FREE(header->mem);
  stMutex_Unlock(&sHeapMutex);
}

#if ST_CFG_TRACK_MEMORY == 1

static size_t memSize(void* user)
{
  stAllocationHeader* header = &((stAllocationHeader*)user)[-1];
  return header->size;
}

#endif

#if ST_CFG_TRACK_MEMORY == 0
void* stReallocateMemoryFromHeapImpl(void* user, size_t nbBytes)
#else
void* stReallocateMemoryFromHeapImpl(void* user, size_t nbBytes, const char* file, int line, int ref)
#endif
{
#if ST_CFG_TRACK_MEMORY == 0
    return realloc(user, nbBytes);
#else
    if (user == NULL)
    {
      return stAllocateMemoryFromHeapImpl(nbBytes, ST_DEFAULT_ALIGNMENT, file, line, ref);
    }
    else
    {
      size_t originalSize = memSize(user);
      void* newMem = stAllocateMemoryFromHeapImpl(nbBytes, ST_DEFAULT_ALIGNMENT, file, line, ref);
      memcpy(newMem, user, stMin(originalSize, nbBytes));
      stReleaseMemoryFromHeapImpl(user);
      return newMem;
    }
#endif
}

ST_PUBLIC_PROBE int stInternal_GetNbMemoryAllocations()
{
  // Not thread-safe!
#if ST_CFG_TRACK_MEMORY == 1
  return 0; // allocations.nbAllocations;
#else
  return 0;
#endif
}

ST_PUBLIC_PROBE int stInternal_PrintMemoryAllocations()
{
  // Not thread-safe!
#if ST_CFG_TRACK_MEMORY == 1
  if (allocations.nbAllocations > 0)
  {
    ST_PRINT_INFOF("Memory Allocations [%i]\n===============================", allocations.nbAllocations);
    u32 count = 0; 
    ST_UNUSED(count);

    ST_LIST_FOR_BEGIN(stAllocationHeader, (&allocations), allocation);
    {
      char refStr[4];
      stSprintIntAsFourCC(allocation->ref, refStr);
      ST_PRINT_INFOF("[%i] Source=%s:%i Size=%llu bytes, Ref=%.4s", count++, allocation->file, allocation->line, (u64) allocation->size, refStr);
    }
    ST_LIST_FOR_END(stAllocationHeader, (&allocations), allocation);
    ST_PRINT_INFOF(">> Used: %llu bytes, Highest: %llu bytes", (u64) allocations.used, (u64) allocations.highest);
  }
#endif
  return 0;
}
#endif
