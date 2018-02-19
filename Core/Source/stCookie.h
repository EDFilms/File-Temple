// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stCookie.h
 *
 * Id number generator based of a random number generator
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_COOKIE_H
#define ST_COOKIE_H

#include "stPlatform.h"

u64 stGenerate64BitCookie(u64 offset);

u32 stGenerate32BitCookie(u32 offset);

#endif
