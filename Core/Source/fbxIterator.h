// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxIterator.h
 *
 * Fbx implementation of the IteratorBase class
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

  /**
   * Iterator implementation of the IteratorBase for the Unity Schema
   */
  struct Iterator : IteratorBase
  {

    static FbxExporterT*      sExporter;
    static AssetManager_t* sAssetManager;

    /**
     * Read an Asset from the current iterator position
     */
    Asset ReadAsset(AssetType type) const;

    /**
     * Read a GameObject from the current iterator position
     */
    Ref<GameObject_t> ReadGameObjectRef() const;
    
    /**
     * Read a Transform from the current iterator position
     */
    Ref<Transform_t> ReadTransformRef() const;

    /**
     * Read many Transforms into a std::vector from the current position
     */
    void ReadTransformRefs(std::vector<Ref<Transform_t>>& values) const;

    /**
     * Read a Standard Mesh Renderer from the current position
     */
    Ref<StandardMeshRenderer_t> ReadStandardMeshRendererRef() const;

    /**
     * Read a Mesh from the current position
     */
    Ref<Mesh_t> ReadMeshRef() const;

    /**
     * Read a SubMesh from the current position
     */
    Ref<SubMesh_t> ReadSubMeshRef() const;

    /**
     * Read many SubMeshes into a std::vector from the current position
     */
    void ReadSubMeshRefs(std::vector<Ref<SubMesh_t>>& values) const;

    /**
     * Read a Material from the current position
     */
    Ref<Material_t> ReadMaterialRef() const;
    
    /**
     * Read many Materials into a std::vector from the current position
     */
    void ReadMaterialRefs(std::vector<Ref<Material_t>>& values) const;

  };

}
