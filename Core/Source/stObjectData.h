// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stObjectData.h
 *
 * Manages memory and classes for changes of object's component-data/"cold" data during a frame.
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_OBJECT_DATA_H
#define ST_OBJECT_DATA_H

#include "stPlatform.h"
#include "stBits.h"
#include "stLinearAllocator.h"

/**
 * Represents a full/partial change of an object's component data during this frame.
 * Without a full change log of previous object datas this information is incomplete.
 */
struct stObjectDataT
{
  struct stObjectDataT     *next, *prev;
  stBool32                  bLoaded     : 1;
  stBool32                  bPersistant : 1;
  stBool32                  bModified   : 1;
  stBool32                  bDeleted    : 1;
  stBool32                  bAllocated  : 1;
  u64                       persistantOffset;
  u32                       persistantByteLength;
  u32                       id;
  u8*                       data;
  u32                       memSize;
  u32                       arraySize;
  u8                        type;
  u8                        nbElements;
  u8                        __padding[2];
};

/**
 * Management/Holder class for object datas.
 */
struct stObjectDatasT
{
  struct stObjectDataT      *first, *last;
  u32                       nextId;
  stDynamicLinearAllocator  mem;
};

/**
 * Initialies an stObjectDatas class and pre-allocate memory.
 */
void stObjectDatas_Initialise(struct stObjectDatasT* objectDatas);

/**
 * Shutdown/Release stObjectDatas (and child stObjectData) objects, and memory.
 */
void stObjectDatas_Shutdown(struct stObjectDatasT* objectDatas);

/**
 * Obtain enough memory to write a change to an object's component. (Used with writing)
 */
struct stObjectDataT* stObjectData_Obtain(struct stObjectDatasT* objectDatas, u8 type, u8 nbElements, u32 arraySize);

/**
 * Obtain enough memory to write a change to an object's component. (Used with reading)
 */
struct stObjectDataT* stObjectData_Obtain_ForReader(struct stObjectDatasT* objectDatas, u32 id);

/**
 * Pre-allocate memory for the loading object data. (Used with reading)
 */
u32 stObjectData_ReserveDataForLoading(struct stObjectDataT* objectData);

/**
 * Release the object-data, and free any memory needed. Just preserving the record.
 */
void stObjectData_Release(struct stObjectDataT* objectData);

/**
 * Has the objectdatas have any non-deleted instance of an object.
 */
stBool stObjectData_Has(struct stObjectDatasT* objectDatas, u32 id);

/**
 * Get the stObjectData from it's object-handle, or return NULL.
 */
struct stObjectDataT* stObjectData_Get(struct stObjectDatasT* objectDatas, u32 id);

/**
 * Try and load the object's data (from the file).
 */
struct stObjectDataT* stObjectData_Load(struct stObjectDatasT* objectDatas, u32 id);

/**
 * Forceload the object's data (from the file).
 */
struct stObjectDataT* stObjectData_LoadImpl(struct stObjectDataT* objectData);

/**
 * Try and Unload the object data and release any memory, but preserve the record.
 */
struct stObjectDataT* stObjectData_Unload(struct stObjectDatasT* objectDatas, u32 id);

/**
 * Force unload the object data and release any memory, but preserve the record.
 */
struct stObjectDataT* stObjectData_UnloadImpl(struct stObjectDataT* objectData);

/**
 * Change the ownership of stObjectDatas to another stObjectDatas
 */
void stObjectDatas_Move(struct stObjectDatasT* dst, struct stObjectDatasT* src);

#endif

