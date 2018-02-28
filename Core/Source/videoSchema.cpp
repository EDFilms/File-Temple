// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * videoSchema.cpp
 *
 * Schema to describe the rules, types and conversions for the
 * saved Video Frames from the SceneTrack file
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "videoSchema.h"

namespace SceneTrackVideo
{
  Schema CreateVideoSchema()
  {
    Schema schema = std::make_shared<SchemaT>();

    schema->makeClass("PhysicsObject", Frequency::Event, 401, SchemaClassType::VideoFrame)
      .Component("Size")
        .Kind(ST_KIND_SIZE)
        .Type<u32>()
        .NbElements(2)
        .End()
      .Component("Image")
        .Kind(ST_KIND_IMAGE)
        .Type<u8>()
        .NbElements(3)
        .ArrayCapacity(INT_MAX)
      .End();
    return schema;
  }
}
