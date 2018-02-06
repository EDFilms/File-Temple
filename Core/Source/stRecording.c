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

#include "stRecording.h"
#include "stThreads.h"
#include "stLibrary.h"
#include "stList.h"
#include "stWriter.h"
#include "stReader.h"
#include "stError.h"
#include "stSecurity.h"

#include <time.h>
#define ST_SECURITY_CHECK  if ((u32) time(NULL) > ST_EXPIRES) return 0;

typedef struct stEnginesT
{
  stContext *first, *last;
  u32        nextId;
} stContexts;

static stSpinLock  sContextLock = ST_SPINLOCK_DEFAULT;
static stContexts  sContexts    = { NULL, NULL, 0 };
static stContext*  sContext     = NULL;

ST_COMMENT(MaybeThreadSafe);
ST_INLINE stBool contextMatchesId(stContext* context, u32 contextId)
{
  return context != NULL && context->id == contextId;
}

void stPrintMemoryStats()
{
  printf("Classes Size: ");
  stDynamicPool_PrintStats(&sContext->classes.mem);
  printf("\n");

  printf("Command Buffer[0] Count: %i\n", sContext->commandBuffer[0]->size);
  printf("Command Buffer[1] Count: %i\n", sContext->commandBuffer[1]->size);
  
  printf("Frames: ");
  u32 deltaPoolCount = 0, deltaCount = 0;
  u32 deltaPoolMemSize = 0, deltaPoolAllocatorCount = 0;

  ST_LIST_FOR_BEGIN(stFrameData, (&sContext->frameDatas.pool), frameData);
  {
    u32 allocatorCount = 0;
    ST_LIST_COUNT(allocatorCount, stFixedLinearAllocator, (&frameData->keyMem));
    deltaPoolMemSize += allocatorCount * frameData->keyMem.size;
    deltaPoolMemSize += allocatorCount * frameData->dataMem.size;
    deltaPoolAllocatorCount += allocatorCount * 2;
    deltaPoolCount++;
  }


  ST_LIST_FOR_END(stFrameData, (&sContext->frameDatas.pool), frameData);
  
  ST_LIST_FOR_BEGIN(stFrameData, (&sContext->frameDatas), frameData);
  {
    u32 allocatorCount = 0;
    ST_LIST_COUNT(allocatorCount, stFixedLinearAllocator, (&frameData->keyMem));
    deltaPoolMemSize += allocatorCount * frameData->keyMem.size;
    deltaPoolMemSize += allocatorCount * frameData->dataMem.size;
    deltaPoolAllocatorCount += allocatorCount * 2;
    deltaCount++;
  }

  ST_LIST_FOR_END(stFrameData, (&sContext->frameDatas.pool), frameData);

  printf("%i Kb Key/Data Memory Allocated, %i Allocators, %i Pool Frames, %i Active Frames\n", deltaPoolMemSize / 1024, deltaPoolAllocatorCount, deltaPoolCount, deltaCount);
  printf("%i Frames Allocated, %i in Pool\n", sContext->frameDatas.count, sContext->frameDatas.poolCount);
}

u32 stCreateRecording()
{
  u32 id = 0;
  stContext* context = NULL;
  
  ST_SECURITY_CHECK;

  ST_LOCK(&sContextLock);
    

    stClearErrors();

    u32 sessionOffset = stMaybeInitialiseLibrary();
    sContexts.nextId += sessionOffset;

    context = ST_NEW_FROM_HEAP(stContext);
    memset(context, 0, sizeof(stContext));

    sContext = context;

    context->type = ST_CONTEXT_TYPE_SAVE;
    context->next = NULL;
    context->prev = NULL;
#if ST_CFG_CHANGES_IS_DEFAULT == 1
    context->format = ST_FORMAT_DELTA;
#else
    context->format = 0;
#endif
    id = context->id  = sContexts.nextId;

    stInitialiseClasses(&context->classes);

    context->recordingMem = ST_NEW_FROM_HEAP(struct stContextRecordingMem);

    stInitialiseCommandBuffer(&context->recordingMem->commandBufferMem[0]);
    stInitialiseCommandBuffer(&context->recordingMem->commandBufferMem[1]);
    stInitialiseHeaderFrames(&context->headerFrames);
    stInitialiseIterators(&context->iterators);

    context->strictMode = false;
    context->recordingTime = 0.0;
    context->nbSubmittedFrames = 0;
    context->commandBuffer[0] = &context->recordingMem->commandBufferMem[0];
    context->commandBuffer[1] = &context->recordingMem->commandBufferMem[1];

    ST_LIST_ADD((&sContexts), context);

  ST_UNLOCK(&sContextLock);

  ST_PRINT_INFOF("Recording Started. Id = %i", id);

  return id;
}

ST_PUBLIC u32 stOpenRecording(stCStr path)
{
  u32 id = 0;
  stContext* context = NULL;
  
  ST_SECURITY_CHECK;

  ST_LOCK(&sContextLock);

  stClearErrors();

  u32 sessionOffset = stMaybeInitialiseLibrary();
  sContexts.nextId += sessionOffset;

  context = ST_NEW_FROM_HEAP(stContext);
  memset(context, 0, sizeof(stContext));

  sContext = context;

  context->type = ST_CONTEXT_TYPE_LOAD;
  context->next = NULL;
  context->prev = NULL;
#if ST_CFG_CHANGES_IS_DEFAULT == 1
  context->format = ST_FORMAT_DELTA;
#else
  context->format = 0;
#endif
  id = context->id  = sContexts.nextId;

  stInitialiseClasses(&context->classes);

  context->recordingMem = NULL;
  // stInitialiseCommandBuffer(&context->recordingMem->commandBufferMem[0]);
  // stInitialiseCommandBuffer(&context->recordingMem->commandBufferMem[1]);
  stInitialiseHeaderFrames(&context->headerFrames);
  stInitialiseIterators(&context->iterators);

  context->strictMode = false;
  context->recordingTime = 0.0;
  context->nbSubmittedFrames = 0;
  context->commandBuffer[0] = NULL; // &context->recordingMem->commandBufferMem[0];
  context->commandBuffer[1] = NULL; //&context->recordingMem->commandBufferMem[1];
  
  ST_LIST_ADD((&sContexts), context);

  stLoadFile_Initialise(path);

  ST_PRINT_INFOF("Recording Opened. Path = %s, Id = %i, Total Frames = %i, Recorded Frames = %i", path,  id, context->headerFrames.count, context->nbSubmittedFrames);

  ST_UNLOCK(&sContextLock);

  return id;
}

ST_PUBLIC_PROBE int stInternal_PrintMemoryAllocations();

void stCloseRecording(u32 contextId)
{

  stContext* context = NULL;
  
  ST_LOCK(&sContextLock);
  
    ST_LIST_MAYBE_FETCH(stContext, (&sContexts), context, contextId, contextMatchesId);
    ST_ASSERT(context != NULL, "Recording with Id could not be found");

    if (context->writeCtx)
    {
      stSaveFile_Shutdown();
    }

    if (context->readCtx)
    {
      stLoadFile_Shutdown();
    }

    stShutdownClasses(&context->classes);

    if (context->recordingMem != NULL)
    {
      stShutdownCommandBuffer(&context->recordingMem->commandBufferMem[1]);
      stShutdownCommandBuffer(&context->recordingMem->commandBufferMem[0]);

      ST_DELETE_FROM_HEAP(context->recordingMem);
      context->recordingMem = NULL;
    }

    stShutdownFrameDatas(&context->frameDatas);
    stShutdownHeaderFrames(&context->headerFrames);
    stShutdownIterators(&context->iterators);

    ST_LIST_REMOVE(stContext, (&sContexts), context);
    ST_RELEASE_MEM(context);
    context = NULL;
    sContext = NULL;
    
    ST_PRINT_INFOF("Recording Closed. Id = %i", contextId);

    stMaybeShutdownLibrary();

  ST_UNLOCK(&sContextLock);
  
}

stContext* stObtainContextLock()
{
  ST_LOCK(&sContextLock);
  return sContext;
}

void stReleaseContextLock(stContext* context)
{
  ST_ASSERT(context == sContext, "Recording switched!");
  ST_UNLOCK(&sContextLock);
  ST_UNUSED(context);
}

stContext* stGetContext_NoMutex()
{
  return sContext;
}

stBool stHasContext_NoMutex()
{
  return (sContext != NULL);
}

void stAppendSaveRecording(stCStr path, stEnum32 format, u32 frames)
{
  if (frames == 0)
    frames = 1;

  ST_LOCK(&sContextLock);
  stSaveFile_Initialise(path, format, frames, false, false);
  ST_UNLOCK(&sContextLock);
}

void stSaveRecording(stCStr path, stEnum32 format)
{
  ST_LOCK(&sContextLock);
  stSaveFile_Initialise(path, format, 1, true, true);
  ST_UNLOCK(&sContextLock);
}

void stReserveFrameDatas(stFrameDatas* frames, s32 value);
void stReserveMemoryFramePoolSize(stFrameDatas* frames, s32 value);

void stMemoryReserve(s32 type, s32 value)
{
  ST_LOCK(&sContextLock);
  switch(type)
  {
    case ST_MEMORY_FRAME_POOL:
    {
      stReserveFrameDatas(&sContext->frameDatas, value);
    }
    break;
    case ST_MEMORY_FRAME_PAGE:
    {
      stReserveMemoryFramePoolSize(&sContext->frameDatas, value);
    }
    break;
  }
  ST_UNLOCK(&sContextLock);
}

