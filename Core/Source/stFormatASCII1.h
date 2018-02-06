
// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#if defined(ST_FWD_SERIALISER)
  { ST_FORMAT_ASCII_1, writer_MakeASCII1},
#elif defined(ST_FWD_READER)
  { ST_FORMAT_ASCII_1, reader_MakeASCII1},
#else

#ifndef SERIALISER_ASCII_1_H
#define SERIALISER_ASCII_1_H

#include "stFile.h"
stEnum32 writer_MakeASCII1(stFileWriter* writer);
stEnum32 reader_MakeASCII1(stFileReader* reader);

#endif
#endif
