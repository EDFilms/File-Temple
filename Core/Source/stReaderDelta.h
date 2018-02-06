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

#ifndef ST_READER_DELTA_H
#define ST_READER_DELTA_H

#include "stPlatform.h"
#include "stReader.h"

stEnum32 stReaderDeltaFile(stReadContext* context, const char* path, struct stFrameDatasT* frameDatas, struct stHeaderFramesT* headerFrames, struct stClassesT* classes);

stEnum32 stReaderFrameData(stReadContext* context, struct stFrameDataT* frame, struct stHeaderFrameT* header);

stEnum32 stReaderObjectData(stReadContext* context, struct stObjectDataT* objectData);

#endif
