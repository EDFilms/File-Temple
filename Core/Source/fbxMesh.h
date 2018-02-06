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

#pragma once

#include "fbxRequired.h"
#include "expMath.h"
#include "fbxIterator.h"
#include "fbxSchema.h"
#include "fbxRef.h"

namespace SceneTrackFbx
{

  class SubMesh_t
  { 
  public:

    u32                    handle;
    std::vector<u32>       indexes;
    stBool32               bDeleted : 1;

    SubMesh_t(u32 handle_);

    bool LoadIndexes(const Iterator& iterator);

    bool IterateOver(const Iterator& iterator, SchemaClass schema);

  };

  typedef Ref<SubMesh_t> SubMeshRef;

  class Mesh_t
  {
  public:
    u32                    handle;
    std::string            name;
    std::vector<Vector3f>  vertices;
    std::vector<Vector3f>  normals;
    std::vector<Vector4f>  tangents;
    std::vector<Vector4f>  colors;
    std::vector<Vector2f>  uv;
    std::vector<Vector2f>  uv2;
    std::vector<Vector2f>  uv3;
    std::vector<Vector2f>  uv4;
    std::vector<Vector4ub> boneIndex;
    std::vector<Vector4f>  boneWeight;
    std::vector<Matrix44f> bindPoses;

    std::vector<Ref<SubMesh_t>>   subMeshes;
    Bounds                 bounds;

    bool  bDeleted ;;
    bool               bExportedMesh;
    FMesh                  fMesh;

    Mesh_t(u32 handle_);

    bool LoadName(const Iterator& iterator)
    {
      return iterator.ReadString(name);
    }

    bool LoadVertices(const Iterator& iterator)
    {
      return iterator.ReadArray(vertices);
    }

    bool LoadNormals(const Iterator& iterator)
    {
      return iterator.ReadArray(normals);
    }

    bool LoadTangents(const Iterator& iterator)
    {
      return iterator.ReadArray(tangents);
    }
    
    bool LoadColors(const Iterator& iterator)
    {
      return iterator.ReadArray(colors);
    }

    bool LoadUv2(const Iterator& iterator)
    {
      return iterator.ReadArray(uv2);
    }

    bool LoadUv3(const Iterator& iterator)
    {
      return iterator.ReadArray(uv3);
    }

    bool LoadUv4(const Iterator& iterator)
    {
      return iterator.ReadArray(uv4);
    }

    bool LoadBoneIndex(const Iterator& iterator)
    {
      return iterator.ReadArray(boneIndex);
    }

    bool LoadBoneWeight(const Iterator& iterator)
    {
      return iterator.ReadArray(boneWeight);
    }

    bool LoadSubMeshHandles(const Iterator& iterator);

    bool LoadBounds(const Iterator& iterator)
    {
      u32 r = stIteratorGetValueArray(iterator.iterator, (u8*) &bounds, (u32) (sizeof(Bounds)), stCalculateStride2(ST_TYPE_FLOAT32, 3, ST_TYPE_FLOAT32, 3));

      return r != sizeof(Bounds);
    }

    bool LoadBindPoses(const Iterator& iterator)
    {
      return iterator.ReadArray(bindPoses);
    }

    bool IterateOver(const Iterator& iterator, SchemaClass schema);

  };

  typedef Ref<Mesh_t> MeshRef;

}
