// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * midiPublic.h
 *
 * Public C interface for the MidiExporter library
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "midiRequired.h"

extern "C"
{

/**
 * Set the swizzle for a particular axis
 * @see Axis
 * @see Node
 */
STMIDI_PUBLIC_FUNCTION(Named="midiSetAxisSwizzle", Text="Set Swizzle for a particular axis", Category="Settings", ThreadSafe)
STMIDI_PUBLIC s32 midiSetAxisSwizzle(stEnum32 nodeType, stEnum32 trsMask, stEnum32 srcAxis, stEnum32 dstAxis, s32 sign);

/**
 * Set the operation for a particular axis
 * @see Axis
 * @see Node
 * @see Operation
 */
STMIDI_PUBLIC_FUNCTION(Named="midiSetAxisOperation", Text="Set Swizzle for a particular axis", Category="Settings", ThreadSafe)
STMIDI_PUBLIC s32 midiSetAxisOperation(stEnum32 nodeType, stEnum32 trsMask, stEnum32 dstAxis, stEnum32 operatorType, f32 constantValue);

/**
 * Set the euler rotation order
 * @see AxisOrder
 */
STMIDI_PUBLIC_FUNCTION(Named="midiSetAxisRotationOrder", Text="Set the Axis Rotation Order", Category="Settings", ThreadSafe)
STMIDI_PUBLIC s32 midiSetAxisRotationOrder(stEnum32 axisOrder);

/**
 * Set the exporter file type
 * @see MidiFileType
 */
STMIDI_PUBLIC_FUNCTION(Named="midiSetFileType", Text="Set File Type to Export As", Category="Settings", ThreadSafe)
STMIDI_PUBLIC s32 midiSetFileType(stEnum32 fileType);

STMIDI_PUBLIC_FUNCTION(Named="midiSetTimingMode", Text="Set Timing Mode", Category="Settings", ThreadSafe)
STMIDI_PUBLIC s32 midiSetTimingMode(stEnum32 timingMode);

STMIDI_PUBLIC_FUNCTION(Named="midiSetTimingValues", Text="Set Timing Mode Values", Category="Settings", ThreadSafe, _first="Ticks per note when TpQN or frames per second when FPS", _second="ticksPerFrame when FPS")
STMIDI_PUBLIC s32 midiSetTimingValues(s32 first, s32 second);

/**
 * Start the export process to convert a SceneTrack file into the desired file format, in one go.
 */
STMIDI_PUBLIC_FUNCTION(Named="midiConvertSceneTrackFile", Text="Convert a Scene Track File to a MIDI events file", Category="Conversion", ThreadSafe)
STMIDI_PUBLIC s32 midiConvertSceneTrackFile(stCStr src, stCStr dst);

/**
 * Start the export process to convert a SceneTrack file into the desired file format, step by step.
 * This function is an alternate to midiConvertSceneTrackFile, to provide a externally prompted
 * system. This is useful for visual updates as a progress bar.
 * @see midiStepConvertSceneTrackFileUpdate
 * @see midiStepConvertProgress
 */
STMIDI_PUBLIC_FUNCTION(Named="midiStepConvertSceneTrackFileBegin", Text="Convert a Scene Track File to a MIDI events file", Category="Conversion", ThreadSafe)
STMIDI_PUBLIC s32 midiStepConvertSceneTrackFileBegin(stCStr src, stCStr dst);

/**
 * Used with midiStepConvertSceneTrackFileBegin convert part of the SceneTrack file, unless the process
 * has finished, then complete the file and save it.
 * @return 0 if the Exporter has completed the frame, but there is more work to do
           1 if the Exporter has completed the file
          -1 if there is an error
 */
STMIDI_PUBLIC_FUNCTION(Named="midiStepConvertSceneTrackFileUpdate", Text="Convert a Scene Track File to a MIDI events file", Category="Conversion", ThreadSafe)
STMIDI_PUBLIC s32 midiStepConvertSceneTrackFileUpdate();

/**
 * Get the progress of the export process as a decimal percentage (0.0f to 1.0f), or -1.0f if there is
 * an error, or the exporter is not exporting.
 */
STMIDI_PUBLIC_FUNCTION(Named="midiStepConvertProgress", Text="Get Conversion Percentage", Category="Conversion", ThreadSafe)
STMIDI_PUBLIC f32 midiStepConvertProgress();

}
