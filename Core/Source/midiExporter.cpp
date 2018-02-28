// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * midiExporter.cpp
 *
 * midiExporter implementation of the Shared ExporterBase_t class
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "midiExporter.h"
#include "midiSettings.h"
#include "midiWriter.h"

namespace SceneTrackMidi
{

  s32 MidiExporterT::Start()
  {
    s32 r = ExporterBase_t<Iterator>::Start();

    if (r == 0)
    {
      // Open output file for writing here.
      ResetObject();
    }

    return 0;
  }

  s32 MidiExporterT::Close()
  {
    // Close output here.
    graph->averageFps /= (f64) graph->nbFrames;
    
    // Write to file using writer here.
    switch(Settings::sMidiFileType)
    {
      case MidiFileType::Midi: 
        EventMidiWriter(dstPath.c_str(), graph, gameObjects);
      break;
      case MidiFileType::Xml: 
        EventXmlWriter(dstPath.c_str(), graph, gameObjects);
      break;
    }

    return ExporterBase_t<Iterator>::Close();
  }

  void MidiExporterT::ExportFrame(f64 frameTime, bool isLastFrame)
  {
    STEXP_UNUSED(isLastFrame);    // Not needed
    STEXP_UNUSED(frameTime);      // Not needed
    
    SubmitObject();
    graph->nbFrames++;
    graph->averageFps += frameTime;
  }

  void MidiExporterT::ReadPhysicsEvent(Iterator& it, SchemaClass& klass)
  {
    if (iterator.iteratorType == ST_ITERATORTYPE_NEWOBJECT)
    {
      object.meta.type   = MidiEventType::Physics;
      object.meta.handle = it.objectHandle;
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

    switch(component->kind)
    {
      case ST_KIND_EVENT:
      {
        it.ReadU8(object.physics.event);
      }
      break;
      case ST_KIND_POSITION:
      {
        it.ReadVectorF32_3(object.physics.contactPoint);
      }
      break;
      case ST_KIND_INTENSITY:
      {
        it.ReadF32(object.physics.strength);
      }
      break;
      case ST_KIND_RELATIONSHIP:
      {
        it.ReadVectorU32_2(object.physics.relationReference);
      }
      break;
      case ST_KIND_TYPE:
      {
        it.ReadCString(object.physics.userData, sizeof(object.physics.userData) / sizeof(char));
      }
      break;
    }

  }

  void MidiExporterT::ReadGameObject(Iterator& it, SchemaClass& klass)
  {

    if (iterator.iteratorType == ST_ITERATORTYPE_NEWOBJECT)
    {
      gameObjects.insert(std::make_pair(it.objectHandle, std::string()));
      return;
    }
    else if (iterator.iteratorType == ST_ITERATORTYPE_DELETEOBJECT)
    {
      return;
    }

    SchemaComponent component;
    if (klass->FindComponent(iterator.componentHandle, component) == false)
    {
      return;
    }

    if (component->kind == ST_KIND_NAMED)
    {
      std::string name;
      it.ReadString(name);

      gameObjects[it.objectHandle] = name;
    }

  }

  void MidiExporterT::ExportValue(Iterator& it, SchemaClass& klass)
  {
    if (it.objectHandle != object.meta.handle)
      SubmitObject();

    switch(klass->classType)
    {
      case SchemaClassType::PhysicsEvent:
      {
        ReadPhysicsEvent(it, klass);
      }
      break;
      case SchemaClassType::GameObject:
      {
        ReadGameObject(it, klass);
      }
      break;
    }
  }

  void MidiExporterT::SubmitObject()
  {
    if (object.meta.type == MidiEventType::None)
      return;

    bool didCreate = false;
    u32 objectId = object.physics.relationReference[0];
    ObjectEventTrack* track = graph->GetOrAdd(objectId, didCreate);


    switch(object.meta.type)
    {
      case MidiEventType::Physics:
      {
        // Ignore Continue events (implied by Start/Stop), Midi doesn't seem to have a Note "Still down" event.
        if (object.physics.event != (u8) EventType::Continue)
        {
          Event& evt = track->Add(EventClass::PhysicsEvent, (EventType) object.physics.event);                                  // Event
          Settings::Trs::sTransform.Transform3<kNodePropertyNodeTranslation>(evt.worldPosition, object.physics.contactPoint);   // ContactPoint
          evt.strength    = object.physics.strength;                                                                            // Strength
          evt.frameTime   = iterator.frameTime;
          evt.frameNumber = iterator.frameNum;
          evt.frameLength = iterator.frameLength;
          evt.otherObject = object.physics.relationReference[1];
        
          if (object.physics.userData[0] != 0)
          {
            track->userData = std::string(object.physics.userData); 
          }

        }
      }
      break;
    }

    ResetObject();
  }

  void MidiExporterT::ResetObject()
  {
    memset(&object, 0, sizeof(object));
    object.meta.handle = 0;
    object.meta.type = MidiEventType::None;
  }
}
