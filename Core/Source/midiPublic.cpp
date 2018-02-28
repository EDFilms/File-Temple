// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * midiPublic.cpp
 *
 * Public C interface for the MidiExporter library
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "midiPublic.h"
#include "midiExporter.h"
#include "midiSettings.h"

using namespace SceneTrackMidi;


template<typename T>
inline bool ValidateEnum(stEnum32 value)
{
  return (value) >= 0 && value < (stEnum32) T::COUNT;
}

s32 midiSetAxisSwizzle(stEnum32 nodeType, stEnum32 trsMask, stEnum32 srcAxis, stEnum32 dstAxis, s32 sign)
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
  }

  return 0;
}

s32 midiSetAxisOperation(stEnum32 nodeType, stEnum32 trsMask, stEnum32 dstAxis, stEnum32 operatorType, f32 constantValue)
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
  }

  return 0;
}

s32 midiSetFileType(stEnum32 fileType)
{
  if (fileType < 0 || fileType >= (stEnum32) MidiFileType::COUNT)
  {
    STFBX_PRINT_ERRORF("midiSetFileVersion::Unknown Fbx file version %i", fileType);
    return -1;
  }

  Settings::sMidiFileType = (MidiFileType) fileType;
  return 0;
}

std::shared_ptr<MidiExporterT> exporter;

s32 midiStepConvertSceneTrackFileBegin(stCStr src, stCStr dst)
{
  s32 r = ExporterBegin("scenetrackmidi", src, dst, exporter);

  if (r == 0)
  {
    Settings::PrintSettings();
  }

  return r;
}

s32 midiStepConvertSceneTrackFileUpdate()
{
  return ExporterUpdate(exporter);
}

f32 midiStepConvertProgress()
{
  return ExporterProgress(exporter);
}

s32 midiConvertSceneTrackFile(stCStr src, stCStr dst)
{
  return ExporterExportOneGo("scenetrackmidi", src, dst, exporter);
}
