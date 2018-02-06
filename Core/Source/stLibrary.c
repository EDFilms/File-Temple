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

#include "stLibrary.h"
#include "stThreads.h"

#include <time.h>

static stSpinLock  sLibraryLock         = ST_SPINLOCK_DEFAULT;
static s32         sInitialisationCount = 0;
static s32         sSessionId           = 0;

FILE* sOut;

u32 stMaybeInitialiseLibrary()
{
  ST_LOCK(&sLibraryLock);
  
  if (sInitialisationCount == 0)
  {
    sOut = fopen("scenetrack.log", "w+");

    if (sOut == NULL)
    { 
      sOut = stderr;
    }
    
    stInitialiseHeapMemory();
    sSessionId = (u32) (time(NULL) ^ 0xCAFEBEEF);
  }

  sInitialisationCount++;

  ST_UNLOCK(&sLibraryLock);

  return sSessionId;
}

void stMaybeShutdownLibrary()
{
  ST_LOCK(&sLibraryLock);

  sInitialisationCount--;

  ST_ASSERT(sInitialisationCount >= 0, "Library has been shutdown to many times!");

  if (sInitialisationCount == 0)
  {
    stShutdownHeapMemory();
    sSessionId = 0;

    if (sOut != NULL && sOut != stderr)
    {
      fclose(sOut);
    }

    sOut = NULL;
  }

  ST_UNLOCK(&sLibraryLock);
}
