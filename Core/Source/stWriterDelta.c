// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stWriterDelta.h
 *
 * SceneTrack binary delta format writer interface for stWriter
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stWriterDelta.h"
#include "stObject.h"
#include "stList.h"
#include "stCommand.h"
#include "stFile.h"
#include "stSymbols.h"
#include "stCookie.h"
#include "stRecording.h"
#include "stCommandKey.h"
#include "stObjectData.h"
#include <limits.h>

void writeIdChange(stWriteContextData* context, stContext* ctx)
{
  stFileWriter* w = &context->delta.writer;
  stFileWriterBeginChunk(w, "IDCH", 0);
    ST_FILEWRITER_WRITE_KP_BEGIN(w);
      ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_HEADER_ID_INDEX, u32, ctx->headerFrames.nextFrameId.index);
      ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_HEADER_ID_OFFSET, u32, ctx->headerFrames.nextFrameId.offset);
      ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_RECORDING_TIME, f64, ctx->recordingTime);
      ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FRAME_COUNT, u32, ctx->nbSubmittedFrames);
    ST_FILEWRITER_WRITE_KP_END(w);
  stFileWriterEndChunk(w);
}

void writeClasses(stWriteContextData* context, stClasses* classes)
{
  stFileWriter* w = &context->delta.writer;

  ST_LIST_FOR_BEGIN(stClass, classes, klass);
    stFileWriterBeginChunk(w, "CLAS", klass->id);
      stFileWriterBeginChunk(w, "META", 0);
        ST_FILEWRITER_WRITE_KP_BEGIN(w);
          ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_ID, u32, klass->id);
          ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_USES, u32, klass->nbUses);
          ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_NB_COMPONENTS, u8, klass->nbComponents);
          ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FREQUENCY, u8, klass->frequency);
          ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FLAGS, u8, klass->flags);
          ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_USERDATA, u32, klass->userData);
          ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_RESERVED1, u8, klass->reserved1);
          ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_RESERVED2, u8, klass->reserved2);
        ST_FILEWRITER_WRITE_KP_END(w);
      stFileWriterEndChunk(w);
      for(u32 i=0;i < klass->nbComponents;i++)
      {
        stComponent* comp = &klass->components[i];
        ST_UNUSED(comp); // Bugfix: VS2015. Doesn't recongise the use of 'comp' below in the ST_FILEWRITER_ macros.
        stFileWriterBeginChunk(w, "COMP", comp->id);
          ST_FILEWRITER_WRITE_KP_BEGIN(w);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_SIZE, u32, comp->memSize);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_ID,   u16, comp->id);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_KIND, u16, comp->kind);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_CAPACITY, s32, comp->capacity);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_TYPE, u8,  comp->type);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_NB_ELEMENTS, u8,  comp->nbElements);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FLAGS, u8,  comp->flags);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_UNITS, u8,  comp->units);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_REFERENCE, u8,   comp->reference);
          ST_FILEWRITER_WRITE_KP_END(w);
        stFileWriterEndChunk(w);
      }
      stFileWriterBeginChunk(w, "OBJS", 0);
      {
        stFileWriterWriteU32(w, klass->objects.nbDWords, ST_ENDIAN);
        stFileWriterWriteData(w, (u8*) klass->objects.bits, sizeof(u32) * klass->objects.nbDWords);
      }
      stFileWriterEndChunk(w);
      stFileWriterTerminate(w);
    stFileWriterEndChunk(w);
  ST_LIST_FOR_END(stClass, classes, klass);
}

stEnum32 serializerDeltaOpen(stWriteContextData* context, const char* path, stEnum32 version, stEnum32 flags, stFrameDatas* frameDatas, stClasses* classes)
{
  if (stFileWriterOpen(&context->delta.writer, path, version, ST_SERIALISER_TYPE_DELTA, flags) == -1)
    return -1;

  context->delta.type = ST_SERIALISER_TYPE_DELTA;
  context->delta.begin = 0;
  context->delta.end = 0;
  context->delta.classes = classes;
  context->delta.wroteClasses = false;
  context->delta.frameDatas = frameDatas;
  context->delta.serialiseCookie = stGenerate32BitCookie((u32) (size_t) context);

  if (classes->first != NULL)
  {
    context->delta.wroteClasses = true;
    writeClasses(context, classes);
  }

  return 0;
}

void stWriterDeltaClose(stWriteContextData* context, stBool keepFrames)
{
  stWriterDeltaUpdate(context, keepFrames);   // Write any remaining data
  writeIdChange(context, stGetContext_NoMutex());     // Write out the Frame Next Id Parameters.
  stFileWriterClose(&context->delta.writer);
}

static void writeFrameMarker(stFileWriter* w, stFrameData* dataFrame, stHeaderFrame* header)
{
  ST_PRINT_CHATTERF("Saving Frame Id=%08x, SeqNum=%i", header->frameId, header->sequenceNum);
  
  // FRAME
  // =============================
  stFileWriterBeginChunk(w, "FRAM", header->frameId);
  {
    // META
    // =============================
    // ID                           u32   header->frameId
    // FRAME_SEQUENCE               u32   header->frameSequence
    // FRAME_LENGTH                 f64   header->frameLength
    // TIME                         f64   header->time
    // FLAGS                        u8    see stSaveHeaderFrameFlags
    // ID_MIN                       u32   header->idMin
    // ID_MAX                       u32   header->idMax
    stFileWriterBeginChunk(w, "META", 0);
    {
      ST_FILEWRITER_WRITE_KP_BEGIN(w);
        ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_ID, u32, header->frameId);
        ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FRAME_SEQUENCE, u32, header->sequenceNum);
        ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FRAME_TIME, f64, header->frameTime);
        ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FRAME_LENGTH, f64, header->frameLength);
        u8 flags = stSaveHeaderFrameFlags(header);
        ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FLAGS, u8, flags);
        ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_ID_MIN, u32, header->idMin);
        ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_ID_MAX, u32, header->idMax);
      ST_FILEWRITER_WRITE_KP_END(w);
    }
    stFileWriterEndChunk(w);

#if ST_CFG_USE_IDMAP == 1
    // IDMP
    // =============================
    // Number of Double Words         u32   header->idMap.nbDWords
    // For each bit:
    //    Object Id is set            u1    header->idMap.bits
    stFileWriterBeginChunk(w, "IDMP", 0);
    {
      stWriteBuffer_Flush(&w->buffer);
      stWriteBuffer_FastU32(&w->buffer, header->idMap.nbDWords);
      for(u32 i=0;i < header->idMap.nbDWords;i++)
      {
        stWriteBuffer_FastU32(&w->buffer, header->idMap.bits[i]);
      }
      stWriteBuffer_Flush(&w->buffer);
    }
    stFileWriterEndChunk(w);
#endif

    // OHDR
    // =============================
    // Object Count                 u32   data->objects.count
    // For each stObjectHeader as object  data->objects.first:
    //    u32                       u32   object->objectId
    //    count                     u8    object->count
    //    flags                     u8    see stSaveObjectFlags
    //    For each keyValue stDeltaKeyValue as keyValue  object->keys:
    //      key                     u64     keyValue->key
    //      data                    u8[n]   keyValue->data, where n is stGetCommandSize(key)
    stFileWriterBeginChunk(w, "OHDR", 0);
    {
      stWriteBuffer_Flush(&w->buffer);
      stWriteBuffer_FastU32(&w->buffer, dataFrame->objects.count);

      ST_LIST_FOR_BEGIN(stObjectHeader, (&dataFrame->objects), object);
      {
        if (object->bPersistant == false)
          continue;

        stWriteBuffer_FastU32(&w->buffer, object->objectId);
        stWriteBuffer_FastU8(&w->buffer, (u8) object->count);
        u8 flags = stSaveObjectFlags(object);
        stWriteBuffer_FastU8(&w->buffer, flags);

        stDeltaKeyValue* keyValue = object->keys;

        for(u32 i=0;i < object->count;i++)
        {
          u64 key = keyValue->key;
          u32 size = stGetCommandSize(key);

          // Inline Arrays (first byte is the size)
          if (stGetCommandArrayType(key) == ST_ARRAY_TYPE_INLINE)
          {
            size = stGetCommandCustomSize(key);
          }

          stWriteBuffer_Write(&w->buffer, &key, sizeof(u64));
          
          if (size == 0)
          {
            keyValue = keyValue->next;
            continue;
          }

          u8* data = keyValue->data;

          stWriteBuffer_Write(&w->buffer, data, size);

          keyValue = keyValue->next;
        }
      }

      ST_LIST_FOR_END(stObjectHeader, (&dataFrame->objects), object);
      stWriteBuffer_Flush(&w->buffer);
    }
    stFileWriterEndChunk(w);

    // ODAT -- Object Data
    // ===================
    ST_LIST_FOR_BEGIN(struct stObjectDataT, (&dataFrame->objectDatas), objectData);
    {
      stBool loadTemp = false;

      if (objectData->bLoaded == false)
      {
        loadTemp = true;
        stObjectData_LoadImpl(objectData);
      }

      stWriteBuffer_Flush(&w->buffer);

      stFileWriterBeginChunk(w, "ODAT", objectData->id);
      {
        stFileWriterBeginChunk(w, "ODHD", objectData->id);
        {
          ST_FILEWRITER_WRITE_KP_BEGIN(w);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_TYPE, u8, objectData->type);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_NB_ELEMENTS, u8, objectData->nbElements);
            ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_SIZE, u32, objectData->arraySize);
          ST_FILEWRITER_WRITE_KP_END(w);
        }
        stFileWriterEndChunk(w);
        stFileWriterBeginChunk(w, "ODDA", objectData->id);
        {
          stWriteBuffer_Write(&w->buffer, objectData->data, objectData->memSize);
          stWriteBuffer_Flush(&w->buffer);
        }
        stFileWriterEndChunk(w);
        stFileWriterTerminate(w);
      }
      stFileWriterEndChunk(w);

      if (loadTemp)
      {
        stObjectData_UnloadImpl(objectData);
      }

    }
    ST_LIST_FOR_END(struct stObjectDataT, (&dataFrame->objectDatas), objectData);

    stFileWriterTerminate(w);
  }
  stFileWriterEndChunk(w);

  fflush(w->buffer.file);
}

void stWriterDeltaUpdate(stWriteContextData* context, stBool keepFrames)
{
  if (context->delta.wroteClasses == false && context->delta.classes->first != NULL)
  {
    context->delta.wroteClasses = true;
    writeClasses(context, context->delta.classes);
  }

  u32 count = 0;
  ST_LIST_COUNT(count, stFrameData, (context->delta.frameDatas));

  ST_LIST_FOR_BEGIN(stFrameData, (context->delta.frameDatas), dataFrame);
  {
    stHeaderFrame* headerFrame = dataFrame->header;
    
    // Already saved?
    if (headerFrame->bSaved && headerFrame->savedVersion == context->delta.serialiseCookie)
    {
      ST_LIST_FOR_CONTINUE(stFrameData, (context->delta.frameDatas), dataFrame);
    }

    // Not for saving or deleted?
    if (headerFrame->bPersistant == false || headerFrame->bDeleted == true)
    {
      //ST_PRINT_CHATTERF("Skip2=%i:%i, Persistant=%i, Deleted=%i\n", headerFrame->frameId, headerFrame->sequenceNum, headerFrame->bPersistant, headerFrame->bDeleted);
      ST_LIST_FOR_CONTINUE(stFrameData, (context->delta.frameDatas), dataFrame);
    }

    // Mark as saved, and then write to disk.
    headerFrame->bSaved = true;
    headerFrame->savedVersion = context->delta.serialiseCookie;
    writeFrameMarker(&context->delta.writer, headerFrame->data, headerFrame);

    if (!keepFrames)
    {
      stUnloadFrame(context->delta.frameDatas, headerFrame);
    }

  }
  ST_LIST_FOR_END(stFrameData, (context->delta.frameDatas), dataFrame);
}
