// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stReader.h
 *
 * High-level SceneTrack file reader classes and functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_READER_H
#define ST_READER_H

#include "stPlatform.h"
#include "stFormat.h"
#include "stFile.h"
#include "stObjectData.h"

struct stFrameDatasT;
struct stClassesT;
struct stHeaderFramesT;
struct stHeaderFrameT;
struct stFrameDataT;

/**
 * High-level context and state class for the delta file reader, frame datas and header frames
 */
typedef struct stDeltaReadContextT
{
  u32                           type;
  stFileReader                  reader;
  struct stFrameDatasT*        frameDatas;
  struct stHeaderFramesT*       headerFrames;
  struct stHeaderFrameT*        lastHeaderFrame;
  stBool                        readClasses;
  u32                           begin, end;
} stDeltaReadContext;

/**
 * Read Context for all types of read contexts
 */
typedef union stReadContextT
{
  stDeltaReadContext    delta;
} stReadContext;

/**
 * Open a file for a ReadContext with the path, with the pre-allocated frameDatas, headerFarmes and classes
 */
stEnum32 stReaderDeltaFile(stReadContext* context, const char* path, struct stFrameDatasT* frameDatas, struct stHeaderFramesT* headerFrames, struct stClassesT* classes);

/**
 * Close a ReadContext from reading, and close the file. 
 * Does not release memory
 */
void stReaderDeltaFileClose(stReadContext* context);

/**
 * Load file from disk and open for reading.
 * stReadContext is placed into the main stContext's <code>readCtx</code> property
 * This is the main function for opening a file for reading.
 * @see stLoadFile_Shutdown
 */
void stLoadFile_Initialise(stCStr path);

/**
 * Close the file from the disk for reading. Uses the main stContext readCtx
 * This the main function for closing a file for reading
 * @see stLoadFile_Initialise
 */
void stLoadFile_Shutdown();

/**
 * Read the frame data from the the main stContext readCtx
 */
void stLoadFrameData(struct stFrameDataT* frame, struct stHeaderFrameT* header);

/**
 * Read the object data from the the main stContext readCtx
 */
void stLoadObjectData(struct stObjectDataT* objectData);

#endif
