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

#ifndef ST_TYPES_H
#define ST_TYPES_H

#include "stPlatform.h"

ST_PUBLIC_ENUM(Named="None",     Value=0, For="Type")
#define ST_TYPE_NONE      0
ST_PUBLIC_ENUM(Named="Uint8",     Value=1, For="Type")
#define ST_TYPE_UINT8     1
ST_PUBLIC_ENUM(Named="Uint16",    Value=2, For="Type")
#define ST_TYPE_UINT16    2
ST_PUBLIC_ENUM(Named="Uint32",    Value=3, For="Type")
#define ST_TYPE_UINT32    3
ST_PUBLIC_ENUM(Named="Uint64",    Value=4, For="Type")
#define ST_TYPE_UINT64    4
ST_PUBLIC_ENUM(Named="Int8",      Value=5, For="Type")
#define ST_TYPE_INT8      5
ST_PUBLIC_ENUM(Named="Int16",     Value=6, For="Type")
#define ST_TYPE_INT16     6
ST_PUBLIC_ENUM(Named="Int32",     Value=7, For="Type")
#define ST_TYPE_INT32     7
ST_PUBLIC_ENUM(Named="Int64",     Value=8, For="Type")
#define ST_TYPE_INT64     8
ST_PUBLIC_ENUM(Named="Float32",   Value=9, For="Type")
#define ST_TYPE_FLOAT32   9
ST_PUBLIC_ENUM(Named="Float64",   Value=10, For="Type")
#define ST_TYPE_FLOAT64   10
ST_PUBLIC_ENUM(Named="String",    Value=11, For="Type")
#define ST_TYPE_STRING    11
ST_PUBLIC_ENUM(Named="Object",    Value=12, For="Type")
#define ST_TYPE_OBJECT    12
ST_PUBLIC_ENUM(Named="ObjectType",      Value=13, For="Type")
#define ST_TYPE_OBJECT_TYPE      13
ST_PRIVATE_ENUM(Named="CString",  Value=21, For="Type")
#define ST_TYPE_CSTRING   21
ST_PRIVATE_ENUM(Named="Enum",     Value=22, For="Type")
#define ST_TYPE_ENUM      22
ST_PRIVATE_ENUM(Named="Marker",   Value=255, For="Type")
#define ST_TYPE_MARKER    255

ST_INLINE u32 stGetTypeMemorySize(stEnum32 type, u32 nbElements, u32 arraySize)
{
  switch(type)
  {
    case ST_TYPE_INT8:    return sizeof(s8)     * nbElements * arraySize;
    case ST_TYPE_INT16:   return sizeof(s16)    * nbElements * arraySize;
    case ST_TYPE_INT32:   return sizeof(s32)    * nbElements * arraySize;
    case ST_TYPE_INT64:   return sizeof(s64)    * nbElements * arraySize;
    case ST_TYPE_UINT8:   return sizeof(u8)     * nbElements * arraySize;
    case ST_TYPE_UINT16:  return sizeof(u16)    * nbElements * arraySize;
    case ST_TYPE_UINT32:  return sizeof(u32)    * nbElements * arraySize;
    case ST_TYPE_UINT64:  return sizeof(u64)    * nbElements * arraySize;
    case ST_TYPE_FLOAT32: return sizeof(f32)    * nbElements * arraySize;
    case ST_TYPE_FLOAT64: return sizeof(f64)    * nbElements * arraySize;
    case ST_TYPE_STRING:  return sizeof(char)   * nbElements * arraySize;
    case ST_TYPE_OBJECT:  return sizeof(u32)    * nbElements * arraySize;
    case ST_TYPE_OBJECT_TYPE:    return sizeof(u32)    * nbElements * arraySize;
    case ST_TYPE_CSTRING: return sizeof(char)   * nbElements * arraySize;
    case ST_TYPE_ENUM:    return sizeof(u8)     * nbElements * arraySize;
    case ST_TYPE_MARKER:  return sizeof(u8)     * nbElements * arraySize;
  }

  ST_ALWAYS_ASSERT("Unknown type");
  return 0;
}

const char* stGetTypeToString(stEnum32 type);

#define ST_MACRO_SHORTHANDTYPE_u8  ST_TYPE_UINT8  
#define ST_MACRO_SHORTHANDTYPE_u16 ST_TYPE_UINT16 
#define ST_MACRO_SHORTHANDTYPE_u32 ST_TYPE_UINT32 
#define ST_MACRO_SHORTHANDTYPE_u64 ST_TYPE_UINT64 
#define ST_MACRO_SHORTHANDTYPE_s8  ST_TYPE_INT8   
#define ST_MACRO_SHORTHANDTYPE_s16 ST_TYPE_INT16  
#define ST_MACRO_SHORTHANDTYPE_s32 ST_TYPE_INT32  
#define ST_MACRO_SHORTHANDTYPE_s64 ST_TYPE_INT64  
#define ST_MACRO_SHORTHANDTYPE_f32 ST_TYPE_FLOAT32
#define ST_MACRO_SHORTHANDTYPE_f64 ST_TYPE_FLOAT64
#define ST_MACRO_SHORTHANDTYPE_str ST_TYPE_STRING

#endif
