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

#include "SceneTrack.h"
#include "expRequired.h"
#include <memory>

namespace EXP_NAMESPACE
{

  typedef int SchemaClassTypeT;

  enum class Units
  {
    Unspecified = ST_UNITS_UNSPECIFIED,
    Degree      = ST_UNITS_DEGREE,
    Radian      = ST_UNITS_RADIAN
  };

  enum class Reference
  {
    Unspecified = ST_REFERENCE_UNSPECIFIED,
    Local       = ST_REFERENCE_LOCAL,
    World       = ST_REFERENCE_WORLD
  };

  enum class Frequency
  {
    Static      = ST_FREQUENCY_STATIC,
    Dynamic     = ST_FREQUENCY_DYNAMIC,
    Stream      = ST_FREQUENCY_STREAM,
    Event       = ST_FREQUENCY_EVENT,
  };

  class SchemaComponentT;
  class SchemaClassT;
  class SchemaT;

  typedef std::shared_ptr<SchemaComponentT> SchemaComponent;
  typedef std::shared_ptr<SchemaClassT> SchemaClass;
  typedef std::shared_ptr<SchemaT> Schema;


  class SchemaComponentT
  {
  public:
#if STEXP_DEBUG == 1
    std::string name;
#endif

    u32       objectType = 0;
    u32       handle     = 0;
    u32       kind       = ST_KIND_NONE;
    u32       nbElements = 1;
    u32       arrayCapacity  = 1;
    u32       type       = ST_TYPE_UINT8;
    Units       units      = Units::Unspecified;
    Reference   reference  = Reference::Unspecified;

    void Register(u32 objectType_);

    void Discover(u32 objectType_);
  };

  class SchemaClassBuilder;

  class SchemaComponentBuilder
  {
    SchemaComponent                  component;
    SchemaClassBuilder&              classBuilder;
    u32                              objectType;

  public:
    SchemaComponentBuilder(SchemaComponent component_, u32 objectType_, SchemaClassBuilder& classBuilder_);

    template<typename T> 
    SchemaComponentBuilder& Type()
    {
      component->type = ::EXP_NAMESPACE::Type<T>::kType;
      return *this;
    }

    SchemaComponentBuilder& Kind(u32 kind);

    SchemaComponentBuilder& ArrayCapacity(u32 arrayCapacity);

    SchemaComponentBuilder& NbElements(u32 nbElements);

    SchemaComponentBuilder& Units(Units units);

    SchemaComponentBuilder& Reference(Reference reference);

    SchemaClassBuilder& End() const;
  };

  class SchemaClassT
  {
  public:

    std::map<std::string, SchemaComponent> components;
    std::map<u32, SchemaComponent> componentsByHandle;
    std::string name;
    u32         objectType;
    Frequency   frequency;
    u32         userData;
    SchemaClassTypeT classType;

    SchemaClassT(const std::string& name_, Frequency frequency_, u32 userData_, SchemaClassTypeT classType_);

    bool FindComponent(u32 componentId, SchemaComponent& outComponent) const;

    void Register();

    void Discover();
  };

  class SchemaClassBuilder
  {
  public:
    SchemaClass klass;

    SchemaClassBuilder(std::shared_ptr<SchemaClassT> class_);

    SchemaComponentBuilder Component(const std::string& name);
  };

  class SchemaT
  {
    std::map<std::string, std::shared_ptr<SchemaClassT>> classes;
    std::map<u32, std::shared_ptr<SchemaClassT>>         classesByObjectType;

  public:

    SchemaClassBuilder makeClass(const std::string& name, Frequency frequency, u32 userData, SchemaClassTypeT classType);

    bool FindClass(u32 objectType, SchemaClass& outClass) const;

    void Register();

    void Discover();
  };

}
