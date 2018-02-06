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

#include "stFormatBinary1.h"
#include "stBinary.h"
#include "stFormat.h"
#include "stError.h"

static void sBinary1_writeU8Impl(FILE* f, u8 c)
{
  fwrite(&c, 1, 1, f);
}

static void sBinary1_writeS8Impl(FILE* f, s8 c)
{
  fwrite(&c, 1, 1, f);
}

static void sBinary1_writeString(FILE* f, stCStr value)
{
  size_t len = strlen(value);
  fwrite(value, len, 1, f);

  sBinary1_writeU8Impl(f, 0);
}

static s32 sBinary1_peekStringLength(FILE* f)
{
  u64 at = stFtell64(f);
  u64 at2 = at;

  while(feof(f) == 0)
  {
    char c = 0;
    ST_FREAD_BYTES_EX(f, (&c), 1, -1);
    if (c == 0)
    {
      at2 = stFtell64(f);
      break;
    }
  }

  s64 size = at2 - at;
  //fseek(f, at, SEEK_SET);
  stFseek64(f, at, SEEK_SET);
  return (u32) size;
}

#define ST_BINARY1_MAX_CHUNKS 64

typedef struct sBinary1_CtxT
{
  FILE*       f;
  u32         nextChunk;
  stFourCc    chunkId[ST_BINARY1_MAX_CHUNKS];
  u64         chunkStart[ST_BINARY1_MAX_CHUNKS];
} sBinary1_Ctx;

static void sBinary1_closeChunk(sBinary1_Ctx* ctx)
{
  if (ctx->nextChunk == 0)
    return;

  u32 chunkId = ctx->nextChunk - 1;

  if (ctx->chunkId[chunkId].c[0] == 0)
    return;
  
  u64 now = stFtell64(ctx->f);
  u64 length = now - ctx->chunkStart[chunkId];

  stFseek64(ctx->f, ctx->chunkStart[chunkId] - sizeof(u32), SEEK_SET);
  fwrite(&length, sizeof(u32), 1, ctx->f);
  stFseek64(ctx->f, now, SEEK_SET);

  ctx->chunkId[chunkId].c[0] = 0;
  ctx->chunkStart[chunkId] = 0;
  --ctx->nextChunk;
}

stEnum32 sBinary1_open(stFileWriter* w, const char* filename, stEnum32 type, stEnum32 flags)
{
  ST_NULL_CHECK(w);
  sBinary1_Ctx* ctx = ST_NEW_FROM_HEAP(sBinary1_Ctx);
  memset(ctx, 0, sizeof(sBinary1_Ctx));
  w->userData = ctx;
  ctx->nextChunk = 0;
  ctx->chunkStart[0] = 0;
  ctx->chunkId[0].c[0] = 0;

  ST_FOPEN(ctx->f, filename, "wb");

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return -1;
  }

#if ST_CFG_BUFFERED_WRITE == 1
  w->buffer.size = ST_KILOBYTES(256);
  w->buffer.file = ctx->f;
  stWriteBuffer_Initialise(&w->buffer);
#endif

  sBinary1_writeU8Impl(ctx->f, 'S');
  sBinary1_writeU8Impl(ctx->f, 'T');
  stBinaryEndianFileWriteU32Ex(ctx->f, ST_FORMAT_BINARY_1, ST_ENDIAN, ST_ENDIAN_LITTLE);
  sBinary1_writeU8Impl(ctx->f, (u8) type);
  sBinary1_writeU8Impl(ctx->f, (u8) flags);

  return 0;
}

void sBinary1_close(stFileWriter* w)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;
  
  if (ctx->f != NULL)
  {
#if ST_CFG_BUFFERED_WRITE == 1
    stWriteBuffer_Shutdown(&w->buffer);
#endif
    sBinary1_closeChunk(ctx);

    ST_FCLOSE(ctx->f); 

    ctx->f = NULL;
  }

  ST_DELETE_FROM_HEAP(w->userData);
  w->userData = NULL;
}

void sBinary1_serializeBeginChunk(stFileWriter* w, stFourCc name, u32 number)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return;
  }

  u32 id = ctx->nextChunk;
  ctx->nextChunk++;

  // chunk header is 12 bytes.
  // 0-3  - chunk fourCC
  // 4-7  - number
  // 7-11 - length (u32). written as zero initially.
  ctx->chunkId[id] = name;
  ctx->chunkStart[id] = stFtell64(ctx->f);

  fwrite(&name.c, 4, 1, ctx->f);
  stBinaryEndianFileWriteU32Ex(ctx->f, number, ST_ENDIAN, ST_ENDIAN_LITTLE);
  u32 length = 0;
  stBinaryEndianFileWriteU32Ex(ctx->f, length, ST_ENDIAN, ST_ENDIAN_LITTLE);
  ctx->chunkStart[id] = stFtell64(ctx->f);
}

void sBinary1_serializeEndChunk(stFileWriter* w)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);

  sBinary1_Ctx* ctx = w->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return;
  }

  sBinary1_closeChunk(ctx);
}

void sBinary1_terminate(stFileWriter* w)
{
  stFourCc cc;
  cc.c[0] = 'T';
  cc.c[1] = 'E';
  cc.c[2] = 'R';
  cc.c[3] = 'M';
  sBinary1_serializeBeginChunk(w, cc, 0xFFFFffff);
  sBinary1_serializeEndChunk(w);
}

void sBinary1_serializeKeyProperty(stFileWriter* w, u8 key, stEnum32 valueType, stEnum32 valueEndian, void* value)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return;
  }

  if (value == NULL && valueType == ST_TYPE_MARKER && key == 0xFF)
  {
    sBinary1_writeU8Impl(ctx->f,  0xFF);
    sBinary1_writeU8Impl(ctx->f,  0xFF);
  }
  else
  {
    sBinary1_writeU8Impl(ctx->f, key);
    sBinary1_writeU8Impl(ctx->f, (u8) valueType);

    switch(valueType)
    {
      case ST_TYPE_UINT8:     sBinary1_writeU8Impl(ctx->f,  (*(u8*)value)); break;
      case ST_TYPE_UINT16:    stBinaryEndianFileWriteU16Ex(ctx->f, (*(u16*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_UINT32:    stBinaryEndianFileWriteU32Ex(ctx->f, (*(u32*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_UINT64:    stBinaryEndianFileWriteU64Ex(ctx->f, (*(u64*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_INT8:      sBinary1_writeS8Impl(ctx->f,  (*(s8*)value)); break;
      case ST_TYPE_INT16:     stBinaryEndianFileWriteS16Ex(ctx->f, (*(s16*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_INT32:     stBinaryEndianFileWriteS32Ex(ctx->f, (*(s32*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_INT64:     stBinaryEndianFileWriteS64Ex(ctx->f, (*(s64*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_FLOAT32:   fwrite(value, sizeof(f32), 1, ctx->f); break;
      case ST_TYPE_FLOAT64:   fwrite(value, sizeof(f64), 1, ctx->f); break;
    }
  }

}

void sBinary1_serializeData(stFileWriter* w, u8* data, u32 count)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return;
  }

  if (fwrite(data, count, 1, ctx->f) == 0)
  {
    stRaiseError(ST_ERROR_DISK);
    return;
  }

}

static void sBinary1_writeCStr(stFileWriter* w, stCStr value)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;
  sBinary1_writeString(ctx->f, value);
}

static void sBinary1_writeU8(stFileWriter* w,  u8 value, stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  fwrite(&value, sizeof(u8), 1, ctx->f);
}

static void sBinary1_writeU16(stFileWriter* w, u16 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  stBinaryEndianFileWriteU16Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
}

static void sBinary1_writeU32(stFileWriter* w, u32 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  stBinaryEndianFileWriteU32Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
}

static void sBinary1_writeU64(stFileWriter* w, u64 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  stBinaryEndianFileWriteU64Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
}

static void sBinary1_writeS8(stFileWriter* w,  s8 value, stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  fwrite(&value, sizeof(s8), 1, ctx->f);
}

static void sBinary1_writeS16(stFileWriter* w, s16 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  stBinaryEndianFileWriteS16Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
}

static void sBinary1_writeS32(stFileWriter* w, s32 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  stBinaryEndianFileWriteS32Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
}

static void sBinary1_writeS64(stFileWriter* w, s64 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  stBinaryEndianFileWriteS64Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
}

static void sBinary1_writeF32(stFileWriter* w, f32 value, stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  fwrite(&value, sizeof(f32), 1, ctx->f);
}

static void sBinary1_writeF64(stFileWriter* w, f64 value, stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sBinary1_Ctx* ctx = w->userData;

  fwrite(&value, sizeof(f64), 1, ctx->f);
}

stEnum32 writer_MakeBinary1(stFileWriter* writer)
{
  writer->format.open                     = sBinary1_open;
  writer->format.close                    = sBinary1_close;
  writer->format.terminate                = sBinary1_terminate;
  writer->format.property                 = sBinary1_serializeKeyProperty;
  writer->format.beginChunk               = sBinary1_serializeBeginChunk;
  writer->format.endChunk                 = sBinary1_serializeEndChunk;
  writer->format.writeData                = sBinary1_serializeData;
  writer->format.writeCStr                = sBinary1_writeCStr;
  writer->format.writeU8                  = sBinary1_writeU8;
  writer->format.writeU16                 = sBinary1_writeU16;
  writer->format.writeU32                 = sBinary1_writeU32;
  writer->format.writeU64                 = sBinary1_writeU64;
  writer->format.writeS8                  = sBinary1_writeS8;
  writer->format.writeS16                 = sBinary1_writeS16;
  writer->format.writeS32                 = sBinary1_writeS32;
  writer->format.writeS64                 = sBinary1_writeS64;
  writer->format.writeF32                 = sBinary1_writeF32;
  writer->format.writeF64                 = sBinary1_writeF64;

  return 0;
}



stEnum32 sBinary1_readOpen(FILE* f, stFileReader* r, stCStr filename, stEnum32* type, stEnum32* flags)
{
  ST_UNUSED(filename);
  ST_NULL_CHECK(r);
  sBinary1_Ctx* ctx = ST_NEW_FROM_HEAP(sBinary1_Ctx);
  memset(ctx, 0, sizeof(sBinary1_Ctx));
  r->userData = ctx;
  ctx->nextChunk = 0;
  ctx->chunkStart[0] = 0;
  ctx->chunkId[0].c[0] = 0;
  
  ctx->f = f;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return 1;
  }

  stFseek64(ctx->f, 6, SEEK_SET);

  u8 typeB, flagsB; 
  ST_FREAD_BYTES(ctx->f, &typeB, 1);
  ST_FREAD_BYTES(ctx->f, &flagsB, 1);
  *type = typeB;
  *flags = flagsB;

  return 0;
}

void sBinary1_readClose(stFileReader* r)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  if (ctx->f != NULL)
  {
    ST_FCLOSE(ctx->f);
    ctx->f = NULL;
  }
  else
  {
    stRaiseError(ST_ERROR_DISK);
  }

  ST_DELETE_FROM_HEAP(r->userData);
  r->userData = NULL;
}

u64 sBinary1_getPosition(stFileReader* r)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return 0;
  }

  return stFtell64(ctx->f);
}

void sBinary1_seek(stFileReader* r, u64 pos)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return;
  }

  if (stFseek64(ctx->f, pos, SEEK_SET) != 0)
  {
    stRaiseError(ST_ERROR_DISK);
  }
}

stBool sBinary1_readChunk(stFileReader* r, stFourCc* name, u32* number, u32* length)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return false;
  }

  ST_FREAD_BYTES(ctx->f, &name->c[0], 4);
  stBinaryEndianFileReadU32Ex(ctx->f, number, ST_ENDIAN_LITTLE, ST_ENDIAN);
  stBinaryEndianFileReadU32Ex(ctx->f, length, ST_ENDIAN_LITTLE, ST_ENDIAN);

  if (name->c[0] == 'T' && name->c[1] == 'E' && name->c[2] == 'R' && name->c[3] == 'M')
    return false;

  return true;
}

stBool sBinary1_skipChunk(stFileReader* r, u32 length)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return false;
  }

  return stFseek64(ctx->f, length, SEEK_CUR) == 0;
}

stBool sBinary1_skipProperty(stFileReader* r, stEnum32 valueType)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return false;
  }

  u64 length = stGetTypeMemorySize(valueType, 1, 1);

  return stFseek64(ctx->f, length, SEEK_CUR) == 0;
}

stBool sBinary1_property(stFileReader* r, u8* key, stEnum32* valueType)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return false;
  }

  ST_FREAD_BYTES(ctx->f, key, sizeof(u8));
  u8 valueType8;
  ST_FREAD_BYTES(ctx->f, &valueType8, sizeof(u8));
  *valueType = valueType8;

  if (*key == 0xFF && valueType8 == 0xFF)
    return false;

  return true;
}

stBool sBinary1_readCStr(stFileReader* r, stStr* value)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  if (ctx->f == NULL)
  {
    stRaiseError(ST_ERROR_DISK);
    return false;
  }

  s32 len = sBinary1_peekStringLength(ctx->f);
  if (len == -1)
    return false;

  if (len == 0)
  {
    (*value) = NULL;
  }
  else
  {
    stStr str = ST_ALLOCATE_MEM(len + 1, ST_DEFAULT_ALIGNMENT);
    ST_FREAD_BYTES(ctx->f, str, len);
    str[len] = 0;
    (*value) = str;
  }
  
  return true;
}

stBool sBinary1_readU8(stFileReader* r,  u8* value,  stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, sizeof(u8));
  return true;
}

stBool sBinary1_readU16(stFileReader* r, u16* value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, sizeof(u16));
  
  if (ST_ENDIAN != valueEndian)
    *value = stSwapU16(*value);
  
  return true;
}

stBool sBinary1_readU32(stFileReader* r, u32* value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, sizeof(u32));

  if (ST_ENDIAN != valueEndian)
    *value = stSwapU32(*value);

  return true;
}

stBool sBinary1_readU64(stFileReader* r, u64* value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, sizeof(u64));

  if (ST_ENDIAN != valueEndian)
    *value = stSwapU64(*value);

  return true;
}

stBool sBinary1_readS8(stFileReader* r,  s8* value,  stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, sizeof(s8));
  return true;
}

stBool sBinary1_readS16(stFileReader* r, s16* value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, sizeof(s16));

  if (ST_ENDIAN != valueEndian)
    *value = stSwapS16(*value);

  return true;
}

stBool sBinary1_readS32(stFileReader* r, s32* value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, sizeof(s32));

  if (ST_ENDIAN != valueEndian)
    *value = stSwapS32(*value);

  return true;
}

stBool sBinary1_readS64(stFileReader* r, s64* value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, sizeof(s64));

  if (ST_ENDIAN != valueEndian)
    *value = stSwapS64(*value);

  return true;
}

stBool sBinary1_readF32(stFileReader* r, f32* value, stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, sizeof(f32));

  return true;
}

stBool sBinary1_readF64(stFileReader* r, f64* value, stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, sizeof(f64));

  return true;
}

stBool sBinary1_readData(stFileReader* r, u8* value, u32 length)
{
  ST_NULL_CHECK(r);
  ST_NULL_CHECK(r->userData);
  sBinary1_Ctx* ctx = r->userData;

  ST_FREAD_BYTES(ctx->f, value, length);

  return true;
}




stEnum32 reader_MakeBinary1(stFileReader* reader)
{
  reader->format.open = sBinary1_readOpen;
  reader->format.close = sBinary1_readClose;
  reader->format.getPosition = sBinary1_getPosition;
  reader->format.seek = sBinary1_seek;
  reader->format.readChunk = sBinary1_readChunk;
  reader->format.skipChunk = sBinary1_skipChunk;
  reader->format.property   = sBinary1_property;
  reader->format.skipProperty = sBinary1_skipProperty;
  reader->format.readCStr   = sBinary1_readCStr;
  reader->format.readU8     = sBinary1_readU8;
  reader->format.readU16    = sBinary1_readU16;
  reader->format.readU32    = sBinary1_readU32;
  reader->format.readU64    = sBinary1_readU64;
  reader->format.readS8     = sBinary1_readS8;
  reader->format.readS16    = sBinary1_readS16;
  reader->format.readS32    = sBinary1_readS32;
  reader->format.readS64    = sBinary1_readS64;
  reader->format.readF32    = sBinary1_readF32;
  reader->format.readF64    = sBinary1_readF64;
  reader->format.readData   = sBinary1_readData;

  return 0;
}
