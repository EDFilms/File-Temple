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

#include "midiRequired.h"

extern "C"
{

STMIDI_PUBLIC_FUNCTION(Named="midiSetAxisSwizzle", Text="Set Swizzle for a particular axis", Category="Settings", ThreadSafe)
STMIDI_PUBLIC s32 midiSetAxisSwizzle(stEnum32 nodeType, stEnum32 trsMask, stEnum32 srcAxis, stEnum32 dstAxis, s32 sign);

STMIDI_PUBLIC_FUNCTION(Named="midiSetAxisOperation", Text="Set Swizzle for a particular axis", Category="Settings", ThreadSafe)
STMIDI_PUBLIC s32 midiSetAxisOperation(stEnum32 nodeType, stEnum32 trsMask, stEnum32 dstAxis, stEnum32 operatorType, f32 constantValue);

STMIDI_PUBLIC_FUNCTION(Named="midiSetAxisRotationOrder", Text="Set the Axis Rotation Order", Category="Settings", ThreadSafe)
STMIDI_PUBLIC s32 midiSetAxisRotationOrder(stEnum32 axisOrder);

STMIDI_PUBLIC_FUNCTION(Named="midiSetFileType", Text="Set File Type to Export As", Category="Settings", ThreadSafe)
STMIDI_PUBLIC s32 midiSetFileType(stEnum32 fileType);

STMIDI_PUBLIC_FUNCTION(Named="midiSetTimingMode", Text="Set Timing Mode", Category="Settings", ThreadSafe)
STMIDI_PUBLIC s32 midiSetTimingMode(stEnum32 timingMode);

STMIDI_PUBLIC_FUNCTION(Named="midiSetTimingValues", Text="Set Timing Mode Values", Category="Settings", ThreadSafe, _first="Ticks per note when TpQN or frames per second when FPS", _second="ticksPerFrame when FPS")
STMIDI_PUBLIC s32 midiSetTimingValues(s32 first, s32 second);

STMIDI_PUBLIC_FUNCTION(Named="midiConvertSceneTrackFile", Text="Convert a Scene Track File to a MIDI events file", Category="Conversion", ThreadSafe)
STMIDI_PUBLIC s32 midiConvertSceneTrackFile(stCStr src, stCStr dst);

STMIDI_PUBLIC_FUNCTION(Named="midiStepConvertSceneTrackFileBegin", Text="Convert a Scene Track File to a MIDI events file", Category="Conversion", ThreadSafe)
STMIDI_PUBLIC s32 midiStepConvertSceneTrackFileBegin(stCStr src, stCStr dst);

STMIDI_PUBLIC_FUNCTION(Named="midiStepConvertSceneTrackFileUpdate", Text="Convert a Scene Track File to a MIDI events file", Category="Conversion", ThreadSafe)
STMIDI_PUBLIC s32 midiStepConvertSceneTrackFileUpdate();

STMIDI_PUBLIC_FUNCTION(Named="midiStepConvertProgress", Text="Get Conversion Percentage", Category="Conversion", ThreadSafe)
STMIDI_PUBLIC f32 midiStepConvertProgress();

}
