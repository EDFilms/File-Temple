// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * videoSchema.h
 *
 * Schema to describe the rules, types and conversions for the
 * saved Video Frames from the SceneTrack file
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "videoRequired.h"
#include "expSchema.h"

namespace SceneTrackVideo
{
  namespace SchemaClassType
  {
    enum
    {
      Unknown,
      VideoFrame
    };
  };

  Schema CreateVideoSchema();

}
