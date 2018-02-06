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

#ifndef ST_ASSET_H
#define ST_ASSET_H

#include "stPlatform.h"
#include "stPool.h"
#include "stBits.h"

ST_PUBLIC_ENUM(Named="Static",  Value=0, For="Frequency")
#define ST_OBJECT_FREQUENCY_STATIC  0
ST_PUBLIC_ENUM(Named="Dynamic", Value=1, For="Frequency")
#define ST_OBJECT_FREQUENCY_DYNAMIC 1
ST_PUBLIC_ENUM(Named="Stream",  Value=2, For="Frequency")
#define ST_OBJECT_FREQUENCY_STREAM  2
ST_PUBLIC_ENUM(Named="Event",   Value=3, For="Frequency")
#define ST_OBJECT_FREQUENCY_EVENT   3

typedef struct stComponentT
{
  u32 memSize;
  u16 id;
  u16 kind;
  s32 capacity;
  u8  type, nbElements, flags;
  u8  units, reference;
} stComponent;

#define ST_MAX_COMPONENTS_PER_CLASS (32)

typedef struct stClassT
{
  struct stClassT *next, *prev;
  u32 id;
  u32 userData;
  stComponent* components;
  u8  nbComponents;
  u8  frequency, flags, reserved1, reserved2;
  u32 nbUses;
  struct stDynamicBitSetT objects;
  stByte componentMem[sizeof(stComponent) * ST_MAX_COMPONENTS_PER_CLASS];
} stClass;

#define ST_MAX_CLASSES_PER_POOL (32)

struct stClassesT
{
  stClass      *first, *last;
  u32           nextClassId;
  u32           nextObjectId;
  u32           nextEventId;
  stDynamicPool mem;
};

typedef struct stClassesT stClasses;

ST_PRIVATE_FUNCTION()
void stInitialiseClasses(stClasses* classes);

ST_PRIVATE_FUNCTION()
void stShutdownClasses(stClasses* classes);

ST_PRIVATE_FUNCTION()
void stResetEventIds(stClasses* classes);

ST_PRIVATE_FUNCTION()
s32 stKindToComponent(stClasses* classes, u32 classId, u32 kind);

ST_PRIVATE_FUNCTION()
s32 stComponentToKind(stClasses* classes, u32 classId, u32 id);

stClass* stGetClass_NoMutex(u32 id);
stClass* stGetClass_NoMutex_NoAssert(u32 id);

ST_INLINE stBool stObjectIdIsEvent(u32 id) { return id >= 0x7FFFFF; }

ST_PUBLIC_FUNCTION(Named="stCreateObjectType", Text="Create Type of Object", Category="Object", ThreadSafe, _objectFrequency="The usage of the Objects of this type when recording", _return="The Object type handle")
ST_PUBLIC u32 stCreateObjectType(stEnum32 objectFrequency);

ST_PUBLIC_FUNCTION(Named="stCreateObjectType", Text="Create Type of Object", Category="Object", ThreadSafe, _objectFrequency="The usage of the Objects of this type when recording", _userData="An user data integer to associate the Type with", _return="The Object type handle")
ST_PUBLIC u32 stCreateObjectTypeEx(stEnum32 objectFrequency, u32 userData);

stClass* stCreateObjectTypeImpl(u32 id, stClasses* classes);

ST_PUBLIC_FUNCTION(Named="stFindObjectType", Text="Find the first type of an Object associated with an user data integer", Category="Object", ThreadSafe, _userData="The user data integer to look for", _return="The Object type Handle, or 0 if it was not found")
ST_PUBLIC u32 stFindObjectType(u32 userData);

ST_PUBLIC_FUNCTION(Named="stAddObjectComponent", Text="Add component to object type", Category="Object", ThreadSafe, _return="The component handle", _objectType="Type of Object", _kind="Kind of component", _type="Type of Component (ST_TYPE)", _nbElements="Number of elements per component. Must be >= 1")
ST_PUBLIC u32 stAddObjectTypeComponent(u32 objectType, stEnum32 kind, stEnum32 type, u32 nbElements);

ST_PUBLIC_FUNCTION(Named="stAddObjectComponentEx", Text="Add component to object type (extended)", Category="Object", ThreadSafe, _return="The component handle", _objectType="Type of Object", _kind="Kind of component", _type="Type of Component (ST_TYPE)", _nbElements="Number of elements per component. Must be >= 1", _arrayCapacity="Maximum number of items in the array. Must be >= 1")
ST_PUBLIC u32 stAddObjectTypeComponentEx(u32 objectType, stEnum32 kind, stEnum32 type, u32 nbElements, u32 arrayCapacity);

ST_PUBLIC_FUNCTION(Named="stAddObjectComponentEx", Text="Add component to object type (extended full)", Category="Object", ThreadSafe, _return="The component handle", _objectType="Type of Object", _kind="Kind of component", _type="Type of Component (ST_TYPE)", _nbElements="Number of elements per component. Must be >= 1", _arrayCapacity="Maximum number of items in the array. Must be >= 1", _units="Unit associated with the values of the component", _reference="What the values are in reference to")
ST_PUBLIC u32 stAddObjectTypeComponentEx2(u32 objectType, stEnum32 kind, stEnum32 type, u32 nbElements, u32 arrayCapacity, stEnum32 units, stEnum32 reference);

stComponent* stAddObjectTypeComponentImpl(u32 id, stClass* class);

ST_PUBLIC_FUNCTION(Named="stFindObjectTypeComponent", Text="Find the first component of an Object according to its Kind", Category="Object", ThreadSafe, _return="The component handle or 0 if it could not be found", _objectType="The handle to the type of the object", _kind="The kind of component to look for")
ST_PUBLIC u32 stFindObjectTypeComponent(u32 objectType, u32 kind);


ST_PUBLIC_FUNCTION(Named="stCreateObject", Text="Create Object", Category="Object", ThreadSafe, _objectType="Handle to the Object Type", _return="The created Object Handle, or 0 if the Object was not created.")
ST_PUBLIC u32 stCreateObject(u32 objectType);

ST_PUBLIC_FUNCTION(Named="stDestroyObject", Text="Destroy Object", Category="Object", ThreadSafe, _objectHandle="Handle of the Object to delete")
ST_PUBLIC void stDestroyObject(u32 objectHandle);

ST_PUBLIC_FUNCTION(Named="stGetObjectType", Text="Get Object Type of Object", Category="Object", ThreadSafe, _objectHandle="Handle of the Object", _return="The Object Type, or 0 if the Type of Object cannot be found")
ST_PUBLIC u32 stGetObjectType(u32 objectHandle);


u32 stGetObjectType_NoMutex(struct stClassesT* classes, u32 objectHandle);


ST_PUBLIC_FUNCTION(Named="stSetValue_uint8", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint8", _objectHandle="Handle to the Object", _component="Component Handle", _x="The Value")
ST_PUBLIC void stSetValue_uint8(u32 objectHandle, u32 component, u8 x);

ST_PUBLIC_FUNCTION(Named="stSetValue_uint16", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint16", _objectHandle="Handle to the Object", _component="Component Handle", _x="The Value")
ST_PUBLIC void stSetValue_uint16(u32 objectHandle, u32 component, u16 x);

ST_PUBLIC_FUNCTION(Named="stSetValue_uint32", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint32", _objectHandle="Handle to the Object", _component="Component Handle", _x="The Value")
ST_PUBLIC void stSetValue_uint32(u32 objectHandle, u32 component, u32 x);

ST_PUBLIC_FUNCTION(Named="stSetValue_uint64", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint64", _objectHandle="Handle to the Object", _component="Component Handle", _x="The Value")
ST_PUBLIC void stSetValue_uint64(u32 objectHandle, u32 component, u64 x);

ST_PUBLIC_FUNCTION(Named="stSetValue_int8", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int8", _objectHandle="Handle to the Object", _component="Component Handle", _x="The Value")
ST_PUBLIC void stSetValue_int8(u32 objectHandle, u32 component, s8 x);

ST_PUBLIC_FUNCTION(Named="stSetValue_int16", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int16", _objectHandle="Handle to the Object", _component="Component Handle", _x="The Value")
ST_PUBLIC void stSetValue_int16(u32 objectHandle, u32 component, s16 x);

ST_PUBLIC_FUNCTION(Named="stSetValue_int32", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int32", _objectHandle="Handle to the Object", _component="Component Handle", _x="The Value")
ST_PUBLIC void stSetValue_int32(u32 objectHandle, u32 component, s32 x);

ST_PUBLIC_FUNCTION(Named="stSetValue_int64", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int64", _objectHandle="Handle to the Object", _component="Component Handle", _x="The Value")
ST_PUBLIC void stSetValue_int64(u32 objectHandle, u32 component, s64 x);

ST_PUBLIC_FUNCTION(Named="stSetValue_float32", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_float32", _objectHandle="Handle to the Object", _component="Component Handle", _x="The Value")
ST_PUBLIC void stSetValue_float32(u32 objectHandle, u32 component, f32 x);

ST_PUBLIC_FUNCTION(Named="stSetValue_float64", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_float64", _objectHandle="Handle to the Object", _component="Component Handle", _x="The Value")
ST_PUBLIC void stSetValue_float64(u32 objectHandle, u32 component, f64 x);

ST_PUBLIC_FUNCTION(Named="stSetValue_string", Text="Set Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_string", _objectHandle="Handle to the Object", _component="Component Handle", _str="A pointer to a C style ASCII string", _strLen="Number of characters of the string, excluding any terminating null character")
ST_PUBLIC void stSetValue_string(u32 objectHandle, u32 component, stCStr str, u32 strLength);



ST_PUBLIC_FUNCTION(Named="stSetValue_2_uint8", Text="Set 2-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint8", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value")
ST_PUBLIC void stSetValue_2_uint8(u32 objectHandle, u32 component, u8 x, u8 y);

ST_PUBLIC_FUNCTION(Named="stSetValue_2_uint16", Text="Set 2-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint16", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value")
ST_PUBLIC void stSetValue_2_uint16(u32 objectHandle, u32 component, u16 x, u16 y);

ST_PUBLIC_FUNCTION(Named="stSetValue_2_uint32", Text="Set 2-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint32", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value")
ST_PUBLIC void stSetValue_2_uint32(u32 objectHandle, u32 component, u32 x, u32 y);

ST_PUBLIC_FUNCTION(Named="stSetValue_2_uint64", Text="Set 2-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint64", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value")
ST_PUBLIC void stSetValue_2_uint64(u32 objectHandle, u32 component, u64 x, u64 y);

ST_PUBLIC_FUNCTION(Named="stSetValue_2_int8", Text="Set 2-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int8", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value")
ST_PUBLIC void stSetValue_2_int8(u32 objectHandle, u32 component, s8 x, s8 y);

ST_PUBLIC_FUNCTION(Named="stSetValue_2_int16", Text="Set 2-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uintXX", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value")
ST_PUBLIC void stSetValue_2_int16(u32 objectHandle, u32 component, s16 x, s16 y);

ST_PUBLIC_FUNCTION(Named="stSetValue_2_int32", Text="Set 2-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uintXX", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value")
ST_PUBLIC void stSetValue_2_int32(u32 objectHandle, u32 component, s32 x, s32 y);

ST_PUBLIC_FUNCTION(Named="stSetValue_2_int64", Text="Set 2-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uintXX", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value")
ST_PUBLIC void stSetValue_2_int64(u32 objectHandle, u32 component, s64 x, s64 y);

ST_PUBLIC_FUNCTION(Named="stSetValue_2_float32", Text="Set 2-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uintXX", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value")
ST_PUBLIC void stSetValue_2_float32(u32 objectHandle, u32 component, f32 x, f32 y);

ST_PUBLIC_FUNCTION(Named="stSetValue_2_float64", Text="Set 2-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uintXX", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value")
ST_PUBLIC void stSetValue_2_float64(u32 objectHandle, u32 component, f64 x, f64 y);



ST_PUBLIC_FUNCTION(Named="stSetValue_3_uint8", Text="Set 3-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint8", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value")
ST_PUBLIC void stSetValue_3_uint8(u32 objectHandle, u32 component, u8 x, u8 y, u8 z);

ST_PUBLIC_FUNCTION(Named="stSetValue_3_uint16", Text="Set 3-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint16", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value")
ST_PUBLIC void stSetValue_3_uint16(u32 objectHandle, u32 component, u16 x, u16 y, u16 z);

ST_PUBLIC_FUNCTION(Named="stSetValue_3_uint32", Text="Set 3-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint32", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value")
ST_PUBLIC void stSetValue_3_uint32(u32 objectHandle, u32 component, u32 x, u32 y, u32 z);

ST_PUBLIC_FUNCTION(Named="stSetValue_3_uint64", Text="Set 3-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint64", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value")
ST_PUBLIC void stSetValue_3_uint64(u32 objectHandle, u32 component, u64 x, u64 y, u64 z);

ST_PUBLIC_FUNCTION(Named="stSetValue_3_int8", Text="Set 3-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int8", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value")
ST_PUBLIC void stSetValue_3_int8(u32 objectHandle, u32 component, s8 x, s8 y, s8 z);

ST_PUBLIC_FUNCTION(Named="stSetValue_3_int16", Text="Set 3-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uintXX", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value")
ST_PUBLIC void stSetValue_3_int16(u32 objectHandle, u32 component, s16 x, s16 y, s16 z);

ST_PUBLIC_FUNCTION(Named="stSetValue_3_int32", Text="Set 3-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uintXX", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value")
ST_PUBLIC void stSetValue_3_int32(u32 objectHandle, u32 component, s32 x, s32 y, s32 z);

ST_PUBLIC_FUNCTION(Named="stSetValue_3_int64", Text="Set 3-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uintXX", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value")
ST_PUBLIC void stSetValue_3_int64(u32 objectHandle, u32 component, s64 x, s64 y, s64 z);

ST_PUBLIC_FUNCTION(Named="stSetValue_3_float32", Text="Set 3-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uintXX", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value")
ST_PUBLIC void stSetValue_3_float32(u32 objectHandle, u32 component, f32 x, f32 y, f32 z);

ST_PUBLIC_FUNCTION(Named="stSetValue_3_float64", Text="Set 3-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uintXX", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value")
ST_PUBLIC void stSetValue_3_float64(u32 objectHandle, u32 component, f64 x, f64 y, f64 z);



ST_PUBLIC_FUNCTION(Named="stSetValue_4_uint8", Text="Set 4-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint8", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value", _w="The Fourth Value")
ST_PUBLIC void stSetValue_4_uint8(u32 objectHandle, u32 component, u8 x, u8 y, u8 z, u8 w);

ST_PUBLIC_FUNCTION(Named="stSetValue_4_uint16", Text="Set 4-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint16", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value", _w="The Fourth Value")
ST_PUBLIC void stSetValue_4_uint16(u32 objectHandle, u32 component, u16 x, u16 y, u16 z, u16 w);

ST_PUBLIC_FUNCTION(Named="stSetValue_4_uint32", Text="Set 4-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint32", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value", _w="The Fourth Value")
ST_PUBLIC void stSetValue_4_uint32(u32 objectHandle, u32 component, u32 x, u32 y, u32 z, u32 w);

ST_PUBLIC_FUNCTION(Named="stSetValue_4_uint64", Text="Set 4-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint64", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value", _w="The Fourth Value")
ST_PUBLIC void stSetValue_4_uint64(u32 objectHandle, u32 component, u64 x, u64 y, u64 z, u64 w);

ST_PUBLIC_FUNCTION(Named="stSetValue_4_int8", Text="Set 4-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int8", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value", _w="The Fourth Value")
ST_PUBLIC void stSetValue_4_int8(u32 objectHandle, u32 component, s8 x, s8 y, s8 z, s8 w);

ST_PUBLIC_FUNCTION(Named="stSetValue_4_int16", Text="Set 4-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int16", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value", _w="The Fourth Value")
ST_PUBLIC void stSetValue_4_int16(u32 objectHandle, u32 component, s16 x, s16 y, s16 z, s16 w);

ST_PUBLIC_FUNCTION(Named="stSetValue_4_int32", Text="Set 4-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int32", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value", _w="The Fourth Value")
ST_PUBLIC void stSetValue_4_int32(u32 objectHandle, u32 component, s32 x, s32 y, s32 z, s32 w);

ST_PUBLIC_FUNCTION(Named="stSetValue_4_int64", Text="Set 4-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int64", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value", _w="The Fourth Value")
ST_PUBLIC void stSetValue_4_int64(u32 objectHandle, u32 component, s64 x, s64 y, s64 z, s64 w);

ST_PUBLIC_FUNCTION(Named="stSetValue_4_float32", Text="Set 4-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_float32", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value", _w="The Fourth Value")
ST_PUBLIC void stSetValue_4_float32(u32 objectHandle, u32 component, f32 x, f32 y, f32 z, f32 w);

ST_PUBLIC_FUNCTION(Named="stSetValue_4_float64", Text="Set 4-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_float64", _objectHandle="Handle to the Object", _component="Component Handle", _x="The First Value", _y="The Second Value", _z="The Third Value", _w="The Fourth Value")
ST_PUBLIC void stSetValue_4_float64(u32 objectHandle, u32 component, f64 x, f64 y, f64 z, f64 w);

ST_PUBLIC_FUNCTION(Named="stCalculatePODVectorMemorySize", Text="Calculate the memory size of a simple POD-style 1 or N sized vector.", Category="Object", ThreadSafe, _return="Number of bytes", _type="A POD Type. Represented as ST_TYPE", _nbElements="Number of elements in the vector", _arraySize="Number of vector items in the array")
ST_PUBLIC u32  stCalculatePODVectorMemorySize(stEnum32 type, u32 nbElements, u32 arraySize);

ST_PUBLIC_FUNCTION(Named="stCalculateStride1", Text="Calculate stride of given types", Category="Object", ThreadSafe, _return="Number of bytes", _type="A POD Type. Represented as ST_TYPE", _count1="Number of types")
ST_PUBLIC u32  stCalculateStride1(stEnum32 type1, u32 count1);

ST_PUBLIC_FUNCTION(Named="stCalculateStride2", Text="Calculate stride of given types", Category="Object", ThreadSafe, _return="Number of bytes", _type="A POD Type. Represented as ST_TYPE", _count1="Number of types", _type2="The second POD Type. Represented as ST_TYPE", _count2="Number of types after type 1")
ST_PUBLIC u32  stCalculateStride2(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2);

ST_PUBLIC_FUNCTION(Named="stCalculateStride3", Text="Calculate stride of given types", Category="Object", ThreadSafe, _return="Number of bytes", _type="A POD Type. Represented as ST_TYPE", _count1="Number of types", _type2="The second POD Type. Represented as ST_TYPE", _count2="Number of types after type 1", _type3="The third POD Type. Represented as ST_TYPE", _count2="Number of types after type 2")
ST_PUBLIC u32  stCalculateStride3(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3);

ST_PUBLIC_FUNCTION(Named="stCalculateStride4", Text="Calculate stride of given types", Category="Object", ThreadSafe)
ST_PUBLIC u32  stCalculateStride4(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3, stEnum32 type4, u32 count4);

ST_PUBLIC_FUNCTION(Named="stCalculateStride5", Text="Calculate stride of given types", Category="Object", ThreadSafe)
ST_PUBLIC u32  stCalculateStride5(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3, stEnum32 type4, u32 count4, stEnum32 type5, u32 count5);

ST_PUBLIC_FUNCTION(Named="stCalculateStride6", Text="Calculate stride of given types", Category="Object", ThreadSafe)
ST_PUBLIC u32  stCalculateStride6(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3, stEnum32 type4, u32 count4, stEnum32 type5, u32 count5, stEnum32 type6, u32 count6);

ST_PUBLIC_FUNCTION(Named="stCalculateStride7", Text="Calculate stride of given types", Category="Object", ThreadSafe)
ST_PUBLIC u32  stCalculateStride7(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3, stEnum32 type4, u32 count4, stEnum32 type5, u32 count5, stEnum32 type6, u32 count6, stEnum32 type7, u32 count7);

ST_PUBLIC_FUNCTION(Named="stCalculateStride8", Text="Calculate stride of given types", Category="Object", ThreadSafe)
ST_PUBLIC u32  stCalculateStride8(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3, stEnum32 type4, u32 count4, stEnum32 type5, u32 count5, stEnum32 type6, u32 count6, stEnum32 type7, u32 count7, stEnum32 type8, u32 count8);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_uint8", Text="Set Array-Component Value", Category="Object", ThreadSafe)
ST_PUBLIC void stSetValue_p_uint8(u32 objectHandle, u32 component, u8* value, u32 arraySize, u32 stride);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_uint16", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint16")
ST_PUBLIC void stSetValue_p_uint16(u32 objectHandle, u32 component, u16* value, u32 arraySize, u32 stride);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_uint32", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint32")
ST_PUBLIC void stSetValue_p_uint32(u32 objectHandle, u32 component, u32* value, u32 arraySize, u32 stride);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_uint64", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint64")
ST_PUBLIC void stSetValue_p_uint64(u32 objectHandle, u32 component, u64* value, u32 arraySize, u32 stride);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_int8", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int8")
ST_PUBLIC void stSetValue_p_int8(u32 objectHandle, u32 component, s8* value, u32 arraySize, u32 stride);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_int16", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int16")
ST_PUBLIC void stSetValue_p_int16(u32 objectHandle, u32 component, s16* value, u32 arraySize, u32 stride);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_int32", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int32")
ST_PUBLIC void stSetValue_p_int32(u32 objectHandle, u32 component, s32* value, u32 arraySize, u32 stride);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_int64", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int64")
ST_PUBLIC void stSetValue_p_int64(u32 objectHandle, u32 component, s64* value, u32 arraySize, u32 stride);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_float32", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_float32")
ST_PUBLIC void stSetValue_p_float32(u32 objectHandle, u32 component, f32* value, u32 arraySize, u32 stride);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_float64", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_float64")
ST_PUBLIC void stSetValue_p_float64(u32 objectHandle, u32 component, f64* value, u32 arraySize, u32 stride);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_uint8Ex", Text="Set Array-Component Value", Category="Object", ThreadSafe)
ST_PUBLIC void stSetValue_p_uint8Ex(u32 objectHandle, u32 component, u8* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_uint16Ex", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint16")
ST_PUBLIC void stSetValue_p_uint16Ex(u32 objectHandle, u32 component, u16* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_uint32Ex", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint32")
ST_PUBLIC void stSetValue_p_uint32Ex(u32 objectHandle, u32 component, u32* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_uint64Ex", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_uint64")
ST_PUBLIC void stSetValue_p_uint64Ex(u32 objectHandle, u32 component, u64* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_int8Ex", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int8")
ST_PUBLIC void stSetValue_p_int8Ex(u32 objectHandle, u32 component, s8* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_int16Ex", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int16")
ST_PUBLIC void stSetValue_p_int16Ex(u32 objectHandle, u32 component, s16* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_int32Ex", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int32")
ST_PUBLIC void stSetValue_p_int32Ex(u32 objectHandle, u32 component, s32* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_int64Ex", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_int64")
ST_PUBLIC void stSetValue_p_int64Ex(u32 objectHandle, u32 component, s64* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_float32Ex", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_float32")
ST_PUBLIC void stSetValue_p_float32Ex(u32 objectHandle, u32 component, f32* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes);

ST_PUBLIC_FUNCTION(Named="stSetValue_p_float64Ex", Text="Set Array-Component Value", Category="Object", ThreadSafe, OverloadHint="stSetValue_float64")
ST_PUBLIC void stSetValue_p_float64Ex(u32 objectHandle, u32 component, f64* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes);

#endif
