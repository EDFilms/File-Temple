// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stAutoDoc.h
 *
 * Dummy Macros for the ToolAPIBuilder
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_AUTODOC_H
#define ST_AUTODOC_H

/**
 * Properties and Describers for the following ST_ Autodoc macros
 */
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

/**
 * Marked as a private function and shouldn't be exported
 */
#define ST_PRIVATE_FUNCTION(...)

/**
 * Marked as a public function and function should be exported in header file
 */
#define ST_PUBLIC_FUNCTION(...)

/**
 * Marked as a private struct and shouldn't be exported
 */
#define ST_PRIVATE_STRUCT(...)

/**
 * Marked as a struct function and struct should be exported in header file
 */
#define ST_PUBLIC_STRUCT(...)

/**
 * Marked as a public enum and enum should be exported in header file
 */
#define ST_PUBLIC_ENUM(...)

/**
 * Marked as a private enum and shouldn't be exported
 */
#define ST_PRIVATE_ENUM(...)

/**
 * Marked as a public field of a struct exported in header file
 */
#define ST_PUBLIC_FIELD(...)

/**
 * Function is marked as a TDD Test
 */
#define ST_TEST(...)

/**
 * Following text/code is marked as a developer comment
 */
#define ST_COMMENT(...)

/**
 * Following text/code is yet to be done
 */
#define ST_TODO(...)

/**
 * Following code is deprecated
 */
#define ST_DEPRECATED(...)

/**
 * Following argument or variable is unsed in this function
 */
#define ST_UNUSED(X) (void)(X)

/**
 * Following function is the initializer function for the given struct, module or process
 */
#define ST_INITIALIZER(...)

/**
 * Following function is the shutdown function for the given struct, module or process
 */
#define ST_SHUTDOWN(...)

#endif
