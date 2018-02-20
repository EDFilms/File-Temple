// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stReaderDelta.h
 *
 * High level file reading for the SceneTrack delta file formats
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_READER_DELTA_H
#define ST_READER_DELTA_H

#include "stPlatform.h"
#include "stReader.h"

/**
 * Read all delta classes and headers frames into memory
 */
stEnum32 stReaderDeltaFile(stReadContext* context, const char* path, struct stFrameDatasT* frameDatas, struct stHeaderFramesT* headerFrames, struct stClassesT* classes);

/**
 * Read a specific frame data into memory
 */
stEnum32 stReaderFrameData(stReadContext* context, struct stFrameDataT* frame, struct stHeaderFrameT* header);

/**
 * Read a specific object data into memory
 */
stEnum32 stReaderObjectData(stReadContext* context, struct stObjectDataT* objectData);

#endif
