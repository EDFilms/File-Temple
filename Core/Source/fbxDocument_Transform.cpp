// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	1.0.0
 */

#include "fbxDocument.h"
#define STFBX_INCLUDE_FBX_SDK
#include "fbxDocument_Shared.h"
#include "fbxSettings.h"

namespace SceneTrackFbx
{

  void FillTransformTrs(TransformRef transform, FbxVector4& traOut, FbxVector4& rotOut, FbxVector4& scaOut)
  {
#if STFBX_SETTINGS_UNITY2FBX == 1
    static Vector3f t, s;
    static Quaternion r;

    auto trs = transform->Object;

    t = trs->localPosition;
    r = trs->localRotation;
    s = trs->localScale; 


    FbxAMatrix m;
    m.SetTQS(
      FbxVector4(-t.x, t.y, t.z),
      FbxQuaternion(r.x, -r.y, -r.z, r.w),
      FbxVector4(s.x, s.y, s.z)
    );

    traOut = m.GetT();
    rotOut = m.GetR();
    scaOut = m.GetS();
#else
    static Vector4f t, s, q;
    static FbxAMatrix m;
    
    auto trs = transform->Object;

    Settings::Trs::sTransform.Transform3<kNodePropertyNodeTranslation>(t, trs->localPosition);
    Settings::Trs::sTransform.Transform4<kNodePropertyNodeRotation>(q, trs->localRotation);
    Settings::Trs::sTransform.Transform3<kNodePropertyNodeScale>(s, trs->localScale);
    
    m.SetTQS(FbxVector4(t.x, t.y, t.z), FbxQuaternion(q.x, q.y, q.z, q.w), FbxVector4(s.x, s.y, s.z));

    traOut = m.GetT();
    rotOut = m.GetR();
    scaOut = m.GetS();

#endif
  }


  FbxNode* CreateOrGetNodeTree(FbxManager* mgr, FbxAnimLayer* animLayer, FbxNode* root, TransformRef transform, f64 time, bool createDummy)
  {
    STFBX_UNUSED(time);

    auto tr = transform->Object;

    // Already has a node.
    if (tr->fNode != nullptr)
      return tr->fNode->node;

    FbxNode* trNode = NULL, *dummyNode = NULL;
    if (tr->gameObject->Object->name.length() > 0)
    {
      trNode = FbxNode::Create(mgr, tr->gameObject->Object->name.c_str());
    }
    else
    {
      std::stringstream ss;
      ss << "Transform_" << transform->Object->handle;

      trNode = FbxNode::Create(mgr, ss.str().c_str());
    }

    if (createDummy)
    {
      std::stringstream dummyName;
      if (tr->gameObject->Object->name.length() > 0)
      {
        dummyName << tr->gameObject->Object->name.c_str() << "_SceneTrackMarker";
      }
      else
      {
        dummyName << "Transform_" << transform->Object->handle << "_SceneTrackMarker";
      }
      
      dummyNode = FbxNode::Create(mgr, dummyName.str().c_str());
    }


    FNode fNode = std::make_shared<FNode_t>();

    STFBX_PRINT_CHATTERF("Creating Transform %i", (u32) transform->Object->handle);

    transform->Object->fNode = fNode;
    fNode->node = trNode;
    fNode->dummy = dummyNode;

    FbxVector4 t, r, s;
    FillTransformTrs(transform, t, r, s);

    trNode->LclTranslation.Set(t);
    fNode->t.Initialise(trNode->LclTranslation, animLayer);

    trNode->LclRotation.Set(r);
    fNode->r.Initialise(trNode->LclRotation, animLayer);

    trNode->LclScaling.Set(s);
    fNode->s.Initialise(trNode->LclScaling, animLayer);

    if (dummyNode != NULL)
    {
      dummyNode->LclTranslation.Set(t);
      dummyNode->LclRotation.Set(r);
      dummyNode->LclScaling.Set(s);
      dummyNode->SetVisibility(false);
      root->AddChild(dummyNode);
    }

    if (Settings::sReferenceFrame == ReferenceFrame::World || tr->hasParent() == false)
    {
      root->AddChild(trNode);
    }
    else
    {
      FbxNode* trParent = CreateOrGetNodeTree(mgr, animLayer, root, tr->parent, time, createDummy);
      trParent->AddChild(trNode);
    }

    return trNode;
  }
  
  void FbxDocumentWriter_t::CreateTransform(TransformRef transform, f64 time)
  { 
    if (transform->Object->fNode == nullptr)
    {
      CreateOrGetNodeTree(MGR, im->animLayer, SCENE->GetRootNode(), transform, time, false);
    }
  }

  static f64 lastUpdateTime = 0.0f;

  void FbxDocumentWriter_t::UpdateTransform(TransformRef transform, f64 time, bool pos, bool rot, bool sca)
  {
    lastUpdateTime = time;

    if (transform->Object->fNode == nullptr)
    {
      CreateOrGetNodeTree(MGR, im->animLayer, SCENE->GetRootNode(), transform, time, false);
      return;
    }

    CreateOrGetNodeTree(MGR, im->animLayer, SCENE->GetRootNode(), transform, time, false);

    transform->Object->markAsExported();

    FbxVector4 t, r, s;
    FillTransformTrs(transform, t, r, s);

    if (pos)
    {
      transform->Object->fNode->t.Update(time, t);
    }

    if (rot)
    {
      transform->Object->fNode->r.Update(time, r);
    }

    if (sca)
    {
      transform->Object->fNode->s.Update(time, s);
    }
    
  }

  void FbxDocumentWriter_t::CloseTransform(TransformRef transform)
  {
    if (transform->Object == nullptr || transform->Object->fNode == nullptr)
    {
      return;
    }
    
    auto fNode = transform->Object->fNode;
    auto node = fNode->node;

    if (fNode->t.GetCurveCount() == 1)
    {
      fNode->t.Shutdown();
    }
    else
    {
      fNode->t.Shutdown();
    }

    if (fNode->r.GetCurveCount() == 1)
    {
      fNode->r.Shutdown();
    }
    else
    {
      fNode->r.Shutdown();
      
      FbxAnimCurveNode *curveNode = node->LclRotation.GetCurveNode(im->animLayer);
      if (curveNode)
      {
        FbxAnimCurveFilterUnroll lUnrollFilter;
        lUnrollFilter.SetForceAutoTangents(true);
        lUnrollFilter.Apply(*curveNode);
      }

    }

    if (fNode->s.GetCurveCount() == 1)
    {
      fNode->s.Shutdown();
    }
    else
    {
      fNode->s.Shutdown();
    }

  }

}
