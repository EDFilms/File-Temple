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

#include "expSchema.h"

#define ST_DEBUG_SCHEMA 0

namespace EXP_NAMESPACE
{
  void SchemaComponentT::Register(u32 objectType_)
  {
    objectType = objectType_;
    handle = stAddObjectTypeComponentEx2(objectType, kind, type, nbElements, arrayCapacity, static_cast<u32>(units), static_cast<u32>(reference));
  }

  void SchemaComponentT::Discover(u32 objectType_)
  {
    objectType = objectType_;
    handle = stFindObjectTypeComponent(objectType, kind);

#if 0
#if ST_DEBUG_SCHEMA == 1
    if (handle == 0)
    {
      STEXP_PRINT_CHATTERF("[Schema][Component] '%s' cannot be found.", name.c_str());
    }
    else
    {
      STEXP_PRINT_CHATTERF("[Schema][Component] '%s' is represented as %i", name.c_str(), handle);
    }
#else
    if (handle == 0)
    {
      STEXP_PRINT_ERRORF("[Schema][Component] '%s' cannot be found.", name.c_str());
    }
#endif
#endif
  }

  SchemaComponentBuilder::SchemaComponentBuilder(SchemaComponent component_, u32 objectType_, SchemaClassBuilder& classBuilder_): component(component_), classBuilder(classBuilder_),  objectType(objectType_)
  {
    Type<u8>();
    Kind(ST_KIND_SIZE);
    ArrayCapacity(1);
    NbElements(1);
  }

  SchemaComponentBuilder& SchemaComponentBuilder::Kind(u32 kind)
  {
    component->kind = kind;
    return *this;
  }

  SchemaComponentBuilder& SchemaComponentBuilder::ArrayCapacity(u32 arrayCapacity)
  {
    component->arrayCapacity = arrayCapacity;
    return *this;
  }

  SchemaComponentBuilder& SchemaComponentBuilder::NbElements(u32 nbElements)
  {
    component->nbElements = nbElements;
    return *this;
  }

  SchemaComponentBuilder& SchemaComponentBuilder::Units(::EXP_NAMESPACE::Units units)
  {
    component->units = units;
    return *this;
  }

  SchemaComponentBuilder& SchemaComponentBuilder::Reference(::EXP_NAMESPACE::Reference reference)
  {
    component->reference = reference;
    return *this;
  }

  SchemaClassBuilder& SchemaComponentBuilder::End() const
  {
    return classBuilder;
  }

  SchemaClassT::SchemaClassT(const std::string& name_, Frequency frequency_, u32 userData_, SchemaClassTypeT classType_): name(name_), objectType(0), frequency(frequency_), userData(userData_), classType(classType_)
  {
  }

  bool SchemaClassT::FindComponent(u32 componentId, SchemaComponent& outComponent) const
  {
    auto it = componentsByHandle.find(componentId);
    if (it == componentsByHandle.end())
      return false;

    outComponent = it->second;
    return true;
  }

  void SchemaClassT::Register()
  {
    objectType = stCreateObjectTypeEx(static_cast<u32>(frequency), userData);

    for(auto it : components)
    {
      SchemaComponent comp = it.second;
      comp->Register(objectType);
      componentsByHandle.insert(std::make_pair(comp->handle, comp));
    }
  }

  void SchemaClassT::Discover()
  {
    objectType = stFindObjectType(userData);

    if (objectType == 0)
    {
#if 0
      STFBX_PRINT_WARNF("No Schema Class found for %s", name.c_str());
#endif
      return;
    }

#if ST_DEBUG_SCHEMA == 1
    STFBX_PRINT_CHATTERF("[Schema][Class] '%s' is represented as %i", name.c_str(), objectType);
#endif

    for(auto it : components)
    {
      SchemaComponent comp = it.second;
      comp->Discover(objectType);
      componentsByHandle.insert(std::make_pair(comp->handle, comp));
    }
  }

  SchemaClassBuilder::SchemaClassBuilder(std::shared_ptr<SchemaClassT> class_): klass(class_)
  {
  }

  SchemaComponentBuilder SchemaClassBuilder::Component(const std::string& name)
  {
    std::shared_ptr<SchemaComponentT> component = std::make_shared<SchemaComponentT>();
#if STEXP_DEBUG == 1
    component->name = name;
#endif
    klass->components.insert(std::make_pair(name, component));

    return SchemaComponentBuilder(component, klass->objectType, *this);
  }

  SchemaClassBuilder SchemaT::makeClass(const std::string& name, Frequency frequency, u32 userData, SchemaClassTypeT classType)
  {
    std::shared_ptr<SchemaClassT> klass = std::make_shared<SchemaClassT>(name, frequency, userData, classType);
    classes.insert(std::make_pair(name, klass));

    return SchemaClassBuilder(klass);
  }

  bool SchemaT::FindClass(u32 objectType, SchemaClass& outClass) const
  {
    auto it = classesByObjectType.find(objectType);

    if (it == classesByObjectType.end())
      return false;

    outClass = it->second;
    return true;
  }

  void SchemaT::Register()
  {
    for(auto it : classes)
    {
      SchemaClass klass = it.second;
      klass->Register();
      classesByObjectType.insert(std::make_pair(klass->objectType, klass));
    }
  }

  void SchemaT::Discover()
  {
    for(auto it : classes)
    {
      SchemaClass klass = it.second;
      klass->Discover();
      classesByObjectType.insert(std::make_pair(klass->objectType, klass));
    }
  }

}
