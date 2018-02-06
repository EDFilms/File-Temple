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

#ifndef ST_STRIDE_H
#define ST_STRIDE_H

ST_INLINE u8* stStrideMemCpy(u8* dst, u8* src, u32 dstStride, u32 srcStride, u32 count)
{
  if (dstStride != srcStride)
  {
    u8 *d = dst, *s = src;
    size_t stride = stMin(srcStride, dstStride);
    //ST_PRINT_CHATTERF("[stStrideMemCpy] Strided Mem Copy. Dst=%p, Src=%p, dstStride=%i, srcStride=%i, stride=%i count=%i", dst, src, dstStride, srcStride, (u32) stride, count);
    while(count--)
    {
      memcpy(d, s, stride);
      d += dstStride;
      s += srcStride;
    }
    return d;
  }
  else
  {
    //ST_PRINT_CHATTERF("[stStrideMemCpy] Whole Mem Copy. Dst=%p, Src=%p, dstStride=%i, srcStride=%i, count=%i", dst, src, dstStride, srcStride, count);
    memcpy(dst, src, count * srcStride);
    return dst + (count * srcStride);
  }
}

// Stride/vector version of memory copy. With < 4 elements as unrolled loops.

#define ST_STRIDE_MEMCOPY(T, SRC, DST, NB_ELEMENTS, ARRAY_SIZE, STRIDE) \
    do { \
      T* _dst = DST; T* _src = SRC; \
      size_t tStride = STRIDE / sizeof(T);\
      printf("Stride = %i, TStride=%i", STRIDE, tStride);\
      switch(NB_ELEMENTS)\
      {\
        case 0: break;\
        case 1:\
        {\
          memcpy(DST, SRC, ARRAY_SIZE);\
        }\
        break;\
        case 2:\
        {\
          for(u32 _i=0;_i < ARRAY_SIZE;_i++)\
          {\
            *_dst++ = _src[0];\
            *_dst++ = _src[1];\
            _src += tStride;\
          }\
        }\
        break;\
        case 3:\
        {\
          for(u32 _i=0;_i < ARRAY_SIZE;_i++)\
          {\
            *_dst++ = _src[0];\
            *_dst++ = _src[1];\
            *_dst++ = _src[2];\
            _src += tStride;\
          }\
        }\
        break;\
        case 4:\
        {\
          for(u32 _i=0;_i < ARRAY_SIZE;_i++)\
          {\
            *_dst++ = _src[0];\
            *_dst++ = _src[1];\
            *_dst++ = _src[2];\
            *_dst++ = _src[3];\
            _src += tStride;\
          }\
        }\
        break;\
        default:\
        {\
          for(u32 _i=0;_i < ARRAY_SIZE;_i++)\
          {\
            for(u8 j=0; j < NB_ELEMENTS;j++)\
            {\
              *_dst++ = _src[j];\
            }\
            _src += tStride;\
          }\
        }\
        break;\
      }\
    }\
    while(0)

#endif
