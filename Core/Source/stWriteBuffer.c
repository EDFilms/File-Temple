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

#include "stWriteBuffer.h"
#include "stError.h"


void stWriteBuffer_Initialise(stWriteBuffer* buffer)
{
  if (buffer->size <= 64)
    buffer->size = 64;

  buffer->buffer.size = buffer->size;
  stFixedLinearAllocator_Initialise(&buffer->buffer);
}

void stWriteBuffer_Shutdown(stWriteBuffer* buffer)
{
  stWriteBuffer_Flush(buffer);
  stFixedLinearAllocator_Shutdown(&buffer->buffer);
}

void stWriteBuffer_Write(stWriteBuffer* buffer, const void* data, u32 size)
{
  // Doesn't fit at all. Write in buffer then write immeditally,
  if (size >= buffer->size)
  {
    stWriteBuffer_Flush(buffer);
    size_t r;
    if ((r = fwrite(data, size, 1, buffer->file)) != 1)
    {
      stRaiseError(ST_ERROR_DISK);
      printf("Disk Error: %llu\n", (u64) r);
    }

    return;
  }
  
  u8* mem = stFixedLinearAllocator_MaybeObtain(&buffer->buffer, size);

  if (mem == NULL)
  {
    // Doesn't partially fit, flush and try again.
    stWriteBuffer_Flush(buffer);
    mem = stFixedLinearAllocator_MaybeObtain(&buffer->buffer, size);
    ST_ASSERT(mem != NULL, "Shouldn't happen.");
  }

  memcpy(mem, data, size);
}

void stWriteBuffer_Flush(stWriteBuffer* buffer)
{
  size_t size = stFixedLinearAllocator_Used(&buffer->buffer);
  if (size)
  {
    size_t r;
    if ((r = fwrite(buffer->buffer.mem, size, 1, buffer->file)) != 1)
    {
      stRaiseError(ST_ERROR_DISK);
      printf("Disk Error: %llu\n", (u64) r);
    }
    stFixedLinearAllocator_Clear(&buffer->buffer);
  }
}
