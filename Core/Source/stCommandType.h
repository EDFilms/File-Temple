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

#ifndef ST_COMMANDS_H
#define ST_COMMANDS_H

#include "stPlatform.h"

#define ST_COMMAND_MAX_PER_FRAME              ((1 << 16))
#define ST_COMMAND_DATA_SIZE                  KILOBYTES(512)
#define ST_COMMAND_POOL_SIZE                  4
#define ST_COMMAND_MAX_SIZE                   (256 * 4)
#define ST_COMMAND_BUFFER_INDEX_SUBMISSION    0
#define ST_COMMAND_BUFFER_INDEX_PROCESSING    1
#define ST_COMMAND_BUFFER_COUNT               2

typedef u16 stCommandBufferIndex;

typedef u64 stCommandKey;

typedef enum
{
  ST_CMD_NONE,
  ST_CMD_OBJ_NEW,
  ST_CMD_OBJ_DELETE,
  ST_CMD_OBJ_VAL_1U8,
  ST_CMD_OBJ_VAL_1U16,
  ST_CMD_OBJ_VAL_1U32,
  ST_CMD_OBJ_VAL_1U64,
  ST_CMD_OBJ_VAL_1S8,
  ST_CMD_OBJ_VAL_1S16,
  ST_CMD_OBJ_VAL_1S32,
  ST_CMD_OBJ_VAL_1S64,
  ST_CMD_OBJ_VAL_1F32,
  ST_CMD_OBJ_VAL_1F64,
  ST_CMD_OBJ_VAL_2U8,
  ST_CMD_OBJ_VAL_2U16,
  ST_CMD_OBJ_VAL_2U32,
  ST_CMD_OBJ_VAL_2U64,
  ST_CMD_OBJ_VAL_2S8,
  ST_CMD_OBJ_VAL_2S16,
  ST_CMD_OBJ_VAL_2S32,
  ST_CMD_OBJ_VAL_2S64,
  ST_CMD_OBJ_VAL_2F32,
  ST_CMD_OBJ_VAL_2F64,
  ST_CMD_OBJ_VAL_3U8,
  ST_CMD_OBJ_VAL_3U16,
  ST_CMD_OBJ_VAL_3U32,
  ST_CMD_OBJ_VAL_3U64,
  ST_CMD_OBJ_VAL_3S8,
  ST_CMD_OBJ_VAL_3S16,
  ST_CMD_OBJ_VAL_3S32,
  ST_CMD_OBJ_VAL_3S64,
  ST_CMD_OBJ_VAL_3F32,
  ST_CMD_OBJ_VAL_3F64,
  ST_CMD_OBJ_VAL_4U8,
  ST_CMD_OBJ_VAL_4U16,
  ST_CMD_OBJ_VAL_4U32,
  ST_CMD_OBJ_VAL_4U64,
  ST_CMD_OBJ_VAL_4S8,
  ST_CMD_OBJ_VAL_4S16,
  ST_CMD_OBJ_VAL_4S32,
  ST_CMD_OBJ_VAL_4S64,
  ST_CMD_OBJ_VAL_4F32,
  ST_CMD_OBJ_VAL_4F64,
  ST_CMD_OBJ_VAL_InlineArrayU8,
  ST_CMD_OBJ_VAL_InlineArrayU16,
  ST_CMD_OBJ_VAL_InlineArrayU32,
  ST_CMD_OBJ_VAL_InlineArrayU64,
  ST_CMD_OBJ_VAL_InlineArrayS8,
  ST_CMD_OBJ_VAL_InlineArrayS16,
  ST_CMD_OBJ_VAL_InlineArrayS32,
  ST_CMD_OBJ_VAL_InlineArrayS64,
  ST_CMD_OBJ_VAL_InlineArrayF32,
  ST_CMD_OBJ_VAL_InlineArrayF64,
  ST_CMD_OBJ_VAL_DataArrayU8,
  ST_CMD_OBJ_VAL_DataArrayU16,
  ST_CMD_OBJ_VAL_DataArrayU32,
  ST_CMD_OBJ_VAL_DataArrayU64,
  ST_CMD_OBJ_VAL_DataArrayS8,
  ST_CMD_OBJ_VAL_DataArrayS16,
  ST_CMD_OBJ_VAL_DataArrayS32,
  ST_CMD_OBJ_VAL_DataArrayS64,
  ST_CMD_OBJ_VAL_DataArrayF32,
  ST_CMD_OBJ_VAL_DataArrayF64,
  ST_CMD_OBJ_VAL_InlineString,
  ST_CMD_OBJ_VAL_DataString,
  ST_CMD_COUNT
} stCommandType;

ST_STATIC_ASSERT(ST_CMD_COUNT < 256, EnsureNotToManyCommands);

extern const u8 stCommandCountDiskSize[ST_CMD_COUNT];

const char* stCommandTypeToString(stCommandType type);

struct stCommandValueType
{
  u8 nbElements;
  u8 type;
};


extern const struct stCommandValueType stCommandValueTypes[ST_CMD_COUNT];

#endif
