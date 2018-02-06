#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "SceneTrackStatic.h"
#include "TestCommon.h"
#include "tinydir/tinydir.h"
#include "TestCRC.h"

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

double runSubmitTime;
u32   MakeSessionId();
void  RunUnityRandomXYZTest(u32 test, u32 session, u32 nbObjects, u32 nbFrames, double timePerFrame = 1.0 / 60.0, bool save = true, u32 sessionOffset = 0, u32 seed = 1, bool printStats = false);
void  LoadAndSaveCopy(u32 test, u32 session, u32 sessionOffset = 0);
u32   CRCTest(u32 test, u32 session, u32 sessionOffset = 0);
void  RunBigTest(u32 test, u32 nbObjects, double hertz = 60.0, u32 duplicateCount = 1);
void  PassTest(u32 test, const char* strName);
bool  hasFile(const char* p);
u64   fileSize(const char* p);

// ST-21 Creation of a GameObject
SCENARIO("(ST-21) Game Objects can be created")
{
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);

  GIVEN("A new recording") {
    u32 recording = stCreateRecording();
    REQUIRE(recording != 0);

    WHEN("A GameObject Type is created") {
      u32 gameObjectType = stCreateObjectType(ST_FREQUENCY_DYNAMIC);

      THEN("Type count is increased") {
        REQUIRE(gameObjectType != 0);
        REQUIRE(stInternal_GetNbClasses() == 1);
      }
    }

    stCloseRecording(recording);
  }

  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);

  PassTest(21, "Game Objects can be created");
}

// ST-22 Creation of a Transform Type
SCENARIO("(ST-22) Creation of a Transform Type")
{
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);

  GIVEN("A new recording") {
    u32 recording = stCreateRecording();
    REQUIRE(recording != 0);

    WHEN("A Transform Type is created") {
      u32 transformType = stCreateObjectType(ST_FREQUENCY_DYNAMIC);

      THEN("Type count is increased") {
        REQUIRE(transformType != 0);
        REQUIRE(stInternal_GetNbClasses() == 1);
      }

      WHEN("A Transform position (XYZ) component is added") {
        
        stAddObjectTypeComponent(transformType, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3);

        THEN("Number of values for transform are increased") {
          REQUIRE(stInternal_GetClassNbComponents(transformType) == 1);
        }
      }

      WHEN("A Transform Type is created") {
        u32 transformObject = stCreateObject(transformType);

        THEN("Object must exist and be valid") {
          REQUIRE(transformObject != 0);
        }
      }
    }
    stCloseRecording(recording);
  }

  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
  PassTest(22, "Creation of a Transform Type");
}

SCENARIO("(ST-23) Update of a Transform Object")
{
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);

  GIVEN("A new recording") {
    u32 recording = stCreateRecording();
    REQUIRE(recording != 0);

    WHEN("A Transform Type is created") {
      u32 transformType = stCreateObjectType(ST_FREQUENCY_DYNAMIC);

      THEN("Type count is increased") {
        REQUIRE(transformType != 0);
        REQUIRE(stInternal_GetNbClasses() == 1);
      }

      WHEN("A Transform position component (XYZ) is added") {

        u32 component = stAddObjectTypeComponent(transformType, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3);

        THEN("Number of values for transform are increased") {
          REQUIRE(stInternal_GetClassNbComponents(transformType) == 1);
        }

        WHEN("A Transform Type is created") {
          u32 transformObject = stCreateObject(transformType);

          THEN("Object must exist and be valid") {
            REQUIRE(stInternal_GetCurrentBufferCommandCount() != 0);
            REQUIRE(transformObject != 0);
          }

          WHEN("A object transform position is updated")
          {
            stSetValue_3_float32(transformObject, component, 1.0f, 2.0f, 3.0f);

            THEN("Command Count has been increaed")
            {
              REQUIRE(stInternal_GetCurrentBufferCommandCount() > 1);
            }
          }
        }
      }


    }

    stCloseRecording(recording);
  }

  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
  PassTest(23, "Update of a Transform Object");
}


SCENARIO("(ST-24) Large Amounts of Data")
{
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);

  GIVEN("A new recording") {
    u32 recording = stCreateRecording();
    REQUIRE(recording != 0);

    WHEN("A Transform Type is created") {
      u32 transformType = stCreateObjectType(ST_FREQUENCY_DYNAMIC);

      THEN("Type count is increased") {
        REQUIRE(transformType != 0);
        REQUIRE(stInternal_GetNbClasses() == 1);
      }

      WHEN("A Transform position component is added") {

        u32 component = stAddObjectTypeComponent(transformType, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3);

        THEN("Number of values for transform are increased") {
          REQUIRE(stInternal_GetClassNbComponents(transformType) == 1);
        }

        WHEN("A Transform Type is created") {
          u32 transformObject = stCreateObject(transformType);

          THEN("Object must exist and be valid") {
            REQUIRE(stInternal_GetCurrentBufferCommandCount() != 0);
            REQUIRE(transformObject != 0);
          }

          WHEN("A object transform position is updated 6400 times")
          {
            s32 cmdCount = stInternal_GetCurrentBufferCommandCount();

            for(s32 i=0;i < 6400;i++)
            {
              stSetValue_3_float32(transformObject, component, 1.0f, 2.0f, 3.0f);

              THEN("Frame command count has been increased by one per change.")
              {
                REQUIRE(stInternal_GetCurrentBufferCommandCount() == cmdCount + 1 + i);
              }
            }
          }
        }
      }
    }
    stCloseRecording(recording);
  }
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
  PassTest(24, "Large Amounts of Data (6400 changes)");
}

SCENARIO("(ST-27) Submit Frame")
{
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);

  GIVEN("A new recording") {
    u32 recording = stCreateRecording();
    REQUIRE(recording != 0);

    WHEN("A Transform Type is created") {
      u32 transformType = stCreateObjectType(ST_FREQUENCY_DYNAMIC);

      THEN("Type count is increased") {
        REQUIRE(transformType != 0);
        REQUIRE(stInternal_GetNbClasses() == 1);
      }

      WHEN("A Transform position XYZ component is added") {

        u32 component = stAddObjectTypeComponent(transformType, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3);

        THEN("Number of values for transform are increased") {
          REQUIRE(stInternal_GetClassNbComponents(transformType) == 1);
        }

        WHEN("A Transform Type is created") {
          u32 transformObject = stCreateObject(transformType);

          THEN("Object must exist and be valid") {
            REQUIRE(stInternal_GetCurrentBufferCommandCount() != 0);
            REQUIRE(transformObject != 0);
          }

          WHEN("A object transform position is updated many times across 24 frames")
          {
            for(s32 i=0;i < 24;i++)
            {
              s32 cmdCount = stInternal_GetCurrentBufferCommandCount();

              stSetValue_3_float32(transformObject, component, 1.0f, 2.0f, 3.0f);

              THEN("Command Count has been increased")
              {
                REQUIRE(stInternal_GetCurrentBufferCommandCount() == cmdCount + 1);
              }

              stSubmit(1.0f / 24.0f);

              THEN("Frame count has been increased")
              {
                REQUIRE(stInternal_GetFrameCount() == 1 + i);
              }
            }
          }
        }
      }
    }
    stCloseRecording(recording);
  }
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
  PassTest(27, "Submit Frame (24 times)");
}


SCENARIO("(ST-28) Save Data To File")
{
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);

  GIVEN("A new recording") {
    u32 recording = stCreateRecording();
    REQUIRE(recording != 0);

    WHEN("A Transform Type is created") {
      u32 transformType = stCreateObjectType(ST_FREQUENCY_DYNAMIC);

      THEN("Type count is increased") {
        REQUIRE(transformType != 0);
        REQUIRE(stInternal_GetNbClasses() == 1);
      }

      WHEN("A Transform position component is added") {

        u32 component = stAddObjectTypeComponent(transformType, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3);

        THEN("Number of values for transform are increased") {
          REQUIRE(stInternal_GetClassNbComponents(transformType) == 1);
        }

        WHEN("A Transform Type is created") {
          u32 transformObject = stCreateObject(transformType);

          THEN("Object must exist and be valid") {
            REQUIRE(stInternal_GetCurrentBufferCommandCount() != 0);
            REQUIRE(transformObject != 0);
          }

          WHEN("A object transform position is updated many times across 24 frames")
          {
            for(s32 i=0;i < 24;i++)
            {
              s32 cmdCount = stInternal_GetCurrentBufferCommandCount();

              stSetValue_3_float32(transformObject, component, 1.0f, 2.0f, 3.0f);

              THEN("Command Count has been increased")
              {
                REQUIRE(stInternal_GetCurrentBufferCommandCount() == cmdCount + 1);
              }

              stSubmit(1.0f / 24.0f);

              THEN("Frame count has been increased")
              {
                REQUIRE(stInternal_GetFrameCount() == 1 + i);
              }
            }

            THEN("Frame count is 24")
            {
              REQUIRE(stInternal_GetFrameCountForce() == 24);
            }
           
            u32 saveSessionId = (1 + rand()) * (1 + rand());

            WHEN("A context is saved")
            {
              char t[32];
              sprintf(t, "st28_%08X_0.st", saveSessionId);
              stSaveRecording(t, ST_FORMAT_BINARY);

              THEN("File was created")
              {
                REQUIRE(hasFile(t));
                u64 size = fileSize(t);
                REQUIRE(size > 70); // About 70 bytes is the minimum size with one object type.
                REQUIRE(size > 2048); // About 2048 bytes for 24 frames with one change.
              }
            }
          }
        }
      }
    }
    
    stCloseRecording(recording);
  }
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
  PassTest(28, "Save Data To File");
}

SCENARIO("(ST-31) Compare Loaded Data")
{
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
  u32 count = 32;

  u32 session = MakeSessionId();
  RunUnityRandomXYZTest(31, session, 32, 24, 1.0 / 24.0, true);

  u32 crcA = CRCTest(31, session);
  for(u32 i=0;i < count;i++)
  {
    LoadAndSaveCopy(31, session, i);
    u32 crcB = CRCTest(31, session, i + 1);
    REQUIRE(crcA == crcB);
  }

  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
  PassTest(31, "Compare Loaded to Saved Data. (32 times).");
}

SCENARIO("(ST-34) Test 16 updates per frame, for 60 seconds at 60hz.")
{
  RunBigTest(33, 16);
}

SCENARIO("(ST-34) Test 32 updates per frame, for 60 seconds at 60hz.")
{
  RunBigTest(34, 32);
}

SCENARIO("(ST-35) Test 64 updates per frame, for 60 seconds at 60hz.")
{
  RunBigTest(35, 64);
}

SCENARIO("(ST-36) Test 128 updates per frame, for 60 seconds at 60hz.")
{
  RunBigTest(36, 128);
}

SCENARIO("(ST-37) Test 256 updates per frame, for 60 seconds at 60hz.")
{
  RunBigTest(37, 256);
}

SCENARIO("(ST-38) Test 512 updates per frame, for 60 seconds at 60hz.")
{
  RunBigTest(38, 512);
}

SCENARIO("(ST-39) Test 512 updates per frame, for 60 seconds at 60hz.")
{
  RunBigTest(39, 1024);
}

SCENARIO("(ST-40) Test 2048 updates per frame, for 60 seconds at 60hz.")
{
  RunBigTest(40, 2048);
}

u32 MakeSessionId()
{
  return (1 + rand()) * (1 + rand());
}

void RunUnityRandomXYZTest(u32 test, u32 session, u32 nbObjects, u32 nbFrames, double timePerFrame, bool save, u32 sessionOffset, u32 seed, bool printStats)
{
  srand(seed);

  double nbSeconds = nbFrames * timePerFrame;
  double totalTime, setupTime, simulationTime, saveTime, closeTime;
  double timeA, timeB, timeA1, timeB1;
  char t[60];


  int allocationsBefore = stInternal_GetNbMemoryAllocations();

  totalTime = 0;
  timeA = seconds();
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
    stSetValue_uint32(gameObjects[i], varGameObject_Layer, i);
  }

  timeB = seconds();
  setupTime = (timeB - timeA);
  totalTime += setupTime;

  timeA = seconds();

  for(uint32_t i=0;i < nbObjects;i++)
  {
    transforms[i] = stCreateObject(classTransform);
  }

  timeB = timeA;

  // Update
  for(uint32_t frame = 0; frame < nbFrames;frame++)
  {
    timeA1 = seconds();

    // Teleport, Rotate and Scale randomly for this object for this frame.
    for(uint32_t i=0;i < nbObjects;i++)
    {
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

    stSubmit((float)timePerFrame);
    
    timeB1 = seconds();
    timeB += (timeB1 - timeA1);

#if defined(REAL_TIME)
    Sleep((DWORD) (frameTime * 1000.0));
#endif
  }

  simulationTime = (timeB - timeA);
  totalTime += simulationTime;

  timeA = seconds();

  timeB = seconds();
  saveTime = (timeB - timeA);
  totalTime += saveTime;

  timeA = seconds();
  stCloseRecording(context);
  timeB = seconds();
  closeTime = (timeB - timeA);
  totalTime += closeTime;

  if (printStats)
  {
    printf("%f seconds. Setup: %f, Sim: %f, Save: %f, Close: %f\n\n", totalTime, setupTime, simulationTime, saveTime, closeTime);
    printf("Time per frame: %f ms\n", ((double) (simulationTime * 1000.0) / nbFrames));
  }

  runSubmitTime = (simulationTime * 1000.0) / nbFrames;
  REQUIRE(totalTime < nbSeconds);

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


void  RunBigTest(u32 test, u32 nbObjects, double hertz, u32 duplicateCount)
{
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
  printf("[ST-%i] %i Changes for 60s at %ihz, with copy check", test, nbObjects, (u32) hertz);
  u32 session = MakeSessionId();
  u32 nbFrames = (u32) hertz * 60; // 60 seconds.
  double timePerFrame = 1.0 / hertz;
  RunUnityRandomXYZTest(test, session, nbObjects, nbFrames, timePerFrame, true, 0, 1, false);

  printf(".");
  u32 crcA = CRCTest(test, session);
  for(u32 i=0;i < duplicateCount;i++)
  {
    LoadAndSaveCopy(test, session, i);
    u32 crcB = CRCTest(test, session, i + 1);
    REQUIRE(crcA == crcB);
    printf(".");
  }

  printf(" - Passed. %f ms per frame.", runSubmitTime);

  printf("\n");
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
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
