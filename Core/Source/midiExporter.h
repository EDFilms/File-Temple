// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * midiExporter.h
 *
 * midiExporter implementation of the Shared ExporterBase_t class
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "midiRequired.h"
#include "midiIterator.h"
#include "expExporterBase.h"
#include "midiEventGraph.h"
#include "midiEventSchema.h"

namespace SceneTrackMidi
{

  /**
   * Midi Event types 
   */
  enum MidiEventType
  {
    None,
    Physics
  };

  /**
   * Shared information about a MidiEvent
   * 
   * Classes that are like MidiEventMetaInfo (MidiEventPhysicsObject)
   * must implement the same fields, in the exact same order, so
   * that the specific and meta information align in memory correctly.
   *
   * @see MidiEventPhysicsObject
   * @see MidiEventObject

   */
  struct MidiEventMetaInfo
  {
    MidiEventType             type;
    u32                       handle;
  };

  /**
   * A Physics event implemented as a MidiEventObject
   */
  struct MidiEventPhysicsObject
  {
    MidiEventType             type;
    u32                       handle;

    u8                        event;
    f32                       contactPoint[3];
    f32                       strength;
    u32                       relationReference[2];
    char                      userData[256];
  };

  /**
   * An Event Object which can be any of the following:
   *    MidiEventPhysicsObject
   *    MidiEventMetaInfo
   */
  union MidiEventObject
  {
    MidiEventMetaInfo         meta;
    MidiEventPhysicsObject    physics;
  };

  /**
   * Implementation of the Exporter pattern for the Midi
   * exporter.
   */
  struct MidiExporterT : ExporterBase_t<Iterator>
  {
    EventGraph* graph;
    std::map<u32, std::string> gameObjects;

    MidiExporterT(const char* srcPath_, const char* dstPath_)
      : ExporterBase_t<Iterator>(srcPath_, dstPath_)
    {
      graph = new EventGraph();
      schema = CreateEventSchema();
    }
    
    ~MidiExporterT() override
    {
      delete graph;
    }

    s32 Start() override;

    s32 Close() override;

    MidiEventObject        object;

    /**
     * Export the current frame to disk, and start a new frame
     * according to frameTime. If this is the last frame, then
     * the file footer is written.
     */
    void ExportFrame(f64 frameTime, bool isLastFrame) override;

    /**
     * Read a physics event from the iterator using the Schema
     * as a guide.
     */
    void ReadPhysicsEvent(Iterator& it, SchemaClass& klass);

    /**
     * Read a GameObject from the iterator using the Schema
     * as a guide.
     */
    void ReadGameObject(Iterator& it, SchemaClass& klass);

    /**
     * A value of a Object has changed, or been created. Export
     * as necessary using the Schema as a guide.
     */
    void ExportValue(Iterator& it, SchemaClass& klass) override;

    void SubmitObject();

    void ResetObject();

  };

}
