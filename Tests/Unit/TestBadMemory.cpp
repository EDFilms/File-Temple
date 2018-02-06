#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "SceneTrackStatic.h"
#include "TestCommon.h"
#include "TestCRC.h"

extern "C" {
  ST_PUBLIC int stInternal_SetMallocMaxSizeMb(int megaBytes);
}

bool MaybeRange(int x, int y, int n)
{
  return (x >= (y - n) && x <= (y + n));
}

TEST_CASE("Force NULL at 40 mb", "[BadMemory]")
{
  printf("In Config.inc make sure these are set:\n");
  printf("#define ST_CFG_TRACK_MEMORY           1\n");
  printf("#define ST_CFG_MALLOC_CONTROLLED_NULL 1\n");
  printf("And Recompile - Otherwise this test will not work\n");

  stInternal_SetMallocMaxSizeMb(40);
  uint32_t context = stCreateRecording();

  REQUIRE(stGetError() == 0);

  uint32_t classTransform = stCreateObjectType(ST_FREQUENCY_DYNAMIC);
  uint32_t varTransform_Position = stAddObjectTypeComponentEx2(classTransform, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_WORLD);

  uint32_t nbObjects = 4096;
  uint32_t nbFrames = 4000;
  uint32_t* transforms = (uint32_t*) malloc(sizeof(uint32_t) * nbObjects);

  // Start
  for(uint32_t i=0;i < nbObjects;i++)
  {
    transforms[i] = stCreateObject(classTransform);
  }

  uint32_t frameNb=0;
  for(;frameNb < nbFrames;frameNb++)
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
      printf("Error reached!\n\nDisk error on frame: %i\n\n", frameNb);
    //  break;
    }

    if (stGetError() == ST_ERROR_RECORDING)
    {
      printf("Error reached!\n\nRecording error on frame: %i\n\n", frameNb);
     break;
    }

    if (stGetError() == ST_ERROR_WRITING)
    {
      printf("Error reached!\n\nWrite error on frame: %i\n\n", frameNb);
     break;
    }

    REQUIRE(frameNb <= 40);

  }
  stCloseRecording(context);

  printf("Test should pass if no assertions, and 'Recording error' message is printed. Should fail around frame 30.\n\n");
  // Recording is expected to fail around 28..32 (Usually 30)
  REQUIRE(MaybeRange(frameNb, 30, 2));

}
