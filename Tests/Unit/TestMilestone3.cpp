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

struct SoundClass
{
  u32 type;
  u32 speaker;
  u32 volume;
  u32 length;
};

struct SpeakerClass
{
  u32 type;
  u32 position;
};

typedef u32 Speaker;
typedef u32 Sound;

SCENARIO("Events")
{
  u32 recording = stCreateRecording();
  stAppendSaveRecording("events.st", ST_FORMAT_BINARY, 2);

  SoundClass soundClass;

  soundClass.type       = stCreateObjectType(ST_FREQUENCY_EVENT);
  soundClass.speaker    = stAddObjectTypeComponent(soundClass.type, ST_KIND_PARENT, ST_TYPE_UINT32, 1);
  soundClass.volume     = stAddObjectTypeComponent(soundClass.type, ST_KIND_INTENSITY, ST_TYPE_FLOAT32, 1);
  soundClass.length     = stAddObjectTypeComponent(soundClass.type, ST_KIND_LENGTH, ST_TYPE_FLOAT32, 1);
  
  SpeakerClass speakerClass;
  
  speakerClass.type     = stCreateObjectType(ST_FREQUENCY_DYNAMIC);
  speakerClass.position = stAddObjectTypeComponent(speakerClass.type, ST_KIND_POSITION, ST_TYPE_FLOAT32, 3);

  Speaker speaker = stCreateObject(speakerClass.type);
  stSetValue_3_float32(speaker, speakerClass.position, 3.0f, 4.0f, 5.0f);

  Sound   sound1  = stCreateObject(soundClass.type);
  stSetValue_uint32(sound1, soundClass.speaker, speaker);


  stSubmit(1.0f);

  Sound   sound2  = stCreateObject(soundClass.type);
  stSetValue_uint32(sound2, soundClass.speaker, speaker);

  stSubmit(1.0f);

  stCloseRecording(recording);

}
