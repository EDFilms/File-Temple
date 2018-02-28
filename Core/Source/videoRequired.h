// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * videoRequired.h
 *
 * Common macros, constants and functions used throughout the FbxExporter
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "SceneTrackStatic.h"
#include "expAutoDoc.h"

#define STVIDEO_PRIVATE_FUNCTION(...)
#define STVIDEO_PUBLIC_FUNCTION(...)
#define STVIDEO_PRIVATE_STRUCT(...)
#define STVIDEO_PUBLIC_STRUCT(...)
#define STVIDEO_PUBLIC_ENUM(...)
#define STVIDEO_PRIVATE_ENUM(...)
#define STVIDEO_TEST(...)
#define STVIDEO_COMMENT(...)
#define STVIDEO_TODO(...)
#define STVIDEO_DEPRECATED(...)
#define STVIDEO_UNUSED(X) (void)(X)
#define STVIDEO_INITIALIZER(...)
#define STVIDEO_SHUTDOWN(...)

#define EXP_LOG_PREFIX "[STVIDEO]"
#define EXP_NAMESPACE SceneTrackVideo
#include "expRequired.h"

#define STVIDEO_DEBUG                   STEXP_DEBUG
#define STVIDEO_DEBUG_LEVEL             STEXP_DEBUG_LEVEL
#define STVIDEO_PUBLIC                  STEXP_PUBLIC
#define STVIDEO_INLINE                  STEXP_INLINE
#define STVIDEO_ARRAY_COUNT             STEXP_ARRAY_COUNT
#define STVIDEO_PRINT_INFO              STEXP_PRINT_INFO
#define STVIDEO_PRINT_INFOF             STEXP_PRINT_INFOF
#define STVIDEO_PRINT_INFOF_BEGIN       STEXP_PRINT_INFOF_BEGIN
#define STVIDEO_PRINT_INFOF_END         STEXP_PRINT_INFOF_END
#define STVIDEO_PRINT_INFOF_PART        STEXP_PRINT_INFOF_PART
#define STVIDEO_PRINT_INFO_PART         STEXP_PRINT_INFO_PART
#define STVIDEO_PRINT_ERRORF            STEXP_PRINT_ERRORF
#define STVIDEO_PRINT_WARNF             STEXP_PRINT_WARNF
#define STVIDEO_PRINT_CHATTERF          STEXP_PRINT_CHATTERF
#define STVIDEO_PRINT_REALLY_CHATTERF   STEXP_PRINT_REALLY_CHATTERF
#define STVIDEO_PRINT_ERROR             STEXP_PRINT_ERROR
#define STVIDEO_PRINT_WARN              STEXP_PRINT_WARN
#define STVIDEO_PRINT_CHATTER           STEXP_PRINT_CHATTER
#define STVIDEO_MARKER                  STEXP_MARKER

namespace SceneTrackVideo
{
  class ExporterT;
  class PhysicsEvent_t;
}
