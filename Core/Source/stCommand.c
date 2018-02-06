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

#include "stCommand.h"
#include "stRecording.h"
#include "stBinary.h"
#include "stMath.h"
#include "stLinearAllocator.h"
#include "stList.h"
#include "stWriter.h"
#include "stReader.h"
#include "stError.h"
#include "stPlatform.h"
#include "stCommandKey.h"

// Before I forget:
// Useful for in frame headers to know a fuzzy version of what objects are in the header,
// Represent as ids (u32), maybe written as 3-byte integers?
// Maybe we can try running this a few times, to group more together.
// http://stackoverflow.com/questions/2305767/converting-sets-of-integers-into-ranges

#include <time.h>

void deleteFrameData(stFrameData* frame);

void stReserveFrameDatas(stFrameDatas* frames, s32 value)
{
  u32 count = 0;
  ST_LIST_COUNT(count, stFrameData, frames);
  
  if (count < (u32) value)
  {
    u32 diff = (u32) value - count;
    for (u32 i=0;i < diff;i++)
    {
      stFrameData* frame;
      frame = ST_NEW_FROM_HEAP(stFrameData);

      if (frame == NULL)
      {
        stRaiseError(ST_ERROR_RESERVE);
        break;
      }

      u32 error = stPopError();


      memset(frame, 0, sizeof(stFrameData));
      frame->keyMem.size = sizeof(stDeltaKeyValue) * 4096;
      stDynamicLinearAllocator_Initialise(&frame->keyMem);
      stDynamicLinearAllocator_Reserve(&frame->keyMem, frames->poolMinSize);
      frame->dataMem.size = ST_KILOBYTES(64);
      stDynamicLinearAllocator_Initialise(&frame->dataMem);
      stDynamicLinearAllocator_Reserve(&frame->dataMem, frames->poolMinSize);
      
      frame->objects.mem.size = sizeof(stObjectHeader) * 32;
      frame->objects.first = NULL;
      frame->objects.last = NULL;
      frame->objects.count = 0;
      frame->objects.memSize = 0;

      stDynamicLinearAllocator_Initialise(&frame->objects.mem);

      ST_LIST_ADD((&frames->pool), frame);

      if (stPeekError() != 0)
      {
        stPushError(error);
        stRaiseError(ST_ERROR_RESERVE);
        deleteFrameData(frame);
        break;
      }

      frames->count++;

    }
  }
}

void stReserveMemoryFramePoolSize(stFrameDatas* frames, s32 value)
{
  if (value < 0)
    value = 0;
  else if (value > 1024)
    value = 1024;

  frames->poolMinSize = (u32) value;

  ST_LIST_FOR_BEGIN(stFrameData, frames, frame);
  {
    stDynamicLinearAllocator_Reserve(&frame->keyMem, frames->poolMinSize);
    stDynamicLinearAllocator_Reserve(&frame->dataMem, frames->poolMinSize);
  }
  ST_LIST_FOR_END(stFrameData, frames, frame);

  ST_LIST_FOR_BEGIN(stFrameData, (&frames->pool), frame);
  {
    stDynamicLinearAllocator_Reserve(&frame->keyMem, frames->poolMinSize);
    stDynamicLinearAllocator_Reserve(&frame->dataMem, frames->poolMinSize);
  }
  ST_LIST_FOR_END(stFrameData, (&frames->pool), frame);
}

void stRecycleFrameData(stFrameDatas* dataFrames, stFrameData* dataFrame)
{
  ST_LIST_REMOVE(stFrameData, dataFrames, dataFrame);

  // ST_PRINT_CHATTERF("Recycle Data=%i\n", dataFrame->header->sequenceNum);

  stHeaderFrame* header = dataFrame->header;
  header->bLoaded = false;
  header->data = NULL;

  ST_LIST_ADD((&dataFrames->pool), dataFrame);
  dataFrame->header = NULL;
  dataFrame->commandCount = 0;
  dataFrame->keys = NULL;
  dataFrame->last = NULL;
  
  dataFrame->objects.first = NULL;
  dataFrame->objects.last = NULL;
  dataFrame->objects.count = 0;
  dataFrame->objects.memSize = 0;

  stDynamicLinearAllocator_Clear(&dataFrame->keyMem);
  stDynamicLinearAllocator_Clear(&dataFrame->dataMem);
  stDynamicLinearAllocator_Clear(&dataFrame->objects.mem);

  stObjectDatas_Shutdown(&dataFrame->objectDatas);
  
  dataFrames->poolCount++;
}

stObjectHeader* stNewObjectHeaderInDataFrame(stFrameData* dataFrame, u32 objectId)
{
  stObjectHeader* objectHeader = ST_DYNAMIC_LINEAR_ALLOCATOR_OBTAIN(stObjectHeader, &dataFrame->objects.mem);
  memset(objectHeader, 0, sizeof(stObjectHeader));

  ST_LIST_ADD((&dataFrame->objects), objectHeader);

  objectHeader->objectId = objectId;
  dataFrame->objects.count++;

  return objectHeader;
}

stObjectHeader* stFindObjectInFrameData(u32 objectId, stFrameData* dataFrame)
{
  // Slow for now, but may need to address this in the future.
  // One thing comes to mind, is we can search each fixed allocator seperately, knowing
  // the exact size and object count and capacity, we know where it begins and ends. 
  // It just has to be multiple bsearches in those regions of memory.

  stObjectHeader* header = dataFrame->objects.first;
  while(header != NULL)
  {
    if (header->objectId == objectId)
      return header;
    header = header->next;
  }

  return NULL;
}

ST_INLINE stCommandBufferIndex reserveCommandSlot(u32* outDataSize, stCommandBuffer* buffer, u32 frameNumber, u8 commandType, u32 objectId, u8 componentId, u8 flags, u16 byteSize, u8 customSize)
{
  stCommandKeyInfo info;
  stCommandKey key;
  stCommandBufferIndex num;

  *outDataSize = stMakeCommandInfo(&info, frameNumber, commandType, objectId, componentId, flags, byteSize, customSize);
  key = stCommandInfoToCommandKey(&info);

  num = buffer->size++;
  buffer->keys[num] = key;

  return num;
}

stByte* stPushCommand(u8 customLength, stCommandType commandType, u32 objectId, u8 componentId, u8 flags)
{
  stContext* context;
  stCommandBuffer* buffer;
  u32 dataSize;
  u8* mem;

  context = stGetContext_NoMutex();

  if (context->recordingMem == NULL)
    return NULL;

  buffer  = context->commandBuffer[ST_COMMAND_BUFFER_INDEX_SUBMISSION];

  u8 length = 0;

  if (customLength != 0)
  {
    length = customLength;
  }
  else
  {
    length = (u8) stCommandCountDiskSize[commandType];
  }

  stCommandBufferIndex num = reserveCommandSlot(
    &dataSize,
    buffer,
    buffer->frameNumber,
    commandType,
    objectId,
    componentId,
    flags,
    length,
    customLength
  );

  mem = stDynamicLinearAllocator_Obtain(&buffer->mem, dataSize);
  buffer->values[num] = num;
  buffer->data[num]   = mem;

  memset(mem, 0, length);

  return mem;
}

stByte* stCommandWriteU8(stByte* data,  u8 value)
{
  return stBinaryWriteU8(data, value);
}

stByte* stCommandWriteU16(stByte* data, u16 value)
{
  return stBinaryWriteU16(data, value);
}

stByte* stCommandWriteU32(stByte* data, u32 value)
{
  return stBinaryWriteU32(data, value);
}

stByte* stCommandWriteU64(stByte* data, u64 value)
{
  return stBinaryWriteU64(data, value);
}

stByte* stCommandWriteS8(stByte* data,  s8 value)
{
  return stBinaryWriteS8(data, value);
}

stByte* stCommandWriteS16(stByte* data, s16 value)
{
  return stBinaryWriteS16(data, value);
}

stByte* stCommandWriteS32(stByte* data, s32 value)
{
  return stBinaryWriteS32(data, value);
}

stByte* stCommandWriteS64(stByte* data, s64 value)
{
  return stBinaryWriteS64(data, value);
}

stByte* stCommandWriteF32(stByte* data, f32 value)
{
  return stBinaryWriteF32(data, value);
}

stByte* stCommandWriteF64(stByte* data, f64 value)
{
  return stBinaryWriteF64(data, value);
}

stByte* stCommandWriteString(stByte* data, const char* str, u32 strLength, u32 strCapacity)
{
  ST_NULL_CHECK(str);
  u32 strCopySize = stMin(strLength, strCapacity - 1);
  memcpy(data, str, strCopySize);
  data[strCopySize] = '\0';
  return data + (strCopySize + 1);
}

ST_PUBLIC_PROBE int stInternal_GetCurrentBufferCommandCount()
{
  stContext* context;
  stCommandBuffer* buffer;

  context = stGetContext_NoMutex();
  buffer  = context->commandBuffer[ST_COMMAND_BUFFER_INDEX_SUBMISSION];

  return (u32) buffer->size;
}

ST_PUBLIC_PROBE int stInternal_GetFrameCount()
{
  stContext* context = stGetContext_NoMutex();

  return (u32) context->frameDatas.count;
}

ST_PUBLIC_PROBE int stInternal_GetFrameCountForce()
{
  stContext* context = stGetContext_NoMutex();
  u32 count = 0;
  ST_LIST_COUNT(count, stFrameData, (&context->frameDatas));

  return count;
}

#if ST_CFG_DATA_PROBE == 1

#include "stProbe.h"

ST_PUBLIC_PROBE void stProbe_DynamicLinearAllocator(const char* name, stDynamicLinearAllocator* allocator);
ST_PUBLIC_PROBE void stProbe_CommandBuffer(const char* name, int index)
{
  stContext* context = stGetContext_NoMutex();

  if (name != NULL)
    stProbe_BeginWithName(name);
  else
    stProbe_Begin();
  
  if (index < 0 || index >= ST_COMMAND_BUFFER_INDEX_PROCESSING)
  {
    stProbe_End();
    return;
  }

  stCommandBuffer* buffer = context->commandBuffer[index];

  stProbe_KeyInt("frameNumber", buffer->frameNumber);
  stProbe_KeyDouble("frameTime", buffer->frameTime);
  stProbe_KeyInt("size", buffer->size);
  
  stProbe_DynamicLinearAllocator("mem", &buffer->mem);

  stProbe_BeginListWithName("keys");
    for(stCommandBufferIndex i=0;i < buffer->size;i++)
    {
      stProbe_ListItem_IntegerKey(buffer->keys[i]);
    }
  stProbe_EndList();

  stProbe_BeginListWithName("values");
    for(stCommandBufferIndex i=0;i < buffer->size;i++)
    {
      stProbe_ListItem_IntegerHex(buffer->values[i]);
    }
  stProbe_EndList();

  stProbe_BeginListWithName("data");
    for(stCommandBufferIndex i=0;i < buffer->size;i++)
    {
      u32 size = stGetCommandSize(buffer->keys[i]);
      stProbe_BeginInline();
        stProbe_KeyPointer("address", buffer->data[i]);
        stProbe_KeyInt("size", size);
        stProbe_KeyMemory("data", buffer->data[i], size);
      stProbe_EndInline();
    }
  stProbe_EndList();

  stProbe_End();
}

#endif


u32 stGetNumRecordedFrames()
{
  stContext* ctx = stObtainContextLock();
  u32 nbFrames = ctx->headerFrames.count;
  stReleaseContextLock(ctx);
  return nbFrames;
}

u32 stGetNumFrames()
{
  stContext* ctx = stObtainContextLock();
  u32 nbFrames = ctx->nbSubmittedFrames;
  stReleaseContextLock(ctx);
  return nbFrames;
}

f64 stGetRecordingTime()
{
  stContext* ctx = stObtainContextLock();
  f64 recordingTime = ctx->recordingTime;
  stReleaseContextLock(ctx);
  return recordingTime;
}

u32 stGetFirstFrame()
{
  stContext* ctx = stObtainContextLock();
  u32 minSeq = ctx->headerFrames.minSeqNum;
  stReleaseContextLock(ctx);
  return minSeq;
}

u32 stGetLastFrame()
{
  stContext* ctx = stObtainContextLock();
  u32 maxSeq = ctx->headerFrames.maxSeqNum;
  stReleaseContextLock(ctx);
  return maxSeq;
}

void stReorderFrames()
{
  stContext* ctx = stObtainContextLock();
  stHeaderFrames* headerFrames = &ctx->headerFrames;
  stReorderFramesImpl(headerFrames);
  stReleaseContextLock(ctx);
}

void stReorderFramesImpl(stHeaderFrames* headerFrames)
{
  if (ST_LIST_NONE(headerFrames))
  {
    headerFrames->count = 0;
    headerFrames->minSeqNum = 0;
    headerFrames->maxSeqNum = 0;
  }
  else if (ST_LIST_ONE(headerFrames))
  {
    headerFrames->count = 1;
    headerFrames->minSeqNum = 0;
    headerFrames->maxSeqNum = 0;
    headerFrames->first->sequenceNum = 0;
  }
  else
  {
    u32 num = 0;
    u32 count = 0;
    u32 min = 0xFFFFFF00;
    u32 max = 0;
    ST_LIST_FOR_BEGIN(stHeaderFrame, headerFrames, headerFrame);
    {
      headerFrame->sequenceNum = num++;
      headerFrame->bModified = true;
      count++;
    }
    ST_LIST_FOR_END(stHeaderFrame, headerFrames, headerFrame);
    headerFrames->count = count;
    headerFrames->minSeqNum = min;
    headerFrames->maxSeqNum = max;
  }
}

