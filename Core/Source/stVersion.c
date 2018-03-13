// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stVersion.cpp
 *
 * Scene Track Version information functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stVersion.h"
#include "stPlatform.h"

ST_PUBLIC_FUNCTION(Named="stGetVersionStrCapacity", Text="Get Library Version String Capacity", Category="Library", ThreadSafe) 
ST_PUBLIC s32 stGetVersionStrCapacity();

s32 stGetVersionStrCapacity()
{
  return (int) (strlen(SCENETRACK_VERSION_STR) + 1);
}


ST_PUBLIC_FUNCTION(Named="stGetVersion", Text="Get Library Version", Category="Library", ThreadSafe)
ST_PUBLIC s32 stGetVersion(stStr outStr, s32 strCapacity);

s32 stGetVersion(stStr outStr, s32 strCapacity)
{
  int length = (int) strlen(SCENETRACK_VERSION_STR);
  
  if (strCapacity < (length + 1))
    return -1;

  memcpy(outStr, SCENETRACK_VERSION_STR, length + 1);
  return 0;
}
