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

#ifndef ST_UNITS_H
#define ST_UNITS_H

#include "stPlatform.h"

ST_PUBLIC_ENUM(Named="Unspecified",   Value=0, For="Units")
#define ST_UNITS_UNSPECIFIED 0
ST_PUBLIC_ENUM(Named="Radian", Value=1, For="Units")
#define ST_UNITS_RADIAN 1
ST_PUBLIC_ENUM(Named="Degree", Value=2, For="Units")
#define ST_UNITS_DEGREE 2
ST_PUBLIC_ENUM(Named="Enumeration", Value=3, For="Units")
#define ST_UNITS_ENUMERATION 3

#endif
