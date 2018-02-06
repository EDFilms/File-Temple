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
#include "fbxAsset.h"
#include "expIterator.h"

namespace SceneTrackFbx
{
  class FbxExporterT;
  class AssetManager_t;

  struct Iterator : IteratorBase
  {

    static FbxExporterT*      sExporter;
    static AssetManager_t* sAssetManager;

    Asset ReadAsset(AssetType type) const;

    Ref<GameObject_t> ReadGameObjectRef() const;

    Ref<Transform_t> ReadTransformRef() const;

    void ReadTransformRefs(std::vector<Ref<Transform_t>>& values) const;

    Ref<StandardMeshRenderer_t> ReadStandardMeshRendererRef() const;

    Ref<Mesh_t> ReadMeshRef() const;

    Ref<SubMesh_t> ReadSubMeshRef() const;

    void ReadSubMeshRefs(std::vector<Ref<SubMesh_t>>& values) const;

    Ref<Material_t> ReadMaterialRef() const;

    void ReadMaterialRefs(std::vector<Ref<Material_t>>& values) const;

  };

}
