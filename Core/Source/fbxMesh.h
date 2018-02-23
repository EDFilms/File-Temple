// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxMesh.h
 *
 * Representation of a triangular mesh and submeshes per the Unity Schema
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "fbxRequired.h"
#include "expMath.h"
#include "fbxIterator.h"
#include "fbxSchema.h"
#include "fbxRef.h"

namespace SceneTrackFbx
{

  /**
   * The current state of indexes which makes up partly or wholly
   * a triangular mesh.
   */
  class SubMesh_t
  { 
  public:

    u32                    handle;
    std::vector<u32>       indexes;
    stBool32               bDeleted : 1;

    SubMesh_t(u32 handle_);

    /**
     * Load indexes from the current position in the given iterator
     */
    bool LoadIndexes(const Iterator& iterator);

    /**
     * Load all or any properties and indexes from the current property
     * from the iterator according to the given Schema
     */
    bool IterateOver(const Iterator& iterator, SchemaClass schema);

  };

  typedef Ref<SubMesh_t> SubMeshRef;

  /**
   * The current state of a triangluar mesh which is made up of
   * vertices, normals, tangents, colours, uvs, bones and submeshes
   */
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

    /**
     * Load the name property from the given iterator at the current position
     */
    bool LoadName(const Iterator& iterator)
    {
      return iterator.ReadString(name);
    }
    
    /**
     * Load the vertices property from the given iterator at the current position
     */
    bool LoadVertices(const Iterator& iterator)
    {
      return iterator.ReadArray(vertices);
    }
    
    /**
     * Load the normals property from the given iterator at the current position
     */
    bool LoadNormals(const Iterator& iterator)
    {
      return iterator.ReadArray(normals);
    }
    
    /**
     * Load the tangents property from the given iterator at the current position
     */
    bool LoadTangents(const Iterator& iterator)
    {
      return iterator.ReadArray(tangents);
    }
    
    /**
     * Load the colors property from the given iterator at the current position
     */
    bool LoadColors(const Iterator& iterator)
    {
      return iterator.ReadArray(colors);
    }
    
    /**
     * Load the uv2 property from the given iterator at the current position
     */
    bool LoadUv2(const Iterator& iterator)
    {
      return iterator.ReadArray(uv2);
    }
    
    /**
     * Load the uv3 property from the given iterator at the current position
     */
    bool LoadUv3(const Iterator& iterator)
    {
      return iterator.ReadArray(uv3);
    }
    
    /**
     * Load the uv4 property from the given iterator at the current position
     */
    bool LoadUv4(const Iterator& iterator)
    {
      return iterator.ReadArray(uv4);
    }
    
    /**
     * Load the bone index property from the given iterator at the current position
     */
    bool LoadBoneIndex(const Iterator& iterator)
    {
      return iterator.ReadArray(boneIndex);
    }
    
    /**
     * Load the bone weight property from the given iterator at the current position
     */
    bool LoadBoneWeight(const Iterator& iterator)
    {
      return iterator.ReadArray(boneWeight);
    }
    
    /**
     * Load the Sub Mesh handles from the given iterator at the current position
     */
    bool LoadSubMeshHandles(const Iterator& iterator);
    
    /**
     * Load the Bounds from the given iterator at the current position
     */
    bool LoadBounds(const Iterator& iterator)
    {
      u32 r = stIteratorGetValueArray(iterator.iterator, (u8*) &bounds, (u32) (sizeof(Bounds)), stCalculateStride2(ST_TYPE_FLOAT32, 3, ST_TYPE_FLOAT32, 3));

      return r != sizeof(Bounds);
    }
    
    /**
     * Load the Bind Poses from the given iterator at the current position
     */
    bool LoadBindPoses(const Iterator& iterator)
    {
      return iterator.ReadArray(bindPoses);
    }
    
    /**
     * From a given iterator and schema iterate and read any or all
     * of the mesh properties from the iterator.
     */
    bool IterateOver(const Iterator& iterator, SchemaClass schema);

  };

  typedef Ref<Mesh_t> MeshRef;

}
