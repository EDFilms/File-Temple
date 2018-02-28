// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * videoExporter.h
 *
 * videoExporter implementation of the Shared ExporterBase_t class
 * 
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
  /**
   * A Single frame of video, including meta data, video data,
   * and the file handle if is being saved.
   */
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
  
  /**
   * Implementation of the Exporter pattern for the Video
   * exporter.
   */
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
    
    /**
     * Export the current (Scene Track) frame to disk, and start a new frame
     * according to frameTime. If this is the last frame, then
     * the file footer is written.
     */
    void ExportFrame(f64 frameTime, bool isLastFrame) override;
    
    /**
     * A value of a Object has changed, or been created. Export
     * as necessary using the Schema as a guide.
     */
    void ExportValue(Iterator& it, SchemaClass& klass) override;

    /**
     * Read a video frame and save to disk as a JPEG image
     */
    void SaveJpegFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& timingFile);
    
    /**
     * Read a video frame and save to disk as a PNG image
     */
    void SavePngFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& timingFile);
    
    /**
     * Read a video frame and append to the currently opened MJPEG video container
     */
    void SaveMjpegFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data, FILE*& file);
    
    /**
     * Read a video frame and append to the currently opened MPEG video container
     */
    void SaveMpegFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data, FILE*& file);
    
    /**
     * Read a video frame and save it to disk as is.
     */
    void SaveRawFrame(u32 objectId, u64 frameNum, f64 frameTime, u32 width, u32 height, u8* data,FILE*& timingFile);

    /**
     * Write the list of frames counts, times and frame file names in a FFPEG timing file format
     */
    void SaveTimingFile(const char* name, u32 objectId, u64 frameNum, f64 frameTime, FILE*& file, u32 width, u32 height);
    
    /**
     * Flip a video frame's image horizontally
     */
    void FlipHorz(VideoFrame* frame);
    
    /**
     * Flip a video frame's image vertically
     */
    void FlipVert(VideoFrame* frame);
    
    /**
     * Flip a video frame's image horizontally and vertically
     */
    void FlipBoth(VideoFrame* frame);

  };

}
