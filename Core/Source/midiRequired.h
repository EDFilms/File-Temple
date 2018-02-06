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

#include "SceneTrackStatic.h"
#include "expAutoDoc.h"

#define STMIDI_PRIVATE_FUNCTION(...)
#define STMIDI_PUBLIC_FUNCTION(...)
#define STMIDI_PRIVATE_STRUCT(...)
#define STMIDI_PUBLIC_STRUCT(...)
#define STMIDI_PUBLIC_ENUM(...)
#define STMIDI_PRIVATE_ENUM(...)
#define STMIDI_TEST(...)
#define STMIDI_COMMENT(...)
#define STMIDI_TODO(...)
#define STMIDI_DEPRECATED(...)
#define STMIDI_UNUSED(X) (void)(X)
#define STMIDI_INITIALIZER(...)
#define STMIDI_SHUTDOWN(...)

#define EXP_LOG_PREFIX "[STMIDI]"
#define EXP_NAMESPACE SceneTrackMidi
#include "expRequired.h"

#define STMIDI_DEBUG                   STEXP_DEBUG
#define STMIDI_DEBUG_LEVEL             STEXP_DEBUG_LEVEL
#define STMIDI_PUBLIC                  STEXP_PUBLIC
#define STMIDI_INLINE                  STEXP_INLINE
#define STMIDI_ARRAY_COUNT             STEXP_ARRAY_COUNT
#define STMIDI_PRINT_INFO              STEXP_PRINT_INFO
#define STMIDI_PRINT_INFOF             STEXP_PRINT_INFOF
#define STMIDI_PRINT_INFOF_BEGIN       STEXP_PRINT_INFOF_BEGIN
#define STMIDI_PRINT_INFOF_END         STEXP_PRINT_INFOF_END
#define STMIDI_PRINT_INFOF_PART        STEXP_PRINT_INFOF_PART
#define STMIDI_PRINT_INFO_PART         STEXP_PRINT_INFO_PART
#define STMIDI_PRINT_ERRORF            STEXP_PRINT_ERRORF
#define STMIDI_PRINT_WARNF             STEXP_PRINT_WARNF
#define STMIDI_PRINT_CHATTERF          STEXP_PRINT_CHATTERF
#define STMIDI_PRINT_REALLY_CHATTERF   STEXP_PRINT_REALLY_CHATTERF
#define STMIDI_PRINT_ERROR             STEXP_PRINT_ERROR
#define STMIDI_PRINT_WARN              STEXP_PRINT_WARN
#define STMIDI_PRINT_CHATTER           STEXP_PRINT_CHATTER
#define STMIDI_MARKER                  STEXP_MARKER

namespace SceneTrackMidi
{
  class ExporterT;
  class PhysicsEvent_t;
}
