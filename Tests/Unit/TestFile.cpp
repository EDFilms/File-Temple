#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "SceneTrackStatic.h"
#include "TestCommon.h"
#include "TestCRC.h"

uint32_t crcFile(const char* path)
{
  FILE* f = fopen(path, "rb");
  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  fseek(f, 0, SEEK_SET);
  
  void* data = malloc(size);
  fread(data, size, 1, f);
  uint32_t crc = crc32_fast(data, size, 0);
  free(data);

  return crc;
}

#if 0
TEST_CASE("Mini-Write", "[File]")
{
  printf("x.0");
  DoRun(1234, 2, 2, (1.0f / 60.0f), true, 993);
  printf("x.1");;
}
#endif

#if 0
TEST_CASE("Massive Write", "[File]")
{
  printf("x.0");
  DoRun(1234, 2048, 3600, (1.0f / 60.0f), true, 998);
  printf("x.1");
}
#endif

#if 1
TEST_CASE("Small-File-Save-Load-Save-CRC", "[File]")
{
  DoRun(1234, 2, 2, (1.0f / 60.0f), true, 998);
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0); // No memory leaks
  uint32_t recording = stOpenRecording("results_2o_2f_bin_as_998.st");

  stSaveRecording("results_2o_2f_bin_as_999.st", ST_FORMAT_BINARY);
  stCloseRecording(recording);
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0); // No memory leaks

  uint32_t crc1 = crcFile("results_2o_2f_bin_as_998.st");
  uint32_t crc2 = crcFile("results_2o_2f_bin_as_999.st");

  REQUIRE(crc1 == crc2);
}
#endif

#if 0
TEST_CASE("Huge-File-Save-Load-Save-CRC", "[File]")
{
  DoRun(1234, 2048, 3600, (1.0f / 60.0f), true, 998);
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0); // No memory leaks
  uint32_t recording = stOpenRecording("results_2048o_3600f_bin_as_998.st");
  stSaveRecording("results_2048o_3600f_bin_as_999.st", ST_FORMAT_BINARY);

  stCloseRecording(recording);
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0); // No memory leaks

  uint32_t crc1 = crcFile("results_2048o_3600f_bin_as_998.st");
  uint32_t crc2 = crcFile("results_2048o_3600f_bin_as_999.st");

  REQUIRE(crc1 == crc2);
}

TEST_CASE("Chain-File-Save-Load-Save-CRC", "[File]")
{
  const uint32_t count = 16;
  const uint32_t nbObjects = 60;
  const uint32_t nbFrames = 30;
  uint32_t nb = 500;
  char t[32];

  DoRun(1234, nbObjects, nbFrames, (1.0f / 60.0f), true, nb);
  sprintf(t, "results_%io_%if_bin_as_%i.st",  nbObjects, nbFrames, nb);
  uint32_t crc1 = crcFile(t);

  REQUIRE(stInternal_GetNbMemoryAllocations() == 0); // No memory leaks

  for(uint32_t i=0;i < count;i++)
  {
    sprintf(t, "results_%io_%if_bin_as_%i.st",  nbObjects, nbFrames, nb);
    uint32_t recording = stOpenRecording(t);
    nb++;
    sprintf(t, "results_%io_%if_bin_as_%i.st",  nbObjects, nbFrames, nb);
    stSaveRecording(t, ST_FORMAT_BINARY);
    stCloseRecording(recording);
    uint32_t crc2 = crcFile(t);

    REQUIRE(crc1 == crc2);
    REQUIRE(stInternal_GetNbMemoryAllocations() == 0); // No memory leaks
  }

}
#endif