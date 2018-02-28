// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * midiSettings.cpp
 *
 * A collection of user settings and enumerations
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "midiSettings.h"

namespace SceneTrackMidi
{
  namespace Settings
  {

    const char* sMidiFileTypeStr[] = {
      "MIDI",
      "XML"
    };

    MidiFileType                sMidiFileType = MidiFileType::Midi;

    namespace Trs
    {
      ::SceneTrackMidi::Trs::TrsSystem sTransform = ::SceneTrackMidi::Trs::TrsSystem();
    }

    void PrintSettings()
    {
      STFBX_PRINT_INFOF("Settings.FileFormat = %s",     sMidiFileTypeStr[(size_t) sMidiFileType]);
      Settings::Trs::sTransform.Print(Node::Transform);
    }

  }
}
