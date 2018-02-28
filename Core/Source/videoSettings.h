// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * videoSettings.h
 *
 * A collection of user settings and enumerations
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "videoRequired.h"
#include "expMath.h"
#include "expTrs.h"

namespace SceneTrackVideo
{

  enum class VideoFileType
  {
    STVIDEO_PUBLIC_ENUM(Named="FILETYPE_JPEG_SEQUENCE",  Value=0, For="FileType")
    JpegSequence = 0,
    STVIDEO_PUBLIC_ENUM(Named="FILETYPE_PNG_SEQUENCE",   Value=1, For="FileType")
    PngSequence  = 1,
    STVIDEO_PUBLIC_ENUM(Named="FILETYPE_MJPEG_SEQUENCE", Value=2, For="FileType")
    MjpegMovie   = 2,
    STVIDEO_PUBLIC_ENUM(Named="FILETYPE_MPEG1_SEQUENCE", Value=3, For="FileType")
    Mpeg1Movie   = 3,
    STVIDEO_PUBLIC_ENUM(Named="FILETYPE_RAW_SEQUENCE",   Value=4, For="FileType")
    RawSequence  = 4,
    COUNT
  };

  enum class ImageFlip
  {
    STVIDEO_PUBLIC_ENUM(Named="None",         Value=0, For="Flip")
    FlipNone = 0,
    STVIDEO_PUBLIC_ENUM(Named="Vertical",     Value=1, For="Flip")
    FlipVert = 1,
    STVIDEO_PUBLIC_ENUM(Named="Horizontal",   Value=2, For="Flip")
    FlipHorz = 2,
    STVIDEO_PUBLIC_ENUM(Named="Both",         Value=3, For="Flip")
    FlipBoth = 3,
    COUNT
  };

  namespace Settings
  {
    extern VideoFileType            sVideoFileType;
    extern ImageFlip                sImageFlip;

    void PrintSettings();
  }
}
