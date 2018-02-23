// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * FbxExporter.h
 *
 * FbxExporter implementation of the Shared ExporterBase_t class
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
  
  /**
   * FbxExporter implementation of the ExporterBase_t class
   */
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

    /**
     * Reset the exporter, and release any existing handles or memory
     */
    void Reset();

    /**
     * Start the export process
     */
    s32  Start();

    /**
     * Close the exporter process, and release the FbxSdk Library
     */
    s32  Close() override;

    /**
     * Export and write the last frame to disk (if needed), and increase the frame counter
     */
    void ExportFrame(f64 frameTime, bool isLastFrame) override;

    /**
     * Export a value from a change of a Object's component
     */
    void ExportValue(Iterator& it, SchemaClass& klass) override;

    /**
     * Find or create a GameObject
     */
    GameObjectRef    FindOrCreateGameObject(u32 objectId);

    /**
     * Find or create a Transform
     */
    TransformRef    FindOrCreateTransform(u32 objectId);

    /**
     * Find or create a Standard Mesh Renderer
     */
    StandardMeshRendererRef    FindOrCreateStandardMeshRenderer(u32 objectId);

    /**
     * Find or create a Mesh
     */
    MeshRef    FindOrCreateMesh(u32 objectId);

    /**
     * Find or create a SubMesh
     */
    SubMeshRef FindOrCreateSubMesh(u32 objectId);

    /**
     * Find or create a Material
     */
    MaterialRef FindOrCreateMaterial(u32 objectId);

    /**
     * Find or create a Skinned Mesh Renderer
     */
    SkinnedMeshRendererRef FindOrCreateSkinnedMeshRenderer(u32 objectId);

    /**
     * Find or create an Asset according to it's name and AssetType
     */
    Asset FindOrCreateAsset(const std::string& name, AssetType type);

  protected:

    void OpenFbx();

    void CloseFbx();

  };

  typedef std::shared_ptr<FbxExporterT> Exporter;

}
