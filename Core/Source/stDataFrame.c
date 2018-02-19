// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stDataFrame.c
 *
 * Frame object-component data creation, management and processing
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stDataFrame.h"
#include "stList.h"
#include "stError.h"
#include "stCommandKey.h"
#include "stHeaderFrame.h"

void deleteFrameData(stFrameData* frame)
{
  stDynamicLinearAllocator_Shutdown(&frame->keyMem);
  stDynamicLinearAllocator_Shutdown(&frame->dataMem);
  stDynamicLinearAllocator_Shutdown(&frame->objects.mem);

  stObjectDatas_Shutdown(&frame->objectDatas);
  
  ST_DELETE_FROM_HEAP(frame);
}

stFrameData* stNewFrameData(stFrameDatas* frames)
{

  stFrameData* frame;

  if (frames->pool.first != NULL)
  {
    frame = frames->pool.first;
    ST_LIST_REMOVE(stFrameData, (&frames->pool), frame);
    frames->poolCount--;
    //printf("Frame Data: Pool\n");
  }
  else
  {
    frame = ST_NEW_FROM_HEAP(stFrameData);

    //printf("Frame Data: New\n");

    if (frame == NULL)
    {
      stRaiseError(ST_ERROR_RECORDING);
      ST_PRINT_ERROR("Cannot Alloate Memory for stFrameData");
      return NULL;
    }

    u32 error = stPopError();

    memset(frame, 0, sizeof(stFrameData));
    frame->keyMem.size = sizeof(stDeltaKeyValue) * 4096;
    stDynamicLinearAllocator_Initialise(&frame->keyMem);
    stDynamicLinearAllocator_Reserve(&frame->keyMem, frames->poolMinSize);
    frame->dataMem.size = ST_KILOBYTES(1024);
    stDynamicLinearAllocator_Initialise(&frame->dataMem);
    stDynamicLinearAllocator_Reserve(&frame->dataMem, frames->poolMinSize);

    frame->objects.mem.size = sizeof(stObjectHeader) * 32;
    frame->objects.first = NULL;
    frame->objects.last = NULL;
    frame->objects.count = 0;
    frame->objects.memSize = 0;

    stDynamicLinearAllocator_Initialise(&frame->objects.mem);

    stObjectDatas_Initialise(&frame->objectDatas);

    if (stPeekError() != 0)
    {
      stPushError(error);
      stRaiseError(ST_ERROR_RECORDING);
      ST_PRINT_ERROR("Cannot Alloate Memory for stFrameData. DynamicLinearAllocator, or ObjectDatas was not allocated");
      deleteFrameData(frame);
      return NULL;
    }

    stPushError(error);
  }

  ST_LIST_ADD(frames, frame);
  frames->count++;

  return frame;
}

stDeltaKeyValue* stNewCommandInFrameDataImpl(stFrameData* frame, stCommandKey key, u32* cmdSize)
{

  *cmdSize   = stGetCommandSize(key);

  if (stGetCommandArrayType(key) == ST_ARRAY_TYPE_INLINE)
  {
    *cmdSize = stGetCommandCustomSize(key);
  }

  stByte* mem  = stDynamicLinearAllocator_Obtain(&frame->dataMem, *cmdSize);

  stDeltaKeyValue* keyValue =  (stDeltaKeyValue*) stDynamicLinearAllocator_Obtain(&frame->keyMem, sizeof(stDeltaKeyValue));

  if (keyValue == NULL)
  {
    ST_PRINT_ERROR("Cannot Allocate Memory for stDeltaKeyValue");
    stRaiseError(ST_ERROR_RECORDING);
    return NULL;
  }

  keyValue->key  = key;
  keyValue->data = mem;
  keyValue->next = NULL;

  // First
  if (frame->last == NULL)
  {
    frame->keys = keyValue;
  }

  // Middle
  if (frame->last != NULL)
  {
    frame->last->next = keyValue;
  }

  frame->last = keyValue;
  frame->commandCount++;

  return keyValue;
}


stByte* stNewCommandInFrameData(stFrameData* frame, stCommandKey key, u32* cmdSize)
{
  stDeltaKeyValue* keyValue = stNewCommandInFrameDataImpl(frame, key, cmdSize);

  return keyValue->data;
}


void stShutdownFrameDatas(stFrameDatas* frames)
{
  ST_LIST_FOR_BEGIN(stFrameData, (&frames->pool), frame);
  {
    deleteFrameData(frame);
  }
  ST_LIST_FOR_END(stFrameData, (&frames->pool), frame);


  ST_LIST_FOR_BEGIN(stFrameData, frames, frame);
  {
    if (frame->header)
    {
      frame->header->data = NULL;
    }
    deleteFrameData(frame);
  }
  ST_LIST_FOR_END(stFrameData, frames, frame);

  frames->pool.first = NULL;
  frames->pool.last = NULL;
  frames->first = NULL;
  frames->last = NULL;
}

