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

typedef union stReadContextT
{
  stDeltaReadContext    delta;
} stReadContext;

stEnum32 stReaderDeltaFile(stReadContext* context, const char* path, struct stFrameDatasT* frameDatas, struct stHeaderFramesT* headerFrames, struct stClassesT* classes);

void stReaderDeltaFileClose(stReadContext* context);

void stLoadFile_Initialise(stCStr path);

void stLoadFile_Shutdown();

void stLoadFrameData(struct stFrameDataT* frame, struct stHeaderFrameT* header);

void stLoadObjectData(struct stObjectDataT* objectData);

#endif
