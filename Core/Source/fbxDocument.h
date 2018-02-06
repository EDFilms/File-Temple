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
#include "fbxSceneGraph.h"

namespace SceneTrackFbx
{
  struct FbxDocumentWriterImpl_t;

  class FbxDocumentWriter_t
  {
  public:

    FbxDocumentWriter_t();
    ~FbxDocumentWriter_t();

    bool Initialise(const std::string& path);
    void Shutdown();
    bool Save();

    FMaterial CreateMaterial(MaterialRef material);
    FMesh CreateNonAnimatedMesh(MeshRef mesh);
    FNode CreateNodeWithMesh(StandardMeshRendererRef smr, f64 time);
    FNode CreateNodeWithSkinnedMesh(SkinnedMeshRendererRef smr, f64 time);
    FTexture CreateOrGetTexture(Asset asset);
    void  CreateTransform(TransformRef transform, f64 time);
    void  UpdateTransform(TransformRef transform, f64 time, bool pos, bool rot, bool sca);
    void  CloseTransform(TransformRef transform);

  private:

    std::string path;
    std::map<std::string, FTexture> sharedTextures;

    FbxDocumentWriterImpl_t* im;

    void _CreateScene();

  };


}
