// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxPublic.h
 *
 * Public API library header functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "fbxRequired.h"

extern "C" 
{

/**
 * Set the swizzle for a particular axis
 * @see Axis
 * @see Node
 */
STFBX_PUBLIC_FUNCTION(Named="fbxSetAxisSwizzle", Text="Set Swizzle for a particular axis", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetAxisSwizzle(stEnum32 nodeType, stEnum32 trsMask, stEnum32 srcAxis, stEnum32 dstAxis, s32 sign);

/**
 * Set the operation for a particular axis
 * @see Axis
 * @see Node
 * @see Operation
 */
STFBX_PUBLIC_FUNCTION(Named="fbxSetAxisOperation", Text="Set Swizzle for a particular axis", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetAxisOperation(stEnum32 nodeType, stEnum32 trsMask, stEnum32 dstAxis, stEnum32 operatorType, f32 constantValue);

/**
 * Set the euler rotation order
 * @see AxisOrder
 */
STFBX_PUBLIC_FUNCTION(Named="fbxSetAxisRotationOrder", Text="Set the Axis Rotation Order", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetAxisRotationOrder(stEnum32 axisOrder);

/**
 * Set the triangle winding order given as ABC, ACB
 */
STFBX_PUBLIC_FUNCTION(Named="fbxSetReverseTriangleWinding", Text="Set the Winding order (ABC) of Triangles to be reverse (ACB)", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetReverseTriangleWinding(stEnum32 reverseWinding);

/**
 * Set the reference frame to export to
 * This function can effectively "flatten" the scene hierarchy, or retain the parent-child node relationship.
 * @see ReferenceFrame
 */
STFBX_PUBLIC_FUNCTION(Named="fbxSetReferenceFrame", Text="Set the Reference Frame to Export to", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetReferenceFrame(stEnum32 referenceFrame);

/**
 * Set the exporter file version and file type
 * @see FbxFileVersion
 */
STFBX_PUBLIC_FUNCTION(Named="fbxSetFileVersion", Text="Set File Version to Export As", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetFileVersion(stEnum32 sdkFileVersion);

/**
 * Set the export frame rate, or a custom frame rate
 * If a custom frame rate is desired, use FbxFrameRate::Custom, and set customRate, otherwise leave customRate to 0.0
 * @see FbxFrameRate
 */
STFBX_PUBLIC_FUNCTION(Named="fbxSetFrameRate", Text="Set Frame Rate", Category="Settings", ThreadSafe)
STFBX_PUBLIC s32 fbxSetFrameRate(stEnum32 frameRate, f64 customRate);

/**
 * Clear the list of asset search paths
 */
STFBX_PUBLIC_FUNCTION(Named="fbxClearAssetPaths", Text="Clear asset search paths", Category="Settings", ThreadSafe)
STFBX_PUBLIC void fbxClearAssetSearchPaths();

/**
 * Add the given path to the list of asset search paths
 */
STFBX_PUBLIC_FUNCTION(Named="fbxAddAssetSearchPath", Text="Add a path to asset search paths", Category="Settings", ThreadSafe)
STFBX_PUBLIC void fbxAddAssetSearchPath(stCStr path);

/**
 * Start the export process to convert a SceneTrack file into the desired file format, in one go.
 * @see fbxSetFileVersion
 */
STFBX_PUBLIC_FUNCTION(Named="fbxConvertSceneTrackFile", Text="Convert a Scene Track File to FBX", Category="Conversion", ThreadSafe)
STFBX_PUBLIC s32 fbxConvertSceneTrackFile(stCStr src, stCStr dst);

/**
 * Start the export process to convert a SceneTrack file into the desired file format, step by step.
 * This function is an alternate to fbxConvertSceneTrackFile, to provide a externally prompted
 * system. This is useful for visual updates as a progress bar.
 * @see fbxSetFileVersion
 * @see fbxStepConvertSceneTrackFileUpdate
 * @see fbxStepConvertProgress
 */
STFBX_PUBLIC_FUNCTION(Named="fbxStepConvertSceneTrackFileBegin", Text="Convert a Scene Track File to FBX", Category="Conversion", ThreadSafe)
STFBX_PUBLIC s32 fbxStepConvertSceneTrackFileBegin(stCStr src, stCStr dst);

/**
 * Used with fbxStepConvertSceneTrackFileBegin convert part of the SceneTrack file, unless the process
 * has finished, then complete the file and save it.
 * @return 0 if the Exporter has completed the frame, but there is more work to do
           1 if the Exporter has completed the file
          -1 if there is an error
 */
STFBX_PUBLIC_FUNCTION(Named="fbxStepConvertSceneTrackFileUpdate", Text="Convert a Scene Track File to FBX", Category="Conversion", ThreadSafe)
STFBX_PUBLIC s32 fbxStepConvertSceneTrackFileUpdate();

/**
 * Get the progress of the export process as a decimal percentage (0.0f to 1.0f), or -1.0f if there is
 * an error, or the exporter is not exporting.
 */
STFBX_PUBLIC_FUNCTION(Named="fbxStepConvertProgress", Text="Get Conversion Percentage", Category="Conversion", ThreadSafe)
STFBX_PUBLIC f32 fbxStepConvertProgress();


}
