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

#ifndef ST_FILE_H
#define ST_FILE_H

#include "stPlatform.h"
#include "stTypes.h"
#include "stWriteBuffer.h"

#define ST_FILE_TYPE_DELTA 0
#define ST_FILE_VERSION_LAST 0xFFFFffff

struct stFileWriterT;
struct stFileReaderT;

typedef struct stFileWriterT stFileWriter;
typedef struct stFileReaderT stFileReader;

typedef struct stFileWriteFormatT
{
  stEnum32(*open)(stFileWriter* w,  stCStr filename, stEnum32 type, stEnum32 flags);
  void(*close)(stFileWriter* w);
  void(*beginChunk)(stFileWriter* w, stFourCc name, u32 number);
  void(*endChunk)(stFileWriter* w);
  void(*terminate)(stFileWriter* w);
  void(*property)(stFileWriter* w, u8 key, stEnum32 valueType, stEnum32 valueEndian, void* value);

  void(*writeCStr)(stFileWriter* w, stCStr value);
  void(*writeU8)(stFileWriter* w,  u8 value, stEnum32 valueEndian);
  void(*writeU16)(stFileWriter* w, u16 value, stEnum32 valueEndian);
  void(*writeU32)(stFileWriter* w, u32 value, stEnum32 valueEndian);
  void(*writeU64)(stFileWriter* w, u64 value, stEnum32 valueEndian);
  void(*writeS8)(stFileWriter* w,  s8 value,  stEnum32 valueEndian);
  void(*writeS16)(stFileWriter* w, s16 value, stEnum32 valueEndian);
  void(*writeS32)(stFileWriter* w, s32 value, stEnum32 valueEndian);
  void(*writeS64)(stFileWriter* w, s64 value, stEnum32 valueEndian);
  void(*writeF32)(stFileWriter* w, f32 value, stEnum32 valueEndian);
  void(*writeF64)(stFileWriter* w, f64 value, stEnum32 valueEndian);
  void(*writeData)(stFileWriter* w, u8* value, u32 length);
} stFileWriteFormat;

typedef struct stFileReadFormatT
{
  stEnum32(*open)(FILE* f, stFileReader* r, stCStr filename, stEnum32* type, stEnum32* flags);
  void(*close)(stFileReader* r);
  stBool(*readChunk)(stFileReader* r, stFourCc* name, u32* number, u32* length);
  stBool(*skipChunk)(stFileReader* r, u32 length);
  stBool(*property)(stFileReader* r, u8* key, stEnum32* valueType);
  stBool(*skipProperty)(stFileReader* r, stEnum32 valueType);

  stBool(*readCStr)(stFileReader* r, stStr* value);
  stBool(*readU8)(stFileReader* r,  u8* value,  stEnum32 valueEndian);
  stBool(*readU16)(stFileReader* r, u16* value, stEnum32 valueEndian);
  stBool(*readU32)(stFileReader* r, u32* value, stEnum32 valueEndian);
  stBool(*readU64)(stFileReader* r, u64* value, stEnum32 valueEndian);
  stBool(*readS8)(stFileReader* r,  s8* value,  stEnum32 valueEndian);
  stBool(*readS16)(stFileReader* r, s16* value, stEnum32 valueEndian);
  stBool(*readS32)(stFileReader* r, s32* value, stEnum32 valueEndian);
  stBool(*readS64)(stFileReader* r, s64* value, stEnum32 valueEndian);
  stBool(*readF32)(stFileReader* r, f32* value, stEnum32 valueEndian);
  stBool(*readF64)(stFileReader* r, f64* value, stEnum32 valueEndian);
  stBool(*readData)(stFileReader* r, u8* value, u32 length);

  u64(*getPosition)(stFileReader* r);
  void(*seek)(stFileReader* r, u64 pos);
} stFileReadFormat;

struct stFileWriterT
{
#if ST_CFG_BUFFERED_WRITE == 1
  stWriteBuffer      buffer;
#endif
  stFileWriteFormat  format;
  void*              userData;
};

struct stFileReaderT
{
  stFileReadFormat   format;
  void*              userData;
};

stEnum32 stFileWriterOpen(stFileWriter* w, const char* path, stEnum32 version, stEnum32 type, stEnum32 flags);

void stFileWriterClose(stFileWriter* w);

void stFileWriterBeginChunk(stFileWriter* w, stCStr fourCcStr, u32 number);

void stFileWriterEndChunk(stFileWriter* w);

void stFileWriterTerminate(stFileWriter* w);

void stFileWriterProperty(stFileWriter* w, u8 key, stEnum32 valueType, stEnum32 valueEndian, void* value);

void stFileWriterWriteData(stFileWriter* w, u8* value, u32 length);

void stFileWriterWriteCStr(stFileWriter* w, stCStr value);

void stFileWriterWriteU8(stFileWriter* w,  u8 value, stEnum32 valueEndian);

void stFileWriterWriteU16(stFileWriter* w, u16 value, stEnum32 valueEndian);

void stFileWriterWriteU32(stFileWriter* w, u32 value, stEnum32 valueEndian);

void stFileWriterWriteU64(stFileWriter* w, u64 value, stEnum32 valueEndian);

void stFileWriterWriteS8(stFileWriter* w,  s8 value, stEnum32 valueEndian);

void stFileWriterWriteS16(stFileWriter* w, s16 value, stEnum32 valueEndian);

void stFileWriterWriteS32(stFileWriter* w, s32 value, stEnum32 valueEndian);

void stFileWriterWriteS64(stFileWriter* w, s64 value, stEnum32 valueEndian);

void stFileWriterWriteF32(stFileWriter* w, f32 value, stEnum32 valueEndian);

void stFileWriterWriteF64(stFileWriter* w, f64 value, stEnum32 valueEndian);

#define ST_FILEWRITER_WRITE_KP_BEGIN(W)  

#define ST_FILEWRITER_WRITE_KP_VALUE_ENDIAN(W, NAME, TYPE, VALUE, ENDIAN) \
  stFileWriterProperty(W, NAME, ST_MACRO_SHORTHANDTYPE_##TYPE, ENDIAN, &VALUE );

#define ST_FILEWRITER_WRITE_KP_VALUE(W, NAME, TYPE, VALUE) \
  stFileWriterProperty(W, NAME, ST_MACRO_SHORTHANDTYPE_##TYPE, ST_ENDIAN, &VALUE );

#define ST_FILEWRITER_WRITE_KP_END(W) \
  stFileWriterProperty(W, 0xFF, 0xFF, ST_ENDIAN, NULL);


stEnum32 stFileReaderOpen(stFileReader* r, const char* path, stEnum32* type, stEnum32* flags);

void stFileReaderClose(stFileReader* r);

u64 stFileReaderGetPosition(stFileReader* r);

void stFileReaderSeek(stFileReader* r, u64 pos);

// Try read a chunk, or return false. Returning false also happens on a chunk terminator.
stBool stFileReaderReadChunk(stFileReader* r, stFourCc* fourCcStr, u32* number, u32* length);

// Skip over the current chunk.
stBool stFileReaderSkipChunk(stFileReader* r, u32 length);

// Try read a property, or return false. Returning false also happens on a property marker.
// Next value should be read as a stFileReaderReadXYZ and correspond to valueType
stBool stFileReaderProperty(stFileReader* w, u8* key, stEnum32* valueType);

stBool stFileReaderSkipProperty(stFileReader* w, stEnum32 valueType);

stBool stFileReaderReadData(stFileReader* w, u8* value, u32 length);

stBool stFileReaderReadCStr(stFileReader* w, stStr* value);

stBool stFileReaderReadU8(stFileReader* w,  u8* value, stEnum32 dstEndian);

stBool stFileReaderReadU16(stFileReader* w, u16* value, stEnum32 dstEndian);

stBool stFileReaderReadU32(stFileReader* w, u32* value, stEnum32 dstEndian);

stBool stFileReaderReadU64(stFileReader* w, u64* value, stEnum32 dstEndian);

stBool stFileReaderReadS8(stFileReader* w,  s8* value, stEnum32 dstEndian);

stBool stFileReaderReadS16(stFileReader* w, s16* value, stEnum32 dstEndian);

stBool stFileReaderReadS32(stFileReader* w, s32* value, stEnum32 dstEndian);

stBool stFileReaderReadS64(stFileReader* w, s64* value, stEnum32 dstEndian);

stBool stFileReaderReadF32(stFileReader* w, f32* value, stEnum32 dstEndian);

stBool stFileReaderReadF64(stFileReader* w, f64* value, stEnum32 dstEndian);

#endif
