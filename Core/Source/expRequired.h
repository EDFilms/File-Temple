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

#pragma once

// Autodoc

#define STEXP_PRIVATE_FUNCTION(...)
#define STEXP_PUBLIC_FUNCTION(...)
#define STEXP_PRIVATE_STRUCT(...)
#define STEXP_PUBLIC_STRUCT(...)
#define STEXP_PUBLIC_ENUM(...)
#define STEXP_PRIVATE_ENUM(...)
#define STEXP_TEST(...)
#define STEXP_COMMENT(...)
#define STEXP_TODO(...)
#define STEXP_DEPRECATED(...)
#define STEXP_UNUSED(X) (void)(X)
#define STEXP_INITIALIZER(...)
#define STEXP_SHUTDOWN(...)

#include "SceneTrack.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <set>

#include <stdint.h>
#include <assert.h>

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
typedef bool        stBool;
typedef u32         stBool32;
typedef const char* stCStr;
typedef char*       stStr;
typedef int         stEnum32;

#define STEXP_PLATFORM_UNKNOWN    0

// Microsoft Windows
#define STEXP_PLATFORM_WINDOWS    10

// Catch-all for Modern Apple Operating System
#define STEXP_PLATFORM_MACH       20
// Mac OS X subtype of Mach
#define STEXP_PLATFORM_OSX        31
// iPhone subtype of Mach
#define STEXP_PLATFORM_IPHONE     32

// Platform is Windows and architecture
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64) || defined(STEXP_WIN32) || defined(STEXP_WIN64)

#define STEXP_PLATFORM STEXP_PLATFORM_WINDOWS
#define STEXP_PUBLIC  __declspec(dllexport)
#define STEXP_INLINE inline

#if defined(WIN64) || defined(_WIN64)
#define STEXP_ARCHITECTURE 64
#define STEXP_DEFAULT_ALIGNMENT 4
#else
#define STEXP_ARCHITECTURE 32
#define STEXP_DEFAULT_ALIGNMENT 4
#endif

#if defined(_DEBUG) || defined(DEBUG)
#define STEXP_DEBUG 1
#define STEXP_DEBUG_LEVEL 3
#endif

// Platform is Mac OS X
#elif defined(__APPLE__) || defined(__MACH__) || defined(STEXP_APPLE)

// See: https://opensource.apple.com/source/CarbonHeaders/CarbonHeaders-18.1/TargetConditionals.h
#include "TargetConditionals.h"
#define STEXP_PLATFORM STEXP_PLATFORM_MACH
#define STEXP_PUBLIC extern
#define STEXP_INLINE static inline

#if TARGET_OS_MAC == 1
#define STEXP_PLATFORM_SPECIFIC STEXP_PLATFORM_OSX
#elif TARGET_OS_IPHONE == 2
#define STEXP_PLATFORM_SPECIFIC STEXP_PLATFORM_IPHONE
#endif

#if TARGET_RT_64_BIT == 1
#define STEXP_ARCHITECTURE 64
#define STEXP_DEFAULT_ALIGNMENT 8
#else
#define STEXP_ARCHITECTURE 32
#define STEXP_DEFAULT_ALIGNMENT 4
#endif

#else
#define STEXP_PLATFORM     STEXP_PLATFORM_UNKNOWN
#define STEXP_ARCHITECTURE 32
#error "Unknown Platform"
#endif

#define STEXP_ENDIAN_LITTLE 0
#define STEXP_ENDIAN_BIG    1
#define STEXP_ENDIAN_MSB    ST_ENDIAN_LITTLE
#define STEXP_ENDIAN_LSB    ST_ENDIAN_BIG

// == Endian
#if STEXP_PLATFORM == STEXP_PLATFORM_WINDOWS

#if defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || defined(_M_ARM)
#define STEXP_ENDIAN STEXP_ENDIAN_LITTLE
#elif defined(_M_PPC)
#define STEXP_ENDIAN STEXP_ENDIAN_BIG
#else
#error "Unknown Endian on Windows"
#endif

#elif STEXP_PLATFORM == STEXP_PLATFORM_MACH

#if TARGET_RT_LITTLE_ENDIAN == 1
#define STEXP_ENDIAN STEXP_ENDIAN_LITTLE
#elif TARGET_RT_BIG_ENDIAN == 1
#define STEXP_ENDIAN STEXP_ENDIAN_BIG
#else
#error "Unknown Endian on Mach"
#endif

#else

#error "Unknown Endian"

#endif

#define STEXP_ARRAY_COUNT(X)     (sizeof(X) / sizeof((X)[0]))


FILE* GetLogFile();

#define ST_LOGF(TYPE, FMT, ...)   do { FILE* tlg = GetLogFile(); if (tlg != NULL) { fprintf(tlg, TYPE " " FMT "\n", __VA_ARGS__); fflush(tlg);  } } while(0)
#define ST_LOG(TYPE, TXT)         do { FILE* tlg = GetLogFile(); if (tlg != NULL) { fprintf(tlg, TYPE " " TXT "\n"); fflush(tlg);  } } while(0)
#define ST_LOGP(FMT, ...)         do { FILE* tlg = GetLogFile(); if (tlg != NULL) { fprintf(tlg, FMT, __VA_ARGS__);  } } while(0)
#define ST_LOGP2(TXT)             do { FILE* tlg = GetLogFile(); if (tlg != NULL) { fprintf(tlg, TXT);   } } while(0)

#if STEXP_DEBUG == 1
#define STEXP_PRINT_INFO(FMT, ...)        ST_LOG("...", FMT)
#define STEXP_PRINT_INFOF(FMT, ...)       ST_LOGF("...", FMT, __VA_ARGS__)
#define STEXP_PRINT_INFOF_BEGIN()         ST_LOGP2("... ")
#define STEXP_PRINT_INFOF_END()           ST_LOGP2("\n")
#define STEXP_PRINT_INFOF_PART(FMT, ...)  ST_LOGP(FMT, __VA_ARGS__)
#define STEXP_PRINT_INFO_PART(FMT)        ST_LOGP2(FMT)
#define STEXP_PRINT_ERRORF(FMT, ...)      ST_LOGF("ERR", FMT, __VA_ARGS__)
#define STEXP_PRINT_WARNF(FMT, ...)       ST_LOGF("WRN", FMT, __VA_ARGS__)
#define STEXP_PRINT_CHATTERF(FMT, ...)    ST_LOGF("...", FMT, __VA_ARGS__)
#define STEXP_PRINT_CHATTER(TXT)          ST_LOG("...", TXT)
#define STEXP_PRINT_ERROR(TXT)            ST_LOG("ERR", TXT)
#define STEXP_PRINT_WARN(TXT)             ST_LOG("WRN", TXT)
#define STEXP_MARKER(A)                   ST_LOG("[MARKER]" ##A)
#define STEXP_DBG_FN "[" __FUNCTION__ "] "
#else
#define STEXP_PRINT_INFO(FMT, ...)        ST_LOG("...", FMT)
#define STEXP_PRINT_INFOF(FMT, ...)       ST_LOGF("...", FMT, __VA_ARGS__)
#define STEXP_PRINT_INFOF_BEGIN()         ST_LOGP2("... ")
#define STEXP_PRINT_INFOF_END()           ST_LOGP2("\n")
#define STEXP_PRINT_INFOF_PART(FMT, ...)  ST_LOGP(FMT, __VA_ARGS__)
#define STEXP_PRINT_INFO_PART(FMT)        ST_LOGP2(FMT)
#define STEXP_PRINT_ERRORF(FMT, ...)      ST_LOGF("ERR", FMT, __VA_ARGS__)
#define STEXP_PRINT_WARNF(FMT, ...)       ST_LOGF("WRN", FMT, __VA_ARGS__)
#define STEXP_PRINT_CHATTERF(FMT, ...)    
#define STEXP_PRINT_CHATTER(TXT)          
#define STEXP_PRINT_ERROR(TXT)            ST_LOG("ERR", TXT)
#define STEXP_PRINT_WARN(TXT)             ST_LOG("WRN", TXT)
#define STEXP_MARKER(A)                   ST_LOG("[MARKER]" ##A)
#define STEXP_DBG_FN "[" __FUNCTION__ "] "
#endif

namespace EXP_NAMESPACE
{
  template<typename T> class Type { public: static constexpr u32 kType = ST_TYPE_NONE;     static constexpr u32 kNbElements = 0; };
  template<> class Type<u8>       { public: static constexpr u32 kType = ST_TYPE_UINT8;    static constexpr u32 kNbElements = 1; }; typedef Type<u8>  u8Type;
  template<> class Type<u16>      { public: static constexpr u32 kType = ST_TYPE_UINT16;   static constexpr u32 kNbElements = 1; }; typedef Type<u16> u16Type;
  template<> class Type<u32>      { public: static constexpr u32 kType = ST_TYPE_UINT32;   static constexpr u32 kNbElements = 1; }; typedef Type<u32> u32Type;
  template<> class Type<u64>      { public: static constexpr u32 kType = ST_TYPE_UINT64;   static constexpr u32 kNbElements = 1; }; typedef Type<u64> u64Type;
  template<> class Type<s8>       { public: static constexpr u32 kType = ST_TYPE_INT8;     static constexpr u32 kNbElements = 1; }; typedef Type<s8>  s8Type;
  template<> class Type<s16>      { public: static constexpr u32 kType = ST_TYPE_INT16;    static constexpr u32 kNbElements = 1; }; typedef Type<s16> s16Type;
  template<> class Type<s32>      { public: static constexpr u32 kType = ST_TYPE_INT32;    static constexpr u32 kNbElements = 1; }; typedef Type<s32> s32Type;
  template<> class Type<s64>      { public: static constexpr u32 kType = ST_TYPE_INT64;    static constexpr u32 kNbElements = 1; }; typedef Type<s64> s64Type;
  template<> class Type<f32>      { public: static constexpr u32 kType = ST_TYPE_FLOAT32;  static constexpr u32 kNbElements = 1; }; typedef Type<f32> f32Type;
  template<> class Type<f64>      { public: static constexpr u32 kType = ST_TYPE_FLOAT64;  static constexpr u32 kNbElements = 1; }; typedef Type<f64> f64Type;
  template<> class Type<stCStr>   { public: static constexpr u32 kType = ST_TYPE_CSTRING;  static constexpr u32 kNbElements = 1; }; typedef Type<stCStr> StrType;
}
