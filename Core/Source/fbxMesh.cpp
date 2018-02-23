// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxMesh.cpp
 *
 * Representation of a triangular mesh and submeshes per the Unity Schema
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "fbxMesh.h"
#include "fbxExporter.h"

namespace SceneTrackFbx
{
  SubMesh_t::SubMesh_t(u32 handle_)
    : handle(handle_), bDeleted(0)
  {
  }

  bool SubMesh_t::LoadIndexes(const Iterator& iterator)
  {
    return iterator.ReadArray(indexes);
  }

  bool SubMesh_t::IterateOver(const Iterator& iterator, SchemaClass schema)
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
        STFBX_PRINT_ERRORF("Unknown Component Type for SubMesh => %i", iterator.componentHandle);
        return false;
      }

      switch(component->kind)
      {
        case ST_KIND_INDEX:
          LoadIndexes(iterator);
        break;
      }


      return true;
    }

  }

  Mesh_t::Mesh_t(u32 handle_)
    : handle(handle_), bDeleted(false), bExportedMesh(false)
  {
  }

  bool Mesh_t::LoadSubMeshHandles(const Iterator& iterator)
  {
    std::vector<u32>       subMeshHandles;
    if (iterator.ReadArray(subMeshHandles) == false)
      return false;

    for(auto id : subMeshHandles)
    {
      SubMeshRef subMesh = Iterator::sExporter->FindOrCreateSubMesh(id);
      subMeshes.push_back(subMesh);
    }

    return true;
  }

  bool Mesh_t::IterateOver(const Iterator& iterator, SchemaClass schema)
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
        STFBX_PRINT_ERRORF("Unknown Component Type for Mesh => %i", iterator.componentHandle);
        return false;
      }
      bool r = false;

      // printf("Mesh Component %s %i\n", component->name.c_str(), component->kind);

      switch(component->kind)
      {
        case ST_KIND_NAMED:
          LoadName(iterator);
        break;
        case ST_KIND_VERTEX:
          LoadVertices(iterator);
        break;
        case ST_KIND_NORMAL:
          LoadNormals(iterator);
        break;
        case ST_KIND_TANGENT:
          LoadTangents(iterator);
        break;
        case ST_KIND_COLOR:
          LoadColors(iterator);
        break;
        case ST_KIND_UV_0:
          r = iterator.ReadArray(uv);
        return r;
        case ST_KIND_UV_1:
          LoadUv2(iterator);
        break;
        case ST_KIND_UV_2:
          LoadUv3(iterator);
        break;
        case ST_KIND_UV_3:
          LoadUv4(iterator);
        break;
        case ST_KIND_BONE_WEIGHT:
          LoadBoneWeight(iterator);
        break;
        case ST_KIND_BONE_INDEX:
          LoadBoneIndex(iterator);
        break;
        case ST_KIND_SIZE:
          // LoadBounds(iterator);
        break;
        case ST_KIND_GEOMETRY:
          LoadSubMeshHandles(iterator);
        break;
        case ST_KIND_POSE:
          LoadBindPoses(iterator);
        break;
      }
      return true;
    }
  }

}
