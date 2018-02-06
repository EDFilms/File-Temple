// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	1.0.0
 */

#include "stObject.h"
#include "stRecording.h"
#include "stList.h"
#include "stTypes.h"
#include "stUnits.h"
#include "stReference.h"
#include "stCommand.h"
#include "stError.h"
#include "stStride.h"

#define ST_MAX_DATA_PER_COMMAND (248)
#define ST_FORCE_ARRAYS_TO_BE_OBJECT_DATA 0

#define ST_ABORT_ON_NULL_HANDLE if (objectHandle == 0) return;

static stClass* findClass_Assert(stClasses* classes, u32 id)
{
  ST_LIST_FOR_BEGIN(stClass, classes, class);
  {
    if (class->id == id)
    {
      return class;
    }
  }
  ST_LIST_FOR_END(stClass, classes, class);

  ST_PRINT_ERRORF("Cannot find class %i", id);
  ST_ALWAYS_ASSERT("Unknown Class");
  return NULL;
}

stClass* stGetClass_NoMutex(u32 id)
{
  return findClass_Assert(&stGetContext_NoMutex()->classes, id);
}

static stClass* findClass_NoAssert(stClasses* classes, u32 id)
{
  ST_LIST_FOR_BEGIN(stClass, classes, class);
  {
    if (class->id == id)
    {
      return class;
    }
  }
  ST_LIST_FOR_END(stClass, classes, class);

  return NULL;
}

stClass* stGetClass_NoMutex_NoAssert(u32 id)
{
  return findClass_NoAssert(&stGetContext_NoMutex()->classes, id);
}


static stClass* findClassUserData(stClasses* classes, u32 userData)
{
  ST_LIST_FOR_BEGIN(stClass, classes, class);
  {
    if (class->userData == userData)
    {
      return class;
    }
  }
  ST_LIST_FOR_END(stClass, classes, class);

  ST_ALWAYS_ASSERT("Unknown Class");
  return NULL;
}


static stClass* findClassUserData_NoAssert(stClasses* classes, u32 userData)
{
  ST_LIST_FOR_BEGIN(stClass, classes, class);
  {
    if (class->userData == userData)
    {
      return class;
    }
  }
  ST_LIST_FOR_END(stClass, classes, class);

  return NULL;
}

static stClass* findClassFromObjectHandle(stClasses* classes, u32 objectHandle)
{
  ST_LIST_FOR_BEGIN(stClass, classes, klass);
  {
    if (stDynamicBitSet_GetBit(&klass->objects, objectHandle))
      return klass;
  }
  ST_LIST_FOR_END(stClass, classes, klass);
  return NULL;
}

static stComponent* findComponentFromKind(stClass* class, u32 kind)
{
  for(u32 i=0;i < class->nbComponents;i++)
  {
    stComponent* component = &class->components[i];
    if (component->kind == kind)
      return component;
  }
  return NULL;
}

static stComponent* getComponentFromHandle(stClass* class, u32 component)
{
  u32 idx = component - 1;
  if (idx >= class->nbComponents)
    return NULL;

  return &class->components[idx];
}

static stComponent* getComponentFromHandle2(stClasses* classes, u32 objectHandle, u32 component)
{
  stClass* class = findClassFromObjectHandle(classes, objectHandle);

  if (class == NULL)
    return NULL;

  u32 idx = component - 1;
  if (idx >= class->nbComponents)
    return NULL;

  return &class->components[idx];
}
void stInitialiseClasses(stClasses* classes)
{
  classes->first        = NULL;
  classes->last         = NULL;
  classes->nextClassId  = 1;
  classes->nextObjectId = 1;
  classes->nextEventId  = 0x7FFFFF;
  classes->mem.nbBlocks = ST_MAX_CLASSES_PER_POOL;
  classes->mem.size = sizeof(stClass);
  stDynamicPool_Initialize(&classes->mem);
}

void stShutdownClasses(stClasses* classes)
{ 
  ST_LIST_FOR_BEGIN(stClass, classes, class);
  {
    stDynamicBitSet_Shutdown(&class->objects);
  }
  ST_LIST_FOR_END(stClass, classes, class);

  classes->first = NULL;
  classes->last  = NULL;
  stDynamicPool_Shutdown(&classes->mem);
}

void stResetEventIds(stClasses* classes)
{
  classes->nextEventId  = 0x7FFFFF;
}

s32 stKindToComponent(stClasses* classes, u32 classId, u32 kind)
{
  stClass* class = findClass_Assert(classes, classId);
  if (class == NULL)
  {
    return -1;
  }

  for(u32 i=0;i < class->nbComponents;i++)
  {
    stComponent* component = &class->components[i];
    if (component->kind == kind)
      return i;
  }

  return -1;
}

s32 stComponentToKind(stClasses* classes, u32 classId, u32 id)
{
  stClass* klass = findClass_Assert(classes, classId);
  if (klass == NULL)
  {
    return -1;
  }

  for(u32 i=0;i < klass->nbComponents;i++)
  {
    stComponent* component = &klass->components[i];
    if (component->id == id)
      return i;
  }

  return -1;
}

ST_PUBLIC_PROBE int stInternal_GetNbClasses()
{
  s32 count;
  stContext* context = stObtainContextLock();

  if (context == NULL)
    return -1;

  ST_LIST_COUNT(count, stClass, (&context->classes));

  stReleaseContextLock(context);

  return count;
}

ST_PUBLIC_PROBE int stInternal_GetClassFrequency(unsigned int classType)
{
  s32 value;
  stContext* context = stObtainContextLock();

  if (context == NULL)
    return -1;
  
  stClass* class = findClass_Assert(&context->classes, classType);
  value = class->frequency;

  stReleaseContextLock(context);

  return value;
}

ST_PUBLIC_PROBE int stInternal_GetClassNbComponents(unsigned int classType)
{
  s32 value;
  stContext* context = stObtainContextLock();

  if (context == NULL)
    return -1;

  stClass* class = findClass_Assert(&context->classes, classType);
  value = class->nbComponents;

  stReleaseContextLock(context);

  return value;
}

ST_PUBLIC_PROBE int stInternal_GetClassComponentType(unsigned int classType, unsigned int componentType)
{
  s32 value;
  stContext* context = stObtainContextLock();

  if (context == NULL)
    return -1;

  stClass* class = findClass_Assert(&context->classes, classType);
  value = class->components[componentType].type;

  stReleaseContextLock(context);

  return value;
}

ST_PUBLIC_PROBE int stInternal_GetClassComponentKind(unsigned int classType, unsigned int componentType)
{
  s32 value;
  stContext* context = stObtainContextLock();

  if (context == NULL)
    return -1;

  stClass* class = findClass_Assert(&context->classes, classType);
  value = class->components[componentType].kind;

  stReleaseContextLock(context);

  return value;
}

ST_PUBLIC_PROBE int stInternal_GetClassComponentNbElements(unsigned int classType, unsigned int componentType)
{
  s32 value;
  stContext* context = stObtainContextLock();

  if (context == NULL)
    return -1;

  stClass* class = findClass_Assert(&context->classes, classType);
  value = class->components[componentType].nbElements;

  stReleaseContextLock(context);

  return value;
}

ST_PUBLIC_PROBE int stInternal_GetClassComponentUnits(unsigned int classType, unsigned int componentType)
{
  s32 value;
  stContext* context = stObtainContextLock();

  if (context == NULL)
    return -1;

  stClass* class = findClass_Assert(&context->classes, classType);
  value = class->components[componentType].units;

  stReleaseContextLock(context);

  return value;
}

ST_PUBLIC_PROBE int stInternal_GetClassComponentReference(unsigned int classType, unsigned int componentType)
{
  s32 value;
  stContext* context = stObtainContextLock();

  if (context == NULL)
    return -1;

  stClass* class = findClass_Assert(&context->classes, classType);
  value = class->components[componentType].reference;

  stReleaseContextLock(context);

  return value;
}

ST_PUBLIC u32 stCreateObjectType(stEnum32 objectFrequency)
{
  return stCreateObjectTypeEx(objectFrequency, 0);
}

ST_PUBLIC u32 stCreateObjectTypeEx(stEnum32 objectFrequency, u32 userData)
{
  stContext* context;
  stClass* klass;
  u32 id;
  
  context               = stObtainContextLock();

  if (context == NULL)
    return 0;

    klass               = stDynamicPool_MaybeAllocateMemory(&context->classes.mem);

    if (klass == NULL)
    {
      stReleaseContextLock(context);
      stRaiseError(ST_ERROR_TYPE);
      return 0;
    }

    klass->next         = NULL;
    klass->prev         = NULL;
    klass->userData     = userData;
    klass->components   = (stComponent*) klass->componentMem;
    klass->frequency    = (u8) objectFrequency;
    klass->nbUses       = 0;
    id = klass->id      = context->classes.nextClassId++;
    klass->nbComponents = 0;
    klass->flags        = 0;
    klass->objects.reference  = 'KLOB';
    stDynamicBitSet_InitialiseZero(&klass->objects);

    ST_LIST_ADD((&context->classes), klass);
  stReleaseContextLock(context);

  //ST_PRINT_CHATTERF("Added Type Id=%i/%i", klass->id, klass->userData);

  return id;
}

stClass* stCreateObjectTypeImpl(u32 id, stClasses* classes)
{
  stClass* klass;

  klass               = stDynamicPool_MaybeAllocateMemory(&classes->mem);

  if (klass == NULL)
  {
    stRaiseError(ST_ERROR_TYPE);
    return NULL;
  }

  klass->next         = NULL;
  klass->prev         = NULL;
  klass->userData     = 0;
  klass->components   = (stComponent*) klass->componentMem;
  klass->frequency    = (u8) ST_OBJECT_FREQUENCY_DYNAMIC;
  klass->nbUses       = 0;
  klass->id      = id;

  if (classes->nextClassId < id)
  {
    classes->nextClassId = id + 1;
  }

  klass->nbComponents = 0;
  klass->flags        = 0;
  klass->objects.reference = 'KLO2';

  stDynamicBitSet_InitialiseZero(&klass->objects);

  ST_LIST_ADD(classes, klass);

  //ST_PRINT_CHATTERF("Loaded Type Id=%i/%i", klass->id, klass->userData);

  return klass;
}

ST_PUBLIC u32 stAddObjectTypeComponent(u32 objectType, stEnum32 kind, stEnum32 type, u32 nbElements)
{
  return stAddObjectTypeComponentEx2(objectType, kind, type, nbElements, 1, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
}

ST_PUBLIC u32 stAddObjectTypeComponentEx(u32 objectType, stEnum32 kind, stEnum32 type, u32 nbElements, u32 arrayCapacity)
{
  return stAddObjectTypeComponentEx2(objectType, kind, type, nbElements, arrayCapacity, ST_UNITS_UNSPECIFIED, ST_REFERENCE_UNSPECIFIED);
}

ST_PUBLIC u32 stAddObjectTypeComponentEx2(u32 objectType, stEnum32 kind, stEnum32 type, u32 nbElements, u32 arrayCapacity, stEnum32 units, stEnum32 reference)
{
  stContext*   context;
  stClass*     klass;
  stComponent* component;
  u32          id;

  if (objectType == 0)
  {
    stRaiseError(ST_ERROR_TYPE);
    return 0;
  }

  context               = stObtainContextLock();
  klass                 = findClass_Assert(&context->classes, objectType);
  ST_ASSERT(klass->nbComponents < ST_MAX_COMPONENTS_PER_CLASS, "Components per klass reached");
  id                    = 1 + klass->nbComponents++;
  component             = &klass->components[id - 1];
  component->id         = (u16) id;
  component->kind       = (u16) kind;
  component->type       = (u8) type;
  component->nbElements = (u8) nbElements;
  component->units      = (u8) units;
  component->reference  = (u8) reference;

  component->capacity   = (arrayCapacity == 0) ? 1 : arrayCapacity;
  component->memSize    = stGetTypeMemorySize(component->type, component->nbElements, arrayCapacity);
  component->flags      = 0;
  stReleaseContextLock(context);

  //ST_PRINT_CHATTERF("Added Component Class=%i/%i, Id=%i, Type=%s(%i)", klass->id, klass->userData, id, stGetTypeToString(component->type), component->nbElements);

  return id;
}

stComponent* stAddObjectTypeComponentImpl(u32 id, stClass* class)
{
  stComponent* component;
  ST_ASSERT(class->nbComponents < ST_MAX_COMPONENTS_PER_CLASS, "Components per class reached");

  if (class->nbComponents < id)
  {
    class->nbComponents = (u8) (id + 1);
  }

  component             = &class->components[id - 1];
  component->id         = (u16) id;
  component->kind       = (u8) 0;
  component->type       = (u8) 0;
  component->nbElements = (u8) 0;
  component->units      = (u8) 0;
  component->reference  = (u8) 0;

  component->capacity   = 1;
  component->memSize    = 0; //getTypeMemorySize(component->type, component->nbElements, 1);
  component->flags      = 0;

  return component;
}

ST_PUBLIC u32 stFindObjectType(u32 userData)
{
  stContext* ctx = stObtainContextLock();


  stClass* class = findClassUserData_NoAssert(&ctx->classes, userData);
  u32 handle = 0;

  if (class != NULL)
  {
    handle = class->id;
  }
  stReleaseContextLock(ctx);

  return handle;
}

ST_PUBLIC u32 stFindObjectTypeComponent(u32 objectType, u32 kind)
{
  stContext* ctx = stObtainContextLock();

  stClass* class = findClass_NoAssert(&ctx->classes, objectType);
  u32 handle = 0;

  if (class != NULL)
  {
    stComponent* comp =  findComponentFromKind(class, kind);
    if (comp != NULL)
    {
      handle = comp->id;
    }
  }

  stReleaseContextLock(ctx);
  return handle;
}


u32 stCreateObject(u32 objectType)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX_VALUE(0);

  stContext* ctx;
  stByte* data;
  u32 id;
  
  ctx = stGetContext_NoMutex();
  
  stClass* klass = findClass_Assert(&ctx->classes, objectType);
  
  if (klass == NULL)
  {
    return 0;
  }
  
  if (klass->frequency == ST_OBJECT_FREQUENCY_EVENT)
  {
    id = ctx->classes.nextEventId++;
  }
  else
  {
    id = ctx->classes.nextObjectId++;
    stDynamicBitSet_SetBit(&klass->objects, id);
  }

  data = stPushCommand(0, ST_CMD_OBJ_NEW, id, 0, 0);

  if (data == NULL)
  {
    stRaiseError(ST_ERROR_RECORDING);
    return 0;
  }

  stCommandWriteU32(data, objectType);

  ST_PRINT_CHATTERF("Created Object %i of Type %i", id, objectType);

  return id;
}

void stDestroyObject(u32 objectHandle)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;

  stPushCommand(0, ST_CMD_OBJ_DELETE, objectHandle, 255, 0);

  ST_PRINT_CHATTERF("Deleted Object %i", objectHandle);
}

u32 stGetObjectType(u32 objectHandle)
{
  stContext* ctx = stGetContext_NoMutex();
  u32 type = stGetObjectType_NoMutex(&ctx->classes, objectHandle);

  return type;
}

u32 stGetObjectType_NoMutex(struct stClassesT* classes, u32 objectHandle)
{
  stClass* class = findClassFromObjectHandle(classes, objectHandle);

  if (class == NULL)
    return 0;

  return class->id;
}

#define ST_DATA_SIZE(X) (u8) ((X))

#define ST_SET_VALUE_N(TYPE, USER_TYPE) \
  void stSetValue_##USER_TYPE(u32 objectHandle, u32 component, TYPE x)\
  {\
    ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();\
    ST_ABORT_ON_NULL_HANDLE;\
    u8* data = stPushCommand(0, ST_CMD_OBJ_VAL_1##TYPE, objectHandle, (u8) component, 0);\
    if (data == NULL) { stRaiseError(ST_ERROR_RECORDING); return; } \
    data = stCommandWrite##TYPE(data, x);\
  } \
  void stSetValue_2_##USER_TYPE(u32 objectHandle, u32 component, TYPE x, TYPE y)\
  {\
    ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();\
    ST_ABORT_ON_NULL_HANDLE;\
    u8* data = stPushCommand(0, ST_CMD_OBJ_VAL_2##TYPE, objectHandle, (u8) component, 0);\
    if (data == NULL) { stRaiseError(ST_ERROR_RECORDING); return; } \
    data = stCommandWrite##TYPE(data, x);\
    data = stCommandWrite##TYPE(data, y);\
  } \
  void stSetValue_3_##USER_TYPE(u32 objectHandle, u32 component, TYPE x, TYPE y, TYPE z)\
  {\
    ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();\
    ST_ABORT_ON_NULL_HANDLE;\
    u8* data = stPushCommand(0, ST_CMD_OBJ_VAL_3##TYPE, objectHandle, (u8) component, 0);\
    if (data == NULL) { stRaiseError(ST_ERROR_RECORDING); return; } \
    data = stCommandWrite##TYPE(data, x);\
    data = stCommandWrite##TYPE(data, y);\
    data = stCommandWrite##TYPE(data, z);\
  }\
  void stSetValue_4_##USER_TYPE(u32 objectHandle, u32 component, TYPE x, TYPE y, TYPE z, TYPE w)\
  {\
    ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();\
    ST_ABORT_ON_NULL_HANDLE;\
    u8* data = stPushCommand(0, ST_CMD_OBJ_VAL_4##TYPE,  objectHandle, (u8) component, 0);\
    if (data == NULL) { stRaiseError(ST_ERROR_RECORDING); return; } \
    data = stCommandWrite##TYPE(data, x);\
    data = stCommandWrite##TYPE(data, y);\
    data = stCommandWrite##TYPE(data, z);\
    data = stCommandWrite##TYPE(data, w);\
  }\

ST_SET_VALUE_N(U8,  uint8);
ST_SET_VALUE_N(U16, uint16);
ST_SET_VALUE_N(U32, uint32);
ST_SET_VALUE_N(U64, uint64);
ST_SET_VALUE_N(S8,  int8);
ST_SET_VALUE_N(S16, int16);
ST_SET_VALUE_N(S32, int32);
ST_SET_VALUE_N(S64, int64);
ST_SET_VALUE_N(F32, float32);
ST_SET_VALUE_N(F64, float64);

ST_PUBLIC u32  stCalculatePODVectorMemorySize(stEnum32 type, u32 nbElements, u32 arraySize)
{
  return stGetTypeMemorySize(type, nbElements, arraySize);
}

// Note: Regarding Stride
//      
//       Stride refers to the byte size of 'one vector' in array. To get to the next 'vector' in memory, the stride is added to the pointer to advance forwards.
//       This system is used because some systems can have a 'ghost data' that is unused by the function which should be skipped over.
//       This is only possible with continoug memory with no programming language data, so only works on POD types.
//      
// Examples:
//      
//      u8, one element.                                Stride = sizeof(u8)
//      
//      |FF|FF|FF|....                                  1
//
//      u16, two elements.                              Stride = sizeof(u16) * 2
//      
//      |FAce,FAce|.....                                4
//      
//      f32, three elements                             Stride = sizeof(f32) * 3
//      
//      |1.23,4.56,7.89|                                12
//      
//      f32, four elements. But we ignore the fourth.   Stride = sizeof(f64) * 4
//
//      |1.23,4.56,7.89,?.??|                           16

ST_PUBLIC u32  stCalculateStride1(stEnum32 type1, u32 count1)
{
  u32 stride = 0;
  stride += stGetTypeMemorySize(type1, 1, count1);
  return stride;
}

ST_PUBLIC u32  stCalculateStride2(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2)
{
  u32 stride = 0;
  stride += stGetTypeMemorySize(type1, 1, count1);
  stride += stGetTypeMemorySize(type2, 1, count2);
  return stride;
}

ST_PUBLIC u32  stCalculateStride3(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3)
{
  u32 stride = 0;
  stride += stGetTypeMemorySize(type1, 1, count1);
  stride += stGetTypeMemorySize(type2, 1, count2);
  stride += stGetTypeMemorySize(type3, 1, count3);
  return stride;
}

ST_PUBLIC u32  stCalculateStride4(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3, stEnum32 type4, u32 count4)
{
  u32 stride = 0;
  stride += stGetTypeMemorySize(type1, 1, count1);
  stride += stGetTypeMemorySize(type2, 1, count2);
  stride += stGetTypeMemorySize(type3, 1, count3);
  stride += stGetTypeMemorySize(type4, 1, count4);
  return stride;
}

ST_PUBLIC u32  stCalculateStride5(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3, stEnum32 type4, u32 count4, stEnum32 type5, u32 count5)
{
  u32 stride = 0;
  stride += stGetTypeMemorySize(type1, 1, count1);
  stride += stGetTypeMemorySize(type2, 1, count2);
  stride += stGetTypeMemorySize(type3, 1, count3);
  stride += stGetTypeMemorySize(type4, 1, count4);
  stride += stGetTypeMemorySize(type5, 1, count5);
  return stride;
}

ST_PUBLIC u32  stCalculateStride6(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3, stEnum32 type4, u32 count4, stEnum32 type5, u32 count5, stEnum32 type6, u32 count6)
{
  u32 stride = 0;
  stride += stGetTypeMemorySize(type1, 1, count1);
  stride += stGetTypeMemorySize(type2, 1, count2);
  stride += stGetTypeMemorySize(type3, 1, count3);
  stride += stGetTypeMemorySize(type4, 1, count4);
  stride += stGetTypeMemorySize(type5, 1, count5);
  stride += stGetTypeMemorySize(type6, 1, count6);
  return stride;
}

ST_PUBLIC u32  stCalculateStride7(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3, stEnum32 type4, u32 count4, stEnum32 type5, u32 count5, stEnum32 type6, u32 count6, stEnum32 type7, u32 count7)
{
  u32 stride = 0;
  stride += stGetTypeMemorySize(type1, 1, count1);
  stride += stGetTypeMemorySize(type2, 1, count2);
  stride += stGetTypeMemorySize(type3, 1, count3);
  stride += stGetTypeMemorySize(type4, 1, count4);
  stride += stGetTypeMemorySize(type5, 1, count5);
  stride += stGetTypeMemorySize(type6, 1, count6);
  stride += stGetTypeMemorySize(type7, 1, count7);
  return stride;
}

ST_PUBLIC u32  stCalculateStride8(stEnum32 type1, u32 count1, stEnum32 type2, u32 count2, stEnum32 type3, u32 count3, stEnum32 type4, u32 count4, stEnum32 type5, u32 count5, stEnum32 type6, u32 count6, stEnum32 type7, u32 count7, stEnum32 type8, u32 count8)
{
  u32 stride = 0;
  stride += stGetTypeMemorySize(type1, 1, count1);
  stride += stGetTypeMemorySize(type2, 1, count2);
  stride += stGetTypeMemorySize(type3, 1, count3);
  stride += stGetTypeMemorySize(type4, 1, count4);
  stride += stGetTypeMemorySize(type5, 1, count5);
  stride += stGetTypeMemorySize(type6, 1, count6);
  stride += stGetTypeMemorySize(type7, 1, count7);
  stride += stGetTypeMemorySize(type8, 1, count8);
  return stride;
}

static u32 findComponentStride(u32 objectHandle, u32 componentHandle)
{
  stContext* ctx = stGetContext_NoMutex();
  stClass* class = findClassFromObjectHandle(&ctx->classes, objectHandle);
  if (class == NULL)
    return 0;
  stComponent* component = getComponentFromHandle(class, componentHandle);
  if (component == NULL)
    return 0;

  return stGetTypeMemorySize(component->type, component->nbElements, 1);
}

static void stSetValue_p_AsInlineData(u32 objectHandle, u32 component, u8 commandType, u8 nbElements, stByte* src, u32 count, u32 srcStride, u32 dstStride, u32 dstMemSize)
{
  u8* dst = stPushCommand(ST_DATA_SIZE(2 + dstMemSize), commandType, objectHandle, (u8) component, 0);

  if (dst == NULL)
  {
    ST_PRINT_ERROR("Cannot value data. Null memory given.");
    stRaiseError(ST_ERROR_RECORDING);
    return;
  }

  dst = stCommandWriteU8(dst, (u8) dstMemSize);
  dst = stCommandWriteU8(dst, (u8) nbElements);

  stStrideMemCpy(dst, src, dstStride, srcStride, count);
}

static void stSetValue_p_AsObjectData(u32 objectHandle, u32 component, u8 commandType, stEnum32 type, u32 nbElements, u32 arraySize, stByte* srcData, u32 srcStride, u32 dstStride)
{

  // Reserve Object Data in the current submission buffer
  stContext* context = stGetContext_NoMutex();
  stCommandBuffer* buffer = context->commandBuffer[ST_COMMAND_BUFFER_INDEX_SUBMISSION];

  struct stObjectDataT* objectData = stObjectData_Obtain(&buffer->objectDatas, (u8) type, (u8) nbElements, arraySize);

  if (objectData == NULL)
  {
    return;
  }

  u8* data = stPushCommand(0, commandType, objectHandle, (u8) component, 0);

  if (data == NULL)
  {
    ST_PRINT_ERROR("Cannot write object data. Null memory given.");
    stRaiseError(ST_ERROR_RECORDING);
    return;
  }

  data = stCommandWriteU32(data, objectData->id);
  data = stCommandWriteU32(data, objectData->arraySize);
  data = stCommandWriteU8(data,  objectData->nbElements);

  stStrideMemCpy(objectData->data, srcData, dstStride, srcStride, arraySize);
}

static void stSetValue_p_entry(u32 objectHandle, u32 component, u8* value, u32 arraySize, u32 stride, u8 inlineCmd, u8 dataArrayCmd, s64 valuePtrOffset)
{
  if (value == NULL || stride == 0 || arraySize == 0)
  {
    stClass* klass = stGetClass_NoMutex(objectHandle);

    u32 classId = 0, userData = 0;

    if (klass != NULL)
    {
      classId = klass->id;
      userData = klass->userData;
    }

    if (value == NULL)
    {
      ST_PRINT_ERRORF("Cannot write value data (p_entry). Null memory given. Object = %i, Component = %i, Type = %i, UserData = %i", objectHandle, component, classId, userData);
    }
    if (stride == 0)
    {
      ST_PRINT_ERRORF("Cannot write value data (p_entry). Stride is 0. Object = %i, Component = %i, Type = %i, UserData = %i", objectHandle, component, classId, userData);
    }

    if (arraySize == 0)
    {
      ST_PRINT_ERRORF("Cannot write value data (p_entry). ArraySize is 0. Object = %i, Component = %i, Type = %i, UserData = %i", objectHandle, component, classId, userData);
    }

    if (stGetContext_NoMutex()->strictMode == true)
    {
      stRaiseError(ST_ERROR_RECORDING);
    }
    return;
  }

  stComponent* comp = getComponentFromHandle2(&stGetContext_NoMutex()->classes, objectHandle, component);

  if (comp == NULL)
  {
    ST_PRINT_ERRORF("Component was not found! For Object %i Component %i", objectHandle, component);
    stRaiseError(ST_ERROR_RECORDING);
    return;
  }

  u32 dstStride  = stGetTypeMemorySize(comp->type, comp->nbElements, 1);
  u32 dstMemSize = dstStride * arraySize;

  u8* src        = value + valuePtrOffset;

#if ST_FORCE_ARRAYS_TO_BE_OBJECT_DATA == 0
  if (dstMemSize < ST_MAX_DATA_PER_COMMAND)
  {
    stSetValue_p_AsInlineData(objectHandle, component, inlineCmd, comp->nbElements, src, arraySize, stride, dstStride, dstMemSize);
  }
  else
  {
    stSetValue_p_AsObjectData(objectHandle, component, dataArrayCmd, comp->type, comp->nbElements, arraySize, src, stride, dstStride);
  }
#else
  ST_UNUSED(inlineCmd);
  ST_UNUSED(destMemSize);
  stSetValue_p_AsObjectData(objectHandle, component, dataArrayCmd, comp->type, comp->nbElements, arraySize, src, stride, destStride);
#endif
}

ST_PUBLIC void stSetValue_p_uint8Ex(u32 objectHandle, u32 component, u8* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_entry(objectHandle, component, (stByte*) value, arraySize, stride, ST_CMD_OBJ_VAL_InlineArrayU8, ST_CMD_OBJ_VAL_DataArrayU8, valuePtrOffsetBytes);
}

ST_PUBLIC void stSetValue_p_uint16Ex(u32 objectHandle, u32 component, u16* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_entry(objectHandle, component, (stByte*) value, arraySize, stride, ST_CMD_OBJ_VAL_InlineArrayU16, ST_CMD_OBJ_VAL_DataArrayU16, valuePtrOffsetBytes);
}

ST_PUBLIC void stSetValue_p_uint32Ex(u32 objectHandle, u32 component, u32* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_entry(objectHandle, component, (stByte*) value, arraySize, stride, ST_CMD_OBJ_VAL_InlineArrayU32, ST_CMD_OBJ_VAL_DataArrayU32, valuePtrOffsetBytes);
}

ST_PUBLIC void stSetValue_p_uint64Ex(u32 objectHandle, u32 component, u64* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_entry(objectHandle, component, (stByte*) value, arraySize, stride, ST_CMD_OBJ_VAL_InlineArrayU64, ST_CMD_OBJ_VAL_DataArrayU64, valuePtrOffsetBytes);
}

ST_PUBLIC void stSetValue_p_int8Ex(u32 objectHandle, u32 component, s8* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_entry(objectHandle, component, (stByte*) value, arraySize, stride, ST_CMD_OBJ_VAL_InlineArrayS8, ST_CMD_OBJ_VAL_DataArrayS8, valuePtrOffsetBytes);
}

ST_PUBLIC void stSetValue_p_int16Ex(u32 objectHandle, u32 component, s16* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_entry(objectHandle, component, (stByte*) value, arraySize, stride, ST_CMD_OBJ_VAL_InlineArrayS16, ST_CMD_OBJ_VAL_DataArrayS16, valuePtrOffsetBytes);
}

ST_PUBLIC void stSetValue_p_int32Ex(u32 objectHandle, u32 component, s32* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_entry(objectHandle, component, (stByte*) value, arraySize, stride, ST_CMD_OBJ_VAL_InlineArrayS32, ST_CMD_OBJ_VAL_DataArrayS32, valuePtrOffsetBytes);
}

ST_PUBLIC void stSetValue_p_int64Ex(u32 objectHandle, u32 component, s64* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_entry(objectHandle, component, (stByte*) value, arraySize, stride, ST_CMD_OBJ_VAL_InlineArrayS64, ST_CMD_OBJ_VAL_DataArrayS64, valuePtrOffsetBytes);
}

ST_PUBLIC void stSetValue_p_float32Ex(u32 objectHandle, u32 component, f32* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_entry(objectHandle, component, (stByte*) value, arraySize, stride, ST_CMD_OBJ_VAL_InlineArrayF32, ST_CMD_OBJ_VAL_DataArrayF32, valuePtrOffsetBytes);
}

ST_PUBLIC void stSetValue_p_float64Ex(u32 objectHandle, u32 component, f64* value, u32 arraySize, u32 stride, s64 valuePtrOffsetBytes)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_entry(objectHandle, component, (stByte*) value, arraySize, stride, ST_CMD_OBJ_VAL_InlineArrayF64, ST_CMD_OBJ_VAL_DataArrayF64, valuePtrOffsetBytes);
}

ST_PUBLIC void stSetValue_p_uint8(u32 objectHandle, u32 component, u8* value, u32 arraySize, u32 stride)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_uint8Ex(objectHandle, component, value, arraySize, stride, 0);
}

ST_PUBLIC void stSetValue_p_uint16(u32 objectHandle, u32 component, u16* value, u32 arraySize, u32 stride)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_uint16Ex(objectHandle, component, value, arraySize, stride, 0);
}

ST_PUBLIC void stSetValue_p_uint32(u32 objectHandle, u32 component, u32* value, u32 arraySize, u32 stride)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_uint32Ex(objectHandle, component, value, arraySize, stride, 0);
}

ST_PUBLIC void stSetValue_p_uint64(u32 objectHandle, u32 component, u64* value, u32 arraySize, u32 stride)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_uint64Ex(objectHandle, component, value, arraySize, stride, 0);
}

ST_PUBLIC void stSetValue_p_int8(u32 objectHandle, u32 component, s8* value, u32 arraySize, u32 stride)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_int8Ex(objectHandle, component, value, arraySize, stride, 0);
}

ST_PUBLIC void stSetValue_p_int16(u32 objectHandle, u32 component, s16* value, u32 arraySize, u32 stride)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_int16Ex(objectHandle, component, value, arraySize, stride, 0);
}

ST_PUBLIC void stSetValue_p_int32(u32 objectHandle, u32 component, s32* value, u32 arraySize, u32 stride)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_int32Ex(objectHandle, component, value, arraySize, stride, 0);
}

ST_PUBLIC void stSetValue_p_int64(u32 objectHandle, u32 component, s64* value, u32 arraySize, u32 stride)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_int64Ex(objectHandle, component, value, arraySize, stride, 0);
}

ST_PUBLIC void stSetValue_p_float32(u32 objectHandle, u32 component, f32* value, u32 arraySize, u32 stride)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_float32Ex(objectHandle, component, value, arraySize, stride, 0);
}

ST_PUBLIC void stSetValue_p_float64(u32 objectHandle, u32 component, f64* value, u32 arraySize, u32 stride)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;
  stSetValue_p_float64Ex(objectHandle, component, value, arraySize, stride, 0);
}


static void stSetValue_p_string_AsInlineData(u32 objectHandle, u32 component, stCommandType commandType, stByte* string, u32 strLength)
{
  u8* dst = stPushCommand(ST_DATA_SIZE(2 + strLength), commandType, objectHandle, (u8) component, 0);

  if (dst == NULL)
  {
    ST_PRINT_ERROR("Cannot value data. Null memory given.");
    stRaiseError(ST_ERROR_RECORDING);
    return;
  }

  dst = stCommandWriteU8(dst, (u8) strLength);
  dst = stStrideMemCpy(dst, string, 1, 1, strLength);
  dst = stCommandWriteU8(dst, 0); // Null terminator.
}

static void stSetValue_p_string_AsObjectData(u32 objectHandle, u32 component, stCommandType commandType, stByte* string, u32 strLength)
{

  // Reserve Object Data in the current submission buffer
  stContext* context = stGetContext_NoMutex();
  stCommandBuffer* buffer = context->commandBuffer[ST_COMMAND_BUFFER_INDEX_SUBMISSION];

  struct stObjectDataT* objectData = stObjectData_Obtain(&buffer->objectDatas, ST_TYPE_UINT8, 1, strLength + 1);

  if (objectData == NULL)
  {
    return;
  }

  u8* data = stPushCommand(0, commandType, objectHandle, (u8) component, 0);

  if (data == NULL)
  {
    ST_PRINT_ERROR("Cannot write object data. Null memory given.");
    stRaiseError(ST_ERROR_RECORDING);
    return;
  }

  data = stCommandWriteU32(data, objectData->id);
  data = stCommandWriteU32(data, strLength);

  u8* objectDataData = stStrideMemCpy(objectData->data, string, 1, 1, strLength);
  stCommandWriteU8(objectDataData, 0); // Null Terminator
}


static void stSetValue_p_string_entry(u32 objectHandle, u32 component, u8* string, u32 strLength, u8 inlineCmd, u8 dataArrayCmd)
{
  if (string == NULL || strLength == 0)
  {
    stClass* klass = stGetClass_NoMutex(objectHandle);

    u32 classId = 0, userData = 0;

    if (klass != NULL)
    {
      classId = klass->id;
      userData = klass->userData;
    }

    if (string == NULL)
    {
      ST_PRINT_ERRORF("Cannot write value data (p_string_entry). Null memory given. Object = %i, Component = %i, Type = %i, UserData = %i", objectHandle, component, classId, userData);
    }

    if (strLength == 0)
    {
      ST_PRINT_ERRORF("Cannot write value data (p_string_entry). ArraySize is 0. Object = %i, Component = %i, Type = %i, UserData = %i", objectHandle, component, classId, userData);
    }

    if (stGetContext_NoMutex()->strictMode == true)
    {
      stRaiseError(ST_ERROR_RECORDING);
    }
    return;
  }

  stComponent* comp = getComponentFromHandle2(&stGetContext_NoMutex()->classes, objectHandle, component);

  if (comp == NULL)
  {
    ST_PRINT_ERRORF("Component was not found! For Object %i Component %i", objectHandle, component);
    stRaiseError(ST_ERROR_RECORDING);
    return;
  }

#if ST_FORCE_ARRAYS_TO_BE_OBJECT_DATA == 0
  if (strLength < (ST_MAX_DATA_PER_COMMAND - 2))
  {
    stSetValue_p_string_AsInlineData(objectHandle, component, inlineCmd, string, strLength);
  }
  else
  {
    stSetValue_p_string_AsObjectData(objectHandle, component, dataArrayCmd, string, strLength);
  }
#else
  ST_UNUSED(inlineCmd);
  stSetValue_p_string_AsObjectData(objectHandle, component, dataArrayCmd, comp->type, string, strLength);
#endif
}

void stSetValue_string(u32 objectHandle, u32 component, stCStr str, u32 strLength)
{
  ST_ABORT_ON_NO_CONTEXT_NO_MUTEX();
  ST_ABORT_ON_NULL_HANDLE;

  if (str == NULL || strLength == 0)
  {
    return;
  }

  ST_PRINT_CHATTERF("String size is %i", strLength);
  ST_PRINT_CHATTERF("Str is %s", str);

  stSetValue_p_string_entry(objectHandle, component, (u8*) str, strLength, ST_CMD_OBJ_VAL_InlineString, ST_CMD_OBJ_VAL_DataString);
}
