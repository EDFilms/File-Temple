// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stCommandType.h
 *
 * Type and Command Format for recordable commands
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_COMMANDS_H
#define ST_COMMANDS_H

#include "stPlatform.h"

/**
 * Constant for the number of commands per frame
 */
#define ST_COMMAND_MAX_PER_FRAME              ((1 << 16))

/**
 * Constant for the maximum data size per frame (UNUSED)
 */
#define ST_COMMAND_DATA_SIZE                  KILOBYTES(512)

/**
 * Constant for the pool size per frame (UNUSED)
 */
#define ST_COMMAND_POOL_SIZE                  4

/**
 * Constant for the command maximum size (UNUSED)
 */
#define ST_COMMAND_MAX_SIZE                   (256 * 4)

/**
 * Submission Command buffer Id
 */
#define ST_COMMAND_BUFFER_INDEX_SUBMISSION    0

/**
 * Processing Command buffer Id
 */
#define ST_COMMAND_BUFFER_INDEX_PROCESSING    1

/**
 * Number of command buffers
 */
#define ST_COMMAND_BUFFER_COUNT               2

typedef u16 stCommandBufferIndex;

typedef u64 stCommandKey;

/**
 * Type of commands (and format) of recordable commands per object per component per frame.
 */
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

/**
 * A lookup table for each stCommandType size on disk
 */
extern const u8 stCommandCountDiskSize[ST_CMD_COUNT];

/**
 * Get a string of the stCommandType enum
 */
const char* stCommandTypeToString(stCommandType type);

/**
 * The data format of a stCommandType
 */
struct stCommandValueType
{
  u8 nbElements;
  u8 type;
};

/**
 * A lookup table of the data formats for each stCommandType
 */
extern const struct stCommandValueType stCommandValueTypes[ST_CMD_COUNT];

#endif
