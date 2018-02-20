// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stFormatASCII1.c
 *
 * Formatter interface for the SceneTrack ASCII-1 Format
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stFormatASCII1.h"
#include "stASCII.h"
#include "stSymbols.h"

#include <stdarg.h>

static void sASCII1_writeString(FILE* f, stCStr value)
{
  fprintf(f, "%s", value);
}

#define ST_ASCII1_MAX_CHUNKS 64

typedef struct sASCII1_CtxT
{
  FILE*       f;
  u32         nextChunk;
  stFourCc    chunkName[ST_ASCII1_MAX_CHUNKS];
  u32         lineLength;
  bool        markerSection;
} sASCII1_Ctx;

ST_INLINE int sASCII1_FPrintF(sASCII1_Ctx* ctx, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  int r =vfprintf(ctx->f, fmt, args);
  va_end(args);
  if (r > 0)
  {
    ctx->lineLength += r;
  }

  return r;
}

ST_INLINE void sASCII1_PrintIndent(sASCII1_Ctx* ctx, u32 n)
{
  switch(ctx->nextChunk + n)
  {
    case 0:
    case 1:
    return;
    case 2:
    sASCII1_FPrintF(ctx, " ");
    return;
    case 3:
    sASCII1_FPrintF(ctx, "  ");
    return;
    case 4:
    sASCII1_FPrintF(ctx, "   ");
    return;
    case 5:
    sASCII1_FPrintF(ctx, "    ");
    return;
    case 6:
    sASCII1_FPrintF(ctx, "     ");
    return;
    case 7:
    sASCII1_FPrintF(ctx, "      ");
    return;
  }
  for(u32 i=0;i < ctx->nextChunk - 1;i++)
    sASCII1_FPrintF(ctx, " ");
}

ST_INLINE void sASCII1_WriteBinaryMarker(sASCII1_Ctx* ctx)
{
  int padding = (60 - ctx->lineLength);
  if (padding > 0)
  {
    for(int i=0;i < padding;i++)
      fprintf(ctx->f, " ");
  }
  fprintf(ctx->f, "# ");
}

ST_INLINE void sASCII1_WriteBinaryString(sASCII1_Ctx* ctx, stCStr string)
{
  size_t len = strlen(string);
  for(size_t i=0;i < len;i++)
    fprintf(ctx->f, "%02X ", string[i]);
  fprintf(ctx->f, "00 ");
}

ST_INLINE void sASCII1_WriteBinary(sASCII1_Ctx* ctx, u8 byte)
{
  fprintf(ctx->f, "%02X ", byte);
}

ST_INLINE void sASCII1_WriteBinaryU8(sASCII1_Ctx* ctx, u8 byte)
{
  fprintf(ctx->f, "%02X ", byte);
}

ST_INLINE void sASCII1_WriteBinaryS8(sASCII1_Ctx* ctx, s8 byte)
{
  fprintf(ctx->f, "%02X ", byte);
}

ST_INLINE void sASCII1_WriteBinaryU16(sASCII1_Ctx* ctx, u16 value)
{
  stU16Bytes b;
  b.value = value;

  fprintf(ctx->f, "%02X %02X ", b.bytes[0], b.bytes[1]);
}

ST_INLINE void sASCII1_WriteBinaryS16(sASCII1_Ctx* ctx, s16 value)
{
  stS16Bytes b;
  b.value = value;

  fprintf(ctx->f, "%02X %02X ", b.bytes[0], b.bytes[1]);
}

ST_INLINE void sASCII1_WriteBinaryU32(sASCII1_Ctx* ctx, u32 value)
{
  stU32Bytes b;
  b.value = value;

  fprintf(ctx->f, "%02X %02X %02X %02X ", b.bytes[0], b.bytes[1], b.bytes[2], b.bytes[3]);
}

ST_INLINE void sASCII1_WriteBinaryS32(sASCII1_Ctx* ctx, s32 value)
{
  stS32Bytes b;
  b.value = value;

  fprintf(ctx->f, "%02X %02X %02X %02X ", b.bytes[0], b.bytes[1], b.bytes[2], b.bytes[3]);
}

ST_INLINE void sASCII1_WriteBinaryU64(sASCII1_Ctx* ctx, u64 value)
{
  stU64Bytes b;
  b.value = value;

  fprintf(ctx->f, "%02X %02X %02X %02X %02X %02X %02X %02X ", b.bytes[0], b.bytes[1], b.bytes[2], b.bytes[3], b.bytes[4], b.bytes[5], b.bytes[6], b.bytes[7]);
}

ST_INLINE void sASCII1_WriteBinaryS64(sASCII1_Ctx* ctx, s64 value)
{
  stS64Bytes b;
  b.value = value;

  fprintf(ctx->f, "%02X %02X %02X %02X %02X %02X %02X %02X ", b.bytes[0], b.bytes[1], b.bytes[2], b.bytes[3], b.bytes[4], b.bytes[5], b.bytes[6], b.bytes[7]);
}

ST_INLINE void sASCII1_WriteBinaryF32(sASCII1_Ctx* ctx, f32 value)
{
  stF32Bytes b;
  b.value = value;

  fprintf(ctx->f, "%02X %02X %02X %02X ", b.bytes[0], b.bytes[1], b.bytes[2], b.bytes[3]);
}

ST_INLINE void sASCII1_WriteBinaryF64(sASCII1_Ctx* ctx, f64 value)
{
  stF64Bytes b;
  b.value = value;

  fprintf(ctx->f, "%02X %02X %02X %02X %02X %02X %02X %02X ", b.bytes[0], b.bytes[1], b.bytes[2], b.bytes[3], b.bytes[4], b.bytes[5], b.bytes[6], b.bytes[7]);
}

ST_INLINE void sASCII1_EndLine(sASCII1_Ctx* ctx)
{
  fprintf(ctx->f, "\n");
  ctx->lineLength = 0;
}

stEnum32 sASCII1_open(stFileWriter* w, const char* filename, stEnum32 type, stEnum32 flags)
{
  ST_NULL_CHECK(w);
  sASCII1_Ctx* ctx = ST_NEW_FROM_HEAP(sASCII1_Ctx);
  memset(ctx, 0, sizeof(sASCII1_Ctx));
  w->userData = ctx;
  ctx->nextChunk = 0;

  ST_FOPEN(ctx->f, filename, "wb");

  if (ctx->f == NULL)
  {
    return -1;
  }

  sASCII1_FPrintF(ctx, "STAAAA");
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinary(ctx, 'S');
  sASCII1_WriteBinary(ctx, 'T');
  sASCII1_WriteBinary(ctx, 'A');
  sASCII1_WriteBinary(ctx, 'A');
  sASCII1_WriteBinary(ctx, 'A');
  sASCII1_WriteBinary(ctx, 'A');
  sASCII1_EndLine(ctx);

  sASCII1_FPrintF(ctx, "TYPE=%02X", type);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinary(ctx, (u8) type);
  sASCII1_EndLine(ctx);

  sASCII1_FPrintF(ctx, "FLAGS=%02X", flags);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinary(ctx, (u8) flags);
  sASCII1_EndLine(ctx);

  return 0;
}

static void sASCII1_closeChunk(sASCII1_Ctx* ctx)
{
  if (ctx->nextChunk == 0)
    return;
  sASCII1_PrintIndent(ctx, 0);
  u32 chunkId = ctx->nextChunk - 1;

  char x[5];
  x[0] = ctx->chunkName[chunkId].c[0];
  x[1] = ctx->chunkName[chunkId].c[1];
  x[2] = ctx->chunkName[chunkId].c[2];
  x[3] = ctx->chunkName[chunkId].c[3];
  x[4] = 0;


  sASCII1_FPrintF(ctx, "end '%s'", x);
  sASCII1_EndLine(ctx);

  --ctx->nextChunk;
}
void sASCII1_close(stFileWriter* w)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  if (ctx->f != NULL)
  {
    sASCII1_closeChunk(ctx);

    ST_FCLOSE(ctx->f);
    ctx->f = NULL;
  }

  ST_DELETE_FROM_HEAP(w->userData);
  w->userData = NULL;
}


void sASCII1_serializeBeginChunk(stFileWriter* w, stFourCc name, u32 number)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  ctx->nextChunk++;
  
  char x[5];
  x[0] = name.c[0];
  x[1] = name.c[1];
  x[2] = name.c[2];
  x[3] = name.c[3];
  x[4] = 0;

  u32 chunkId = ctx->nextChunk - 1;
  ctx->chunkName[chunkId] = name;
  sASCII1_PrintIndent(ctx, 0);

  sASCII1_FPrintF(ctx, "begin '%s', %i", x, number);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinary(ctx, x[0]);
  sASCII1_WriteBinary(ctx, x[1]);
  sASCII1_WriteBinary(ctx, x[2]);
  sASCII1_WriteBinary(ctx, x[3]);
  sASCII1_WriteBinaryU32(ctx, number);
  sASCII1_WriteBinaryU32(ctx, 0xCAFEBEEF);
  sASCII1_EndLine(ctx);

}

void sASCII1_serializeEndChunk(stFileWriter* w)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;
  sASCII1_closeChunk(ctx);
}


void sASCII1_terminate(stFileWriter* w)
{
  stFourCc cc;
  cc.c[0] = 'T';
  cc.c[1] = 'E';
  cc.c[2] = 'R';
  cc.c[3] = 'M';
  sASCII1_serializeBeginChunk(w, cc, 0xFFFFffff);
  sASCII1_serializeEndChunk(w);
}

void sASCII1_writeData(stFileWriter* w, u8* data, u32 count)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);
  sASCII1_FPrintF(ctx, "data");
  sASCII1_WriteBinaryMarker(ctx);
  for(u32 i=0;i < count;i++)
  {
    sASCII1_WriteBinary(ctx, data[i]);
  }
  sASCII1_EndLine(ctx);
}

void sASCII1_serializeKeyProperty(stFileWriter* w, u8 key, stEnum32 valueType, stEnum32 valueEndian, void* value)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;
  sASCII1_PrintIndent(ctx, 1);
  
  if (key == 0xFF && valueType == 0xFF)
  {
    sASCII1_FPrintF(ctx, "property Terminator");
    sASCII1_WriteBinaryMarker(ctx);
    sASCII1_WriteBinary(ctx, 0xFF);
    sASCII1_WriteBinary(ctx, 0xFF);
    sASCII1_EndLine(ctx);
  }
  else
  {
    sASCII1_FPrintF(ctx, "property %s %s ", stGetSymbolNamesStr(key), stGetTypeToString(valueType));

    switch(valueType)
    {
      case ST_TYPE_UINT8:     ctx->lineLength += stASCIIEndianFileWriteU8Ex(ctx->f,  (*(u8*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_UINT16:    ctx->lineLength += stASCIIEndianFileWriteU16Ex(ctx->f, (*(u16*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_UINT32:    ctx->lineLength += stASCIIEndianFileWriteU32Ex(ctx->f, (*(u32*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_UINT64:    ctx->lineLength += stASCIIEndianFileWriteU64Ex(ctx->f, (*(u64*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_INT8:      ctx->lineLength += stASCIIEndianFileWriteS8Ex(ctx->f,  (*(s8*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_INT16:     ctx->lineLength += stASCIIEndianFileWriteS16Ex(ctx->f, (*(s16*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_INT32:     ctx->lineLength += stASCIIEndianFileWriteS32Ex(ctx->f, (*(s32*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_INT64:     ctx->lineLength += stASCIIEndianFileWriteS64Ex(ctx->f, (*(s64*)value), valueEndian, ST_ENDIAN_LITTLE); break;
      case ST_TYPE_FLOAT32:   ctx->lineLength += fprintf(ctx->f, "%f", *(f32*)value); break;
      case ST_TYPE_FLOAT64:   ctx->lineLength += fprintf(ctx->f, "%f", *(f64*)value); break;
    }

    sASCII1_WriteBinaryMarker(ctx);

    sASCII1_WriteBinary(ctx, (u8) key);
    sASCII1_WriteBinary(ctx, (u8) valueType);

    switch(valueType)
    {
      case ST_TYPE_UINT8:     sASCII1_WriteBinaryU8(ctx,  (*(u8*)value)); break;
      case ST_TYPE_UINT16:    sASCII1_WriteBinaryU16(ctx, (*(u16*)value)); break;
      case ST_TYPE_UINT32:    sASCII1_WriteBinaryU32(ctx, (*(u32*)value)); break;
      case ST_TYPE_UINT64:    sASCII1_WriteBinaryU64(ctx, (*(u64*)value)); break;
      case ST_TYPE_INT8:      sASCII1_WriteBinaryS8(ctx,  (*(s8*)value)); break;
      case ST_TYPE_INT16:     sASCII1_WriteBinaryS16(ctx, (*(s16*)value)); break;
      case ST_TYPE_INT32:     sASCII1_WriteBinaryS32(ctx, (*(s32*)value)); break;
      case ST_TYPE_INT64:     sASCII1_WriteBinaryS64(ctx, (*(s64*)value)); break;
      case ST_TYPE_FLOAT32:   sASCII1_WriteBinaryF32(ctx, *(f32*)value); break;
      case ST_TYPE_FLOAT64:   sASCII1_WriteBinaryF64(ctx, *(f64*)value); break;
    }

    sASCII1_EndLine(ctx);
  }

}

static void sASCII1_writeCStr(stFileWriter* w, stCStr value)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;
  size_t len = strlen(value);

  sASCII1_PrintIndent(ctx, 1);
  sASCII1_FPrintF(ctx, "string %i %s\n", len, value);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryString(ctx, value);
  sASCII1_EndLine(ctx);

}

static void sASCII1_writeU8(stFileWriter* w,  u8 value, stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);

  ctx->lineLength += fprintf(ctx->f, "u8 ");
  ctx->lineLength += stASCIIEndianFileWriteU8Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryU8(ctx, value);
  sASCII1_EndLine(ctx);

}

static void sASCII1_writeU16(stFileWriter* w, u16 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);
  ctx->lineLength += fprintf(ctx->f, "u16 ");
  ctx->lineLength += stASCIIEndianFileWriteU16Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryU16(ctx, value);
  sASCII1_EndLine(ctx);
}

static void sASCII1_writeU32(stFileWriter* w, u32 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);
  ctx->lineLength += fprintf(ctx->f, "u32 ");
  ctx->lineLength += stASCIIEndianFileWriteU32Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryU32(ctx, value);
  sASCII1_EndLine(ctx);
}

static void sASCII1_writeU64(stFileWriter* w, u64 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);
  ctx->lineLength += fprintf(ctx->f, "u64 ");
  ctx->lineLength += stASCIIEndianFileWriteU64Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryU64(ctx, value);
  sASCII1_EndLine(ctx);
}

static void sASCII1_writeS8(stFileWriter* w,  s8 value, stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);
  ctx->lineLength += fprintf(ctx->f, "s8 ");
  ctx->lineLength += stASCIIEndianFileWriteS8Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryS8(ctx, value);
  sASCII1_EndLine(ctx);
}

static void sASCII1_writeS16(stFileWriter* w, s16 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);
  ctx->lineLength += fprintf(ctx->f, "s16 ");
  ctx->lineLength += stASCIIEndianFileWriteS16Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryS16(ctx, value);
  sASCII1_EndLine(ctx);
}

static void sASCII1_writeS32(stFileWriter* w, s32 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);
  ctx->lineLength += fprintf(ctx->f, "s32 ");
  ctx->lineLength += stASCIIEndianFileWriteS32Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryS32(ctx, value);
  sASCII1_EndLine(ctx);
}

static void sASCII1_writeS64(stFileWriter* w, s64 value, stEnum32 valueEndian)
{
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);
  ctx->lineLength += fprintf(ctx->f, "s64 ");
  ctx->lineLength += stASCIIEndianFileWriteS64Ex(ctx->f, value, valueEndian, ST_ENDIAN_LITTLE);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryS64(ctx, value);
  sASCII1_EndLine(ctx);
}

static void sASCII1_writeF32(stFileWriter* w, f32 value, stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);
  ctx->lineLength += fprintf(ctx->f, "f32 %f", value);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryF32(ctx, value);
  sASCII1_EndLine(ctx);
}

static void sASCII1_writeF64(stFileWriter* w, f64 value, stEnum32 valueEndian)
{
  ST_UNUSED(valueEndian);
  ST_NULL_CHECK(w);
  ST_NULL_CHECK(w->userData);
  sASCII1_Ctx* ctx = w->userData;

  sASCII1_PrintIndent(ctx, 1);
  ctx->lineLength += fprintf(ctx->f, "f64 %f", value);
  sASCII1_WriteBinaryMarker(ctx);
  sASCII1_WriteBinaryF64(ctx, value);
  sASCII1_EndLine(ctx);
}

stEnum32 writer_MakeASCII1(stFileWriter* writer)
{
  writer->format.open                     = sASCII1_open;
  writer->format.close                    = sASCII1_close;
  writer->format.property                 = sASCII1_serializeKeyProperty;
  writer->format.beginChunk               = sASCII1_serializeBeginChunk;
  writer->format.endChunk                 = sASCII1_serializeEndChunk;
  writer->format.terminate                = sASCII1_terminate;
  writer->format.writeData                = sASCII1_writeData;

  writer->format.writeCStr                = sASCII1_writeCStr;
  writer->format.writeU8                  = sASCII1_writeU8;
  writer->format.writeU16                 = sASCII1_writeU16;
  writer->format.writeU32                 = sASCII1_writeU32;
  writer->format.writeU64                 = sASCII1_writeU64;
  writer->format.writeS8                  = sASCII1_writeS8;
  writer->format.writeS16                 = sASCII1_writeS16;
  writer->format.writeS32                 = sASCII1_writeS32;
  writer->format.writeS64                 = sASCII1_writeS64;
  writer->format.writeF32                 = sASCII1_writeF32;
  writer->format.writeF64                 = sASCII1_writeF64;

  return 0;
}



stEnum32 sASCII1_readOpen(FILE* f, stFileReader* r, stCStr filename, stEnum32* type, stEnum32* flags)
{
  ST_UNUSED(f);
  ST_UNUSED(r);
  ST_UNUSED(filename);
  ST_UNUSED(type);
  ST_UNUSED(flags);
  ST_TODO();
  return false;
}

void sASCII1_readClose(stFileReader* r)
{
  ST_UNUSED(r);
  ST_TODO();
}

stBool sASCII1_readChunk(stFileReader* r, stFourCc* name, u32* number, u32* length)
{
  ST_UNUSED(r);
  ST_UNUSED(name);
  ST_UNUSED(number);
  ST_UNUSED(length);
  ST_TODO();
  return false;
}

stBool sASCII1_skipChunk(stFileReader* r, u32 length)
{
  ST_UNUSED(r);
  ST_UNUSED(length);
  ST_TODO();
  return false;
}

stBool sASCII1_property(stFileReader* r, u8* key, stEnum32* valueType)
{
  ST_UNUSED(r);
  ST_UNUSED(key);
  ST_UNUSED(valueType);
  ST_TODO();
  return false;
}

stBool sASCII1_readCStr(stFileReader* r, stStr* value)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_TODO();
  return false;
}

stBool sASCII1_readU8(stFileReader* r,  u8* value,  stEnum32 valueEndian)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(valueEndian);
  ST_TODO();
  return false;
}

stBool sASCII1_readU16(stFileReader* r, u16* value, stEnum32 valueEndian)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(valueEndian);
  ST_TODO();
  return false;
}

stBool sASCII1_readU32(stFileReader* r, u32* value, stEnum32 valueEndian)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(valueEndian);
  ST_TODO();
  return false;
}

stBool sASCII1_readU64(stFileReader* r, u64* value, stEnum32 valueEndian)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(valueEndian);
  ST_TODO();
  return false;
}

stBool sASCII1_readS8(stFileReader* r,  s8* value,  stEnum32 valueEndian)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(valueEndian);
  ST_TODO();
  return false;
}

stBool sASCII1_readS16(stFileReader* r, s16* value, stEnum32 valueEndian)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(valueEndian);
  ST_TODO();
  return false;
}

stBool sASCII1_readS32(stFileReader* r, s32* value, stEnum32 valueEndian)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(valueEndian);
  ST_TODO();
  return false;
}

stBool sASCII1_readS64(stFileReader* r, s64* value, stEnum32 valueEndian)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(valueEndian);
  ST_TODO();
  return false;
}

stBool sASCII1_readF32(stFileReader* r, f32* value, stEnum32 valueEndian)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(valueEndian);
  ST_TODO();
  return false;
}

stBool sASCII1_readF64(stFileReader* r, f64* value, stEnum32 valueEndian)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(valueEndian);
  ST_TODO();
  return false;
}

stBool sASCII1_readData(stFileReader* r, u8* value, u32 length)
{
  ST_UNUSED(r);
  ST_UNUSED(value);
  ST_UNUSED(length);
  ST_TODO();
  return false;
}




stEnum32 reader_MakeASCII1(stFileReader* reader)
{
  reader->format.open = sASCII1_readOpen;
  reader->format.close = sASCII1_readClose;
  reader->format.readChunk = sASCII1_readChunk;
  reader->format.skipChunk = sASCII1_skipChunk;
  reader->format.property = sASCII1_property;
  reader->format.readCStr   = sASCII1_readCStr;
  reader->format.readU8     = sASCII1_readU8;
  reader->format.readU16    = sASCII1_readU16;
  reader->format.readU32    = sASCII1_readU32;
  reader->format.readU64    = sASCII1_readU64;
  reader->format.readS8     = sASCII1_readS8;
  reader->format.readS16    = sASCII1_readS16;
  reader->format.readS32    = sASCII1_readS32;
  reader->format.readS64    = sASCII1_readS64;
  reader->format.readF32    = sASCII1_readF32;
  reader->format.readF64    = sASCII1_readF64;
  reader->format.readData   = sASCII1_readData;

  return 0;
}

