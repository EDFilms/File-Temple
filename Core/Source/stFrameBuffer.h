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

#ifndef ST_FRAME_H
#define ST_FRAME_H

#include "stPlatform.h"
#include "stCommandType.h"
#include "stLinearAllocator.h"
#include "stObjectData.h"

struct stContextT;

typedef struct stCommandBufferT
{
  // Number of commands
  stCommandBufferIndex     size;
  // Frame Number
  u32                      frameNumber;
  // Frame Time (as 64-bit) - Time when the frame starts
  f64                      frameTime;
  // Frame Delta (as 64-bit) - Length of Frame
  f64                      frameLength;
  // Pool and allocated memory
  stDynamicLinearAllocator mem;

  // Object Datas
  struct stObjectDatasT    objectDatas;

  // Keys associated to command
  stCommandKey             keys[ST_COMMAND_MAX_PER_FRAME + 1];

  // Index-Pointer to data corresponding to keys
  stCommandBufferIndex     values[ST_COMMAND_MAX_PER_FRAME + 1];

  // Pointer to command data (in mem) in order of submission, values (with keys) point to these to make sense of
  // the data.
  u8*                      data[ST_COMMAND_MAX_PER_FRAME + 1];

} stCommandBuffer;

void stInitialiseCommandBuffer(stCommandBuffer* buffer);

void stShutdownCommandBuffer(stCommandBuffer* buffer);

void stSortCommands(stCommandBuffer* buffer);

void stCycleBuffers(struct stContextT* context);

#endif
