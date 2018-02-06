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

#ifndef ST_TESTS_H
#define ST_TESTS_H

#include "stPlatform.h"

#if ST_CFG_INTERNAL_TESTS == 1

#include "stBinary.h"

#define ST_REQUIRE(COND, TEXT)                    assert(COND && "[Test Failed] " TEXT "\n");
// Match in little endian order
#define ST_REQUIRE_MATCH_U8(VALUE, B0, TEXT)      assert((VALUE == B0) && "[Test Failed] " TEXT "\n");
#define ST_REQUIRE_MATCH_U16(VALUE, B0, B1, TEXT) do { assert((VALUE == B0) && "[Test Failed] " TEXT "\n"); };

#define ST_DO_TEST(NAME)

#endif
#endif
