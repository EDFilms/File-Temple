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

#include "videoSettings.h"

namespace SceneTrackVideo
{
  namespace Settings
  {

    const char* sVideoFileTypeStr[] = {
      "JPEG Sequence",
      "PNG Sequence",
      "MJPEG Movie",
      "MPEG1 Movie"
    };

    const char* sImageFlipStr[] = {
      "None",
      "Vertical",
      "Horizontal",
      "Both"
    };
    
    VideoFileType                sVideoFileType = VideoFileType::JpegSequence;
    ImageFlip                    sImageFlip = ImageFlip::FlipNone;

    void PrintSettings()
    {
      STVIDEO_PRINT_INFOF("Settings.FileFormat = %s", sVideoFileTypeStr[(size_t) sVideoFileType]);
      STVIDEO_PRINT_INFOF("Settings.ImageFlip  = %s", sImageFlipStr[(size_t) sImageFlip]);
    }

  }
}
