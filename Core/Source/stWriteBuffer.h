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

#ifndef ST_WRITE_BUFFER_H
#define ST_WRITE_BUFFER_H

#include "stLinearAllocator.h"

struct stWriteBufferT
{
  FILE*                  file;
  u32                    size;
  stFixedLinearAllocator buffer;
};

typedef struct stWriteBufferT stWriteBuffer;

void stWriteBuffer_Initialise(stWriteBuffer* buffer);

void stWriteBuffer_Shutdown(stWriteBuffer* buffer);

void stWriteBuffer_Write(stWriteBuffer* buffer, const void* data, u32 size);

void stWriteBuffer_Flush(stWriteBuffer* buffer);

ST_INLINE void stWriteBuffer_FastU8(stWriteBuffer* buffer, u8 value)
{
  stWriteBuffer_Write(buffer, &value, sizeof(u8));
}

ST_INLINE void stWriteBuffer_FastU16(stWriteBuffer* buffer, u16 value)
{
  stWriteBuffer_Write(buffer, &value, sizeof(u16));
}

ST_INLINE void stWriteBuffer_FastU32(stWriteBuffer* buffer, u32 value)
{
  stWriteBuffer_Write(buffer, &value, sizeof(u32));
}

#endif
