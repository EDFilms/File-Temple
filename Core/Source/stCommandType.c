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

#include "stCommand.h"
#include "stTypes.h"
#include <limits.h>

const u8 stCommandCountDiskSize[ST_CMD_COUNT] = {
  0,                                         // ST_CMD_NONE,
  sizeof(u32),                               // ST_CMD_OBJ_NEW,
  0,                                         // ST_CMD_OBJ_DELETE,
  1 * sizeof(u8),                            // ST_CMD_OBJ_VAL_1U8,
  1 * sizeof(u16),                           // ST_CMD_OBJ_VAL_1U16,
  1 * sizeof(u32),                           // ST_CMD_OBJ_VAL_1U32,
  1 * sizeof(u64),                           // ST_CMD_OBJ_VAL_1U64,
  1 * sizeof(s8),                            // ST_CMD_OBJ_VAL_1S8,
  1 * sizeof(s16),                           // ST_CMD_OBJ_VAL_1S16,
  1 * sizeof(s32),                           // ST_CMD_OBJ_VAL_1S32,
  1 * sizeof(s64),                           // ST_CMD_OBJ_VAL_1S64,
  1 * sizeof(f32),                           // ST_CMD_OBJ_VAL_1F32,
  1 * sizeof(f64),                           // ST_CMD_OBJ_VAL_1F64,
  2 * sizeof(u8),                            // ST_CMD_OBJ_VAL_2U8,
  2 * sizeof(u16),                           // ST_CMD_OBJ_VAL_2U16,
  2 * sizeof(u32),                           // ST_CMD_OBJ_VAL_2U32,
  2 * sizeof(u64),                           // ST_CMD_OBJ_VAL_2U64,
  2 * sizeof(s8),                            // ST_CMD_OBJ_VAL_2S8,
  2 * sizeof(s16),                           // ST_CMD_OBJ_VAL_2S16,
  2 * sizeof(s32),                           // ST_CMD_OBJ_VAL_2S32,
  2 * sizeof(s64),                           // ST_CMD_OBJ_VAL_2S64,
  2 * sizeof(f32),                           // ST_CMD_OBJ_VAL_2F32,
  2 * sizeof(f64),                           // ST_CMD_OBJ_VAL_2F64,
  3 * sizeof(u8),                            // ST_CMD_OBJ_VAL_3U8,
  3 * sizeof(u16),                           // ST_CMD_OBJ_VAL_3U16,
  3 * sizeof(u32),                           // ST_CMD_OBJ_VAL_3U32,
  3 * sizeof(u64),                           // ST_CMD_OBJ_VAL_3U64,
  3 * sizeof(s8),                            // ST_CMD_OBJ_VAL_3S8,
  3 * sizeof(s16),                           // ST_CMD_OBJ_VAL_3S16,
  3 * sizeof(s32),                           // ST_CMD_OBJ_VAL_3S32,
  3 * sizeof(s64),                           // ST_CMD_OBJ_VAL_3S64,
  3 * sizeof(f32),                           // ST_CMD_OBJ_VAL_3F32,
  3 * sizeof(f64),                           // ST_CMD_OBJ_VAL_3F64,
  4 * sizeof(u8),                            // ST_CMD_OBJ_VAL_4U8,
  4 * sizeof(u16),                           // ST_CMD_OBJ_VAL_4U16,
  4 * sizeof(u32),                           // ST_CMD_OBJ_VAL_4U32,
  4 * sizeof(u64),                           // ST_CMD_OBJ_VAL_4U64,
  4 * sizeof(s8),                            // ST_CMD_OBJ_VAL_4S8,
  4 * sizeof(s16),                           // ST_CMD_OBJ_VAL_4S16,
  4 * sizeof(s32),                           // ST_CMD_OBJ_VAL_4S32,
  4 * sizeof(s64),                           // ST_CMD_OBJ_VAL_4S64,
  4 * sizeof(f32),                           // ST_CMD_OBJ_VAL_4F32,
  4 * sizeof(f64),                           // ST_CMD_OBJ_VAL_4F64,

  // TODO:

  255,                                       // ST_CMD_OBJ_VAL_InlineArrayU8,
  255,                                       // ST_CMD_OBJ_VAL_InlineArrayU16,
  255,                                       // ST_CMD_OBJ_VAL_InlineArrayU32,
  255,                                       // ST_CMD_OBJ_VAL_InlineArrayU64,
  255,                                       // ST_CMD_OBJ_VAL_InlineArrayS8,
  255,                                       // ST_CMD_OBJ_VAL_InlineArrayS16,
  255,                                       // ST_CMD_OBJ_VAL_InlineArrayS32,
  255,                                       // ST_CMD_OBJ_VAL_InlineArrayS64,
  255,                                       // ST_CMD_OBJ_VAL_InlineArrayF32,
  255,                                       // ST_CMD_OBJ_VAL_InlineArrayF64,

  255,                                       // ST_CMD_OBJ_VAL_InlineString,
  2 * sizeof(u32),                           // ST_CMD_OBJ_VAL_DataString,

  // u32 ObjectData Id
  // u32 Array Size
  // u8  Nb Elements
  // u32[0] = Object Data Id
  // u32[1] = Array Size
#define POOL_ARRAY_SIZE (sizeof(u32) + sizeof(u32) + sizeof(u8))
  POOL_ARRAY_SIZE,                           // ST_CMD_OBJ_VAL_PooledArrayU8,
  POOL_ARRAY_SIZE,                           // ST_CMD_OBJ_VAL_PooledArrayU16,
  POOL_ARRAY_SIZE,                           // ST_CMD_OBJ_VAL_PooledArrayU32,
  POOL_ARRAY_SIZE,                           // ST_CMD_OBJ_VAL_PooledArrayU64,
  POOL_ARRAY_SIZE,                           // ST_CMD_OBJ_VAL_PooledArrayS8,
  POOL_ARRAY_SIZE,                           // ST_CMD_OBJ_VAL_PooledArrayS16,
  POOL_ARRAY_SIZE,                           // ST_CMD_OBJ_VAL_PooledArrayS32,
  POOL_ARRAY_SIZE,                           // ST_CMD_OBJ_VAL_PooledArrayS64,
  POOL_ARRAY_SIZE,                           // ST_CMD_OBJ_VAL_PooledArrayF32,
  POOL_ARRAY_SIZE,                           // ST_CMD_OBJ_VAL_PooledArrayF64,
};

const struct stCommandValueType stCommandValueTypes[ST_CMD_COUNT] = {
  { 0, 0 },                                     // ST_CMD_NONE,
  { 1,  ST_TYPE_OBJECT_TYPE },                  // ST_CMD_OBJ_NEW,
  { 0, 0 },                                     // ST_CMD_OBJ_DELETE,
  { 1 , ST_TYPE_UINT8 },                        // ST_CMD_OBJ_VAL_1U8,
  { 1 , ST_TYPE_UINT16 },                       // ST_CMD_OBJ_VAL_1U16,
  { 1 , ST_TYPE_UINT32 },                       // ST_CMD_OBJ_VAL_1U32,
  { 1 , ST_TYPE_UINT64 },                       // ST_CMD_OBJ_VAL_1U64,
  { 1 , ST_TYPE_INT8 },                         // ST_CMD_OBJ_VAL_1S8,
  { 1 , ST_TYPE_INT16 },                        // ST_CMD_OBJ_VAL_1S16,
  { 1 , ST_TYPE_INT32 },                        // ST_CMD_OBJ_VAL_1S32,
  { 1 , ST_TYPE_INT64 },                        // ST_CMD_OBJ_VAL_1S64,
  { 1 , ST_TYPE_FLOAT32 },                      // ST_CMD_OBJ_VAL_1F32,
  { 1 , ST_TYPE_FLOAT64 },                      // ST_CMD_OBJ_VAL_1F64,
  { 2 , ST_TYPE_UINT8 },                        // ST_CMD_OBJ_VAL_2U8,
  { 2 , ST_TYPE_UINT16 },                       // ST_CMD_OBJ_VAL_2U16,
  { 2 , ST_TYPE_UINT32 },                       // ST_CMD_OBJ_VAL_2U32,
  { 2 , ST_TYPE_UINT64 },                       // ST_CMD_OBJ_VAL_2U64,
  { 2 , ST_TYPE_INT8 },                         // ST_CMD_OBJ_VAL_2S8,
  { 2 , ST_TYPE_INT16 },                        // ST_CMD_OBJ_VAL_2S16,
  { 2 , ST_TYPE_INT32 },                        // ST_CMD_OBJ_VAL_2S32,
  { 2 , ST_TYPE_INT64 },                        // ST_CMD_OBJ_VAL_2S64,
  { 2 , ST_TYPE_FLOAT32 },                      // ST_CMD_OBJ_VAL_2F32,
  { 2 , ST_TYPE_FLOAT64 },                      // ST_CMD_OBJ_VAL_2F64,
  { 3 , ST_TYPE_UINT8 },                        // ST_CMD_OBJ_VAL_3U8,
  { 3 , ST_TYPE_UINT16 },                       // ST_CMD_OBJ_VAL_3U16,
  { 3 , ST_TYPE_UINT32 },                       // ST_CMD_OBJ_VAL_3U32,
  { 3 , ST_TYPE_UINT64 },                       // ST_CMD_OBJ_VAL_3U64,
  { 3 , ST_TYPE_INT8 },                         // ST_CMD_OBJ_VAL_3S8,
  { 3 , ST_TYPE_INT16 },                        // ST_CMD_OBJ_VAL_3S16,
  { 3 , ST_TYPE_INT32 },                        // ST_CMD_OBJ_VAL_3S32,
  { 3 , ST_TYPE_INT64 },                        // ST_CMD_OBJ_VAL_3S64,
  { 3 , ST_TYPE_FLOAT32 },                      // ST_CMD_OBJ_VAL_3F32,
  { 3 , ST_TYPE_FLOAT64 },                      // ST_CMD_OBJ_VAL_3F64,
  { 4 , ST_TYPE_UINT8 },                        // ST_CMD_OBJ_VAL_4U8,
  { 4 , ST_TYPE_UINT16 },                       // ST_CMD_OBJ_VAL_4U16,
  { 4 , ST_TYPE_UINT32 },                       // ST_CMD_OBJ_VAL_4U32,
  { 4 , ST_TYPE_UINT64 },                       // ST_CMD_OBJ_VAL_4U64,
  { 4 , ST_TYPE_INT8 },                         // ST_CMD_OBJ_VAL_4S8,
  { 4 , ST_TYPE_INT16 },                        // ST_CMD_OBJ_VAL_4S16,
  { 4 , ST_TYPE_INT32 },                        // ST_CMD_OBJ_VAL_4S32,
  { 4 , ST_TYPE_INT64 },                        // ST_CMD_OBJ_VAL_4S64,
  { 4 , ST_TYPE_FLOAT32 },                      // ST_CMD_OBJ_VAL_4F32,
  { 4 , ST_TYPE_FLOAT64 },                      // ST_CMD_OBJ_VAL_4F64,

  { 1, ST_TYPE_UINT8 },                         // ST_CMD_OBJ_VAL_InlineArrayU8,
  { 1, ST_TYPE_UINT16 },                        // ST_CMD_OBJ_VAL_InlineArrayU16,
  { 1, ST_TYPE_UINT32 },                        // ST_CMD_OBJ_VAL_InlineArrayU32,
  { 1, ST_TYPE_UINT64 },                        // ST_CMD_OBJ_VAL_InlineArrayU64,
  { 1, ST_TYPE_INT8 },                          // ST_CMD_OBJ_VAL_InlineArrayS8,
  { 1, ST_TYPE_INT16 },                         // ST_CMD_OBJ_VAL_InlineArrayS16,
  { 1, ST_TYPE_INT32 },                         // ST_CMD_OBJ_VAL_InlineArrayS32,
  { 1, ST_TYPE_INT64 },                         // ST_CMD_OBJ_VAL_InlineArrayS64,
  { 1, ST_TYPE_FLOAT32 },                       // ST_CMD_OBJ_VAL_InlineArrayF32,
  { 1, ST_TYPE_FLOAT64 },                       // ST_CMD_OBJ_VAL_InlineArrayF64,
  { 1, ST_TYPE_UINT8 },                         // ST_CMD_OBJ_VAL_PooledArrayU8,
  { 1, ST_TYPE_UINT16 },                        // ST_CMD_OBJ_VAL_PooledArrayU16,
  { 1, ST_TYPE_UINT32 },                        // ST_CMD_OBJ_VAL_PooledArrayU32,
  { 1, ST_TYPE_UINT64 },                        // ST_CMD_OBJ_VAL_PooledArrayU64,
  { 1, ST_TYPE_INT8 },                          // ST_CMD_OBJ_VAL_PooledArrayS8,
  { 1, ST_TYPE_INT16 },                         // ST_CMD_OBJ_VAL_PooledArrayS16,
  { 1, ST_TYPE_INT32 },                         // ST_CMD_OBJ_VAL_PooledArrayS32,
  { 1, ST_TYPE_INT64 },                         // ST_CMD_OBJ_VAL_PooledArrayS64,
  { 1, ST_TYPE_FLOAT32 },                       // ST_CMD_OBJ_VAL_PooledArrayF32,
  { 1, ST_TYPE_FLOAT64 },                       // ST_CMD_OBJ_VAL_PooledArrayF64,

  { 1, ST_TYPE_CSTRING },                       // ST_CMD_OBJ_VAL_InlineString,
  { 1, ST_TYPE_CSTRING },                       // ST_CMD_OBJ_VAL_DataString,

};


const char* stCommandType_Str[ST_CMD_COUNT] = {
  "NONE",
  "OBJ_NEW",
  "OBJ_DELETE",
  "OBJ_VAL_1U8",
  "OBJ_VAL_1U16",
  "OBJ_VAL_1U32",
  "OBJ_VAL_1U64",
  "OBJ_VAL_1S8",
  "OBJ_VAL_1S16",
  "OBJ_VAL_1S32",
  "OBJ_VAL_1S64",
  "OBJ_VAL_1F32",
  "OBJ_VAL_1F64",
  "OBJ_VAL_2U8",
  "OBJ_VAL_2U16",
  "OBJ_VAL_2U32",
  "OBJ_VAL_2U64",
  "OBJ_VAL_2S8",
  "OBJ_VAL_2S16",
  "OBJ_VAL_2S32",
  "OBJ_VAL_2S64",
  "OBJ_VAL_2F32",
  "OBJ_VAL_2F64",
  "OBJ_VAL_3U8",
  "OBJ_VAL_3U16",
  "OBJ_VAL_3U32",
  "OBJ_VAL_3U64",
  "OBJ_VAL_3S8",
  "OBJ_VAL_3S16",
  "OBJ_VAL_3S32",
  "OBJ_VAL_3S64",
  "OBJ_VAL_3F32",
  "OBJ_VAL_3F64",
  "OBJ_VAL_4U8",
  "OBJ_VAL_4U16",
  "OBJ_VAL_4U32",
  "OBJ_VAL_4U64",
  "OBJ_VAL_4S8",
  "OBJ_VAL_4S16",
  "OBJ_VAL_4S32",
  "OBJ_VAL_4S64",
  "OBJ_VAL_4F32",
  "OBJ_VAL_4F64",
  "OBJ_VAL_InlineArrayU8",
  "OBJ_VAL_InlineArrayU16",
  "OBJ_VAL_InlineArrayU32",
  "OBJ_VAL_InlineArrayU64",
  "OBJ_VAL_InlineArrayS8",
  "OBJ_VAL_InlineArrayS16",
  "OBJ_VAL_InlineArrayS32",
  "OBJ_VAL_InlineArrayS64",
  "OBJ_VAL_InlineArrayF32",
  "OBJ_VAL_InlineArrayF64",
  "OBJ_VAL_DataArrayU8",
  "OBJ_VAL_DataArrayU16",
  "OBJ_VAL_DataArrayU32",
  "OBJ_VAL_DataArrayU64",
  "OBJ_VAL_DataArrayS8",
  "OBJ_VAL_DataArrayS16",
  "OBJ_VAL_DataArrayS32",
  "OBJ_VAL_DataArrayS64",
  "OBJ_VAL_DataArrayF32",
  "OBJ_VAL_DataArrayF64",
  "OBJ_VAL_InlineString",
  "OBJ_VAL_DataString"
};

const char* stCommandTypeToString(stCommandType type)
{
  if (type >= ST_CMD_COUNT)
    return "UNKNOWN";

  return stCommandType_Str[type];
}
