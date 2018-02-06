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
#include "fbxSettings.h"
#include "fbxVersion.h"

#include <sstream>

#define STFBX_INCLUDE_FBX_SDK
#include "fbxDocument_Shared.h"

namespace SceneTrackFbx
{

  FbxDocumentWriter_t::FbxDocumentWriter_t()
    : path("export.fbx")
  {
    im = new FbxDocumentWriterImpl_t();
  }

  FbxDocumentWriter_t::~FbxDocumentWriter_t()
  {
    delete im;
  }

  bool FbxDocumentWriter_t::Initialise(const std::string& path_)
  {
    path = std::string(path_);

    MGR = FbxManager::Create();
    if (MGR == NULL)
    {
      STFBX_PRINT_ERROR("[FBX] SDK Could not be initialised!");
      return false;
    }

    FbxIOSettings* ios = FbxIOSettings::Create(MGR, IOSROOT);
    MGR->SetIOSettings(ios);
    
    im->scene = FbxScene::Create(MGR, "SceneTrack");
    if (im->scene == NULL)
    {
      STFBX_PRINT_ERROR("[FBX] Could not create Scene");
      return false;
    }

    if (Settings::sFbxFrameRate != FbxFrameRate::Default)
    {

      FbxGlobalSettings& globalSettings = SCENE->GetGlobalSettings();

      if (Settings::sFbxFrameRate == FbxFrameRate::Custom)
      {
        globalSettings.SetTimeMode(FbxTime::EMode::eCustom);
        globalSettings.SetCustomFrameRate(Settings::sFbxFrameRateCustomValue);
      }
      else
      {
        FbxTime::EMode mode = TimeRateToFbxTimeEMode(Settings::sFbxFrameRate);
        globalSettings.SetTimeMode(mode);
      }
    }

#if 0

    FbxAxisSystem lAxisSytemReference = SCENE->GetGlobalSettings().GetAxisSystem();

    int upSign, forwardSign;
    auto up = lAxisSytemReference.GetUpVector(upSign);
    auto forward =lAxisSytemReference.GetFrontVector(forwardSign);



    auto cs = lAxisSytemReference.GetCoorSystem();
    
    char fAxis = '?', fSign = forwardSign > 0 ? '+' : ':';
    char uAxis = '?', uSign = upSign > 0 ? '+' : ':';

    /*
        Fbx notes about Axis System
        ---------------------------

        For example if the up axis is X, the remain two axes will be Y And Z, so the ParityEven is Y, and the ParityOdd is Z
        ; If the up axis is Y, the remain two axes will X And Z, so the ParityEven is X, and the ParityOdd is Z; 
        If the up axis is Z, the remain two axes will X And Y, so the ParityEven is X, and the ParityOdd is Y. 
    */
    switch(up)
    {
      case FbxAxisSystem::eXAxis: 
      {
        uAxis = 'X';
        fAxis = (forward == FbxAxisSystem::eParityEven ? 'Y' : 'Z');
      }
      break;
      case FbxAxisSystem::eYAxis:
      {
        uAxis = 'Y';
        fAxis = (forward == FbxAxisSystem::eParityEven ? 'X' : 'Z');
      }
      break;
      case FbxAxisSystem::eZAxis:
      {
        uAxis = 'Z';
        fAxis = (forward == FbxAxisSystem::eParityEven ? 'X' : 'Y');
      }
      break;
    }

    STFBX_PRINT_INFOF("FBX Axis System is; Up=%c%c,  Forward=%c%c,  LeftHanded=%i", uSign, uAxis, fSign, fAxis, (int) cs); 
#endif

    _CreateScene();

    return true;
  }

  void FbxDocumentWriter_t::Shutdown()
  {
    if (MGR != NULL)
    {
      MGR->Destroy();
      MGR = NULL;
    }

  }

  bool FbxDocumentWriter_t::Save()
  {
    int major, minor, revision;
    bool status;

    FbxExporter* exporter = FbxExporter::Create(MGR, "");

    int fileFormat = MGR->GetIOPluginRegistry()->GetNativeWriterFormat();

    auto ios = ((MGR->GetIOSettings()));

    ios->SetBoolProp(EXP_FBX_MATERIAL,        true);
    ios->SetBoolProp(EXP_FBX_TEXTURE,         true);
    ios->SetBoolProp(EXP_FBX_EMBEDDED,        true);
    ios->SetBoolProp(EXP_FBX_SHAPE,           true);
    ios->SetBoolProp(EXP_FBX_GOBO,            true);
    ios->SetBoolProp(EXP_FBX_ANIMATION,       true);
    ios->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

    if (exporter->Initialize(path.c_str(), fileFormat, ios) == false)
    {
      STFBX_PRINT_ERRORF("[FBX] Could not initialize exporter. Reason: %s", exporter->GetStatus().GetErrorString());
      return false;
    }

    const char* fileVersionStr;
    switch(Settings::sFbxFileVersion)
    {
      case  FbxFileVersion::FBX_53_MB55:         fileVersionStr = FBX_53_MB55_COMPATIBLE;    break;
      case  FbxFileVersion::FBX_60_MB60:         fileVersionStr = FBX_60_COMPATIBLE;         break;
      case  FbxFileVersion::FBX_200508_MB70:     fileVersionStr = FBX_2005_08_COMPATIBLE;    break;
      case  FbxFileVersion::FBX_200602_MB75:     fileVersionStr = FBX_2006_02_COMPATIBLE;    break;
      case  FbxFileVersion::FBX_200608:          fileVersionStr = FBX_2006_08_COMPATIBLE;    break;
      case  FbxFileVersion::FBX_200611:          fileVersionStr = FBX_2006_11_COMPATIBLE;    break;
      case  FbxFileVersion::FBX_200900:          fileVersionStr = FBX_2009_00_COMPATIBLE;    break;
      case  FbxFileVersion::FBX_200900v7:        fileVersionStr = FBX_2009_00_V7_COMPATIBLE; break;
      case  FbxFileVersion::FBX_201000:          fileVersionStr = FBX_2010_00_COMPATIBLE;    break;
      case  FbxFileVersion::FBX_201100:          fileVersionStr = FBX_2011_00_COMPATIBLE;    break;
      case  FbxFileVersion::FBX_201200:          fileVersionStr = FBX_2012_00_COMPATIBLE;    break;
      case  FbxFileVersion::FBX_201300:          fileVersionStr = FBX_2013_00_COMPATIBLE;    break;
      case  FbxFileVersion::FBX_201400:          fileVersionStr = FBX_2014_00_COMPATIBLE;    break;
      default: case  FbxFileVersion::FBX_201600: fileVersionStr = FBX_2016_00_COMPATIBLE;    break;
    }

    exporter->SetFileExportVersion(fileVersionStr);

    FbxManager::GetFileFormatVersion(major, minor, revision);
    STFBX_PRINT_CHATTERF("[FBX] File format version %d.%d.%d", major, minor, revision);

    status = exporter->Export(im->scene);
    exporter->Destroy();

    STFBX_PRINT_CHATTERF("[FBX] Document Saved = %i", status);

    return status;
  }

  void FbxDocumentWriter_t::_CreateScene()
  {
    FbxDocumentInfo* sceneInfo = FbxDocumentInfo::Create(MGR, "SceneInfo");
    sceneInfo->mTitle = "SceneTrack";
    sceneInfo->mSubject = "SceneTrack Export";
    sceneInfo->mAuthor = "SceneTrack";
    sceneInfo->mRevision = SCENETRACKFBX_VERSION_STR;

    SCENE->SetSceneInfo(sceneInfo);

    im->animStack = FbxAnimStack::Create(SCENE, "Animation");
    im->animLayer = FbxAnimLayer::Create(SCENE, "Layer0");

    im->animStack->AddMember(im->animLayer);
  }

}

#undef MGR
#undef SCENE
