// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stFile.c
 *
 * File Writing and Reading classes for specific SceneTrack formats
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stFile.h"
#include "stFormat.h"
#include "stFormatBinary1.h"
#include "stFormatASCII1.h"
#include "stBinary.h"

typedef stEnum32 (*serializerSetupFunction)(stFileWriter* writer);

typedef stEnum32 (*readerSetupFunction)(stFileReader* reader);


typedef struct
{
  stEnum32                   version;
  serializerSetupFunction  entry;
} VersionSerializationBinding;

#define ST_FWD_SERIALISER
static const VersionSerializationBinding kWriters[] = {
#include "stFormatBinary1.h"
#include "stFormatASCII1.h"
};
#undef ST_FWD_SERIALISER

typedef struct
{
  stEnum32               version;
  readerSetupFunction  entry;
} VersionReaderBinding;
#define ST_FWD_READER
static const VersionReaderBinding kReaders[] = {
#include "stFormatBinary1.h"
#include "stFormatASCII1.h"
};
#undef ST_FWD_READER

stEnum32 stFileWriterOpen(stFileWriter* w, stCStr path, stEnum32 version, stEnum32 type, stEnum32 flags)
{
  ST_NULL_CHECK(w);

  for(u32 i=0;i < ST_ARRAY_COUNT(kWriters);i++)
  {
    if (kWriters[i].version == version)
    {
      u32 r = kWriters[i].entry(w);
      if (r != 0)
        return r;
      break;
    }
  }

  return w->format.open(w, path, type, flags);
}

void stFileWriterClose(stFileWriter* w)
{
  ST_NULL_CHECK(w);
  w->format.close(w);
}

void stFileWriterBeginChunk(stFileWriter* w, stCStr fourCcStr, u32 number)
{
  ST_NULL_CHECK(w);
  stFourCc cc;
  stMakeFourCc(&cc, fourCcStr);
  w->format.beginChunk(w, cc, number);
}

void stFileWriterEndChunk(stFileWriter* w)
{
  ST_NULL_CHECK(w);
  w->format.endChunk(w);
}

void stFileWriterTerminate(stFileWriter* w)
{
  ST_NULL_CHECK(w);
  w->format.terminate(w);
}

void stFileWriterProperty(stFileWriter* w, u8 key, stEnum32 valueType, stEnum32 valueEndian, void* value)
{
  ST_NULL_CHECK(w);
  w->format.property(w, key, valueType, valueEndian, value);
}

void stFileWriterWriteData(stFileWriter* w, u8* value, u32 length)
{
  ST_NULL_CHECK(w);
  w->format.writeData(w, value, length);
}

void stFileWriterWriteCStr(stFileWriter* w, stCStr value)
{
  ST_NULL_CHECK(w);
  w->format.writeCStr(w, value);
}

void stFileWriterWriteU8(stFileWriter* w,  u8 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  w->format.writeU8(w, value, valueEndian);
}

void stFileWriterWriteU16(stFileWriter* w, u16 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  w->format.writeU16(w, value, valueEndian);
}

void stFileWriterWriteU32(stFileWriter* w, u32 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  w->format.writeU32(w, value, valueEndian);
}

void stFileWriterWriteU64(stFileWriter* w, u64 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  w->format.writeU64(w, value, valueEndian);
}

void stFileWriterWriteS8(stFileWriter* w,  s8 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  w->format.writeS8(w, value, valueEndian);
}

void stFileWriterWriteS16(stFileWriter* w, s16 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  w->format.writeS16(w, value, valueEndian);
}

void stFileWriterWriteS32(stFileWriter* w, s32 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  w->format.writeS32(w, value, valueEndian);
}

void stFileWriterWriteS64(stFileWriter* w, s64 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  w->format.writeS64(w, value, valueEndian);
}

void stFileWriterWriteF32(stFileWriter* w, f32 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  w->format.writeF32(w, value, valueEndian);
}

void stFileWriterWriteF64(stFileWriter* w, f64 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  w->format.writeF64(w, value, valueEndian);
}


stEnum32 stFileReaderOpen(stFileReader* r, stCStr path, stEnum32* type, stEnum32* flags)
{
  ST_NULL_CHECK(r);
  
  FILE* f;
  ST_FOPEN(f, path, "rb");

  if (f == NULL)
  {
    ST_PRINT_ERRORF("Could not open %s file is in use or does not exist", path);
    return -1;
  }

  u8 magic[6] = { 0 };
  ST_FREAD_BYTES_EX(f, magic, 6, -1);

  if (!(magic[0] == 'S' && magic[1] =='T'))
  {
    ST_PRINT_ERRORF("%s is not a Scene Track file", path);
    return -1;
  }

  stEnum32 version = *((u32*)(&magic[2]));

#pragma warning( push )
#pragma warning( disable: 4127 )
  if (ST_ENDIAN != ST_ENDIAN_LITTLE)
    version = stSwapU32(version);
#pragma warning( pop )

  r->format.open = NULL;

  for(u32 i=0;i < ST_ARRAY_COUNT(kWriters);i++)
  {
    if (kWriters[i].version == version)
    {
      u32 rt = kReaders[i].entry(r);
      if (rt != 0)
        return rt;
      break;
    }
  }
  
  if (r->format.open == NULL)
  {
    ST_PRINT_ERRORF("Could not recongize the format of the SceneTrack file: %s. Magic=%08X", path, version);
    return -1;
  }

  stFseek64(f, 0, SEEK_SET);

  return r->format.open(f, r, path, type, flags);
}

void stFileReaderClose(stFileReader* r)
{
  ST_NULL_CHECK(r);
  r->format.close(r);
}

u64 stFileReaderGetPosition(stFileReader* r)
{
  ST_NULL_CHECK(r);
  return r->format.getPosition(r);
}

void stFileReaderSeek(stFileReader* r, u64 pos)
{
  ST_NULL_CHECK(r);
  r->format.seek(r, pos);
}

// Try read a chunk, or return false. Returning false also happens on a chunk terminator.
stBool stFileReaderReadChunk(stFileReader* r, stFourCc* fourCc, u32* number, u32* length)
{
  ST_NULL_CHECK(r);
  return r->format.readChunk(r, fourCc, number, length);
}

// Skip over the current chunk.
stBool stFileReaderSkipChunk(stFileReader* r, u32 length)
{
  ST_NULL_CHECK(r);
  return r->format.skipChunk(r, length);
}

// Try read a property, or return false. Returning false also happens on a property marker.
// Next value should be read as a stFileReaderReadXYZ and correspond to valueType
stBool stFileReaderProperty(stFileReader* r, u8* key, stEnum32* valueType)
{
  ST_NULL_CHECK(r);
  return r->format.property(r, key, valueType);
}

stBool stFileReaderSkipProperty(stFileReader* r, stEnum32 valueType)
{
  ST_NULL_CHECK(r);
  return r->format.skipProperty(r, valueType);
}

stBool stFileReaderReadData(stFileReader* r, u8* value, u32 length)
{
  ST_NULL_CHECK(r);
  return r->format.readData(r, value, length);
}

stBool stFileReaderReadCStr(stFileReader* r, stStr* value)
{
  ST_NULL_CHECK(r);
  return r->format.readCStr(r, value);
}

stBool stFileReaderReadU8(stFileReader* r,  u8* value, stEnum32 dstEndian)
{
  ST_NULL_CHECK(r);
  return r->format.readU8(r, value, dstEndian);
}

stBool stFileReaderReadU16(stFileReader* r, u16* value, stEnum32 dstEndian)
{
  ST_NULL_CHECK(r);
  return r->format.readU16(r, value, dstEndian);
}

stBool stFileReaderReadU32(stFileReader* r, u32* value, stEnum32 dstEndian)
{
  ST_NULL_CHECK(r);
  return r->format.readU32(r, value, dstEndian);
}

stBool stFileReaderReadU64(stFileReader* r, u64* value, stEnum32 dstEndian)
{
  ST_NULL_CHECK(r);
  return r->format.readU64(r, value, dstEndian);
}

stBool stFileReaderReadS8(stFileReader* r,  s8* value, stEnum32 dstEndian)
{
  ST_NULL_CHECK(r);
  return r->format.readS8(r, value, dstEndian);
}

stBool stFileReaderReadS16(stFileReader* r, s16* value, stEnum32 dstEndian)
{
  ST_NULL_CHECK(r);
  return r->format.readS16(r, value, dstEndian);
}

stBool stFileReaderReadS32(stFileReader* r, s32* value, stEnum32 dstEndian)
{
  ST_NULL_CHECK(r);
  return r->format.readS32(r, value, dstEndian);
}

stBool stFileReaderReadS64(stFileReader* r, s64* value, stEnum32 dstEndian)
{
  ST_NULL_CHECK(r);
  return r->format.readS64(r, value, dstEndian);
}

stBool stFileReaderReadF32(stFileReader* r, f32* value, stEnum32 dstEndian)
{
  ST_NULL_CHECK(r);
  return r->format.readF32(r, value, dstEndian);
}

stBool stFileReaderReadF64(stFileReader* r, f64* value, stEnum32 dstEndian)
{
  ST_NULL_CHECK(r);
  return r->format.readF64(r, value, dstEndian);
}
