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

#ifndef ST_HEADER_FRAME_H
#define ST_HEADER_FRAME_H

#include "stPlatform.h"
#include "stLinearAllocator.h"
#include "stMath.h"
#include "stBits.h"

struct stFrameDataT;
struct stFrameDatasT;
struct stContextT;

typedef struct stBitSetT stIdMap;

struct stHeaderFrameT
{
  struct stHeaderFrameT *next, *prev;
  f64                    frameTime, frameLength;
  // Frame is persistant and should be saved to disk
  stBool32               bPersistant : 1;
  // Frame is has loaded frameData and full object data
  stBool32               bLoaded     : 1;
  // Frame has been modified through editing or recording
  stBool32               bModified   : 1;
  // Frame has been deleted but still exists in memory. It should be ignored.
  stBool32               bDeleted    : 1;
  // Frame is saved
  stBool32               bSaved      : 1;
  u32                    savedVersion;
  u32                    frameId;       // Unique frame Id.
  u32                    sequenceNum;   // Public frame number. Possibly not the same as the position in the linked list.
  u64                    persistantOffset;
  u32                    persistantByteLength;
#if ST_CFG_USE_IDMAP == 1
  stIdMap                idMap;
#endif
  u32                    idMin, idMax;
  struct stFrameDataT*   data;
};

typedef struct stHeaderFrameT stHeaderFrame;

struct stHeaderFramesT
{
  stHeaderFrame            *first, *last;
  u32                       count;
  stDynamicLinearAllocator  mem;
  stQpr                     nextFrameId;
  u32                       minSeqNum, maxSeqNum;
};

typedef struct stHeaderFramesT stHeaderFrames;

ST_INLINE u8 stSaveHeaderFrameFlags(stHeaderFrame* headerFrame)
{
  u8 flags = 0;
  if (headerFrame->bPersistant)
    flags |= (1 << 0);
  return flags;
}

ST_INLINE void stLoadHeaderFrameFlags(u8 flags, stHeaderFrame* headerFrame)
{
  headerFrame->bPersistant = ((flags & (1 << 0)) != 0);
}

struct stHeaderFrameT* stNewHeaderFrame(struct stHeaderFramesT* headerFrames);

struct stHeaderFrameT* stNewHeaderFrameWithId(struct stHeaderFramesT* headerFrames, u32 id);

void stDeleteHeaderFrame(struct stHeaderFrameT* header, struct stHeaderFramesT* headerFrames);

struct stHeaderFrameT* stFindHeaderFrameBySeqNum(struct stHeaderFramesT* headerFrames, u32 seqNum);

struct stHeaderFrameT* stFindHeaderFrameBySeqNumOrNearest(struct stHeaderFramesT* headerFrames, u32 seqNum);

void stInitialiseHeaderFrames(struct stHeaderFramesT* headerFrames);

void stShutdownHeaderFrames(struct stHeaderFramesT* headerFrames);

stBool stLoadFrame_Internal(struct stContextT* context, struct stHeaderFrameT* headerFrame);

void stUnloadFrame(struct stFrameDatasT* frames, struct stHeaderFrameT* headerFrame);

stBool stLoadFrame_NoMutex(struct stHeaderFrameT* headerFrame);

stBool stUnloadFrame_NoMutex(struct stHeaderFrameT* headerFrame);

void stHeaderFramePrint_Impl(struct stHeaderFrameT* headerFrame, const char* comment);

stBool stHeaderFrameIsEmpty(struct stHeaderFrameT* headerFrame);

stBool stHeaderFrameIsOne(struct stHeaderFrameT* headerFrame);

stBool stHeaderFrameIsMany(struct stHeaderFrameT* headerFrame);

#endif
