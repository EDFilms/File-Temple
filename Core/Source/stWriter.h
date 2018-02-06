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

#ifndef ST_WRITER_H
#define ST_WRITER_H

#include "stPlatform.h"
#include "stFormat.h"
#include "stFile.h"

struct stClassesT;

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

union stWriteContextDataT
{
  stDeltaWriteContext delta;
};

typedef union stWriteContextDataT stWriteContextData;

typedef void(*stWriterUpdateFunction)(stWriteContextData* context, stBool keepFrames);
typedef void(*stWriterCloseFunction)(stWriteContextData* context, stBool keepFrames);

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

stEnum32 stSerializerOpenDelta(stWriteContext* context, const char* path, stEnum32 version, stEnum32 flags, struct stFrameDatasT* frameDatas, struct stClassesT* classes);
void stSerializerClose(stWriteContext* context);
void stSerializerUpdate(stWriteContext* context);

ST_PUBLIC_ENUM(Named="Binary",  Value=0, For="Format")
#define ST_FORMAT_BINARY  0

// Private. Deprecated.
#define ST_FORMAT_TEXT 1

void stSaveFile_Initialise(stCStr path, stEnum32 format, u32 everyNFrames, stBool writeNow, stBool keepFrames);

void stSaveFile_Shutdown();

void stSaveFile_Update();

#endif
