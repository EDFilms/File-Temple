// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stWriteBuffer.h
 *
 * Delayed file writing class and functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_WRITE_BUFFER_H
#define ST_WRITE_BUFFER_H

#include "stLinearAllocator.h"

/**
 * Write buffer class to temporarily hold data to be written to a file until a specified
 * period or when the write buffer is full.
 */
struct stWriteBufferT
{
  FILE*                  file;
  u32                    size;
  stFixedLinearAllocator buffer;
};

typedef struct stWriteBufferT stWriteBuffer;

/**
 * Initialise a write buffer memory
 * buffer->file variable should be preallocated
 */
void stWriteBuffer_Initialise(stWriteBuffer* buffer);

/**
 * Flush any data for writing and release the buffer's memory
 */
void stWriteBuffer_Shutdown(stWriteBuffer* buffer);

/**
 * Write data to the file. Data writing may be delayed until a flush
 * is called, or when the buffer is full.
 */
void stWriteBuffer_Write(stWriteBuffer* buffer, const void* data, u32 size);

/**
 * Write all buffered data to the file, and then clear the buffer
 */
void stWriteBuffer_Flush(stWriteBuffer* buffer);

/**
 * Inline function to quickly write a uint8 to the buffer
 */
ST_INLINE void stWriteBuffer_FastU8(stWriteBuffer* buffer, u8 value)
{
  stWriteBuffer_Write(buffer, &value, sizeof(u8));
}

/**
 * Inline function to quickly write a uint16 to the buffer
 */
ST_INLINE void stWriteBuffer_FastU16(stWriteBuffer* buffer, u16 value)
{
  stWriteBuffer_Write(buffer, &value, sizeof(u16));
}

/**
 * Inline function to quickly write a uint32 to the buffer
 */
ST_INLINE void stWriteBuffer_FastU32(stWriteBuffer* buffer, u32 value)
{
  stWriteBuffer_Write(buffer, &value, sizeof(u32));
}

#endif
