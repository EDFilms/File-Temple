#ifndef SCENETRACKFBX_H
#define SCENETRACKFBX_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
  extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
  #define STFBX_PUBLIC  __declspec(dllimport)
#else
  #define STFBX_PUBLIC  extern
#endif

#define STFBX_NODE_TRANSFORM (0)
#define STFBX_NODE_BONE (1)
#define STFBX_NODE_MESH (2)

#define STFBX_NODEPROPERTY_TRANSLATION (1)
#define STFBX_NODEPROPERTY_VERTEX (1)
#define STFBX_NODEPROPERTY_ROTATION (2)
#define STFBX_NODEPROPERTY_NORMAL (2)
#define STFBX_NODEPROPERTY_SCALE (4)

#define STFBX_AXIS_X (0)
#define STFBX_AXIS_Y (1)
#define STFBX_AXIS_Z (2)
#define STFBX_AXIS_W (3)

#define STFBX_AXISORDER_XYZ (0)
#define STFBX_AXISORDER_XZY (1)
#define STFBX_AXISORDER_YXZ (2)
#define STFBX_AXISORDER_YZX (3)
#define STFBX_AXISORDER_ZXY (4)
#define STFBX_AXISORDER_ZYX (5)

#define STFBX_OPERATOR_SETZERO (0)
#define STFBX_OPERATOR_SETONE (1)
#define STFBX_OPERATOR_VALUE (2)
#define STFBX_OPERATOR_CONSTANT (3)
#define STFBX_OPERATOR_ADD (4)
#define STFBX_OPERATOR_SUBTRACT (5)
#define STFBX_OPERATOR_MULTIPLY (6)
#define STFBX_OPERATOR_DIVIDE (7)
#define STFBX_OPERATOR_NEGATE (8)

#define STFBX_FBXFILEVERSION_FBX_53_MB55 (0)
#define STFBX_FBXFILEVERSION_FBX_60_MB60 (1)
#define STFBX_FBXFILEVERSION_FBX_200508_MB70 (2)
#define STFBX_FBXFILEVERSION_FBX_200602_MB75 (3)
#define STFBX_FBXFILEVERSION_FBX_200608 (4)
#define STFBX_FBXFILEVERSION_FBX_200611 (5)
#define STFBX_FBXFILEVERSION_FBX_200900 (6)
#define STFBX_FBXFILEVERSION_FBX_200900V7 (7)
#define STFBX_FBXFILEVERSION_FBX_201000 (8)
#define STFBX_FBXFILEVERSION_FBX_201100 (9)
#define STFBX_FBXFILEVERSION_FBX_201200 (10)
#define STFBX_FBXFILEVERSION_FBX_201300 (11)
#define STFBX_FBXFILEVERSION_FBX_201400 (12)
#define STFBX_FBXFILEVERSION_FBX_201600 (13)

#define STFBX_FPS_DEFAULT (0)
#define STFBX_FPS_CUSTOM (1)
#define STFBX_FPS_FPS_23_976 (2)
#define STFBX_FPS_FPS_24 (3)
#define STFBX_FPS_FPS_30 (4)
#define STFBX_FPS_FPS_30_DROP (5)
#define STFBX_FPS_FPS_48 (6)
#define STFBX_FPS_FPS_50 (7)
#define STFBX_FPS_FPS_59_94 (8)
#define STFBX_FPS_FPS_60 (9)
#define STFBX_FPS_FPS_72 (10)
#define STFBX_FPS_FPS_96 (11)
#define STFBX_FPS_FPS_100 (12)
#define STFBX_FPS_FPS_120 (13)
#define STFBX_FPS_FPS_1000 (14)
#define STFBX_FPS_FPS_PAL (15)
#define STFBX_FPS_FPS_NTSC (16)
#define STFBX_FPS_FPS_NTSC_DROP (17)

#define STFBX_REFERENCEFRAME_LOCAL (0)
#define STFBX_REFERENCEFRAME_WORLD (1)


/*
 * Function: fbxSetAxisSwizzle
 * ---------------------------
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
STFBX_PUBLIC int32_t fbxSetAxisSwizzle(int nodeType, int trsMask, int srcAxis, int dstAxis, int32_t sign);

/*
 * Function: fbxSetAxisOperation
 * -----------------------------
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
STFBX_PUBLIC int32_t fbxSetAxisOperation(int nodeType, int trsMask, int dstAxis, int operatorType, float constantValue);

/*
 * Function: fbxSetAxisRotationOrder
 * ---------------------------------
 *  
 *  Set the Axis Rotation Order
 *  
 *  Parameters:
 *   1. axisOrder
 */
STFBX_PUBLIC int32_t fbxSetAxisRotationOrder(int axisOrder);

/*
 * Function: fbxSetReverseTriangleWinding
 * --------------------------------------
 *  
 *  Set the Winding order (ABC) of Triangles to be reverse (ACB)
 *  
 *  Parameters:
 *   1. reverseWinding
 */
STFBX_PUBLIC int32_t fbxSetReverseTriangleWinding(int reverseWinding);

/*
 * Function: fbxSetReferenceFrame
 * ------------------------------
 *  
 *  Set the Reference Frame to Export to
 *  
 *  Parameters:
 *   1. referenceFrame
 */
STFBX_PUBLIC int32_t fbxSetReferenceFrame(int referenceFrame);

/*
 * Function: fbxSetFileVersion
 * ---------------------------
 *  
 *  Set File Version to Export As
 *  
 *  Parameters:
 *   1. sdkFileVersion
 */
STFBX_PUBLIC int32_t fbxSetFileVersion(int sdkFileVersion);

/*
 * Function: fbxSetFrameRate
 * -------------------------
 *  
 *  Set Frame Rate
 *  
 *  Parameters:
 *   1. frameRate
 *   2. customRate
 */
STFBX_PUBLIC int32_t fbxSetFrameRate(int frameRate, double customRate);

/*
 * Function: fbxClearAssetSearchPaths
 * ----------------------------------
 *  
 *  Clear asset search paths
 *  
 */
STFBX_PUBLIC void fbxClearAssetSearchPaths();

/*
 * Function: fbxAddAssetSearchPath
 * -------------------------------
 *  
 *  Add a path to asset search paths
 *  
 *  Parameters:
 *   1. path
 */
STFBX_PUBLIC void fbxAddAssetSearchPath(const char* path);

/*
 * Function: fbxConvertSceneTrackFile
 * ----------------------------------
 *  
 *  Convert a Scene Track File to FBX
 *  
 *  Parameters:
 *   1. src
 *   2. dst
 */
STFBX_PUBLIC int32_t fbxConvertSceneTrackFile(const char* src, const char* dst);

/*
 * Function: fbxStepConvertSceneTrackFileBegin
 * -------------------------------------------
 *  
 *  Convert a Scene Track File to FBX
 *  
 *  Parameters:
 *   1. src
 *   2. dst
 */
STFBX_PUBLIC int32_t fbxStepConvertSceneTrackFileBegin(const char* src, const char* dst);

/*
 * Function: fbxStepConvertSceneTrackFileUpdate
 * --------------------------------------------
 *  
 *  Convert a Scene Track File to FBX
 *  
 */
STFBX_PUBLIC int32_t fbxStepConvertSceneTrackFileUpdate();

/*
 * Function: fbxStepConvertProgress
 * --------------------------------
 *  
 *  Get Conversion Percentage
 *  
 */
STFBX_PUBLIC float fbxStepConvertProgress();

/*
 * Function: fbxGetVersionStrCapacity
 * ----------------------------------
 *  
 *  Get Library Version String Capacity
 *  
 */
STFBX_PUBLIC int32_t fbxGetVersionStrCapacity();

/*
 * Function: fbxGetVersion
 * -----------------------
 *  
 *  Get Library Version
 *  
 *  Parameters:
 *   1. outStr
 *   2. strCapacity
 */
STFBX_PUBLIC int32_t fbxGetVersion(char* outStr, int32_t strCapacity);
#ifdef __cplusplus
}
#endif

#endif
