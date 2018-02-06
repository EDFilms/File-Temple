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

#ifndef ST_COMMAND_H
#define ST_COMMAND_H

#include "stPlatform.h"
#include "stLinearAllocator.h"
#include "stPool.h"
#include "stMath.h"
#include "stLazyLoad.h"

#include "stObjectHeader.h"

struct stContextT;

typedef struct stTempCommandBufferT
{
  stCommandKey             keys[ST_COMMAND_MAX_PER_FRAME + 1];
  stCommandBufferIndex     values[ST_COMMAND_MAX_PER_FRAME + 1];
} stTempCommandBuffer;

struct stHeaderFrameT;
struct stHeaderFramesT;
struct stFrameDataT;
struct stFrameDatasT;

stObjectHeader* stNewObjectHeaderInDataFrame(struct stFrameDataT* dataFrame, u32 id);

stObjectHeader* stFindObjectInFrameData(u32 objectId, struct stFrameDataT* dataFrame);

stByte* stPushCommand(u8 customLength, stCommandType commandType, u32 objectId, u8 componentId, u8 flags);
stByte* stCommandWriteU8(stByte*  data,  u8 value);
stByte* stCommandWriteU16(stByte* data, u16 value);
stByte* stCommandWriteU32(stByte* data, u32 value);
stByte* stCommandWriteU64(stByte* data, u64 value);
stByte* stCommandWriteS8(stByte*  data,  s8 value);
stByte* stCommandWriteS16(stByte* data, s16 value);
stByte* stCommandWriteS32(stByte* data, s32 value);
stByte* stCommandWriteS64(stByte* data, s64 value);
stByte* stCommandWriteF32(stByte* data, f32 value);
stByte* stCommandWriteF64(stByte* data, f64 value);
stByte* stCommandWriteString(stByte* data, const char* str, u32 strLength, u32 strCapacity);

ST_PUBLIC_FUNCTION(Named="stSubmit", Text="Submit changes to frame, and maybe write frame to the file.", ThreadSafe, Category="Recording", _frameLength="How long is the frame in seconds")
ST_PUBLIC void stSubmit(f32 frameLength);

ST_PUBLIC_FUNCTION(Named="stGetNumRecordedFrames", Text="Get Number of Frames", Category="Recording", _return="Number of frames recorded");
ST_PUBLIC u32 stGetNumRecordedFrames();

ST_PUBLIC_FUNCTION(Named="stGetNumFrames", Text="Get Number of Frames", Category="Recording", _return="Number of total frames, including recorded and non-recorded");
ST_PUBLIC u32 stGetNumFrames();

ST_PUBLIC_FUNCTION(Named="stGetRecordinTime", Text="Get Length of recording in Seconds", Category="Recording", _return="Get the length of the recording in seconds");
ST_PUBLIC f64 stGetRecordingTime();

ST_PUBLIC_FUNCTION(Named="stFirstFrame", Text="Get First Frame Number", Category="Recording", _return="The frame number of the first frame");
ST_PUBLIC u32 stGetFirstFrame();

ST_PUBLIC_FUNCTION(Named="stFirstFrame", Text="Get Last Frame Number", Category="Recording", _return="The frame number of the last frame");
ST_PUBLIC u32 stGetLastFrame();

ST_PUBLIC_FUNCTION(Named="stReorderFrames", Text="Reorder frames in the given order", Category="Recording")
ST_PUBLIC void stReorderFrames();

void stReorderFramesImpl(struct stHeaderFramesT* headerFrames);

#endif
