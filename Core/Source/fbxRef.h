// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * fbxRef
 *
 * A reference system to tie un/partially or fulled loaded SceneTrack objects to corresponding
 * classes based upon Ids.
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
   * A Ref (or Reference) represents a SceneTrack object handle to a Schema class
   * instance. Once a reference to a SceneTrack object is found, then a reference
   * should be created, once additional information about that object is found
   * out then the Schema class is created, and relevant information is filled
   * out.
   *
   * This system allows other Schema classes and objects to use References
   * without managing load states. In a sense the information will always
   * be there, once it's loaded.
   *
   */
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

      /**
       * Update the id to given, and set it to having an id.
       */
      void UpdateId(u32 id)
      {
        Handle = id;
        bHasId = true;
      }

      /**
       * Assign an object to the reference, and mark as loaded
       */
      void UpdateObject(const std::shared_ptr<T>& object)
      {
        Object  = object;
        bLoaded = true;
      }

      /**
       * Remove the assign object from the reference, and mark as deleted
       */
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

  /**
   * Reference instance. This should be used by anything that
   * requires as Schema Object based on a SceneTrack handle.
   *
   * Because it is a shared_ptr, the Reference is the same
   * between all objects that use it.
   *
   */
  template <typename T> 
  using Ref = std::shared_ptr<Ref_t<T>>;

  /**
   * Manager class for references
   */
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

    /**
     * Get or fail a reference based upon a SceneTrack Object Id
     */
    bool Get(Ref<T>& maybeOutHandle, u32 id) const
    {
      const auto it = references.find(id);
      if (it == references.end())
        return false;

      maybeOutHandle = it->second;
      return true;
    }

    /**
     * Create a reference based upon a SceneTrack Object Id
     */
    Ref<T> Create(u32 id)
    {
      std::shared_ptr<T> obj = std::make_shared<T>(id);
      Ref<T> ref = std::make_shared<Ref_t<T>>(id, obj);

      references.insert(std::make_pair(id, ref));
      return ref;
    }

    /**
     * Get a Reference based upon a SceneTrack Object Id.
     * If the reference does not exist, then create one
     * but mark it as unloaded.
     */
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
