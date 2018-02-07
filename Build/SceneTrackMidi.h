#ifndef SCENETRACKMIDI_H
#define SCENETRACKMIDI_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
  extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
  #define STMIDI_PUBLIC  __declspec(dllimport)
#else
  #define STMIDI_PUBLIC  extern
#endif

#define STMIDI_NODE_TRANSFORM (0)
#define STMIDI_NODE_BONE (1)
#define STMIDI_NODE_MESH (2)

#define STMIDI_NODEPROPERTY_TRANSLATION (1)
#define STMIDI_NODEPROPERTY_VERTEX (1)
#define STMIDI_NODEPROPERTY_ROTATION (2)
#define STMIDI_NODEPROPERTY_NORMAL (2)
#define STMIDI_NODEPROPERTY_SCALE (4)

#define STMIDI_AXIS_X (0)
#define STMIDI_AXIS_Y (1)
#define STMIDI_AXIS_Z (2)
#define STMIDI_AXIS_W (3)

#define STMIDI_AXISORDER_XYZ (0)
#define STMIDI_AXISORDER_XZY (1)
#define STMIDI_AXISORDER_YXZ (2)
#define STMIDI_AXISORDER_YZX (3)
#define STMIDI_AXISORDER_ZXY (4)
#define STMIDI_AXISORDER_ZYX (5)

#define STMIDI_OPERATOR_SETZERO (0)
#define STMIDI_OPERATOR_SETONE (1)
#define STMIDI_OPERATOR_VALUE (2)
#define STMIDI_OPERATOR_CONSTANT (3)
#define STMIDI_OPERATOR_ADD (4)
#define STMIDI_OPERATOR_SUBTRACT (5)
#define STMIDI_OPERATOR_MULTIPLY (6)
#define STMIDI_OPERATOR_DIVIDE (7)
#define STMIDI_OPERATOR_NEGATE (8)


/*
 * Function: midiSetAxisSwizzle
 * ----------------------------
 *  
 *  Set Swizzle for a particular axis
 *  
 *  Parameters:
 *   1. nodeType
 *   2. trsMask
 *   3. srcAxis
 *   4. dstAxis
 *   5. sign
 */
STMIDI_PUBLIC int32_t midiSetAxisSwizzle(int nodeType, int trsMask, int srcAxis, int dstAxis, int32_t sign);

/*
 * Function: midiSetAxisOperation
 * ------------------------------
 *  
 *  Set Swizzle for a particular axis
 *  
 *  Parameters:
 *   1. nodeType
 *   2. trsMask
 *   3. dstAxis
 *   4. operatorType
 *   5. constantValue
 */
STMIDI_PUBLIC int32_t midiSetAxisOperation(int nodeType, int trsMask, int dstAxis, int operatorType, float constantValue);

/*
 * Function: midiSetAxisRotationOrder
 * ----------------------------------
 *  
 *  Set the Axis Rotation Order
 *  
 *  Parameters:
 *   1. axisOrder
 */
STMIDI_PUBLIC int32_t midiSetAxisRotationOrder(int axisOrder);

/*
 * Function: midiSetFileType
 * -------------------------
 *  
 *  Set File Type to Export As
 *  
 *  Parameters:
 *   1. fileType
 */
STMIDI_PUBLIC int32_t midiSetFileType(int fileType);

/*
 * Function: midiSetTimingMode
 * ---------------------------
 *  
 *  Set Timing Mode
 *  
 *  Parameters:
 *   1. timingMode
 */
STMIDI_PUBLIC int32_t midiSetTimingMode(int timingMode);

/*
 * Function: midiSetTimingValues
 * -----------------------------
 *  
 *  Set Timing Mode Values
 *  
 *  Parameters:
 *   1. first: Ticks per note when TpQN or frames per second when FPS
 *   2. second: ticksPerFrame when FPS
 */
STMIDI_PUBLIC int32_t midiSetTimingValues(int32_t first, int32_t second);

/*
 * Function: midiConvertSceneTrackFile
 * -----------------------------------
 *  
 *  Convert a Scene Track File to a MIDI events file
 *  
 *  Parameters:
 *   1. src
 *   2. dst
 */
STMIDI_PUBLIC int32_t midiConvertSceneTrackFile(const char* src, const char* dst);

/*
 * Function: midiStepConvertSceneTrackFileBegin
 * --------------------------------------------
 *  
 *  Convert a Scene Track File to a MIDI events file
 *  
 *  Parameters:
 *   1. src
 *   2. dst
 */
STMIDI_PUBLIC int32_t midiStepConvertSceneTrackFileBegin(const char* src, const char* dst);

/*
 * Function: midiStepConvertSceneTrackFileUpdate
 * ---------------------------------------------
 *  
 *  Convert a Scene Track File to a MIDI events file
 *  
 */
STMIDI_PUBLIC int32_t midiStepConvertSceneTrackFileUpdate();

/*
 * Function: midiStepConvertProgress
 * ---------------------------------
 *  
 *  Get Conversion Percentage
 *  
 */
STMIDI_PUBLIC float midiStepConvertProgress();

/*
 * Function: midiGetVersionStrCapacity
 * -----------------------------------
 *  
 *  Get Library Version String Capacity
 *  
 */
STMIDI_PUBLIC int32_t midiGetVersionStrCapacity();

/*
 * Function: midiGetVersion
 * ------------------------
 *  
 *  Get Library Version
 *  
 *  Parameters:
 *   1. outStr
 *   2. strCapacity
 */
STMIDI_PUBLIC int32_t midiGetVersion(char* outStr, int32_t strCapacity);
#ifdef __cplusplus
}
#endif

#endif
