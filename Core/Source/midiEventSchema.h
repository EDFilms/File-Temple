// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * midiEventSchema.h
 *
 * Schema to describe the rules, types and conversions for a spatial and
 * temporal Event graph to convert into musical notes
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "midiRequired.h"
#include "expSchema.h"

namespace SceneTrackMidi
{
  namespace SchemaClassType
  {
    enum
    {
      Unknown,
      GameObject,
      PhysicsEvent
    };
  };

  Schema CreateEventSchema();

}
