// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stReference.h
 *
 * Reference enums in relation to other values, or to euclidean space
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_REFERENCE_H
#define ST_REFERENCE_H

#include "stPlatform.h"

ST_PUBLIC_ENUM(Named="Unspecified",   Value=0, For="Reference")
#define ST_REFERENCE_UNSPECIFIED 0
ST_PUBLIC_ENUM(Named="World",         Value=1, For="Reference")
#define ST_REFERENCE_WORLD 1
ST_PUBLIC_ENUM(Named="Local",         Value=2, For="Reference")
#define ST_REFERENCE_LOCAL 2
ST_PUBLIC_ENUM(Named="InternalAsset", Value=3, For="Reference")
#define ST_REFERENCE_INTERNAL_ASSET 3
ST_PUBLIC_ENUM(Named="ExternalAsset", Value=4, For="Reference")
#define ST_REFERENCE_EXTERNAL_ASSET 4

#endif
