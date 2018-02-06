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

#include "fbxRequired.h"
#include "fbxVersion.h"

extern "C"
{

STFBX_PUBLIC_FUNCTION(Named="fbxGetVersionStrCapacity", Text="Get Library Version String Capacity", Category="Library", ThreadSafe) 
STFBX_PUBLIC s32 fbxGetVersionStrCapacity();

s32 fbxGetVersionStrCapacity()
{
  return (int) (strlen(SCENETRACKFBX_VERSION_STR) + 1);
}


STFBX_PUBLIC_FUNCTION(Named="fbxGetVersion", Text="Get Library Version", Category="Library", ThreadSafe)
STFBX_PUBLIC s32 fbxGetVersion(stStr outStr, s32 strCapacity);

s32 fbxGetVersion(stStr outStr, s32 strCapacity)
{
  int length = (int) strlen(SCENETRACKFBX_VERSION_STR);
  
  if (strCapacity < (length + 1))
    return -1;

  memcpy(outStr, SCENETRACKFBX_VERSION_STR, length + 1);
  return 0;
}

}
