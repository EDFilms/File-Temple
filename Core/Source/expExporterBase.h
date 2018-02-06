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

#include "expRequired.h"
#include "expSchema.h"
#include "expIterator.h"

void OpenLogFile(const char* name);
void CloseLogFile();

namespace EXP_NAMESPACE
{
  
  template<typename IteratorT>
  class ExporterBase_t
  {
  public:

    std::string     srcPath, dstPath;
    
    bool            hasClass;
    SchemaClass     objectClass;
    Schema          schema;
    u32             iteratorHandle;
    IteratorT       iterator;

    u32             recording;
    u32             framesProcessed;
    f32             progress;

    ExporterBase_t(const char* srcPath_, const char* dstPath_)
      : srcPath(srcPath_), dstPath(dstPath_), hasClass(false), iteratorHandle(0), recording(0), framesProcessed(0), progress(0)
    {
    }

    virtual ~ExporterBase_t()
    {
    }

    IteratorT&     GetIterator()
    {
      return iterator;
    }

    bool OpenSceneTrack()
    {
      framesProcessed = 0;
      progress = 0.0f;

      recording = stOpenRecording(srcPath.c_str());

      if (recording == 0)
      {
        STEXP_PRINT_ERROR("Recording was not opened!");
        return false;
      }

      if (stGetNumFrames() == 0)
      {
        STEXP_PRINT_ERROR("Recording has no frames!");
        // nststCloseRecording(recording);
        recording = 0;
        return false;
      }
      
      schema->Discover();

      IteratorT& it = GetIterator();
      iteratorHandle = it.iterator = stCreateForwardIterator();

      STEXP_PRINT_CHATTERF("[Iterator] Recording Opened. Iterator = %i", iteratorHandle);
      STEXP_PRINT_CHATTERF("[Iterator] Frame Count = %i Frames of %i Recorded.", stGetNumFrames(), stGetNumRecordedFrames());
      STEXP_PRINT_CHATTERF("[Iterator] Frame Time = %fs", stGetRecordingTime());
      
      return true;
    }

    void CloseSceneTrack()
    {
      stDisposeIterator(iteratorHandle);
      stCloseRecording(recording);

      iterator.Reset();
    }

    virtual void Reset()
    {
      iterator.Reset();
    }

    virtual s32 Start()
    {
      Reset();
      framesProcessed = 0;
      progress = 0.0f;

      if (OpenSceneTrack() == false)
      { 
        STEXP_PRINT_ERROR("SceneTrack File was not opened. Fbx Export Failed.");
        return -1;
      }

      return 0;
    }
    
    s32 UpdateForEntireFrame()
    {
      bool frameChanged = false;

      while(true)
      {
        bool r = IterateOnce(frameChanged);

        if (r == false)
        {
          Close();
          return 1;
        }

        if (frameChanged)
        {
          stIteratorNext(iteratorHandle);
          return 0;
        }

        stIteratorNext(iteratorHandle);
      }
    }

    virtual s32 Update()
    {
      bool frameChanged = false;
      bool r = IterateOnce(frameChanged);

      if (r == true)
      {
        stIteratorNext(iteratorHandle);
        return 0;
      }

      Close();
      return 1;
    }

    bool IterateOnce(bool& frameChanged)
    {
      IteratorT& it = GetIterator();
      
      //f64 lastFrameTime = it.frameTime; 
      
      // End of File. Commit Frame State. Close.
      if (stIteratorEnded(it.iterator))
      {
        ExportFrame(it.frameTime, true);
        framesProcessed++;
        progress = (((float) iterator.frameNum) / (float) stGetNumFrames());
        hasClass = false;
        it.lastObjectHandle = UINT32_MAX;
        //STEXP_PRINT_INFOF("Processed Frame %i", it.frameNum);
        return false;
      }

      it.Update();

      // Change of Frame. Commit Frame State.
      if (it.bFrameChanged)
      {
        frameChanged = true;
        ExportFrame(it.frameTime, false);
        framesProcessed++;
        progress = (((float) iterator.frameNum) / (float) stGetNumFrames());
        hasClass = false;
        it.lastObjectHandle = UINT32_MAX;
        //STEXP_PRINT_INFOF("Processed Frame %i", it.frameNum);
      }

      // Class Cache
      if (it.lastObjectHandle != it.objectHandle)
      {
        hasClass = schema->FindClass(it.objectType, objectClass);
      }

      // Apply changes if class is in schema.
      if (hasClass)
        ExportValue(it, objectClass);
      
      return true;
    }

    // Exporter Functions. Must Override.

    virtual void ExportFrame(f64 frameTime, bool isLastFrame) = 0;

    virtual void ExportValue(IteratorT& it, SchemaClass& klass) = 0;

    virtual s32 Close()
    {
      CloseSceneTrack();
     
      STEXP_PRINT_INFOF("End of SceneTrack File. %i Frames Processed.", framesProcessed);
      STEXP_PRINT_INFO("Saved File.");

      progress = 1.0f;

      return 1;
    }

    s32 GetCurrentFrame() const
    {
      return iterator.frameNum;
    }

    s32 GetFrameCount() const
    {
      return stGetNumFrames();
    }
    
    f32 GetProgress() const
    { 
      return progress; 
    }

  };

  template<typename TExporter>
  s32 ExporterBegin(const char* name, const char* srcPath, const char* dstPath, std::shared_ptr<TExporter>& exporter)
  {
    
    OpenLogFile(name);

    if (srcPath == NULL)
    {
      STEXP_PRINT_ERROR("Empty source path given!");
      exporter = nullptr;
      CloseLogFile();
      return -1;
    }

    if (dstPath == NULL)
    {
      STEXP_PRINT_ERROR("Empty destination path given!");
      exporter = nullptr;
      CloseLogFile();
      return -1;
    }

    exporter = std::make_shared<TExporter>(srcPath, dstPath);


    if (exporter->Start() != 0)
    {
      STEXP_PRINT_ERROR("Exporter could not setup");
      exporter = nullptr;
      CloseLogFile();
      return -1;
    }

    return 0;

  }

  template<typename TExporter>
  s32 ExporterUpdate(TExporter& exporter)
  {

    if (exporter == nullptr)
      return -1;

    s32 r = exporter->UpdateForEntireFrame();

    if (r == 1 || r == -1) // Finished or Error.
    {
      exporter = nullptr;
      CloseLogFile();
    }

    return r;
  }

  template<typename TExporter>
  f32 ExporterProgress(TExporter& exporter)
  {
    if (exporter == nullptr)
      return -1.0f;

    return exporter->GetProgress();
  }

  template<typename TExporter>
  s32 ExporterExportOneGo(const char* name, const char* srcPath, const char* dstPath, TExporter& exporter)
  {
    s32  r = ExporterBegin(name, srcPath, dstPath, exporter);

    if (r == 0)
    {
      while((r = ExporterUpdate(exporter)) == 0)
        ;
    }

    return r == 1 ? 0 : r;
  }
  
}
