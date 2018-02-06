#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "SceneTrackStatic.h"
#include "TestCommon.h"
#include "TestCRC.h"

TEST_CASE("Bad-Writing-Bad-Path", "[BadWriting]")
{
#if 0
  uint32_t context = stCreateRecording();
  stAppendSaveRecording("R:/bad_path.st", ST_FORMAT_BINARY, 2);

  REQUIRE(stGetError() == ST_ERROR_DISK);
  REQUIRE(stGetError() == 0);
  
  uint32_t classTransform = stCreateObjectType(ST_FREQUENCY_DYNAMIC);
  uint32_t varTransform_Position = stAddObjectTypeComponentEx2(classTransform, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_WORLD);

  uint32_t nbObjects = 30;
  uint32_t* transforms = (uint32_t*) malloc(sizeof(uint32_t) * nbObjects);

  // Start
  for(uint32_t i=0;i < nbObjects;i++)
  {
    transforms[i] = stCreateObject(classTransform);
  }

  for(uint32_t i=0;i < nbObjects;i++)
  {
    Vector3 p;
    p.x = randFloat2(100);
    p.y = randFloat2(100);
    p.z = randFloat2(100);

    stSetValue_3_float32(transforms[i], varTransform_Position, p.x, p.y, p.z);
  }

  stSubmit(1.0f / 60.0f);
  stCloseRecording(context);
#endif

}


TEST_CASE("Write-Until-Harddrive-Is-Full", "[BadWriting]")
{

  uint32_t context = stCreateRecording();
  stAppendSaveRecording("huge_file.st", ST_FORMAT_BINARY, 2);

  REQUIRE(stGetError() == 0);

  uint32_t classTransform = stCreateObjectType(ST_FREQUENCY_DYNAMIC);
  uint32_t varTransform_Position = stAddObjectTypeComponentEx2(classTransform, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_WORLD);

  uint32_t nbObjects = 4096;
  uint32_t nbFrames = 60000;
  uint32_t* transforms = (uint32_t*) malloc(sizeof(uint32_t) * nbObjects);

  // Start
  for(uint32_t i=0;i < nbObjects;i++)
  {
    transforms[i] = stCreateObject(classTransform);
  }

  for(uint32_t i=0;i < nbFrames;i++)
  {

    for(uint32_t j=0;j < nbObjects;j++)
    {
      Vector3 p;
      p.x = randFloat2(100);
      p.y = randFloat2(100);
      p.z = randFloat2(100);

      stSetValue_3_float32(transforms[j], varTransform_Position, p.x, p.y, p.z);
    }

    stSubmit(1.0f / 60.0f);

    if (stGetError() == ST_ERROR_DISK)
    {
      printf("Error reached!\n\nPlease Check no harddrive space to pass test\n\n");
      return;
    }
  }
  stCloseRecording(context);

  printf("Error wasn't reached.\n\nCheck no harddrive space to fail test\n\n");
  REQUIRE(false);
}
