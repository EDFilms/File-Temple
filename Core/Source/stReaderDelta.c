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

#include "stReaderDelta.h"
#include "stObject.h"
#include "stList.h"
#include "stCommand.h"
#include "stFile.h"
#include "stSymbols.h"
#include "stObject.h"
#include "stError.h"
#include "stRecording.h"
#include "stObjectData.h"
#include "stCommandKey.h"

static void readObjectDataImpl(stFileReader* r, struct stObjectDataT* objectData, stBool peekMode);

static void readClasses(stReadContext* context, stClasses* classes, u32 objectTypeId)
{
  stFileReader* r = &context->delta.reader;
  
  stClass* klass = stCreateObjectTypeImpl(objectTypeId, classes);
  stComponent* comp = NULL;

  u8 key, lastKey = 0;
  stEnum32 valueType;
  stFourCc header;
  u32 headerUint;
  u32 number, length;

  while(stFileReaderReadChunk(r, &header, &number, &length))
  {
    headerUint = stFourCcToUInt(&header);
    switch(headerUint)
    {
      default:
      {
        ST_PRINT_ERRORF("Unknown ObjectType Chunk %c%c%c%c\n", header.c[0], header.c[1], header.c[2], header.c[3]);
        stFileReaderSkipChunk(r, length);
      }
      break;
      case 'META':
      {
        while(stFileReaderProperty(r, &key, &valueType))
        {
          //  stFileWriterBeginChunk(w, "META", 0);
          //  ST_FILEWRITER_WRITE_KP_BEGIN(w);
          //  ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_ID, u32, klass->id);
          //  ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_USES, u32, klass->nbUses);
          //  ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_NB_COMPONENTS, u8, klass->nbComponents);
          //  ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FREQUENCY, u8, klass->frequency);
          //  ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FLAGS, u8, klass->flags);
          //  ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_RESERVED1, u8, klass->reserved1);
          //  ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_RESERVED2, u8, klass->reserved2);
          //  ST_FILEWRITER_WRITE_KP_END(w);
          //  stFileWriterEndChunk(w);
          switch(key)
          {
            default:
              ST_PRINT_ERRORF("Unknown object type meta key %i %i\n", key, lastKey);
              stFileReaderSkipProperty(r, valueType);
            break;
            case ST_SYMBOL_ID: 
              ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
              stFileReaderReadU32(r, &klass->id, ST_ENDIAN);
            break;
            case ST_SYMBOL_USES: 
              ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
              stFileReaderReadU32(r, &klass->nbUses, ST_ENDIAN);
            break;
            case ST_SYMBOL_NB_COMPONENTS: 
              ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
              stFileReaderReadU8(r, &klass->nbComponents, ST_ENDIAN);
            break;
            case ST_SYMBOL_FREQUENCY: 
              ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
              stFileReaderReadU8(r, &klass->frequency, ST_ENDIAN);
            break;
            case ST_SYMBOL_FLAGS: 
              ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
              stFileReaderReadU8(r, &klass->flags, ST_ENDIAN);
            break;
            case ST_SYMBOL_USERDATA: 
              ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
              stFileReaderReadU32(r, &klass->userData, ST_ENDIAN);
            break;
            case ST_SYMBOL_RESERVED1: 
              ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
              stFileReaderReadU8(r, &klass->reserved1, ST_ENDIAN);
            break;
            case ST_SYMBOL_RESERVED2: 
              ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
              stFileReaderReadU8(r, &klass->reserved2, ST_ENDIAN);
            break;
          }

          lastKey = key;
        }
      }
      break;
      case 'COMP':
        comp = stAddObjectTypeComponentImpl(number, klass);

        while(stFileReaderProperty(r, &key, &valueType))
        {
          //    ST_FILEWRITER_WRITE_KP_BEGIN(w);
          //    ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_SIZE, u32, comp->memSize);
          //    ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_ID,   u16, comp->id);
          //    ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_KIND, u16, comp->kind);
          //    ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_CAPACITY, s32, comp->capacity);
          //    ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_TYPE, u8,  comp->type);
          //    ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_NB_ELEMENTS, u8,  comp->nbElements);
          //    ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_FLAGS, u8,  comp->flags);
          //    ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_UNITS, u8,  comp->units);
          //    ST_FILEWRITER_WRITE_KP_VALUE(w, ST_SYMBOL_REFERENCE, u8,   comp->reference);
          //    ST_FILEWRITER_WRITE_KP_END(w);
          //    stFileWriterEndChunk(w);
          switch(key)
          {
            default:
              ST_PRINT_ERRORF("Unknown object type meta key %i %i\n", key, lastKey);
              stFileReaderSkipProperty(r, valueType);
            break;
            case ST_SYMBOL_SIZE: 
              ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
              stFileReaderReadU32(r, &comp->memSize, ST_ENDIAN);
              //ST_PRINT_REALLY_CHATTERF("Component:MemSize = %i", comp->memSize);
            break;
            case ST_SYMBOL_ID: 
              ST_ASSERT(valueType == ST_TYPE_UINT16, "Unexpected type");
              stFileReaderReadU16(r, &comp->id, ST_ENDIAN);
              //ST_PRINT_REALLY_CHATTERF("Component:Id = %i", comp->id);
            break;
            case ST_SYMBOL_KIND: 
              ST_ASSERT(valueType == ST_TYPE_UINT16, "Unexpected type");
              stFileReaderReadU16(r, &comp->kind, ST_ENDIAN);
              //ST_PRINT_REALLY_CHATTERF("Component:Kind = %i", comp->kind);
            break;
            case ST_SYMBOL_CAPACITY: 
              ST_ASSERT(valueType == ST_TYPE_INT32, "Unexpected type");
              stFileReaderReadS32(r, &comp->capacity, ST_ENDIAN);
              //ST_PRINT_REALLY_CHATTERF("Component:Capacity = %i", comp->capacity);
            break;
            case ST_SYMBOL_TYPE: 
              ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
              stFileReaderReadU8(r, &comp->type, ST_ENDIAN);
              //ST_PRINT_REALLY_CHATTERF("Component:Type = %i", comp->type);
            break;
            case ST_SYMBOL_NB_ELEMENTS: 
              ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
              stFileReaderReadU8(r, &comp->nbElements, ST_ENDIAN);
              //ST_PRINT_REALLY_CHATTERF("Component:nbElements = %i", comp->nbElements);
            break;
            case ST_SYMBOL_FLAGS: 
              ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
              stFileReaderReadU8(r, &comp->flags, ST_ENDIAN);
              //ST_PRINT_REALLY_CHATTERF("Component:flags = %i", comp->flags);
            break;
            case ST_SYMBOL_UNITS: 
              ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
              stFileReaderReadU8(r, &comp->units, ST_ENDIAN);
              //ST_PRINT_REALLY_CHATTERF("Component:units = %i", comp->units);
            break;
            case ST_SYMBOL_REFERENCE: 
              ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
              stFileReaderReadU8(r, &comp->reference, ST_ENDIAN);
              //ST_PRINT_REALLY_CHATTERF("Component:reference = %i", comp->units);
            break;
          }
        }
      break;
      case 'OBJS':
      {
        u32 nbDWords = 0;
        stFileReaderReadU32(r, &nbDWords, ST_ENDIAN);
        stDynamicBitSet_Shutdown(&klass->objects); 
        stDynamicBitSet_Initialise2(&klass->objects, nbDWords);
        stFileReaderReadData(r, (u8*) klass->objects.bits, nbDWords * sizeof(u32));
      }
      break;
    }
  }
  
}

// META
// =============================
// ID                           u32   header->frameId
// FRAME_SEQUENCE               u32   header->frameSequence
// FRAME_LENGTH                 f64   header->frameLength
// TIME                         f64   header->time
// FLAGS                        u8    see stSaveHeaderFrameFlags
// ID_MIN                       u32   header->idMin
// ID_MAX                       u32   header->idMax
static void readFrameMetaChunk(stFileReader* r, stHeaderFrame* frame)
{
  u8 key;
  stEnum32 valueType;
  
  while(stFileReaderProperty(r, &key, &valueType))
  {
    switch(key)
    {
      default:
      {
        stFileReaderSkipProperty(r, valueType);
      }
      break;
      case ST_SYMBOL_FRAME_ID:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
        u32 id = frame->frameId;
        stFileReaderReadU32(r, &id, ST_ENDIAN);
        ST_ASSERT(id != frame->frameId, "Frame Ids are not identicial");
      }
      break;
      case ST_SYMBOL_FRAME_SEQUENCE:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
        stFileReaderReadU32(r, &frame->sequenceNum, ST_ENDIAN);
      }
      break;
      case ST_SYMBOL_FRAME_TIME:
      {
        ST_ASSERT(valueType == ST_TYPE_FLOAT64, "Unexpected type");
        stFileReaderReadF64(r, &frame->frameTime, ST_ENDIAN);
      }
      break;
      case ST_SYMBOL_FRAME_LENGTH:
      {
        ST_ASSERT(valueType == ST_TYPE_FLOAT64, "Unexpected type");
        stFileReaderReadF64(r, &frame->frameLength, ST_ENDIAN);
      }
      break;
      case ST_SYMBOL_FLAGS:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
        u8 flags;
        stFileReaderReadU8(r, &flags, ST_ENDIAN);
        stLoadHeaderFrameFlags(flags, frame);
      }
      break;
      case ST_SYMBOL_ID_MIN:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
        stFileReaderReadU32(r, &frame->idMin, ST_ENDIAN);
      }
      break;
      case ST_SYMBOL_ID_MAX:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
        stFileReaderReadU32(r, &frame->idMax, ST_ENDIAN);
      }
      break;
    }
  }
}

#if ST_CFG_USE_IDMAP == 1
// IDMP
// =============================
// Number of Double Words         u32   header->idMap.nbDWords
// For each bit:
//    Object Id is set            u1    header->idMap.bits
static void readIdMapChunk(stFileReader* r, stHeaderFrame* frame)
{
  u32 nbDWords;
  stFileReaderReadU32(r, &nbDWords, ST_ENDIAN);

  if (nbDWords == 0)
    return;

  stBitSet_Shutdown(&frame->idMap);
  stBitSet_Initialise2(&frame->idMap, nbDWords);

  stFileReaderReadData(r, (u8*) frame->idMap.bits, nbDWords * sizeof(u32));
}
#endif

// OHDR
// =============================
// Object Count                 u32   data->objects.count
// For each stObjectHeader as object  data->objects.first:
//    u32                       u32   object->objectId
//    count                     u8    object->count
//    flags                     u8    see stLoadObjectFlags
//    For each keyValue stDeltaKeyValue as keyValue  object->keys:
//      key                     u64     keyValue->key
//      data                    u8[n]   keyValue->data, where n is stGetCommandSize(key)
static void readObjectHeaderChunk(stFileReader* r, stFrameData* frame)
{
  u32 count;
  stFileReaderReadU32(r, &count, ST_ENDIAN);
  for(u32 i=0;i < count;i++)
  {
    u32 objectId;
    stFileReaderReadU32(r, &objectId, ST_ENDIAN);
    stObjectHeader* objectHeader = stNewObjectHeaderInDataFrame(frame, objectId);
    objectHeader->bPersistant = true;
    objectHeader->bLoaded = true;

    u8 commandCount;
    stFileReaderReadU8(r, &commandCount, ST_ENDIAN);
    objectHeader->count = commandCount;

    u8 flags;
    stFileReaderReadU8(r, &flags, ST_ENDIAN);
    stLoadObjectFlags(flags, objectHeader);

    for(u8 j=0;j < commandCount;j++)
    {
      u64 cmdKey = 0;
      u32 cmdSize = 0;

      stFileReaderReadU64(r, &cmdKey, ST_ENDIAN);
      stDeltaKeyValue* keyValue = stNewCommandInFrameDataImpl(frame, cmdKey, &cmdSize);
      stFileReaderReadData(r, keyValue->data, cmdSize);

      if (objectHeader->keys == NULL)
      {
        objectHeader->keys = keyValue;
      }

    }
    
  }
}

// ODHD   -- ObjectData Header
// =============================
// type               TYPE            u8   objectData->type
// nbElements         NB_ELEMENTS     u8   objectData->nbElements
// arraySize          SIZE            u32  objectData->arraySize
void readObjectDataMetaChunk(stFileReader* r, struct stObjectDataT* objectData)
{
  u8 key;
  stEnum32 valueType;

  while(stFileReaderProperty(r, &key, &valueType))
  {
    switch(key)
    {
      default:
      {
        stFileReaderSkipProperty(r, valueType);
      }
      break;
      case ST_SYMBOL_TYPE:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
        stFileReaderReadU8(r, &objectData->type, ST_ENDIAN);
      }
      break;
      case ST_SYMBOL_NB_ELEMENTS:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT8, "Unexpected type");
        stFileReaderReadU8(r, &objectData->nbElements, ST_ENDIAN);
      }
      break;
      case ST_SYMBOL_SIZE:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
        stFileReaderReadU32(r, &objectData->arraySize, ST_ENDIAN);
      }
      break;
    }
  }
}

// ODDA - ObjectData Data
// <data>                       u32   objectData->mem  (memSize is implied through stGetTypeMemorySize)
void readObjectDataData(stFileReader* r, struct stObjectDataT* objectData)
{
  u32 memSize = stObjectData_ReserveDataForLoading(objectData);
  stFileReaderReadData(r, objectData->data, memSize);
}

static void readIdsChunk(stFileReader* r, stContext* ctx)
{
  u8 key;
  stEnum32 valueType;

  while(stFileReaderProperty(r, &key, &valueType))
  {
    switch(key)
    {
      default:
      {
        stFileReaderSkipProperty(r, valueType);
      }
      break;
      case ST_SYMBOL_HEADER_ID_INDEX:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
        stFileReaderReadU32(r, &ctx->headerFrames.nextFrameId.index, ST_ENDIAN);
      }
      break;
      case ST_SYMBOL_HEADER_ID_OFFSET:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
        stFileReaderReadU32(r, &ctx->headerFrames.nextFrameId.offset, ST_ENDIAN);
      }
      break;
      case ST_SYMBOL_RECORDING_TIME:
      {
        ST_ASSERT(valueType == ST_TYPE_FLOAT64, "Unexpected type");
        stFileReaderReadF64(r, &ctx->recordingTime, ST_ENDIAN);
      }
      break;
      case ST_SYMBOL_FRAME_COUNT:
      {
        ST_ASSERT(valueType == ST_TYPE_UINT32, "Unexpected type");
        stFileReaderReadU32(r, &ctx->nbSubmittedFrames, ST_ENDIAN);
      }
      break;
    }
  }
}

static void readFrameDataImpl(stFileReader* r, stHeaderFrame* frame, stFrameData* frameData)
{
  stFourCc header;
  u32 number, length;

  stBool peekMode = (frameData == NULL);

  if (peekMode)
  {
    frame->persistantOffset = stFileReaderGetPosition(r);
  }

  while(stFileReaderReadChunk(r, &header, &number, &length))
  {
    switch(stFourCcToUInt(&header))
    {
      default:
      {
        ST_PRINT_ERRORF("Unknown frame chunk %c%c%c%c at HEAD\n", header.c[0], header.c[1], header.c[2], header.c[3]);
        stFileReaderSkipChunk(r, length);
      }
      break;
      case 'META':
      {
        if (peekMode)
        {
          readFrameMetaChunk(r, frame);
        }
        else
        {
          stFileReaderSkipChunk(r, length);
        }
      }
      break;
#if ST_CFG_USE_IDMAP == 1
      case 'IDMP':
      {
        if (peekMode)
        {
          readIdMapChunk(r, frame);
        }
        else
        {
          stFileReaderSkipChunk(r, length);
        }
      }
      break;
#endif
      case 'OHDR':
      {
        if (peekMode)
        {
          stFileReaderSkipChunk(r, length);
        }
        else
        {
          readObjectHeaderChunk(r, frameData);
        }
      }
      break;
      case 'ODAT':
      {
        if (frameData != NULL)
        {
          struct stObjectDataT* objectData = stObjectData_Obtain_ForReader(&frameData->objectDatas, number);
          readObjectDataImpl(r, objectData, true);
        }
        else
        {
          stFileReaderSkipChunk(r, length);
        }
      }
      break;
    }
  }

}


static void readObjectDataImpl(stFileReader* r, struct stObjectDataT* objectData, stBool peekMode)
{
  stFourCc header;
  u32 number, length;

  if (peekMode)
  {
    objectData->persistantOffset = stFileReaderGetPosition(r);
  }

  while(stFileReaderReadChunk(r, &header, &number, &length))
  {
    switch(stFourCcToUInt(&header))
    {
      default:
      {
        ST_PRINT_ERRORF("Unknown frame chunk %c%c%c%c at HEAD\n", header.c[0], header.c[1], header.c[2], header.c[3]);
        stFileReaderSkipChunk(r, length);
      }
      break;
      case 'ODHD':
      {
        if (peekMode)
        {
          readObjectDataMetaChunk(r, objectData);
        }
        else
        {
          stFileReaderSkipChunk(r, length);
        }
        // Read meta information
      }
      break;
      case 'ODDA':
      {
        if (peekMode)
        {
          stFileReaderSkipChunk(r, length);
        }
        else
        {
          readObjectDataData(r, objectData);
        }
      }
      break;
    }
  }

}

static void peekFrameData(stReadContext* context, stHeaderFrames* headerFrames, u32 id)
{
  stFileReader* r = &context->delta.reader;
  stHeaderFrame* frameHeader = stNewHeaderFrameWithId(headerFrames, id); 

  readFrameDataImpl(r, frameHeader, NULL);
}

stEnum32 stReaderFrameData(stReadContext* context, struct stFrameDataT* frameData, struct stHeaderFrameT* header)
{
  stFileReader* r = &context->delta.reader;
  stFileReaderSeek(r, header->persistantOffset);
  
  readFrameDataImpl(r, header, frameData);
  return 0;
}

stEnum32 stReaderObjectData(stReadContext* context, struct stObjectDataT* objectData)
{
  stFileReader* r = &context->delta.reader;
  stFileReaderSeek(r, objectData->persistantOffset);

  readObjectDataImpl(r, objectData, false);
  return 0;
}

stEnum32 stReaderDeltaFile(stReadContext* context, const char* path, struct stFrameDatasT* frameDatas, struct stHeaderFramesT* headerFrames, stClasses* classes)
{

  stEnum32 fileType, flags;
  if (stFileReaderOpen(&context->delta.reader, path, &fileType, &flags) == -1)
  {
    ST_PRINT_ERRORF("File %s was not opened\n", path);
    return -1;
  }
  context->delta.type = ST_SERIALISER_TYPE_DELTA;
  context->delta.frameDatas = frameDatas;
  context->delta.headerFrames = headerFrames;
  context->delta.lastHeaderFrame = NULL;

  stFourCc header;
  u32      headerUint, number, length;
  
  while(stFileReaderReadChunk(&context->delta.reader, &header, &number, &length))
  {
    headerUint = stFourCcToUInt(&header);
    switch(headerUint)
    {
      case 'CLAS':
        readClasses(context, classes, number);
      break;
      case 'FRAM':
        peekFrameData(context, headerFrames, number);
      break;
      case 'IDCH':
        readIdsChunk(&context->delta.reader, stGetContext_NoMutex());
      break;
    }
  }

  if (context->delta.lastHeaderFrame != NULL && headerFrames->first == NULL)
  {
    headerFrames->first = context->delta.lastHeaderFrame;
  }

  return 0;
}


void stReaderDeltaFileClose(stReadContext* context)
{
  stFileReaderClose(&context->delta.reader);
}
