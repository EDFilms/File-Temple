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

#include "midiEventSchema.h"

namespace SceneTrackMidi
{
  Schema CreateEventSchema()
  {
    Schema schema = std::make_shared<SchemaT>();
    
    schema->makeClass("GameObject", Frequency::Dynamic, 100, SchemaClassType::GameObject)
      .Component("Name")
        .Kind(ST_KIND_NAMED)
        .Type<stStr>()
      .End()
      .Component("Layer")
        .Kind(ST_KIND_LAYER)
        .Type<s32>()
      .End()
      .Component("Transform")
        .Kind(ST_KIND_SPATIAL)
        .Type<u32>()
      .End()
      .Component("Visibility")
        .Kind(ST_KIND_INTENSITY)
        .Type<u8>()
        .NbElements(1)
      .End();

    schema->makeClass("PhysicsObject", Frequency::Event, 301, SchemaClassType::PhysicsEvent)
      .Component("Event")
        .Kind(ST_KIND_EVENT)
        .Type<u8>()
      .End()
      .Component("ContactPoint")
        .Kind(ST_KIND_POSITION)
        .Type<f32>()
        .NbElements(3)
        .Reference(Reference::World)
      .End()
      .Component("Strength")
        .Kind(ST_KIND_INTENSITY)
        .Type<f32>()
      .End()
      .Component("RelationReference")
        .Kind(ST_KIND_RELATIONSHIP)
        .Type<u32>()
        .NbElements(2)
      .End()
      .Component("UserData")
        .Kind(ST_KIND_TYPE)
        .Type<stCStr>()
      .End();

    return schema;
  }
}
