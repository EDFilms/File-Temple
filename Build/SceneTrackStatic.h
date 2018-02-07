#ifndef SCENETRACK_H
#define SCENETRACK_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
  extern "C" {
#endif

  #define ST_PUBLIC
#define ST_ERROR_MEMORY (1)
#define ST_ERROR_DISK (2)
#define ST_ERROR_RECORDING (4)
#define ST_ERROR_PLAYING (8)
#define ST_ERROR_WRITING (16)
#define ST_ERROR_READING (32)
#define ST_ERROR_TYPE (64)
#define ST_ERROR_RESERVE (128)
#define ST_ERROR_ITERATOR (256)

#define ST_ITERATORTYPE_NONE (0)
#define ST_ITERATORTYPE_END (1)
#define ST_ITERATORTYPE_FRAME (2)
#define ST_ITERATORTYPE_OBJECT (3)
#define ST_ITERATORTYPE_NEWOBJECT (4)
#define ST_ITERATORTYPE_DELETEOBJECT (5)
#define ST_ITERATORTYPE_VALUE (6)

#define ST_ITERATORCHANGEFLAGS_FRAME (1)
#define ST_ITERATORCHANGEFLAGS_OBJECT (2)
#define ST_ITERATORCHANGEFLAGS_VALUE (4)

#define ST_KIND_NONE (0)
#define ST_KIND_LENGTH (100)
#define ST_KIND_MINIMUM (101)
#define ST_KIND_MAXIMUM (102)
#define ST_KIND_INTENSITY (103)
#define ST_KIND_NAMED (104)
#define ST_KIND_TYPE (105)
#define ST_KIND_REPEAT (106)
#define ST_KIND_RELATIONSHIP (201)
#define ST_KIND_SPATIAL (202)
#define ST_KIND_GEOMETRY (203)
#define ST_KIND_AUDIO (204)
#define ST_KIND_VIDEO (205)
#define ST_KIND_EVENT (206)
#define ST_KIND_SURFACE (207)
#define ST_KIND_LAYER (300)
#define ST_KIND_TAG (301)
#define ST_KIND_PARENT (302)
#define ST_KIND_CHILD (304)
#define ST_KIND_NEXT (305)
#define ST_KIND_PREVIOUS (306)
#define ST_KIND_SIZE (400)
#define ST_KIND_WIDTH (401)
#define ST_KIND_HEIGHT (402)
#define ST_KIND_DEPTH (403)
#define ST_KIND_X (404)
#define ST_KIND_Y (405)
#define ST_KIND_Z (406)
#define ST_KIND_YAW (407)
#define ST_KIND_PITCH (408)
#define ST_KIND_ROLL (409)
#define ST_KIND_POSITION (410)
#define ST_KIND_ROTATION (411)
#define ST_KIND_SCALE (412)
#define ST_KIND_TRANSFORM (413)
#define ST_KIND_LINEAR_VELOCITY (414)
#define ST_KIND_LINEAR_ACCELERATION (415)
#define ST_KIND_ANGULAR_VELOCITY (416)
#define ST_KIND_ANGULAR_ACCELERATION (417)
#define ST_KIND_OFFSET (418)
#define ST_KIND_VERTEX (500)
#define ST_KIND_INDEX (501)
#define ST_KIND_NORMAL (502)
#define ST_KIND_TANGENT (503)
#define ST_KIND_COLOR (504)
#define ST_KIND_UV_0 (505)
#define ST_KIND_UV_1 (506)
#define ST_KIND_UV_2 (507)
#define ST_KIND_UV_3 (508)
#define ST_KIND_UV_4 (509)
#define ST_KIND_UV_5 (510)
#define ST_KIND_UV_6 (511)
#define ST_KIND_UV_7 (512)
#define ST_KIND_BONE_BEGIN (513)
#define ST_KIND_BONE_END (514)
#define ST_KIND_BONE_LENGTH (515)
#define ST_KIND_BONE_WEIGHT (516)
#define ST_KIND_BONE_INDEX (517)
#define ST_KIND_BONE (518)
#define ST_KIND_POSE (519)
#define ST_KIND_PCM (600)
#define ST_KIND_IMAGE (700)
#define ST_KIND_R (701)
#define ST_KIND_G (702)
#define ST_KIND_B (703)
#define ST_KIND_A (704)
#define ST_KIND_AMBIENT (800)
#define ST_KIND_SPECULAR (801)
#define ST_KIND_EMISSIVE (802)
#define ST_KIND_REFRACTION (803)
#define ST_KIND_ROUGHNESS (804)
#define ST_KIND_REFLECTION (806)
#define ST_KIND_TRANSPARENCY (807)

#define ST_FREQUENCY_STATIC (0)
#define ST_FREQUENCY_DYNAMIC (1)
#define ST_FREQUENCY_STREAM (2)
#define ST_FREQUENCY_EVENT (3)

#define ST_MEMORY_FRAMEPOOL (0)
#define ST_MEMORY_FRAMEPAGE (0)

#define ST_REFERENCE_UNSPECIFIED (0)
#define ST_REFERENCE_WORLD (1)
#define ST_REFERENCE_LOCAL (2)
#define ST_REFERENCE_INTERNALASSET (3)
#define ST_REFERENCE_EXTERNALASSET (4)

#define ST_TYPE_NONE (0)
#define ST_TYPE_UINT8 (1)
#define ST_TYPE_UINT16 (2)
#define ST_TYPE_UINT32 (3)
#define ST_TYPE_UINT64 (4)
#define ST_TYPE_INT8 (5)
#define ST_TYPE_INT16 (6)
#define ST_TYPE_INT32 (7)
#define ST_TYPE_INT64 (8)
#define ST_TYPE_FLOAT32 (9)
#define ST_TYPE_FLOAT64 (10)
#define ST_TYPE_STRING (11)
#define ST_TYPE_OBJECT (12)
#define ST_TYPE_OBJECTTYPE (13)
#define ST_TYPE_CSTRING (21)
#define ST_TYPE_ENUM (22)
#define ST_TYPE_MARKER (255)

#define ST_UNITS_UNSPECIFIED (0)
#define ST_UNITS_RADIAN (1)
#define ST_UNITS_DEGREE (2)
#define ST_UNITS_ENUMERATION (3)

#define ST_FORMAT_BINARY (0)


/*
 * Function: stSubmit
 * ------------------
 *  
 *  Submit changes to frame, and maybe write frame to the file.
 *  
 *  Parameters:
 *   1. frameLength: How long is the frame in seconds
 */
ST_PUBLIC void stSubmit(float frameLength);

/*
 * Function: stGetNumRecordedFrames
 * --------------------------------
 *  
 *  Get Number of Frames
 *  
 *  Return: 
 *   Number of frames recorded
 */
ST_PUBLIC uint32_t stGetNumRecordedFrames();

/*
 * Function: stGetNumFrames
 * ------------------------
 *  
 *  Get Number of Frames
 *  
 *  Return: 
 *   Number of total frames, including recorded and non-recorded
 */
ST_PUBLIC uint32_t stGetNumFrames();

/*
 * Function: stGetRecordingTime
 * ----------------------------
 *  
 *  Get Length of recording in Seconds
 *  
 *  Return: 
 *   Get the length of the recording in seconds
 */
ST_PUBLIC double stGetRecordingTime();

/*
 * Function: stGetFirstFrame
 * -------------------------
 *  
 *  Get First Frame Number
 *  
 *  Return: 
 *   The frame number of the first frame
 */
ST_PUBLIC uint32_t stGetFirstFrame();

/*
 * Function: stGetLastFrame
 * ------------------------
 *  
 *  Get Last Frame Number
 *  
 *  Return: 
 *   The frame number of the last frame
 */
ST_PUBLIC uint32_t stGetLastFrame();

/*
 * Function: stReorderFrames
 * -------------------------
 *  
 *  Reorder frames in the given order
 *  
 */
ST_PUBLIC void stReorderFrames();

/*
 * Function: stGetError
 * --------------------
 *  
 *  Get an error or return 0.
 *  
 */
ST_PUBLIC uint32_t stGetError();

/*
 * Function: stDisposeIterator
 * ---------------------------
 *  
 *  Dispose of an iterator
 *  
 *  Parameters:
 *   1. iteratorId
 */
ST_PUBLIC uint32_t stDisposeIterator(uint32_t iteratorId);

/*
 * Function: stCreateForwardIterator
 * ---------------------------------
 *  
 *  Create a forward access frame, object and value iterator, and start at the first frame
 *  
 */
ST_PUBLIC uint32_t stCreateForwardIterator();

/*
 * Function: stIteratorNext
 * ------------------------
 *  
 *  Go to the next available value
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC int32_t stIteratorNext(uint32_t iterator);

/*
 * Function: stIteratorEnded
 * -------------------------
 *  
 *  Has Iterator reached the end
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC int32_t stIteratorEnded(uint32_t iterator);

/*
 * Function: stIteratorGetType
 * ---------------------------
 *  
 *  Get Frame
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC int32_t stIteratorGetType(uint32_t iterator);

/*
 * Function: stIteratorGetChangeFlags
 * ----------------------------------
 *  
 *  Get Change Flags
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC int32_t stIteratorGetChangeFlags(uint32_t iterator);

/*
 * Function: stIteratorGetFrame
 * ----------------------------
 *  
 *  Get Frame
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC int32_t stIteratorGetFrame(uint32_t iterator);

/*
 * Function: stIteratorGetFrameTime
 * --------------------------------
 *  
 *  Get Start Time in Seconds
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC double stIteratorGetFrameTime(uint32_t iterator);

/*
 * Function: stIteratorGetFrameLength
 * ----------------------------------
 *  
 *  Get Frame Length in Seconds
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC double stIteratorGetFrameLength(uint32_t iterator);

/*
 * Function: stIteratorGetObjectHandle
 * -----------------------------------
 *  
 *  Get Object Handle
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC uint32_t stIteratorGetObjectHandle(uint32_t iterator);

/*
 * Function: stIteratorGetObjectType
 * ---------------------------------
 *  
 *  Get Component Type
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC uint32_t stIteratorGetObjectType(uint32_t iterator);

/*
 * Function: stIteratorGetIsEvent
 * ------------------------------
 *  
 *  Get Is the Object an Event
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC uint32_t stIteratorGetIsEvent(uint32_t iterator);

/*
 * Function: stIteratorGetComponentHandle
 * --------------------------------------
 *  
 *  Get Component
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC uint32_t stIteratorGetComponentHandle(uint32_t iterator);

/*
 * Function: stIteratorGetValueType
 * --------------------------------
 *  
 *  Get Component Type
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC uint32_t stIteratorGetValueType(uint32_t iterator);

/*
 * Function: stIteratorGetValueArraySize
 * -------------------------------------
 *  
 *  Get Component Array Size
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC uint32_t stIteratorGetValueArraySize(uint32_t iterator);

/*
 * Function: stIteratorGetValueNbElements
 * --------------------------------------
 *  
 *  Get Component Number of Elements
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC uint32_t stIteratorGetValueNbElements(uint32_t iterator);

/*
 * Function: stIteratorGetValue_uint8
 * ----------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. element
 */
ST_PUBLIC uint8_t stIteratorGetValue_uint8(uint32_t iterator, uint32_t element);

/*
 * Function: stIteratorGetValue_uint16
 * -----------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. element
 */
ST_PUBLIC uint16_t stIteratorGetValue_uint16(uint32_t iterator, uint32_t element);

/*
 * Function: stIteratorGetValue_uint32
 * -----------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. element
 */
ST_PUBLIC uint32_t stIteratorGetValue_uint32(uint32_t iterator, uint32_t element);

/*
 * Function: stIteratorGetValue_uint64
 * -----------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. element
 */
ST_PUBLIC uint64_t stIteratorGetValue_uint64(uint32_t iterator, uint32_t element);

/*
 * Function: stIteratorGetValue_int8
 * ---------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. element
 */
ST_PUBLIC int8_t stIteratorGetValue_int8(uint32_t iterator, uint32_t element);

/*
 * Function: stIteratorGetValue_int16
 * ----------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. element
 */
ST_PUBLIC int16_t stIteratorGetValue_int16(uint32_t iterator, uint32_t element);

/*
 * Function: stIteratorGetValue_int32
 * ----------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. element
 */
ST_PUBLIC int32_t stIteratorGetValue_int32(uint32_t iterator, uint32_t element);

/*
 * Function: stIteratorGetValue_int64
 * ----------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. element
 */
ST_PUBLIC int64_t stIteratorGetValue_int64(uint32_t iterator, uint32_t element);

/*
 * Function: stIteratorGetValue_float32
 * ------------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. element
 */
ST_PUBLIC float stIteratorGetValue_float32(uint32_t iterator, uint32_t element);

/*
 * Function: stIteratorGetValue_float64
 * ------------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. element
 */
ST_PUBLIC double stIteratorGetValue_float64(uint32_t iterator, uint32_t element);

/*
 * Function: stIteratorGetValue_stringLength
 * -----------------------------------------
 *  
 *  Get Component String Length
 *  
 *  Parameters:
 *   1. iterator
 */
ST_PUBLIC int32_t stIteratorGetValue_stringLength(uint32_t iterator);

/*
 * Function: stIteratorGetValue_string
 * -----------------------------------
 *  
 *  Put the contents of the string into the given char array of given capacity.
 *  
 *  Parameters:
 *   1. iterator
 *   2. String (Output)
 *   3. strCapacity
 */
ST_PUBLIC int32_t stIteratorGetValue_string(uint32_t iterator, char* out_String, uint32_t strCapacity);

/*
 * Function: stIteratorGetValueArray
 * ---------------------------------
 *  
 *  Get Component Value
 *  
 *  Parameters:
 *   1. iterator
 *   2. targetArray (Output)
 *   3. targetArraySizeInBytes
 *   4. stride
 */
ST_PUBLIC uint32_t stIteratorGetValueArray(uint32_t iterator, void* out_targetArray, uint32_t targetArraySizeInBytes, uint32_t stride);

/*
 * Function: stCreateKind
 * ----------------------
 *  
 *  Create User Kind
 *  
 *  Parameters:
 *   1. name
 */
ST_PUBLIC uint32_t stCreateKind(const char* name);

/*
 * Function: stCreateObjectType
 * ----------------------------
 *  
 *  Create Type of Object
 *  
 *  Parameters:
 *   1. objectFrequency: The usage of the Objects of this type when recording
 *  Return: 
 *   The Object type handle
 */
ST_PUBLIC uint32_t stCreateObjectType(int objectFrequency);

/*
 * Function: stCreateObjectTypeEx
 * ------------------------------
 *  
 *  Create Type of Object
 *  
 *  Parameters:
 *   1. objectFrequency: The usage of the Objects of this type when recording
 *   2. userData: An user data integer to associate the Type with
 *  Return: 
 *   The Object type handle
 */
ST_PUBLIC uint32_t stCreateObjectTypeEx(int objectFrequency, uint32_t userData);

/*
 * Function: stFindObjectType
 * --------------------------
 *  
 *  Find the first type of an Object associated with an user data integer
 *  
 *  Parameters:
 *   1. userData: The user data integer to look for
 *  Return: 
 *   The Object type Handle, or 0 if it was not found
 */
ST_PUBLIC uint32_t stFindObjectType(uint32_t userData);

/*
 * Function: stAddObjectTypeComponent
 * ----------------------------------
 *  
 *  Add component to object type
 *  
 *  Parameters:
 *   1. objectType: Type of Object
 *   2. kind: Kind of component
 *   3. type: Type of Component (ST_TYPE)
 *   4. nbElements: Number of elements per component. Must be >= 1
 *  Return: 
 *   The component handle
 */
ST_PUBLIC uint32_t stAddObjectTypeComponent(uint32_t objectType, int kind, int type, uint32_t nbElements);

/*
 * Function: stAddObjectTypeComponentEx
 * ------------------------------------
 *  
 *  Add component to object type (extended)
 *  
 *  Parameters:
 *   1. objectType: Type of Object
 *   2. kind: Kind of component
 *   3. type: Type of Component (ST_TYPE)
 *   4. nbElements: Number of elements per component. Must be >= 1
 *   5. arrayCapacity: Maximum number of items in the array. Must be >= 1
 *  Return: 
 *   The component handle
 */
ST_PUBLIC uint32_t stAddObjectTypeComponentEx(uint32_t objectType, int kind, int type, uint32_t nbElements, uint32_t arrayCapacity);

/*
 * Function: stAddObjectTypeComponentEx2
 * -------------------------------------
 *  
 *  Add component to object type (extended full)
 *  
 *  Parameters:
 *   1. objectType: Type of Object
 *   2. kind: Kind of component
 *   3. type: Type of Component (ST_TYPE)
 *   4. nbElements: Number of elements per component. Must be >= 1
 *   5. arrayCapacity: Maximum number of items in the array. Must be >= 1
 *   6. units: Unit associated with the values of the component
 *   7. reference: What the values are in reference to
 *  Return: 
 *   The component handle
 */
ST_PUBLIC uint32_t stAddObjectTypeComponentEx2(uint32_t objectType, int kind, int type, uint32_t nbElements, uint32_t arrayCapacity, int units, int reference);

/*
 * Function: stFindObjectTypeComponent
 * -----------------------------------
 *  
 *  Find the first component of an Object according to its Kind
 *  
 *  Parameters:
 *   1. objectType: The handle to the type of the object
 *   2. kind: The kind of component to look for
 *  Return: 
 *   The component handle or 0 if it could not be found
 */
ST_PUBLIC uint32_t stFindObjectTypeComponent(uint32_t objectType, uint32_t kind);

/*
 * Function: stCreateObject
 * ------------------------
 *  
 *  Create Object
 *  
 *  Parameters:
 *   1. objectType: Handle to the Object Type
 *  Return: 
 *   The created Object Handle, or 0 if the Object was not created.
 */
ST_PUBLIC uint32_t stCreateObject(uint32_t objectType);

/*
 * Function: stDestroyObject
 * -------------------------
 *  
 *  Destroy Object
 *  
 *  Parameters:
 *   1. objectHandle: Handle of the Object to delete
 */
ST_PUBLIC void stDestroyObject(uint32_t objectHandle);

/*
 * Function: stGetObjectType
 * -------------------------
 *  
 *  Get Object Type of Object
 *  
 *  Parameters:
 *   1. objectHandle: Handle of the Object
 *  Return: 
 *   The Object Type, or 0 if the Type of Object cannot be found
 */
ST_PUBLIC uint32_t stGetObjectType(uint32_t objectHandle);

/*
 * Function: stSetValue_uint8
 * --------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The Value
 */
ST_PUBLIC void stSetValue_uint8(uint32_t objectHandle, uint32_t component, uint8_t x);

/*
 * Function: stSetValue_uint16
 * ---------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The Value
 */
ST_PUBLIC void stSetValue_uint16(uint32_t objectHandle, uint32_t component, uint16_t x);

/*
 * Function: stSetValue_uint32
 * ---------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The Value
 */
ST_PUBLIC void stSetValue_uint32(uint32_t objectHandle, uint32_t component, uint32_t x);

/*
 * Function: stSetValue_uint64
 * ---------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The Value
 */
ST_PUBLIC void stSetValue_uint64(uint32_t objectHandle, uint32_t component, uint64_t x);

/*
 * Function: stSetValue_int8
 * -------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The Value
 */
ST_PUBLIC void stSetValue_int8(uint32_t objectHandle, uint32_t component, int8_t x);

/*
 * Function: stSetValue_int16
 * --------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The Value
 */
ST_PUBLIC void stSetValue_int16(uint32_t objectHandle, uint32_t component, int16_t x);

/*
 * Function: stSetValue_int32
 * --------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The Value
 */
ST_PUBLIC void stSetValue_int32(uint32_t objectHandle, uint32_t component, int32_t x);

/*
 * Function: stSetValue_int64
 * --------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The Value
 */
ST_PUBLIC void stSetValue_int64(uint32_t objectHandle, uint32_t component, int64_t x);

/*
 * Function: stSetValue_float32
 * ----------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The Value
 */
ST_PUBLIC void stSetValue_float32(uint32_t objectHandle, uint32_t component, float x);

/*
 * Function: stSetValue_float64
 * ----------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The Value
 */
ST_PUBLIC void stSetValue_float64(uint32_t objectHandle, uint32_t component, double x);

/*
 * Function: stSetValue_string
 * ---------------------------
 *  
 *  Set Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. str: A pointer to a C style ASCII string
 *   4. strLength
 */
ST_PUBLIC void stSetValue_string(uint32_t objectHandle, uint32_t component, const char* str, uint32_t strLength);

/*
 * Function: stSetValue_2_uint8
 * ----------------------------
 *  
 *  Set 2-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 */
ST_PUBLIC void stSetValue_2_uint8(uint32_t objectHandle, uint32_t component, uint8_t x, uint8_t y);

/*
 * Function: stSetValue_2_uint16
 * -----------------------------
 *  
 *  Set 2-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 */
ST_PUBLIC void stSetValue_2_uint16(uint32_t objectHandle, uint32_t component, uint16_t x, uint16_t y);

/*
 * Function: stSetValue_2_uint32
 * -----------------------------
 *  
 *  Set 2-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 */
ST_PUBLIC void stSetValue_2_uint32(uint32_t objectHandle, uint32_t component, uint32_t x, uint32_t y);

/*
 * Function: stSetValue_2_uint64
 * -----------------------------
 *  
 *  Set 2-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 */
ST_PUBLIC void stSetValue_2_uint64(uint32_t objectHandle, uint32_t component, uint64_t x, uint64_t y);

/*
 * Function: stSetValue_2_int8
 * ---------------------------
 *  
 *  Set 2-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 */
ST_PUBLIC void stSetValue_2_int8(uint32_t objectHandle, uint32_t component, int8_t x, int8_t y);

/*
 * Function: stSetValue_2_int16
 * ----------------------------
 *  
 *  Set 2-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 */
ST_PUBLIC void stSetValue_2_int16(uint32_t objectHandle, uint32_t component, int16_t x, int16_t y);

/*
 * Function: stSetValue_2_int32
 * ----------------------------
 *  
 *  Set 2-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 */
ST_PUBLIC void stSetValue_2_int32(uint32_t objectHandle, uint32_t component, int32_t x, int32_t y);

/*
 * Function: stSetValue_2_int64
 * ----------------------------
 *  
 *  Set 2-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 */
ST_PUBLIC void stSetValue_2_int64(uint32_t objectHandle, uint32_t component, int64_t x, int64_t y);

/*
 * Function: stSetValue_2_float32
 * ------------------------------
 *  
 *  Set 2-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 */
ST_PUBLIC void stSetValue_2_float32(uint32_t objectHandle, uint32_t component, float x, float y);

/*
 * Function: stSetValue_2_float64
 * ------------------------------
 *  
 *  Set 2-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 */
ST_PUBLIC void stSetValue_2_float64(uint32_t objectHandle, uint32_t component, double x, double y);

/*
 * Function: stSetValue_3_uint8
 * ----------------------------
 *  
 *  Set 3-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 */
ST_PUBLIC void stSetValue_3_uint8(uint32_t objectHandle, uint32_t component, uint8_t x, uint8_t y, uint8_t z);

/*
 * Function: stSetValue_3_uint16
 * -----------------------------
 *  
 *  Set 3-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 */
ST_PUBLIC void stSetValue_3_uint16(uint32_t objectHandle, uint32_t component, uint16_t x, uint16_t y, uint16_t z);

/*
 * Function: stSetValue_3_uint32
 * -----------------------------
 *  
 *  Set 3-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 */
ST_PUBLIC void stSetValue_3_uint32(uint32_t objectHandle, uint32_t component, uint32_t x, uint32_t y, uint32_t z);

/*
 * Function: stSetValue_3_uint64
 * -----------------------------
 *  
 *  Set 3-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 */
ST_PUBLIC void stSetValue_3_uint64(uint32_t objectHandle, uint32_t component, uint64_t x, uint64_t y, uint64_t z);

/*
 * Function: stSetValue_3_int8
 * ---------------------------
 *  
 *  Set 3-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 */
ST_PUBLIC void stSetValue_3_int8(uint32_t objectHandle, uint32_t component, int8_t x, int8_t y, int8_t z);

/*
 * Function: stSetValue_3_int16
 * ----------------------------
 *  
 *  Set 3-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 */
ST_PUBLIC void stSetValue_3_int16(uint32_t objectHandle, uint32_t component, int16_t x, int16_t y, int16_t z);

/*
 * Function: stSetValue_3_int32
 * ----------------------------
 *  
 *  Set 3-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 */
ST_PUBLIC void stSetValue_3_int32(uint32_t objectHandle, uint32_t component, int32_t x, int32_t y, int32_t z);

/*
 * Function: stSetValue_3_int64
 * ----------------------------
 *  
 *  Set 3-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 */
ST_PUBLIC void stSetValue_3_int64(uint32_t objectHandle, uint32_t component, int64_t x, int64_t y, int64_t z);

/*
 * Function: stSetValue_3_float32
 * ------------------------------
 *  
 *  Set 3-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 */
ST_PUBLIC void stSetValue_3_float32(uint32_t objectHandle, uint32_t component, float x, float y, float z);

/*
 * Function: stSetValue_3_float64
 * ------------------------------
 *  
 *  Set 3-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 */
ST_PUBLIC void stSetValue_3_float64(uint32_t objectHandle, uint32_t component, double x, double y, double z);

/*
 * Function: stSetValue_4_uint8
 * ----------------------------
 *  
 *  Set 4-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 *   6. w: The Fourth Value
 */
ST_PUBLIC void stSetValue_4_uint8(uint32_t objectHandle, uint32_t component, uint8_t x, uint8_t y, uint8_t z, uint8_t w);

/*
 * Function: stSetValue_4_uint16
 * -----------------------------
 *  
 *  Set 4-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 *   6. w: The Fourth Value
 */
ST_PUBLIC void stSetValue_4_uint16(uint32_t objectHandle, uint32_t component, uint16_t x, uint16_t y, uint16_t z, uint16_t w);

/*
 * Function: stSetValue_4_uint32
 * -----------------------------
 *  
 *  Set 4-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 *   6. w: The Fourth Value
 */
ST_PUBLIC void stSetValue_4_uint32(uint32_t objectHandle, uint32_t component, uint32_t x, uint32_t y, uint32_t z, uint32_t w);

/*
 * Function: stSetValue_4_uint64
 * -----------------------------
 *  
 *  Set 4-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 *   6. w: The Fourth Value
 */
ST_PUBLIC void stSetValue_4_uint64(uint32_t objectHandle, uint32_t component, uint64_t x, uint64_t y, uint64_t z, uint64_t w);

/*
 * Function: stSetValue_4_int8
 * ---------------------------
 *  
 *  Set 4-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 *   6. w: The Fourth Value
 */
ST_PUBLIC void stSetValue_4_int8(uint32_t objectHandle, uint32_t component, int8_t x, int8_t y, int8_t z, int8_t w);

/*
 * Function: stSetValue_4_int16
 * ----------------------------
 *  
 *  Set 4-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 *   6. w: The Fourth Value
 */
ST_PUBLIC void stSetValue_4_int16(uint32_t objectHandle, uint32_t component, int16_t x, int16_t y, int16_t z, int16_t w);

/*
 * Function: stSetValue_4_int32
 * ----------------------------
 *  
 *  Set 4-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 *   6. w: The Fourth Value
 */
ST_PUBLIC void stSetValue_4_int32(uint32_t objectHandle, uint32_t component, int32_t x, int32_t y, int32_t z, int32_t w);

/*
 * Function: stSetValue_4_int64
 * ----------------------------
 *  
 *  Set 4-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 *   6. w: The Fourth Value
 */
ST_PUBLIC void stSetValue_4_int64(uint32_t objectHandle, uint32_t component, int64_t x, int64_t y, int64_t z, int64_t w);

/*
 * Function: stSetValue_4_float32
 * ------------------------------
 *  
 *  Set 4-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 *   6. w: The Fourth Value
 */
ST_PUBLIC void stSetValue_4_float32(uint32_t objectHandle, uint32_t component, float x, float y, float z, float w);

/*
 * Function: stSetValue_4_float64
 * ------------------------------
 *  
 *  Set 4-Component Value
 *  
 *  Parameters:
 *   1. objectHandle: Handle to the Object
 *   2. component: Component Handle
 *   3. x: The First Value
 *   4. y: The Second Value
 *   5. z: The Third Value
 *   6. w: The Fourth Value
 */
ST_PUBLIC void stSetValue_4_float64(uint32_t objectHandle, uint32_t component, double x, double y, double z, double w);

/*
 * Function: stCalculatePODVectorMemorySize
 * ----------------------------------------
 *  
 *  Calculate the memory size of a simple POD-style 1 or N sized vector.
 *  
 *  Parameters:
 *   1. type: A POD Type. Represented as ST_TYPE
 *   2. nbElements: Number of elements in the vector
 *   3. arraySize: Number of vector items in the array
 *  Return: 
 *   Number of bytes
 */
ST_PUBLIC uint32_t stCalculatePODVectorMemorySize(int type, uint32_t nbElements, uint32_t arraySize);

/*
 * Function: stCalculateStride1
 * ----------------------------
 *  
 *  Calculate stride of given types
 *  
 *  Parameters:
 *   1. type1
 *   2. count1: Number of types
 *  Return: 
 *   Number of bytes
 */
ST_PUBLIC uint32_t stCalculateStride1(int type1, uint32_t count1);

/*
 * Function: stCalculateStride2
 * ----------------------------
 *  
 *  Calculate stride of given types
 *  
 *  Parameters:
 *   1. type1
 *   2. count1: Number of types
 *   3. type2: The second POD Type. Represented as ST_TYPE
 *   4. count2: Number of types after type 1
 *  Return: 
 *   Number of bytes
 */
ST_PUBLIC uint32_t stCalculateStride2(int type1, uint32_t count1, int type2, uint32_t count2);

/*
 * Function: stCalculateStride3
 * ----------------------------
 *  
 *  Calculate stride of given types
 *  
 *  Parameters:
 *   1. type1
 *   2. count1: Number of types
 *   3. type2: The second POD Type. Represented as ST_TYPE
 *   4. count2: Number of types after type 1
 *   5. type3: The third POD Type. Represented as ST_TYPE
 *   6. count3
 *  Return: 
 *   Number of bytes
 */
ST_PUBLIC uint32_t stCalculateStride3(int type1, uint32_t count1, int type2, uint32_t count2, int type3, uint32_t count3);

/*
 * Function: stCalculateStride4
 * ----------------------------
 *  
 *  Calculate stride of given types
 *  
 *  Parameters:
 *   1. type1
 *   2. count1
 *   3. type2
 *   4. count2
 *   5. type3
 *   6. count3
 *   7. type4
 *   8. count4
 */
ST_PUBLIC uint32_t stCalculateStride4(int type1, uint32_t count1, int type2, uint32_t count2, int type3, uint32_t count3, int type4, uint32_t count4);

/*
 * Function: stCalculateStride5
 * ----------------------------
 *  
 *  Calculate stride of given types
 *  
 *  Parameters:
 *   1. type1
 *   2. count1
 *   3. type2
 *   4. count2
 *   5. type3
 *   6. count3
 *   7. type4
 *   8. count4
 *   9. type5
 *   10. count5
 */
ST_PUBLIC uint32_t stCalculateStride5(int type1, uint32_t count1, int type2, uint32_t count2, int type3, uint32_t count3, int type4, uint32_t count4, int type5, uint32_t count5);

/*
 * Function: stCalculateStride6
 * ----------------------------
 *  
 *  Calculate stride of given types
 *  
 *  Parameters:
 *   1. type1
 *   2. count1
 *   3. type2
 *   4. count2
 *   5. type3
 *   6. count3
 *   7. type4
 *   8. count4
 *   9. type5
 *   10. count5
 *   11. type6
 *   12. count6
 */
ST_PUBLIC uint32_t stCalculateStride6(int type1, uint32_t count1, int type2, uint32_t count2, int type3, uint32_t count3, int type4, uint32_t count4, int type5, uint32_t count5, int type6, uint32_t count6);

/*
 * Function: stCalculateStride7
 * ----------------------------
 *  
 *  Calculate stride of given types
 *  
 *  Parameters:
 *   1. type1
 *   2. count1
 *   3. type2
 *   4. count2
 *   5. type3
 *   6. count3
 *   7. type4
 *   8. count4
 *   9. type5
 *   10. count5
 *   11. type6
 *   12. count6
 *   13. type7
 *   14. count7
 */
ST_PUBLIC uint32_t stCalculateStride7(int type1, uint32_t count1, int type2, uint32_t count2, int type3, uint32_t count3, int type4, uint32_t count4, int type5, uint32_t count5, int type6, uint32_t count6, int type7, uint32_t count7);

/*
 * Function: stCalculateStride8
 * ----------------------------
 *  
 *  Calculate stride of given types
 *  
 *  Parameters:
 *   1. type1
 *   2. count1
 *   3. type2
 *   4. count2
 *   5. type3
 *   6. count3
 *   7. type4
 *   8. count4
 *   9. type5
 *   10. count5
 *   11. type6
 *   12. count6
 *   13. type7
 *   14. count7
 *   15. type8
 *   16. count8
 */
ST_PUBLIC uint32_t stCalculateStride8(int type1, uint32_t count1, int type2, uint32_t count2, int type3, uint32_t count3, int type4, uint32_t count4, int type5, uint32_t count5, int type6, uint32_t count6, int type7, uint32_t count7, int type8, uint32_t count8);

/*
 * Function: stSetValue_p_uint8
 * ----------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 */
ST_PUBLIC void stSetValue_p_uint8(uint32_t objectHandle, uint32_t component, uint8_t* value, uint32_t arraySize, uint32_t stride);

/*
 * Function: stSetValue_p_uint16
 * -----------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 */
ST_PUBLIC void stSetValue_p_uint16(uint32_t objectHandle, uint32_t component, uint16_t* value, uint32_t arraySize, uint32_t stride);

/*
 * Function: stSetValue_p_uint32
 * -----------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 */
ST_PUBLIC void stSetValue_p_uint32(uint32_t objectHandle, uint32_t component, uint32_t* value, uint32_t arraySize, uint32_t stride);

/*
 * Function: stSetValue_p_uint64
 * -----------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 */
ST_PUBLIC void stSetValue_p_uint64(uint32_t objectHandle, uint32_t component, uint64_t* value, uint32_t arraySize, uint32_t stride);

/*
 * Function: stSetValue_p_int8
 * ---------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 */
ST_PUBLIC void stSetValue_p_int8(uint32_t objectHandle, uint32_t component, int8_t* value, uint32_t arraySize, uint32_t stride);

/*
 * Function: stSetValue_p_int16
 * ----------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 */
ST_PUBLIC void stSetValue_p_int16(uint32_t objectHandle, uint32_t component, int16_t* value, uint32_t arraySize, uint32_t stride);

/*
 * Function: stSetValue_p_int32
 * ----------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 */
ST_PUBLIC void stSetValue_p_int32(uint32_t objectHandle, uint32_t component, int32_t* value, uint32_t arraySize, uint32_t stride);

/*
 * Function: stSetValue_p_int64
 * ----------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 */
ST_PUBLIC void stSetValue_p_int64(uint32_t objectHandle, uint32_t component, int64_t* value, uint32_t arraySize, uint32_t stride);

/*
 * Function: stSetValue_p_float32
 * ------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 */
ST_PUBLIC void stSetValue_p_float32(uint32_t objectHandle, uint32_t component, float* value, uint32_t arraySize, uint32_t stride);

/*
 * Function: stSetValue_p_float64
 * ------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 */
ST_PUBLIC void stSetValue_p_float64(uint32_t objectHandle, uint32_t component, double* value, uint32_t arraySize, uint32_t stride);

/*
 * Function: stSetValue_p_uint8Ex
 * ------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 *   6. valuePtrOffsetBytes
 */
ST_PUBLIC void stSetValue_p_uint8Ex(uint32_t objectHandle, uint32_t component, uint8_t* value, uint32_t arraySize, uint32_t stride, int64_t valuePtrOffsetBytes);

/*
 * Function: stSetValue_p_uint16Ex
 * -------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 *   6. valuePtrOffsetBytes
 */
ST_PUBLIC void stSetValue_p_uint16Ex(uint32_t objectHandle, uint32_t component, uint16_t* value, uint32_t arraySize, uint32_t stride, int64_t valuePtrOffsetBytes);

/*
 * Function: stSetValue_p_uint32Ex
 * -------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 *   6. valuePtrOffsetBytes
 */
ST_PUBLIC void stSetValue_p_uint32Ex(uint32_t objectHandle, uint32_t component, uint32_t* value, uint32_t arraySize, uint32_t stride, int64_t valuePtrOffsetBytes);

/*
 * Function: stSetValue_p_uint64Ex
 * -------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 *   6. valuePtrOffsetBytes
 */
ST_PUBLIC void stSetValue_p_uint64Ex(uint32_t objectHandle, uint32_t component, uint64_t* value, uint32_t arraySize, uint32_t stride, int64_t valuePtrOffsetBytes);

/*
 * Function: stSetValue_p_int8Ex
 * -----------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 *   6. valuePtrOffsetBytes
 */
ST_PUBLIC void stSetValue_p_int8Ex(uint32_t objectHandle, uint32_t component, int8_t* value, uint32_t arraySize, uint32_t stride, int64_t valuePtrOffsetBytes);

/*
 * Function: stSetValue_p_int16Ex
 * ------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 *   6. valuePtrOffsetBytes
 */
ST_PUBLIC void stSetValue_p_int16Ex(uint32_t objectHandle, uint32_t component, int16_t* value, uint32_t arraySize, uint32_t stride, int64_t valuePtrOffsetBytes);

/*
 * Function: stSetValue_p_int32Ex
 * ------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 *   6. valuePtrOffsetBytes
 */
ST_PUBLIC void stSetValue_p_int32Ex(uint32_t objectHandle, uint32_t component, int32_t* value, uint32_t arraySize, uint32_t stride, int64_t valuePtrOffsetBytes);

/*
 * Function: stSetValue_p_int64Ex
 * ------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 *   6. valuePtrOffsetBytes
 */
ST_PUBLIC void stSetValue_p_int64Ex(uint32_t objectHandle, uint32_t component, int64_t* value, uint32_t arraySize, uint32_t stride, int64_t valuePtrOffsetBytes);

/*
 * Function: stSetValue_p_float32Ex
 * --------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 *   6. valuePtrOffsetBytes
 */
ST_PUBLIC void stSetValue_p_float32Ex(uint32_t objectHandle, uint32_t component, float* value, uint32_t arraySize, uint32_t stride, int64_t valuePtrOffsetBytes);

/*
 * Function: stSetValue_p_float64Ex
 * --------------------------------
 *  
 *  Set Array-Component Value
 *  
 *  Parameters:
 *   1. objectHandle
 *   2. component
 *   3. value
 *   4. arraySize
 *   5. stride
 *   6. valuePtrOffsetBytes
 */
ST_PUBLIC void stSetValue_p_float64Ex(uint32_t objectHandle, uint32_t component, double* value, uint32_t arraySize, uint32_t stride, int64_t valuePtrOffsetBytes);

/*
 * Function: stCreateRecording
 * ---------------------------
 *  
 *  Create Recording for writing
 *  
 */
ST_PUBLIC uint32_t stCreateRecording();

/*
 * Function: stOpenRecording
 * -------------------------
 *  
 *  Open Recording for reading
 *  
 *  Parameters:
 *   1. path
 */
ST_PUBLIC uint32_t stOpenRecording(const char* path);

/*
 * Function: stCloseRecording
 * --------------------------
 *  
 *  Shutdown Recording
 *  
 *  Parameters:
 *   1. recording
 */
ST_PUBLIC void stCloseRecording(uint32_t recording);

/*
 * Function: stChangeActiveRecording
 * ---------------------------------
 *  
 *  Change Recording
 *  
 *  Parameters:
 *   1. recording
 */
ST_PUBLIC void stChangeActiveRecording(uint32_t recording);

/*
 * Function: stAppendSaveRecording
 * -------------------------------
 *  
 *  Save Recording
 *  
 *  Parameters:
 *   1. path
 *   2. format
 *   3. frames
 */
ST_PUBLIC void stAppendSaveRecording(const char* path, int format, uint32_t frames);

/*
 * Function: stSaveRecording
 * -------------------------
 *  
 *  Save Recording
 *  
 *  Parameters:
 *   1. path
 *   2. format
 */
ST_PUBLIC void stSaveRecording(const char* path, int format);

/*
 * Function: stMemoryReserve
 * -------------------------
 *  
 *  Reserve Memory, Pages or Frames to help with performance.
 *  
 *  Parameters:
 *   1. type
 *   2. value
 */
ST_PUBLIC void stMemoryReserve(int32_t type, int32_t value);

/*
 * Function: stGetVersionStrCapacity
 * ---------------------------------
 *  
 *  Get Library Version String Capacity
 *  
 */
ST_PUBLIC int32_t stGetVersionStrCapacity();

/*
 * Function: stGetVersion
 * ----------------------
 *  
 *  Get Library Version
 *  
 *  Parameters:
 *   1. outStr
 *   2. strCapacity
 */
ST_PUBLIC int32_t stGetVersion(char* outStr, int32_t strCapacity);
#ifdef __cplusplus
}
#endif

#endif
