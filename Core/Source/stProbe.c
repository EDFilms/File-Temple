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

#ifndef ST_PROBE_H
#define ST_PROBE_H

#include "stPlatform.h"

#if ST_CFG_DATA_PROBE == 1

int stProbe_TabCount = 0;
int  stProbe_PushedTabCount = 0;
char stProbe_LFChar = '\n';

#endif
#endif
