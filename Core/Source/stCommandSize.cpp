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

#define ATTACHMENT_ID    sizeof(u32)
#define INLINE_ARRAY     sizeof(u8)

const u32 stCommandCountDiskSize[ST_CMD_COUNT] = {
  sizeof(u32),                    // ST_CMD_OBJ_NEW,
  0,                              // ST_CMD_OBJ_DELETE,
  1 * sizeof(U8),                 // ST_CMD_OBJ_VAL_1U8,
  1 * sizeof(U16),                // ST_CMD_OBJ_VAL_1U16,
  1 * sizeof(U32),                // ST_CMD_OBJ_VAL_1U32,
  1 * sizeof(U64),                // ST_CMD_OBJ_VAL_1U64,
  1 * sizeof(S8),                 // ST_CMD_OBJ_VAL_1S8,
  1 * sizeof(S16),                // ST_CMD_OBJ_VAL_1S16,
  1 * sizeof(S32),                // ST_CMD_OBJ_VAL_1S32,
  1 * sizeof(S64),                // ST_CMD_OBJ_VAL_1S64,
  1 * sizeof(F32),                // ST_CMD_OBJ_VAL_1F32,
  1 * sizeof(F64),                // ST_CMD_OBJ_VAL_1F64,
  2 * sizeof(U8),                 // ST_CMD_OBJ_VAL_2U8,
  2 * sizeof(U16),                // ST_CMD_OBJ_VAL_2U16,
  2 * sizeof(U32),                // ST_CMD_OBJ_VAL_2U32,
  2 * sizeof(U64),                // ST_CMD_OBJ_VAL_2U64,
  2 * sizeof(S8),                 // ST_CMD_OBJ_VAL_2S8,
  2 * sizeof(S16),                // ST_CMD_OBJ_VAL_2S16,
  2 * sizeof(S32),                // ST_CMD_OBJ_VAL_2S32,
  2 * sizeof(S64),                // ST_CMD_OBJ_VAL_2S64,
  2 * sizeof(F32),                // ST_CMD_OBJ_VAL_2F32,
  2 * sizeof(F64),                // ST_CMD_OBJ_VAL_2F64,
  3 * sizeof(U8),                 // ST_CMD_OBJ_VAL_3U8,
  3 * sizeof(U16),                // ST_CMD_OBJ_VAL_3U16,
  3 * sizeof(U32),                // ST_CMD_OBJ_VAL_3U32,
  3 * sizeof(U64),                // ST_CMD_OBJ_VAL_3U64,
  3 * sizeof(S8),                 // ST_CMD_OBJ_VAL_3S8,
  3 * sizeof(S16),                // ST_CMD_OBJ_VAL_3S16,
  3 * sizeof(S32),                // ST_CMD_OBJ_VAL_3S32,
  3 * sizeof(S64),                // ST_CMD_OBJ_VAL_3S64,
  3 * sizeof(F32),                // ST_CMD_OBJ_VAL_3F32,
  3 * sizeof(F64),                // ST_CMD_OBJ_VAL_3F64,
  4 * sizeof(U8),                 // ST_CMD_OBJ_VAL_4U8,
  4 * sizeof(U16),                // ST_CMD_OBJ_VAL_4U16,
  4 * sizeof(U32),                // ST_CMD_OBJ_VAL_4U32,
  4 * sizeof(U64),                // ST_CMD_OBJ_VAL_4U64,
  4 * sizeof(S8),                 // ST_CMD_OBJ_VAL_4S8,
  4 * sizeof(S16),                // ST_CMD_OBJ_VAL_4S16,
  4 * sizeof(S32),                // ST_CMD_OBJ_VAL_4S32,
  4 * sizeof(S64),                // ST_CMD_OBJ_VAL_4S64,
  4 * sizeof(F32),                // ST_CMD_OBJ_VAL_4F32,
  4 * sizeof(F64),                // ST_CMD_OBJ_VAL_4F64,
  INLINE_ARRAY,                   // ST_CMD_OBJ_VAL_InlineArrayU8,
  INLINE_ARRAY,                   // ST_CMD_OBJ_VAL_InlineArrayU16,
  INLINE_ARRAY,                   // ST_CMD_OBJ_VAL_InlineArrayU32,
  INLINE_ARRAY,                   // ST_CMD_OBJ_VAL_InlineArrayU64,
  INLINE_ARRAY,                   // ST_CMD_OBJ_VAL_InlineArrayS8,
  INLINE_ARRAY,                   // ST_CMD_OBJ_VAL_InlineArrayS16,
  INLINE_ARRAY,                   // ST_CMD_OBJ_VAL_InlineArrayS32,
  INLINE_ARRAY,                   // ST_CMD_OBJ_VAL_InlineArrayS64,
  INLINE_ARRAY,                   // ST_CMD_OBJ_VAL_InlineArrayF32,
  INLINE_ARRAY,                   // ST_CMD_OBJ_VAL_InlineArrayF64,
  ATTACHMENT_ID,                  // ST_CMD_OBJ_VAL_PooledArrayU8,
  ATTACHMENT_ID,                  // ST_CMD_OBJ_VAL_PooledArrayU16,
  ATTACHMENT_ID,                  // ST_CMD_OBJ_VAL_PooledArrayU32,
  ATTACHMENT_ID,                  // ST_CMD_OBJ_VAL_PooledArrayU64,
  ATTACHMENT_ID,                  // ST_CMD_OBJ_VAL_PooledArrayS8,
  ATTACHMENT_ID,                  // ST_CMD_OBJ_VAL_PooledArrayS16,
  ATTACHMENT_ID,                  // ST_CMD_OBJ_VAL_PooledArrayS32,
  ATTACHMENT_ID,                  // ST_CMD_OBJ_VAL_PooledArrayS64,
  ATTACHMENT_ID,                  // ST_CMD_OBJ_VAL_PooledArrayF32,
  ATTACHMENT_ID                   // ST_CMD_OBJ_VAL_PooledArrayF64
};
