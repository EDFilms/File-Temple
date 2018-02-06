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

#include "videoRequired.h"
#include "videoVersion.h"

extern "C"
{

STVIDEO_PUBLIC_FUNCTION(Named="videoGetVersionStrCapacity", Text="Get Library Version String Capacity", Category="Library", ThreadSafe) 
STVIDEO_PUBLIC s32 videoGetVersionStrCapacity();

s32 videoGetVersionStrCapacity()
{
  return (int) (strlen(SCENETRACKVIDEO_VERSION_STR) + 1);
}


STVIDEO_PUBLIC_FUNCTION(Named="videoGetVersion", Text="Get Library Version", Category="Library", ThreadSafe)
STVIDEO_PUBLIC s32 videoGetVersion(stStr outStr, s32 strCapacity);

s32 videoGetVersion(stStr outStr, s32 strCapacity)
{
  int length = (int) strlen(SCENETRACKVIDEO_VERSION_STR);
  
  if (strCapacity < (length + 1))
    return -1;

  memcpy(outStr, SCENETRACKVIDEO_VERSION_STR, length + 1);
  return 0;
}

}
