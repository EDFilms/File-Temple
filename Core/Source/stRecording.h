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

#ifndef ST_CONTEXT_H
#define ST_CONTEXT_H

#include "stPlatform.h"
#include "stObject.h"
#include "stCommand.h"
#include "stFrameBuffer.h"
#include "stHeaderFrame.h"
#include "stDataFrame.h"
#include "stIterator.h"

#define ST_FORMAT_DELTA 0

#define ST_CONTEXT_TYPE_SAVE 0
#define ST_CONTEXT_TYPE_LOAD 1

#define ST_ABORT_ON_NO_CONTEXT_NO_MUTEX()        if (stHasContext_NoMutex() == false) return;
#define ST_ABORT_ON_NO_CONTEXT_NO_MUTEX_VALUE(X) if (stHasContext_NoMutex() == false) return X;

union stReadContextT;
struct stWriteContextT;

struct stContextRecordingMem
{
  stCommandBuffer             commandBufferMem[ST_COMMAND_BUFFER_COUNT];
  stTempCommandBuffer         tempBuffer;
};

ST_PRIVATE_STRUCT(Named="stContext")
struct stContextT
{
  struct stContextT             *next, *prev;
  u32                            type;
  u32                            format;
  u32                            id;
  f64                            recordingTime;
  u32                            nbSubmittedFrames;
  stBool                         strictMode;
  union stReadContextT*          readCtx;
  struct stWriteContextT*        writeCtx;
  stClasses                      classes;
  stFrameDatas                   frameDatas;
  stHeaderFrames                 headerFrames;
  stIterators                    iterators;
  stCommandBuffer*               commandBuffer[ST_COMMAND_BUFFER_COUNT];
  struct stContextRecordingMem*  recordingMem;
};

typedef struct stContextT stContext;

ST_PUBLIC_FUNCTION(Named="stCreateRecording", Text="Create Recording for writing", Category="Recording", ThreadSafe)
ST_PUBLIC u32  stCreateRecording();

ST_PUBLIC_FUNCTION(Named="stOpenRecording", Text="Open Recording for reading", Category="Recording", ThreadSafe)
ST_PUBLIC u32  stOpenRecording(stCStr path);

ST_PUBLIC_FUNCTION(Named="stCloseRecording", Text="Shutdown Recording", Category="Recording", ThreadSafe)
ST_PUBLIC void stCloseRecording(u32 recording);

ST_PUBLIC_FUNCTION(Named="stChangeRecording", Text="Change Recording", Category="Recording", ThreadSafe)
ST_PUBLIC void stChangeActiveRecording(u32 recording);

ST_PUBLIC_FUNCTION(Named="stAppendSaveRecording", Text="Save Recording", Category="Recording", ThreadSafe)
ST_PUBLIC void stAppendSaveRecording(stCStr path, stEnum32 format, u32 frames);

ST_PUBLIC_FUNCTION(Named="stSaveRecording", Text="Save Recording", Category="Recording", ThreadSafe)
ST_PUBLIC void stSaveRecording(stCStr path, stEnum32 format);

ST_PUBLIC_ENUM(Named="FramePool", Value=0, For="Memory")
#define ST_MEMORY_FRAME_POOL 0
ST_PUBLIC_ENUM(Named="FramePage", Value=0, For="Memory")
#define ST_MEMORY_FRAME_PAGE 1

ST_PUBLIC_FUNCTION(Named="stMemoryReserve", Text="Reserve Memory, Pages or Frames to help with performance.", ThreadSafe, Category="Recording")
ST_PUBLIC void stMemoryReserve(s32 type, s32 value);

stContext* stGetContext_NoMutex();

stBool     stHasContext_NoMutex();

//ST_PRIVATE_FUNCTION(Named="stGetContext", Text="Obtain Current Recording and Spinlock", ThreadSafe)
stContext* stObtainContextLock();

//ST_PRIVATE_FUNCTION(Named="stGetContext", Text="Release Current Recording and Spinlock", ThreadSafe)
void stReleaseContextLock(stContext* context);


#endif
