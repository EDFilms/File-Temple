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

#include "fbxRequired.h"
#include "fbxRef.h"
#include "fbxSchema.h"
#include "fbxIterator.h"
#include "fbxAsset.h"

namespace SceneTrackFbx
{
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

    bool IterateOver(const Iterator& iterator, SchemaClass schema);

  };

  typedef Ref<Material_t> MaterialRef;

}
