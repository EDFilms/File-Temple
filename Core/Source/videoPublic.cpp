// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * videoPublic.cpp
 *
 * Public API library header functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "videoPublic.h"
#include "videoExporter.h"
#include "videoSettings.h"

using namespace SceneTrackVideo;

s32 videoSetFileType(stEnum32 fileType)
{
  if (fileType < 0 || fileType >= (stEnum32) VideoFileType::COUNT)
  {
    STVIDEO_PRINT_ERRORF("videoSetFileType::Unknown file type %i", fileType);
    return -1;
  }

  Settings::sVideoFileType = (VideoFileType) fileType;
  return 0;
}

s32 videoSetImageFlip(stEnum32 imageFlip)
{
  if (imageFlip < 0 || imageFlip >= (stEnum32) ImageFlip::COUNT)
  {
    STVIDEO_PRINT_ERRORF("videoSetFileType::Unknown flip type %i", imageFlip);
    return -1;
  }

  Settings::sImageFlip = (ImageFlip) imageFlip;
  return 0;
}

std::shared_ptr<VideoExporterT> exporter;

s32 videoStepConvertSceneTrackFileBegin(stCStr src, stCStr dst)
{
  s32 r = ExporterBegin("scenetrackvideo", src, dst, exporter);

  if (r == 0)
  {
    Settings::PrintSettings();
  }

  return r;
}

s32 videoStepConvertSceneTrackFileUpdate()
{
  return ExporterUpdate(exporter);
}

f32 videoStepConvertProgress()
{
  return ExporterProgress(exporter);
}

s32 videoConvertSceneTrackFile(stCStr src, stCStr dst)
{
  return ExporterExportOneGo("scenetrackvideo", src, dst, exporter);
}
