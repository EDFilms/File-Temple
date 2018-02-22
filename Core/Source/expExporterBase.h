// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * expExporterBase.h
 *
 * Shared utility class to iterate through a SceneTrack file
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "expRequired.h"
#include "expSchema.h"
#include "expIterator.h"

/**
 * Open log file for debugging and information purposes
 * @see CloseLogFile
 */
void OpenLogFile(const char* name);

/**
 * Close the log file and release the file handle
 * @see CloseLogFile
 */
void CloseLogFile();

namespace EXP_NAMESPACE
{
  
  /**
   * Shared exporter base class to iterate through the SceneTrack file and provide
   * some support for maintaining state, holding the current schema, and handling
   * the SceneTrack library.
   */
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

    /**
     * Get a reference to the current iterator
     */
    IteratorT&     GetIterator()
    {
      return iterator;
    }

    /**
     * Open the SceneTrack file according to srcPath
     * If opened; the iterator will be created and will point to the first frame.
     */
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

    /**
     * Close the SceneTrack file and reset the iterator
     */
    void CloseSceneTrack()
    {
      stDisposeIterator(iteratorHandle);
      stCloseRecording(recording);

      iterator.Reset();
    }

    /**
     * Reset's the iterator
     */
    virtual void Reset()
    {
      iterator.Reset();
    }

    /**
     * Callback function to start the export processor. 
     */
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
    
    /**
     * Perform a export of an entire frames worth of data
     */
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

    /**
     * Export for the current object's change only as per the iterator
     */
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

    /**
     * Perform an export for a single object's change
     */
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

    /**
     * A new frame has started, the previous changes of data by the exporter
     * must be written to disk or finalised.
     */
    virtual void ExportFrame(f64 frameTime, bool isLastFrame) = 0;

    /**
     * A new value has started; details are given in the iterator and
     * what the value is provided in the schema.
     */
    virtual void ExportValue(IteratorT& it, SchemaClass& klass) = 0;

    /**
     * Close the exporting process
     */
    virtual s32 Close()
    {
      CloseSceneTrack();
     
      STEXP_PRINT_INFOF("End of SceneTrack File. %i Frames Processed.", framesProcessed);
      STEXP_PRINT_INFO("Saved File.");

      progress = 1.0f;

      return 1;
    }

    /**
     * Get the current frame in the iterator
     */
    s32 GetCurrentFrame() const
    {
      return iterator.frameNum;
    }

    /**
     * Get the number of frames available for exporting
     */
    s32 GetFrameCount() const
    {
      return stGetNumFrames();
    }
    
    /**
     * Get the percentage in exporting; represented as <code>current frame / total frames</code>
     */
    f32 GetProgress() const
    { 
      return progress; 
    }

  };

  /**
   * Master function to start exporting, using the TExporter interface
   */
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

  /**
   * Master function to export for an entire frame
   */
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

  /**
   * Master function to return decimal progress, or if there is no exporter return -1
   * @return The progress or -1.0
   */
  template<typename TExporter>
  f32 ExporterProgress(TExporter& exporter)
  {
    if (exporter == nullptr)
      return -1.0f;

    return exporter->GetProgress();
  }

  /**
   * Master function to export an entire SceneTrack file into another format (TExporter) in one attempt
   */
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
