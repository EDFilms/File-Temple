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

#ifndef ST_LIBRARY_H
#define ST_LIBRARY_H

#include "stPlatform.h"

ST_PRIVATE_FUNCTION(Named="stMaybeInitialiseLibrary", Text="Maybe Initialise the Library", ThreadSafe);
u32 stMaybeInitialiseLibrary();

ST_PRIVATE_FUNCTION(Named="stMaybeShutdownLibrary", Text="Maybe Shutdown the Library", ThreadSafe);
void stMaybeShutdownLibrary();

#endif
