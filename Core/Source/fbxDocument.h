// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxDocument.h
 *
 * Represents an FBX Document based upon the Unity Schema
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

  /**
   * Document writer and management class for an Fbx File based
   * upon the Unity Schema
   * @see FbxDocumentWriterImpl_t
   * @see MakeUnitySchema
   */
  class FbxDocumentWriter_t
  {
  public:

    FbxDocumentWriter_t();
    ~FbxDocumentWriter_t();

    /**
     * Initialise a document with the given save path
     */
    bool Initialise(const std::string& path);

    /**
     * Close a document and release any handles. 
     * Does not save!
     */
    void Shutdown();

    /**
     * Save the document now.
     */
    bool Save();

    /**
     * Create a FMaterial Fbx Object based upon a  MaterialRef
     */
    FMaterial CreateMaterial(MaterialRef material);

    /**
     * Create a FMesh non-animated mesh based upon a MeshRef
     */
    FMesh CreateNonAnimatedMesh(MeshRef mesh);

    /**
     * Create a FNode with a FMesh based upon a StandardMeshRenderer
     */
    FNode CreateNodeWithMesh(StandardMeshRendererRef smr, f64 time);

    /**
     * Create a FNode with a FMesh based upon a SkinnedMeshRenderer
     */
    FNode CreateNodeWithSkinnedMesh(SkinnedMeshRendererRef smr, f64 time);

    /**
     * Create or a fetch an FTexture based upon a Texture based Asset
     */
    FTexture CreateOrGetTexture(Asset asset);

    /**
     * Create a transform based upon a TransformRef
     */
    void  CreateTransform(TransformRef transform, f64 time);

    /**
     * Update a transforms position, rotation and/or scale at a given time
     */
    void  UpdateTransform(TransformRef transform, f64 time, bool pos, bool rot, bool sca);

    /**
     * Close a transform and release any handles at the current time
     */
    void  CloseTransform(TransformRef transform);

  private:

    std::string path;
    std::map<std::string, FTexture> sharedTextures;

    FbxDocumentWriterImpl_t* im;

    void _CreateScene();

  };


}
