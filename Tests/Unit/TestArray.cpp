#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "SceneTrackStatic.h"
#include "TestCommon.h"
#include "tinydir/tinydir.h"
#include "TestCRC.h"
#include <vector>
#include <map>

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
void  LoadAndSaveCopy(u32 test, u32 session, u32 sessionOffset = 0);
u32   CRCTest(u32 test, u32 session, u32 sessionOffset = 0);
void  RunBigTest(u32 test, u32 nbObjects, double hertz = 60.0, u32 duplicateCount = 1);
void  WriteMemToFile(u32 test, u32 session, u32 sessionOffset, u8* data, size_t size);
void  PassTest(u32 test, const char* strName);
bool  hasFile(const char* p);
u64   fileSize(const char* p);
uint32_t stOpenRecordingEx(u32 test, u32 session, u32 sessionOffset);
void TestRecording(u32 test, u32 sessionId, u32 sessionOffset);


template<typename T>
struct Vector3T
{
  T x, y, z;
};

template<typename T>
struct Vector4T
{
  T x, y, z, w;
};

template<typename T>
struct RGBA
{
  T r, g, b, a;
};

#if 0

TEST_CASE("(ST-xx) Sequential Numbers")
{
  const u32 sessionId = MakeSessionId();
  const u32 sessionOffset = 0;
  const u32 count     = 100;
  const u32 test      = 99;

  u32 recording = stCreateRecording();


  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, sessionId, sessionOffset);
  stAppendSaveRecording(t0, ST_FORMAT_BINARY, 2);
  
  u32 numbersType = stCreateObjectType(ST_FREQUENCY_STATIC);
  u32 indexType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_INDEX, ST_TYPE_UINT16, 3, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);

  Vector3T<u16>* nums = new Vector3T<u16>[100];

  u16 j = 0;
  for(u32 i=0;i < count;i++)
  {
    nums[i].x = j++;
    nums[i].y = j++;
    nums[i].z = j++;
  }

  //size_t vs = sizeof(Vector3T<u16>);

  u32 stride = stCalculateStride1(ST_TYPE_UINT16, 3);

  u32 listOfNumbers = stCreateObject(numbersType);

  //u32 memorySize = stCalculatePODVectorMemorySize(ST_TYPE_UINT16, 3, count);

  //std::cout << "Size Of = " << vs << ", Stride = " <<  stride << ", Vector Size = " << 3 << ", Count = " << count << ", Memory Size = " << memorySize << std::endl;

  stSetValue_p_uint16(listOfNumbers, indexType, (u16*) nums, count, stride);

  stSubmit(1.0f);

  stCloseRecording(recording);


  u32 crc1 = CRCTest(test, sessionId, sessionOffset);
  LoadAndSaveCopy(test, sessionId, sessionOffset);
  u32 crc2 = CRCTest(test, sessionId, sessionOffset + 1);
  REQUIRE(crc1 == crc2);
}

TEST_CASE("(ST-xx) Stride Sequential Numbers")
{
  const u32 sessionId = MakeSessionId();
  const u32 sessionOffset = 0;
  const u32 count     = 100;
  const u32 test      = 100;

  u32 recording = stCreateRecording();


  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, sessionId, sessionOffset);
  stAppendSaveRecording(t0, ST_FORMAT_BINARY, 2);

  u32 numbersType = stCreateObjectType(ST_FREQUENCY_STATIC);
  u32 indexType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_INDEX, ST_TYPE_UINT16, 3, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);

  Vector4T<u16>* nums = new Vector4T<u16>[100];

  u16 j = 1;
  for(u32 i=0;i < count;i++)
  {
    nums[i].x = j++;
    nums[i].y = j++;
    nums[i].z = j++;
    nums[i].w = 0xCAFE;
  }

  //size_t vs = sizeof(Vector3T<u16>);

  u32 stride = stCalculateStride1(ST_TYPE_UINT16, 4);

  u32 listOfNumbers = stCreateObject(numbersType);

  //u32 memorySize = stCalculatePODVectorMemorySize(ST_TYPE_UINT16, 4, count);

  //std::cout << "Size Of = " << vs << ", Stride = " <<  stride << ", Vector Size = " << 3 << ", Count = " << count << ", Memory Size = " << memorySize << std::endl;

  stSetValue_p_uint16(listOfNumbers, indexType, (u16*) nums, count, stride);

  stSubmit(1.0f);

  stCloseRecording(recording);

  u32 crc1 = CRCTest(test, sessionId, sessionOffset);
  LoadAndSaveCopy(test, sessionId, sessionOffset);
  u32 crc2 = CRCTest(test, sessionId, sessionOffset + 1);
  REQUIRE(crc1 == crc2);
}

TEST_CASE("(ST-xx) Multiple Sequential Numbers")
{
  const u32 sessionId = MakeSessionId();
  const u32 sessionOffset = 0;
  const u32 count     = 100;
  const u32 test      = 101;

  u32 recording = stCreateRecording();


  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, sessionId, sessionOffset);
  stAppendSaveRecording(t0, ST_FORMAT_BINARY, 2);

  u32 numbersType = stCreateObjectType(ST_FREQUENCY_STATIC);
  u32 xType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_X, ST_TYPE_UINT16, 3, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
  u32 yType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_Y, ST_TYPE_UINT32, 3, count * 2, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);

  Vector3T<u16>* x = new Vector3T<u16>[count];

  u16 j16 = 0;
  for(u32 i=0;i < count;i++)
  {
    x[i].x = j16++;
    x[i].y = j16++;
    x[i].z = j16++;
  }

  Vector3T<u32>* y = new Vector3T<u32>[count * 2];

  u32 j32 = 0xFF;
  for(u32 i=0;i < count * 2;i++)
  {
    y[i].x = j32++;
    y[i].y = j32++;
    y[i].z = j32++;
  }

  u32 xStride = stCalculateStride1(ST_TYPE_UINT16, 3);
  u32 yStride = stCalculateStride1(ST_TYPE_UINT32, 3);

  u32 listOfNumbers = stCreateObject(numbersType);

  stSetValue_p_uint16(listOfNumbers, xType, (u16*) x, count, xStride);
  stSetValue_p_uint32(listOfNumbers, yType, (u32*) y, count * 2, yStride);

  stSubmit(1.0f);

  stCloseRecording(recording);

  u32 crc1 = CRCTest(test, sessionId, sessionOffset);
  LoadAndSaveCopy(test, sessionId, sessionOffset);
  u32 crc2 = CRCTest(test, sessionId, sessionOffset + 1);
  REQUIRE(crc1 == crc2);
}

TEST_CASE("(ST-xx) Stride Multiple Sequential Numbers")
{
  const u32 sessionId = MakeSessionId();
  const u32 sessionOffset = 0;
  const u32 count     = 100;
  const u32 test      = 102;

  u32 recording = stCreateRecording();


  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, sessionId, sessionOffset);
  stAppendSaveRecording(t0, ST_FORMAT_BINARY, 2);

  u32 numbersType = stCreateObjectType(ST_FREQUENCY_STATIC);
  u32 xType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_X, ST_TYPE_UINT16, 3, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
  u32 yType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_Y, ST_TYPE_UINT32, 3, count * 2, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);

  Vector4T<u16>* x = new Vector4T<u16>[count];

  u16 j16 = 0;
  for(u32 i=0;i < count;i++)
  {
    x[i].x = j16++;
    x[i].y = j16++;
    x[i].z = j16++;
    x[i].w = 1;
  }

  Vector4T<u32>* y = new Vector4T<u32>[count * 2];

  u32 j32 = 0xFF;
  for(u32 i=0;i < count * 2;i++)
  {
    y[i].x = j32++;
    y[i].y = j32++;
    y[i].z = j32++;
    y[i].w = 2;
  }

  u32 xStride = stCalculateStride1(ST_TYPE_UINT16, 4);
  u32 yStride = stCalculateStride1(ST_TYPE_UINT32, 4);

  u32 listOfNumbers = stCreateObject(numbersType);

  stSetValue_p_uint16(listOfNumbers, xType, (u16*) x, count, xStride);
  stSetValue_p_uint32(listOfNumbers, yType, (u32*) y, count * 2, yStride);

  stSubmit(1.0f);

  stCloseRecording(recording);

  u32 crc1 = CRCTest(test, sessionId, sessionOffset);
  LoadAndSaveCopy(test, sessionId, sessionOffset);
  u32 crc2 = CRCTest(test, sessionId, sessionOffset + 1);
  REQUIRE(crc1 == crc2);
}

TEST_CASE("(ST-xx) RGBA to seperate R-G-B-A arrays")
{
  const u32 sessionId = MakeSessionId();
  const u32 sessionOffset = 0;
  const u32 width     = 128;
  const u32 height    = 128;
  const u32 count     = width * height;
  const u32 test      = 103;

  u32 recording = stCreateRecording();


  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, sessionId, sessionOffset);
  stAppendSaveRecording(t0, ST_FORMAT_BINARY, 2);

  u32 numbersType = stCreateObjectType(ST_FREQUENCY_STATIC);
  u32 rType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_R, ST_TYPE_UINT8, 1, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
  u32 gType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_G, ST_TYPE_UINT8, 1, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
  u32 bType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_B, ST_TYPE_UINT8, 1, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
  u32 aType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_A, ST_TYPE_UINT8, 1, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);

  printf("r=%i, g=%i, b=%i, a=%i\n", rType, gType, bType, aType);

  RGBA<u8>* colours1 = new RGBA<u8>[count];

//  u8 r8 = 0x00;
//  u8 g8 = 0x44;
//  u8 b8 = 0x88;
//  
  for(u32 i=0;i < count;i++)
  {
    colours1[i].r = 0xAA; //r8++;
    colours1[i].g = 0xBB; //g8++;
    colours1[i].b = 0xCC; //b8++;
    colours1[i].a = 0x55; //i & 0xFF;
  }

  u32 rgbaStride = stCalculateStride1(ST_TYPE_UINT8, 4);

  u32 object_handle = stCreateObject(numbersType);

  u8* r1 = ((u8*) &colours1[0]) + 0;
  u8* g1 = ((u8*) &colours1[0]) + 1;
  u8* b1 = ((u8*) &colours1[0]) + 2;
  u8* a1 = ((u8*) &colours1[0]) + 3;

  WriteMemToFile(test, sessionId, 0, r1, count * rgbaStride);

  stSetValue_p_uint8(object_handle, rType, (u8*) r1, count, rgbaStride);
  stSetValue_p_uint8(object_handle, gType, (u8*) g1, count, rgbaStride);
  stSetValue_p_uint8(object_handle, bType, (u8*) b1, count, rgbaStride);
  stSetValue_p_uint8(object_handle, aType, (u8*) a1, count, rgbaStride);

  ST_UNUSED(r1);
  ST_UNUSED(g1);
  ST_UNUSED(b1);
  ST_UNUSED(a1);


  stSubmit(1.0f);

  stCloseRecording(recording);

  u32 crc1 = CRCTest(test, sessionId, sessionOffset);
  LoadAndSaveCopy(test, sessionId, sessionOffset);
  u32 crc2 = CRCTest(test, sessionId, sessionOffset + 1);
  REQUIRE(crc1 == crc2);

  u32 recording2 = stOpenRecordingEx(test, sessionId, sessionOffset);

  u32 it = stCreateFrameIterator();

  RGBA<u8>* colours2 = new RGBA<u8>[count];

  u8* r2 = ((u8*) &colours2[0]) + 0;
  u8* g2 = ((u8*) &colours2[0]) + 1;
  u8* b2 = ((u8*) &colours2[0]) + 2;
  u8* a2 = ((u8*) &colours2[0]) + 3;

  u32 rIdx = 0;

  while(stIteratorEnded(it) == 0)
  {
    u32 frameId         = stIteratorGetFrame(it);
    u32 objectHandle    = stIteratorGetObjectHandle(it);
    u32 objectType      = stIteratorGetObjectType(it);
    u32 componentHandle = stIteratorGetComponentHandle(it);
    u32 componentType   = stIteratorGetValueType(it);
    u32 itType          = stIteratorGetType(it);
    u32 nbElements      = stIteratorGetValueNbElements(it);
    u32 arraySize       = stIteratorGetValueArraySize(it);

    printf("Type=%i => Fr=%i, Ob=%i, Ty=%i, Co=%i, Ty=%i, NbElements=%i, ArraySize=%i\n", itType, frameId, objectHandle, objectType, componentHandle, componentType, nbElements, arraySize);
    

    u8* t = NULL;
    u8 *t1 = NULL, *t2 = NULL;
    if (componentHandle == rType)
    {
      t = r2;
      t2 = r1;
    }
    else if (componentHandle == gType)
    {
      t = g2;
      t2 = g1;
    }
    else if (componentHandle == bType)
    {
      t = b2;
      t2 = b1;
    }
    else if (componentHandle == aType)
    {
      t = a2;
      t2 = a1;
    }

    if (t != NULL)
    {
      u32 res = stIteratorGetValueArray(it, t, count * sizeof(u8), rgbaStride);
      printf("=> %i\n", res);

      t1 = t; 
      for(u32 i=0;i < count;i++)
      {
        REQUIRE(*t1 == *t2);
        t1 += rgbaStride;
        t2 += rgbaStride;
      }

      WriteMemToFile(test, sessionId, 1 + rIdx++, r2, count * rgbaStride);

    }

    stIteratorNext(it);
  }

  u8* c1 = (u8*) colours1;
  u8* c2 = (u8*) colours2;
  u32 result = memcmp(colours1, colours2, count * sizeof(RGBA<u8>));

  REQUIRE(result == 0);

  printf(">>>>> %i  %p vs %p\n", result, c1, c2);
  
  stDisposeIterator(it);

  stCloseRecording(recording2);
}



TEST_CASE("(ST-xx) Stride Multiple Sequential Numbers 2")
{
  const u32 sessionId = MakeSessionId();
  const u32 sessionOffset = 0;
  const u32 count     = 100;
  const u32 test      = 102;

  u32 recording = stCreateRecording();


  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, sessionId, sessionOffset);
  stAppendSaveRecording(t0, ST_FORMAT_BINARY, 2);

  u32 numbersType = stCreateObjectType(ST_FREQUENCY_STATIC);
  u32 xType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_X, ST_TYPE_UINT16, 3, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
  u32 yType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_Y, ST_TYPE_UINT32, 3, count * 2, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);

  Vector4T<u16>* x = new Vector4T<u16>[count];

  u16 j16 = 0;
  for(u32 i=0;i < count;i++)
  {
    x[i].x = j16++;
    x[i].y = j16++;
    x[i].z = j16++;
    x[i].w = 1;
  }

  Vector4T<u32>* y = new Vector4T<u32>[count * 2];

  u32 j32 = 0xFF;
  for(u32 i=0;i < count * 2;i++)
  {
    y[i].x = j32++;
    y[i].y = j32++;
    y[i].z = j32++;
    y[i].w = 2;
  }

  u32 xStride = stCalculateStride1(ST_TYPE_UINT16, 4);
  u32 yStride = stCalculateStride1(ST_TYPE_UINT32, 4);

  u32 listOfNumbers = stCreateObject(numbersType);

  stSetValue_p_uint16(listOfNumbers, xType, (u16*) x, count, xStride);
  stSetValue_p_uint32(listOfNumbers, yType, (u32*) y, count * 2, yStride);

  stSubmit(1.0f);

  stCloseRecording(recording);

  u32 crc1 = CRCTest(test, sessionId, sessionOffset);
  LoadAndSaveCopy(test, sessionId, sessionOffset);
  u32 crc2 = CRCTest(test, sessionId, sessionOffset + 1);
  REQUIRE(crc1 == crc2);
}

#endif

#if 0
TEST_CASE("(ST-xx) Pattern Checked")
{
  const u32 sessionId = MakeSessionId();
  const u32 sessionOffset = 0;
  const u32 count     = 1 << 16;
  const u32 test      = 103;

  u32 recording = stCreateRecording();

  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, sessionId, sessionOffset);
  stAppendSaveRecording(t0, ST_FORMAT_BINARY, 2);

  u32 numbersType = stCreateObjectType(ST_FREQUENCY_STATIC);
  u32 xType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_X, ST_TYPE_UINT32, 1, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
  
  u32* nums = new u32[count];

  for(u32 i=0;i < count;i++)
  {
    nums[i] = 33 + i;
  }

  u32 stride = stCalculateStride1(ST_TYPE_UINT32, 1);

  u32 object_handle = stCreateObject(numbersType);

  WriteMemToFile(test, sessionId, 0, (u8*) nums, count * stride);

  stSetValue_p_uint32(object_handle, xType, nums, count, stride);
  
  stSubmit(1.0f);

  stCloseRecording(recording);

  u32 crc1 = CRCTest(test, sessionId, sessionOffset);
  LoadAndSaveCopy(test, sessionId, sessionOffset);
  u32 crc2 = CRCTest(test, sessionId, sessionOffset + 1);
  REQUIRE(crc1 == crc2);

  u32 recording2 = stOpenRecordingEx(test, sessionId, sessionOffset);

  u32 it = stCreateFrameIterator();

  u32* nums2 = new u32[count];
  u32 rIdx = 0;

  while(stIteratorEnded(it) == 0)
  {
    u32 frameId         = stIteratorGetFrame(it);
    u32 objectHandle    = stIteratorGetObjectHandle(it);
    u32 objectType      = stIteratorGetObjectType(it);
    u32 componentHandle = stIteratorGetComponentHandle(it);
    u32 componentType   = stIteratorGetValueType(it);
    u32 itType          = stIteratorGetType(it);
    u32 nbElements      = stIteratorGetValueNbElements(it);
    u32 arraySize       = stIteratorGetValueArraySize(it);

    printf("Type=%i => Fr=%i, Ob=%i, Ty=%i, Co=%i, Ty=%i, NbElements=%i, ArraySize=%i\n", itType, frameId, objectHandle, objectType, componentHandle, componentType, nbElements, arraySize);


    if (componentHandle == xType)
    {
      u32 res = stIteratorGetValueArray(it, nums2, count * sizeof(u32), stride);
      printf("=> %i\n", res);

      WriteMemToFile(test, sessionId, 1 + rIdx++, (u8*) nums2, count * stride);
 
      for(u32 i=0;i < count;i++)
      {
        REQUIRE(nums[i] == (33 + i));
        REQUIRE(nums[i] == nums2[i]);
      }

    }
    
    stIteratorNext(it);
  }

  u32 result = memcmp(nums, nums2, count * sizeof(u32));

  REQUIRE(result == 0);

  stDisposeIterator(it);

  stCloseRecording(recording2);
}
#endif

TEST_CASE("(ST-xx) Array Two Frames")
{
  const u32 sessionId = MakeSessionId();
  const u32 sessionOffset = 0;
  const u32 count     = 1 << 16;
  const u32 test      = 110;

  u32 recording = stCreateRecording();

  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, sessionId, sessionOffset);
  stAppendSaveRecording(t0, ST_FORMAT_BINARY, 2);

  u32 numbersType = stCreateObjectType(ST_FREQUENCY_STATIC);
  u32 xType  = stAddObjectTypeComponentEx2(numbersType, ST_KIND_X, ST_TYPE_UINT32, 1, count, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);

  u32* nums = new u32[count];

  for(u32 i=0;i < count;i++)
  {
    nums[i] = 33 + i;
  }

  u32 stride = stCalculateStride1(ST_TYPE_UINT32, 1);

  u32 object_handle = stCreateObject(numbersType);

  WriteMemToFile(test, sessionId, 0, (u8*) nums, count * stride);

  stSetValue_p_uint32(object_handle, xType, nums, count, stride);

  stSubmit(1.0f);
  stCreateObject(numbersType);
  stSubmit(1.25f);

  stCreateObject(numbersType);
  stSubmit(1.35f);

  stCloseRecording(recording);

  TestRecording(test, sessionId, sessionOffset);
}
// ===============================================

u32 MakeSessionId()
{
  return (1 + rand()) * (1 + rand());
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

void TestRecording(u32 test, u32 sessionId, u32 sessionOffset)
{
  u32 crc1 = CRCTest(test, sessionId, sessionOffset);
  LoadAndSaveCopy(test, sessionId, sessionOffset);
  u32 crc2 = CRCTest(test, sessionId, sessionOffset + 1);
  REQUIRE(crc1 == crc2);
}

void     WriteMemToFile(u32 test, u32 session, u32 sessionOffset, u8* data, size_t size)
{
  char t0[32];
  sprintf(t0, "st%i_%08X_%i.bin", test, session, sessionOffset);

  FILE* f = fopen(t0, "wb");
  fwrite(data, size, 1, f);
  fclose(f);
}

uint32_t stOpenRecordingEx(u32 test, u32 session, u32 sessionOffset)
{
  char t0[32];
  sprintf(t0, "st%i_%08X_%i.st", test, session, sessionOffset);
  uint32_t recording = stOpenRecording(t0);

  return recording;
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
