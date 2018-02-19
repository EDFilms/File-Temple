// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stCommand.h
 *
 * Recorded Object and Variable commands creation, management
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
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

/**
 * Non-sorted temporary fixed sized command buffer organised by keys (stCommandKey) and values.
 */
typedef struct stTempCommandBufferT
{
  stCommandKey             keys[ST_COMMAND_MAX_PER_FRAME + 1];
  stCommandBufferIndex     values[ST_COMMAND_MAX_PER_FRAME + 1];
} stTempCommandBuffer;

struct stHeaderFrameT;
struct stHeaderFramesT;
struct stFrameDataT;
struct stFrameDatasT;

/**
 * Create a object header of id for a creation, destruction or change-of during the frame.
 * @return The stObjectHeader
 */
stObjectHeader* stNewObjectHeaderInDataFrame(struct stFrameDataT* dataFrame, u32 id);

/**
 * Find a object header of the object's id in a data frame, or NULL.
 * @return the stObjectHeader or NULL.
 */
stObjectHeader* stFindObjectInFrameData(u32 objectId, struct stFrameDataT* dataFrame);

/**
 * Push an un-sorted command to the current frame command buffer.
 *
 * Command must be of any stCommandType and exactly of the format given.
 * 
 * Used with stCommandWrite functions for command data.
 *
 * @see stCommandType
 * @see stCommandWriteU8
 * @see stCommandWriteU16
 * @see stCommandWriteU32
 * @see stCommandWriteU64
 * @see stCommandWriteS8
 * @see stCommandWriteS16
 * @see stCommandWriteS32
 * @see stCommandWriteS64
 * @see stCommandWriteF32
 * @see stCommandWriteF64
 * @see stCommandWriteString
 * @return A pointer to write the data to (using stCommandWrite)
 */
stByte* stPushCommand(u8 customLength, stCommandType commandType, u32 objectId, u8 componentId, u8 flags);

/**
 * Append an u8 value to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteU8(stByte*  data,  u8 value);

/**
 * Append an u16 value to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteU16(stByte* data, u16 value);

/**
 * Append an u32 value to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteU32(stByte* data, u32 value);

/**
 * Append an u64 value to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteU64(stByte* data, u64 value);

/**
 * Append an s8 value to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteS8(stByte*  data,  s8 value);

/**
 * Append an s16 value to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteS16(stByte* data, s16 value);

/**
 * Append an s32 value to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteS32(stByte* data, s32 value);

/**
 * Append an s64 value to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteS64(stByte* data, s64 value);

/**
 * Append an f32 value to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteF32(stByte* data, f32 value);

/**
 * Append an f64 value to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteF64(stByte* data, f64 value);

/**
 * Append an fixed length C-style ASCII string to a pushed command.
 * @see stPushCommand
 * @return A pointer to write the next data to.
 */
stByte* stCommandWriteString(stByte* data, const char* str, u32 strLength, u32 strCapacity);

/**
 * Submit changes to frame of given length, and maybe write frame to the file.
 */
ST_PUBLIC_FUNCTION(Named="stSubmit", Text="Submit changes to frame, and maybe write frame to the file.", ThreadSafe, Category="Recording", _frameLength="How long is the frame in seconds")
ST_PUBLIC void stSubmit(f32 frameLength);

/**
 * Get the number of recorded frames
 */
ST_PUBLIC_FUNCTION(Named="stGetNumRecordedFrames", Text="Get Number of Frames", Category="Recording", _return="Number of frames recorded");
ST_PUBLIC u32 stGetNumRecordedFrames();

/**
 * Get the total number of frames
 */
ST_PUBLIC_FUNCTION(Named="stGetNumFrames", Text="Get Number of Frames", Category="Recording", _return="Number of total frames, including recorded and non-recorded");
ST_PUBLIC u32 stGetNumFrames();

/**
 * Get the recording time in seconds
 */
ST_PUBLIC_FUNCTION(Named="stGetRecordinTime", Text="Get Length of recording in Seconds", Category="Recording", _return="Get the length of the recording in seconds");
ST_PUBLIC f64 stGetRecordingTime();

/**
 * Get the first frame number
 */
ST_PUBLIC_FUNCTION(Named="stFirstFrame", Text="Get First Frame Number", Category="Recording", _return="The frame number of the first frame");
ST_PUBLIC u32 stGetFirstFrame();

/**
 * Get the last frame number
 */
ST_PUBLIC_FUNCTION(Named="stFirstFrame", Text="Get Last Frame Number", Category="Recording", _return="The frame number of the last frame");
ST_PUBLIC u32 stGetLastFrame();

/**
 * Reorder the frames in sequential order
 */
ST_PUBLIC_FUNCTION(Named="stReorderFrames", Text="Reorder frames in the given order", Category="Recording")
ST_PUBLIC void stReorderFrames();

/**
 * Implementation of stReorderFrames
 */
void stReorderFramesImpl(struct stHeaderFramesT* headerFrames);

#endif
