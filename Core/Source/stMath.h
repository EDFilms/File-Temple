// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stMath.h
 *
 * Helper math and unique-id functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_MATH_H
#define ST_MATH_H

#include "stPlatform.h"

/**
 * Perform a ceiling division on two numbers
 */
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

/**
 * Create a QPR struct for generating unique/random numbers.
 * @see stQpr_Next
 */
void stQpr_Initialise(stQpr* qpr, u32 seedBase, u32 seedOffset);

/**
 * Get a unique/random number
 * @return the unique/random number
 */
u32 stQpr_Next(stQpr* qpr);

#endif
