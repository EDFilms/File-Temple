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

#include "stWriter.h"
#include "stWriterDelta.h"
#include "stRecording.h"
#include "stObject.h"
#include "stError.h"

stEnum32 serializerDeltaOpen(stWriteContextData* context, const char* path, stEnum32 version, stEnum32 flags, struct stFrameDatasT* frameDatas, struct stClassesT* classes);

stEnum32 stSerializerOpenDelta(stWriteContext* serializer, const char* path, stEnum32 version, stEnum32 flags, struct stFrameDatasT* frameDatas, struct stClassesT* classes)
{
  ST_NULL_CHECK(serializer);
  serializer->close = stWriterDeltaClose;
  serializer->update = stWriterDeltaUpdate;

  return serializerDeltaOpen(&serializer->data, path, version, flags, frameDatas, classes);
}

void stSerializerClose(stWriteContext* context)
{
  context->close(&context->data, context->keepFrames);
}

void stSerializerUpdate(stWriteContext* context)
{
  context->update(&context->data, context->keepFrames);
}

void stSaveFile_Update()
{
  stContext* ctx = stObtainContextLock();
  if (ctx->writeCtx)
  {
    stSerializerUpdate(ctx->writeCtx);
  }
  stReleaseContextLock(ctx);
}

void stSaveFile_Shutdown()
{
  stContext* ctx = stGetContext_NoMutex();
  if (ctx->writeCtx)
  {
    stSerializerClose(ctx->writeCtx);
    ST_DELETE_FROM_HEAP(ctx->writeCtx);
    ctx->writeCtx = NULL;
  }
}

void stSaveFile_Initialise(stCStr path, stEnum32 format, u32 frames, stBool writeNow, stBool keepFrames)
{
  stContext* ctx = stObtainContextLock();

  if (ctx->writeCtx != NULL)
  {
    ST_PRINT_CHATTERF(ST_DBG_FN "Write Context already exists. Closing.");
    stSerializerClose(ctx->writeCtx);
    ST_DELETE_FROM_HEAP(ctx->writeCtx);
  }

  ctx->writeCtx = ST_NEW_FROM_HEAP(stWriteContext);

  if (ctx->writeCtx == NULL)
  {
    stRaiseError(ST_ERROR_WRITING);
    ST_PRINT_ERROR("Write context was not created. Reason: Null from Heap");
    return;
  }

  ctx->writeCtx->appendMode = !writeNow;
  ctx->writeCtx->keepFrames = keepFrames;
  ctx->writeCtx->frames = frames;
  ctx->writeCtx->frameSaveCounter = frames;

  strcpy(ctx->writeCtx->target, path);

  ctx->writeCtx->format = ST_FORMAT_BINARY_1;

  if (format == ST_FORMAT_BINARY)
    ctx->writeCtx->format = ST_FORMAT_BINARY_1;
  else if (format == ST_FORMAT_TEXT)
    ctx->writeCtx->format = ST_FORMAT_ASCII_1;

  s32 r = 0;
  if ((r = stSerializerOpenDelta(ctx->writeCtx, path, ctx->writeCtx->format, 0, &ctx->frameDatas, &ctx->classes)) != 0)
  {
    stRaiseError(ST_ERROR_DISK);
    ST_DELETE_FROM_HEAP(ctx->writeCtx);
    ctx->writeCtx = NULL;
    ST_PRINT_ERRORF("Write context was not created. Reason: stSerializerOpenDelta failed: r=%i", r);
  }
  
  if (ctx->writeCtx && writeNow)
  {
    ST_PRINT_CHATTER(ST_DBG_FN "[WriteNow] Begin");
    // Save existing frames.
    stSerializerUpdate(ctx->writeCtx);


    ST_PRINT_CHATTERF(ST_DBG_FN "[WriteNow] Checking to save header frames: Context Type == ST_CONTEXT_TYPE_LOAD=%i, HeaderFrame Count = %i", ctx->type == ST_CONTEXT_TYPE_LOAD, ctx->headerFrames.count);
    // Save header frames.
    if (ctx->type == ST_CONTEXT_TYPE_LOAD && ctx->headerFrames.count != 0)
    {
      ST_PRINT_CHATTER(ST_DBG_FN "[WriteNow] Writing Header Frames and Data");
      stBool keepFramesPushed = ctx->writeCtx->keepFrames;
      ctx->writeCtx->keepFrames = false;

      stHeaderFrame* headerFrame = ctx->headerFrames.first;

      while(headerFrame != NULL)
      {
        stHeaderFrame* next = headerFrame->next;
        stLoadFrame_Internal(ctx, headerFrame);
        stSerializerUpdate(ctx->writeCtx);
        headerFrame = next;
      }

      ctx->writeCtx->keepFrames = keepFramesPushed;

      ST_PRINT_CHATTER(ST_DBG_FN "[WriteNow] Wrote Header Frames and Data");
    }

    ST_PRINT_CHATTER(ST_DBG_FN "[WriteNow] Closing.");
    stSerializerClose(ctx->writeCtx);
    ST_DELETE_FROM_HEAP(ctx->writeCtx);
    ctx->writeCtx = NULL;
    ST_PRINT_CHATTER(ST_DBG_FN "[WriteNow] Closed.");
  }

  stReleaseContextLock(ctx);
}
