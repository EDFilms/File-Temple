
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

#if defined(ST_FWD_SERIALISER)
  { ST_FORMAT_BINARY_1, writer_MakeBinary1},
#elif defined(ST_FWD_READER)
  { ST_FORMAT_ASCII_1, reader_MakeBinary1},
#else

#ifndef SERIALISER_BINARY_1_H
#define SERIALISER_BINARY_1_H

#include "stFile.h"
stEnum32 writer_MakeBinary1(stFileWriter* writer);
stEnum32 reader_MakeBinary1(stFileReader* writer);

#endif
#endif
