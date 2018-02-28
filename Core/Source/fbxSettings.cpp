// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxSettings.cpp
 *
 * A collection of user settings and enumerations
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "fbxSettings.h"

namespace SceneTrackFbx
{
  const char* sFbxFileVersionStr[] = {
    "FBX_53_MB55",
    "FBX_60_MB60",
    "FBX_200508_MB70",
    "FBX_200602_MB75",
    "FBX_200608",
    "FBX_200611",
    "FBX_200900",
    "FBX_200900v7",
    "FBX_201000",
    "FBX_201100",
    "FBX_201200",
    "FBX_201300",
    "FBX_201400",
    "FBX_201600"
  };

  const char* sAxisOrderStr[] = {
    "XYZ",
    "XZY",
    "YXZ",
    "YZX",
    "ZXY",
    "ZYX"
  };

  const char* sFbxFrameRateStr[] = {
    "Default",
    "Custom",
    "23.976 FPS",
    "24 FPS",
    "30 FPS",
    "30 FPS",
    "48 FPS",
    "50 FPS",
    "59.94 FPS",
    "60 FPS",
    "72 FPS",
    "96 FPS",
    "100 FPS",
    "120 FPS",
    "1000 FPS",
    "PAL",
    "NTSC",
    "NTSC (Drop)"
  };

  namespace Settings
  {
    FbxFileVersion           sFbxFileVersion = FbxFileVersion::LATEST;
    ReferenceFrame           sReferenceFrame = ReferenceFrame::Local;
    std::set<std::string>    sAssetSearchPaths;
    FbxFrameRate             sFbxFrameRate = FbxFrameRate::Default;
    f64                      sFbxFrameRateCustomValue = 0.0;

    namespace Trs
    {
      ::SceneTrackFbx::Trs::TrsSystem sTransform = ::SceneTrackFbx::Trs::TrsSystem();
      ::SceneTrackFbx::Trs::TrsSystem sMesh      = ::SceneTrackFbx::Trs::TrsSystem();
      AxisOrder                       sRotationAxisOrder;
    }

    TriangleWindingOrder sTriangleWinding = TriangleWindingOrder::AsIs;

    void PrintSettings()
    {
      STFBX_PRINT_INFOF("Settings.FileFormat = %s",     sFbxFileVersionStr[(size_t) sFbxFileVersion]);
      STFBX_PRINT_INFOF("Settings.ReferenceFrame = %s", sReferenceFrame == ReferenceFrame::Local ? "Local" : "World");
      
      if (sFbxFrameRate == FbxFrameRate::Custom)
      {
        STFBX_PRINT_INFOF("Settings.FrameRate = %f (Custom)", sFbxFrameRateCustomValue);
      }
      else
      {
        STFBX_PRINT_INFOF("Settings.FrameRate = %s", sFbxFrameRateStr[(size_t) sFbxFrameRate]);
      }
      
      switch(sTriangleWinding)
      {
        case TriangleWindingOrder::AsIs:     STFBX_PRINT_INFO("Settings.ReverseTriangleWinding = AsIs");     break;
        case TriangleWindingOrder::Reverse:  STFBX_PRINT_INFO("Settings.ReverseTriangleWinding = Reverse");  break;
        case TriangleWindingOrder::Both:     STFBX_PRINT_INFO("Settings.ReverseTriangleWinding = Both");     break;
      }

      Settings::Trs::sTransform.Print(Node::Transform);
      Settings::Trs::sMesh.Print(Node::Mesh);

      u32 idx = 0;
      STFBX_UNUSED(idx);
      for(auto path : Settings::sAssetSearchPaths)
      {
        STFBX_PRINT_INFOF("Settings.AssetSearchPath[%i] = %s", idx++, path.c_str());
      }

    }

  }
}
