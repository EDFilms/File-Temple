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

#ifndef ST_AUTODOC_H
#define ST_AUTODOC_H

enum
{
  Public,
  Private,
  Named,
  Text,
  For,
  Value,
  MaybeNull,
  ThreadSafe,
  NotThreadSafe,
  MaybeThreadSafe
};

#define ST_PRIVATE_FUNCTION(...)
#define ST_PUBLIC_FUNCTION(...)
#define ST_PRIVATE_STRUCT(...)
#define ST_PUBLIC_STRUCT(...)
#define ST_PUBLIC_ENUM(...)
#define ST_PRIVATE_ENUM(...)
#define ST_PUBLIC_FIELD(...)
#define ST_TEST(...)
#define ST_COMMENT(...)
#define ST_TODO(...)
#define ST_DEPRECATED(...)
#define ST_UNUSED(X) (void)(X)
#define ST_INITIALIZER(...)
#define ST_SHUTDOWN(...)

#endif
