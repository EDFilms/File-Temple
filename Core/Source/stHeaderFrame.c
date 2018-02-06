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

#include "stHeaderFrame.h"
#include "stReaderDelta.h"
#include "stError.h"
#include "stCommand.h"
#include "stRecording.h"
#include "stList.h"
#include "stDataFrame.h"
#include "stProbe.h"

#include <time.h>

void stRecycleFrameData(stFrameDatas* dataFrames, stFrameData* dataFrame);

stBool stLoadFrame_Internal(struct stContextT* context, stHeaderFrame* headerFrame)
{
  stFrameData* frameData = stNewFrameData(&context->frameDatas);

  frameData->commandCount = 0;

  if (frameData == NULL)
  {
    stRaiseError(ST_ERROR_READING);
    return false;
  }

  stLoadFrameData(frameData, headerFrame);
  headerFrame->data = frameData;
  frameData->header = headerFrame;

  return true;
}

stBool stLoadFrame_NoMutex(struct stHeaderFrameT* headerFrame)
{
  ST_NULL_CHECK(headerFrame);
  if (headerFrame->bLoaded)
    return true;

  stContext* ctx = stGetContext_NoMutex();

  return stLoadFrame_Internal(ctx, headerFrame);
}

stBool stUnloadFrame_NoMutex(struct stHeaderFrameT* headerFrame)
{

  if (headerFrame != NULL)
  {
    stContext* ctx = stGetContext_NoMutex();
    stUnloadFrame(&ctx->frameDatas, headerFrame);
  }

  return true;
}

void stUnloadFrame(stFrameDatas* frames, stHeaderFrame* headerFrame)
{
  ST_NULL_CHECK(headerFrame);

  headerFrame->bLoaded = false;

  stFrameData* data = headerFrame->data;

  if (data != NULL)
  {
    headerFrame->data = NULL;
    stRecycleFrameData(frames, data);
  }
}


stHeaderFrame* stNewHeaderFrame(stHeaderFrames* headerFrames)
{
  return stNewHeaderFrameWithId(headerFrames, stQpr_Next(&headerFrames->nextFrameId));
}

stHeaderFrame* stNewHeaderFrameWithId(stHeaderFrames* headerFrames, u32 id)
{
  if (stHasError(ST_ERROR_READING) || stHasError(ST_ERROR_PLAYING))
    return NULL;

  stHeaderFrame* headerFrame = ST_DYNAMIC_LINEAR_ALLOCATOR_OBTAIN(stHeaderFrame, &headerFrames->mem);

  if (headerFrame == NULL)
  {
    stRaiseError(ST_ERROR_READING);
    return NULL;
  }

  memset(headerFrame, 0, sizeof(stHeaderFrame));
  headerFrame->frameId = id;
  headerFrame->bPersistant = true;

  headerFrames->count++;
  ST_LIST_ADD(headerFrames, headerFrame);
  return headerFrame;
}

void stDeleteHeaderFrame(stHeaderFrame* header, stHeaderFrames* headerFrames)
{
  ST_UNUSED(headerFrames);
  // stDynamicLinearAllocator_Shutdown(&header->objects.mem);

  header->bDeleted = true;
  header->bPersistant = false;
#if ST_CFG_USE_IDMAP == 1
  stBitSet_Shutdown(&header->idMap);
#endif
}


stHeaderFrame* stFindHeaderFrameBySeqNum(stHeaderFrames* headerFrames, u32 seqNum)
{
  ST_LIST_FOR_BEGIN(stHeaderFrame, headerFrames, headerFrame);
  {
    if (headerFrame->sequenceNum == seqNum)
      return headerFrame;
  }
  ST_LIST_FOR_END(stHeaderFrame, headerFrames, headerFrame);

  return NULL;
}

stHeaderFrame* stFindHeaderFrameBySeqNumOrNearest(stHeaderFrames* headerFrames, u32 seqNum)
{
  stHeaderFrame* headerFrame = stFindHeaderFrameBySeqNum(headerFrames, seqNum);

  if (headerFrame != NULL)
  {
    return headerFrame;
  }

  s32 bestDistance = INT32_MAX;
  stHeaderFrame* nearest = NULL;

  ST_LIST_FOR_BEGIN(stHeaderFrame, headerFrames, it);
  {
    s32 distance = (seqNum - it->sequenceNum);
    distance *= distance;

    if (distance < bestDistance)
    {
      nearest = it;
      bestDistance = distance;
    }
  }
  ST_LIST_FOR_END(stHeaderFrame, headerFrames, headerFrame);

  return nearest;
}



void stInitialiseHeaderFrames(stHeaderFrames* headerFrames)
{
  headerFrames->mem.size = sizeof(stHeaderFrame) * 10 * 24;

  stDynamicLinearAllocator_Initialise(&headerFrames->mem);
  headerFrames->count = 0;
  headerFrames->first = NULL;

  stQpr_Initialise(&headerFrames->nextFrameId, (u32) time(NULL), 0x3212334);
}

void stShutdownHeaderFrames(stHeaderFrames* headerFrames)
{
#if ST_CFG_USE_IDMAP == 1
  ST_LIST_FOR_BEGIN(stHeaderFrame, headerFrames, headerFrame);
  {
    stBitSet_Shutdown(&headerFrame->idMap);
  }
  ST_LIST_FOR_END(stHeaderFrame, headerFrames, headerFrame);
#endif
  stDynamicLinearAllocator_Shutdown(&headerFrames->mem);
  headerFrames->count = 0;
  headerFrames->first = NULL;
}

void stHeaderFramePrint_Impl(struct stHeaderFrameT* headerFrame, const char* comment)
{
  stProbe_BeginWithName(comment);
  {
    if (headerFrame == NULL || (headerFrame != NULL && headerFrame->bDeleted))
    {
      goto end;
    }

    stProbe_KeyIntAsHex("frameId", headerFrame->frameId);
    stProbe_KeyInt("sequenceNum", headerFrame->sequenceNum);
    stProbe_KeyDouble("frameLength", headerFrame->frameLength);
    stProbe_KeyDouble("frameTime", headerFrame->frameTime);
    stProbe_KeyInt("persistantByteLength", headerFrame->persistantByteLength);
    stProbe_KeyInt("persistantOffset", headerFrame->persistantOffset);
    stProbe_KeyInt("savedVersion", headerFrame->savedVersion);

    if (headerFrame->next)
      stProbe_KeyIntAsHex("next", headerFrame->next->frameId);
    else
      stProbe_KeyIntAsHex("next", 0);

    if (headerFrame->prev)
      stProbe_KeyIntAsHex("prev", headerFrame->prev->frameId);
    else
      stProbe_KeyIntAsHex("prev", 0);

    stProbe_BeginWithName("flags");
    {
      stProbe_KeyBool("bLoaded", headerFrame->bLoaded);
      stProbe_KeyBool("bSaved", headerFrame->bSaved);
      stProbe_KeyBool("bDeleted", headerFrame->bDeleted);
      stProbe_KeyBool("bModified", headerFrame->bModified);
      stProbe_KeyBool("bPersistant", headerFrame->bPersistant);
    }
    stProbe_End();

    stProbe_BeginWithName("id");
    {
      stProbe_KeyInt("min", headerFrame->idMin);
      stProbe_KeyInt("max", headerFrame->idMax);
#if ST_CFG_USE_IDMAP == 1
      stProbe_KeyMemory("map", headerFrame->idMap.bits, headerFrame->idMap.nbDWords * sizeof(u32));
#endif
    }
    stProbe_End();

  }
  end:
  stProbe_End();
}

stBool stHeaderFrameIsEmpty(struct stHeaderFrameT* headerFrame)
{
  return (headerFrame != NULL && headerFrame->idMin == 0 && headerFrame->idMax == 0);
}

stBool stHeaderFrameIsOne(struct stHeaderFrameT* headerFrame)
{
  return (headerFrame != NULL && headerFrame->idMin != 0 && headerFrame->idMax == headerFrame->idMax);
}

stBool stHeaderFrameIsMany(struct stHeaderFrameT* headerFrame)
{
  return (headerFrame != NULL && headerFrame->idMin != 0 && headerFrame->idMax != headerFrame->idMax);
}
