// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxMaterial.h
 *
 * Represents a state of a Material according to the Unity Schema
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "fbxRequired.h"
#include "fbxRef.h"
#include "fbxSchema.h"
#include "fbxIterator.h"
#include "fbxAsset.h"

namespace SceneTrackFbx
{
  /**
   * An implementation of the current state of a Material as per the Unity Schema
   */
  class Material_t
  {
  public:

    u32             handle;
    std::string     name;
    Asset           surface;
    Asset           image;
    Vector3f        color, specular, emissive;
    f32             transparency, reflection;

    stBool32        bExported : 1;
    stBool32        bDeleted : 1;

    FMaterial       fMaterial;

    Material_t(u32 handle);

    /**
     * From a given iterator and schema iterate and read any or all
     * of the material properties from the iterator.
     */
    bool IterateOver(const Iterator& iterator, SchemaClass schema);

  };

  typedef Ref<Material_t> MaterialRef;

}
