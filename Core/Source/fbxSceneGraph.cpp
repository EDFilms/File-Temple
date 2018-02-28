// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxSceneGraph.cpp
 *
 * Collection of classes that correspond to the Unity Scene (or a SceneGraph
 * like it) which matches to the Unity Schema
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "fbxSceneGraph.h"
#include "fbxExporter.h"
#include "fbxSettings.h"

namespace SceneTrackFbx
{

  GameObject_t::GameObject_t(u32 handle_)
    : handle(handle_), name(), layer(0), transform(), isVisible(true), bDeleted(0), bResolvedTransform(0)
  {
  }

  GameObject_t::~GameObject_t()
  {
  }

  bool GameObject_t::IterateOver(const Iterator& iterator, SchemaClass schema)
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
        STFBX_PRINT_ERRORF("Unknown Component Type for GameObject => %i, Object = %i", iterator.componentHandle, iterator.objectHandle);
        return false;
      }

      switch(component->kind)
      {
        case ST_KIND_NAMED:
          iterator.ReadString(name);
        break;
        case ST_KIND_LAYER:
          iterator.ReadS32(layer);
        break;
        case ST_KIND_SPATIAL:
          transform = iterator.ReadTransformRef();
        break;
        case ST_KIND_INTENSITY:
          u8 visible8;
          iterator.ReadU8(visible8);

          isVisible = (visible8 != 0);
        break;
      }

      return true;
    }
  }

  Transform_t::Transform_t(u32 handle_)
    : handle(handle_),
      localPosition(0,0,0),
      localRotation(0,0,0,1), 
      localScale(1,1,1), 
      parent(), 
      bDeleted(false),
      bTransformOutOfDate(true),
      bPosOutOfDate(false),
      bRotOutOfDate(false),
      bScaOutOfDate(false)
  {
    markAsExported();
  }

  void Transform_t::markAsExported()
  {
    bTransformOutOfDate = false;
    bPosOutOfDate = false;
    bRotOutOfDate = false;
    bScaOutOfDate = false;
  }

  bool Transform_t::IterateOver(const Iterator& iterator, SchemaClass schema)
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
        STFBX_PRINT_ERRORF("Unknown Component Type for Transform => %i", iterator.componentHandle);
        return true;
      }

      switch(component->kind)
      {
        case ST_KIND_POSITION:
        {
          Vector3f newLocalPosition;
          iterator.ReadVector3(newLocalPosition);

          if (Vector3f::NotEquals(localPosition, newLocalPosition))
          {
            localPosition = newLocalPosition;
            bTransformOutOfDate = true;
            bPosOutOfDate = true;
          }
        }
        break;
        case ST_KIND_ROTATION:
        {
          Vector4f newLocalRotation;
          iterator.ReadVector4(newLocalRotation);

          if (Vector4f::NotEquals(localRotation, newLocalRotation))
          {
            localRotation = newLocalRotation;
            bTransformOutOfDate = true;
            bRotOutOfDate = true;
          }
        }
        break;
        case ST_KIND_SCALE:
        {
          Vector3f newLocalScale;
          iterator.ReadVector3(newLocalScale);

          if (Vector3f::NotEquals(localScale, newLocalScale))
          {
            localScale = newLocalScale;
            bTransformOutOfDate = true;
            bScaOutOfDate = true;
          }
        }
        break;
        case ST_KIND_PARENT:
          parent = iterator.ReadTransformRef();
          bTransformOutOfDate = true;
          bPosOutOfDate = true;
          bRotOutOfDate = true;
          bScaOutOfDate = true;
        break;
      }

      return true;
    }
  }

  StandardMeshRenderer_t::StandardMeshRenderer_t(u32 handle_)
    : handle(handle_), bCreated(0), bDeleted(0)
  {
  }

  bool StandardMeshRenderer_t::IterateOver(const Iterator& iterator, SchemaClass schema)
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
        STFBX_PRINT_ERRORF("Unknown Component Type for StandardMeshRenderer => %i", iterator.componentHandle);
        return false;
      }

      switch(component->kind)
      {
        case ST_KIND_SURFACE:
          iterator.ReadMaterialRefs(materials);
        break;
        case ST_KIND_GEOMETRY:
          mesh = iterator.ReadMeshRef();
        break;
        case ST_KIND_PARENT:
          parent = iterator.ReadTransformRef();
        break;
      }

      return true;
    }

  }

  SkinnedMeshRenderer_t::SkinnedMeshRenderer_t(u32 handle_)
    : handle(handle_), bCreated(0), bDeleted(0)
  {
  }


  bool SkinnedMeshRenderer_t::IterateOver(const Iterator& iterator, SchemaClass schema)
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
        STFBX_PRINT_ERRORF("Unknown Component Type for SkinnedMeshRenderer => %i", iterator.componentHandle);
        return false;
      }

      switch(component->kind)
      {
        case ST_KIND_SURFACE:
          iterator.ReadMaterialRefs(materials);
        break;
        case ST_KIND_GEOMETRY:
          mesh = iterator.ReadMeshRef();
        break;
        case ST_KIND_PARENT:
          parent = iterator.ReadTransformRef();
        break;
        case ST_KIND_BONE:
          iterator.ReadTransformRefs(bones);
        break;
        case ST_KIND_BONE_BEGIN:
          boneTransform = iterator.ReadTransformRef();
        break;
      }

      return true;
    }

  }

}
