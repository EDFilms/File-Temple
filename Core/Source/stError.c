// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stError.c
 *
 * Error Handler
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stError.h"

static u32 errorCode = 0;

u32 stGetError()
{
  for(u32 i=0;i < 32;i++)
  {
    u32 bit = 1 << i;
    if ((errorCode & bit) != 0)
    {
      errorCode &= ~bit;
      return (bit);
    }
  }

  return 0;
}

u32 stPeekError()
{
  return errorCode;
}

void stRaiseError(u32 error)
{
  errorCode |= error;
}

stBool stHasError(u32 error)
{
  return (errorCode & error) != 0;
}

void stClearErrors()
{
  errorCode = 0;
}

u32 stPopError()
{
  u32 s = errorCode;
  errorCode = 0;
  return s;
}

void stPushError(u32 errorCodes)
{
  errorCode = errorCodes;
}
