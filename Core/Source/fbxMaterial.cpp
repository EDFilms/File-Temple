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

#include "fbxMesh.h"
#include "fbxExporter.h"

namespace SceneTrackFbx
{
  Material_t::Material_t(u32 handle_)
    : handle(handle_), 
      color(1.0f, 1.0f, 1.0f),
      emissive(0.0f, 0.0f, 0.0f),
      specular(0.5f, 0.5f, 0.5f),
      transparency(0),
      reflection(0),
      bExported(0),
      bDeleted(0)
  {
  }

  bool Material_t::IterateOver(const Iterator& iterator, SchemaClass schema)
  {
    if (iterator.iteratorType == ST_ITERATORTYPE_NEWOBJECT)
    {
      return true;
    }
    else if (iterator.iteratorType == ST_ITERATORTYPE_DELETEOBJECT)
    {
      bDeleted = true;
      return true;
    }
    else
    {
      SchemaComponent component;
      if (schema->FindComponent(iterator.componentHandle, component) == false)
      {
        STFBX_PRINT_ERRORF("Unknown Component Type for Material => %i", iterator.componentHandle);
        return false;
      }

      switch(component->kind)
      {
        case ST_KIND_NAMED:
          iterator.ReadString(name);
        break;
        case ST_KIND_SURFACE:
          surface = iterator.ReadAsset(AssetType::Shader);
        break;
        case ST_KIND_IMAGE:
          image = iterator.ReadAsset(AssetType::Texture);
        break;
        case ST_KIND_COLOR:
          iterator.ReadVector3(color);
        break;
        case ST_KIND_SPECULAR:
          iterator.ReadVector3(specular);
        break;
        case ST_KIND_EMISSIVE:
          iterator.ReadVector3(emissive);
        break;
        case ST_KIND_TRANSPARENCY:
          iterator.ReadF32(transparency);
        break;
        case ST_KIND_REFLECTION:
          iterator.ReadF32(reflection);
        break;
      }
      return true;
    }
  }

}
