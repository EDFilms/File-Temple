// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxAsset.h
 *
 * Representation and management classes for external assets
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "fbxRequired.h"

namespace SceneTrackFbx
{

  /**
   * Type of external assets
   */
  enum class AssetType
  {
    Texture,
    Shader,
    COUNT
  };

  typedef std::pair<std::string, AssetType> AssetId;

  /**
   * Asset search paths
   */
  struct AssetPaths_t
  {
    // name (without extension), full path
    std::multimap<std::string, std::string> textures;
    // name (without extension), full path
    std::multimap<std::string, std::string> shaders;

    /**
     * Add an image asset search path
     */
    void AddImage(const std::string& name, const char* extension, const std::string& path);
    
    /**
     * Add a shader asset search path
     */
    void AddShader(const std::string& name, const char* extension, const std::string& path);

  };

  /**
   * Class representation of an external asset
   */
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

    /**
     * Try and resolve the path for the asset by it's name.
     */
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

  /**
   * Singleton type class to manage and fetch Asset instances
   */
  class AssetManager_t
  {
  public:

    std::map<AssetId, Asset> references;
    AssetPaths_t             assetPaths;

    AssetManager_t();

    ~AssetManager_t()
    {
    }

    /**
     * Using Settings::sAssetSearchPaths go through all directories and build
     * a list of usable assets but do not load them.
     */
    void DiscoverAssets();

    /**
     * Try and get an asset according to it's name and type
     */
    bool Get(Asset& maybeOutHandle, const std::string& id, AssetType type) const
    {
      const auto it = references.find(std::make_pair(id, type));
      if (it == references.end())
        return false;

      maybeOutHandle = it->second;
      return true;
    }

    /**
     * Create an asset based on it's name and type
     */
    Asset Create(const std::string& id, AssetType type)
    {
      Asset asset = std::make_shared<Asset_t>(id, type);

      references.insert(std::make_pair(std::make_pair(id, type), asset));
      return asset;
    }

    /**
     * Try and get an asset according to it's name and type, if
     * it does not exist then create it.
     */
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
