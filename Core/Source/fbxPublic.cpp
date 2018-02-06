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

#include "fbxPublic.h"
#include "fbxExporter.h"
#include "expTrs.h"
#include "fbxSettings.h"
#include "time.h"
#include <sstream>

using namespace SceneTrackFbx;

template<typename T>
inline bool ValidateEnum(stEnum32 value)
{
  return (value) >= 0 && value < (stEnum32) T::COUNT;
}

s32 fbxSetAxisSwizzle(stEnum32 nodeType, stEnum32 trsMask, stEnum32 srcAxis, stEnum32 dstAxis, s32 sign)
{
  if (!ValidateEnum<Node>(nodeType) || !ValidateEnum<Axis>(srcAxis) || !ValidateEnum<Axis>(dstAxis))
  {
    return -1;
  }

  switch((Node)nodeType)
  {
    case Node::Transform:
    {
      Settings::Trs::sTransform.ConfigureSwizzleFromMask(trsMask, (Axis) srcAxis, (Axis) dstAxis, sign);
    }
    break;
    case Node::Mesh:
    {
      Settings::Trs::sMesh.ConfigureSwizzleFromMask(trsMask, (Axis) srcAxis, (Axis) dstAxis, sign);
    }
    break;
  }

  return 0;
}

s32 fbxSetAxisOperation(stEnum32 nodeType, stEnum32 trsMask, stEnum32 dstAxis, stEnum32 operatorType, f32 constantValue)
{
  if (!ValidateEnum<Node>(nodeType) || !ValidateEnum<Operator>(operatorType) || !ValidateEnum<Axis>(dstAxis))
  {
    return -1;
  }

  switch((Node)nodeType)
  {
    case Node::Transform:
    {
      Settings::Trs::sTransform.ConfigureFunctionFromMask(trsMask, (Axis) dstAxis, (Operator) operatorType, constantValue);
    }
    break;
    case Node::Mesh:
    {
      Settings::Trs::sMesh.ConfigureFunctionFromMask(trsMask, (Axis) dstAxis, (Operator) operatorType, constantValue);
    }
    break;
  }

  return 0;
}

void fbxClearAssetSearchPaths()
{
  Settings::sAssetSearchPaths.clear();
}

void fbxAddAssetSearchPath(stCStr path)
{
  Settings::sAssetSearchPaths.insert(path);
}

s32 fbxSetFileVersion(stEnum32 fbxFileVersion)
{
  if (fbxFileVersion < 0 || fbxFileVersion >= (stEnum32) FbxFileVersion::COUNT)
  {
    STFBX_PRINT_ERRORF("fbxSetFileVersion::Unknown Fbx file version %i", fbxFileVersion);
    return -1;
  }

  Settings::sFbxFileVersion = (FbxFileVersion) fbxFileVersion;
  return 0;
}

s32 fbxSetAxisRotationOrder(stEnum32 axisOrder)
{
  if (axisOrder < 0 || axisOrder >= (stEnum32) AxisOrder::COUNT)
  {
    STFBX_PRINT_ERRORF("fbxSetAxisRotationOrder::Unknown Axis order %i", axisOrder);
    return -1;
  }
  Settings::Trs::sRotationAxisOrder = (AxisOrder) axisOrder;
  return 0;
}

s32 fbxSetReferenceFrame(stEnum32 referenceFrame)
{
  if (referenceFrame < 0 || referenceFrame >= (stEnum32) ReferenceFrame::COUNT)
  {
    STFBX_PRINT_ERRORF("fbxSetReferenceFrame::Unknown reference frame %i", referenceFrame);
    return -1;
  }

  Settings::sReferenceFrame = (ReferenceFrame) referenceFrame;
  return 0;
}

s32 fbxSetReverseTriangleWinding(stEnum32 reverseWinding)
{
  if (reverseWinding < 0 || reverseWinding > 1)
  {
    STFBX_PRINT_ERRORF("fbxSetReferenceFrame::Unknown reverse Winding option %i", reverseWinding);
    return -1;
  }

  if (reverseWinding == 0)
    Settings::sTriangleWinding = Settings::TriangleWindingOrder::AsIs;
  else if (reverseWinding == 1)
    Settings::sTriangleWinding = Settings::TriangleWindingOrder::Reverse;
  else if (reverseWinding == 2)
    Settings::sTriangleWinding = Settings::TriangleWindingOrder::Both;

  return 0; 
}


s32 fbxSetFrameRate(stEnum32 frameRate, f64 customRate)
{
  if (frameRate < 0 || frameRate >= (stEnum32) FbxFrameRate::COUNT)
  {
    STFBX_PRINT_ERRORF("fbxSetReferenceFrame::Unknown frame rate %i", frameRate);
    return -1;
  }

  Settings::sFbxFrameRate = (FbxFrameRate) frameRate;
  Settings::sFbxFrameRateCustomValue = customRate;
  return 0;
}


std::shared_ptr<FbxExporterT> exporter;

s32 fbxStepConvertSceneTrackFileBegin(stCStr src, stCStr dst)
{
  s32 r = ExporterBegin("scenetrackfbx", src, dst, exporter);

  if (r == 0)
  {
    Settings::PrintSettings();
  }

  return r;
}

s32 fbxStepConvertSceneTrackFileUpdate()
{
  return ExporterUpdate(exporter);
}

f32 fbxStepConvertProgress()
{
  return ExporterProgress(exporter);
}

s32 fbxConvertSceneTrackFile(stCStr src, stCStr dst)
{
  return ExporterExportOneGo("scenetrackfbx", src, dst, exporter);
}
