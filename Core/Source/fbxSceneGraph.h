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
