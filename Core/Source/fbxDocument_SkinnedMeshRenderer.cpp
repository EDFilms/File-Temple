// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxDocument_Material.cpp
 *
 * Skinned Mesh Renderer functions for FbxDocument
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


  static void CreateBoneTree(FbxManager* mgr, FbxScene* scene, FbxAnimLayer* animLayer, FbxNode* boneRoot, SkinnedMeshRendererRef smr, f64 time)
  {
    std::vector<TransformRef>& bones = smr->Object->bones;

    std::stringstream ss;
    FbxSkeleton* skeleton;

//    ss << "Bone_Skeleton_Root";
//    skeleton = FbxSkeleton::Create(scene, ss.str().c_str());
//    boneRoot->SetNodeAttribute(skeleton);
//    skeleton->SetSkeletonType(FbxSkeleton::eRoot);

    for(auto boneRef : bones)
    {
      auto transform = boneRef->Object;
      u32 boneId = transform->handle;

      FbxNode* node = CreateOrGetNodeTree(mgr, animLayer, boneRoot, boneRef, time, false);

      ss.clear();
      ss << "Bone_Skeleton_" << boneId;
      skeleton = FbxSkeleton::Create(scene, ss.str().c_str());
      node->SetNodeAttribute(skeleton);

      if (transform->hasParent())
      {
        skeleton->SetSkeletonType(FbxSkeleton::eLimb);
        
        //STFBX_PRINT_INFOF("Bone Parent (Yes) %s", node->GetName());
      }
      else
      {
        //STFBX_PRINT_INFOF("Bone Parent (No) %s", node->GetName());

        skeleton->SetSkeletonType(FbxSkeleton::eRoot);
      }
      
      // TODO: Multiple skeletons sharing same bones.
      
      if (transform->fBone == nullptr)
      {
        FBone fBone = std::make_shared<FBone_t>();
        transform->fBone = fBone;
        transform->fBone->skeleton = skeleton;
      }
      
    }

  }
  
  void PrintList(NodeList& list)
  {
    for(int i=0;i < list.Size();i++)
    {
      FbxNode* node = list.GetAt(i);
      if (node == NULL)
      {
        STFBX_PRINT_ERRORF("-- [%i] Null", i);
      }
      else
      {
        STFBX_PRINT_ERRORF("-- [%i] %s", i, node->GetName());
      }
    }
  }

  static void MapBoneWeights(FbxManager* mgr, FbxScene* scene, std::vector<Vector4ub>& boneIndexes, std::vector<Vector4f>& boneWeights, std::vector<Matrix44f>& bindPoses, SkinnedMeshRendererRef smrRef, f64 time, FbxMesh* mesh, FbxNode* rootNode)
  {
    STFBX_UNUSED(bindPoses);

    auto smr = smrRef->Object;
    std::vector<FbxCluster*> clusters;
    clusters.reserve(smr->bones.size());

    for(u32 i=0;i < smr->bones.size();i++)
    {
      auto bn = smr->bones[i]->Object;

      FbxCluster* cluster = FbxCluster::Create(mgr, bn->fNode->node->GetName());
      cluster->SetLink(bn->fNode->node);
      cluster->SetLinkMode(FbxCluster::eTotalOne);

      clusters.push_back(cluster);
    }

    for(u32 vertexNum=0;vertexNum < boneIndexes.size();vertexNum++)
    {
      Vector4ub index4  = boneIndexes[vertexNum];
      Vector4f  weight4 = boneWeights[vertexNum];

      for(u32 j=0;j < 4;j++)
      {
        f32 weight = weight4.element(j);
        if (weight > 0.0f)
        {
          size_t index = index4.element(j);
          FbxCluster* cluster = clusters[index];
          cluster->AddControlPointIndex(vertexNum, weight);
        }
      }
    }

    FbxTime fTime;
    fTime.SetSecondDouble(time);

    FbxSkin* skin = FbxSkin::Create(scene, mesh->GetName());

    for(u32 i=0;i < clusters.size();i++)
    {
      FbxCluster* cluster = clusters[i];

#if 0
      FbxAMatrix m = smr->parent->Object->fNode->node->EvaluateGlobalTransform(/*fTime*/);

      cluster->SetTransformMatrix(m);

      FbxNode* boneNode = smr->bones[i]->Object->fNode->node;
      m = boneNode->EvaluateGlobalTransform(/*fTime*/);
      cluster->SetTransformLinkMatrix(m);
#else
      FbxNode* node = smr->parent->Object->fNode->node;
      FbxAMatrix m = node->EvaluateGlobalTransform(/*fTime*/);

      cluster->SetTransformMatrix(m);

      FbxNode* boneNode = smr->bones[i]->Object->fNode->node;
      m = boneNode->EvaluateGlobalTransform(/*fTime*/);
      cluster->SetTransformLinkMatrix(m);
#endif

      skin->AddCluster(cluster);
    }
    
    mesh->AddDeformer(skin);

#if 0
    FbxPose* bindPose = FbxPose::Create(scene, mesh->GetName());
    bindPose->SetIsBindPose(true);
    
    for(u32 i=0;i < clusters.size();i++)
    {
      FbxNode* clusterNode = smr->bones[i]->Object->fNode->node;

      Matrix44f bindPose44f = bindPoses[i];
      FbxMatrix bindMatrix(
        bindPose44f.m[0],  bindPose44f.m[1],  bindPose44f.m[2],  bindPose44f.m[3], 
        bindPose44f.m[4],  bindPose44f.m[5],  bindPose44f.m[6],  bindPose44f.m[7], 
        bindPose44f.m[8],  bindPose44f.m[9],  bindPose44f.m[10], bindPose44f.m[11], 
        -bindPose44f.m[12], bindPose44f.m[13], bindPose44f.m[14], bindPose44f.m[15]
      );

      STFBX_PRINT_INFOF("Bind Pose for %s", clusterNode->GetName());
      STFBX_PRINT_INFOF("%f %f %f %f", bindPose44f.m[0], bindPose44f.m[1], bindPose44f.m[2], bindPose44f.m[3]); 
      STFBX_PRINT_INFOF("%f %f %f %f", bindPose44f.m[4], bindPose44f.m[5], bindPose44f.m[6], bindPose44f.m[7]); 
      STFBX_PRINT_INFOF("%f %f %f %f", bindPose44f.m[8], bindPose44f.m[9], bindPose44f.m[10], bindPose44f.m[11]); 
      STFBX_PRINT_INFOF("%f %f %f %f", -bindPose44f.m[12], bindPose44f.m[13], bindPose44f.m[14], bindPose44f.m[15]); 
      
      bindPose->Add(clusterNode, bindMatrix, true);
    }

    if (bindPose->IsValidBindPose(rootNode) == false)
    {
      STFBX_PRINT_ERRORF("Bind Pose for Mesh/Node is incorrect for the following reasons (Mesh: %s, Node/Transform: %s):", mesh->GetName(), rootNode->GetName()); 
      
      NodeList missingAncestors;
      NodeList missingDeformers;
      NodeList missingDeformerAncestors;
      NodeList wrongMatrices;
    
      bindPose->IsValidBindPoseVerbose(rootNode, missingAncestors, missingDeformers, missingDeformerAncestors, wrongMatrices);
        
      if (missingAncestors.Size() > 0)
      {
        STFBX_PRINT_ERRORF("- %i Missing Parent Bone (Ancestors)", missingAncestors.Size());
        PrintList(missingAncestors);
      }

      if (missingDeformers.Size() > 0)
      {
        STFBX_PRINT_ERRORF("- %i Missing Deformers:", missingDeformers.Size());
        PrintList(missingDeformers);
      }

      if (missingDeformerAncestors.Size() > 0)
      {
        STFBX_PRINT_ERRORF("- %i Missing Deformers Ancestors:-", missingDeformerAncestors.Size());
        PrintList(missingDeformerAncestors);
      }

      if (wrongMatrices.Size() > 0)
      {
        STFBX_PRINT_ERRORF("- %i Incorrect BindPose matrices:-", wrongMatrices.Size());
        PrintList(wrongMatrices);
      }

    }

    scene->AddPose(bindPose);

#else
    STFBX_UNUSED(rootNode);
#endif

  }

  FNode FbxDocumentWriter_t::CreateNodeWithSkinnedMesh(SkinnedMeshRendererRef smrRef, f64 time)
  {
    auto smr = smrRef->Object;
    auto mesh = smr->mesh->Object;
    FMesh fMesh = mesh->fMesh;

    if (fMesh.get() == nullptr)
    {
      STFBX_PRINT_ERROR("[FBX] Not exporting SkinnedMeshRenderer. No FMesh");
      return nullptr;
    }

    FbxMesh* clonedMesh = (FbxMesh*) fMesh->mesh->Clone();


#if STFBX_CONFIG_DUMMY_PRECISION_NODE_ON_SKINNED_MESH_RENDERERS == 1
    bool createDummy = true;
#else
    bool createDummy = false;
#endif

    FbxNode* node = CreateOrGetNodeTree(MGR, im->animLayer, SCENE->GetRootNode(), smr->parent, time, createDummy);
    node->SetNodeAttribute(clonedMesh);
    ApplyMaterials(smr->materials, node, this); 
    
    TransformRef root = smr->boneTransform;

    if (smr->bones.size() > 0 && root != nullptr)
    {
      FbxNode* rootNode;
      
      if (root != nullptr)
      {
        rootNode = CreateOrGetNodeTree(MGR, im->animLayer, node /*SCENE->GetRootNode()*/, root, time, false);
      }  
      else
      { 
        rootNode =  CreateOrGetNodeTree(MGR, im->animLayer, SCENE->GetRootNode(), root, time, false);
      }

      CreateBoneTree(MGR, SCENE, im->animLayer, rootNode /* usually/was rootNode */, smrRef, time);
      MapBoneWeights(MGR, SCENE, mesh->boneIndex, mesh->boneWeight, mesh->bindPoses, smrRef, time, clonedMesh, rootNode);

    }
    else
    {
      if (smr->bones.size() == 0)
      {
        STFBX_PRINT_ERRORF("No Skeleton was created for %s because there was no bones assigned to it!", node->GetName());
      }
      if (smr->boneTransform == nullptr)
      {
        STFBX_PRINT_ERRORF("No Skeleton was created for %s because the root bone was not specified!", node->GetName());
      }
    }
    smr->bCreated = true;
    return smr->fNode;
  }

}
