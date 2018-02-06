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

ST_PUBLIC_FUNCTION(Named="stGetError", Text="Get an error or return 0.", ThreadSafe, Category="Recording")
ST_PUBLIC u32 stGetError();

u32 stPeekError();

void stRaiseError(u32 error);

stBool stHasError(u32 error);

void stClearErrors();

u32 stPopError();

void stPushError(u32 errorCodes);



#endif
