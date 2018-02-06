#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "tinydir/tinydir.h"
#include "TestCommon.h"
#include "SceneTrackFbx.h"
#include "TestFbxMeshCube.h"

#define TEST_1
//#define TEST_AXIS
//#define TEST_CUBE
//#define TEST_RAIN
//#define TEST_ANIM


// ===============================================

void  Convert(u32 test, u32 sessionId, u32 sessionOffset);
void  Convert2(u32 test, u32 sessionId, u32 sessionOffset, u32 sessionOffset2);
void  Rain(u32 count);
u32   MakeSessionId();
void  LoadAndSaveCopy(u32 test, u32 session, u32 sessionOffset = 0);
void  TestRecording(u32 test, u32 session, u32 sessionOffset = 0);
u32   CRCTest(u32 test, u32 session, u32 sessionOffset = 0);
void  stAppendSaveRecordingEx(u32 test, u32 sessionId, u32 sessionOffset, int format, u32 frames);

// ===============================================

#define TEST_1
TEST_CASE("FbxTestAxis")
{
  //fbxSetFileVersion(STFBX_FBXFILEVERSION_FBX_201400);
  Convert(301, 0xFAFAFAFA, 0);
}

#if defined(TEST_AXIS)

TEST_CASE("FbxTestAxis")
{
  fbxSetFileVersion(STFBX_FBXFILEVERSION_FBX_201400);
  Convert(200, 0xA1A1A1A1, 0);
}

#endif

#if defined(TEST_CUBE)

TEST_CASE("FbxImportCubeMesh")
{
  Convert(201, 0xCAFEBEEF, 0);
}

TEST_CASE("FbxImportCubeAnimatedMesh")
{
  Convert(202, 0x0C0FFEE, 0);
}

#endif

#if defined(TEST_RAIN)

TEST_CASE("FbxImportRain30")
{
  fbxSetFileVersion(STFBX_FBXFILEVERSION_FBX_201400);
  Rain(30);
}

TEST_CASE("FbxImportRain60")
{
  fbxSetFileVersion(STFBX_FBXFILEVERSION_FBX_201400);
  Rain(60);
}

TEST_CASE("FbxImportRain90")
{
  fbxSetFileVersion(STFBX_FBXFILEVERSION_FBX_201400);
  Rain(90);
}

TEST_CASE("FbxImportRain120")
{
  fbxSetFileVersion(STFBX_FBXFILEVERSION_FBX_201400);
  Rain(120);
}

TEST_CASE("FbxImportRain150")
{
  fbxSetFileVersion(STFBX_FBXFILEVERSION_FBX_201400);
  Rain(150);
}

#endif

#if defined(TEST_ANIM)

TEST_CASE("FbxImportFlagAnimation36")
{
  fbxSetFileVersion(STFBX_FBXFILEVERSION_FBX_201400);
  Convert(204, 0xf01dab1e, 0);
}

#if 0

struct XYZ
{
  int e0, e1, e2;
};

#define X STFBX_AXIS_X
#define Y STFBX_AXIS_Y
#define Z STFBX_AXIS_Z

XYZ xyz[6] = {
  {X,Y,Z},
  {X,Z,Y},
  {Y,X,Z},
  {Y,Z,X},
  {Z,X,Y},
  {Z,Y,X}
};

#undef X
#undef Y
#undef Z

TEST_CASE("FbxImportFlagAnimation36")
{
  u32 tx, ty, tz, rx, ry, rz;

  for(u32 i=0;i < 6;i++)
  {
    tx = xyz[i].e0;
    ty = xyz[i].e1;
    tz = xyz[i].e2;

    for(u32 j=0;j < 6;j++)
    {
      rx = xyz[i].e0;
      ry = xyz[i].e1;
      rz = xyz[i].e2;
      
      fbxSetAxisSwizzle(STFBX_NODE_BONE, STFBX_TRS_TRANSLATION, STFBX_AXIS_X, tx);
      fbxSetAxisSwizzle(STFBX_NODE_BONE, STFBX_TRS_TRANSLATION, STFBX_AXIS_Y, ty);
      fbxSetAxisSwizzle(STFBX_NODE_BONE, STFBX_TRS_TRANSLATION, STFBX_AXIS_Z, tz);

      fbxSetAxisSwizzle(STFBX_NODE_BONE, STFBX_TRS_ROTATION, STFBX_AXIS_X, rx);
      fbxSetAxisSwizzle(STFBX_NODE_BONE, STFBX_TRS_ROTATION, STFBX_AXIS_Y, ry);
      fbxSetAxisSwizzle(STFBX_NODE_BONE, STFBX_TRS_ROTATION, STFBX_AXIS_Z, rz);

      Convert2(204, 0xf01dab1e, 0, i* 6 + j);
    }
  }

}
#endif

#endif

// ===============================================

void Convert(u32 test, u32 sessionId, u32 sessionOffset)
{
  char srcPath[32];
  sprintf(srcPath, "st%i_%08X_%i.st", test, sessionId, sessionOffset);

  char dstPath[32];
  sprintf(dstPath, "st%i_%08X_%i.fbx", test, sessionId, sessionOffset);

  u32 result = fbxConvertSceneTrackFile(srcPath, dstPath);

  REQUIRE(result == 0);
}

void Convert2(u32 test, u32 sessionId, u32 sessionOffset, u32 sessionOffset2)
{
  char srcPath[32];
  sprintf(srcPath, "st%i_%08X_%i.st", test, sessionId, sessionOffset);

  char dstPath[32];
  sprintf(dstPath, "st%i_%08X_%i.fbx", test, sessionId, sessionOffset2);

  u32 result = fbxConvertSceneTrackFile(srcPath, dstPath);

  REQUIRE(result == 0);
}

void Rain(u32 count)
{
  const u32 sessionId = 0xBABEF000 + count;
  const u32 sessionOffset = 0;
  const u32 test = 203 + count;

  char srcPath[32];
  sprintf(srcPath, "st%i_%08X_%i.st", test, sessionId, sessionOffset);

  char dstPath[32];
  sprintf(dstPath, "st%i_%08X_%i.fbx", test, sessionId, sessionOffset);

  u32 result = fbxConvertSceneTrackFile(srcPath, dstPath);

  REQUIRE(result == 0);
}
