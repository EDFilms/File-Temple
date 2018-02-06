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
#include "fbxMesh.h"
#include "fbxMaterial.h"
#include "fbxSchema.h"
#include "fbxRef.h"
#include "fbxSceneGraph.h"
#include "fbxAsset.h"
#include "expExporterBase.h"

namespace SceneTrackFbx
{

  class FbxExporterT : public ExporterBase_t<Iterator>
  {
    
  public:

    RefManager<GameObject_t>     gameObjects;
    RefManager<Transform_t>     transforms;
    RefManager<StandardMeshRenderer_t>     standardMeshRenderers;
    RefManager<SkinnedMeshRenderer_t>      skinnedMeshRenderers;
    RefManager<Mesh_t>     meshes;
    RefManager<SubMesh_t>  subMeshes;
    RefManager<Material_t> materials;
    AssetManager           assets;

    FbxDocumentWriter      document;

    FbxExporterT(const char* srcPath_, const char* dstPath_);

    ~FbxExporterT();


    void Reset();
    s32  Start();
    s32  Close() override;

    void ExportFrame(f64 frameTime, bool isLastFrame) override;

    void ExportValue(Iterator& it, SchemaClass& klass) override;

    GameObjectRef    FindOrCreateGameObject(u32 objectId);

    TransformRef    FindOrCreateTransform(u32 objectId);

    StandardMeshRendererRef    FindOrCreateStandardMeshRenderer(u32 objectId);

    MeshRef    FindOrCreateMesh(u32 objectId);

    SubMeshRef FindOrCreateSubMesh(u32 objectId);

    MaterialRef FindOrCreateMaterial(u32 objectId);

    SkinnedMeshRendererRef FindOrCreateSkinnedMeshRenderer(u32 objectId);

    Asset FindOrCreateAsset(const std::string& name, AssetType type);

  protected:

    void OpenFbx();

    void CloseFbx();

  };

  typedef std::shared_ptr<FbxExporterT> Exporter;

}
