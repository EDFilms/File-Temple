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

#ifndef ST_BITS_H
#define ST_BITS_H

#include "stPlatform.h"
#include "stError.h"

struct stBitSetT
{
  stBool32* bits;
  u32       nbDWords;
};

ST_INLINE void stBitSet_Initialise(struct stBitSetT* set, u32 nbBits)
{
  set->nbDWords = (nbBits + 31) >> 5;
  set->bits = ST_ALLOCATE_MEM(sizeof(u32) * set->nbDWords, ST_DEFAULT_ALIGNMENT);
  memset(set->bits, 0, set->nbDWords * sizeof(u32));
}

ST_INLINE void stBitSet_Initialise2(struct stBitSetT* set, u32 nbDWords)
{
  set->nbDWords = nbDWords;
  set->bits = ST_ALLOCATE_MEM(sizeof(u32) * set->nbDWords, ST_DEFAULT_ALIGNMENT);
  memset(set->bits, 0, set->nbDWords * sizeof(u32));
}

ST_INLINE void stBitSet_Shutdown(struct stBitSetT* set)
{
  if (set->bits != NULL)
  {
    ST_RELEASE_MEM(set->bits);
    set->bits = NULL;
    set->nbDWords = 0;
  }
}

ST_INLINE void stBit_SetBit(struct stBitSetT* set, u32 bit)
{
  ST_ASSERT(bit < set->nbDWords * 32, "Out of bounds");
  set->bits[bit >> 5] |= (1 << (bit & 31));
}

ST_INLINE void stBit_ClearBit(struct stBitSetT* set, u32 bit)
{
  ST_ASSERT(bit < set->nbDWords * 32, "Out of bounds");
  set->bits[bit >> 5] &= ~(1 << (bit & 31));
}

ST_INLINE stBool32 stBit_GetBit(struct stBitSetT* set, u32 bit)
{
  ST_ASSERT(bit < set->nbDWords * 32, "Out of bounds");
  return ((set->bits[bit >> 5] & (1 << (bit & 31))));
}

struct stDynamicBitSetT
{
  stBool32* bits;
  u32       nbDWords;
  int       reference;
};

ST_INLINE void stDynamicBitSet_InitialiseZero(struct stDynamicBitSetT* set)
{
  set->nbDWords = 0;
  set->bits = NULL;
}

ST_INLINE void stDynamicBitSet_Initialise(struct stDynamicBitSetT* set, u32 nbBits)
{
  set->nbDWords = 1 + ((nbBits + 31) >> 5);
  set->bits = NULL;
  set->bits = ST_REALLOCATE_MEM(set->bits, sizeof(u32) * (1 + set->nbDWords));
  memset(set->bits, 0, (1 + set->nbDWords) * sizeof(u32));
}

ST_INLINE void stDynamicBitSet_Initialise2(struct stDynamicBitSetT* set, u32 nbDWords)
{
  set->nbDWords = nbDWords;
  set->bits = ST_REALLOCATE_MEM(set->bits, sizeof(u32) * (1 + set->nbDWords));
  memset((u8*) set->bits, 0, (1 + set->nbDWords) * sizeof(u32));
}

ST_INLINE void stDynamicBitSet_Shutdown(struct stDynamicBitSetT* set)
{
  if (set->bits != NULL)
  {
    //ST_RELEASE_MEM(set->bits);
    ST_REALLOCATE_MEM_REF(set->bits, 0, set->reference);
    set->bits = NULL;
    set->nbDWords = 0;
  }
}

ST_INLINE void stDynamicBitSet_MaybeGrow(struct stDynamicBitSetT* set, u32 bits)
{
  u32 nbDwords = (bits + 31) >> 5;
  if (nbDwords > set->nbDWords)
  {
    set->bits = ST_REALLOCATE_MEM(set->bits, sizeof(u32) * nbDwords);

    if (set->bits == NULL)
    {
      stRaiseError(ST_ERROR_MEMORY);
      return;
    }

    memset(((u8*) set->bits) + (set->nbDWords * sizeof(u32)), 0, (nbDwords - set->nbDWords) * sizeof(u32));

    set->nbDWords = nbDwords;
  }
}

ST_INLINE void stDynamicBitSet_SetBit(struct stDynamicBitSetT* set, u32 bit)
{
  stDynamicBitSet_MaybeGrow(set, bit + 1);
  set->bits[bit >> 5] |= (1 << (bit & 31));
}

ST_INLINE void stDynamicBitSet_ClearBit(struct stDynamicBitSetT* set, u32 bit)
{
  stDynamicBitSet_MaybeGrow(set, bit + 1);
  set->bits[bit >> 5] &= ~(1 << (bit & 31));
}

ST_INLINE stBool32 stDynamicBitSet_GetBit(struct stDynamicBitSetT* set, u32 bit)
{
  if (bit >= set->nbDWords * 32)
  {
    return false;
  }
  u32 b = (bit >> 5);
  return ((set->bits[b] & (1 << (bit & 31))));
}

#endif
