// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	1.0.0
 */

#pragma once

#include "fbxRequired.h"

extern "C" 
{

STFBX_PUBLIC_FUNCTION(Named="fbxSetAxisSwizzle", Text="Set Swizzle for a particular axis", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetAxisSwizzle(stEnum32 nodeType, stEnum32 trsMask, stEnum32 srcAxis, stEnum32 dstAxis, s32 sign);

STFBX_PUBLIC_FUNCTION(Named="fbxSetAxisOperation", Text="Set Swizzle for a particular axis", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetAxisOperation(stEnum32 nodeType, stEnum32 trsMask, stEnum32 dstAxis, stEnum32 operatorType, f32 constantValue);

STFBX_PUBLIC_FUNCTION(Named="fbxSetAxisRotationOrder", Text="Set the Axis Rotation Order", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetAxisRotationOrder(stEnum32 axisOrder);

STFBX_PUBLIC_FUNCTION(Named="fbxSetReverseTriangleWinding", Text="Set the Winding order (ABC) of Triangles to be reverse (ACB)", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetReverseTriangleWinding(stEnum32 reverseWinding);

STFBX_PUBLIC_FUNCTION(Named="fbxSetReferenceFrame", Text="Set the Reference Frame to Export to", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetReferenceFrame(stEnum32 referenceFrame);

STFBX_PUBLIC_FUNCTION(Named="fbxSetFileVersion", Text="Set File Version to Export As", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetFileVersion(stEnum32 sdkFileVersion);

STFBX_PUBLIC_FUNCTION(Named="fbxSetFrameRate", Text="Set Frame Rate", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetFrameRate(stEnum32 frameRate, f64 customRate);

STFBX_PUBLIC_FUNCTION(Named="fbxClearAssetPaths", Text="Clear asset search paths", Category="Settings", ThreadSafe)
STFBX_PUBLIC void fbxClearAssetSearchPaths();

STFBX_PUBLIC_FUNCTION(Named="fbxAddAssetSearchPath", Text="Add a path to asset search paths", Category="Settings", ThreadSafe)
STFBX_PUBLIC void fbxAddAssetSearchPath(stCStr path);

STFBX_PUBLIC_FUNCTION(Named="fbxConvertSceneTrackFile", Text="Convert a Scene Track File to FBX", Category="Conversion", ThreadSafe)
STFBX_PUBLIC s32 fbxConvertSceneTrackFile(stCStr src, stCStr dst);

STFBX_PUBLIC_FUNCTION(Named="fbxStepConvertSceneTrackFileBegin", Text="Convert a Scene Track File to FBX", Category="Conversion", ThreadSafe)
STFBX_PUBLIC s32 fbxStepConvertSceneTrackFileBegin(stCStr src, stCStr dst);

STFBX_PUBLIC_FUNCTION(Named="fbxStepConvertSceneTrackFileUpdate", Text="Convert a Scene Track File to FBX", Category="Conversion", ThreadSafe)
STFBX_PUBLIC s32 fbxStepConvertSceneTrackFileUpdate();

STFBX_PUBLIC_FUNCTION(Named="fbxStepConvertProgress", Text="Get Conversion Percentage", Category="Conversion", ThreadSafe)
STFBX_PUBLIC f32 fbxStepConvertProgress();


}
