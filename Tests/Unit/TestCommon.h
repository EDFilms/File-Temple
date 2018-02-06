#pragma once

#include <stdint.h>
#include "SceneTrack.h"
#include "catch.hpp"
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

#define ST_UNUSED(X) (void)(X)


// http://create.stephan-brumme.com/windows-and-linux-code-timing/
#ifdef _MSC_VER
// Windows
#include <Windows.h>
#else
// Linux
#include <time.h>
#endif
/// return a timestamp with sub-second precision
/** QueryPerformanceCounter and clock_gettime have an undefined starting point (null/zero)
and can wrap around, i.e. be nulled again. **/
inline double seconds()
{
#ifdef _MSC_VER
  static LARGE_INTEGER frequency;
  if (frequency.QuadPart == 0)
    ::QueryPerformanceFrequency(&frequency);
  LARGE_INTEGER now;
  ::QueryPerformanceCounter(&now);
  return now.QuadPart / double(frequency.QuadPart);
#else
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec + now.tv_nsec / 1000000000.0;
#endif
}


extern "C" {
  ST_PUBLIC int stInternal_GetNbMemoryAllocations();
  ST_PUBLIC int stInternal_GetNbClasses();
  ST_PUBLIC int stInternal_GetClassFrequency(unsigned int classType);
  ST_PUBLIC int stInternal_GetClassNbComponents(unsigned int classType);
  ST_PUBLIC int stInternal_GetClassComponentType(unsigned int classType, unsigned int componentType);
  ST_PUBLIC int stInternal_GetClassComponentKind(unsigned int classType, unsigned int componentType);
  ST_PUBLIC int stInternal_GetClassComponentNbElements(unsigned int classType, unsigned int componentType);
  ST_PUBLIC int stInternal_GetClassComponentUnits(unsigned int classType, unsigned int componentType);
  ST_PUBLIC int stInternal_GetClassComponentReference(unsigned int classType, unsigned int componentType);
  ST_PUBLIC int stInternal_GetCurrentBufferCommandCount();
  ST_PUBLIC int stInternal_GetFrameCount();
  ST_PUBLIC int stInternal_GetFrameCountForce();
}

typedef struct 
{
  float x, y, z;
} Vector3;

inline float randFloat(float m)
{
  return ((float)rand()/(float)(RAND_MAX)) * m;
}

inline float randFloat2(float m)
{
  return randFloat(m) - (m * 0.5f);
}

inline int randInt(int m)
{
  return rand() % m;
}

inline void DoRun(int seed, uint32_t nbObjects, uint32_t nbFrames, float frameTime, bool appendSave, uint32_t offset)
{

  srand(seed);
//  double nbSeconds = nbFrames * frameTime;
  double totalTime, setupTime, simulationTime, saveTime, closeTime;
  double timeA, timeB, timeA1, timeB1;
  char t[60];


  // Create Context
  int allocationsBefore = stInternal_GetNbMemoryAllocations();


  totalTime = 0;
  timeA = seconds();
  uint32_t context = stCreateRecording();
  stMemoryReserve(0, 30);
  stMemoryReserve(1, 16);

  if (appendSave)
  {
      sprintf(t, "results_%io_%if_bin_as_%i.st", nbObjects, nbFrames, offset);
      stAppendSaveRecording(t, ST_FORMAT_BINARY, 2);
  }

  REQUIRE(context != 0);

  // Register
  
  uint32_t classGameObject = stCreateObjectType(ST_FREQUENCY_DYNAMIC);
  uint32_t varGameObject_Layer = stAddObjectTypeComponent(classGameObject, ST_KIND_POSITION, ST_TYPE_UINT32, 0);

  uint32_t classTransform = stCreateObjectType(ST_FREQUENCY_DYNAMIC);
  uint32_t varTransform_Position = stAddObjectTypeComponentEx2(classTransform, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_WORLD);
  uint32_t varTransform_Rotation = stAddObjectTypeComponentEx2(classTransform, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_DEGREE, ST_REFERENCE_WORLD);
  uint32_t varTransform_LocalScale = stAddObjectTypeComponentEx2(classTransform, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_LOCAL);

  uint32_t* gameObjects = (uint32_t*) malloc(sizeof(uint32_t) * nbObjects);
  uint32_t* transforms = (uint32_t*) malloc(sizeof(uint32_t) * nbObjects);
  
  // Start
  for(uint32_t i=0;i < nbObjects;i++)
  {
    gameObjects[i] = stCreateObject(classGameObject);
    //printf("[0] %i New GameObject\n", gameObjects[i]);
    transforms[i] = stCreateObject(classTransform);
    //printf("[0] %i New Transform\n", transforms[i]);
    stSetValue_uint32(gameObjects[i], varGameObject_Layer, i);
    //printf("[0] %i GameObject.Layer=%i\n", gameObjects[i], i);
  }

  timeB = seconds();
  setupTime = (timeB - timeA);
  totalTime += setupTime;

  // Update
  for(uint32_t frame = 0; frame < nbFrames;frame++)
  {
    timeA1 = seconds();
//    uint32_t k = 0;

    for(uint32_t i=0;i < nbObjects;i++)
    {
//      if (frame > 3)
//      {
//        if ((k++) % 5 != 0)
//        {
//          continue;
//        }
//      }

      Vector3 p;
      p.x = randFloat2(100);
      p.y = randFloat2(100);
      p.z = randFloat2(100);

      stSetValue_3_float32(transforms[i], varTransform_Position, p.x, p.y, p.z);

      //printf("[%i] %i Transform.Position\n", frame, transforms[i]);

      Vector3 r;
      r.x = randFloat(360);
      r.y = randFloat(360);
      r.z = randFloat(360);

      stSetValue_3_float32(transforms[i], varTransform_Rotation, r.x, r.y, r.z);

      //printf("[%i] %i Transform.Rotation\n", frame, transforms[i]);

      Vector3 s;
      s.x = randFloat(1);
      s.y = randFloat(1);
      s.z = randFloat(1);

      stSetValue_3_float32(transforms[i], varTransform_LocalScale, s.x, s.y, s.z);

      //printf("[%i] %i Transform.Scale\n", frame, transforms[i]);

    }

    //printf("[%i] ** Submit\n", frame);
    stSubmit(frameTime);
    
    timeB1 = seconds();
    timeB += (timeB1 - timeA1);

#if defined(REAL_TIME)
    Sleep((DWORD) (frameTime * 1000.0));
#endif
  }

  simulationTime = (timeB - timeA);
  totalTime += simulationTime;

  timeA = seconds();
  
  if (!appendSave)
  {
    sprintf(t, "results_%io_%if_bin_%i.st", nbObjects, nbFrames, offset);
    stSaveRecording(t, ST_FORMAT_BINARY);
  }

  timeB = seconds();
  saveTime = (timeB - timeA);
  totalTime += saveTime;

  timeA = seconds();
  stCloseRecording(context);
  timeB = seconds();
  closeTime = (timeB - timeA);
  totalTime += closeTime;

  printf("%f seconds. Setup: %f, Sim: %f, Save: %f, Close: %f\n\n", totalTime, setupTime, simulationTime, saveTime, closeTime);

  printf("Time per frame: %f ms\n", ((double) (simulationTime * 1000.0) / nbFrames));

#if defined(NDEBUG)
//  REQUIRE(totalTime < nbSeconds);
#endif
  int allocationsAfter = stInternal_GetNbMemoryAllocations();
  REQUIRE(allocationsBefore == allocationsAfter); // No memory leaks

}

