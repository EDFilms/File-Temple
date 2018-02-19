// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stFile.h
 *
 * File Writing and Reading classes for specific SceneTrack formats
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

/**
 * SceneTrack File-format writer interface
 */
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

/**
 * SceneTrack File-format reader interface
 */
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

/**
 * SceneTrack file writer
 */
struct stFileWriterT
{
#if ST_CFG_BUFFERED_WRITE == 1
  stWriteBuffer      buffer;
#endif
  stFileWriteFormat  format;
  void*              userData;
};

/**
 * SceneTrack file reader
 */
struct stFileReaderT
{
  stFileReadFormat   format;
  void*              userData;
};

/**
 * Create a filewriter and open file, according to version and specified scene-track fileformat
 * @return If the file was opened or not
 * @see stFileWriterClose
 */
stEnum32 stFileWriterOpen(stFileWriter* w, const char* path, stEnum32 version, stEnum32 type, stEnum32 flags);

/**
 * Close a filewriter thus closing the file
 */
void stFileWriterClose(stFileWriter* w);

/**
 * Start writing a chunk of a given FourCC and associated number
 * @see stFileWriterEndChunk
 */
void stFileWriterBeginChunk(stFileWriter* w, stCStr fourCcStr, u32 number);

/**
 * Mark the end of the chunk
 * @see stFileWriterBeginChunk
 * @see stFileWriterTerminate
 */
void stFileWriterEndChunk(stFileWriter* w);

/**
 * Write a terminator marker
 */
void stFileWriterTerminate(stFileWriter* w);

/**
 * Write a key-pair property into the file
 */
void stFileWriterProperty(stFileWriter* w, u8 key, stEnum32 valueType, stEnum32 valueEndian, void* value);

/**
 * Write a byte-array of given length to the file
 */
void stFileWriterWriteData(stFileWriter* w, u8* value, u32 length);

/**
 * Write a C style null terminated string to a file
 */
void stFileWriterWriteCStr(stFileWriter* w, stCStr value);

/**
 * Write an u8 to a file with the given endian.
 */
void stFileWriterWriteU8(stFileWriter* w,  u8 value, stEnum32 valueEndian);

/**
 * Write an u16 to a file with the given endian.
 */
void stFileWriterWriteU16(stFileWriter* w, u16 value, stEnum32 valueEndian);

/**
 * Write an u32 to a file with the given endian.
 */
void stFileWriterWriteU32(stFileWriter* w, u32 value, stEnum32 valueEndian);

/**
 * Write an u64 to a file with the given endian.
 */
void stFileWriterWriteU64(stFileWriter* w, u64 value, stEnum32 valueEndian);

/**
 * Write an s8 to a file with the given endian.
 */
void stFileWriterWriteS8(stFileWriter* w,  s8 value, stEnum32 valueEndian);

/**
 * Write an s16 to a file with the given endian.
 */
void stFileWriterWriteS16(stFileWriter* w, s16 value, stEnum32 valueEndian);

/**
 * Write an s32 to a file with the given endian.
 */
void stFileWriterWriteS32(stFileWriter* w, s32 value, stEnum32 valueEndian);

/**
 * Write an s64 to a file with the given endian.
 */
void stFileWriterWriteS64(stFileWriter* w, s64 value, stEnum32 valueEndian);

/**
 * Write an f32 to a file with the given endian.
 */
void stFileWriterWriteF32(stFileWriter* w, f32 value, stEnum32 valueEndian);

/**
 * Write an f64 to a file with the given endian.
 */
void stFileWriterWriteF64(stFileWriter* w, f64 value, stEnum32 valueEndian);

#define ST_FILEWRITER_WRITE_KP_BEGIN(W)  

#define ST_FILEWRITER_WRITE_KP_VALUE_ENDIAN(W, NAME, TYPE, VALUE, ENDIAN) \
  stFileWriterProperty(W, NAME, ST_MACRO_SHORTHANDTYPE_##TYPE, ENDIAN, &VALUE );

#define ST_FILEWRITER_WRITE_KP_VALUE(W, NAME, TYPE, VALUE) \
  stFileWriterProperty(W, NAME, ST_MACRO_SHORTHANDTYPE_##TYPE, ST_ENDIAN, &VALUE );

#define ST_FILEWRITER_WRITE_KP_END(W) \
  stFileWriterProperty(W, 0xFF, 0xFF, ST_ENDIAN, NULL);
  
/**
 * Create a filereader and open a file. Flags and type are passed back out as pointers
 * @return If the file was opened or not
 * @see stFileWriterClose
 */
stEnum32 stFileReaderOpen(stFileReader* r, const char* path, stEnum32* type, stEnum32* flags);

/**
 * Close a filereader thus closing the file
 */
void stFileReaderClose(stFileReader* r);

/**
 * Get position in file in bytes
 */
u64 stFileReaderGetPosition(stFileReader* r);

/**
 * Seek ahead in file in bytes
 */
void stFileReaderSeek(stFileReader* r, u64 pos);

/** 
 * Try read a chunk, or return false. Returning false also happens on a chunk terminator.
 */
stBool stFileReaderReadChunk(stFileReader* r, stFourCc* fourCcStr, u32* number, u32* length);

/**
 * Skip over the current chunk.
 */
stBool stFileReaderSkipChunk(stFileReader* r, u32 length);

/**
 * Try read a property, or return false. Returning false also happens on a property marker.
 * Next value should be read as a stFileReaderReadXYZ and correspond to valueType
 */
stBool stFileReaderProperty(stFileReader* w, u8* key, stEnum32* valueType);

/**
 * Try and skip over property of type
 */
stBool stFileReaderSkipProperty(stFileReader* w, stEnum32 valueType);

/**
 * Try and read data from a file into a byte array of given length
 */
stBool stFileReaderReadData(stFileReader* w, u8* value, u32 length);

/**
 * Try and read a null terminated string from a file into the value.
 */
stBool stFileReaderReadCStr(stFileReader* w, stStr* value);

/**
 * Try and read a u8 value into the destination endian.
 */
stBool stFileReaderReadU8(stFileReader* w,  u8* value, stEnum32 dstEndian);

/**
 * Try and read a u16 value into the destination endian.
 */
stBool stFileReaderReadU16(stFileReader* w, u16* value, stEnum32 dstEndian);

/**
 * Try and read a u32 value into the destination endian.
 */
stBool stFileReaderReadU32(stFileReader* w, u32* value, stEnum32 dstEndian);

/**
 * Try and read a u64 value into the destination endian.
 */
stBool stFileReaderReadU64(stFileReader* w, u64* value, stEnum32 dstEndian);

/**
 * Try and read a s8 value into the destination endian.
 */
stBool stFileReaderReadS8(stFileReader* w,  s8* value, stEnum32 dstEndian);

/**
 * Try and read a s16 value into the destination endian.
 */
stBool stFileReaderReadS16(stFileReader* w, s16* value, stEnum32 dstEndian);

/**
 * Try and read a s32 value into the destination endian.
 */
stBool stFileReaderReadS32(stFileReader* w, s32* value, stEnum32 dstEndian);

/**
 * Try and read a s64 value into the destination endian.
 */
stBool stFileReaderReadS64(stFileReader* w, s64* value, stEnum32 dstEndian);

/**
 * Try and read a f32 value
 * @note dstEndian is ignored
 */
stBool stFileReaderReadF32(stFileReader* w, f32* value, stEnum32 dstEndian);

/**
 * Try and read a f64 value
 * @note dstEndian is ignored
 */
stBool stFileReaderReadF64(stFileReader* w, f64* value, stEnum32 dstEndian);

#endif
