#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "SceneTrackStatic.h"
#include "TestCommon.h"
#include "TestCRC.h"

TEST_CASE("1-Zero Allocations at Start", "[Memory]")
{
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
}

TEST_CASE("2-Zero Allocations at End", "[Memory]")
{
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
  DoRun(1234, 4, 4, 1.0f / 60.0f, true, 0);
  REQUIRE(stInternal_GetNbMemoryAllocations() == 0);
}
