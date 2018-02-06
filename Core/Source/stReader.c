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

#include "stReader.h"
#include "stReaderDelta.h"
#include "stRecording.h"
#include "stError.h"

void stLoadFile_Initialise(stCStr path)
{
  stContext* ctx = stObtainContextLock();
#if 0
  stReadContext readCtx;
  stReaderDeltaFile(&readCtx, path, &ctx->frameDatas, &ctx->headerFrames, &ctx->classes);
#else

  ctx->readCtx = ST_NEW_FROM_HEAP(stReadContext);

  if (ctx->readCtx == NULL)
  {
    stRaiseError(ST_ERROR_READING);
    return;
  }

  stReaderDeltaFile(ctx->readCtx, path, &ctx->frameDatas, &ctx->headerFrames, &ctx->classes);
#endif
  stReleaseContextLock(ctx);
}

void stLoadFile_Shutdown()
{
  stContext* ctx = stGetContext_NoMutex();
  if (ctx->readCtx)
  {
    stReaderDeltaFileClose(ctx->readCtx);
    ST_DELETE_FROM_HEAP(ctx->readCtx);
    ctx->readCtx = NULL;
  }
}

void stLoadFrameData(struct stFrameDataT* frame, struct stHeaderFrameT* header)
{
  stContext* ctx = stGetContext_NoMutex();

  if (ctx->readCtx == NULL)
  {
    stRaiseError(ST_ERROR_READING);
    return;
  }

  stReaderFrameData(ctx->readCtx, frame, header);
}

void stLoadObjectData(struct stObjectDataT* objectData)
{
  stContext* ctx = stGetContext_NoMutex();

  if (ctx->readCtx == NULL)
  {
    stRaiseError(ST_ERROR_READING);
    return;
  }

  stReaderObjectData(ctx->readCtx, objectData);
}
