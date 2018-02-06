#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "SceneTrackStatic.h"
#include "TestCommon.h"
#include "TestCRC.h"

TEST_CASE("Context is created", "[Object]")
{
  uint32_t context = stCreateRecording();
  REQUIRE(context != 0);
  stCloseRecording(context);
}

TEST_CASE("Type is created", "[Object]")
{
  uint32_t context = stCreateRecording();
  REQUIRE(context != 0);

  int32_t count1 = stInternal_GetNbClasses();
  REQUIRE(count1 == 0);

  uint32_t typeId1 = stCreateObjectType(ST_FREQUENCY_DYNAMIC);
  REQUIRE(typeId1 > 0);

  int32_t count2 = stInternal_GetNbClasses();
  REQUIRE(count2 == 1);
  REQUIRE(stInternal_GetClassFrequency(typeId1) == ST_FREQUENCY_DYNAMIC);
  REQUIRE(stInternal_GetClassNbComponents(typeId1) == 0);

  uint32_t typeId2 = stCreateObjectType(ST_FREQUENCY_STREAM);
  REQUIRE(typeId2 > 1);
  REQUIRE(typeId2 > typeId1);

  int32_t count3 = stInternal_GetNbClasses();
  REQUIRE(count3 == 2);
  REQUIRE(stInternal_GetClassFrequency(typeId2) == ST_FREQUENCY_STREAM);
  REQUIRE(stInternal_GetClassNbComponents(typeId2) == 0);

  uint32_t componentType1 = stAddObjectTypeComponent(typeId2, ST_KIND_X, ST_TYPE_INT32, 1);
  REQUIRE(stInternal_GetClassNbComponents(typeId1) == 0);
  REQUIRE(stInternal_GetClassNbComponents(typeId2) == 1);
  REQUIRE(stInternal_GetClassComponentType(typeId2, componentType1) == ST_TYPE_INT32);
  REQUIRE(stInternal_GetClassComponentKind(typeId2, componentType1) == ST_KIND_X);
  REQUIRE(stInternal_GetClassComponentNbElements(typeId2, componentType1) == 1);
  REQUIRE(stInternal_GetClassComponentReference(typeId2, componentType1) == ST_REFERENCE_UNSPECIFIED);
  REQUIRE(stInternal_GetClassComponentUnits(typeId2, componentType1) == ST_UNITS_UNSPECIFIED);



  stCloseRecording(context);
}

TEST_CASE("Random Submissions", "[Recording]")
{

  srand(1234);

  uint32_t nbObjects = 25;
  uint32_t nbFrames  = 100;

  // Create Context

  uint32_t context = stCreateRecording();
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
    stSetValue_uint32(gameObjects[i], varGameObject_Layer, randInt(32));
  }

  for(uint32_t i=0;i < nbObjects;i++)
  {
    transforms[i] = stCreateObject(classTransform);

    Vector3 p;
    p.x = randFloat2(100);
    p.y = randFloat2(100);
    p.z = randFloat2(100);

    stSetValue_3_float32(transforms[i], varTransform_Position, p.x, p.y, p.z);

    Vector3 r;
    r.x = randFloat(360);
    r.y = randFloat(360);
    r.z = randFloat(360);

    stSetValue_3_float32(transforms[i], varTransform_Rotation, r.x, r.y, r.z);

    Vector3 s;
    s.x = randFloat(1);
    s.y = randFloat(1);
    s.z = randFloat(1);

    stSetValue_3_float32(transforms[i], varTransform_LocalScale, s.x, s.y, s.z);
  }

  // Update
  for(uint32_t frame = 0; frame < nbFrames;frame++)
  {
    for(uint32_t i=0;i < nbObjects;i++)
    {
      transforms[i] = stCreateObject(classTransform);

      Vector3 p;
      p.x = randFloat2(100);
      p.y = randFloat2(100);
      p.z = randFloat2(100);

      stSetValue_3_float32(transforms[i], varTransform_Position, p.x, p.y, p.z);

      Vector3 r;
      r.x = randFloat(360);
      r.y = randFloat(360);
      r.z = randFloat(360);

      stSetValue_3_float32(transforms[i], varTransform_Rotation, r.x, r.y, r.z);

      Vector3 s;
      s.x = randFloat(1);
      s.y = randFloat(1);
      s.z = randFloat(1);

      stSetValue_3_float32(transforms[i], varTransform_LocalScale, s.x, s.y, s.z);
    }

    stSubmit(1.0f / 60.0f);
  }


  stSaveRecording("st_random_bin.st", ST_FORMAT_BINARY);

  stCloseRecording(context);

}
