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
#include "expMath.h"
#include "expTrs.h"

// Transform will ignore User Settings and use a Fbx2Unity version that is hardcoded
#define STFBX_SETTINGS_UNITY2FBX 0

namespace SceneTrackFbx
{
  enum class FbxFileVersion
  {
    STFBX_PUBLIC_ENUM(Named="FBX_53_MB55",        Value=0, For="FbxFileVersion")
    FBX_53_MB55 = 0,
    STFBX_PUBLIC_ENUM(Named="FBX_60_MB60",        Value=1, For="FbxFileVersion")
    FBX_60_MB60 = 1,
    STFBX_PUBLIC_ENUM(Named="FBX_200508_MB70",    Value=2, For="FbxFileVersion")
    FBX_200508_MB70 = 2,
    STFBX_PUBLIC_ENUM(Named="FBX_200602_MB75",    Value=3, For="FbxFileVersion")
    FBX_200602_MB75 = 3,
    STFBX_PUBLIC_ENUM(Named="FBX_200608",         Value=4, For="FbxFileVersion")
    FBX_200608 = 4,
    STFBX_PUBLIC_ENUM(Named="FBX_200611",         Value=5, For="FbxFileVersion")
    FBX_200611 = 5,
    STFBX_PUBLIC_ENUM(Named="FBX_200900",         Value=6, For="FbxFileVersion")
    FBX_200900 = 6,
    STFBX_PUBLIC_ENUM(Named="FBX_200900v7",       Value=7, For="FbxFileVersion")
    FBX_200900v7 = 7,
    STFBX_PUBLIC_ENUM(Named="FBX_201000",         Value=8, For="FbxFileVersion")
    FBX_201000 = 8,
    STFBX_PUBLIC_ENUM(Named="FBX_201100",         Value=9, For="FbxFileVersion")
    FBX_201100 = 9,
    STFBX_PUBLIC_ENUM(Named="FBX_201200",         Value=10, For="FbxFileVersion")
    FBX_201200 = 10,
    STFBX_PUBLIC_ENUM(Named="FBX_201300",         Value=11, For="FbxFileVersion")
    FBX_201300 = 11,
    STFBX_PUBLIC_ENUM(Named="FBX_201400",         Value=12, For="FbxFileVersion")
    FBX_201400 = 12,
    STFBX_PUBLIC_ENUM(Named="FBX_201600",         Value=13, For="FbxFileVersion")
    FBX_201600 = 13,

    COUNT,
    LATEST = FBX_201600
  };

  enum class FbxFrameRate
  {
    STFBX_PUBLIC_ENUM(Named="DEFAULT",        Value=0, For="Fps", Text="Default Frame Rate")
    Default = 0,
    STFBX_PUBLIC_ENUM(Named="CUSTOM",         Value=1, For="Fps", Text="Custom")
    Custom  = 1,
    STFBX_PUBLIC_ENUM(Named="FPS_23_976",     Value=2, For="Fps", Text="~23.976 FPS")
    Fps23_976 = 2,
    STFBX_PUBLIC_ENUM(Named="FPS_24",         Value=3, For="Fps", Text="24 FPS")
    Fps24 = 3,
    STFBX_PUBLIC_ENUM(Named="FPS_30",         Value=4, For="Fps", Text="30 FPS")
    Fps30 = 4,
    STFBX_PUBLIC_ENUM(Named="FPS_30_DROP",    Value=5, For="Fps", Text="30 FPS Drop")
    Fps30Drop = 5,
    STFBX_PUBLIC_ENUM(Named="FPS_48",         Value=6, For="Fps", Text="48 FPS")
    Fps48 = 6,
    STFBX_PUBLIC_ENUM(Named="FPS_50",         Value=7, For="Fps", Text="50 FPS")
    Fps50 = 7,
    STFBX_PUBLIC_ENUM(Named="FPS_59_94",      Value=8, For="Fps", Text="~59.94 FPS")
    Fps59_94 = 8,
    STFBX_PUBLIC_ENUM(Named="FPS_60",         Value=9, For="Fps", Text="60 FPS")
    Fps60 = 9,
    STFBX_PUBLIC_ENUM(Named="FPS_72",         Value=10, For="Fps", Text="72 FPS")
    Fps72 = 10,
    STFBX_PUBLIC_ENUM(Named="FPS_96",         Value=11, For="Fps", Text="72 FPS")
    Fps96 = 11,
    STFBX_PUBLIC_ENUM(Named="FPS_100",        Value=12, For="Fps", Text="100 FPS")
    Fps100 = 12,
    STFBX_PUBLIC_ENUM(Named="FPS_120",        Value=13, For="Fps", Text="120 FPS")
    Fps120 = 13,
    STFBX_PUBLIC_ENUM(Named="FPS_1000",       Value=14, For="Fps", Text="1000 FPS")
    Fps1000 = 14,
    STFBX_PUBLIC_ENUM(Named="FPS_PAL",        Value=15, For="Fps", Text="25 FPS PAL/SECAM")
    Pal = 15,
    STFBX_PUBLIC_ENUM(Named="FPS_NTSC",       Value=16, For="Fps", Text="~29.97 NTSC")
    Ntsc = 16,
    STFBX_PUBLIC_ENUM(Named="FPS_NTSC_DROP",  Value=17, For="Fps", Text="~29.97 NTSC Drop")
    NtscDrop = 17,
    COUNT
  };

  enum class ReferenceFrame
  {
    STFBX_PUBLIC_ENUM(Named="Local",        Value=0, For="ReferenceFrame")
    Local = 0,
    STFBX_PUBLIC_ENUM(Named="World",        Value=1, For="ReferenceFrame")
    World = 1,
    COUNT
  };

  namespace Settings
  {
    extern FbxFileVersion            sFbxFileVersion;
    extern ReferenceFrame            sReferenceFrame;
    extern std::set<std::string>     sAssetSearchPaths;
    extern FbxFrameRate              sFbxFrameRate;
    extern f64                       sFbxFrameRateCustomValue;
    
    namespace Trs
    {
      extern ::SceneTrackFbx::Trs::TrsSystem sTransform;
      extern ::SceneTrackFbx::Trs::TrsSystem sMesh;
      extern AxisOrder                       sRotationAxisOrder;
    }

    enum class TriangleWindingOrder
    {
      AsIs,
      Reverse,
      Both
    };

    extern TriangleWindingOrder           sTriangleWinding;

    void PrintSettings();
  }
}
