// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stObjectData.c
 *
 * Manages memory and classes for changes of object's component-data/"cold" data during a frame.
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stObjectData.h"
#include "stList.h"
#include "stTypes.h"
#include "stReaderDelta.h"

void stObjectDatas_Initialise(struct stObjectDatasT* objectDatas)
{
  ST_DYNAMIC_LINEAR_ALLOCATOR_INITIALISE(objectDatas->mem, struct stObjectDataT, 16);
  objectDatas->first = NULL;
  objectDatas->last  = NULL;
  objectDatas->nextId = 1;
}

void stObjectDatas_Shutdown(struct stObjectDatasT* objectDatas)
{
  ST_LIST_FOR_BEGIN(struct stObjectDataT, objectDatas, objectData);
  {
    stObjectData_Release(objectData);
  }
  ST_LIST_FOR_END(struct stObjectDataT, objectDatas, objectData);

  stDynamicLinearAllocator_Shutdown(&objectDatas->mem);
  objectDatas->first = NULL;
  objectDatas->last  = NULL;
  objectDatas->nextId = 1;
}

struct stObjectDataT* stObjectData_Obtain(struct stObjectDatasT* objectDatas, u8 type, u8 nbElements, u32 arraySize)
{
  u32 id = objectDatas->nextId++;
  u32 memSize = stGetTypeMemorySize(type, nbElements, arraySize);

  if (memSize == 0)
    return NULL;

  struct stObjectDataT* objectData = ST_DYNAMIC_LINEAR_ALLOCATOR_OBTAIN(struct stObjectDataT, (&objectDatas->mem));
  
  objectData->id          = id;
  objectData->next        = NULL;
  objectData->prev        = NULL;
  objectData->memSize     = memSize;
  objectData->type        = type;
  objectData->nbElements  = nbElements;
  objectData->arraySize   = arraySize;
  objectData->data        = ST_ALLOCATE_MEM(memSize, ST_DEFAULT_ALIGNMENT);
  objectData->bLoaded     = true;
  objectData->bPersistant = true;
  objectData->bModified   = true;
  objectData->bDeleted    = false;
  objectData->bAllocated  = true;

  ST_LIST_ADD(objectDatas, objectData);

  return objectData;
}

struct stObjectDataT* stObjectData_Obtain_ForReader(struct stObjectDatasT* objectDatas, u32 id)
{
  if (id > objectDatas->nextId)
  {
    objectDatas->nextId = id + 1;
  }

  struct stObjectDataT* objectData = ST_DYNAMIC_LINEAR_ALLOCATOR_OBTAIN(struct stObjectDataT, (&objectDatas->mem));

  objectData->id          = id;
  objectData->next        = NULL;
  objectData->prev        = NULL;
  objectData->memSize     = 0;
  objectData->type        = 0;
  objectData->nbElements  = 0;
  objectData->arraySize   = 0;
  objectData->data        = NULL;
  objectData->bLoaded     = false;
  objectData->bPersistant = true;
  objectData->bModified   = true;
  objectData->bDeleted    = false;
  objectData->bAllocated  = false;

  ST_LIST_ADD(objectDatas, objectData);

  return objectData;
}

u32 stObjectData_ReserveDataForLoading(struct stObjectDataT* objectData)
{
  ST_NULL_CHECK(objectData);

  objectData->memSize = stGetTypeMemorySize(objectData->type, objectData->nbElements, objectData->arraySize);
  ST_ASSERT(objectData->memSize > 0, "Object Data Memory Size is zero!");

  objectData->data        = ST_ALLOCATE_MEM(objectData->memSize, ST_DEFAULT_ALIGNMENT);
  objectData->bLoaded     = true;
  objectData->bAllocated  = true;

  return objectData->memSize;
}

void stObjectData_Release(struct stObjectDataT* objectData)
{
  ST_NULL_CHECK(objectData);

  if (objectData->bDeleted == false)
  {
    objectData->bDeleted    = true;
    objectData->bPersistant = false;
    objectData->bLoaded     = false;

    if (objectData->bAllocated)
    {
      objectData->bAllocated = false;
      ST_RELEASE_MEM(objectData->data);
      objectData->data = NULL;
    }

    objectData->type        = ST_TYPE_NONE;
    objectData->nbElements  = 0;
    objectData->arraySize   = 0;
    objectData->memSize     = 0;
  }
}

stBool stObjectData_Has(struct stObjectDatasT* objectDatas, u32 id)
{
  return id < objectDatas->nextId;
}

struct stObjectDataT* stObjectData_Get(struct stObjectDatasT* objectDatas, u32 id)
{
  if (id > objectDatas->nextId)
    return NULL;

  ST_LIST_FOR_BEGIN(struct stObjectDataT, objectDatas, it);
  {
    if (it->id == id)
    {
      if (it->bDeleted)
        return NULL;

      return it;
    }
  }
  ST_LIST_FOR_END(struct stObjectDataT, objectDatas, it);

  return NULL;
}

struct stObjectDataT* stObjectData_Load(struct stObjectDatasT* objectDatas, u32 id)
{
  struct stObjectDataT* objectData = stObjectData_Get(objectDatas, id);
  if (objectData == NULL)
    return NULL;

  return stObjectData_LoadImpl(objectData);
}

struct stObjectDataT* stObjectData_LoadImpl(struct stObjectDataT* objectData)
{
  ST_NULL_CHECK(objectData);

  if (objectData->bLoaded)
    return objectData;
  
  stLoadObjectData(objectData);
  objectData->bLoaded = true;
  objectData->bAllocated = true;
  return objectData;
}

struct stObjectDataT* stObjectData_Unload(struct stObjectDatasT* objectDatas, u32 id)
{
  struct stObjectDataT* objectData = stObjectData_Get(objectDatas, id);
  if (objectData == NULL)
    return NULL;

  return stObjectData_UnloadImpl(objectData);
}

struct stObjectDataT* stObjectData_UnloadImpl(struct stObjectDataT* objectData)
{
  if (objectData->bLoaded)
  {
    objectData->bLoaded = false;
  }

  if (objectData->bAllocated)
  {
    objectData->bAllocated = false;
    ST_RELEASE_MEM(objectData->data);
    objectData->data = NULL;
  }

  return objectData;
}

void stObjectDatas_Move(struct stObjectDatasT* dst, struct stObjectDatasT* src)
{
  dst->first  = src->first;
  dst->last   = src->last;
  dst->nextId = src->nextId;
  stDynamicLinearAllocator_Move(&dst->mem, &src->mem);
  
  stObjectDatas_Initialise(src);
}

