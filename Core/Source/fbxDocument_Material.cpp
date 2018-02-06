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
#include "expTrs.h"
#include "fbxSettings.h"
#include "fbxExporter.h"

namespace SceneTrackFbx
{

  FTexture FbxDocumentWriter_t::CreateOrGetTexture(Asset asset)
  {
    auto const it = sharedTextures.find(asset->handle);

    if (it != sharedTextures.end())
    {
      return it->second;
    }

    FTexture texture = std::make_shared<FTexture_t>();
    if (asset->bResolvedPath == false)
    {
      asset->TryResolvePath(Iterator::sAssetManager->assetPaths);
    }

    texture->texture = FbxFileTexture::Create(SCENE, asset->handle.c_str());
    
    if (asset->bResolvedPath)
    {
      texture->texture->SetFileName(asset->path.c_str());
    }

    sharedTextures.insert(std::make_pair(asset->handle, texture));
    return texture;
  }

  FMaterial FbxDocumentWriter_t::CreateMaterial(MaterialRef materialRef)
  {
    auto mat = materialRef->Object;

    FMaterial fMaterial = std::make_shared<FMaterial_t>();

    FbxSurfacePhong* material = FbxSurfacePhong::Create(SCENE, "");

    material->Emissive.Set(ToFbxDouble3(mat->emissive));
    material->Ambient.Set(ToFbxDouble3(mat->color));
    material->Specular.Set(ToFbxDouble3(mat->specular));
    material->TransparencyFactor.Set(mat->transparency);
    material->Shininess.Set(0.5f);
    material->ReflectionFactor.Set(mat->reflection);

#if STFBX_CONFIG_TEXTURES_ENABLED == 1

    if (materialRef->Object->image != nullptr)
    {
      FTexture texture = CreateOrGetTexture(materialRef->Object->image);
      if (texture->texture != nullptr)
      {
        material->Diffuse.ConnectSrcObject(texture->texture);
      }
    }
#endif

    fMaterial->material = material;
    materialRef->Object->bExported = true;
    materialRef->Object->fMaterial = fMaterial;
    return fMaterial;
  }

  void  ApplyMaterials(std::vector<MaterialRef> materials, FbxNode* node, FbxDocumentWriter_t* doc)
  {
#if STFBX_CONFIG_MATERIALS_ENABLED
    for(auto it : materials)
    {
      auto m = it->Object;

      if (m->bExported == false)
      {
        doc->CreateMaterial(it);
      }

      node->AddMaterial(m->fMaterial->material);
    }
#else
    STFBX_UNUSED(materials);
    STFBX_UNUSED(node);
    STFBX_UNUSED(doc);
#endif
  }


}
