// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * Config.inc
 *
 * Compile time configuration for SceneTrack
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

/**
 * Is the library multi-threaded?
 * @default 0
 */
#define ST_CFG_MULTI_THREADED         0

/**
 * Enable extra debugging to print out some of the state of SceneTrack
 * @default 1
 */
#define ST_CFG_DATA_PROBE             1

/**
 * Enable any internal tests
 * @default 1
 */
#define ST_CFG_INTERNAL_TESTS         1

/**
 * The delta file format is the default file format
 * @default 1
 */
#define ST_CFG_JOURNALING_IS_DEFAULT  1

/**
 * When reading from a file, take extra checks for checking for end of
 * files, or file read errors.
 * @default 1
 */
#define ST_CFG_FILE_READING_SAFE_MODE 1

/**
 * When writing to a file. Keep any data in a fixed length buffer,
 * and then write that entire buffer as a whole for performance reasons
 * @default 1
 */
#define ST_CFG_BUFFERED_WRITE         1

/**
 * Take extra steps to track all memory allocations, reallocations
 * and releases
 * @default 0
 */
#define ST_CFG_TRACK_MEMORY           0

/**
 * Return a null when memory allocated is higher than the expected
 * threshold. Used with ST_CFG_TRACK_MEMORY
 * @default 0
 */
#define ST_CFG_MALLOC_CONTROLLED_NULL 0

/**
 * Use the nedmalloc allocator
 * (deprecated)
 * @default 0
 */
#define ST_CFG_USE_NEDMALLOC          0

/** 
 * Use the expermential id map system for added performance for
 * tracking object changes between frames
 * @default 0
 */
#define ST_CFG_USE_IDMAP              0
