// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stError.h
 *
 * Error Handler
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_ERROR_H
#define ST_ERROR_H

#include "stPlatform.h"


ST_PUBLIC_ENUM(Named="Memory",    Value=1, For="Error", Text="Memory allocation and releasing")
#define ST_ERROR_MEMORY     1

ST_PUBLIC_ENUM(Named="Disk",      Value=2, For="Error", Text="Disk operations")
#define ST_ERROR_DISK       2

ST_PUBLIC_ENUM(Named="Recording", Value=4, For="Error", Text="Recording operations")
#define ST_ERROR_RECORDING  4

ST_PUBLIC_ENUM(Named="Playing",   Value=8, For="Error", Text="Playing operations")
#define ST_ERROR_PLAYING    8

ST_PUBLIC_ENUM(Named="Writing",   Value=16, For="Error", Text="Writing recordings to disk")
#define ST_ERROR_WRITING    16

ST_PUBLIC_ENUM(Named="Reading",   Value=32, For="Error", Text="Reading recordings from disk")
#define ST_ERROR_READING    32

ST_PUBLIC_ENUM(Named="Type",      Value=64, For="Error", Text="Class and Type operations")
#define ST_ERROR_TYPE      64

ST_PUBLIC_ENUM(Named="Reserve",   Value=128, For="Error", Text="Object and Frame Performance Reservations")
#define ST_ERROR_RESERVE    128

ST_PUBLIC_ENUM(Named="Iterator",   Value=256, For="Error", Text="Iterators")
#define ST_ERROR_ITERATOR    256

/**
 * Get an error or return 0
 * @public
 */
ST_PUBLIC_FUNCTION(Named="stGetError", Text="Get an error or return 0.", ThreadSafe, Category="Recording")
ST_PUBLIC u32 stGetError();

/**
 * Peek if there is an error but do not clear it
 */
u32 stPeekError();

/**
 * Raise an error
 */
void stRaiseError(u32 error);

/**
 * Is there an error?
 */
stBool stHasError(u32 error);

/**
 * Clear errors
 */
void stClearErrors();

/**
 * Return the errors and set the errors to 0.
 */
u32 stPopError();

/**
 * Set the errors to given
 */
void stPushError(u32 errorCodes);



#endif
