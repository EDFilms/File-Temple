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

#ifndef ST_VECTOR_H
#define ST_VECTOR_H

#include "stPlatform.h"

#define ST_VECTOR_T(TYPE)             struct { TYPE *mem; u32 size, capacity; }
#define stVectorInitialise(V)         do { (V).size = (V).capacity = 0; (V).mem = NULL; } while(0)
#define stVectorShutdown(V)           do { ST_RELEASE_MEM((V).mem); stVectorInitialise((V)); } while(0)
#define stVectorAt(V, I)              ((V).mem[(I)])
#define stVectorSize(V)               ((V).size)
#define stVectorCapacity(V)           ((V).capacity)
#define stVectorEmpty(V)              ((V).size == 0)
#define stVectorOne(V)                ((V).size == 1)
#define stVectorAny(V)                ((V).size != 0)
#define stVectorFull(V)               ((V).size == (V).capacity)
#define stVectorResize(TYPE, V, S)    do { (V).capacity = (S); (V).mem = ST_REALLOCATE_MEM((V).mem, (S) * sizeof(TYPE)); } while(0)
#define stVectorReserve(TYPE, V, S)   do { if (stVectorCapacity((V)) < S) { stVectorResize(TYPE, V, S); }  } while(0)
#define stVectorPop(V)                ((V).mem[--(V).size])
#define stVectorPush(TYPE, V, X)      do {\
                                          if (stVectorFull(V)) { stVectorResize(TYPE, V, (((V).capacity) == 0 ? 8 : (V).capacity * 2)); }\
                                          (V).mem[(V).size++] = (X);\
                                      } while(0)

#endif
