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

#include "stMath.h"

static u32 permuteQPR(unsigned int x)
{
  static const u32 prime = 4294967291u;
  if (x >= prime)
    return x;  // The 5 integers out of range are mapped to themselves.
  u32 residue = ((u64) x * x) % prime;
  return (x <= prime / 2) ? residue : prime - residue;
}

void stQpr_Initialise(stQpr* qpr, u32 seedBase, u32 seedOffset)
{
  qpr->index  = permuteQPR(permuteQPR(seedBase) + 0x682f0161);
  qpr->offset = permuteQPR(permuteQPR(seedOffset) + 0x46790905);
}

u32 stQpr_Next(stQpr* qpr)
{
  //ST_PRINT_CHATTERF("________0000000000000000000000000000000 Permate");
  return permuteQPR((permuteQPR(qpr->index++) + qpr->offset) ^ 0x5bf03635);
}
