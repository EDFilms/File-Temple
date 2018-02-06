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

#include "midiRequired.h"
#include "midiVersion.h"

extern "C"
{

STMIDI_PUBLIC_FUNCTION(Named="midiGetVersionStrCapacity", Text="Get Library Version String Capacity", Category="Library", ThreadSafe) 
STMIDI_PUBLIC s32 midiGetVersionStrCapacity();

s32 midiGetVersionStrCapacity()
{
  return (int) (strlen(SCENETRACKMIDI_VERSION_STR) + 1);
}


STMIDI_PUBLIC_FUNCTION(Named="midiGetVersion", Text="Get Library Version", Category="Library", ThreadSafe)
STMIDI_PUBLIC s32 midiGetVersion(stStr outStr, s32 strCapacity);

s32 midiGetVersion(stStr outStr, s32 strCapacity)
{
  int length = (int) strlen(SCENETRACKMIDI_VERSION_STR);
  
  if (strCapacity < (length + 1))
    return -1;

  memcpy(outStr, SCENETRACKMIDI_VERSION_STR, length + 1);
  return 0;
}

}
