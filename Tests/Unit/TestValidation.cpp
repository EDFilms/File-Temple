#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "SceneTrackStatic.h"
#include "TestCommon.h"
#include "tinydir/tinydir.h"
#include <vector>
#include <map>
#include "TestCRC.h"
#include "float.h"

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;
typedef float     f32;
typedef double    f64;
typedef u8        stByte;

std::ofstream debugValues;

std::ostream& operator<<(std::ostream& os, const Vector3& point)
{
  os << point.x << "," << point.y << "," << point.z;
  return os;
}

bool FloatEquals(float a, float b)
{
  return fabs(a - b) < FLT_EPSILON;
}

bool operator==(const Vector3& a, const Vector3& b)
{
  return FloatEquals(a.x, b.x) && FloatEquals(a.y, b.y) && FloatEquals(a.z, b.z);
}

bool operator!=(const Vector3& a, const Vector3& b)
{
  return !(a == b);
}

Vector3 Make_Vector3(float a, float b, float c)
{
  return {a,b,c};
}

double runSubmitTime;
u32   MakeSessionId();
void  LoadAndSaveCopy(u32 test, u32 session, u32 sessionOffset = 0);
u32   CRCTest(u32 test, u32 session, u32 sessionOffset = 0);
void  RunBigTest(u32 test, u32 nbObjects, double hertz = 60.0, u32 duplicateCount = 1);
void  PassTest(u32 test, const char* strName);
bool  hasFile(const char* p);
u64   fileSize(const char* p);
uint32_t stOpenRecordingEx(u32 test, u32 session, u32 sessionOffset);

void stSetPodValue(u32 objectHandle, u32 component, u8 value)
{
  stSetValue_uint8(objectHandle, component, value);
}

void stSetPodValue(u32 objectHandle, u32 component, u16 value)
{
  stSetValue_uint16(objectHandle, component, value);
}

void stSetPodValue(u32 objectHandle, u32 component, u32 value)
{
  stSetValue_uint32(objectHandle, component, value);
}

void stSetPodValue(u32 objectHandle, u32 component, u64 value)
{
  stSetValue_uint64(objectHandle, component, value);
}

void stSetPodValue(u32 objectHandle, u32 component, s8 value)
{
  stSetValue_int8(objectHandle, component, value);
}

void stSetPodValue(u32 objectHandle, u32 component, s16 value)
{
  stSetValue_int16(objectHandle, component, value);
}

void stSetPodValue(u32 objectHandle, u32 component, s32 value)
{
  stSetValue_int32(objectHandle, component, value);
}

void stSetPodValue(u32 objectHandle, u32 component, s64 value)
{
  stSetValue_int64(objectHandle, component, value);
}

void stSetPodValue(u32 objectHandle, u32 component, f32 value)
{
  stSetValue_float32(objectHandle, component, value);
}

void stSetPodValue(u32 objectHandle, u32 component, f64 value)
{
  stSetValue_float64(objectHandle, component, value);
}

void stSetPodValue(u32 objectHandle, u32 component, const Vector3& value)
{
  stSetValue_3_float32(objectHandle, component, value.x, value.y, value.z);
}

class Unity
{
public:
  static u32 frame;
};

u32 Unity::frame = 0;

template<typename T> 
class PodHistoryValue
{
public:

  struct TValue
  {
    T   value;
    u32 visits;
    u32 badVisits;
  };

  u32 handle, componentId;
  std::map< u32, std::shared_ptr<TValue> > values;
  u32 badVisits;
  const char* strName, *parentStrName;

  void Init(u32 handle_, u32 componentId_, const T& value, const char* strName_, const char* parentStrName_)
  {
    handle = handle_;
    componentId = componentId_;
    badVisits = 0;
    strName = strName_;
    parentStrName = parentStrName_;

    debugValues << "[" << Unity::frame << "] " << parentStrName << "(" << handle << ")." << strName << "(" << componentId << ") = " << value << " (New)" << std::endl;
    Set_(value);
  }
  
  void Set(const T& value)
  {
    Set_(value);
    debugValues << "[" << Unity::frame << "] " << parentStrName << "(" << handle << ")." << strName << "(" << componentId << ") = " << value << " (Update)" << std::endl;
  }

  void Set_(const T& value)
  {
    std::shared_ptr<TValue> v = std::make_shared<TValue>();
    v->value = value;
    v->visits = 0;
    v->badVisits = 0;

    values.insert(std::make_pair(Unity::frame, v));
    stSetPodValue(handle, componentId, value);
  }

  T Get(u32 frame) const
  {
    const auto it = values.find(frame);
    if (it == values.end())
      return {};
    return (*it).second->value;
  }

  void Visit(u32 frame, T value)
  {
    const auto it = values.find(frame);
    if (it == values.end())
    {
      badVisits++;
      return;
    }

    std::shared_ptr<TValue> v = (*it).second;

    debugValues << "[" << frame << "] " << parentStrName << "(" << handle << ")." << strName << "(" << componentId << ") = " << value << " (Visit)" << std::endl;

    v->visits++;
    if(v->value != value)
    {
      v->visits++;
      v->badVisits++;
    }
    else
    {
      v->visits++;
    }
  }

  bool VisitedCorrectly()
  {

    for(auto it : values)
    {
      if (it.second->badVisits > 0 || it.second->visits == 0)
      {
        printf("Failed Visit on: %i:%i. Frame = %i, Visits = %i, Bad Visits = %i\n", handle, componentId, it.first, it.second->visits, it.second->badVisits);
        return false;
      }
    }

    if (badVisits > 0)
    {
      printf("Failed Visit on: %i:%i. Bad Visits = %i\n", handle, componentId, badVisits);
      return false;
    }
    return true;
  }

};

template<typename T>
class ObjectContainer
{
public:
  std::map<u32, T*> objects;

  T* New()
  {
    T* go = new T();
    objects.insert(std::make_pair(go->handle, go));
    return go;
  }

  template<typename T1>
  T* New(const T1& a1)
  {
    T* go = new T(a1);
    objects.insert(std::make_pair(go->handle, go));
    return go;
  }

  T* Fetch(u32 id) const
  {
    const auto it = objects.find(id);
    if (it == objects.end())
      return NULL;
    return ((*it).second);
  }

};

class GameObjectClass
{
public:
  static u32 _Type;
  static u32 Id   ;
  static u32 Layer;

  static void Register()
  {
    _Type = stCreateObjectType(ST_FREQUENCY_DYNAMIC);
    Id    = stAddObjectTypeComponent(_Type, ST_KIND_INDEX, ST_TYPE_UINT32, 0);
    Layer = stAddObjectTypeComponent(_Type, ST_KIND_POSITION, ST_TYPE_UINT32, 0);
  }
};

u32 GameObjectClass::_Type  = 0;
u32 GameObjectClass::Id     = 0;
u32 GameObjectClass::Layer  = 0;

class GameObject
{
public:

  static u32 nextId;
  u32 handle;
  
  PodHistoryValue<u32> id;
  PodHistoryValue<u32> layer;

  GameObject()
  {
    handle = stCreateObject(GameObjectClass::_Type);
    debugValues << "[" << Unity::frame << "] " << handle <<  " = GameObject.New" << std::endl;

    id.Init(handle, GameObjectClass::Id, nextId++, "Id", "GameObject");
    layer.Init(handle, GameObjectClass::Layer, 32, "Layer", "GameObject");
  }

  void VisitValue(u32 iterator)
  {
    u32 frame = stIteratorGetFrame(iterator);
    u32 componentHandle = stIteratorGetComponentHandle(iterator);
    u32 componentType = stIteratorGetValueType(iterator);

    if (componentType == ST_TYPE_OBJECT_TYPE)
    {
      debugValues << "[" << frame << "] " << handle <<  " = GameObject.New (Visit)" << std::endl;
    }
    else if (componentHandle == GameObjectClass::Id)
    {
      REQUIRE(componentType == ST_TYPE_UINT32);
      u32 value = stIteratorGetValue_uint32(iterator, 0);
      id.Visit(frame, value);
      printf("Visit:  GameObject(%i).Id at Frame %i", handle, frame);
    }
    else if (componentHandle == GameObjectClass::Layer)
    {
      REQUIRE(componentType == ST_TYPE_UINT32);
      u32 value = stIteratorGetValue_uint32(iterator, 0);
      layer.Visit(frame, value);
      printf("Visit:  GameObject(%i).Layer at Frame %i", handle, frame);
    }
    else
    {
      assert(false);
    }
  }

  bool VisitedCorrectly()
  {
    if (id.VisitedCorrectly() == false)
      return false;
    
    if (layer.VisitedCorrectly() == false)
      return false;

    return true;
  }

};

u32 GameObject::nextId = 1;

class TransformClass
{
public:
  static u32 _Type        ;
  static u32 Id           ;
  static u32 GameObject   ;
  static u32 LocalPosition;
  static u32 LocalRotation;
  static u32 WorldScale   ;
  static u32 LossyScale   ;

  static void Register()
  {
    _Type         = stCreateObjectType(ST_FREQUENCY_DYNAMIC);
    Id            = stAddObjectTypeComponent(_Type, ST_KIND_INDEX, ST_TYPE_UINT32, 0);
    GameObject    = stAddObjectTypeComponent(_Type, ST_KIND_PARENT, ST_TYPE_UINT32, 0);
    LocalPosition = stAddObjectTypeComponentEx2(_Type, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_LOCAL);
    LocalRotation = stAddObjectTypeComponentEx2(_Type, ST_KIND_ROTATION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_LOCAL);
    WorldScale    = stAddObjectTypeComponentEx2(_Type, ST_KIND_SCALE, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_WORLD);
    LossyScale    = stAddObjectTypeComponentEx2(_Type, ST_KIND_SCALE, ST_TYPE_FLOAT32, 1, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_WORLD);
  }
};

u32 TransformClass::_Type         = 0;
u32 TransformClass::Id            = 0;
u32 TransformClass::GameObject    = 0;
u32 TransformClass::LocalPosition = 0;
u32 TransformClass::LocalRotation = 0;
u32 TransformClass::WorldScale    = 0;
u32 TransformClass::LossyScale    = 0;

class Transform
{
public:
  static u32 nextId;
  u32 handle;

  PodHistoryValue<u32> id;
  PodHistoryValue<u32> gameObject;
  PodHistoryValue<Vector3> localPosition;
  PodHistoryValue<Vector3> localRotation;
  PodHistoryValue<Vector3> worldScale;
  PodHistoryValue<float> lossyScale;

  Transform(u32 gameObjectHandle)
  {
    handle = stCreateObject(TransformClass::_Type);
    debugValues << "[" << Unity::frame << "] " << handle <<  " = Transform.New" << std::endl;

    id.Init(handle, TransformClass::Id, nextId++, "Id", "Transform");
    gameObject.Init(handle, TransformClass::GameObject, gameObjectHandle, "GameObject", "Transform");
    localPosition.Init(handle, TransformClass::LocalPosition, Make_Vector3(0,0,0), "LocalPosition", "Transform");
    localRotation.Init(handle, TransformClass::LocalRotation, Make_Vector3(0,0,0), "LocalRotation", "Transform");
    worldScale.Init(handle, TransformClass::WorldScale, Make_Vector3(1,1,1), "WorldScale", "Transform");
    lossyScale.Init(handle, TransformClass::LossyScale, 1, "LossyScale", "Transform");
  }

  void VisitValue(u32 iterator)
  {
    u32 frame = stIteratorGetFrame(iterator);
    u32 componentHandle = stIteratorGetComponentHandle(iterator);
    u32 componentType = stIteratorGetValueType(iterator);

    if (componentType == ST_TYPE_OBJECT_TYPE)
    {
      debugValues << "[" << frame << "] " << handle <<  " = Transform.New (Visit)" << std::endl;
    }
    else if (componentHandle == TransformClass::Id)
    {
      REQUIRE(componentType == ST_TYPE_UINT32);
      u32 value = stIteratorGetValue_uint32(iterator, 0);
      id.Visit(frame, value);
    }
    else if (componentHandle == TransformClass::GameObject)
    {
      REQUIRE(componentType == ST_TYPE_UINT32);
      u32 value = stIteratorGetValue_uint32(iterator, 0);
      gameObject.Visit(frame, value);
    }
    else if (componentHandle == TransformClass::LocalPosition)
    {
      REQUIRE(componentType == ST_TYPE_FLOAT32);
      Vector3 value;
      value.x = stIteratorGetValue_float32(iterator, 0);
      value.y = stIteratorGetValue_float32(iterator, 1);
      value.z = stIteratorGetValue_float32(iterator, 2);
      localPosition.Visit(frame, value);
    }
    else if (componentHandle == TransformClass::LocalRotation)
    {
      REQUIRE(componentType == ST_TYPE_FLOAT32);
      Vector3 value;
      value.x = stIteratorGetValue_float32(iterator, 0);
      value.y = stIteratorGetValue_float32(iterator, 1);
      value.z = stIteratorGetValue_float32(iterator, 2);
      localRotation.Visit(frame, value);
    }
    else if (componentHandle == TransformClass::WorldScale)
    {
      REQUIRE(componentType == ST_TYPE_FLOAT32);
      Vector3 value;
      value.x = stIteratorGetValue_float32(iterator, 0);
      value.y = stIteratorGetValue_float32(iterator, 1);
      value.z = stIteratorGetValue_float32(iterator, 2);
      worldScale.Visit(frame, value);
    }
    else if (componentHandle == TransformClass::LossyScale)
    {
      REQUIRE(componentType == ST_TYPE_FLOAT32);
      float value = stIteratorGetValue_float32(iterator, 0);
      lossyScale.Visit(frame, value);
    }
    else
    {
      assert(false);
    }
  }

  bool VisitedCorrectly()
  {
    REQUIRE(id.VisitedCorrectly());
    REQUIRE(gameObject.VisitedCorrectly());
    REQUIRE(localPosition.VisitedCorrectly());
    REQUIRE(localRotation.VisitedCorrectly());
    REQUIRE(worldScale.VisitedCorrectly());
    REQUIRE(lossyScale.VisitedCorrectly());

    return true;
  }

};

u32 Transform::nextId = 1;


typedef ObjectContainer<GameObject> GameObjects;
typedef ObjectContainer<Transform>  Transforms;

void RunUnityOrderedTest(u32 test, u32 session, u32 nbObjects, u32 nbFrames, double timePerFrame, bool save, u32 sessionOffset, GameObjects& gameObjects, Transforms& transforms);

// ST-xx Game Objects are saved and looked at
SCENARIO("(ST-xx) Game Objects are saved and looked at")
{
  GameObjects gameObjects;
  Transforms  transforms;

  const u32 kTestId  = 99;
  const u32 kSession = MakeSessionId();
  const u32 kSessionOffset = 0;
  const u32 kFrameCount = 2;
  const u32 kObjectCount = 2;
  const u32 kComponentCount = 3;

  debugValues.open("test.txt"); 

  RunUnityOrderedTest(kTestId, kSession, kObjectCount, kFrameCount, 1.0f / 60.0f, true, kSessionOffset, gameObjects, transforms);

  debugValues << "------------------------" << std::endl;

  u32 recording = stOpenRecordingEx(kTestId, kSession, kSessionOffset);
  u32 iterator   = stCreateForwardIterator();

  REQUIRE(iterator != 0);

  u32 nbFrames  = stGetRecordingCount();
  REQUIRE(nbFrames == (1 + kFrameCount));

  u32 count = 0;
  const u32 kMaxIteratorTurns = kFrameCount * kObjectCount * (1 + kComponentCount);

  while(true)
  {
    REQUIRE(count < 40);

    u32 objectHandle = stIteratorGetObjectHandle(iterator);
    s32 ended = stIteratorEnded(iterator);
    u32 objectType = stIteratorGetObjectType(iterator);

    //printf("[%i] Ended: %i, Fr: %i, Ob: %i (%i), Co: %i, Type: %i\n", count, ended, frame, objectHandle, objectType, componentHandle, componentType);

    if (ended != 0)
    {
      //printf("[%i] Ended: Reason = %i\n", count, ended);
      break;
    }

    REQUIRE((objectType == TransformClass::_Type || objectType == GameObjectClass::_Type));

    if (objectType == TransformClass::_Type)
    {
      Transform* transform = transforms.Fetch(objectHandle);
      assert(transform != nullptr);
      transform->VisitValue(iterator);
    }
    else if (objectType == GameObjectClass::_Type)
    {
      GameObject* gameObject = gameObjects.Fetch(objectHandle);
      assert(gameObject != nullptr);
      gameObject->VisitValue(iterator);
    }
    else
    {
      assert(false);
    }

    stIteratorNext(iterator);

    count++;
  }

#if 1
  for(auto it : gameObjects.objects)
  {
    GameObject* go = it.second;
    REQUIRE(go->VisitedCorrectly());
  }

  for(auto it : transforms.objects)
  {
    Transform* transform = it.second;
    REQUIRE(transform->VisitedCorrectly());
  }
#endif

  printf("Count = %i, MaxCount = %i\n", count, kMaxIteratorTurns);

  stDisposeIterator(iterator);
  stCloseRecording(recording);

  debugValues.close();
}

// ===============================================


u32 MakeSessionId()
{
  return (1 + rand()) * (1 + rand());
}

float GenerateFloat(u32 frame, u32 objectId, u32 offset)
{
  float m = (float) frame + objectId + offset;
  return m;
}

void RunUnityOrderedTest(u32 test, u32 session, u32 nbObjects, u32 nbFrames, double timePerFrame, bool save, u32 sessionOffset, GameObjects& gameObjects, Transforms& transforms)
{
  char t[60];

  int allocationsBefore = stInternal_GetNbMemoryAllocations();

  uint32_t context = stCreateRecording();
  stMemoryReserve(0, 30);
  stMemoryReserve(1, 16);

  if (save)
  {
    sprintf(t, "st%i_%08X_%i.st", test, session, sessionOffset);
    stAppendSaveRecording(t, ST_FORMAT_BINARY, 2);
  }

  REQUIRE(context != 0);

  // Register
  Unity::frame = 0;
  GameObjectClass::Register();
  GameObject::nextId = 1;
  TransformClass::Register();
  Transform::nextId = 1;

  // Start
  for(uint32_t i=0;i < nbObjects;i++)
  {
    GameObject* gameObject = gameObjects.New();
    transforms.New(gameObject->handle);
  }

  stSubmit((float)timePerFrame);
  Unity::frame++;

  // Update
  for(uint32_t frame = 0; frame < nbFrames;frame++)
  {
    // Teleport, Rotate and Scale randomly for this object for this frame.
    u32 i=0;
    for(auto it : transforms.objects)
    {
      Transform* transform = it.second;

      Vector3 p;
      p.x = GenerateFloat(frame, i, 0);
      p.y = GenerateFloat(frame, i, 1);
      p.z = GenerateFloat(frame, i, 2);

      transform->localPosition.Set(p);

      Vector3 r;
      r.x = GenerateFloat(frame, i, 3);
      r.y = GenerateFloat(frame, i, 4);
      r.z = GenerateFloat(frame, i, 5);

      transform->localRotation.Set(r);

      Vector3 s;
      s.x = GenerateFloat(frame, i, 6);
      s.y = GenerateFloat(frame, i, 7);
      s.z = GenerateFloat(frame, i, 8);

      transform->worldScale.Set(s);

      float ls;
      ls  = GenerateFloat(frame, i, 9);

      transform->lossyScale.Set(ls);

      i++;
    }

    stSubmit((float)timePerFrame);
    Unity::frame++;

  }
  
  stCloseRecording(context);

  int allocationsAfter = stInternal_GetNbMemoryAllocations();
  REQUIRE(allocationsBefore == allocationsAfter); // No memory leaks
}

void  LoadAndSaveCopy(u32 test, u32 session, u32 sessionOffset)
{
  char t0[32], t1[32];
  sprintf(t0, "st%i_%08X_%i.st", test, session, sessionOffset);
  sprintf(t1, "st%i_%08X_%i.st", test, session, sessionOffset + 1);

  uint32_t recording = stOpenRecording(t0);
  stSaveRecording(t1, ST_FORMAT_BINARY);
  stCloseRecording(recording);
}

uint32_t stOpenRecordingEx(u32 test, u32 session, u32 sessionOffset)
{
  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, session, sessionOffset);
  uint32_t recording = stOpenRecording(t0);

  return recording;
}

u32 CRCTest(u32 test, u32 session, u32 sessionOffset)
{
  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, session, sessionOffset);
  FILE* f = fopen(t0, "rb");
  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  fseek(f, 0, SEEK_SET);

  void* data = malloc(size);
  fread(data, size, 1, f);
  uint32_t crc = crc32_fast(data, size, 0);
  free(data);

  return crc;
}

void PassTest(u32 test, const char* name)
{
  printf("[ST-%i] %s - Passed.\n", test, name);
}

bool hasFile(const char* p)
{
  tinydir_dir dir;
  tinydir_open(&dir, ".");

  while (dir.has_next)
  {
    tinydir_file file;
    tinydir_readfile(&dir, &file);

    if (file.is_dir)
    {
      tinydir_next(&dir);
      continue;
    }

    if (strcmp(p, file.name) == 0)
    {
      tinydir_close(&dir);
      return true;
    }


    tinydir_next(&dir);
  }

  tinydir_close(&dir);
  return false;
}

u64 fileSize(const char* p)
{
  FILE* f = fopen(p, "rb");
  s64 size;
#if defined(_WIN32)
  _fseeki64(f, 0, SEEK_END);
  size = _ftelli64(f);
#else
  fseeko(f, 0, SEEK_END);
  size = ftello(f);
#endif
  fclose(f);
  return (u64) size;
}
