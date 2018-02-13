// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stLibrary.h
 *
 * Library Initialiser and Shutdown functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_LIBRARY_H
#define ST_LIBRARY_H

#include "stPlatform.h"

/**
 * Tries to initialise the SceneTrack library.
 *
 * If this is the first time then the log file is opened.
 *
 * This function is thread safe.
 * @return An unique session id
 */
ST_PRIVATE_FUNCTION(Named="stMaybeInitialiseLibrary", Text="Maybe Initialise the Library", ThreadSafe);
u32 stMaybeInitialiseLibrary();

/**
 * Tries to shutdown the SceneTrack library, after being initialised.
 *
 * This function will assert if the library was not initialised.
 * This function is thread safe.
 */
ST_PRIVATE_FUNCTION(Named="stMaybeShutdownLibrary", Text="Maybe Shutdown the Library", ThreadSafe);
void stMaybeShutdownLibrary();

#endif
