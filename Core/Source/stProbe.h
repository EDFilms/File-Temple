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

#ifndef ST_PROBE_H
#define ST_PROBE_H

#include "stPlatform.h"

#include <stdio.h>
#include <inttypes.h>

extern int  stProbe_TabCount;
extern int  stProbe_PushedTabCount;
extern char stProbe_LFChar;

ST_INLINE void stProbe_Indent()
{
  for(int i=0;i < stProbe_TabCount;i++)
    printf("\t");
}

ST_INLINE void stProbe_Begin()
{
  stProbe_Indent(); printf("{%c", stProbe_LFChar);
  stProbe_TabCount++;
}

ST_INLINE void stProbe_BeginWithName(const char* name)
{
  stProbe_Indent(); printf("\"%s\" : {%c", name, stProbe_LFChar);
  stProbe_TabCount++;
}

ST_INLINE void stProbe_End()
{
  stProbe_TabCount--;
  stProbe_Indent(); printf("}, %c", stProbe_LFChar);
}

ST_INLINE void stProbe_BeginListWithName(const char* name)
{
  stProbe_Indent(); printf("\"%s\" : [%c", name, stProbe_LFChar);
  stProbe_TabCount++;
}

ST_INLINE void stProbe_EndList()
{
  stProbe_TabCount--;
  stProbe_Indent(); printf("], %c", stProbe_LFChar);
}

ST_INLINE void stProbe_BeginInline()
{
  stProbe_Indent(); 
  stProbe_PushedTabCount = stProbe_TabCount;
  stProbe_TabCount = 0;
  stProbe_LFChar = ' ';
  printf("{%c", stProbe_LFChar);
}

ST_INLINE void stProbe_EndInline()
{
  stProbe_TabCount = stProbe_PushedTabCount;
  stProbe_LFChar = '\n';
  printf("}, %c", stProbe_LFChar);
}

ST_INLINE void stProbe_KeyInt(const char* name, int64_t value)
{
  stProbe_Indent(); printf("\"%s\" : %" PRId64 ",%c", name, value, stProbe_LFChar);
}

ST_INLINE void stProbe_KeyIntAsHex(const char* name, int64_t value)
{
  stProbe_Indent(); printf("\"%s\" : %" PRIx64 ",%c", name, value, stProbe_LFChar);
}

ST_INLINE void stProbe_KeyBool(const char* name, stBool value)
{
  stProbe_Indent(); printf("\"%s\" : %s,%c", name, value ? "true" : "false", stProbe_LFChar);
}

ST_INLINE void stProbe_KeyDouble(const char* name, double value)
{
  stProbe_Indent(); printf("\"%s\" : %f,%c", name, value, stProbe_LFChar);
}

ST_INLINE void stProbe_KeyPointer(const char* name, void* value)
{
  stProbe_Indent(); printf("\"%s\" : \"%p\",%c", name, value, stProbe_LFChar);
}

ST_INLINE void stProbe_KeyMemory(const char* name, void* mem, u32 size)
{
  ST_UNUSED(size);

  stProbe_Indent(); printf("\"%s\" : ", name);
  u8* p = (u8*) mem;
  if (p == NULL || size == 0)
  {
    printf("\"null\", %c", stProbe_LFChar);
    return;
  }

  printf("\"");
  for(u32 i=0;i < size;i++)
  {
    printf("%02X", p[i]);
  }
  printf("\",%c", stProbe_LFChar);
}

ST_INLINE void stProbe_KeyString(const char* name, const char* value)
{
  stProbe_Indent(); printf("\"%s\" : \"%s\",%c", name, value, stProbe_LFChar);
}
ST_INLINE void stProbe_ListItem_Integer(u64 value)
{
  stProbe_Indent(); printf("%" PRId64 ",%c", value, stProbe_LFChar);
}

ST_INLINE void stProbe_ListItem_IntegerHex(u64 value)
{
  stProbe_Indent(); printf("%" PRIX64 ",%c", value, stProbe_LFChar);
}

ST_INLINE void stProbe_ListItem_IntegerMem(u64 value)
{
  stProbe_Indent(); 
  
  u8* p = (u8*) ((void*) &value);
  
  for(u32 i=0;i < sizeof(u64);i++)
  {
    printf("%02X", p[i]);
  }

  printf("%c", stProbe_LFChar);
}

ST_INLINE void stProbe_ListItem_IntegerKey(u64 value)
{
  stProbe_Indent(); 

  u8* p = (u8*) ((void*) &value);

  for(u32 i=0;i < sizeof(u64);i++)
  {
    printf("%02X", p[7-i]);
  }

  printf("%c", stProbe_LFChar);
}

ST_INLINE void stProbe_ListItem_Pointer(void* value)
{
  stProbe_Indent(); printf("\"%p\",%c", value, stProbe_LFChar);
}

#endif
