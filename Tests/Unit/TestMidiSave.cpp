#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "SceneTrackStatic.h"
#include "SceneTrackMidi.h"

TEST_CASE("Midi-Save", "[Midi]")
{
  midiConvertSceneTrackFile("midi_test.st", "midi_test.mid");
}
