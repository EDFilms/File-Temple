// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stWriterDelta.h
 *
 * SceneTrack binary delta format writer interface for stWriter
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_DELTA_SERIALIZER_H
#define ST_DELTA_SERIALIZER_H

#include "stPlatform.h"
#include "stWriter.h"

/**
 * Perform a maintence update. Flushing any written data, making sure the classes are written, and write
   any header frames to disk if necessary.
 */
void stWriterDeltaUpdate(stWriteContextData* context, stBool keepFrames);

/**
 * Close the context and releasing any used memory
 */
void stWriterDeltaClose(stWriteContextData* context, stBool keepFrames);

#endif
