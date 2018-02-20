// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stHeaderFrame.h
 *
 * "Hot"/Common data for each recorded frame which may be in a partial or fully loaded state in memory.
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
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

/**
 * Frame Header class which represents meta-data and holds and references the "colder" frame data
 * stFrameData data.
 */
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

/**
 * Container class and id information for stHeaderFrames
 */
struct stHeaderFramesT
{
  stHeaderFrame            *first, *last;
  u32                       count;
  stDynamicLinearAllocator  mem;
  stQpr                     nextFrameId;
  u32                       minSeqNum, maxSeqNum;
};

typedef struct stHeaderFramesT stHeaderFrames;

/**
 * Convert stHeaderFrame flags in a u8 as bit flags
 */
ST_INLINE u8 stSaveHeaderFrameFlags(stHeaderFrame* headerFrame)
{
  u8 flags = 0;
  if (headerFrame->bPersistant)
    flags |= (1 << 0);
  return flags;
}

/**
 * Convert u8 bit flags into stHeaderFrame flags
 */
ST_INLINE void stLoadHeaderFrameFlags(u8 flags, stHeaderFrame* headerFrame)
{
  headerFrame->bPersistant = ((flags & (1 << 0)) != 0);
}

/**
 * Create and allocate memory for a new header frame
 * @return The new stHeaderFrame or NULL if there is a raised error or the linear allocator has run out of memory
 */
struct stHeaderFrameT* stNewHeaderFrame(struct stHeaderFramesT* headerFrames);

/**
 * Create and allocate memory for a new header frame, with a specific id.
 * @return The new stHeaderFrame or NULL if there is a raised error or the linear allocator has run out of memory
 */
struct stHeaderFrameT* stNewHeaderFrameWithId(struct stHeaderFramesT* headerFrames, u32 id);

/**
 * Mark a header frame as deleted (but does not release any memory)
 */
void stDeleteHeaderFrame(struct stHeaderFrameT* header, struct stHeaderFramesT* headerFrames);

/**
 * Find a frame by it's sequence number (frame number) or return NULL
 * @return The header frame or NULL
 */
struct stHeaderFrameT* stFindHeaderFrameBySeqNum(struct stHeaderFramesT* headerFrames, u32 seqNum);

/**
 * Find a frame by it's sequence number (frame number) or the nearest frame to it
 * @return The header frame or a header frame near to it
 */
struct stHeaderFrameT* stFindHeaderFrameBySeqNumOrNearest(struct stHeaderFramesT* headerFrames, u32 seqNum);

/**
 * Initialise header frames class and preallocate memory
 */
void stInitialiseHeaderFrames(struct stHeaderFramesT* headerFrames);

/**
 * Release header frames class and release memory
 */
void stShutdownHeaderFrames(struct stHeaderFramesT* headerFrames);

/**
 * Create a stHeaderFrame and load it in from the currently opened file and position
 */
stBool stLoadFrame_Internal(struct stContextT* context, struct stHeaderFrameT* headerFrame);

/**
 * Unload the frame, and recycle the memory into the internal header frame pool
 */
void stUnloadFrame(struct stFrameDatasT* frames, struct stHeaderFrameT* headerFrame);

/**
 * Call stLoadFrame_Internal without using the shared mutex
 */
stBool stLoadFrame_NoMutex(struct stHeaderFrameT* headerFrame);

/**
 * Call stUnloadFrame without using the shared mutex
 */
stBool stUnloadFrame_NoMutex(struct stHeaderFrameT* headerFrame);

/**
 * Print out to stdout the header frame as debug information
 */
void stHeaderFramePrint_Impl(struct stHeaderFrameT* headerFrame, const char* comment);

/**
 * Does the header frame no changes?
 */
stBool stHeaderFrameIsEmpty(struct stHeaderFrameT* headerFrame);

/**
 * Does the header frame exactly one change?
 */
stBool stHeaderFrameIsOne(struct stHeaderFrameT* headerFrame);

/**
 * Does the header frame has more than one change?
 */
stBool stHeaderFrameIsMany(struct stHeaderFrameT* headerFrame);

#endif
