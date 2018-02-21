// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stWriter.h
 *
 * Writer counterpart to the stReadContext
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_WRITER_H
#define ST_WRITER_H

#include "stPlatform.h"
#include "stFormat.h"
#include "stFile.h"

struct stClassesT;

/**
 * SceneTrack delta format writer context
 */
typedef struct stDeltaWriteContextT
{
  u32                  type;
  stFileWriter         writer;
  struct stFrameDatasT*       frameDatas;
  stBool               wroteClasses;
  struct stClassesT*   classes;
  u32                  begin, end;
  u32                  serialiseCookie;
} stDeltaWriteContext;

/**
 * Write context data interface
 */
union stWriteContextDataT
{
  stDeltaWriteContext delta;
};

typedef union stWriteContextDataT stWriteContextData;

typedef void(*stWriterUpdateFunction)(stWriteContextData* context, stBool keepFrames);
typedef void(*stWriterCloseFunction)(stWriteContextData* context, stBool keepFrames);

/**
 * Write context interface
 */
typedef struct stWriteContextT
{
  stWriterCloseFunction  close;
  stWriterUpdateFunction update;
  stWriteContextData     data;
  u32                    frames;
  s32                    frameSaveCounter;
  u32                    format;
  stBool                 appendMode, keepFrames;
  char                   target[256];
} stWriteContext;

/**
 * Open a writer with the path, and given context. For writing frames, classes to the file.
 */
stEnum32 stSerializerOpenDelta(stWriteContext* context, const char* path, stEnum32 version, stEnum32 flags, struct stFrameDatasT* frameDatas, struct stClassesT* classes);

/**
 * Close the write context, releasing any memory and the file handle
 */
void stSerializerClose(stWriteContext* context);

/**
 * Call update on the context which acts as a maintence writing function.
 */
void stSerializerUpdate(stWriteContext* context);

ST_PUBLIC_ENUM(Named="Binary",  Value=0, For="Format")
#define ST_FORMAT_BINARY  0

// Private. Deprecated.
#define ST_FORMAT_TEXT 1

/**
 * Generic master all-in-one function to start writing to a file
 * writer context is stored in the active stContext->writeCtx variable
 */
void stSaveFile_Initialise(stCStr path, stEnum32 format, u32 everyNFrames, stBool writeNow, stBool keepFrames);

/**
 * Close the active stContext->writeCtx. Flushing any data to disk, closing the handle, and releasing any used memory
 */
void stSaveFile_Shutdown();

/**
 * Perform a maintence update and/or flush data to disk on the stContext->writeCtx
 */
void stSaveFile_Update();

#endif
