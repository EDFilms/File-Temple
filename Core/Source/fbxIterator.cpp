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

#include "fbxIterator.h"
#include "fbxExporter.h"

namespace SceneTrackFbx
{

  FbxExporterT* Iterator::sExporter = nullptr;
  AssetManager_t* Iterator::sAssetManager = nullptr;

  Ref<GameObject_t> Iterator::ReadGameObjectRef() const
  {
    u32 handle = stIteratorGetValue_uint32(iterator, 0);
    return sExporter->FindOrCreateGameObject(handle);
  }

  Asset Iterator::ReadAsset(AssetType type) const
  {
    std::string assetName;
    ReadString(assetName);
    
    return sExporter->FindOrCreateAsset(assetName, type);
  }

  Ref<Transform_t> Iterator::ReadTransformRef() const
  {
    u32 handle = stIteratorGetValue_uint32(iterator, 0);
    return sExporter->FindOrCreateTransform(handle);
  }

  void Iterator::ReadTransformRefs(std::vector<Ref<Transform_t>>& values) const
  {
    std::vector<u32> handles;
    ReadArray(handles);
    
    values.reserve(handles.size());
    
    for(auto handle : handles)
    {
      values.push_back(sExporter->FindOrCreateTransform(handle));
    }
  }

  Ref<StandardMeshRenderer_t> Iterator::ReadStandardMeshRendererRef() const
  {
    u32 handle = stIteratorGetValue_uint32(iterator, 0);
    return sExporter->FindOrCreateStandardMeshRenderer(handle);
  }

  Ref<Mesh_t> Iterator::ReadMeshRef() const
  {
    u32 handle = stIteratorGetValue_uint32(iterator, 0);
    return sExporter->FindOrCreateMesh(handle);
  }

  Ref<SubMesh_t> Iterator::ReadSubMeshRef() const
  {
    u32 handle = stIteratorGetValue_uint32(iterator, 0);
    return sExporter->FindOrCreateSubMesh(handle);
  }

  void Iterator::ReadSubMeshRefs(std::vector<Ref<SubMesh_t>>& values) const
  {
    std::vector<u32> handles;
    ReadArray(handles);

    values.reserve(handles.size());

    for(auto handle : handles)
    {
      values.push_back(sExporter->FindOrCreateSubMesh(handle));
    }
  }

  Ref<Material_t> Iterator::ReadMaterialRef() const
  {
    u32 handle = stIteratorGetValue_uint32(iterator, 0);
    return sExporter->FindOrCreateMaterial(handle);
  }

  void Iterator::ReadMaterialRefs(std::vector<Ref<Material_t>>& values) const
  {
    std::vector<u32> handles;
    ReadArray(handles);

    values.reserve(handles.size());

    for(auto handle : handles)
    {
      values.push_back(sExporter->FindOrCreateMaterial(handle));
    }
  }


}
