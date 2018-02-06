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

#ifndef ST_OBJECT_HEADER
#define ST_OBJECT_HEADER

#include "stPlatform.h"
#include "stLinearAllocator.h"
#include "stCommandType.h"

struct stDeltaKeyValueT;
typedef struct stDeltaKeyValueT stDeltaKeyValue;

struct stDeltaKeyValueT
{
  stCommandKey              key;
  u8*                       data;
  stDeltaKeyValue*          next;
};

void stDeltaKeyValuePrint_Impl(stDeltaKeyValue* value, const char* comment);

struct stObjectHeaderT
{
  struct stObjectHeaderT *next, *prev;
  u32                     objectId;
  stDeltaKeyValue*        keys;
  u16                     count;
  // Object is loaded into memory (i.e. keys/mem exist in memory)
  stBool32                bLoaded : 1;
  // Object is persistant and should be saved.
  stBool32                bPersistant : 1;
  // Object was created here
  stBool32                bCreated : 1;
  // Object was deleted here
  stBool32                bDeleted : 1;
  // Object was updated here
  stBool32                bUpdated : 1;
};

ST_INLINE void stLoadObjectFlags(u8 flags, struct stObjectHeaderT* object)
{
  object->bCreated = ((flags & (1 << 0)) != 0);
  object->bDeleted = ((flags & (1 << 1)) != 0);
  object->bUpdated = ((flags & (1 << 2)) != 0);
}

ST_INLINE u8 stSaveObjectFlags(struct stObjectHeaderT* object)
{
  u8 flags = 0;
  if (object->bCreated)
    flags |= (1 << 0);
  if (object->bDeleted)
    flags |= (1 << 1);
  if (object->bUpdated)
    flags |= (1 << 2);
  return flags;
}

typedef struct stObjectHeaderT stObjectHeader;

struct stObjectHeadersT
{
  stObjectHeader           *first,   *last;
  u32                       count;    // how many objects
  u32                       memSize;  // Size of 'mem + keys' per object in bytes.
  stDynamicLinearAllocator  mem;      // Memory for stObjectHeader
};

typedef struct stObjectHeadersT stObjectHeaders;

void stObjectHeaderPrint_Impl(stObjectHeader* objectHeader, const char* comment);

#endif
