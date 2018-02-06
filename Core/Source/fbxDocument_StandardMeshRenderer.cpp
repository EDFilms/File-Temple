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

#include "fbxDocument.h"
#define STFBX_INCLUDE_FBX_SDK
#include "fbxDocument_Shared.h"

namespace SceneTrackFbx
{

  FNode FbxDocumentWriter_t::CreateNodeWithMesh(StandardMeshRendererRef smr, f64 time)
  {
    FMesh mesh = smr->Object->mesh->Object->fMesh;

    if (mesh.get() == nullptr)
    {
      STFBX_PRINT_ERROR("[FBX] Not exporting StandardSceneRenderer. No FMesh");
      return nullptr;
    }
    
    FbxNode* node = CreateOrGetNodeTree(MGR, im->animLayer, SCENE->GetRootNode(), smr->Object->parent, time, false);
    node->SetNodeAttribute(mesh->mesh);
    ApplyMaterials(smr->Object->materials, node, this);

    STFBX_PRINT_CHATTERF("-- Attaching Mesh %i to Transform %i", smr->Object->mesh->Handle, smr->Object->parent->Handle);

    smr->Object->bCreated = true;
    return smr->Object->fNode;
  }

}
