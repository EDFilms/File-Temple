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

#include "midiRequired.h"
#include "midiIterator.h"
#include "expExporterBase.h"
#include "midiEventGraph.h"
#include "midiEventSchema.h"

namespace SceneTrackMidi
{

  enum MidiEventType
  {
    None,
    Physics
  };

  struct MidiEventMetaInfo
  {
    MidiEventType             type;
    u32                       handle;
  };

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

  union MidiEventObject
  {
    MidiEventMetaInfo         meta;
    MidiEventPhysicsObject    physics;
  };

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

    void ExportFrame(f64 frameTime, bool isLastFrame) override;

    void ReadPhysicsEvent(Iterator& it, SchemaClass& klass);

    void ReadGameObject(Iterator& it, SchemaClass& klass);

    void ExportValue(Iterator& it, SchemaClass& klass) override;

    void SubmitObject();

    void ResetObject();

  };

}
