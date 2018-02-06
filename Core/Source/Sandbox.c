// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "SceneTrackStatic.h"

ST_PUBLIC int stTest_CommandKeyBits();
ST_PUBLIC void stProbe_CommandBuffer(const char* name, int index);

int main1()
{
  uint32_t ctx = stCreateRecording();

  stTest_CommandKeyBits();

  uint32_t objectType      = stCreateObjectType(ST_FREQUENCY_DYNAMIC);
  uint32_t object_X        = stAddObjectTypeComponent(objectType, ST_KIND_X, ST_TYPE_UINT32, 1);
  uint32_t object_Y        = stAddObjectTypeComponent(objectType, ST_KIND_Y, ST_TYPE_UINT32, 1);

  uint32_t object1 = stCreateObject(objectType);
  stSetValue_uint32(object1, object_X, 1);
  
  uint32_t object2 = stCreateObject(objectType);
  stSetValue_uint32(object2, object_X, 4);
  stSetValue_uint32(object1, object_Y, 2);

  stProbe_CommandBuffer("SubmissionBuffer:Before", 0);
  stProbe_CommandBuffer("SortedBuffer:Before", 1);

  stSubmit(1.0f / 60.0f);

  stProbe_CommandBuffer("SubmissionBuffer:After", 0);
  stProbe_CommandBuffer("SortedBuffer:After", 1);

  stSaveRecording("test2_src.st", ST_FORMAT_BINARY);
  stSaveRecording("test2_src_text.st", ST_FORMAT_TEXT);
  stCloseRecording(ctx);
  return 0;
}

int main2()
{
  uint32_t ctx = stOpenRecording("test2_src.st");

  stSaveRecording("test2_target.st", ST_FORMAT_TEXT);
  stCloseRecording(ctx);

  return 0;
}

int main()
{
  main1();
  main2();
}