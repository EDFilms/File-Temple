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

#ifdef STFBX_INCLUDE_FBX_SDK

#pragma once

#include "fbxDocument.h"
#include "fbxSettings.h"
#include <fbxsdk.h>
#include <sstream>

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(MGR->GetIOSettings()))
#endif

#define MGR   (im->mgr)
#define SCENE (im->scene)

namespace SceneTrackFbx
{

  inline FbxVector2 ToFbxVector2(const Vector2f& v)
  {
    return FbxVector2(v.x, v.y);
  }

  inline FbxVector4 ToFbxVector4(const Vector3f& v)
  {
    return FbxVector4(v.x, v.y, v.z, 1.0);
  }

  inline FbxVector4 ToFbxVector4(f32 x, f32 y, f32 z)
  {
    return FbxVector4(x, y, z, 1.0);
  }

  inline FbxVector4 ToFbxEulerZXY(const Vector3f& v)
  {
    return FbxVector4(v.z, v.x, v.y, 1.0);
  }

  inline FbxVector4 ToFbxVector4(const Vector4f& v)
  {
    return FbxVector4(v.x, v.y, v.z, v.w);
  }

  inline FbxColor ToFbxColor(const Vector4f& v)
  {
    return FbxColor(v.x, v.y, v.z, v.w);
  }

  inline FbxQuaternion ToFbxQuaternion(const Vector4f& v)
  {
    return FbxQuaternion(v.x, v.y, v.z, v.w);
  }

  inline FbxDouble3 ToFbxDouble3(const Vector3f& v)
  {
    return FbxDouble3(v.x, v.y, v.z);
  }

  inline FbxAMatrix ToFbxAMatrix(const Matrix44f& m)
  {
    FbxAMatrix s;
    s.SetRow(0, FbxVector4(m.m[0],  m.m[1],  m.m[2],  m.m[3]));
    s.SetRow(1, FbxVector4(m.m[4],  m.m[5],  m.m[6],  m.m[7]));
    s.SetRow(2, FbxVector4(m.m[8],  m.m[9],  m.m[10], m.m[11]));
    s.SetRow(3, FbxVector4(m.m[12], m.m[13], m.m[14], m.m[15]));
    //return s.Transpose();
    return s;
  }

  inline FbxTime::EMode TimeRateToFbxTimeEMode(FbxFrameRate r)
  {
    switch(r)
    {
      default:
      case FbxFrameRate::Default:   return FbxTime::EMode::eDefaultMode;
      case FbxFrameRate::Custom:    return FbxTime::EMode::eCustom;
      case FbxFrameRate::Fps23_976: return FbxTime::EMode::eFilmFullFrame;
      case FbxFrameRate::Fps24:     return FbxTime::EMode::eFrames24;
      case FbxFrameRate::Fps30:     return FbxTime::EMode::eFrames30;
      case FbxFrameRate::Fps30Drop: return FbxTime::EMode::eFrames30Drop;
      case FbxFrameRate::Fps48:     return FbxTime::EMode::eFrames48;
      case FbxFrameRate::Fps50:     return FbxTime::EMode::eFrames50;
      case FbxFrameRate::Fps59_94:  return FbxTime::EMode::eFrames59dot94;
      case FbxFrameRate::Fps60:     return FbxTime::EMode::eFrames60;
      case FbxFrameRate::Fps72:     return FbxTime::EMode::eFrames72;
      case FbxFrameRate::Fps96:     return FbxTime::EMode::eFrames96;
      case FbxFrameRate::Fps100:    return FbxTime::EMode::eFrames100;
      case FbxFrameRate::Fps120:    return FbxTime::EMode::eFrames120;
      case FbxFrameRate::Fps1000:   return FbxTime::EMode::eFrames1000;
      case FbxFrameRate::Pal:       return FbxTime::EMode::ePAL;
      case FbxFrameRate::Ntsc:      return FbxTime::EMode::eNTSCFullFrame;
      case FbxFrameRate::NtscDrop:  return FbxTime::EMode::eNTSCDropFrame;
    }
  }

  struct FbxDocumentWriterImpl_t
  {
    FbxDocumentWriterImpl_t()
      : mgr(nullptr), scene(nullptr), animStack(nullptr), animLayer(nullptr)
    {
    }

    FbxManager*   mgr;
    FbxScene*     scene;
    FbxAnimStack* animStack;
    FbxAnimLayer* animLayer;
  };


  enum FCurveType
  {
    TX,
    TY,
    TZ,
    RX,
    RY,
    RZ,
    SX,
    SY,
    SZ
  };

  template<int Type>
  class FCurveInfo {};

  template<> class FCurveInfo<TX> { public: static const char* Name() { return "X"; } };
  template<> class FCurveInfo<TY> { public: static const char* Name() { return "Y"; } };
  template<> class FCurveInfo<TZ> { public: static const char* Name() { return "Z"; } };
  template<> class FCurveInfo<RX> { public: static const char* Name() { return "X"; } };
  template<> class FCurveInfo<RY> { public: static const char* Name() { return "Y"; } };
  template<> class FCurveInfo<RZ> { public: static const char* Name() { return "Z"; } };
  template<> class FCurveInfo<SX> { public: static const char* Name() { return "X"; } };
  template<> class FCurveInfo<SY> { public: static const char* Name() { return "Y"; } };
  template<> class FCurveInfo<SZ> { public: static const char* Name() { return "Z"; } };

  template<int Type>
  class FCurve
  {
  public:
    ::FbxAnimCurve* curve;
    bool            isOpen;

    void Initialise(FbxPropertyT<FbxDouble3>& prop, FbxAnimLayer* animLayer)
    {
      curve = prop.GetCurve(animLayer, FCurveInfo<Type>::Name(), true);
      curve->KeyModifyBegin();
      isOpen = true;
    }

    void Shutdown()
    {
      if (isOpen)
      {
        curve->KeyModifyEnd();
        isOpen = false;
      }
    }

    void Update(f64 time, f32 value)
    {
      FbxTime t;
      t.SetSecondDouble(time);
      auto keyIndex = curve->KeyAdd(t);
      curve->KeySet(keyIndex, t, value, FbxAnimCurveDef::eInterpolationLinear);
    }

  };

  template<int XType>
  class FCurve3
  {
  public:
    FCurve<XType + 0> x;
    FCurve<XType + 1> y;
    FCurve<XType + 2> z;

    void Initialise(FbxPropertyT<FbxDouble3>& prop, FbxAnimLayer* animLayer)
    {
      x.Initialise(prop, animLayer);
      y.Initialise(prop, animLayer);
      z.Initialise(prop, animLayer);
    }

    void Shutdown()
    {
      x.Shutdown();
      y.Shutdown();
      z.Shutdown();
    }

    void Update(f64 time, const FbxVector4& value)
    {
      x.Update(time, (f32) value.mData[0]);
      y.Update(time, (f32) value.mData[1]);
      z.Update(time, (f32) value.mData[2]);
    }

    int GetCurveCount() const
    {
      return x.curve->KeyGetCount();
    }

  };

  class FNode_t
  {
  public:
    ::FbxNode*  node;
    ::FbxNode*  dummy;

    FCurve3<TX> t;
    FCurve3<RX> r;
    FCurve3<SX> s;
    
    stBool32    bDeleted              : 1;
  };

  class FMesh_t
  {
  public:
    ::FbxMesh* mesh;  // Managed by Fbx Sdk
    stBool32   bCreated : 1;
    stBool32   bDeleted : 1;

    FMesh_t()
      : mesh(nullptr), bCreated(0), bDeleted(0)
    {
    }
  };

  class FSkeleton_t
  {
  public:
    ::FbxSkeleton* skeleton;
    stBool32   bCreated : 1;
    stBool32   bDeleted : 1;

    FSkeleton_t()
      : skeleton(nullptr), bCreated(0), bDeleted(0)
    {
    }
  };

  class FBone_t
  {
  public:
    FbxSkeleton*             skeleton;

    std::shared_ptr<FBone_t> parent;
    stBool32                 bCreated : 1;
    stBool32                 bDeleted : 1;

    FCurve3<TX>              t;
    FCurve3<RX>              r;
    FCurve3<SX>              s;

    FBone_t()
      : 
        skeleton(nullptr),
        bCreated(0), 
        bDeleted(0), 
        parent(nullptr)
    {
    }

  };

  class FMaterial_t
  {
  public:
    FbxSurfacePhong* material;


    stBool32                 bCreated : 1;
    stBool32                 bDeleted : 1;

    FMaterial_t()
      :
        material(nullptr),
        bCreated(0), 
        bDeleted(0)
    {
    }
  
  };

  class FTexture_t
  {
  public:

    FbxFileTexture* texture;

    stBool32                 bCreated : 1;
    stBool32                 bDeleted : 1;

    FTexture_t()
      :
      texture(nullptr),
      bCreated(0), 
      bDeleted(0)
    {
    }
  };


  inline FbxEuler::EOrder ToRotationOrder(AxisOrder axisOrder)
  {
    switch(axisOrder)
    {
      default:
      case AxisOrder::XYZ: return FbxEuler::EOrder::eOrderXYZ;
      case AxisOrder::XZY: return FbxEuler::EOrder::eOrderXZY;
      case AxisOrder::YXZ: return FbxEuler::EOrder::eOrderYXZ;
      case AxisOrder::YZX: return FbxEuler::EOrder::eOrderYZX;
      case AxisOrder::ZXY: return FbxEuler::EOrder::eOrderZXY;
      case AxisOrder::ZYX: return FbxEuler::EOrder::eOrderZYX;
    }
  }

  FbxNode* CreateOrGetNodeTree(FbxManager* mgr, FbxAnimLayer* animLayer, FbxNode* root, TransformRef transform, f64 time, bool createGlobalDummy);

  void  ApplyMaterials(std::vector<MaterialRef> materials, FbxNode* node, FbxDocumentWriter_t* doc);
}

#endif
