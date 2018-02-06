// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	1.0.0
 */

#ifndef ST_OBJECT_DATA_H
#define ST_OBJECT_DATA_H

#include "stPlatform.h"
#include "stBits.h"
#include "stLinearAllocator.h"

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

struct stObjectDatasT
{
  struct stObjectDataT      *first, *last;
  u32                       nextId;
  stDynamicLinearAllocator  mem;
};

void stObjectDatas_Initialise(struct stObjectDatasT* objectDatas);

void stObjectDatas_Shutdown(struct stObjectDatasT* objectDatas);

struct stObjectDataT* stObjectData_Obtain(struct stObjectDatasT* objectDatas, u8 type, u8 nbElements, u32 arraySize);

struct stObjectDataT* stObjectData_Obtain_ForReader(struct stObjectDatasT* objectDatas, u32 id);

u32 stObjectData_ReserveDataForLoading(struct stObjectDataT* objectData);

void stObjectData_Release(struct stObjectDataT* objectData);

stBool stObjectData_Has(struct stObjectDatasT* objectDatas, u32 id);

struct stObjectDataT* stObjectData_Get(struct stObjectDatasT* objectDatas, u32 id);

struct stObjectDataT* stObjectData_Load(struct stObjectDatasT* objectDatas, u32 id);

struct stObjectDataT* stObjectData_LoadImpl(struct stObjectDataT* objectData);

struct stObjectDataT* stObjectData_Unload(struct stObjectDatasT* objectDatas, u32 id);

struct stObjectDataT* stObjectData_UnloadImpl(struct stObjectDataT* objectData);

void stObjectDatas_Move(struct stObjectDatasT* dst, struct stObjectDatasT* src);

#endif

