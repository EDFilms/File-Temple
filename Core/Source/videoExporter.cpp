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

#include "videoExporter.h"
#include "videoSettings.h"

#include <sstream>
#include <fstream>


#include "lodepng.h"
#include "tiny_jpeg.h"
#include "jo_mpeg.h"

namespace SceneTrackVideo
{
  VideoExporterT::~VideoExporterT()
  { 
      
    std::stringstream path;
    path << dstPath << "_FFmpeg.txt";
    std::string strPath(path.str());

    std::ofstream rec;
    rec.open (strPath);

    for(auto it : frames)
    {
      rec << dstPath << '_' << it.first << ".txt" << std::endl;
      delete it.second;
    }
    
    rec.close(); 

  }

  s32 VideoExporterT::Start()
  {
    s32 r = ExporterBase_t<Iterator>::Start();

    if (r == 0)
    {
      // Open output file for writing here.
    }

    return 0;
  }

  s32 VideoExporterT::Close()
  {
    // Close output here.

    return ExporterBase_t<Iterator>::Close();
  }

  void VideoExporterT::ExportFrame(f64 frameTime, bool isLastFrame)
  {
    STVIDEO_UNUSED(frameTime);
    STVIDEO_UNUSED(isLastFrame);

    for(auto frameIt : frames)
    {
      VideoFrame* frame = frameIt.second;

      if (frame->outOfDate)
      {
        switch(Settings::sImageFlip)
        {
          case ImageFlip::FlipVert:
          {
            FlipVert(frame);
          }
          break;
          case ImageFlip::FlipHorz:
          {
            FlipHorz(frame);
          }
          break;
          case ImageFlip::FlipBoth:
          {
            FlipBoth(frame);
          }
          break;
        }

        switch(Settings::sVideoFileType)
        {
          case VideoFileType::JpegSequence:
          {
            if (frame->data != NULL)
            {
              SaveJpegFrame(frameIt.first, iterator.frameNum, iterator.frameLength, frame->width, frame->height, frame->data, frame->timingFile);
            }
          }
          break;
          case VideoFileType::PngSequence:
          {
            if (frame->data != NULL)
            {
              SavePngFrame(frameIt.first, iterator.frameNum, iterator.frameLength, frame->width, frame->height, frame->data, frame->timingFile);
            }
          }
          break;
          case VideoFileType::MjpegMovie:
          {
            if (frame->data != NULL)
            {
              SaveMjpegFrame(frameIt.first, iterator.frameNum, iterator.frameLength, frame->width, frame->height, frame->data, frame->file);
            }
          }
          break;
          case VideoFileType::Mpeg1Movie:
          {
            if (frame->data != NULL)
            {
              SaveMpegFrame(frameIt.first, iterator.frameNum, iterator.frameLength, frame->width, frame->height, frame->data, frame->file);
            }
          }
          break;
          case VideoFileType::RawSequence:
          {
            if (frame->data != NULL)
            {
              SaveRawFrame(frameIt.first, iterator.frameNum, iterator.frameLength, frame->width, frame->height, frame->data, frame->timingFile);
            }
          }
          break;
        }

        frame->outOfDate = false;
      }
    }
  }

  void VideoExporterT::ExportValue(Iterator& it, SchemaClass& klass)
  {
    if (iterator.iteratorType == ST_ITERATORTYPE_NEWOBJECT)
    {
      VideoFrame* frame = new VideoFrame();
      frames.insert(std::make_pair(it.objectHandle, frame));
      return;
    }
    else if (iterator.iteratorType == ST_ITERATORTYPE_DELETEOBJECT)
    {
      return;
    }

    SchemaComponent component;
    if (klass->FindComponent(iterator.componentHandle, component) == false)
    {
      STEXP_PRINT_ERRORF("Unknown Component Type for GameObject => %i, Object = %i", iterator.componentHandle, iterator.objectHandle);
      return;
    }

    auto frameIt = frames.find(it.objectHandle);

    if (frameIt == frames.end())
      return;

    VideoFrame* frame = frameIt->second;

    switch(component->kind)
    {
      case ST_KIND_SIZE:
      {
        u32 wh[2];
        it.ReadVectorU32_2(wh);

        frame->width = wh[0];
        frame->height = wh[1];
        frame->outOfDate = true;
      }
      break;
      case ST_KIND_IMAGE:
      {
        u32 size = iterator.arraySize * 3;

        if (frame->dataSize == 0 || size > frame->dataSize)
        {
          frame->data = (u8*) realloc(frame->data, size);
          frame->dataSize = size;
        }

        stIteratorGetValueArray(iterator.iterator, frame->data, frame->dataSize, stCalculateStride1(ST_TYPE_INT8, 3));
        frame->outOfDate = true;
      }
      break;
    }

  }

  void VideoExporterT::SaveJpegFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& timingFile)
  {
    STEXP_UNUSED(frameTime);

    std::stringstream path;
    path << dstPath << '_' << objectId << '_' << frameNum << ".jpg";
    std::string strPath(path.str());

    if (tje_encode_to_file(strPath.c_str(), width, height, 3, data) == false)
    {
      STVIDEO_PRINT_INFOF("Object %i Frame %i was not saved as png", objectId, (u32) frameNum);
    }
    else
    {
      SaveTimingFile(strPath.c_str(), objectId, frameNum, frameTime, timingFile, width, height);
    }
  }

  void VideoExporterT::SavePngFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& timingFile)
  {
    STEXP_UNUSED(frameTime);

    std::stringstream path;
    path << dstPath << '_' << objectId << '_' << frameNum << ".png";
    std::string strPath(path.str());
    
    u32 r = 0;

    if ( (r = lodepng_encode_file(strPath.c_str(), data, width, height, LCT_RGB, 8)) != 0)
    {
      STVIDEO_PRINT_INFOF("Object %i Frame %i was not saved as png to '%s', %i", objectId, (u32) frameNum, strPath.c_str(), r);
    }
    else
    {
      SaveTimingFile(strPath.c_str(), objectId, frameNum, frameTime, timingFile, width, height); 
    }
  }

  static void mjpeg_func(void* context, void* data, int size)
  {
    FILE* fd = (FILE*)context;
    fwrite(data, size, 1, fd);
  }

  void VideoExporterT::SaveMjpegFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& file)
  {
    STEXP_UNUSED(frameNum);
    STEXP_UNUSED(frameTime);

    if (file == NULL)
    {
      std::stringstream path;
      path << dstPath << '_' << objectId << ".mjpeg";

#if STEXP_PLATFORM == STEXP_PLATFORM_WINDOWS
      fopen_s(&file, path.str().c_str(), "wb");
#else
      file = fopen(path.str().c_str(), "wb");
#endif 

      STVIDEO_PRINT_INFOF("Open MJPEG %i %i", objectId, (u32) frameNum);
    }

    tje_encode_with_func(mjpeg_func, file, 3, width, height, 3, data);
  }

  void VideoExporterT::SaveMpegFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& file)
  {
    STEXP_UNUSED(frameNum);
    STEXP_UNUSED(frameTime);

    if (file == NULL)
    {
      std::stringstream path;
      path << dstPath << '_' << objectId << ".mpeg";

#if STEXP_PLATFORM == STEXP_PLATFORM_WINDOWS
      fopen_s(&file, path.str().c_str(), "wb");
#else
      file = fopen(path.str().c_str(), "wb");
#endif
    }

    jo_write_mpeg(file, data, width, height, 24);
  }
  
  void VideoExporterT::SaveRawFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& timingFile)
  {
    STEXP_UNUSED(frameTime);

    std::stringstream path;
    path << dstPath << '_' << objectId << '_' << frameNum << ".rgb";
    std::string strPath(path.str());
    
    u32 r = 0;
    FILE* f = NULL;

#if STEXP_PLATFORM == STEXP_PLATFORM_WINDOWS
    fopen_s(&f, strPath.c_str(), "wb");
#else
    f = fopen(strPath.c_str(), "wb");
#endif

    if (f == NULL)
    {
      STVIDEO_PRINT_INFOF("Object %i Frame %i was not saved as rgb to '%s', %i", objectId, (u32) frameNum, strPath.c_str(), r);
    }
    else
    {
      fwrite(data, 1, width * height * 3, f);
      fclose(f);
      SaveTimingFile(strPath.c_str(), objectId, frameNum, frameTime, timingFile, width, height); 
    }

  }

  void VideoExporterT::SaveTimingFile(const char* name, u32 objectId, u64 frameNum, f64 frameTime, FILE*& file, u32 width, u32 height)
  {
    STEXP_UNUSED(frameNum);

    if (file == NULL)
    {
      std::stringstream path;
      path << dstPath << '_' << objectId << ".txt";

#if STEXP_PLATFORM == STEXP_PLATFORM_WINDOWS
      fopen_s(&file, path.str().c_str(), "wb");
#else
      file = fopen(path.str().c_str(), "wb");
#endif

      // # 1280x720
      std::stringstream header;
      header << "# " << width << "x" << height << std::endl;
   
      std::string headerStr(header.str());

      fwrite(headerStr.c_str(), 1, headerStr.length(), file);
    }

    // file '0.png'
    // duration 0.097

    std::string path(name);

    std::stringstream ss;
    ss << "file '" << path.substr(path.find_last_of("\\/") + 1) << "'" << std::endl;
    ss << "duration " << frameTime << std::endl;

    std::string s(ss.str());

    fwrite(s.c_str(), 1, s.length(), file);
  }

#define COL(I, J, C) frame->data[(J * frame->width * 3) + (I * 3) + C]
#define SWAP(A, B, T) T = A; A = B; B = T;
  void VideoExporterT::FlipHorz(VideoFrame* frame)
  {
    u32 ww = frame->width - 1;
    u32 i1;
    u8 r0, g0, b0, r1, g1, b1;

    for(u32 j=0;j < frame->height;j++)
    {
      for(u32 i0=0;i0 < frame->width / 2;i0++)
      {
        i1 = ww - i0;
        r0 = COL(i0, j, 0);
        g0 = COL(i0, j, 1);
        b0 = COL(i0, j, 2);
        r1 = COL(i1, j, 0);
        g1 = COL(i1, j, 1);
        b1 = COL(i1, j, 2);

        COL(i0, j, 0) = r1;
        COL(i0, j, 1) = g1;
        COL(i0, j, 2) = b1;
        COL(i1, j, 0) = r0;
        COL(i1, j, 1) = g0;
        COL(i1, j, 2) = b0;
      }
    }
  }

  void VideoExporterT::FlipVert(VideoFrame* frame)
  {
    u32 stride = frame->width * 3;

    if (frame->tempSize == 0 || stride > frame->tempSize)
    {
      frame->temp = (u8*) realloc(frame->temp, stride);
      frame->tempSize = stride;
    }

    for(u32 j=0;j < frame->height / 2;j++)
    {
      u32 j1 = (frame->height - 1) - j;

      // A => T
      memcpy(frame->temp, (frame->data + j * stride), stride);
      // B => A
      memcpy(frame->data + (j * stride), frame->data + (j1 * stride), stride);
      // T = > B
      memcpy(frame->data + (j1 * stride), frame->temp, stride);
    }

  }

  void VideoExporterT::FlipBoth(VideoFrame* frame)
  {
    FlipHorz(frame);
    FlipVert(frame);
  }

}
