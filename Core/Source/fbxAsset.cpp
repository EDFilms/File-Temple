// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxAsset.cpp
 *
 * Representation and management classes for external assets
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "fbxAsset.h"
#include "fbxSettings.h"

#include "tinydir/tinydir.h"

#define MAX_SEARCH_DEPTH 8

namespace SceneTrackFbx
{

  static const char* kImageExtensions[] = {
    "dds", "psd", "tif", "tiff", "jpg", "jpeg", "tga", "targa", "png", "gif", "bmp", "iff", "pict"
  };

  static const char* kShaderExtensions[] = {
    "shader"
  };

  void Asset_t::TryResolvePath(AssetPaths_t& paths)
  {
    bResolvedPath = false;

    // TODO: Find the first off the asset, depending on type, that's it.

    switch(type)
    {
      case AssetType::Texture: 
      {
        bResolvedPath = true;
        auto it = paths.textures.find(handle);
        if (it == paths.textures.end())
        {
          bBadPath = true;
          STFBX_PRINT_ERRORF("Couldn't Resolve TextureAssset %s. Missing Path or File?", handle.c_str());
        }
        else
        {
          bBadPath = false;
          path = it->second;
          // STFBX_PRINT_INFOF("Resolved TextureAssset %s to %s", handle.c_str(), path.c_str());
        }
      }
      break;
      case AssetType::Shader:
      {
        bResolvedPath = true;
        auto it = paths.shaders.find(handle);
        if (it == paths.shaders.end())
        {
          bBadPath = true;
          STFBX_PRINT_ERRORF("Couldn't Resolve ShaderAsset %s. Missing Path or File?", handle.c_str());
        }
        else
        {
          bBadPath = false;
          path = it->second;
          STFBX_PRINT_INFOF("Resolved ShaderAsset %s to %s", handle.c_str(), path.c_str());
        }
      }
      break;
    }
  }

  void AssetPaths_t::AddImage(const std::string& filename, const char* extension, const std::string& path)
  {
    std::string name = filename.substr(0, filename.length() - strlen(extension) - 1);
    textures.insert(std::make_pair(name, path));
  }

  void AssetPaths_t::AddShader(const std::string& filename, const char* extension, const std::string& path)
  {
    std::string name = filename.substr(0, filename.length() - strlen(extension) - 1);
    shaders.insert(std::make_pair(name, path));
  }

  AssetManager_t::AssetManager_t()
  {
    DiscoverAssets();
  }

#if STEXP_PLATFORM == STEXP_PLATFORM_WINDOWS
#define ST_STRCMPI _strcmpi
#else
#define ST_STRCMPI strcasecmp
#endif

  static s32  FindInArray(const char** strArray, u32 length, const char* match)
  {
    for(u32 i=0;i < length;i++)
    {
      const char* test = strArray[i];

      if (ST_STRCMPI(test, match) == 0)
        return i;
    }
    return -1;
  }

  static bool IsImage(const char* extension)
  {
    return FindInArray(kImageExtensions, STFBX_ARRAY_COUNT(kImageExtensions), extension) != -1;
  }

  static bool IsShader(const char* extension)
  {
    return FindInArray(kShaderExtensions, STFBX_ARRAY_COUNT(kShaderExtensions), extension) != -1;
  }

  void DiscoverFolder(const std::string& folder, AssetPaths_t& assets, int depth)
  {
    if (depth > MAX_SEARCH_DEPTH)
      return;

    tinydir_dir dir;
    tinydir_open(&dir, folder.c_str());

    while(dir.has_next)
    {
      tinydir_file file;
      tinydir_readfile(&dir, &file);

      if (file.name[0] == '.')
      {
        tinydir_next(&dir);
        continue;
      }

      if (file.is_dir)
      {
        DiscoverFolder(file.path, assets, depth + 1); 
      }
      else
      {
        if (IsImage(file.extension))
        {
          assets.AddImage(file.name, file.extension, file.path);
        }
        else if (IsShader(file.extension))
        {
          assets.AddShader(file.name, file.extension, file.path);
        }
      }

      tinydir_next(&dir); 
    }

    tinydir_close(&dir);
    
  }

  void AssetManager_t::DiscoverAssets()
  {
    for(auto path : Settings::sAssetSearchPaths)
    {
      DiscoverFolder(path, assetPaths, 0);
    }
  }

}
