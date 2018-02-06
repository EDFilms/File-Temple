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

#ifndef ST_MATH_H
#define ST_MATH_H

#include "stPlatform.h"

ST_INLINE u32 ceilingDivision(u32 dividend, u32 divisor)
{
  if (dividend == 0)
      return 0;

  return 1 + ((dividend - 1) / divisor);
}

// http://preshing.com/20121224/how-to-generate-a-sequence-of-unique-random-integers/
typedef struct
{
  u32 index, offset;
} stQpr;

void stQpr_Initialise(stQpr* qpr, u32 seedBase, u32 seedOffset);

u32 stQpr_Next(stQpr* qpr);

#endif
