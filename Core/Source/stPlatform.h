// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stPlatform.h
 *
 * Configuration macros and common functions/macros, and Windows/macOS portable functions.
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_PLATFORM_H
#define ST_PLATFORM_H

#include "stAutoDoc.h"

#include "Config.inc"

#ifndef ST_CFG_MULTI_THREADED
#define ST_CFG_MULTI_THREADED   1
#endif

#ifndef ST_CFG_DATA_PROBE
#define ST_CFG_DATA_PROBE       1
#endif

#ifndef ST_CFG_INTERNAL_TESTS
#define ST_CFG_INTERNAL_TESTS   1
#endif

#ifndef ST_CFG_CHANGES_IS_DEFAULT
#define ST_CFG_JOURNALING_IS_DEFAULT 1
#endif

#ifndef ST_CFG_FILE_READING_SAFE_MODE
#define ST_CFG_FILE_READING_SAFE_MODE 1
#endif

#define ST_PLATFORM_UNKNOWN    0

// Microsoft Windows
#define ST_PLATFORM_WINDOWS    10

// Catch-all for Modern Apple Operating System
#define ST_PLATFORM_MACH       20
// Mac OS X subtype of Mach
#define ST_PLATFORM_OSX        31
// iPhone subtype of Mach
#define ST_PLATFORM_IPHONE     32


// == Platform
#ifndef ST_PLATFORM

// Platform is Windows and architecture
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64) || defined(ST_WIN32) || defined(ST_WIN64)

  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #define __UNDEF_LEAN_AND_MEAN
  #endif
  #include <windows.h>
  #ifdef __UNDEF_LEAN_AND_MEAN
    #undef WIN32_LEAN_AND_MEAN
    #undef __UNDEF_LEAN_AND_MEAN
  #endif


  #define ST_PLATFORM ST_PLATFORM_WINDOWS
  #define ST_PUBLIC __declspec(dllexport)
  #define ST_INLINE inline

  #if defined(WIN64) || defined(_WIN64)
    #define ST_ARCHITECTURE 64
    #define ST_DEFAULT_ALIGNMENT 4
  #else
    #define ST_ARCHITECTURE 32
    #define ST_DEFAULT_ALIGNMENT 4
  #endif

  #if defined(_DEBUG) || defined(DEBUG)
    #define ST_DEBUG 1
  #endif

// Platform is Mac OS X
#elif defined(__APPLE__) || defined(__MACH__) || defined(ST_APPLE)

  // See: https://opensource.apple.com/source/CarbonHeaders/CarbonHeaders-18.1/TargetConditionals.h
  #include "TargetConditionals.h"
  #define ST_PLATFORM ST_PLATFORM_MACH
  #define ST_PUBLIC   extern
  #define ST_INLINE static inline

  #if TARGET_OS_MAC == 1
    #define ST_PLATFORM_SPECIFIC ST_PLATFORM_OSX
  #elif TARGET_OS_IPHONE == 2
    #define ST_PLATFORM_SPECIFIC ST_PLATFORM_IPHONE
  #endif

  #if TARGET_RT_64_BIT == 1
    #define ST_ARCHITECTURE 64
    #define ST_DEFAULT_ALIGNMENT 8
  #else
    #define ST_ARCHITECTURE 32
    #define ST_DEFAULT_ALIGNMENT 4
  #endif

#else
#define ST_PLATFORM     ST_PLATFORM_UNKNOWN
#define ST_ARCHITECTURE 32
#error "Unknown Platform"
#endif

#endif

#define ST_ENDIAN_LITTLE 0
#define ST_ENDIAN_BIG    1
#define ST_ENDIAN_MSB    ST_ENDIAN_LITTLE
#define ST_ENDIAN_LSB    ST_ENDIAN_BIG

// == Endian
#if ST_PLATFORM == ST_PLATFORM_WINDOWS

  #if defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || defined(_M_ARM)
    #define ST_ENDIAN ST_ENDIAN_LITTLE
  #elif defined(_M_PPC)
    #define ST_ENDIAN ST_ENDIAN_BIG
  #else
    #error "Unknown Endian on Windows"
  #endif

#elif ST_PLATFORM == ST_PLATFORM_MACH

  #if TARGET_RT_LITTLE_ENDIAN == 1
    #define ST_ENDIAN ST_ENDIAN_LITTLE
  #elif TARGET_RT_BIG_ENDIAN == 1
    #define ST_ENDIAN ST_ENDIAN_BIG
  #else
    #error "Unknown Endian on Mach"
  #endif

#else

  #error "Unknown Endian"
  
#endif

#ifndef ST_DEBUG
#define ST_DEBUG 0
#endif

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
  POD (Plain old data) types are in the format of:
    u - unsigned integer
    s - signed 
    f - float
  With the number of bits following the type.
 */
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;
typedef float       f32;
typedef double      f64;
typedef u8          stByte;
typedef _Bool       stBool;
typedef u32         stBool32;
typedef const char* stCStr;
typedef char*       stStr;
typedef int         stEnum32;

#define U8  u8
#define U16 u16
#define U32 u32
#define U64 u64
#define S8  s8
#define S16 s16
#define S32 s32
#define S64 s64
#define F32 f32
#define F64 f64

typedef struct
{
  char c[4];
} stFourCc;

/**
 * Make a FourCC (Four character code) from a string.
 * Used for the SceneTrack (.st) file header and chunk headers.
 */
ST_INLINE void stMakeFourCc(stFourCc* cc, const char* x)
{
  cc->c[0] = 0;
  cc->c[1] = 0;
  cc->c[2] = 0;
  cc->c[3] = 0;

  if (x == NULL)
    return;
  else
  {
    size_t n = strlen(x);
    if (n > 4)
      n = 4;
    for(u32 i=0;i < n;i++)
      cc->c[i] = x[i];
  }
}

/**
 * Converts a FourCC into an 32-bit integer with respect to endianness
 */
ST_INLINE u32 stFourCcToUInt(stFourCc* cc)
{
#if ST_ENDIAN != ST_ENDIAN_LITTLE
  return (u32)(cc->c[0]) + (((u32)(cc->c[1])) << 8) + (((u32)(cc->c[2])) << 16) + (((u32)(cc->c[3])) << 24);
#else
  return (u32)(cc->c[3]) + (((u32)(cc->c[2])) << 8) + (((u32)(cc->c[1])) << 16) + (((u32)(cc->c[0])) << 24);
#endif
}

/**
 * Print out a FourCC to stdout
 */
ST_INLINE void stPrintIntAsFourCc(int v)
{
  char c[4];
#if ST_ENDIAN != ST_ENDIAN_LITTLE
  c[0] = v & 0xff;
  c[1] = (v >> 8) & 0xff;
  c[2] = (v >> 16) & 0xff;
  c[3] = (v >> 24) & 0xff;
#else
  c[3] = v & 0xff;
  c[2] = (v >> 8) & 0xff;
  c[1] = (v >> 16) & 0xff;
  c[0] = (v >> 24) & 0xff;
#endif
  printf("%.4s", c);
  
}

/**
 * Print out a FourCC to a char array
 */
ST_INLINE void stSprintIntAsFourCC(int v, char c[4])
{
#if ST_ENDIAN != ST_ENDIAN_LITTLE
  c[0] = v & 0xff;
  c[1] = (v >> 8) & 0xff;
  c[2] = (v >> 16) & 0xff;
  c[3] = (v >> 24) & 0xff;
#else
  c[3] = v & 0xff;
  c[2] = (v >> 8) & 0xff;
  c[1] = (v >> 16) & 0xff;
  c[0] = (v >> 24) & 0xff;
#endif
}

/**
 * Portable version of fseek for 64-bit file addressing (> 4GB).
 * All file code should use this version instead of the native platform version; fseek.
 */
ST_INLINE int stFseek64(FILE* stream, s64 pos, int origin)
{
#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  return _fseeki64(stream, pos, origin);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  return fseeko(stream, pos, origin);
#endif
}

/**
 * Portable version of ftell for 64-bit file addressing (> 4GB)
 * All file code should use this version instead of the native platform version; ftell.
 */
ST_INLINE s64 stFtell64(FILE* stream)
{
#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  return _ftelli64(stream);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  return ftello(stream);
#endif
}

/**
 * Macro for the minimum of two values
 */
#define stMin(X, Y) ((X) < (Y) ? (X) : (Y))

/**
 * Macro for the maximum of two values
 */
#define stMax(X, Y) ((X) > (Y) ? (X) : (Y))

#ifndef true
  #define true 1
#endif

#ifndef false
  #define false 0
#endif

#ifndef NULL
  #define NULL 0
#endif


/**
 * Assert/Warning Macros
 *
 * Assert macros provide a portable and configurable way of asserting or warning the user/programmer
 * for any conflicts or incorrect parameters in code.
 */
#define ST_NOOP               ;

/**
 * Macro for a compile time based assertion
 */
#define ST_STATIC_ASSERT(COND, TOKEN) typedef int TOKEN[COND ? 1 : -1]

/**
 * Macro to issue a warning to stdout when a condition is met.
 */
#define ST_WARNING(TRUE_COND, FMT, ...) do { if (TRUE_COND) { printf(FMT "\n", __VA_ARGS__); } } while(0)

/**
 * Macro to issue always a warning when a condition is met.
 */
#define ST_ALWAYS_WARNING(FMT, ...) do { printf(FMT "\n", __VA_ARGS__); } while(0)

extern FILE* sOut;

/**
 * Macro to log a formatted string to the log file
 */
#define ST_LOG(TYPE, FMT, ...)  do { if (sOut != NULL) { fprintf(sOut, "[ST] " TYPE " " FMT "\n", __VA_ARGS__); fflush(sOut);  } } while(0)

/**
 * Macro to log a string to the log file
 */
#define ST_LOG2(TYPE, TXT)      do { if (sOut != NULL) { fprintf(sOut, "[ST] " TYPE " " TXT "\n"); fflush(sOut);  } } while(0)


#if ST_DEBUG == 1
#define ST_LOG_LEVEL 4
#else
#define ST_LOG_LEVEL 1
#endif

/**
 * Macros to extensive debug information to the log
 */
#if ST_LOG_LEVEL >= 4
#define ST_PRINT_REALLY_CHATTERF(FMT, ...)    ST_LOG("...", FMT, __VA_ARGS__)
#define ST_PRINT_REALLY_CHATTER(FMT)          ST_LOG2("...", FMT)
#else
#define ST_PRINT_REALLY_CHATTERF(FMT, ...)
#define ST_PRINT_REALLY_CHATTER(FMT)
#endif

/**
 * Macros to debug information to the log
 */
#if ST_LOG_LEVEL >= 3
#define ST_PRINT_CHATTERF(FMT, ...)            ST_LOG("...", FMT, __VA_ARGS__)
#define ST_PRINT_CHATTER(FMT)                  ST_LOG2("...", FMT)
#else
#define ST_PRINT_CHATTERF(FMT, ...)
#define ST_PRINT_CHATTER(FMT)
#endif

/**
 * Macros to warning information to the log
 */
#if ST_LOG_LEVEL >= 2
#define ST_PRINT_WARNF(FMT, ...)               ST_LOG("WAR", FMT, __VA_ARGS__)
#define ST_PRINT_WARN(FMT)                     ST_LOG2("WAR", FMT)
#else
#define ST_PRINT_WARNF(FMT, ...)
#define ST_PRINT_WARN(FMT)
#endif

/**
 * Macros to error information to the log
 */
#if ST_LOG_LEVEL >= 1
#define ST_PRINT_ERRORF(FMT, ...)               ST_LOG("ERR", FMT, __VA_ARGS__)
#define ST_PRINT_ERROR(FMT)                     ST_LOG2("ERR", FMT)
#else
#define ST_PRINT_ERRORF(FMT, ...)
#define ST_PRINT_ERROR(FMT)
#endif

/**
 * Macros to user information to the log
 */
#if ST_LOG_LEVEL >= 0
#define ST_PRINT_INFOF(FMT, ...)               ST_LOG("   ", FMT, __VA_ARGS__)
#define ST_PRINT_INFO(FMT)                     ST_LOG2("   ", FMT)
#else
#define ST_PRINT_INFOF(FMT, ...)
#define ST_PRINT_INFO(FMT)
#endif

#define ST_DBG_FN "[" __FUNCTION__ "] "


/**
 * Macro to assert when a pointer type is NULL
 */
#define ST_NULL_CHECK(X)       do { if ((X) == NULL) { ST_ALWAYS_ASSERT("Pointer Is Null"); } } while(0)

/**
 * Portable Macro to assert and write to the log when a condition is NOT met.
 * Please use this macro instead of assert due to portability reasons.
 */
#define ST_ASSERT(X, TEXT)     do { if (!(X)) { ST_LOG("!!!", "Assertion!\n\n%s\n%s:%i", TEXT, __FILE__, __LINE__); } assert(X); } while(0)

/**
 * Portable Macro to assert and write to the log.
 * Please use this macro instead of assert(false) due to portability reasons.
 */
#define ST_ALWAYS_ASSERT(TEXT) do { ST_LOG("!!!", "Assertion!\n\n%s\n%s:%i", TEXT, __FILE__, __LINE__); assert(0); } while(0)


/**
 * Macro to convert units into bytes
 */
#define ST_BYTES(X)           (X)

/**
 * Macro to bytes into kilobytes
 */
#define ST_KILOBYTES(X)       ((X) * 1024)

/**
 * Macro to kilobytes into megabytes
 */
#define ST_MEGABYTES(X)       ((ST_KILOBYTES(X)) * 1024)

/**
 * Macro to count the number of items in the array at compile time
 */
#define ST_ARRAY_COUNT(X)     (sizeof(X) / sizeof((X)[0]))
#define STR_XSTR(X) #X

/**
 * Macro to stringify a token
 */
#define STR_STR(X) ST_XSTR(X)

/**
 * Macro to set the power of 2 flag bit of flags.
 */
#define ST_SET_FLAG(FLAGS, FLAG)    do { (FLAGS) |= (FLAG); } while(0)

/**
 * Macro to clear the power of 2 flag bit of flags.
 */
#define ST_CLEAR_FLAG(FLAGS, FLAG)  do { (FLAGS) &= ~(FLAG); } while(0)

/**
 * Macro to check the power of 2 flag bit of flags.
 */
#define ST_HAS_FLAG(FLAGS, FLAG)    ( ((FLAGS) & (FLAG)) != 0 )

#ifndef ST_CFG_INTERNAL_TESTS
#define ST_CFG_INTERNAL_TESTS 1
#endif

#ifndef ST_CFG_DATA_PROBE
#define ST_CFG_DATA_PROBE 1
#endif

#if ST_CFG_INTERNAL_TESTS == 1
#define ST_PUBLIC_TEST        ST_PUBLIC
#else
#define ST_PUBLIC_TEST
#endif

#if ST_CFG_DATA_PROBE == 1
#define ST_PUBLIC_PROBE        ST_PUBLIC
#else
#define ST_PUBLIC_PROBE
#endif

/**
 * Memory allocation
 *
 * These are a collection of portable and threadsafe macros to obtain, reallocate and release memory.
 * Based on configuration these macros can be switched to extensive logging to track memory leaks or heavy usage.
 * Please do not use malloc, free or realloc but use these portable macros instead.
 * @see ST_CFG_TRACK_MEMORY
 */
#if ST_CFG_TRACK_MEMORY == 0

/**
 * Allocate some memory from the heap
 * @threadsafe
 */
void* stAllocateMemoryFromHeapImpl(size_t nbBytes, size_t alignment);
#define ST_ALLOCATE_MEM(NB_BYTES, ALIGNMENT)  stAllocateMemoryFromHeapImpl(NB_BYTES, ALIGNMENT)
void* stReallocateMemoryFromHeapImpl(void* mem, size_t size);

/**
 * Reallocate (grow or shrink) from existing allocated memory from the heap
 * @threadsafe
 */
#define ST_REALLOCATE_MEM(MEM, NB_BYTES)  stReallocateMemoryFromHeapImpl(MEM, NB_BYTES)
#define ST_REALLOCATE_MEM_REF(MEM, NB_BYTES, REF)  stReallocateMemoryFromHeapImpl(MEM, NB_BYTES)
#else

/**
 * Allocate some memory from the heap (with debugging information)
 * @threadsafe
 */
void* stAllocateMemoryFromHeapImpl(size_t nbBytes, size_t alignment, const char* file, int line, int ref);
#define ST_ALLOCATE_MEM(NB_BYTES, ALIGNMENT)  stAllocateMemoryFromHeapImpl(NB_BYTES, ALIGNMENT, __FILE__, __LINE__, 0)
#define ST_ALLOCATE_MEM_REF(NB_BYTES, ALIGNMENT, REF)  stAllocateMemoryFromHeapImpl(NB_BYTES, ALIGNMENT, __FILE__, __LINE__, REF)

/**
 * Reallocate (grow or shrink) from existing allocated memory from the heap (with debugging information)
 * @threadsafe
 */
void* stReallocateMemoryFromHeapImpl(void* mem, size_t size, const char* file, int line, int ref);
#define ST_REALLOCATE_MEM(MEM, NB_BYTES)  stReallocateMemoryFromHeapImpl(MEM, NB_BYTES, __FILE__, __LINE__, 0)
#define ST_REALLOCATE_MEM_REF(MEM, NB_BYTES, REF)  stReallocateMemoryFromHeapImpl(MEM, NB_BYTES, __FILE__, __LINE__, REF)
#endif

/**
 * Release allocated memory from the heap.
 * @threadsafe
 */
void  stReleaseMemoryFromHeapImpl(void* memory);
#define ST_RELEASE_MEM(MEM) stReleaseMemoryFromHeapImpl(MEM)

/**
 * Initialies the memory mutex and memory tracking (if enabled)
 */
void  stInitialiseHeapMemory();

/**
 * Releases the memory mutex and memory tracking (if enabled)
 * Does not free any other allocated memory!
 */
void  stShutdownHeapMemory();

/**
 * Helper macro to allocate type of variable from the heap with default memory alignment.
 * Similar to C++ new
 */
#define ST_NEW_FROM_HEAP(TYPE)          ST_ALLOCATE_MEM(sizeof(TYPE), ST_DEFAULT_ALIGNMENT)

/**
 * Helper macro to release a variable that was allocated from the heap.
 * Similar to C++ delete
 */
#define ST_DELETE_FROM_HEAP(MEM)        ST_RELEASE_MEM(MEM)

/**
 * Helper macro to adjust the size of allocated memory from the heap
 */
#define ST_REALLOC_FROM_HEAP(MEM, SIZE) ST_REALLOCATE_MEM(MEM, SIZE)

/**
 * Portable macro for opening a file.
 * All file code should use this version instead of the native platform version; fopen.
 */
#define ST_FOPEN(FD, P, A)  do { FD = fopen(P, A); ST_LOG("FILE", "Opened File %s as %s, FileDesc=%p", P, A, FD); } while(0)

/**
 * Portable macro for closing a file.
 * All file code should use this version instead of the native platform version; fclose.
 */
#define ST_FCLOSE(FD)  do { int _r = fclose(FD); ST_LOG("FILE", "Closed FileDesc=%p, Result = %i", FD, _r); } while(0) 

#endif
