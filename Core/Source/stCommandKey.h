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

#ifndef ST_COMMAND_KEY_H
#define ST_COMMAND_KEY_H

#include "stPlatform.h"
#include "stCommandType.h"
#include "stMath.h"

#define ST_KEY_SHIFT_MASK(BITS, SHIFT)      ((((u64) (BITS) - 1)) << SHIFT)

#define ST_KEY_BITS_CUSTOM_SIZE        0x08
#define ST_KEY_SHIFT_CUSTOM_SIZE       0x00
#define ST_KEY_SIZE_CUSTOM_SIZE        (1 << ST_KEY_BITS_CUSTOM_SIZE)
#define ST_KEY_MASK_CUSTOM_SIZE        ST_KEY_SHIFT_MASK(ST_KEY_SIZE_CUSTOM_SIZE, ST_KEY_SHIFT_CUSTOM_SIZE)

#define ST_KEY_BITS_FLAGS       0x08
#define ST_KEY_SHIFT_FLAGS      0x08
#define ST_KEY_SIZE_FLAGS       (1 << ST_KEY_BITS_FLAGS)
#define ST_KEY_MASK_FLAGS       ST_KEY_SHIFT_MASK(ST_KEY_SIZE_FLAGS, ST_KEY_SHIFT_FLAGS)

#define ST_KEY_BITS_COMMAND     0x08
#define ST_KEY_SHIFT_COMMAND    0x10
#define ST_KEY_SIZE_COMMAND     (1 << ST_KEY_BITS_COMMAND)
#define ST_KEY_MASK_COMMAND     ST_KEY_SHIFT_MASK(ST_KEY_SIZE_COMMAND, ST_KEY_SHIFT_COMMAND)

#define ST_KEY_BITS_COMPONENT   0x08
#define ST_KEY_SHIFT_COMPONENT  0x18
#define ST_KEY_SIZE_COMPONENT   (1 << ST_KEY_BITS_COMPONENT)
#define ST_KEY_MASK_COMPONENT   ST_KEY_SHIFT_MASK(ST_KEY_SIZE_COMPONENT, ST_KEY_SHIFT_COMPONENT)

#define ST_KEY_BITS_OBJECT      0x18
#define ST_KEY_SHIFT_OBJECT     0x20
#define ST_KEY_SIZE_OBJECT      (1 << ST_KEY_BITS_OBJECT)
#define ST_KEY_MASK_OBJECT      ST_KEY_SHIFT_MASK(ST_KEY_SIZE_OBJECT, ST_KEY_SHIFT_OBJECT)

#define ST_KEY_BITS_FRAME       0x08
#define ST_KEY_SHIFT_FRAME      0x38
#define ST_KEY_SIZE_FRAME       (1 << ST_KEY_BITS_FRAME)
#define ST_KEY_MASK_FRAME       ST_KEY_SHIFT_MASK(ST_KEY_SIZE_FRAME, ST_KEY_SHIFT_FRAME)

struct stCommandKeyInfoT
{
  u64 object;
  u64 frameModulus;
  u64 commandType;
  u64 component;
  u64 flags;
  u64 customSize;
};

typedef struct stCommandKeyInfoT stCommandKeyInfo;

/*
--|  < MSB                                                        LSB >  |                                 
--|          .      3|        .      2|        .      1|        .      0 |                                 
--|  fedcba9876543210|fedcba9876543210|fedcba0976543210|fedcba9876543210 | Name         | Start | Length |
--+------------------+----------------+----------------+-----------------+--------------+-------+--------+
--|  FFFFFFFFoooooooo-oooooooooooooooo-cccccccctttttttt-ffffffffssssssss- Custom Size   | 0x00  | 0x08   |
--|         |                        |        |       |        +--------- Flags         | 0x08  | 0x08   |
--|         |                        |        |       +------------------ CommandType   |       | 0x08   |
--|         |                        |        +-------------------------- Component     |       | 0x08   |
--|         |                        +----------------------------------- Object        |       | 0x18   |
--|         +------------------------------------------------------------ Frame Modulus |       | 0x08   |
*/

// Make Command Info
// Returns: True data size
ST_INLINE u32 stMakeCommandInfo(stCommandKeyInfo* info, u32 frameNumber, stEnum32 commandType, u32 objectId, u8 componentId, u8 flags, u16 byteSize, u8 customSize)
{
  ST_NULL_CHECK(info);

  info->frameModulus = frameNumber & 255; // Faster than frameNumber % 256
  info->commandType  = commandType;
  info->object       = objectId;
  info->component    = componentId;
  info->flags        = flags;
  info->customSize   = customSize;

  return (u32) byteSize;
}

ST_INLINE u64 stCommandInfoToCommandKey(stCommandKeyInfo* info)
{
  const u64 customSize = ((info->customSize)   << ST_KEY_SHIFT_CUSTOM_SIZE)      & ST_KEY_MASK_CUSTOM_SIZE;
  const u64 flags      = ((info->flags)        << ST_KEY_SHIFT_FLAGS)            & ST_KEY_MASK_FLAGS;
  const u64 command    = ((info->commandType)  << ST_KEY_SHIFT_COMMAND)          & ST_KEY_MASK_COMMAND;
  const u64 component  = ((info->component)    << ST_KEY_SHIFT_COMPONENT)        & ST_KEY_MASK_COMPONENT;
  const u64 object     = ((info->object)       << ST_KEY_SHIFT_OBJECT)           & ST_KEY_MASK_OBJECT;
  const u64 frame      = ((info->frameModulus) << ST_KEY_SHIFT_FRAME)            & ST_KEY_MASK_FRAME;

  const u64 key = customSize | flags | command | component | object | frame;

  return key;
}


ST_INLINE u32 stGetCommandObject(u64 key)
{
  u64 m = ((key & ST_KEY_MASK_OBJECT) >> ST_KEY_SHIFT_OBJECT);
  return (u32) m;
}

ST_INLINE stBool stGetCommandIsEvent(u64 key)
{
  return stGetCommandObject(key) >= 0x7FFFFF;
}

ST_INLINE u32 stGetCommandComponent(u64 key)
{
  u64 m = ((key & ST_KEY_MASK_COMPONENT) >> ST_KEY_SHIFT_COMPONENT);
  return (u32) m;
}

ST_INLINE stCommandType stGetCommandType(u64 key)
{
  u64 m = ((key & ST_KEY_MASK_COMMAND) >> ST_KEY_SHIFT_COMMAND);
  return (stCommandType) m;
}

ST_INLINE u32 stGetCommandSize(u64 key)
{
  u32 m = stGetCommandType(key);
  return stCommandCountDiskSize[m];
}

ST_INLINE u32 stGetCommandCustomSize(u64 key)
{
  u64 m = ((key & ST_KEY_MASK_CUSTOM_SIZE) >> ST_KEY_SHIFT_CUSTOM_SIZE);
  return (u32) m;
}

ST_INLINE void stGetCommandDataTypeAndNbElements(u64 key, u32* type, u32* nbElements)
{
  stCommandType m = stGetCommandType(key);
  struct stCommandValueType k = stCommandValueTypes[m];
  (*type) = k.type;
  (*nbElements) = k.nbElements;
}

#define ST_ARRAY_TYPE_NONE        0
#define ST_ARRAY_TYPE_INLINE      1
#define ST_ARRAY_TYPE_OBJECT_DATA 2

// 0 - Not an array
// 1 - Inline
// 2 - Object Data
ST_INLINE u32 stGetCommandArrayType(u64 key)
{
  stCommandType type = stGetCommandType(key);

  if ((type >= ST_CMD_OBJ_VAL_InlineArrayU8 && type <= ST_CMD_OBJ_VAL_InlineArrayF64) || type == ST_CMD_OBJ_VAL_InlineString)
    return ST_ARRAY_TYPE_INLINE;
  else if ((type >= ST_CMD_OBJ_VAL_DataArrayU8 && type <= ST_CMD_OBJ_VAL_DataArrayF64) || type == ST_CMD_OBJ_VAL_DataString)
    return ST_ARRAY_TYPE_OBJECT_DATA;
  return ST_ARRAY_TYPE_NONE;
}

typedef struct stCommandKeyUT
{
  stCommandKey     key;
  u64              numeric;
} stCommandKeyU;

ST_STATIC_ASSERT(sizeof(stCommandKey) == sizeof(u64), Ensure64BitKey);


#endif
