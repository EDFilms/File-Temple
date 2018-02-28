// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * videoPublic.h
 *
 * Public API library header functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "videoRequired.h"

extern "C"
{

/**
 * Set the exporter file type
 * @see VideoFileType
 */
STVIDEO_PUBLIC_FUNCTION(Named="videoSetFileType", Text="Set File Version to Export As", Category="Settings", ThreadSafe)
STVIDEO_PUBLIC s32 videoSetFileType(stEnum32 fileType);

/**
 * Set the video flipping parameters
 * @see ImageFlip
 */
STVIDEO_PUBLIC_FUNCTION(Named="videoSetImageFlip", Text="Set any Image flip correction", Category="Settings", ThreadSafe)
STVIDEO_PUBLIC s32 videoSetImageFlip(stEnum32 imageFlip);

/**
 * Start the export process to convert a SceneTrack file into the desired file format, in one go.
 * @see VideoFileType
 */
STVIDEO_PUBLIC_FUNCTION(Named="videoConvertSceneTrackFile", Text="Convert a Scene Track File to a video recording file", Category="Conversion", ThreadSafe)
STVIDEO_PUBLIC s32 videoConvertSceneTrackFile(stCStr src, stCStr dst);

/**
 * Start the export process to convert a SceneTrack file into the desired file format, step by step.
 * This function is an alternate to fbxConvertSceneTrackFile, to provide a externally prompted
 * system. This is useful for visual updates as a progress bar.
 * @see fbxSetFileVersion
 * @see videoStepConvertSceneTrackFileUpdate
 * @see videoStepConvertProgress
 */
STVIDEO_PUBLIC_FUNCTION(Named="videoStepConvertSceneTrackFileBegin", Text="Convert a Scene Track File to a video recording file", Category="Conversion", ThreadSafe)
STVIDEO_PUBLIC s32 videoStepConvertSceneTrackFileBegin(stCStr src, stCStr dst);

/**
 * Used with fbxStepConvertSceneTrackFileBegin convert part of the SceneTrack file, unless the process
 * has finished, then complete the file and save it.
 * @return 0 if the Exporter has completed the frame, but there is more work to do
           1 if the Exporter has completed the file
          -1 if there is an error
 */
STVIDEO_PUBLIC_FUNCTION(Named="videoStepConvertSceneTrackFileUpdate", Text="Convert a Scene Track File to a video recording file", Category="Conversion", ThreadSafe)
STVIDEO_PUBLIC s32 videoStepConvertSceneTrackFileUpdate();

/**
 * Get the progress of the export process as a decimal percentage (0.0f to 1.0f), or -1.0f if there is
 * an error, or the exporter is not exporting.
 */
STVIDEO_PUBLIC_FUNCTION(Named="videoStepConvertProgress", Text="Get Conversion Percentage", Category="Conversion", ThreadSafe)
STVIDEO_PUBLIC f32 videoStepConvertProgress();

}
