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

namespace SceneTrackFbx
{

  enum class AssetType
  {
    Texture,
    Shader,
    COUNT
  };

  typedef std::pair<std::string, AssetType> AssetId;

  struct AssetPaths_t
  {
    // name (without extension), full path
    std::multimap<std::string, std::string> textures;
    // name (without extension), full path
    std::multimap<std::string, std::string> shaders;

    void AddImage(const std::string& name, const char* extension, const std::string& path);
    void AddShader(const std::string& name, const char* extension, const std::string& path);

  };

  class Asset_t
  {
  public:

    Asset_t(const std::string& name_, AssetType type_)
      : handle(name_), type(type_), bHasId(1), bResolvedPath(0), bDeleted(0), bBadPath(0)
    {
    }

    ~Asset_t()
    {
      bResolvedPath  = false;
      bDeleted = true;
    }

    void TryResolvePath(AssetPaths_t& assetPaths);

    std::string        handle;
    AssetType          type;
    std::string        path;

    stBool32           bHasId : 1;
    stBool32           bResolvedPath  : 1;
    stBool32           bDeleted : 1;
    stBool32           bBadPath : 1;
  };

  typedef std::shared_ptr<Asset_t> Asset;

  class AssetManager_t
  {
  public:

    std::map<AssetId, Asset> references;
    AssetPaths_t             assetPaths;

    AssetManager_t();

    ~AssetManager_t()
    {
    }

    void DiscoverAssets();

    bool Get(Asset& maybeOutHandle, const std::string& id, AssetType type) const
    {
      const auto it = references.find(std::make_pair(id, type));
      if (it == references.end())
        return false;

      maybeOutHandle = it->second;
      return true;
    }

    Asset Create(const std::string& id, AssetType type)
    {
      Asset asset = std::make_shared<Asset_t>(id, type);

      references.insert(std::make_pair(std::make_pair(id, type), asset));
      return asset;
    }

    Asset GetOrCreate(const std::string& id, AssetType type)
    {
      const auto it = references.find(std::make_pair(id, type));
      if (it == references.end())
        return Create(id, type);

      return it->second;
    }

  };

  typedef std::shared_ptr<AssetManager_t> AssetManager;

}
