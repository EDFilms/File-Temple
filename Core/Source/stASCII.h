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

#ifndef ST_ASCII_H
#define ST_ASCII_H

#include "stPlatform.h"
#include "stBinary.h"
#include <inttypes.h>

ST_INLINE int stASCIIEndianFileWriteU8Ex(FILE* file, u8 value, stEnum32 src, stEnum32 dst)
{
  ST_UNUSED(src);
  ST_UNUSED(dst);
  return fprintf(file, "%" PRIu8, value);
}

ST_INLINE int stASCIIEndianFileWriteS8Ex(FILE* file, s8 value, stEnum32 src, stEnum32 dst)
{
  ST_UNUSED(src);
  ST_UNUSED(dst);
  return fprintf(file, "%" PRIi8, value);
}

ST_INLINE int stASCIIEndianFileWriteU16Ex(FILE* file, u16 value, stEnum32 src, stEnum32 dst)
{
  stU16Bytes b;

  if (src != dst)
    b.value = stSwapU16(value);
  else
    b.value = value;

  return fprintf(file, "%" PRIu16, b.value);
}

ST_INLINE int stASCIIEndianFileWriteS16Ex(FILE* file, s16 value, stEnum32 src, stEnum32 dst)
{
  stU16Bytes b;

  if (src != dst)
    b.value = stSwapS16(value);
  else
    b.value = value;

  return fprintf(file, "%" PRIi16, b.value);
}

ST_INLINE int  stASCIIEndianFileWriteU32Ex(FILE* file, u32 value, stEnum32 src, stEnum32 dst)
{
  stU32Bytes b;

  if (src != dst)
    b.value = stSwapU32(value);
  else
    b.value = value;

  return fprintf(file, "%" PRIu32, b.value);
}

ST_INLINE int stASCIIEndianFileWriteS32Ex(FILE* file, s32 value, stEnum32 src, stEnum32 dst)
{
  stS32Bytes b;

  if (src != dst)
    b.value = stSwapS32(value);
  else
    b.value = value;

  return fprintf(file, "%" PRIi32, b.value);
}


ST_INLINE int  stASCIIEndianFileWriteU64Ex(FILE* file, u64 value, stEnum32 src, stEnum32 dst)
{
  stU64Bytes b;

  if (src != dst)
    b.value = stSwapU64(value);
  else
    b.value = value;

  return fprintf(file, "%" PRIu64, b.value);
}


ST_INLINE int stASCIIEndianFileWriteS64Ex(FILE* file, s64 value, stEnum32 src, stEnum32 dst)
{
  stS64Bytes b;

  if (src != dst)
    b.value = stSwapS64(value);
  else
    b.value = value;

  return fprintf(file, "%" PRIi64, b.value);
}

ST_INLINE int stASCIIEndianFileWriteHexU64Ex(FILE* file, u64 value, stEnum32 src, stEnum32 dst)
{
  stU64Bytes b;

  if (src != dst)
    b.value = stSwapU64(value);
  else
    b.value = value;

  return fprintf(file, "%" PRIX64, b.value);
}


ST_INLINE int stASCIIEndianFileWriteHexS64Ex(FILE* file, s64 value, stEnum32 src, stEnum32 dst)
{
  stS64Bytes b;

  if (src != dst)
    b.value = stSwapS64(value);
  else
    b.value = value;

  return fprintf(file, "%" PRIX64, b.value);
}
#endif
