#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "tinydir/tinydir.h"
#include "TestFbxMockUnity.h"
#include "TestCRC.h"
#include "TestFbxMeshCube.h"
#include "TestFbxMeshCylinder.h"
#include "TestFbxMeshRobot2.h"
#include "TestFbxMeshRobot2Anim.h"

using namespace Unity;

#define TEST_AXIS
//#define TEST_CUBES
//#define TEST_RAIN
//#define TEST_ANIM

// ===============================================

void  Rain(u32 count, u32 seconds = 4);
u32   MakeSessionId();
void  LoadAndSaveCopy(u32 test, u32 session, u32 sessionOffset = 0);
void  TestRecording(u32 test, u32 session, u32 sessionOffset = 0);
u32   CRCTest(u32 test, u32 session, u32 sessionOffset = 0);
void  stAppendSaveRecordingEx(u32 test, u32 sessionId, u32 sessionOffset, int format, u32 frames);

// ===============================================

TEST_CASE("FbxExportAxis")
{
  const u32 sessionId     = 0xA1A1A1A1;
  const u32 sessionOffset = 0;
  const u32 test          = 200;

  u32 recording = stCreateRecording();
  stAppendSaveRecordingEx(test, sessionId, sessionOffset, ST_FORMAT_BINARY, 2);

  CreateUnitySchema();

  CreateGameObject(Vector(+1,0,0,1))->setName("PX");
  CreateGameObject(Vector(-1,0,0,1))->setName("NX");
  CreateGameObject(Vector(0,+1,0,1))->setName("PY");
  CreateGameObject(Vector(0,-1,0,1))->setName("NY");
  CreateGameObject(Vector(0,0,+1,1))->setName("PZ");
  CreateGameObject(Vector(0,0,-1,1))->setName("NZ");

  stSubmit(1.0f);

  stCloseRecording(recording);

  TestRecording(test, sessionId, sessionOffset);
}

#if defined(TEST_CUBES)

TEST_CASE("FbxExportCubeMesh")
{

  const u32 sessionId     = 0xCAFEBEEF;
  const u32 sessionOffset = 0;
  const u32 test          = 201;

  u32 recording = stCreateRecording();
  stAppendSaveRecordingEx(test, sessionId, sessionOffset, ST_FORMAT_BINARY, 2);

  CreateUnitySchema();

  std::pair<MeshPtr, SubMeshPtr> cubeMesh = CreateMesh1(
    kCube_Vertices,                   // Vertices
    kCube_Normals,                    // Normals
    kCube_Tangents,                   // Tangents
    kCube_Uv,                         // UV0
    nullptr,                          // Colors (NONE)
    kCube_Indexes_0,                  // Indexes
    ARRAY_COUNT(kCube_Vertices) / 3,  // Number of Vertices
    ARRAY_COUNT(kCube_Indexes_0)      // Number of Indexes  (Number of Triangles = Number of Indexes / 3)
  ); 

  CreateStandardMeshRenderer(cubeMesh.first, Vector(0,0,0), Vector(0,0,0), Vector(1,1,1,1));

  std::pair<MeshPtr, SubMeshPtr> cylinderMesh = CreateMesh1(
    kCylinder_Vertices,                   // Vertices
    kCylinder_Normals,                    // Normals
    kCylinder_Tangents,                   // Tangents
    nullptr,                           // UV0 (NONE)
    nullptr,                          // Colors (NONE)
    kCylinder_Indexes_0,                  // Indexes
    ARRAY_COUNT(kCylinder_Vertices) / 3,  // Number of Vertices
    ARRAY_COUNT(kCylinder_Indexes_0)      // Number of Indexes  (Number of Triangles = Number of Indexes / 3)
  ); 

  CreateStandardMeshRenderer(cylinderMesh.first, Vector(2,0,2), Vector(0,0,0), Vector(1,1,1,1));



  stSubmit(1.0f);
  stCloseRecording(recording);

  TestRecording(test, sessionId, sessionOffset);
}

// ===============================================

TEST_CASE("FbxExportCubeMesh2")
{

  const u32 sessionId     = 0x0C0FFEE;
  const u32 sessionOffset = 0;
  const u32 test          = 202;

  u32 recording = stCreateRecording();
  stAppendSaveRecordingEx(test, sessionId, sessionOffset, ST_FORMAT_BINARY, 2);

  CreateUnitySchema();

  std::pair<MeshPtr, SubMeshPtr> cubeMesh = CreateMesh1(
    kCube_Vertices,                   // Vertices
    kCube_Normals,                    // Normals
    kCube_Tangents,                   // Tangents
    kCube_Uv,                         // UV0
    nullptr,                          // Colors (NONE)
    kCube_Indexes_0,                  // Indexes
    ARRAY_COUNT(kCube_Vertices) / 3,  // Number of Vertices
    ARRAY_COUNT(kCube_Indexes_0)      // Number of Indexes  (Number of Triangles = Number of Indexes / 3)
  ); 
  
  const float frameRate = 1.0f / 30.0f;
  const u32   nbFrames  = 61;

  Vector posChange = Vector(1, 2, 3);
  Vector rotChange = Vector(45, 90, -30);
  Vector scaFinale = Vector(2, 4, 1.5);

  auto smr = CreateStandardMeshRenderer(cubeMesh.first, Vector(0,0,0), Vector(0,0,0), Vector(1,1,1,1));
  auto tr = smr->getParent();

  std::ofstream f;
  char t0[32];
  sprintf(t0, "st%i_%08X_%i.txt", test, sessionId, sessionOffset);
  f.open(t0);

  for(u32 frameNb=0; frameNb < nbFrames;frameNb++)
  {
    Vector pos = tr->getLocalPosition();
    Vector rot = tr->getLocalRotation();
    Vector sca = tr->getLocalScale();

    if (frameNb > 0)
    {
      pos = Vector::AddVelocity(pos, posChange, frameRate);
      rot = Vector::AddVelocity(rot, rotChange, frameRate);
      sca = Vector::Lerp(Vector(1,1,1,1),  scaFinale, frameRate * frameNb);

      tr->setLocalPosition(pos);
      tr->setLocalRotation(rot);
      tr->setLocalScale(sca);
    }
    
    f << (1 + frameNb) << ":" << std::endl;
    f << "\t" << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    f << "\t" << rot.x << ", " << rot.y << ", " << rot.z << std::endl;
    f << "\t" << sca.x << ", " << sca.y << ", " << sca.z << std::endl;

    stSubmit(frameRate);
  }


  stCloseRecording(recording);

  TestRecording(test, sessionId, sessionOffset);
}
#endif

#if defined(TEST_RAIN)

TEST_CASE("FbxExportRain30")
{
  Rain(30);
}

TEST_CASE("FbxExportRain60")
{
  Rain(60);
}

TEST_CASE("FbxExportRain90")
{
  Rain(90);
}

TEST_CASE("FbxExportRain120")
{
  Rain(120);
}

TEST_CASE("FbxExportRain150")
{
  Rain(150);
}
#endif

#if defined(TEST_ANIM)

TEST_CASE("FbxExportFlagAnimation")
{
  const u32 sessionId     = 0xf01dab1e;
  const u32 sessionOffset = 0;
  const u32 test          = 204;

  u32 recording = stCreateRecording();
  stAppendSaveRecordingEx(test, sessionId, sessionOffset, ST_FORMAT_BINARY, 2);

  CreateUnitySchema();

  std::pair<MeshPtr, SubMeshPtr> robot2Mesh = CreateAnimatedMesh1(
    kRobot2_Vertices,                   // Vertices
    kRobot2_Normals,                    // Normals 
    kRobot2_Tangents,                   // Tangents
    kRobot2_Uv,                         // UV0     
    nullptr,                            // Colors  
    kRobot2_Indexes_0,                  // Indexes
    kRobot2_BoneWeights_Weight,         // Bones Weight
    kRobot2_BoneWeights_Index,          // Bones Index
    ARRAY_COUNT(kRobot2_Vertices) / 3,  // Number of Vertices
    ARRAY_COUNT(kRobot2_Indexes_0),     // Number of Indexes  (Number of Triangles = Number of Indexes / 3)
    kRobot2_BindPoses,                  // Bind Poses
    kRobot2_BoneCount                   // Bone Count
  ); 

  // --+ Robot Kyle (robotKyle), Transform, Animator, IK Script
  //   | 0,0,0  0,0,0
  //   +---- Robot 2 (smr), Transform, SkMR
  //   | 0,0,0  0,0,0
  //   +----Root (Transform)
  //     Pos: -2.187856e-33, 1.062831, 0.03512985
  //     Rot: 0, -90.00001, -83.956
  //        |
  //        + ---- <All the bones>

  auto robotKyle = CreateGameObject();
  auto smr = CreateSkinnedMeshRenderer(robot2Mesh.first, robotKyle);
  auto root = CreateGameObject(robotKyle->getTransform());

  smr->setBoneTransform(root->getTransform());
  
  root->getTransform()->setLocalPosition(Vector(-2.187856e-33f, 1.062831f, 0.03512985f));
  root->getTransform()->setLocalRotation(Vector(0.0f, -90.00001f, -83.956f));

  std::vector<BonePtr> bones;
  bones.reserve(kRobot2_BoneCount);

  // Initial setup.
  for(u32 i=0;i < kRobot2_BoneCount;i++)
  {
    BonePtr bone = CreateBone();
    bone->setId((u8)i);
    bones.push_back(bone);

    smr->addBone(bone);
  }

  smr->addBone_Apply();

  // Parenting.
  for (u32 i=0;i < kRobot2_BoneCount;i++)
  {
    BonePtr bone = bones[i];
    s8 parentId = kRobot2_BoneParent[i];

    if (parentId < 0)
    {
      printf("Bone#%i Id=%i\n", bone->getHandle(), bone->getId());
    }
    else
    {
      BonePtr parent = bones[parentId];
      bone->setParent(parent);
      printf("Bone#%i Id=%i  Parent#%i Id=%i\n", bone->getHandle(), bone->getId(), parent->getHandle(), parent->getId());
    }
  }

#if 0
  stSubmit(1.0f);
#else
  const float* frameData = &kRobot2_FrameData[0];
  float lastFrameTime = 0.0f;

  for (u32 frameNum=0;frameNum < kRobot2_FrameCount;frameNum++)
  {
    float frameTime = *frameData++;
    for(u32 boneId=0;boneId < kRobot2_BoneCount;boneId++)
    {
      float tx = *frameData++;
      float ty = *frameData++;
      float tz = *frameData++;
      float rx = *frameData++;
      float ry = *frameData++;
      float rz = *frameData++;

      BonePtr bone = bones[boneId];
      bone->setLocalPosition(Vector(tx, ty, tz));
      bone->setLocalRotation(Vector(rx, ry, rz));
    }
    stSubmit(frameTime - lastFrameTime);
    lastFrameTime = frameTime;
    //printf("Frame: %i => %f\n", frameNum, frameTime);
  }
#endif

  stCloseRecording(recording);

  TestRecording(test, sessionId, sessionOffset);
}

#endif 

// ===============================================

struct CM3Momentum
{
  Vector p, r, s0, s1;
};

u32 wang_hash(u32 seed)
{
  seed = (seed ^ 61) ^ (seed >> 16);
  seed *= 9;
  seed = seed ^ (seed >> 4);
  seed *= 0x27d4eb2d;
  seed = seed ^ (seed >> 15);
  return seed;
}

float randFloat(u32* seed, float min, float max)
{
  *seed = wang_hash(*seed);
  return min + static_cast <float> (*seed) /( static_cast <float> (UINT_MAX/(max-min)));
}

void Rain(u32 count, u32 seconds)
{
  const u32 sessionId = 0xBABEF000 + count;
  const u32 sessionOffset = 0;
  const u32 test = 203 + count;

  u32 recording = stCreateRecording();
  stAppendSaveRecordingEx(test, sessionId, sessionOffset, ST_FORMAT_BINARY, 2);

  CreateUnitySchema();

  std::pair<MeshPtr, SubMeshPtr> cubeMesh = CreateMesh1(
    kCube_Vertices,                   // Vertices
    kCube_Normals,                    // Normals
    kCube_Tangents,                   // Tangents
    kCube_Uv,                         // UV0
    nullptr,                          // Colors (NONE)
    kCube_Indexes_0,                  // Indexes
    ARRAY_COUNT(kCube_Vertices) / 3,  // Number of Vertices
    ARRAY_COUNT(kCube_Indexes_0)      // Number of Indexes  (Number of Triangles = Number of Indexes / 3)
  ); 

  const float frameRate = 1.0f / 30.0f;
  const u32   nbFrames  = 1 + (30 * seconds);

  std::vector<CM3Momentum> momentum;
  std::vector<StandardMeshRendererPtr> smrs;

  std::ofstream f;
  char t0[32];
  sprintf(t0, "st%i_%08X_%i.txt", test, sessionId, sessionOffset);
  f.open(t0);

  // Rain!
  for(u32 i=0;i < count;i++)
  {
    u32 seed = 33 + (i * 7);
    CM3Momentum m;
    m.p = Vector(randFloat(&seed, -1, 1), randFloat(&seed, -10, 0), randFloat(&seed, -1, -1));
    m.r = Vector(randFloat(&seed, -90, 90), randFloat(&seed, -90, 90), randFloat(&seed, -90, -90));
    m.s0 = Vector(randFloat(&seed, 0.1f, 2), randFloat(&seed, 0.1f, 2), randFloat(&seed, 0.1f, 2));
    m.s1 = Vector(randFloat(&seed, 0.1f, 2), randFloat(&seed, 0.1f, 2), randFloat(&seed, 0.1f, 2));

    Vector t = Vector(randFloat(&seed, -10, 10), randFloat(&seed, -10, -10), 10);

    auto smr = CreateStandardMeshRenderer(cubeMesh.first, t, Vector(0,0,0), m.s0);

    momentum.push_back(m);
    smrs.push_back(smr);
  }

  for(u32 frameNb=0; frameNb < nbFrames;frameNb++)
  {
    for(u32 i=0;i < count;i++)
    {
      auto smr = smrs[i];
      auto tr = smr->getParent();
      CM3Momentum& m = momentum[i];

      Vector pos = tr->getLocalPosition();
      Vector rot = tr->getLocalRotation();
      Vector sca = tr->getLocalScale();

      if (frameNb > 0)
      {
        pos = Vector::AddVelocity(pos, m.p, frameRate);
        rot = Vector::AddVelocity(rot, m.r, frameRate);
        sca = Vector::Lerp(m.s0, m.s1, frameRate * frameNb);

        tr->setLocalPosition(pos);
        tr->setLocalRotation(rot);
        tr->setLocalScale(sca);
      }

      f << (1 + frameNb) << ", " << i << ":" << std::endl;
      f << "\t" << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
      f << "\t" << rot.x << ", " << rot.y << ", " << rot.z << std::endl;
      f << "\t" << sca.x << ", " << sca.y << ", " << sca.z << std::endl;
    }
    stSubmit(frameRate);
  }


  stCloseRecording(recording);

  TestRecording(test, sessionId, sessionOffset);
}

u32 MakeSessionId()
{
  return (1 + rand()) * (1 + rand());
}

float GenerateFloat(u32 frame, u32 objectId, u32 offset)
{
  float m = (float) frame + objectId + offset;
  return m;
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

void stAppendSaveRecordingEx(u32 test, u32 sessionId, u32 sessionOffset, int format, u32 frames)
{
  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, sessionId, sessionOffset);
  return stAppendSaveRecording(t0, format, frames);
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

void TestRecording(u32 test, u32 sessionId, u32 sessionOffset)
{
  u32 crc1 = CRCTest(test, sessionId, sessionOffset);
  LoadAndSaveCopy(test, sessionId, sessionOffset);
  u32 crc2 = CRCTest(test, sessionId, sessionOffset + 1);
  REQUIRE(crc1 == crc2);
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
