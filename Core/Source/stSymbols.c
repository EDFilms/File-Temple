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

#include "stPlatform.h"
#include "stSymbols.h"

#define ST_SYMBOL(N) #N,

static const char* kSymbolNamesStr[] =
{
#include "stSymbols.inc"
};

const char* stGetSymbolNamesStr(u8 symbol)
{
  if (symbol == 255)
    return "END";
  return kSymbolNamesStr[symbol];
}
