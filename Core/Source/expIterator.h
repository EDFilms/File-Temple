// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stIterator.h
 *
 * Represent a C++ object of the internal state of a SceneTrack iterator
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "expRequired.h"
#include "expMath.h"
#include "SceneTrack.h"

namespace EXP_NAMESPACE
{

  /**
   * Represents the state of a SceneTrack iterator whilst using the Schema
   * system to identify the Object's component, and providing some helper
   * functions to turn the data into more friendlier objects.
   */
  struct IteratorBase
  {
    u32   iterator;

    u32   frameNum;
    u32   lastObjectHandle;
    u32   objectHandle;
    u32   objectType;
    u32   componentHandle;
    u32   componentType;
    u32   iteratorType;
    u32   nbElements;
    u32   arraySize;
    f64   frameTime, frameLength;

    stBool32 bFrameChanged : 1;
    stBool32 bObjectChanged : 1;
    stBool32 bValueChanged : 1;

    /**
     * Fetch and learn information from the current SceneTrack iterator
     */
    void Update()
    {
      lastObjectHandle  = objectHandle;
      frameNum        = stIteratorGetFrame(iterator);
      objectHandle    = stIteratorGetObjectHandle(iterator);
      objectType      = stIteratorGetObjectType(iterator);
      componentHandle = stIteratorGetComponentHandle(iterator);
      componentType   = stIteratorGetValueType(iterator);
      iteratorType    = stIteratorGetType(iterator);
      nbElements      = stIteratorGetValueNbElements(iterator);
      arraySize       = stIteratorGetValueArraySize(iterator);
      frameTime       = stIteratorGetFrameTime(iterator);
      frameLength     = stIteratorGetFrameLength(iterator);

      u32 flags       = stIteratorGetChangeFlags(iterator);

      bFrameChanged  = ((flags & ST_ITERATORCHANGEFLAGS_FRAME)  != 0);
      bObjectChanged = ((flags & ST_ITERATORCHANGEFLAGS_OBJECT) != 0);
      bValueChanged  = ((flags & ST_ITERATORCHANGEFLAGS_VALUE)  != 0);
    }

    /**
     * Reset the iterator to the default values
     */
    void Reset()
    {
      iterator = 0;
      frameTime = 0.0f;
      frameNum = 0;
      lastObjectHandle = UINT32_MAX;
      objectHandle = 0;
      objectType = 0;
      componentHandle = 0;
      componentType = 0;
      iteratorType = 0;
      nbElements = 0;
      arraySize = 0;
      bFrameChanged = false;
      bObjectChanged = false;
      bValueChanged = false;
    }

    /**
     * Read a SceneTrack string, and return as a std::string
     */
    bool ReadString(std::string& str) const
    {
      s32 length = stIteratorGetValue_stringLength(iterator);
      if (length > 0)
      {
        str.reserve((u32) (length + 1));
        str.resize(length);
        return stIteratorGetValue_string(iterator, &str[0], length + 1) == length;
      }
      return false;
    }

    /**
     * Read a SceneTrack string, and return as a c-style string
     */
    bool ReadCString(char* str, size_t capacity) const
    {
      memset(str, 0, capacity);

      s32 length = stIteratorGetValue_stringLength(iterator);
      if (length > 0 && (length + 1) < capacity)
      {
        return stIteratorGetValue_string(iterator, &str[0], length + 1) == length;
      }
      return false;
    }

    /**
     * Read an array of any POD type, and insert into a std::vector
     */
    template<typename T>
    bool ReadArray(std::vector<T>& v) const
    {
      v.resize(arraySize);
      u32 r = stIteratorGetValueArray(iterator, &v[0], (u32) (v.size() * sizeof(T)), stCalculateStride1(Type<T>::kType, Type<T>::kNbElements));

      return r != arraySize;
    }

    /**
     * Read a unsigned int8 value
     */
    void ReadU8(u8& value) const
    {
      value = stIteratorGetValue_uint8(iterator, 0);
    }
    
    /**
     * Read a unsigned int16 value
     */
    void ReadU16(u16& value) const
    {
      value = stIteratorGetValue_uint16(iterator, 0);
    }
    
    /**
     * Read a unsigned int32 value
     */
    void ReadU32(u32& value) const
    {
      value = stIteratorGetValue_uint32(iterator, 0);
    }
    
    /**
     * Read a unsigned int64 value
     */
    void ReadU64(u64& value) const
    {
      value = stIteratorGetValue_uint64(iterator, 0);
    }
    
    /**
     * Read a Signed int8 value
     */
    void ReadS8(s8& value) const
    {
      value = stIteratorGetValue_uint8(iterator, 0);
    }
    
    /**
     * Read a Signed int16 value
     */
    void ReadS16(s16& value) const
    {
      value = stIteratorGetValue_int16(iterator, 0);
    }
    
    /**
     * Read a Signed int32 value
     */
    void ReadS32(s32& value) const
    {
      value = stIteratorGetValue_int32(iterator, 0);
    }
    
    /**
     * Read a Signed int64 value
     */
    void ReadS64(s64& value) const
    {
      value = stIteratorGetValue_int64(iterator, 0);
    }
    
    /**
     * Read a Signed float32 (float) value
     */
    void ReadF32(f32& value) const
    {
      value = stIteratorGetValue_float32(iterator, 0);
    }
    
    /**
     * Read a Signed float64 (double) value
     */
    void ReadF64(f64& value) const
    {
      value = stIteratorGetValue_float64(iterator, 0);
    }

    /**
     * Read a two-component uint32 vector
     */
    void ReadVector2U(Vector2u& value) const
    {
      value.x = stIteratorGetValue_uint32(iterator, 0);
      value.y = stIteratorGetValue_uint32(iterator, 1);
    }
    
    /**
     * Read a two-component uint32 vector as an rray
     */
    void ReadVectorU32_2(u32* value) const
    {
      value[0] = stIteratorGetValue_uint32(iterator, 0);
      value[1] = stIteratorGetValue_uint32(iterator, 1);
    }
    
    /**
     * Read a three-component float32 vector as an array
     */
    void ReadVectorF32_3(float* value) const
    {
      value[0] = stIteratorGetValue_float32(iterator, 0);
      value[1] = stIteratorGetValue_float32(iterator, 1);
      value[2] = stIteratorGetValue_float32(iterator, 2);
    }
    
    /**
     * Read a three-component float32 vector
     */
    void ReadVector3(Vector3f& value) const
    {
      value.x = stIteratorGetValue_float32(iterator, 0);
      value.y = stIteratorGetValue_float32(iterator, 1);
      value.z = stIteratorGetValue_float32(iterator, 2);
    }
    
    /**
     * Read a four-component float32 vector
     */
    void ReadVector4(Vector4f& value) const
    {
      value.x = stIteratorGetValue_float32(iterator, 0);
      value.y = stIteratorGetValue_float32(iterator, 1);
      value.z = stIteratorGetValue_float32(iterator, 2);
      value.w = stIteratorGetValue_float32(iterator, 3);
    }

  };
}
