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

#include "fbxExporter.h"
#include "fbxDocument.h"
#include "fbxUnitySchema.h"

namespace SceneTrackFbx
{

  FbxExporterT::FbxExporterT(const char* srcPath_, const char* dstPath_)
    : ExporterBase_t(srcPath_, dstPath_)
  {
    schema = MakeUnitySchema();
    document = std::make_shared<FbxDocumentWriter_t>();
  }

  FbxExporterT::~FbxExporterT()
  {
  }

  void FbxExporterT::Reset()
  {
    ExporterBase_t<Iterator>::Reset();
    Iterator::sExporter = this;

    gameObjects = std::make_shared<RefManagerT<GameObject_t>>();
    transforms  = std::make_shared<RefManagerT<Transform_t>>();
    standardMeshRenderers = std::make_shared<RefManagerT<StandardMeshRenderer_t>>();
    skinnedMeshRenderers = std::make_shared<RefManagerT<SkinnedMeshRenderer_t>>();
    meshes    = std::make_shared<RefManagerT<Mesh_t>>();
    subMeshes = std::make_shared<RefManagerT<SubMesh_t>>();
    materials = std::make_shared<RefManagerT<Material_t>>();
    assets = std::make_shared<AssetManager_t>();

    Iterator::sAssetManager = assets.get();

  }

  s32 FbxExporterT::Start()
  {
    s32 r = ExporterBase_t<Iterator>::Start();

    if (r == 0)
    {
      OpenFbx();
    }

    return r;
  }

  s32 FbxExporterT::Close()
  {
    // Close any animation curves.
    for(auto it : transforms->references)
    {
      auto trRef = it.second;
      auto tr = trRef->Object;

      if (tr == nullptr || tr->bDeleted)
      {
        //STFBX_PRINT_CHATTERF("Not Closing Transform %i, NullPtr or Deleted", it.first);
        continue;
      }

      //STFBX_PRINT_CHATTERF("Closing Transform %i", it.first);
      document->CloseTransform(trRef);
    }


    CloseFbx();
    
    return ExporterBase_t<Iterator>::Close();
  }

  void FbxExporterT::ExportFrame(f64 time, bool isLastFrame)
  {
    STEXP_UNUSED(isLastFrame);

    for(auto it : gameObjects->references)
    {
      auto go = it.second;
      
      if (go == nullptr)
      {
        STFBX_PRINT_ERRORF("Object mesh for %i has been referenced but not loaded!", it.first);
        continue;
      }

      if (go->Object->bResolvedTransform == false && go->Object->transform->Object != nullptr)
      {
        go->Object->transform->Object->gameObject = go;
        go->Object->bResolvedTransform = true;
      }

    }

    // Go through all Meshes, and serialise those first, if haven't been saved.
    for(auto it : meshes->references)
    {
      auto mesh = it.second;

      if (mesh == nullptr)
      {
        STFBX_PRINT_ERRORF("Object mesh for %i has been referenced but not loaded!", it.first);
        continue;
      }

      if (mesh->Object->bExportedMesh == false)
      {
        STFBX_PRINT_CHATTERF("Saving Mesh %i/'%s'", it.first, mesh->Object->name.c_str());
        document->CreateNonAnimatedMesh(mesh);
      }
      
      //TODO: Updating meshes.

    }

    //TODO: Then commit all changes to StandardMeshRenderers, create or update them, etc.
    for(auto it : standardMeshRenderers->references)
    {
      auto smrRef = it.second;
      auto smr = smrRef->Object;

      if (smr == nullptr)
      {
        STFBX_PRINT_ERRORF("StandardMeshRenderer for %i has been referenced but not loaded!", it.first);
        continue;
      }

      if (smr->bCreated == false)
      {
        STFBX_PRINT_CHATTERF("Saving StandardMeshRenderer %i", it.first);
        document->CreateNodeWithMesh(smrRef, time);
      }
    }
    
    for(auto it : skinnedMeshRenderers->references)
    {
      auto smrRef = it.second;
      auto smr = smrRef->Object;

      if (smr == nullptr)
      {
        STFBX_PRINT_ERRORF("SkinnedMeshRenderer for %i has been referenced but not loaded!", it.first);
        continue;
      }

      if (smr->bCreated == false)
      {
        STFBX_PRINT_CHATTERF("Saving SkinnedMeshRenderer %i", it.first);
        document->CreateNodeWithSkinnedMesh(smrRef, time);
      }
    }

    for(auto it : transforms->references)
    {
      auto trRef = it.second;
      auto tr = trRef->Object;

      if (tr == nullptr)
      {
        STFBX_PRINT_ERRORF("Transform for %i has been referenced but not loaded!", it.first);
        continue;
      }

      if (tr->bDeleted)
        continue;

      if (tr->bTransformOutOfDate == true)
      {
        //STFBX_PRINT_CHATTERF("Updating Transform %i", it.first);
        document->UpdateTransform(trRef, time, tr->bPosOutOfDate, tr->bRotOutOfDate, tr->bScaOutOfDate);
      }
    }

  }

  void FbxExporterT::ExportValue(Iterator& it, SchemaClass& klass)
  {
    switch(klass->classType)
    {
      default:
      case SchemaClassType::Unknown:
      {
        STFBX_PRINT_ERRORF("Unknown Exporter for Class. Object = %i, ObjectType = %i, Component = %i, ComponentType = %i", it.objectHandle, klass->classType, it.componentHandle, it.componentType);
      }
      break;
      case SchemaClassType::GameObject:
      {
        GameObjectRef go = FindOrCreateGameObject(it.objectHandle);
        go->Object->IterateOver(it, klass);
      }
      break;
      case SchemaClassType::Transform:
      {
        TransformRef tr = FindOrCreateTransform(it.objectHandle);
        tr->Object->IterateOver(it, klass);
      }
      break;
      case SchemaClassType::StandardMeshRenderer:
      {
        StandardMeshRendererRef smr = FindOrCreateStandardMeshRenderer(it.objectHandle);
        smr->Object->IterateOver(it, klass);
      }
      break;
      case SchemaClassType::SkinnedMeshRenderer:
      {
        SkinnedMeshRendererRef smr = FindOrCreateSkinnedMeshRenderer(it.objectHandle);
        smr->Object->IterateOver(it, klass);
      }
      break;
      case SchemaClassType::Mesh:
      {
        MeshRef mesh = FindOrCreateMesh(it.objectHandle);
        mesh->Object->IterateOver(it, klass);
      }
      break;
      case SchemaClassType::SubMesh:
      {
        SubMeshRef subMesh = FindOrCreateSubMesh(it.objectHandle);
        subMesh->Object->IterateOver(it, klass);
      }
      break;
      case SchemaClassType::Material:
      {
        MaterialRef material = FindOrCreateMaterial(it.objectHandle);
        material ->Object->IterateOver(it, klass);
      }
      break;
    }
  }

  GameObjectRef FbxExporterT::FindOrCreateGameObject(u32 objectId)
  {
    return gameObjects->GetOrCreate(objectId);
  }

  TransformRef FbxExporterT::FindOrCreateTransform(u32 objectId)
  {
    return transforms->GetOrCreate(objectId);
  }

  StandardMeshRendererRef FbxExporterT::FindOrCreateStandardMeshRenderer(u32 objectId)
  {
    return standardMeshRenderers->GetOrCreate(objectId);
  }

  MeshRef FbxExporterT::FindOrCreateMesh(u32 objectId)
  {
    return meshes->GetOrCreate(objectId);
  }

  SubMeshRef FbxExporterT::FindOrCreateSubMesh(u32 objectId)
  {
    return subMeshes->GetOrCreate(objectId);
  }

  MaterialRef FbxExporterT::FindOrCreateMaterial(u32 objectId)
  {
    return materials->GetOrCreate(objectId);
  }

  SkinnedMeshRendererRef FbxExporterT::FindOrCreateSkinnedMeshRenderer(u32 objectId)
  {
    return skinnedMeshRenderers->GetOrCreate(objectId);
  }

  Asset FbxExporterT::FindOrCreateAsset(const std::string& name, AssetType type)
  {
    return assets->GetOrCreate(name, type);
  }

  void FbxExporterT::OpenFbx()
  {
    document->Initialise(dstPath);
  }

  void FbxExporterT::CloseFbx()
  {
    document->Save();
    document->Shutdown();
  }

}
