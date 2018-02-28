// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxSceneGraph.h
 *
 * Collection of classes that correspond to the Unity Scene (or a SceneGraph
 * like it) which matches to the Unity Schema
 *
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "fbxRequired.h"
#include "fbxRef.h"
#include "expMath.h"
#include "fbxMaterial.h"
#include "fbxMesh.h"

namespace SceneTrackFbx
{
  
  class GameObject_t;
  class Transform_t;
  class StandardMeshRenderer_t;
  class SkinnedMeshRenderer_t;

  typedef Ref<GameObject_t>           GameObjectRef;
  typedef Ref<Transform_t>            TransformRef;
  typedef Ref<StandardMeshRenderer_t> StandardMeshRendererRef;
  typedef Ref<SkinnedMeshRenderer_t>  SkinnedMeshRendererRef;

  /**
   * A GameObject is a container class of a Transform.
   */
  class GameObject_t
  {
    public:
      
      u32                                      handle;
      std::string                              name;
      s32                                      layer;
      Ref<Transform_t>                         transform;
      bool                                     isVisible;

      stBool32                                 bDeleted : 1;
      stBool32                                 bResolvedTransform : 1;

    GameObject_t(u32 handle);
    ~GameObject_t();

    bool IterateOver(const Iterator& iterator, SchemaClass schema);
  };

  /**
   * A Transform is represents a node in the SceneGraph which
   * has some spatial information (Transform, Rotation and Scale).
   *
   * Transforms can have child-transforms forming a parent-child
   * hierarchy.
   *
   * It is also indirectly a container class for components;
   *    StandardMeshRenderer
   *    SkinnedMeshRenderer
   *
   * Transforms may also act as bones for skeletons.
   */
  class Transform_t
  {
    public:

      u32                           handle;
      Vector3f                      localPosition;
      Quaternion                    localRotation;
      Vector3f                      localScale;
      TransformRef                  parent;

      Ref<GameObject_t>             gameObject;

      stBool32                      bDeleted : 1;
      stBool32                      bTransformOutOfDate : 1;
      stBool32                      bPosOutOfDate : 1;
      stBool32                      bRotOutOfDate : 1;
      stBool32                      bScaOutOfDate : 1;

      FNode                         fNode;
      FBone                         fBone;
      
      Transform_t(u32 handle);

      void markAsExported();

      bool IterateOver(const Iterator& iterator, SchemaClass schema);

      inline bool hasParent() const
      {
        return parent.get() != nullptr && parent->bHasId;
      }

  };

  /**
   * StandardMeshRenderer is a type of component that
   * provides a single instance of a non-animated Mesh with
   * material information.
   */
  class StandardMeshRenderer_t
  {
  public:

    u32                      handle;
    std::vector<MaterialRef> materials;
    MeshRef                  mesh;
    TransformRef             parent;

    stBool32                 bCreated : 1;
    stBool32                 bDeleted : 1;
    FNode                    fNode;
    StandardMeshRenderer_t(u32 handle);

    bool IterateOver(const Iterator& iterator, SchemaClass schema);

  };

  /**
   * A SkinnedMeshRenderer is a type of component that
   * provides a single instance of a skeletal animated Mesh
   * with material and skeletal information.
   *
   * Skeletonal bones are represented as child transforms.
   */
  class SkinnedMeshRenderer_t
  {
  public:

    u32                      handle;
    std::vector<MaterialRef> materials;
    MeshRef                  mesh;
    TransformRef             parent;
    std::vector<TransformRef> bones;
    TransformRef             boneTransform;

    stBool32                 bCreated : 1;
    stBool32                 bDeleted : 1;
    FNode                    fNode;
    FSkeleton                fSkeleton;

    SkinnedMeshRenderer_t(u32 handle);
    bool IterateOver(const Iterator& iterator, SchemaClass schema);

  };

}
