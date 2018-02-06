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

#include "fbxRequired.h"
#include "expMath.h"
#include "expTrs.h"

namespace SceneTrackMidi
{

  enum class MidiFileType
  {
    STFBX_PUBLIC_ENUM(Named="FILETYPE_MIDI",          Value=0, For="FileType")
    Midi = 0,
    STFBX_PUBLIC_ENUM(Named="FILETYPE_XML",           Value=1, For="FileType")
    Xml  = 1,

    COUNT
  };

  namespace Settings
  {
    extern MidiFileType            sMidiFileType;

    namespace Trs
    {
      extern ::SceneTrackMidi::Trs::TrsSystem sTransform;
    }

    void PrintSettings();
  }
}
