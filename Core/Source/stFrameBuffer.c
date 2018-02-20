// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stFrameBuffer.c
 *
 * Container classes for storing and processing commands for the currently submitted frame
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stFrameBuffer.h"
#include "stRecording.h"
#include "stWriter.h"
#include "stList.h"
#include "stError.h"
#include "stCommandKey.h"

#define BX_RADIXSORT64_FN_NAME   sortRadixCommands
#define BX_RADIXSORT64_USER_TYPE stCommandBufferIndex
#include <bx/radixsort64.h>
#undef BX_RADIXSORT64_FN_NAME
#undef BX_RADIXSORT64_USER_TYPE

#if ST_CFG_MULTI_THREADED == 0
#define ST_PROCESSING_THREAD_HOLD(CONTEXT_PTR)
#define ST_PROCESSING_THREAD_RESUME(CONTEXT_PTR)
#else
#define ST_PROCESSING_THREAD_HOLD(CONTEXT_PTR)      // TODO
#define ST_PROCESSING_THREAD_RESUME(CONTEXT_PTR)    // TODO
#endif

void deleteFrameData(stFrameData* frame);

static void resetCommandBuffer(stCommandBuffer* buffer)
{
  ST_NULL_CHECK(buffer);

  stDynamicLinearAllocator_Clear(&buffer->mem);

  memset(buffer->keys, 0, sizeof(buffer->keys));
  memset(buffer->data, 0, sizeof(buffer->data));
  memset(buffer->values, 0, sizeof(buffer->values));

  buffer->size = 0;
}

#define FUZZY_SIZE 4
#define FUZZY_ID(X)     (1 + (X / (FUZZY_SIZE)))
#define UNFUZZY_ID(X)   (FUZZY_SIZE * (X - 1))

ST_INLINE u32 unfuzzyId(u32 x, u32 min, u32 max)
{
  x = (FUZZY_SIZE * (x - 1));
  if (x < min)
    x = min;
  else if (x > max)
    x = max;
  return x;
}

static void createFrameFromCommandBuffer(u32 size, u32 frameNumber, f64 frameTime, f64 frameLength, stCommandKey* keys, stCommandBufferIndex* values, u8** data, stFrameDatas* frameDeltas, stHeaderFrames* headers, struct stObjectDatasT* objectDatas)
{
  if (stHasError(ST_ERROR_RECORDING) || stHasError(ST_ERROR_WRITING))
  {
    ST_PRINT_ERROR("Will not submit frame. There is an error recording or writing"); 
    return;
  }

  stHeaderFrame* header  = NULL;
  stFrameData* frameData = NULL;

  header = stNewHeaderFrame(headers);

  if (header == NULL)
  {
    goto badFrame;
  }

  frameData = stNewFrameData(frameDeltas);

  if (frameData == NULL)
  {
    goto badFrame;
  }

  header->bPersistant = true;
  header->bLoaded     = true;

  header->sequenceNum = frameNumber;
  header->frameTime   = frameTime;
  header->frameLength = frameLength;
  header->data = frameData;
  header->bSaved = false;
  header->savedVersion = 0;

  frameData->header = header;
  frameData->commandCount = 0;

  stDynamicLinearAllocator_Clear(&frameData->dataMem);

  stObjectHeader* objectHeader = NULL;


  u32 minId = 0, maxId = 0;

  if (size >= 1)
  {
    minId = stGetCommandObject(keys[0]);
    maxId = stGetCommandObject(keys[size - 1]);

    // Has events
    if (maxId >= 0x7FFFFF)
    {
      // All Events
      if (minId >= 0x7FFFFF)
      {
        minId = 0;
        maxId = 0;
      }
      // Some Events.
      else
      {
        for(s32 i=size-1;i > -1;i--)
        {
          u32 id = stGetCommandObject(keys[i]);
          if (id < 0x7FFFFF)
          {
            maxId = id;
            break;
          }
        }
      }
    }


    header->idMin = minId;
    header->idMax = maxId;

#if ST_CFG_USE_IDMAP == 1
    if (header->idMin >= 1)
    {
      stBitSet_Initialise(&header->idMap, stMax(1, (header->idMax - header->idMin)));
    }
#endif

  }
  else
  {
    header->idMin = 0;
    header->idMax = 0;
  }

  for(u32 i=0;i < size;i++)
  {
    stCommandKey cmdKey = keys[i];
    u8* cmdData = data[values[i]];
    u32 cmdSize = stGetCommandSize(cmdKey);

    stDeltaKeyValue* keyValue = stNewCommandInFrameDataImpl(frameData, cmdKey, &cmdSize);

    if (keyValue == NULL)
    {
      goto badFrame;
    }

    memcpy(keyValue->data, cmdData, cmdSize);

    u32 objectId = stGetCommandObject(cmdKey);
    stCommandType cmdType  = stGetCommandType(cmdKey);

    if (objectHeader == NULL || objectHeader->objectId != objectId)
    {
      objectHeader = ST_DYNAMIC_LINEAR_ALLOCATOR_OBTAIN(stObjectHeader, &frameData->objects.mem);
      memset(objectHeader, 0, sizeof(stObjectHeader));

      objectHeader->objectId = objectId;
      objectHeader->bLoaded = true;
      objectHeader->bPersistant = true;
      objectHeader->keys = keyValue;

      ST_LIST_ADD((&frameData->objects), objectHeader);
      frameData->objects.count++;
    }

    if (cmdType == ST_CMD_OBJ_NEW)
    {
      objectHeader->bCreated = true;
    }
    else if (cmdType == ST_CMD_OBJ_DELETE)
    {
      objectHeader->bDeleted = true;
    }
    else
    {
      objectHeader->bUpdated = true;
    }

    objectHeader->count++;
    
#if ST_CFG_USE_IDMAP == 1
    if (objectId < 0x7FFFFF)
    {
      u32 bitId = objectId - minId;
      stBit_SetBit(&header->idMap, bitId);
    }
#endif
      
  }

  // Move over object data.
  stObjectDatas_Move(&frameData->objectDatas, objectDatas);

  return;

  badFrame:
  stRaiseError(ST_ERROR_RECORDING);

  if (header != NULL)
  {
    header->data = NULL;
    stDeleteHeaderFrame(header, headers);
  }

  if (frameData != NULL)
  {
    ST_LIST_REMOVE(stFrameData, frameDeltas, frameData);
    deleteFrameData(frameData);
  }

  return;
}

void stInitialiseCommandBuffer(stCommandBuffer* buffer)
{
  buffer->frameNumber = 0;
  buffer->frameTime   = 0.0f;
  buffer->size        = 0;

  buffer->mem.size = ST_KILOBYTES(512);
  stDynamicLinearAllocator_Initialise(&buffer->mem);

  stObjectDatas_Initialise(&buffer->objectDatas);
}

void stShutdownCommandBuffer(stCommandBuffer* buffer)
{
  stDynamicLinearAllocator_Shutdown(&buffer->mem);
  stObjectDatas_Shutdown(&buffer->objectDatas);
}

void stCycleBuffers(stContext* ctx)
{
  stCommandBuffer* buffers[3];
  u32 frameNumber ;

  buffers[0] = ctx->commandBuffer[0];
  buffers[1] = ctx->commandBuffer[1];
  frameNumber = buffers[0]->frameNumber;

  if (ctx->format == ST_FORMAT_DELTA)
  {
    buffers[0] = ctx->commandBuffer[1];
    buffers[1] = ctx->commandBuffer[0];
  }

  resetCommandBuffer(buffers[ST_COMMAND_BUFFER_INDEX_SUBMISSION]);
  buffers[ST_COMMAND_BUFFER_INDEX_SUBMISSION]->frameNumber = frameNumber + 1;


  ctx->commandBuffer[0] = buffers[0]; 
  ctx->commandBuffer[1] = buffers[1]; 
}

void WriteKeysToConsole(stCommandKey* keys, u32 count)
{
  const char *padding="                                                     ";

  if (count == 0)
  {
    ST_PRINT_CHATTERF("*** Submitted Empty Frame.");
    return ;
  }

  ST_PRINT_CHATTERF("*** Frame Submission. Size=%i", count);
  for(u32 i=0;i < count;i++)
  {
    u64 key = keys[i];
    u32 Type            = stGetCommandType(key);
    u32 Size            = stGetCommandSize(key);
    u32 ObjectHandle    = stGetCommandObject(key);

    u32 ObjectType      = (u32) stGetObjectType(ObjectHandle);
    stClass* Klass      = stGetClass_NoMutex_NoAssert(ObjectType);

    u32 userData        = 0;
    
    if (Klass != NULL)
    {
      userData = Klass->userData;
    }

    u32 ComponentHandle = stGetCommandComponent(key);
    u32 ComponentType, nbElements;
    stGetCommandDataTypeAndNbElements(key, &ComponentType, &nbElements);
    
    const char* cmdTypeStr =   stCommandTypeToString(Type);

    int padLen = 24 - (int) strlen(cmdTypeStr); // Calc Padding length
    if(padLen < 0) padLen = 0;    // Avoid negative length

    // Xcode Unused Fix
    ST_UNUSED(Size);
    ST_UNUSED(Klass);
    ST_UNUSED(ComponentHandle);
    ST_UNUSED(padding);
      
    ST_PRINT_CHATTERF("... [%.4i] Key=%016llX Cmd=%s%*.*s Size=%.3ib Object#%.4i/%i=%.4i Component=%.3i, Type=%s(%i) ",
        i,
      key,
      cmdTypeStr,
      padLen, padLen, padding,
      Size,
      ObjectType,
      userData,
      ObjectHandle,
      ComponentHandle,
      stGetTypeToString(ComponentType),
      nbElements
      );
  }
}

void stSubmit(f32 frameLength)
{
  stContext* ctx = stGetContext_NoMutex();
  ST_PROCESSING_THREAD_HOLD(ctx);

  if (ctx->recordingMem == NULL)
    return;

  stCycleBuffers(ctx);

  stCommandBuffer* buffer = ctx->commandBuffer[ST_COMMAND_BUFFER_INDEX_PROCESSING];

  f64 frameTime = ctx->recordingTime;
  ctx->recordingTime += frameLength;
  ctx->nbSubmittedFrames++;

  u32 frameNb = ctx->nbSubmittedFrames - 1;
  stBool savedFrame = false;
  buffer->frameTime = frameTime;

  if (buffer->size > 0)
  {
    savedFrame = true;

#if 0
    WriteKeysToConsole(buffer->keys, buffer->size);
#endif

    sortRadixCommands(buffer->keys, ctx->recordingMem->tempBuffer.keys, buffer->values, ctx->recordingMem->tempBuffer.values, buffer->size);
    createFrameFromCommandBuffer(buffer->size, frameNb, frameTime, (f64) frameLength, buffer->keys, buffer->values, buffer->data, &ctx->frameDatas, &ctx->headerFrames, &buffer->objectDatas);
  }

  ctx->headerFrames.minSeqNum = stMin(ctx->headerFrames.minSeqNum, frameNb);
  ctx->headerFrames.maxSeqNum = stMax(ctx->headerFrames.maxSeqNum, frameNb);
  stResetEventIds(&ctx->classes);

#if 0
  WriteKeysToConsole(buffer->keys, buffer->size);
#endif

  if (savedFrame && ctx->writeCtx != NULL && ctx->writeCtx->appendMode && --ctx->writeCtx->frameSaveCounter <= 0)
  {
    ctx->writeCtx->frameSaveCounter = ctx->writeCtx->frames;
    stSaveFile_Update();
  }

  ST_PROCESSING_THREAD_RESUME(ctx);
}

