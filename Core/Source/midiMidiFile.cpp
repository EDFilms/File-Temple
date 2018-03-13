// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stMidiFile.cpp
 *
 * Midi File writer interface
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "midiMidiFile.h"

namespace SceneTrackMidi
{
  
  inline u16 Swap(u16 val) 
  {
    return (val << 8) | (val >> 8 );
  }

  inline  s16 Swap(s16 val) 
  {
    return (val << 8) | ((val >> 8) & 0xFF);
  }

  inline  u32 Swap(u32 val)
  {
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
  }

  inline  s32 Swap(s32 val)
  {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | ((val >> 16) & 0xFFFF);
  }

  inline  u64 Swap(u64 val)
  {
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
  }

  inline  s64 Swap(s64 val)
  {
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
  }
  
  template<typename T> 
  void WriteBigEndian(FILE* f, T value)
  {
#if STEXP_ENDIAN == STEXP_ENDIAN_LITTLE
    value = Swap(value);
#endif
    fwrite(&value, sizeof(T), 1, f);
  }


  void WriteVariableLength(std::vector<u8>& data, s64 number)
  {
    unsigned char b;
    unsigned long buffer;
    buffer = number & 0x7f;
    while ((number >>= 7) > 0) {
      buffer <<= 8;
      buffer |= 0x80 + (number & 0x7f);
    }
    while (1) {
      b = buffer & 0xff;
      data.push_back(b);
      if (buffer & 0x80) buffer >>= 8;
      else break;
    }
  }
  
  void WriteVariableLengthU(std::vector<u8>& data, u64 number)
  {
    unsigned char b;
    unsigned long buffer;
    buffer = number & 0x7f;
    while ((number >>= 7) > 0) {
      buffer <<= 8;
      buffer |= 0x80 + (number & 0x7f);
    }
    while (1) {
      b = buffer & 0xff;
      data.push_back(b);
      if (buffer & 0x80) buffer >>= 8;
      else break;
    }
  }
  
  MidiFile::MidiFile()
  {
    file = nullptr;
  }

  MidiFile::~MidiFile()
  {
    if (file != nullptr)
    {
      Close();
    }
  }

  u8 head1[] = {
    0x05,
    0x0F,
    0x09,
    0x0A,
    0x40
  };

  u8 head2[] = {
    0x05,
    0x0F,
    0x06,
    0x47,
    0x65,
    0x6E,
    0x65,
    0x72,
    0x61,
    0x6C,
    0x20,
    0x4D,
    0x49,
    0x44,
    0x49,
    0x20,
    0x2D,
    0x20,
    0x4D,
    0x69,
    0x63,
    0x72,
    0x6F,
    0x73,
    0x6F,
    0x66,
    0x74,
    0x20,
    0x47,
    0x53,
    0x20,
    0x57,
    0x61,
    0x76,
    0x65,
    0x74,
    0x61,
    0x62,
    0x6C,
    0x65,
    0x20,
    0x53,
    0x79,
    0x6E,
    0x74,
    0x68
  };

  u8 head3[] = {
    0x05,
    0x0F,
    0x2D,
    0x4D,
    0x69,
    0x63,
    0x72,
    0x6F,
    0x73,
    0x6F,
    0x66,
    0x74,
    0x20,
    0x53,
    0x61,
    0x6E,
    0x73,
    0x20,
    0x53,
    0x65,
    0x72,
    0x69,
    0x66,
    0x2C,
    0x38,
    0x2E,
    0x32,
    0x35,
    0x2C,
    0x46,
    0x61,
    0x6C,
    0x73,
    0x65,
    0x2C,
    0x46,
    0x61,
    0x6C,
    0x73,
    0x65,
    0x2C,
    0x31,
    0x2C,
    0x30
  };

  void WriteExtendedData(std::vector<u8>& data, u8* a, u8 length)
  {
    data.push_back(0x00);
    data.push_back(0xFF);
    data.push_back(0x7F);
    data.push_back(length);

    for(u32 i=0;i < length;i++)
      data.push_back(a[i]);
  }

  void MidiFile::Open(const char* path, u16 nbTracks)
  {
    secondsMultiplier = 1.0f;
    trackCount = 0;

#if STEXP_PLATFORM == STEXP_PLATFORM_WINDOWS
    fopen_s(&file, path, "wb");
#else
    file = fopen(path, "wb");
#endif
 
    fwrite("MThd", 4, 1, file);
    WriteBigEndian<u32>(file, 6);         // Header Size
    WriteBigEndian<u16>(file, 1);         // Format - Multiple track format
    WriteBigEndian<u16>(file, 1 + nbTracks);  // Number of tracks

    //ticksPerQuarterNote = 60;     // 100 = 1 second

    tempo               = 960;      // 1,000,000 = useconds per beat?
    ticksPerQuarterNote = 2500;     // 100 = 1 second

    secondsMultiplier   = 1000.0f;  // 100 = 1 second

    // tempo/ticksPerQuarterNote = timing resolution
    // 10ms.   

    WriteBigEndian<u16>(file, (u16) ticksPerQuarterNote);
     
    StartTrack(""); 
    _WriteTimeSig(4, 4, 24, 8);
    _WriteTempo(tempo);

    EndTrack();
  }

  void MidiFile::Close()
  {
    if (file == nullptr)
      return;

    fclose(file);
  }

#define NOTE_UP 0xffffffffffffffffu

  void MidiFile::StartTrack(const std::string& trackName_)
  {
    trackName = trackName_;
    trackData.clear();
    lastTime_us = 0;
    lastTimeFrac = 0;


    for(u32 i=0;i < 127;i++)
      noteTime[i] = NOTE_UP;

//    if (trackCount > 0)
//    {
//      WriteExtendedData(trackData, head1, sizeof(head1));
//      WriteExtendedData(trackData, head2, sizeof(head2));
//      WriteExtendedData(trackData, head3, sizeof(head3));
//    }

    if (trackName_.length() > 0)
    {
      WriteTrackName(0, 0, trackName);
    }

    trackCount++;
  }

  void MidiFile::EndTrack()
  {
    _WriteByte(0x00);
    _WriteByte(0xFF);   // End of Track.
    _WriteByte(0x2F);
    _WriteByte(0x00);

    // Write track header to file
    fwrite("MTrk", 4, 1, file);
    WriteBigEndian<u32>(file, (u32) trackData.size());

    // Write track data to file
    fwrite( (u8*) &trackData[0], 1, (u32) trackData.size(), file);

    trackData.clear();
  }
  
  void MidiFile::_WriteTime(u64 absTime, s64 timeOffsetTicks)
  {
    #if 0
    const f64 MICROSECONDS_F = 1000000.0;

    f64 timeMicroSeconds = (absTime * MICROSECONDS_F);

    s64 s = (u64) timeMicroSeconds + timeOffsetTicks;
    if (s < 0)
      s = 0;

    u64 delta = s - lastTime;
    lastTime = s;

    WriteVariableLength(trackData, delta / 1000);
    #else

    STEXP_UNUSED(timeOffsetTicks);
    
   // ST_LOGF("TIME", "Abs = %llu", absTime);
    //ST_LOGF("TIME", "Last = %llu", lastTime_us);

    u64 delta_us = absTime - lastTime_us;
    lastTime_us = absTime;
   // ST_LOGF("TIME", "Delta = %llu", delta_us);

    u64 ticks = delta_us / 25;
    f64 ticks_d = (f64) delta_us / 25.0;
    f64 frac = ticks_d - (f64) ticks;
    lastTimeFrac += frac;
    while(lastTimeFrac > 1.0)
    { 
      ticks++;
      lastTimeFrac -= 1.0;
    }
     
   // ST_LOGF("TIME", "Ticks.1 = %llu", ticks);
    //ticks += timeOffsetTicks;
    //ST_LOGF("TIME", "Ticks.2 = %llu", ticks);
    WriteVariableLengthU(trackData, ticks); 
    #endif
  }

  void MidiFile::_WriteStatus(u8 category, u8 channel)
  {
    u8 status = (category << 4) | channel;
    _WriteByte(status);
  }

  void MidiFile::_WriteByte(u8 byte)
  {
    trackData.push_back(byte);
  }

  void MidiFile::WriteOnNote(u64 absTime, u8 note, u8 velocity)
  {
    if (noteTime[note] != NOTE_UP)
      return;

    // time
    _WriteTime(absTime);
    // status
    _WriteByte(0x90);
    // note
    _WriteByte(note & 0x7F);
    // velocity
    _WriteByte(velocity & 0x7F);

    noteTime[note] = absTime;
  }

  void MidiFile::WriteOffNote(u64 absTime, u8 note)
  {
    if (noteTime[note] == NOTE_UP)
      return;

    // time
    _WriteTime(absTime);
    // status
    _WriteByte(0x80);
    // note
    _WriteByte(note & 0x7F);
    // velocity
    _WriteByte(0);

    noteTime[note] = NOTE_UP;
  }

  void MidiFile::WriteLyric(u64 time, s64 timeOffsetTicks, const std::string& text)
  {
    _WriteTime(time, timeOffsetTicks);
    _WriteByte(0xFF);
    _WriteByte(0x05);
    _WriteString(text);
  }

  void MidiFile::WriteCuePoint(u64 time, s64 timeOffsetTicks, const std::string& text)
  {
    _WriteTime(time, timeOffsetTicks);
    _WriteByte(0xFF);
    _WriteByte(0x06);
    _WriteString(text);
  }

  void MidiFile::WriteTrackName(u64 time, s64 timeOffsetTicks, const std::string& text)
  {
    _WriteTime(time, timeOffsetTicks);
    _WriteByte(0xFF);
    _WriteByte(0x03);
    _WriteString(text);
  }

  void MidiFile::_WriteTempo(u64 tempo_)
  {
    _WriteByte(0x00);
    _WriteByte(0xFF); // Meta Event
    _WriteByte(0x51);
    _WriteByte(0x03);
    u64 t = (u64) 60000000 / tempo_;
    _WriteByte((u8)((t & 0xFF0000) >> 16));
    _WriteByte((u8)((t & 0x00FF00) >> 8));
    _WriteByte((u8)((t & 0x0000FF)));
  }

  void MidiFile::_WriteCStr(const char* str)
  {
    u32 len = (u32) strlen(str);
    WriteVariableLength(trackData, len);
    for(u32 i=0;i < len;i++)
      _WriteByte(str[i]);
  }

  void MidiFile::_WriteString(const std::string& str)
  {
    u32 len = (u32) str.length();
    WriteVariableLength(trackData, len);
    for(u32 i=0;i < len;i++)
      _WriteByte(str[i]);
  }

  void MidiFile::_WriteTimeSig(u8 num, u8 denom, u8 clocksPerTick, u8 num32nds)
  {
    _WriteByte(0x00);
    _WriteByte(0xFF); // Meta Event
    _WriteByte(0x58);
    _WriteByte(0x04);
    _WriteByte(num);
   
    // denom is encoded as power of 2.
    u8 dd = 0;
    while (denom >>= 1)
      dd++;

    _WriteByte(dd);
    _WriteByte(clocksPerTick);
    _WriteByte(num32nds);
  }
  

}


