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

#pragma once

#include "videoRequired.h"

extern "C"
{

STVIDEO_PUBLIC_FUNCTION(Named="videoSetFileType", Text="Set File Version to Export As", Category="Settings", ThreadSafe)
STVIDEO_PUBLIC s32 videoSetFileType(stEnum32 fileType);

STVIDEO_PUBLIC_FUNCTION(Named="videoSetImageFlip", Text="Set any Image flip correction", Category="Settings", ThreadSafe)
STVIDEO_PUBLIC s32 videoSetImageFlip(stEnum32 imageFlip);

STVIDEO_PUBLIC_FUNCTION(Named="videoConvertSceneTrackFile", Text="Convert a Scene Track File to a video recording file", Category="Conversion", ThreadSafe)
STVIDEO_PUBLIC s32 videoConvertSceneTrackFile(stCStr src, stCStr dst);

STVIDEO_PUBLIC_FUNCTION(Named="videoStepConvertSceneTrackFileBegin", Text="Convert a Scene Track File to a video recording file", Category="Conversion", ThreadSafe)
STVIDEO_PUBLIC s32 videoStepConvertSceneTrackFileBegin(stCStr src, stCStr dst);

STVIDEO_PUBLIC_FUNCTION(Named="videoStepConvertSceneTrackFileUpdate", Text="Convert a Scene Track File to a video recording file", Category="Conversion", ThreadSafe)
STVIDEO_PUBLIC s32 videoStepConvertSceneTrackFileUpdate();

STVIDEO_PUBLIC_FUNCTION(Named="videoStepConvertProgress", Text="Get Conversion Percentage", Category="Conversion", ThreadSafe)
STVIDEO_PUBLIC f32 videoStepConvertProgress();

}
