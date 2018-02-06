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

#define ST_LOCAL_CFG_DEBUG_ITERATOR 0

#include "stIterator.h"
#include "stRecording.h"
#include "stError.h"
#include "stList.h"
#include "stCommandKey.h"
#include "stTypes.h"
#include "stProbe.h"
#include "stBinary.h"
#include <Source/Source/stStride.h>

#define ST_FIRST_AVAILABLE_OBJECT                   (UINT32_MAX) - 0x01
#define ST_FIRST_AVAILABLE_OBJECT_BUT_DONT_AUTOLOAD (UINT32_MAX) - 0x02

#if ST_LOCAL_CFG_DEBUG_ITERATOR == 0
#define ST_DEBUG_ITERATOR(COMMENT) stIteratorDebug(iterator, COMMENT)
#else
#define ST_DEBUG_ITERATOR(IGNORED_COMMENT)
#endif

struct stIteratorParams
{
  stBool32                    bIterateFrames  : 1;
  stBool32                    bAdjustFrame    : 1;
  stBool32                    bLazyLoadFrame  : 1;
  stBool32                    bIterateObjects : 1;
  stBool32                    bAdjustObject   : 1;
  stBool32                    bLazyLoadObject : 1;
  stBool32                    bIterateValues  : 1;
  stBool32                    bAdjustValues   : 1;
  stBool32                    bLazyLoadValues : 1;

  u32                         frameSeq,   frameMin,  frameMax;
  u32                         objectId,   objectMin, objectMax;
  u32                         valueIndex, valueMin,  valueMax;
  enum stKeyValueIteratorType valueType;
};

ST_INLINE stHeaderFrame* stLazyLoadOrGet_stLazyHeaderFrame(stLazyHeaderFrame* lazyHeaderFrame)
{
  // Note: Id is treated as sequenceNumber, not the frame number

  stHeaderFrames* headerFrames = lazyHeaderFrame->p;
  return stFindHeaderFrameBySeqNumOrNearest(headerFrames, lazyHeaderFrame->id);
}

ST_INLINE stObjectHeader* stLazyLoadOrGet_stLazyObjectHeader(stLazyObjectHeader* lazyObjectHeader)
{
  stHeaderFrame* headerFrame = lazyObjectHeader->p;

  // This can be very much improved by additional functions to check to see if the id exists
  // in the frame using the id bitmap, and idMin and idMax.

  if (headerFrame->bLoaded == false)
  {
    if (stLoadFrame_NoMutex(headerFrame) == false)
    {
      return NULL;
    }

    if (lazyObjectHeader->id == 0)
      return NULL;

    stObjectHeader* header = stFindObjectInFrameData(lazyObjectHeader->id, headerFrame->data);
    ST_ASSERT(header != NULL, "Object header is expected");
    return header;
  }
  
  return NULL;
}

ST_INLINE stDeltaKeyValue* stLazyLoadOrGet_stLazyDeltaKeyValue(stLazyDeltaKeyValue* lazyDeltaKeyValue)
{
  stObjectHeader* header = lazyDeltaKeyValue->p.objectHeader;

  switch(lazyDeltaKeyValue->p.valueType)
  {
    default:
    case ST_VALUE_ITERATOR_TYPE_FORWARD_ITERATOR:
    {
      // TODO: Will need to start caching this, the last value, especially if the index increases each time.
      stDeltaKeyValue* kv = header->keys;
      u32 i=0;

      for(;i < lazyDeltaKeyValue->id;i++)
      {
        kv = kv->next;
        if (kv == NULL)
        {
          break;
        }
      }

      return kv;
    }
    break;
    case ST_VALUE_ITERATOR_TYPE_COMPONENT_TYPE:
    {
      stDeltaKeyValue* kv = header->keys;
      for(u32 i=0;i < header->count;i++)
      {
        if (stGetCommandComponent(kv->key) == lazyDeltaKeyValue->id)
        {
          return kv;
        }
        kv = kv->next;
      }
    }
    break;
  }

  return NULL;
}

static void setIteratorParamsDefault(struct stIteratorParams* p)
{
  p->bIterateFrames  = true;
  p->bAdjustFrame    = true;
  p->bLazyLoadFrame  = true;
  p->frameSeq        = 0;
  p->frameMin        = 0;
  p->frameMax        = ST_ITERATOR_MAX_ID;

  p->bIterateObjects = true;
  p->bAdjustObject   = true;
  p->bLazyLoadObject = true;
  p->objectId        = 0;
  p->objectMin       = 0;
  p->objectMax       = ST_ITERATOR_MAX_ID;

  p->bIterateValues  = true;
  p->bAdjustValues   = true;
  p->bLazyLoadValues = true;
  p->valueIndex      = 0;
  p->valueMin        = 0;
  p->valueMax        = ST_ITERATOR_MAX_ID;
  p->valueType       = ST_VALUE_ITERATOR_TYPE_FORWARD_ITERATOR;

}

static stBool validateFrameIterator(stIterator* iterator);
static stBool startIterator(stIterator* iterator);
static stBool nextIterator(stIterator* iterator);

static stIterator* createIterator(stContext* ctx, struct stIteratorParams* params)
{
  stIterator* iterator = NULL;

  if (ctx->iterators.pool > 0)
  {
    ST_LIST_FOR_BEGIN(stIterator, (&ctx->iterators), it);
    {
      if (it->bDeleted)
      {
        iterator = it;
        break;
      }
    }
    ST_LIST_FOR_END(stFrameIterator, (&ctx->frameIterators), it);
  }
  else
  {  
    iterator = ST_DYNAMIC_LINEAR_ALLOCATOR_OBTAIN(stIterator, &ctx->iterators.mem);
    memset(iterator, 0, sizeof(stIterator));
    ST_LIST_ADD((&ctx->iterators), iterator);
  }

  if (iterator == NULL)
  {
    stRaiseError(ST_ERROR_ITERATOR);
    stReleaseContextLock(ctx);
    return NULL;
  }

  iterator->iteratorId                = stQpr_Next(&ctx->iterators.nextId);
  iterator->headerFrames              = &ctx->headerFrames;
  iterator->frame.id                  = params->frameSeq;
  iterator->frame.object              = NULL;
  iterator->frame.bLazyLoad           = params->bLazyLoadFrame;
  iterator->frame.bAdjustmentAllowed  = params->bAdjustFrame;
  iterator->frame.p                   = (&ctx->headerFrames);
  iterator->bIterateFrames            = params->bIterateFrames;

  iterator->object.id                 = params->objectId;
  iterator->object.object             = NULL;
  iterator->object.bLazyLoad          = params->bLazyLoadFrame;
  iterator->object.bAdjustmentAllowed = params->bAdjustObject;
  iterator->object.p                  = NULL;
  iterator->bIterateObjects           = params->bIterateObjects;

  iterator->value.id                  = params->valueIndex;
  iterator->value.object              = NULL;
  iterator->value.bLazyLoad           = params->bLazyLoadValues;
  iterator->value.bAdjustmentAllowed  = params->bAdjustObject;
  iterator->value.p.objectHeader      = NULL;
  iterator->value.p.valueType         = 0;
  iterator->bIterateValues            = params->bIterateValues;

  iterator->lastEventId               = 0;
  iterator->eventClass                = NULL;

  iterator->bGcUnloadLastFrame        = true;

  ST_DEBUG_ITERATOR("CreateIterator.BeforeStart");

  if (!startIterator(iterator))
  {
    ST_DEBUG_ITERATOR("CreateIterator.FailedStart");
    stRaiseError(ST_ERROR_ITERATOR);
    ST_PRINT_ERRORF("Frame iterator was invalid for %i", params->frameSeq);
    iterator->bDeleted = true;
    ctx->iterators.pool++;
    stReleaseContextLock(ctx);
    return NULL;
  }

  ST_DEBUG_ITERATOR("CreateIterator.AfterStart");

  return iterator;
}

static void deleteIterator(stIterators* iterators, stIterator* iterator)
{
  if (iterator->bDeleted == false)
  {
    iterator->bDeleted = true;
    iterators->pool++;
  }
}

static stIterator* findIterator(stIterators* iterators, u32 iteratorId)
{
  ST_UNUSED(iteratorId);    // Visual Studio doesn't recongise the macro of using iteratorId below.
  ST_LIST_FOR_BEGIN(stIterator, iterators, it);
  {
    if (it->iteratorId && !it->bDeleted)
      return it;
  }
  ST_LIST_FOR_END(stIterator, iterators, it);

  return NULL;
}

static void stResolveEventClass(stIterator* iterator)
{
  if (stGetCommandType(iterator->value.object->key) == ST_CMD_OBJ_NEW)
  {
    u32* data = (u32*) iterator->value.object->data;
    u32 classId = (*data);
    
    if (iterator->eventClass != NULL && iterator->eventClass->id == classId)
      return;

    
    
    iterator->eventClass = stGetClass_NoMutex(classId);
  }
}

static stBool maybeMinIterator_Value(stIterator* iterator)
{
  iterator->value.id = iterator->valueRules.min;

  struct stLazyDeltaKeyValueData p;
  p.objectHeader = iterator->object.object;
  p.valueType = iterator->valueType;
  ST_LAZY_LOAD_INITIALISE(iterator->value, stLazyDeltaKeyValue, iterator->valueRules.min, true, p);
  
  if (stObjectIdIsEvent(iterator->object.id) && iterator->value.object != NULL && stGetCommandType(iterator->value.object->key) == ST_CMD_OBJ_NEW)
  {
    stResolveEventClass(iterator);
  }

  return iterator->value.object != NULL;
}

static stBool maybeNextIterator_Value(stIterator* iterator)
{

  if (iterator->valueType == ST_VALUE_ITERATOR_TYPE_FORWARD_ITERATOR)
  {
    
    //ST_PRINT_REALLY_CHATTERF("[Iterator][Next][ForwardValue][Before] It=%p, Id=%i, Value=%p", iterator, iterator->value.id, iterator->value.object);
    // ++ on id. until it meets count, then return NULL, otherwise go through.

    if (iterator->object.object == NULL)
      return false;

    if ((++iterator->value.id) == iterator->object.object->count)
      return false;

    struct stLazyDeltaKeyValueData p;
    p.objectHeader = iterator->object.object;
    p.valueType = iterator->valueType;

    ST_LAZY_FETCH_OBJECT(stLazyDeltaKeyValue, iterator->value);
    //ST_PRINT_REALLY_CHATTERF("[Iterator][Next][ForwardValue][After] It=%p, Id=%i, Value=%p", iterator, iterator->value.id, iterator->value.object);

    if (stObjectIdIsEvent(iterator->object.id) && iterator->value.object != NULL && stGetCommandType(iterator->value.object->key) == ST_CMD_OBJ_NEW)
    {
      stResolveEventClass(iterator);
    }

  }
  else
  {
    //ST_PRINT_REALLY_CHATTERF("[Iterator][Next][ComponentValue] It=%p, Comment=No Next.", iterator);
    return false; // For now.
  }

  return iterator->value.object != NULL;
}

stBool maybeMinIterator_Object(stIterator* iterator)
{
  u32 first = stMax(iterator->objectRules.min, iterator->frame.object->idMin);

  ST_LAZY_LOAD_INITIALISE(iterator->object, stLazyObjectHeader, first, true, iterator->frame.object);
  return iterator->object.object != NULL;
}

stBool maybeNextIterator_Object(stIterator* iterator)
{
  // The next object should be the next LL object in the objectHeader, if it's NULL then we've reached the end of the frame
  if (iterator->object.object == NULL)
    return false;

  stObjectHeader* next = iterator->object.object->next;
  if (next == NULL)
    return false;

  iterator->object.object = next;
  iterator->object.id     = next->objectId;

  if (stObjectIdIsEvent(iterator->object.id))
  {
    iterator->lastEventId = iterator->object.id;
  }


  return true;
}

stBool maybeMinIterator_Frame(stIterator* iterator)
{
  stHeaderFrames* headerFrames = iterator->headerFrames;
  ST_LAZY_LOAD_INITIALISE(iterator->frame, stLazyHeaderFrame, iterator->frameRules.min, true, headerFrames);
  return iterator->frame.object != NULL;
}

int stInternal_PrintMemoryAllocations();

stBool maybeNextIterator_Frame(stIterator* iterator)
{
  // The next frame should be the next LL frame in the frameHeader, if it's NULL then we've reached the end of the recording.
  // We assume that the frames are correctly sorted.
  if (iterator->frame.object == NULL)
    return false;

  stHeaderFrame* lastFrame = iterator->frame.object;

  stHeaderFrame* frame = iterator->frame.object->next;

  if (frame == NULL)
    return false;

  while(stHeaderFrameIsEmpty(frame))
  {
    ST_PRINT_CHATTERF("Frame %i is empty, skipping", frame->frameId);
    frame = frame->next;

    if (frame == NULL)
      return false;
  }

  iterator->frame.object = frame;
  iterator->frame.id     = frame->sequenceNum;

  if (lastFrame && iterator->bGcUnloadLastFrame)
  {
    stUnloadFrame_NoMutex(lastFrame);
  }

#if ST_CFG_TRACK_MEMORY == 1
  ST_PRINT_INFOF("Frame %i", iterator->frame.id);
  stInternal_PrintMemoryAllocations();
#endif

  return true;
}

stBool startIterator(stIterator* iterator)
{
  iterator->bEnded = false;
  bool isGoodIterator = true;

  // 1. Get and load the frame, regardless if we iterate it or not. We need *a* frame.
  //    In iterator->frame, id corresponds to sequenceNumber.
  ST_LAZY_LOAD_INITIALISE(iterator->frame, stLazyHeaderFrame, iterator->frameRules.min, true, iterator->headerFrames);
  
  // 2. Now find the object.
  //    Only fetch the object if we are allowed to iterate through objects
  //    Only fetch the object if there is a frame
  //    We should adjust min, to the minimum object (given in the object header) so it correctly adjusts.
  //    The frame should be loaded, but handledthrough the LazyLoadOrGet
  //    Set iterator->value. p to object

  if (iterator->bIterateObjects && iterator->frame.object != NULL)
  {
    if (!maybeMinIterator_Object(iterator))
    {
      ST_LAZY_LOAD_SHUTDOWN(iterator->object);
      isGoodIterator = false;
    }
  }
  else
  {
    ST_LAZY_LOAD_SHUTDOWN(iterator->object);
  }

  // 3. Now find the value
  //    Only fetch the value if there is an object
  //    Depending on the type, we should either:
  //      (Kind)        Check to see if the kind is part of the class, and get the offset.
  //      (Iterator)    Adjust id to 0 if necessary (which is unsigned).
  //    Fetch the value, if there is an object. Then the frame has been loaded, so don't need to worry about that.
  if (iterator->bIterateValues && iterator->object.object != NULL)
  {
    if (!maybeMinIterator_Value(iterator))
    {
      ST_LAZY_LOAD_SHUTDOWN(iterator->value);
      isGoodIterator = false;
    }
  }
  else
  {
    ST_LAZY_LOAD_SHUTDOWN(iterator->value);
  }

  return isGoodIterator;
}



static stBool attemptNextValue(stIterator* iterator)
{
  if (iterator->bIterateValues)
  {
    return maybeNextIterator_Value(iterator);
  }
  return false;
}

static stBool attemptNextObject(stIterator* iterator)
{
  if (iterator->bIterateObjects)
  {
    if (maybeNextIterator_Object(iterator))
    {
      ST_ASSERT(maybeMinIterator_Value(iterator) == true, "Expected at least one value in an object");   // These should return true.
      return true;
    }
  }
  return false;
}

static stBool attemptNextFrame(stIterator* iterator)
{
  if (iterator->bIterateFrames)
  {
    if (maybeNextIterator_Frame(iterator))
    {
      ST_ASSERT(maybeMinIterator_Object(iterator) == true, "Expected at least one object in a frame"); 
      ST_ASSERT(maybeMinIterator_Value(iterator) == true, "Expected at least one value in an object"); 

      return true;
    }
  }
  return false;
}

static stBool nextIteratorImpl(stIterator* iterator)
{
#if 1

  iterator->bEnded = false;

  if (attemptNextValue(iterator) == false)
  {
    if (attemptNextObject(iterator) == false)
    {
      if (attemptNextFrame(iterator) == false)
      {
        iterator->bEnded = true;
        return false; // End of iterator
      }
    }
  }

  return true;
#else
  // 3. Can we iterate values?
  //    Yes:
  //      Can we go next? 
  //        Yes:
  //          Next Value and RETURN
  //        No:
  //          Object should iterate
  //    No:
  //      Object should iterate
  if (iterator->bIterateValues)
  {
    if (maybeNextIterator_Value(iterator))
    {
      return true;
    }
  }

  // 2. Can Object iterate AND can we iterate objects?
  //    Yes:
  //      Can we go next?
  //        Yes:
  //          Next Object, and Min Value and RETURN
  //        No:
  //          Frame should iterate
  //    No:
  //      Frame should iterate
  if (iterator->bIterateObjects)
  {
    if (maybeNextIterator_Object(iterator))
    {
      maybeMinIterator_Value(iterator);
      return true; // Maybe???
    }
  }
  
  // 3. Can Frame iterate AND can we iterate frames?
  //    Yes:
  //      Can we go next?
  //        Yes:
  //          Next Frame, Min Object and Min Value and RETURN
  //        No:
  //          Iterator has ended
  //    No:
  //      Iterator has ended
  if (iterator->bIterateFrames)
  {
    if (maybeNextIterator_Frame(iterator))
    {
      if (maybeMinIterator_Object(iterator))
      {
        maybeMinIterator_Value(iterator);
      }
      return true;
    }
  }

  iterator->bEnded = true;
  return false;
#endif
}

static void getIteratorIdState(stIterator* iterator, u32* frame, u32* object, stCommandKey* value)
{
  if (iterator->frame.object != NULL)
  {
    *frame = iterator->frame.id;
  }
  else
  {
    *frame = 0;
  }

  if (iterator->object.object != NULL)
  {
    *object = iterator->object.id;
  }
  else
  {
    *object = 0;
  }

  if (iterator->value.object != NULL)
  {
    *value = iterator->value.object->key;
  }
  else
  {
    *value = 0;
  }

}

static stBool nextIterator(stIterator* iterator)
{
  stBool didNext;
  u32 lastFrame, lastObject, nowFrame, nowObject;
  stCommandKey lastValue, nowValue;

  getIteratorIdState(iterator, &lastFrame, &lastObject, &lastValue);
  didNext = nextIteratorImpl(iterator);
  getIteratorIdState(iterator, &nowFrame, &nowObject, &nowValue);

  iterator->bChangedFrame  = (lastFrame != nowFrame);
  iterator->bChangedObject = (lastObject != nowObject);
  iterator->bChangedValue  = (lastValue != nowValue);
  
  return didNext;
}

u32 stCreateForwardIterator()
{
  return stCreateForwardIteratorImpl(0);
}

u32 stCreateForwardIteratorImpl(u32 frameSeq)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = NULL; 

  struct stIteratorParams params;
  setIteratorParamsDefault(&params);
  params.valueMin = params.frameSeq = frameSeq;

  //ST_PRINT_CHATTERF("[Iterator][CreateFrame] Min=%i", frameSeq);

  iterator = createIterator(ctx, &params);

  stReleaseContextLock(ctx);
  return iterator != NULL ? iterator->iteratorId : 0;
}

u32 stDisposeIterator(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = NULL; 

  iterator = findIterator(&ctx->iterators, iteratorHandle);
  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stRaiseError(ST_ERROR_ITERATOR);
    return 1;
  }

  deleteIterator(&ctx->iterators, iterator);

  stReleaseContextLock(ctx);
  return 0;
}

void stInitialiseIterators(stIterators* frameIterators)
{
  frameIterators->mem.size = sizeof(stIterator) * 4;
  stDynamicLinearAllocator_Initialise(&frameIterators->mem);

  frameIterators->first = NULL;
  frameIterators->last = NULL;

  u32 baseId = stGetContext_NoMutex()->id ^ 0x2213fa;
  u32 offsetId = baseId ^ 0x123812f;

  stQpr_Initialise(&frameIterators->nextId, baseId, offsetId);
}

void stShutdownIterators(stIterators* frameIterators)
{
  stDynamicLinearAllocator_Shutdown(&frameIterators->mem);

  frameIterators->first = NULL;
  frameIterators->last = NULL;
}

s32 stIteratorNext(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    return -1;
  }

  if (iterator->bEnded)
    return 0;

  ST_DEBUG_ITERATOR("Next.Before");
  stBool result = nextIterator(iterator);
  ST_DEBUG_ITERATOR("Next.After");

  stReleaseContextLock(ctx);

  return (u32) result;
}

s32 stIteratorEnded(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    return -1;
  }

  stBool result = iterator->bEnded;

  ST_DEBUG_ITERATOR("Ended");
  stReleaseContextLock(ctx);

  return (u32) result;
}

s32 stIteratorGetType(u32 iteratorHandle)
{ 
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    return -1;
  }

  stHeaderFrame* headerFrame;
  ST_LAZY_GET_OBJECT(stLazyHeaderFrame, iterator->frame, headerFrame);

  if (headerFrame == NULL)
  {
    stReleaseContextLock(ctx);
    return -1;
  }

  s32 r = 0;

  if (iterator->bEnded)
  {
    r = ST_ITERATOR_TYPE_END;
  }
  else if (iterator->bIterateObjects)
  {
    if (iterator->bIterateValues)
    {
      stDeltaKeyValue* kv = iterator->value.object;

      if (kv != NULL)
      {
        stCommandType cmdType = stGetCommandType(kv->key);

        if (cmdType == ST_CMD_OBJ_NEW)
          r = ST_ITERATOR_TYPE_NEW_OBJECT;
        else if (cmdType == ST_CMD_OBJ_DELETE)
          r = ST_ITERATOR_TYPE_DELETE_OBJECT;
        else
          r = ST_ITERATOR_TYPE_VALUE;
      }
      else
      {
        r = -1;
      }
    }
    else
    {
      r = ST_ITERATOR_TYPE_OBJECT;
    }
  }
  else
  {
    r = ST_ITERATOR_TYPE_FRAME;
  }

  stReleaseContextLock(ctx);

  return r;
}

s32 stIteratorGetChangeFlags(u32 iteratorHandle)
{

  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    return -1;
  }

  stHeaderFrame* headerFrame;
  ST_LAZY_GET_OBJECT(stLazyHeaderFrame, iterator->frame, headerFrame);

  if (headerFrame == NULL)
  {
    stReleaseContextLock(ctx);
    return -1;
  }

  s32 r = 0;

  if (iterator->bChangedFrame)
  {
    r |= ST_ITERATOR_CHANGE_FLAGS_FRAME;
  }

  if (iterator->bChangedObject)
  {
    r |= ST_ITERATOR_CHANGE_FLAGS_OBJECT;
  }

  if (iterator->bChangedValue)
  {
    r |= ST_ITERATOR_CHANGE_FLAGS_VALUE;
  }

  stReleaseContextLock(ctx);

  return r;
}

s32 stIteratorGetFrame(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    return -1;
  }

  stHeaderFrame* headerFrame;
  ST_LAZY_GET_OBJECT(stLazyHeaderFrame, iterator->frame, headerFrame);

  if (headerFrame == NULL)
  {
    stReleaseContextLock(ctx);
    return -1;
  }

  ST_DEBUG_ITERATOR("GetFrame");
  stReleaseContextLock(ctx);

  return (u32) headerFrame->sequenceNum;
}

ST_PUBLIC f64 stIteratorGetFrameTime(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    return 0.0;
  }

  stHeaderFrame* headerFrame;
  ST_LAZY_GET_OBJECT(stLazyHeaderFrame, iterator->frame, headerFrame);

  if (headerFrame == NULL)
  {
    stReleaseContextLock(ctx);
    return 0.0;
  }

  ST_DEBUG_ITERATOR("GetFrame");
  stReleaseContextLock(ctx);

  return headerFrame->frameTime;
}

ST_PUBLIC f64 stIteratorGetFrameLength(u32 iteratorHandle)

{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    return 0.0;
  }

  stHeaderFrame* headerFrame;
  ST_LAZY_GET_OBJECT(stLazyHeaderFrame, iterator->frame, headerFrame);

  if (headerFrame == NULL)
  {
    stReleaseContextLock(ctx);
    return 0.0;
  }

  ST_DEBUG_ITERATOR("GetFrame");
  stReleaseContextLock(ctx);

  return headerFrame->frameLength;
}


ST_PUBLIC u32 stIteratorGetObjectHandle(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  stObjectHeader* objectHeader = iterator->object.object;
  ST_DEBUG_ITERATOR("GetObjectHandle");
  stReleaseContextLock(ctx);

  return (objectHeader == NULL) ? 0 : objectHeader->objectId;
}

ST_PUBLIC u32 stIteratorGetObjectType(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  stObjectHeader* objectHeader = iterator->object.object;

  if (objectHeader == NULL)
  {
    stReleaseContextLock(ctx);
    return 0;
  }

  u32 classId = 0;
  
  if (stObjectIdIsEvent(objectHeader->objectId))
  {
    if (iterator->eventClass != NULL)
    {
      classId = iterator->eventClass->id;
    }
  }
  else
  {
    classId = stGetObjectType_NoMutex(&ctx->classes, objectHeader->objectId);
  }

  ST_DEBUG_ITERATOR("GetObjectType");
  stReleaseContextLock(ctx);

  return classId;
}

u32 stIteratorGetIsEvent(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  stObjectHeader* objectHeader = iterator->object.object;

  if (objectHeader == NULL)
  {
    stReleaseContextLock(ctx);
    return 0;
  }

  u32 isEvent = stObjectIdIsEvent(objectHeader->objectId);


  ST_DEBUG_ITERATOR("GetObjectHandle");
  stReleaseContextLock(ctx);

  return isEvent;
}

ST_PUBLIC u32 stIteratorGetComponentHandle(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  stDeltaKeyValue* keyValue = iterator->value.object;

  ST_DEBUG_ITERATOR("GetComponentId");
  stReleaseContextLock(ctx);

  return (keyValue == NULL) ? 0 : stGetCommandComponent(keyValue->key);
}

ST_PUBLIC u32 stIteratorGetValueType(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  stDeltaKeyValue* keyValue = iterator->value.object;
  u32 type = ST_TYPE_NONE, nbElements = 0;

  if (keyValue != NULL)
  {
    stGetCommandDataTypeAndNbElements(keyValue->key, &type, &nbElements);
  }

  ST_DEBUG_ITERATOR("GetType");
  stReleaseContextLock(ctx);

  return type;
}

ST_PUBLIC u32 stIteratorGetValueArraySize(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  stDeltaKeyValue* keyValue = iterator->value.object;
  u32 size = 0;

  if (keyValue != NULL)
  {
    u32 arrayType = stGetCommandArrayType(keyValue->key);
    
    if (arrayType == ST_ARRAY_TYPE_NONE)
    {
      size = 1;
    }
    else if (arrayType == ST_ARRAY_TYPE_INLINE)
    {
      u8* data = (u8*) (keyValue->data);

      u32 srcMemSize = (u32) *(data++);
      u32 nbElements = (u32) *(data);

      u32 type = ST_TYPE_NONE, nbElementsIgnored = 0;
      stGetCommandDataTypeAndNbElements(keyValue->key, &type, &nbElementsIgnored);
      u32 srcStride = stGetTypeMemorySize(type, nbElements, 1);

      size = srcMemSize / srcStride;
    }
    else if (arrayType == ST_ARRAY_TYPE_OBJECT_DATA)
    {
      u32* data = (u32*) (keyValue->data);
      size = data[1];
    }
  }

  ST_DEBUG_ITERATOR("GetComponentArraySize");
  stReleaseContextLock(ctx);

  return size;
}

ST_PUBLIC u32 stIteratorGetValueNbElements(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  stDeltaKeyValue* keyValue = iterator->value.object;
  u32 type = ST_TYPE_NONE, nbElements = 0;

  if (keyValue != NULL)
  {
    stGetCommandDataTypeAndNbElements(keyValue->key, &type, &nbElements);
  }

  ST_DEBUG_ITERATOR("GetComponentNbElements");
  stReleaseContextLock(ctx);

  return nbElements;
}

ST_PUBLIC s32 stIteratorGetValue_stringLength(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  stDeltaKeyValue* keyValue = iterator->value.object;

  if (keyValue == NULL)
  {
    stReleaseContextLock(ctx);
    return 0;
  }

  u32 value = 0;
  u32 type = ST_TYPE_NONE, nbElements = 0;
  stGetCommandDataTypeAndNbElements(keyValue->key, &type, &nbElements);

  if (type == ST_TYPE_CSTRING)
  {
    if (stGetCommandType(keyValue->key) == ST_CMD_OBJ_VAL_InlineString)
    {
      u8* data = (u8*) keyValue->data;
      value = (s32) data[0];
    }
    else
    {
      u32* data = (u32*) keyValue->data;
      value = (s32) data[1];
    }
  }
  else
  {
    stRaiseError(ST_ERROR_ITERATOR);
  }

  ST_DEBUG_ITERATOR("stIteratorGetValue_stringCapacity");
  stReleaseContextLock(ctx);

  return value;
}

s32 stIteratorGetValue_string(u32 iteratorHandle, stStr out_String, u32 strCapacity)
{

  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  if (out_String == NULL)
  {
    ST_PRINT_WARNF("Null pointer given %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  if (strCapacity <= 1)
  {
    ST_PRINT_WARNF("Capacity too small, should be at least 1 byte %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  stDeltaKeyValue* keyValue = iterator->value.object;

  if (keyValue == NULL)
  {
    stReleaseContextLock(ctx);
    return 0;
  }

  u32 type = ST_TYPE_NONE, nbElements = 0;
  stGetCommandDataTypeAndNbElements(keyValue->key, &type, &nbElements);

  if (type == ST_TYPE_CSTRING)
  {
    u32   strLength = 0;
    char* strSrc  = 0;

    if (stGetCommandType(keyValue->key) == ST_CMD_OBJ_VAL_InlineString)
    {
      u8* data = (u8*) keyValue->data;
      strLength = (s32) data[0];
      strSrc = (char*) (data + sizeof(u8));
    }
    else
    {
      u32* data = (u32*) keyValue->data;
      u32 srcObjectDataId = data[0]; 
      strLength = (s32) data[1];

      // Frame is already loaded, because of the above.
      stHeaderFrame* headerFrame = iterator->frame.object;
      stFrameData* frameData = headerFrame->data;

      struct stObjectDataT* objectData = stObjectData_Get(&frameData->objectDatas, srcObjectDataId);

      if (objectData == NULL)
      {
        ST_PRINT_ERRORF("Internal Error; Cannot find Object data matching id; %i", srcObjectDataId);
        return 0;
      }

      if (stObjectData_LoadImpl(objectData) == NULL)
      {
        ST_PRINT_ERRORF("Internal Error; Object data could not be loaded; %i", srcObjectDataId);
        return 0;
      }

      strSrc = (char*) objectData->data;
    }

    if (strLength >= strCapacity)
    {
      strLength = strCapacity - 1;
    }

    memcpy(out_String, strSrc, strLength);
    out_String[strLength] = '\0';
 
    ST_DEBUG_ITERATOR("stIteratorGetValue_stringCapacity");
    stReleaseContextLock(ctx);


    // TODO: Release Object Data, etc.

    return strLength;
  }
  else
  {
    stRaiseError(ST_ERROR_ITERATOR);
    ST_DEBUG_ITERATOR("stIteratorGetValue_stringCapacity");
    stReleaseContextLock(ctx);
    return 0;
  }

}

#define stIteratorGetValue_entry(T, TYPE)\
      stContext* ctx = stObtainContextLock();\
      stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);\
      if (iterator == NULL)\
      {\
        ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);\
        stReleaseContextLock(ctx);\
        stRaiseError(ST_ERROR_ITERATOR);\
        return 0;\
      }\
      stDeltaKeyValue* keyValue = iterator->value.object;\
      if (keyValue == NULL)\
      {\
        stReleaseContextLock(ctx);\
        return 0;\
      }\
      T value = 0;\
      u32 type = ST_TYPE_NONE, nbElements = 0;\
      stGetCommandDataTypeAndNbElements(keyValue->key, &type, &nbElements);\
      if (type == TYPE && nbElements >= 1 && element < nbElements)\
      {\
        T* data = (T*) keyValue->data;\
        value = data[element];\
      }\
      else\
      {\
        stRaiseError(ST_ERROR_ITERATOR);\
      }\
      stReleaseContextLock(ctx);\
      return value;

ST_PUBLIC u8 stIteratorGetValue_uint8(u32 iteratorHandle, u32 element)
{
  stIteratorGetValue_entry(u8, ST_TYPE_UINT8);
}

ST_PUBLIC u16 stIteratorGetValue_uint16(u32 iteratorHandle, u32 element)
{
  stIteratorGetValue_entry(u16, ST_TYPE_UINT16);
}

ST_PUBLIC u32 stIteratorGetValue_uint32(u32 iteratorHandle, u32 element)
{
  stIteratorGetValue_entry(u32, ST_TYPE_UINT32);
}

ST_PUBLIC u64 stIteratorGetValue_uint64(u32 iteratorHandle, u32 element)
{
  stIteratorGetValue_entry(u64, ST_TYPE_UINT64);
}

ST_PUBLIC s8 stIteratorGetValue_int8(u32 iteratorHandle, u32 element)
{
  stIteratorGetValue_entry(s8, ST_TYPE_INT8);
}

ST_PUBLIC s16 stIteratorGetValue_int16(u32 iteratorHandle, u32 element)
{
  stIteratorGetValue_entry(s16, ST_TYPE_INT16);
}

ST_PUBLIC s32 stIteratorGetValue_int32(u32 iteratorHandle, u32 element)
{
  stIteratorGetValue_entry(s32, ST_TYPE_INT32);
}

ST_PUBLIC s64 stIteratorGetValue_int64(u32 iteratorHandle, u32 element)
{
  stIteratorGetValue_entry(s64, ST_TYPE_INT64);
}

ST_PUBLIC f32 stIteratorGetValue_float32(u32 iteratorHandle, u32 element)
{
  stIteratorGetValue_entry(f32, ST_TYPE_FLOAT32);
}

ST_PUBLIC f64 stIteratorGetValue_float64(u32 iteratorHandle, u32 element)
{
  stIteratorGetValue_entry(f64, ST_TYPE_FLOAT64);
}

static s32 iteratorGetValueArray_InlineArray(stIterator* iterator, stDeltaKeyValue* keyValue, void* dstVoidPtr, u32 dstMemSize, u32 dstStride)
{
  ST_UNUSED(iterator);

  u8* data = (u8*) (keyValue->data);

  u32 srcMemSize = (u32) *(data++);
  u32 nbElements = (u32) *(data++);
  
  if (srcMemSize > dstMemSize)
  {
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  u32 type = ST_TYPE_NONE, nbElementsIgnored = 0;
  stGetCommandDataTypeAndNbElements(keyValue->key, &type, &nbElementsIgnored);
  u32 srcStride = stGetTypeMemorySize(type, nbElements, 1);

  if (dstStride < srcStride)
    return 0;

  u8* src = data;
  u8* dst = (u8*) dstVoidPtr;

  u32 arraySize = srcMemSize / srcStride;

  stStrideMemCpy(dst, src, dstStride, srcStride, arraySize);

  return srcMemSize;
}

ST_PUBLIC u32 iteratorGetValueArray_ObjectData(stIterator* iterator, stDeltaKeyValue* keyValue, void* dstVoidPtr, u32 dstMemSize, u32 dstStride)
{

  u32  srcObjectDataId = (*(u32*) (keyValue->data));

  // Frame is already loaded, because of the above.
  stHeaderFrame* headerFrame = iterator->frame.object;
  stFrameData* frameData = headerFrame->data;

  struct stObjectDataT* objectData = stObjectData_Get(&frameData->objectDatas, srcObjectDataId);

  if (objectData == NULL)
  {
    ST_PRINT_ERRORF("Internal Error; Cannot find Object data matching id; %i", srcObjectDataId);
    return 0;
  }

  if (stObjectData_LoadImpl(objectData) == NULL)
  {
    ST_PRINT_ERRORF("Internal Error; Object data could not be loaded; %i", srcObjectDataId);
    return 0;
  }

  u32 type = ST_TYPE_NONE;
  u32 nbElements = 0;
  //u32 arraySize = (*((u32*) (keyValue->data) + 1));
  stGetCommandDataTypeAndNbElements(keyValue->key, &type, &nbElements);
  nbElements = *((u8*) (keyValue->data + sizeof(u32) + sizeof(u32))); // nbElements is stored in keyValue data as the 5th byte, this can be quicker
                                                                      // than a component lookup as the cost of a byte.

  u32 srcStride = stGetTypeMemorySize(type, nbElements, 1);

  if (dstStride < srcStride)
  {
    ST_PRINT_ERRORF("User stride is less than the data stride. Need=%i, Got=%i", srcStride, dstStride);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  if (objectData->memSize > dstMemSize)
  {
    ST_PRINT_ERRORF("User memory size is less than the data memory size. Need=%i, Got=%i", objectData->memSize, dstMemSize);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }

  u8* src = objectData->data;
  u8* dst = (u8*) dstVoidPtr;

  stStrideMemCpy(dst, src, dstStride, srcStride, objectData->arraySize);

  return objectData->memSize;
}

ST_PUBLIC u32 stIteratorGetValueArray(u32 iteratorHandle, void* dstVoidPtr, u32 dstMemSize, u32 dstStride)
{
  
  if (dstVoidPtr == NULL)
  {
    ST_PRINT_ERRORF("Given user pointer is NULL %p", dstVoidPtr);
    return 0;
  }

  if (dstMemSize == 0)
  {
    ST_PRINT_ERRORF("Given user mem is 0 in size; %i", dstMemSize);
    return 0;
  }

  if (dstStride == 0)
  {
    ST_PRINT_ERRORF("Given user stride is 0; %i", dstStride);
    return 0;
  }

  if (dstMemSize < dstStride)
  {
    ST_PRINT_ERRORF("Given user stride %i is smaller than memory Size %i", dstStride, dstMemSize);
    return 0;
  }


  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);
  if (iterator == NULL)
  {
    ST_PRINT_WARNF("Unknown Iterator = %i", iteratorHandle);
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 0;
  }
  stDeltaKeyValue* keyValue = iterator->value.object;
  
  if (keyValue == NULL)
  {
    ST_PRINT_WARNF("No Object on iterator %i", iteratorHandle);
    stReleaseContextLock(ctx);
    return 0;
  }

  switch(stGetCommandArrayType(keyValue->key))
  {
    default:
    case ST_ARRAY_TYPE_NONE:
    {
      ST_PRINT_WARNF("Not an array %i", iteratorHandle);
      break;
    }
    case ST_ARRAY_TYPE_INLINE:
    {
      u32 response = iteratorGetValueArray_InlineArray(iterator, keyValue, dstVoidPtr, dstMemSize, dstStride);
      stReleaseContextLock(ctx);
      return response;
    }
    case ST_ARRAY_TYPE_OBJECT_DATA:
    {
      u32 response = iteratorGetValueArray_ObjectData(iterator, keyValue, dstVoidPtr, dstMemSize, dstStride);
      stReleaseContextLock(ctx);
      return response;
    }
  }

  stReleaseContextLock(ctx);
  return 0;
}















void stIteratorPrint_Impl(stIterator* iterator, const char* comment)
{
  stProbe_BeginWithName(comment);
  {
    if (iterator == NULL || (iterator != NULL && iterator->bDeleted))
    {
      goto end;
    }

    stProbe_BeginWithName("flags");
    {
      stProbe_KeyBool("bIterateFrames", iterator->bIterateFrames);
      stProbe_KeyBool("bIterateObjects", iterator->bIterateObjects);
      stProbe_KeyBool("bIterateValues", iterator->bIterateValues);
      stProbe_KeyBool("bEnded", iterator->bEnded);
    }
    stProbe_End();

    stProbe_BeginWithName("frameRules");
    {
      stProbe_KeyInt("min", iterator->frameRules.min);
      stProbe_KeyInt("max", iterator->frameRules.max);
    }
    stProbe_End();
    stProbe_BeginWithName("lazyFrame");
    {
      stProbe_KeyBool("bAdjustmentAllowed", iterator->frame.bAdjustmentAllowed);
      stProbe_KeyBool("bLazyLoad", iterator->frame.bLazyLoad);
      stProbe_KeyInt("id", iterator->frame.id);
      stProbe_KeyPointer("object", iterator->frame.object);
      stProbe_KeyPointer("p", iterator->frame.p);
    }
    stProbe_End();
    
    if (iterator->frame.object)
    {
      stHeaderFramePrint_Impl(iterator->frame.object, "frame");
    }
    else
    {
      stProbe_BeginWithName("frame");
      stProbe_End();
    }

    stProbe_BeginWithName("objectRules");
    {
      stProbe_KeyInt("min", iterator->objectRules.min);
      stProbe_KeyInt("max", iterator->objectRules.max);
    }
    stProbe_End();

    stProbe_BeginWithName("lazyObject");
    {
      stProbe_KeyBool("bAdjustmentAllowed", iterator->object.bAdjustmentAllowed);
      stProbe_KeyBool("bLazyLoad", iterator->object.bLazyLoad);
      stProbe_KeyInt("id", iterator->object.id);
      stProbe_KeyPointer("object", iterator->object.object);
      if (iterator->object.p != NULL)
        stProbe_KeyIntAsHex("p", iterator->object.p->frameId);
      else
        stProbe_KeyIntAsHex("p", 0);
    }
    stProbe_End();

    if (iterator->object.object)
    {
      stObjectHeaderPrint_Impl(iterator->object.object, "object");
    }
    else
    {
      stProbe_BeginWithName("object");
      stProbe_End();
    }

    stProbe_BeginWithName("valueRules");
    {
      switch(iterator->valueType)
      {
        case ST_VALUE_ITERATOR_TYPE_FORWARD_ITERATOR:
        {
          stProbe_KeyInt("min", iterator->valueRules.min);
          stProbe_KeyInt("max", iterator->valueRules.max);
          stProbe_KeyString("type", "forwardIterator");
        };
        break;
        case ST_VALUE_ITERATOR_TYPE_COMPONENT_TYPE:
        {
          stProbe_KeyInt("component", iterator->valueRules.min);
          stProbe_KeyString("type", "component");
        };
        break;
      }
    }
    stProbe_End();

    stProbe_BeginWithName("lazyValue");
    {
      stProbe_KeyBool("bAdjustmentAllowed", iterator->value.bAdjustmentAllowed);
      stProbe_KeyBool("bLazyLoad", iterator->value.bLazyLoad);
      stProbe_KeyInt("id", iterator->value.id);
      stProbe_KeyPointer("object", iterator->value.object);
      stProbe_BeginWithName("p");
      {
        if (iterator->value.p.objectHeader != NULL)
        {
          stProbe_KeyIntAsHex("p", iterator->value.p.objectHeader->objectId);
        }
        else
        {
          stProbe_KeyIntAsHex("p", 0);
        }
        switch(iterator->value.p.valueType)
        {
          case ST_VALUE_ITERATOR_TYPE_FORWARD_ITERATOR:
          {
            stProbe_KeyString("type", "forwardIterator");
          };
          break;
          case ST_VALUE_ITERATOR_TYPE_COMPONENT_TYPE:
          {
            stProbe_KeyString("type", "component");
          };
          break;
        }
      }
      stProbe_End();
    }
    stProbe_End();

    if (iterator->frame.object)
    {
      stDeltaKeyValuePrint_Impl(iterator->value.object, "value");
    }
    else
    {
      stProbe_BeginWithName("value");
      stProbe_End();
    }

  }
  end:
  stProbe_End();
}

void stIteratorDebug(stIterator* iterator, const char* comment)
{
  ST_UNUSED(comment); // Bug in VS2015. Doesn't recongise usage in the macro with strict compiling
                      // warning C4100: 'comment': unreferenced formal parameter. 
  ST_UNUSED(iterator); 
#if 0
  
  if (iterator == NULL)
  {
    ST_PRINT_CHATTERF("[Iterator][%s] => NULL", comment);
  }
  else
  {
    ST_PRINT_CHATTERF("[Iterator][%s] => %c%c%c Fr:%u:%u, Ob:%u:%u, Va:%u:%" PRIx64,
        comment,
        iterator->bIterateFrames  ? 'f' : '-',
        iterator->bIterateObjects ? 'o' : '-',
        iterator->bIterateValues  ? 'v' : '-',
        iterator->frame.id,
        iterator->frame.object != NULL ? iterator->frame.object->sequenceNum : 0,
        iterator->object.id,
        iterator->object.object != NULL ? iterator->object.object->objectId  : 0,
        iterator->value.id,
        iterator->value.object != NULL ? iterator->value.object->key : 0
      );
  }
#endif
}

#if ST_CFG_DATA_PROBE == 1
ST_PUBLIC_PROBE int stIteratorPrint(u32 iteratorHandle)
{
  stContext* ctx = stObtainContextLock();
  stIterator* iterator = findIterator(&ctx->iterators, iteratorHandle);

  if (iterator == NULL)
  {
    stReleaseContextLock(ctx);
    stRaiseError(ST_ERROR_ITERATOR);
    return 1;
  }

  stIteratorPrint_Impl(iterator, "Probe");

  stReleaseContextLock(ctx);
  return 0;
}
#endif
