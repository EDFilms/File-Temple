#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "SceneTrackStatic.h"
// #define REAL_TIME
#include "TestCommon.h"
#include "TestCRC.h"

#define MAX_TIME 60
#define FPS 60

#define TEST_RUN(NB_OBJECTS, SECONDS) \
  TEST_CASE(#NB_OBJECTS "o_" #SECONDS, "[BigData]") { DO_RUN(NB_OBJECTS, SECONDS); }

#define DO_RUN(NB_OBJECTS, SECONDS) \
  do { printf("%i objects for %i seconds at %i Hz\n", NB_OBJECTS, SECONDS, FPS); \
    DoRun(1234, NB_OBJECTS, SECONDS * FPS, (1.0f / FPS), true, 0); \
  } while(0)

//#if (MAX_TIME >= 10)
//TEST_RUN(16,   1);
//TEST_RUN(16,   10);
//TEST_RUN(32,   10);
//TEST_RUN(64,   10);
//TEST_RUN(128,  10);
//TEST_RUN(256,  10);
//TEST_RUN(512,  10);
//TEST_RUN(1024, 10);
//TEST_RUN(2048, 10);
//#endif
//
//#if (MAX_TIME >= 20)
//TEST_RUN(16,   20);
//TEST_RUN(32,   20);
//TEST_RUN(64,   20);
//TEST_RUN(128,  20);
//TEST_RUN(256,  20);
//TEST_RUN(512,  20);
//TEST_RUN(1024, 20);
//TEST_RUN(2048, 20);
//#endif
//
//#if (MAX_TIME >= 30)
//TEST_RUN(16,   30);
//TEST_RUN(32,   30);
//TEST_RUN(64,   30);
//TEST_RUN(128,  30);
//TEST_RUN(256,  30);
//TEST_RUN(512,  30);
//TEST_RUN(1024, 30);
//TEST_RUN(2048, 30);
//#endif
//
//#if (MAX_TIME >= 45)
//TEST_RUN(16,   45);
//TEST_RUN(32,   45);
//TEST_RUN(64,   45);
//TEST_RUN(128,  45);
//TEST_RUN(256,  45);
//TEST_RUN(512,  45);
//TEST_RUN(1024, 45);
//TEST_RUN(2048, 45);
//#endif

#if (MAX_TIME >= 60)
//TEST_RUN(16,   60);
//TEST_RUN(32,   60);
//TEST_RUN(64,   60);
//TEST_RUN(128,  60);
//TEST_RUN(256,  60);
//TEST_RUN(512,  60);
//TEST_RUN(1024, 60);
TEST_RUN(2048, 60);
#endif
