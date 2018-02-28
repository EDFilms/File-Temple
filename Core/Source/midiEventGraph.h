// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * midiEventGraph.h
 *
 * Represents a spatial and temporal graph of events that
 * correspond to musical notes.
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "midiRequired.h"
#include "expMath.h"

namespace SceneTrackMidi
{
  /**
   * Event classes as an enumeration
   */
  enum class EventClass
  {
    None,
    PhysicsEvent
  };

  /**
   * Type of the Event State; Started, Continuing or Stopped.
   */
  enum class EventType
  {
    Start    = 0,
    Continue = 1,
    Stop     = 2
  };

  /**
   * Event master class
   */
  class Event
  {
  public:
    
    Event() {}

    Event(EventClass klass_, EventType event_)
      : frameNumber(0), frameTime(0.0), frameLength(0.0), klass(klass_), event(event_), strength(1.0f), worldPosition(0,0,0), otherObject(0)
    {
    }

    ~Event()
    {
    }

    u32           frameNumber;
    f64           frameTime,      // Seconds in
                  frameLength;    // Length of rame
    EventClass    klass;
    EventType     event;
    f32           strength;
    Vector3f      worldPosition;
    u32           otherObject;
  };

  /**
   * Container class for Events for a particular SceneTrack Object
   */
  class ObjectEventTrack
  {
  public:

    ObjectEventTrack(u32 objectId_)
      : objectId(objectId_)
    {
    }

    ~ObjectEventTrack()
    {
    }
    
    /**
     * Append an event to the track
     */
    Event& Add(EventClass klass_, EventType event_)
    {
      events.push_back(Event(klass_, event_));
      return events[events.size() - 1];
    }

    u32                objectId;
    std::string        userData;
    std::vector<Event> events;
  };

  /**
   * Container class for all ObjectEventTracks organised by SceneTrack Objects
   */
  class EventGraph
  {
  public:
    std::map<u32, ObjectEventTrack*> tracks;
    f64 averageFps;
    u32 nbFrames;

    EventGraph()
    {
      averageFps = 0;
      nbFrames   = 0;
    }

    ~EventGraph()
    {
      for(auto frame : tracks)
      {
        delete frame.second;
      }

      tracks.clear();
    }

    /**
     * Get or Create an EventTrack corresponding to the SceneTrackObject
     * didCreate is used by the function for if the Track was created
     * or already existed.
     */
    ObjectEventTrack* GetOrAdd(u32 objectId, bool& didCreate)
    {
      auto it = tracks.find(objectId);

      if (it == tracks.end())
      {
        ObjectEventTrack* track = new ObjectEventTrack(objectId);
        tracks.insert(std::make_pair(objectId, track));
        didCreate = true;
        return track;
      }

      didCreate = false;
      return it->second;
    }

  };

}
