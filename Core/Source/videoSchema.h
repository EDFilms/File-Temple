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
