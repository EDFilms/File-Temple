// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stTypes.c
 *
 * Portable way of expressing standard C compatible plain old data types
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stTypes.h"

const char* stGetTypeToString(stEnum32 type)
{
switch(type)
{
  case ST_TYPE_INT8:    return "Int8";
  case ST_TYPE_INT16:   return "Int16";
  case ST_TYPE_INT32:   return "Int32";
  case ST_TYPE_INT64:   return "Int64";
  case ST_TYPE_UINT8:   return "UInt8";
  case ST_TYPE_UINT16:  return "UInt16";
  case ST_TYPE_UINT32:  return "UInt32";
  case ST_TYPE_UINT64:  return "UInt64";
  case ST_TYPE_FLOAT32: return "Float32";
  case ST_TYPE_FLOAT64: return "Float64";
  case ST_TYPE_STRING:  return "String";
  case ST_TYPE_OBJECT:  return "Object";
  case ST_TYPE_OBJECT_TYPE:    return "Type";
  case ST_TYPE_CSTRING: return "CString";
  case ST_TYPE_ENUM:    return "Enum";
}

//ST_ALWAYS_ASSERT("Unknown type");
return "???";
}
