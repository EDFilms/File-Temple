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

#include "stCookie.h"
#include "stBinary.h"
#include <stdlib.h>

u32 stGenerate32BitCookie(u32 offset)
{
  stU32Bytes a;
  a.value = offset;
  for(u32 i=0;i < 4;i++)
  {
    a.bytes[i] = ((u32) a.bytes[i] + rand()) & 0xFF;
  }
  return a.value;
}

u64 stGenerate64BitCookie(u64 offset)
{
  stU64Bytes a;
  a.value = offset;
  for(u32 i=0;i < 8;i++)
  {
    a.bytes[i] = ((u32) a.bytes[i] + rand()) & 0xFF;
  }
  return a.value;
}
