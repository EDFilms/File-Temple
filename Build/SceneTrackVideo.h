#ifndef SCENETRACKVIDEO_H
#define SCENETRACKVIDEO_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
  extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
  #define STVIDEO_PUBLIC  __declspec(dllimport)
#else
  #define STVIDEO_PUBLIC  extern
#endif

#define STVIDEO_FILETYPE_FILETYPE_JPEG_SEQUENCE (0)
#define STVIDEO_FILETYPE_FILETYPE_PNG_SEQUENCE (1)
#define STVIDEO_FILETYPE_FILETYPE_MJPEG_SEQUENCE (2)
#define STVIDEO_FILETYPE_FILETYPE_MPEG1_SEQUENCE (3)
#define STVIDEO_FILETYPE_FILETYPE_RAW_SEQUENCE (4)

#define STVIDEO_FLIP_NONE (0)
#define STVIDEO_FLIP_VERTICAL (1)
#define STVIDEO_FLIP_HORIZONTAL (2)
#define STVIDEO_FLIP_BOTH (3)


/*
 * Function: videoSetFileType
 * --------------------------
 *  
 *  Set File Version to Export As
 *  
 *  Parameters:
 *   1. fileType
 */
STVIDEO_PUBLIC int32_t videoSetFileType(int fileType);

/*
 * Function: videoSetImageFlip
 * ---------------------------
 *  
 *  Set any Image flip correction
 *  
 *  Parameters:
 *   1. imageFlip
 */
STVIDEO_PUBLIC int32_t videoSetImageFlip(int imageFlip);

/*
 * Function: videoConvertSceneTrackFile
 * ------------------------------------
 *  
 *  Convert a Scene Track File to a video recording file
 *  
 *  Parameters:
 *   1. src
 *   2. dst
 */
STVIDEO_PUBLIC int32_t videoConvertSceneTrackFile(const char* src, const char* dst);

/*
 * Function: videoStepConvertSceneTrackFileBegin
 * ---------------------------------------------
 *  
 *  Convert a Scene Track File to a video recording file
 *  
 *  Parameters:
 *   1. src
 *   2. dst
 */
STVIDEO_PUBLIC int32_t videoStepConvertSceneTrackFileBegin(const char* src, const char* dst);

/*
 * Function: videoStepConvertSceneTrackFileUpdate
 * ----------------------------------------------
 *  
 *  Convert a Scene Track File to a video recording file
 *  
 */
STVIDEO_PUBLIC int32_t videoStepConvertSceneTrackFileUpdate();

/*
 * Function: videoStepConvertProgress
 * ----------------------------------
 *  
 *  Get Conversion Percentage
 *  
 */
STVIDEO_PUBLIC float videoStepConvertProgress();

/*
 * Function: videoGetVersionStrCapacity
 * ------------------------------------
 *  
 *  Get Library Version String Capacity
 *  
 */
STVIDEO_PUBLIC int32_t videoGetVersionStrCapacity();

/*
 * Function: videoGetVersion
 * -------------------------
 *  
 *  Get Library Version
 *  
 *  Parameters:
 *   1. outStr
 *   2. strCapacity
 */
STVIDEO_PUBLIC int32_t videoGetVersion(char* outStr, int32_t strCapacity);
#ifdef __cplusplus
}
#endif

#endif
