// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stMidiFile.h
 *
 * Midi File writer interface
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
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

  /**
   * A Midi file writer class that represents the file handle,
   * and provides low-level functions to write the Midi information
   * according to the Midi schema.
   */
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

    /**
     * Open a file for writing. If the file already exists
     * then the file is cleared.
     */
    void Open(const char* path, u16 nbTracks);

    /**
     * Close the file from writing
     */
    void Close();

    /**
     * Start writing a track with the given name. 
     * The track id is the number tracks already written.
     */
    void StartTrack(const std::string& trackName_);

    /**
     * End writing the track
     */
    void EndTrack();

    /**
     * Write a note that is down. (Pressed)
     */
    void WriteOnNote(f64 time, u8 note, u8 velocity);

    /**
     * Write a note that is up. (Released)
     */
    void WriteOffNote(f64 time, u8 note);

    /**
     * Write a lyric at a specific time
     */
    void WriteLyric(f64 time, s64 timeOffsetTicks, const std::string& text);
    
    /**
     * Write a cue point at a specific time
     */
    void WriteCuePoint(f64 time, s64 timeOffsetTicks, const std::string& text);
    
    /**
     * Write a track name at a specific time
     */
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
