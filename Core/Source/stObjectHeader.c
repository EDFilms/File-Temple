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

#include "stObjectHeader.h"
#include "stProbe.h"
#include "stCommandKey.h"

void stDeltaKeyValuePrint_Impl(stDeltaKeyValue* value, const char* comment)
{
  stProbe_BeginWithName(comment);
  {
    if (value == NULL)
    {
      goto end;
    }

    stProbe_KeyIntAsHex("key", value->key);
    stProbe_KeyMemory("data", value->data, stGetCommandSize(value->key));

  }
  end:
  stProbe_End();
}

void stObjectHeaderPrint_Impl(stObjectHeader* objectHeader, const char* comment)
{
  stProbe_BeginWithName(comment);
  {
    if (objectHeader == NULL)
    {
      goto end;
    }

    stProbe_KeyIntAsHex("objectId", objectHeader->objectId);
    stProbe_KeyInt("count", objectHeader->count);


    stProbe_BeginWithName("flags");
    {
      stProbe_KeyBool("bLoaded", objectHeader->bLoaded);
      stProbe_KeyBool("bPersistant", objectHeader->bPersistant);
      stProbe_KeyBool("bCreated", objectHeader->bCreated);
      stProbe_KeyBool("bDeleted", objectHeader->bDeleted);
      stProbe_KeyBool("bUpdated", objectHeader->bUpdated);
    }
    stProbe_End();

  }
  end:
  stProbe_End();
}
