// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stDataFrame.h
 *
 * Frame object-component data creation, management and processing
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

/**
 * Frame data class to hold command key and data
 */
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

/**
 * Pool Container class for stFrameDataT
 */
typedef struct stFrameDataPoolT
{
  stFrameData *first, *last;
} sFrameDataPool;

/**
 * Container class for stFrameDataT
 */
struct stFrameDatasT
{
  stFrameData   *first, *last;
  sFrameDataPool pool;
  u32 poolMinSize;
  u32 count, poolCount;
};

typedef struct stFrameDatasT stFrameDatas;




/**
 * Create a new stFrameData from the stFrameDataPool pool or allocate from memory
 * @return the stFrameDataT
 */
struct stFrameDataT* stNewFrameData(struct stFrameDatasT* frames);

/**
 * Reserve data in stFrameData for writing to, and associate with the given stCommandKey.
 * Size is calculated from stCommandKey, and cmdSize is the calculated size (as output).
 * @return The stDeltaKeyValueT struct.
 */
stDeltaKeyValue* stNewCommandInFrameDataImpl(struct stFrameDataT* frame, stCommandKey key, u32* cmdSize);

/**
 * Reserve data in stFrameData for writing to, and associate with the given stCommandKey.
 * Size is calculated from stCommandKey, and cmdSize is the calculated size (as output).
 * @return A stByte array to write too.
 */
stByte* stNewCommandInFrameData(struct stFrameDataT* frame, stCommandKey key, u32* cmdSize);

/**
 * Release stFrameData and memory of given stFrameDatas.
 */
void stShutdownFrameDatas(struct stFrameDatasT* frames);

#endif
