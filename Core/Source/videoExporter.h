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
#include "videoIterator.h"
#include "videoSchema.h"
#include "expExporterBase.h"

namespace SceneTrackVideo
{
  struct VideoFrame
  {
    u32   width, height;
    u8*   data, *temp;
    u32   dataSize, tempSize;
    bool  outOfDate;
    FILE* file;
    FILE* timingFile;

    VideoFrame()
      : width(0), height(0), data(NULL), temp(NULL), outOfDate(false), dataSize(0), tempSize(0), file(NULL), timingFile(NULL)
    {
    }

    ~VideoFrame()
    {
      if (data != NULL)
      {
        free(data);
        data = NULL;
        dataSize = 0;
      }

      if (temp != NULL)
      {
        free(temp);
        temp = NULL;
        tempSize = 0;
      }

      if (file != NULL)
      {
        fclose(file);
      }

      if (timingFile != NULL)
      {
        fclose(timingFile);
      }

    }
  };

  struct VideoExporterT : ExporterBase_t<Iterator>
  {
    
    std::map<u32, VideoFrame*> frames;

    VideoExporterT(const char* srcPath_, const char* dstPath_)
      : ExporterBase_t<Iterator>(srcPath_, dstPath_)
    {
      schema = CreateVideoSchema();
    }

    ~VideoExporterT() override;

    s32 Start() override;

    s32 Close() override;

    void ExportFrame(f64 frameTime, bool isLastFrame) override;

    void ExportValue(Iterator& it, SchemaClass& klass) override;

    void SaveJpegFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& timingFile);

    void SavePngFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& timingFile);

    void SaveMjpegFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data, FILE*& file);

    void SaveMpegFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data, FILE*& file);
    
    void SaveRawFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& timingFile);

    void SaveTimingFile(const char* name, u32 objectId, u64 frameNum, f64 frameTime, FILE*& file, u32 width, u32 height);

    void FlipHorz(VideoFrame* frame);

    void FlipVert(VideoFrame* frame);

    void FlipBoth(VideoFrame* frame);

  };

}
