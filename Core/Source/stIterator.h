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

#ifndef ST_ITERATOR_H
#define ST_ITERATOR_H

#include "stPlatform.h"
#include "stObjectHeader.h"
#include "stLazyLoad.h"
#include "stMath.h"

struct stClassT;

struct stIteratorRules
{
  u32 min, max;
};

#define ST_ITERATOR_MAX_ID       0xFFFFFFFE

enum stKeyValueIteratorType
{
  // Index in the linked list. Min/Max is respected.
  ST_VALUE_ITERATOR_TYPE_FORWARD_ITERATOR = 0,
  // Specific type of component. Min/Max is ignored.
  ST_VALUE_ITERATOR_TYPE_COMPONENT_TYPE
};

typedef ST_LAZY_LOAD_T(struct stHeaderFrameT, struct stHeaderFramesT*)   stLazyHeaderFrame;
typedef ST_LAZY_LOAD_T(stObjectHeader, struct stHeaderFrameT*)   stLazyObjectHeader;

struct stLazyDeltaKeyValueData
{
  struct stObjectHeaderT*      objectHeader;

  // When LIST_INDEX then stLazyDeltaKeyValue.id corresponds to offset in linked list
  // When COMPONENT_TYPE then stLazyDeltaKeyValue.id corresponds to the component in the key for the first keyvalue in linked list.
  enum stKeyValueIteratorType  valueType;
};

typedef ST_LAZY_LOAD_T(stDeltaKeyValue, struct stLazyDeltaKeyValueData) stLazyDeltaKeyValue;

struct stIteratorT
{
  struct stIteratorT            *next, *prev;
  u32                            iteratorId;
  u32                            uses;
  // Iterator is deleted
  stBool32                       bDeleted           : 1;
  // Iterate has ended.
  stBool32                       bEnded             : 1;
  // Iterator can iterate through frames
  stBool32                       bIterateFrames     : 1;
  // Iterator can iterate through objects
  stBool32                       bIterateObjects    : 1;
  // Iterator can iterate through values
  stBool32                       bIterateValues     : 1;
  // Iterator moved to a different frame
  stBool32                       bChangedFrame      : 1;
  // Iterator moved to a different object
  stBool32                       bChangedObject     : 1;
  // Iterator moved to a different value
  stBool32                       bChangedValue      : 1;
  // Iterator should unload collect previous frames
  stBool32                       bGcUnloadLastFrame : 1;

  struct stHeaderFramesT*        headerFrames;
  stLazyHeaderFrame              frame;
  struct stIteratorRules         frameRules;
  stLazyObjectHeader             object;
  struct stIteratorRules         objectRules;
  stLazyDeltaKeyValue            value;
  struct stIteratorRules         valueRules;
  enum stKeyValueIteratorType    valueType;
  struct stClassT*               eventClass;
  u32                            lastEventId;
};

typedef struct stIteratorT stIterator;

struct stIteratorsT
{
  struct stIteratorT        *first, *last;
  stDynamicLinearAllocator   mem;
  stQpr                      nextId;
  u32                        pool;
};

typedef struct stIteratorsT stIterators;

void stInitialiseIterators(stIterators* frameIterators);

void stShutdownIterators(stIterators* frameIterators);

ST_PUBLIC_ENUM(Named="None",            Value=0, For="IteratorType")
#define ST_ITERATOR_TYPE_NONE           0
ST_PUBLIC_ENUM(Named="End",             Value=1, For="IteratorType")
#define ST_ITERATOR_TYPE_END            1
ST_PUBLIC_ENUM(Named="Frame",           Value=2, For="IteratorType")
#define ST_ITERATOR_TYPE_FRAME          2
ST_PUBLIC_ENUM(Named="Object",          Value=3, For="IteratorType")
#define ST_ITERATOR_TYPE_OBJECT         3
ST_PUBLIC_ENUM(Named="NewObject",       Value=4, For="IteratorType")
#define ST_ITERATOR_TYPE_NEW_OBJECT     4
ST_PUBLIC_ENUM(Named="DeleteObject",    Value=5, For="IteratorType")
#define ST_ITERATOR_TYPE_DELETE_OBJECT  5
ST_PUBLIC_ENUM(Named="Value",           Value=6, For="IteratorType")
#define ST_ITERATOR_TYPE_VALUE          6

ST_PUBLIC_ENUM(Named="Frame",           Value=1, For="IteratorChangeFlags")
#define ST_ITERATOR_CHANGE_FLAGS_FRAME 1
ST_PUBLIC_ENUM(Named="Object",          Value=2, For="IteratorChangeFlags")
#define ST_ITERATOR_CHANGE_FLAGS_OBJECT 2
ST_PUBLIC_ENUM(Named="Value",           Value=4, For="IteratorChangeFlags")
#define ST_ITERATOR_CHANGE_FLAGS_VALUE  4

ST_PUBLIC_FUNCTION(Named="stDisposeIterator", Text="Dispose of an iterator", Category="Reading")
ST_PUBLIC u32 stDisposeIterator(u32 iteratorId);

ST_PUBLIC_FUNCTION(Named="stCreateForwardIterator", Text="Create a forward access frame, object and value iterator, and start at the first frame", Category="Reading")
ST_PUBLIC u32 stCreateForwardIterator();

u32 stCreateForwardIteratorImpl(u32 frame);

ST_PUBLIC_FUNCTION(Named="stIteratorNext", Text="Go to the next available value", Category="Reading")
ST_PUBLIC s32 stIteratorNext(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorEnded", Text="Has Iterator reached the end", Category="Reading")
ST_PUBLIC s32 stIteratorEnded(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetType", Text="Get Frame", Category="Reading")
ST_PUBLIC s32 stIteratorGetType(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetChangeFlags", Text="Get Change Flags", Category="Reading")
ST_PUBLIC s32 stIteratorGetChangeFlags(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetFrame", Text="Get Frame", Category="Reading")
ST_PUBLIC s32 stIteratorGetFrame(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetFrameTime", Text="Get Start Time in Seconds", Category="Reading")
ST_PUBLIC f64 stIteratorGetFrameTime(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetFrameLength", Text="Get Frame Length in Seconds", Category="Reading")
ST_PUBLIC f64 stIteratorGetFrameLength(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetObjectHandle", Text="Get Object Handle", Category="Reading")
ST_PUBLIC u32 stIteratorGetObjectHandle(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetObjectType", Text="Get Component Type", Category="Reading")
ST_PUBLIC u32 stIteratorGetObjectType(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetIsEvent", Text="Get Is the Object an Event", Category="Reading")
ST_PUBLIC u32 stIteratorGetIsEvent(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetComponentId", Text="Get Component", Category="Reading")
ST_PUBLIC u32 stIteratorGetComponentHandle(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetComponentType", Text="Get Component Type", Category="Reading")
ST_PUBLIC u32 stIteratorGetValueType(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetComponentArraySize", Text="Get Component Array Size", Category="Reading")
ST_PUBLIC u32 stIteratorGetValueArraySize(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetComponentNbElements", Text="Get Component Number of Elements", Category="Reading")
ST_PUBLIC u32 stIteratorGetValueNbElements(u32 iterator);

ST_PUBLIC_STRUCT(Named="stIteratorState", Text="Iterator State")
struct stIteratorStateT
{
  ST_PUBLIC_FIELD(Named="Type", Type="s32")
  s32 type;
  ST_PUBLIC_FIELD(Named="Frame", Type="u32")
  u32 frame;
  ST_PUBLIC_FIELD(Named="FrameTime", Type="f64")
  f64 frameTime;
  ST_PUBLIC_FIELD(Named="FrameLength", Type="f64")
  f64 frameLength;
  ST_PUBLIC_FIELD(Named="ObjectHandle", Type="u32")
  u32 objectHandle;
  ST_PUBLIC_FIELD(Named="ObjectType", Type="u32")
  u32 objectType;
  ST_PUBLIC_FIELD(Named="ComponentHandle", Type="u32")
  u32 componentHandle;
  ST_PUBLIC_FIELD(Named="ValueType", Type="u32")
  u32 valueType;
  ST_PUBLIC_FIELD(Named="NbElements", Type="u32")
  u32 nbElements;
  ST_PUBLIC_FIELD(Named="ArraySize", Type="u32")
  u32 arraySize;
  ST_PUBLIC_FIELD(Named="Stride", Type="u32")
  u32 stride;
};

typedef struct stIteratorStateT stIteratorState;

//ST_PUBLIC_FUNCTION(Named="stIteratorGetState", Text="Get State of the Iterator", Category="Reading")
//ST_PUBLIC 
s32 stIteratorGetState(stIteratorState* state);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_uint8", Text="Get Component Value", Category="Reading")
ST_PUBLIC u8 stIteratorGetValue_uint8(u32 iterator, u32 element);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_uint16", Text="Get Component Value", Category="Reading")
ST_PUBLIC u16 stIteratorGetValue_uint16(u32 iterator, u32 element);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_uint32", Text="Get Component Value", Category="Reading")
ST_PUBLIC u32 stIteratorGetValue_uint32(u32 iterator, u32 element);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_uint64", Text="Get Component Value", Category="Reading")
ST_PUBLIC u64 stIteratorGetValue_uint64(u32 iterator, u32 element);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_uint8", Text="Get Component Value", Category="Reading")
ST_PUBLIC s8 stIteratorGetValue_int8(u32 iterator, u32 element);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_uint16", Text="Get Component Value", Category="Reading")
ST_PUBLIC s16 stIteratorGetValue_int16(u32 iterator, u32 element);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_uint32", Text="Get Component Value", Category="Reading")
ST_PUBLIC s32 stIteratorGetValue_int32(u32 iterator, u32 element);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_uint64", Text="Get Component Value", Category="Reading")
ST_PUBLIC s64 stIteratorGetValue_int64(u32 iterator, u32 element);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_float32", Text="Get Component Value", Category="Reading")
ST_PUBLIC f32 stIteratorGetValue_float32(u32 iterator, u32 element);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_float64", Text="Get Component Value", Category="Reading")
ST_PUBLIC f64 stIteratorGetValue_float64(u32 iterator, u32 element);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_stringCapacity", Text="Get Component String Length", Category="Reading")
ST_PUBLIC s32 stIteratorGetValue_stringLength(u32 iterator);

ST_PUBLIC_FUNCTION(Named="stIteratorGetValue_string", Text="Put the contents of the string into the given char array of given capacity.", Category="Reading")
ST_PUBLIC s32 stIteratorGetValue_string(u32 iterator, stStr out_String, u32 strCapacity);

ST_PUBLIC_FUNCTION(Named="stIteratorGetArrayValue", Text="Get Component Value", Category="Reading")
ST_PUBLIC u32 stIteratorGetValueArray(u32 iterator, void* out_targetArray, u32 targetArraySizeInBytes, u32 stride);

#if ST_CFG_DATA_PROBE == 1
ST_PUBLIC_PROBE int stIteratorPrint(u32 iterator);
#endif

void stIteratorPrint_Impl(stIterator* iterator, const char* comment);

void stIteratorDebug(stIterator* iterator, const char* comment);

#endif
