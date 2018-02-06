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

#ifndef ST_DATA_FRAME_H
#define ST_DATA_FRAME_H

#include "stPlatform.h"
#include "stLinearAllocator.h"
#include "stObjectHeader.h"
#include "stObjectData.h"


struct stFrameDataT
{
  struct stFrameDataT      *next, *prev;

  struct stHeaderFrameT*    header;
  u32                       commandCount;

  stObjectHeaders           objects;
  struct stDeltaKeyValueT   *keys, *last;
  stDynamicLinearAllocator  keyMem;
  stDynamicLinearAllocator  dataMem;
  struct stObjectDatasT     objectDatas;
};

typedef struct stFrameDataT stFrameData;

typedef struct stFrameDataPoolT
{
  stFrameData *first, *last;
} sFrameDataPool;

struct stFrameDatasT
{
  stFrameData   *first, *last;
  sFrameDataPool pool;
  u32 poolMinSize;
  u32 count, poolCount;
};

typedef struct stFrameDatasT stFrameDatas;





struct stFrameDataT* stNewFrameData(struct stFrameDatasT* frames);

stDeltaKeyValue* stNewCommandInFrameDataImpl(struct stFrameDataT* frame, stCommandKey key, u32* cmdSize);

stByte* stNewCommandInFrameData(struct stFrameDataT* frame, stCommandKey key, u32* cmdSize);

void stShutdownFrameDatas(struct stFrameDatasT* frames);


#endif
