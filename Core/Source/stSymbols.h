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

#ifndef ST_SYMBOLS_H
#define ST_SYMBOLS_H

#include "stPlatform.h"

#if defined(ST_SYMBOL)
#undef ST_SYMBOL
#endif

#define ST_SYMBOL(N) ST_SYMBOL_##N,
enum
{
#include "stSymbols.inc"
  ST_SYMBOLS_COUNT,
  ST_SYMBOLS_END_MARKER = 255
};

#undef ST_SYMBOL
const char* stGetSymbolNamesStr(u8 symbol);

#endif
