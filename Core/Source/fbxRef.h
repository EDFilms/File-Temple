// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	1.0.0
 */

#pragma once

#include "fbxRequired.h"

namespace SceneTrackFbx
{
  
  template<typename T>
  class Ref_t
  {
    public:

      Ref_t()
        : Handle(0),
          bHasId(0),
          bLoaded(0),
          bDeleted(0)
      {
      }

      Ref_t(u32 id)
        : Handle(id),
          bHasId(id != 0),
          bLoaded(0),
          bDeleted(0)
      {
      }

      Ref_t(u32 id, std::shared_ptr<T> obj)
        : Handle(id),
          bHasId(1),
          bLoaded(1),
          bDeleted(0),
          Object(obj)
      {
      }

      ~Ref_t()
      {
        bLoaded  = false;
        bDeleted = true;
      }

      void UpdateId(u32 id)
      {
        Handle = id;
        bHasId = true;
      }

      void UpdateObject(const std::shared_ptr<T>& object)
      {
        Object  = object;
        bLoaded = true;
      }

      void DeleteObject()
      {
        Object.reset();
        bDeleted = true;
      }

      u32                Handle;
      std::shared_ptr<T> Object;

      stBool32           bHasId : 1;
      stBool32           bLoaded  : 1;
      stBool32           bDeleted : 1;
  };

  template <typename T> 
  using Ref = std::shared_ptr<Ref_t<T>>;

  template<typename T>
  class RefManagerT
  {
  public:

    std::map<u32, Ref<T>> references;

    RefManagerT()
    {
    }

    ~RefManagerT()
    {
    }

    bool Get(Ref<T>& maybeOutHandle, u32 id) const
    {
      const auto it = references.find(id);
      if (it == references.end())
        return false;

      maybeOutHandle = it->second;
      return true;
    }

    Ref<T> Create(u32 id)
    {
      std::shared_ptr<T> obj = std::make_shared<T>(id);
      Ref<T> ref = std::make_shared<Ref_t<T>>(id, obj);

      references.insert(std::make_pair(id, ref));
      return ref;
    }

    Ref<T> GetOrCreate(u32 id)
    {
      const auto it = references.find(id);
      if (it == references.end())
        return Create(id);

      return it->second;
    }

  };

  template <typename T> 
  using RefManager = std::shared_ptr<RefManagerT<T>>;

}
