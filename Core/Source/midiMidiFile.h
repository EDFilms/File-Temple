// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	1.0.0
 */

#pragma once

#include "midiRequired.h"
#include "midiSettings.h"
#include <stdio.h>

namespace SceneTrackMidi
{

  enum Instrument
  {
    AcousticGrand = 0,
  };

  class MidiFile
  {
    FILE* file;
    std::string     trackName;
    std::vector<u8> trackData;
    u64             noteTime[127];
    u64             lastTime; // This may be per note.
    u64             tempo, ticksPerQuarterNote;
    f64             secondsMultiplier;
    u32             trackCount;

  public:

    MidiFile();
    ~MidiFile();

    void Open(const char* path, u16 nbTracks);
    void Close();

    void StartTrack(const std::string& trackName_);
    void EndTrack();

    void WriteOnNote(f64 time, u8 note, u8 velocity);
    void WriteOffNote(f64 time, u8 note);
    void WriteLyric(f64 time, s64 timeOffsetTicks, const std::string& text);
    void WriteCuePoint(f64 time, s64 timeOffsetTicks, const std::string& text);
    void WriteTrackName(f64 time, s64 timeOffsetTicks, const std::string& text);

  private:

    void _WriteTimeSig(u8 num, u8 denom, u8 clocksPerTick, u8 num32nds);
    void _WriteTempo(u64 tempo_);
    void _WriteCStr(const char* str);
    void _WriteString(const std::string& str);

    void _WriteTime(f64 time, s64 timeOffsetTicks = 0);
    void _WriteStatus(u8 category, u8 channel);
    void _WriteByte(u8 byte);
  };
}
