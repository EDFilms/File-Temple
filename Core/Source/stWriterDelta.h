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

#ifndef ST_DELTA_SERIALIZER_H
#define ST_DELTA_SERIALIZER_H

#include "stPlatform.h"
#include "stWriter.h"

void stWriterDeltaUpdate(stWriteContextData* context, stBool keepFrames);
void stWriterDeltaClose(stWriteContextData* context, stBool keepFrames);

#endif
