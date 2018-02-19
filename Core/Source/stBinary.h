// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stBinary.h
 *
 * Endian convert functions and binary writing file functions
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_BINARY_H
#define ST_BINARY_H

#include "stPlatform.h"

/**
 * Swap the endianness of a u16
 */
ST_INLINE u16 stSwapU16(u16 val) 
{
  return (val << 8) | (val >> 8 );
}

/**
 * Swap the endianness of a s16
 */
ST_INLINE s16 stSwapS16(s16 val) 
{
  return (val << 8) | ((val >> 8) & 0xFF);
}

/**
 * Swap the endianness of a u32
 */
ST_INLINE u32 stSwapU32(u32 val)
{
  val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
  return (val << 16) | (val >> 16);
}

/**
 * Swap the endianness of a s32
 */
ST_INLINE s32 stSwapS32(s32 val)
{
  val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
  return (val << 16) | ((val >> 16) & 0xFFFF);
}

/**
 * Swap the endianness of a u64
 */
ST_INLINE u64 stSwapU64(u64 val)
{
  val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
  val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
  return (val << 32) | (val >> 32);
}

/**
 * Swap the endianness of a s64
 */
ST_INLINE s64 stSwapS64(s64 val)
{
  val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
  val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
  return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

/**
 * Union helper to convert u16 into it's native 2 bytes (to system endian)
 */
typedef union stU16Bytes
{
  stByte bytes[2];
  u16    value;
} stU16Bytes;

/**
 * Union helper to convert s16 into it's native 2 bytes (to system endian)
 */
typedef union stS16Bytes
{
  stByte bytes[2];
  s16    value;
} stS16Bytes;

/**
 * Union helper to convert u32 into it's native 4 bytes (to system endian)
 */
typedef union stU32Bytes
{
  stByte bytes[4];
  u32    value;
} stU32Bytes;

/**
 * Union helper to convert s32 into it's native 4 bytes (to system endian)
 */
typedef union stS32Bytes
{
  stByte bytes[4];
  s32    value;
} stS32Bytes;

/**
 * Union helper to convert u64 into it's native 8 bytes (to system endian)
 */
typedef union stU64Bytes
{
  stByte bytes[8];
  u64    value;
} stU64Bytes;

/**
 * Union helper to convert s64 into it's native 8 bytes (to system endian)
 */
typedef union stS64Bytes
{
  stByte bytes[8];
  s64    value;
} stS64Bytes;

/**
 * Union helper to convert f32 into it's native 4 bytes
 */
typedef union stF32Bytes
{
  stByte bytes[sizeof(f32)];
  f32    value;
} stF32Bytes;

/**
 * Union helper to convert f64 into it's native 4 bytes
 */
typedef union stF64Bytes
{
  stByte bytes[sizeof(f64)];
  f64   value;
} stF64Bytes;

#if ST_CFG_FILE_READING_SAFE_MODE == 1
/**
 * Helper macro to read NUM_BYTES from a given file or return false. Safe version.
 */
#define ST_FREAD_BYTES(F, TARGET, NUM_BYTES) if (!(feof(F) == 0 && fread(TARGET, NUM_BYTES, 1, F) == 1)) return false

/**
 * Helper macro to read NUM_BYTES from a given file or return given value. Safe version.
 */
#define ST_FREAD_BYTES_EX(F, TARGET, NUM_BYTES, FALSE_RET) if (!(feof(F) == 0 && fread(TARGET, NUM_BYTES, 1, F) == 1)) return FALSE_RET
#else
/**
 * Helper macro to read NUM_BYTES from a given file.
 */
#define ST_FREAD_BYTES(F, TARGET, NUM_BYTES) fread(TARGET, NUM_BYTES, 1, F);
/**
 * Helper macro to read NUM_BYTES from a given file.
 */
#define ST_FREAD_BYTES_EX(F, TARGET, NUM_BYTES, FALSE_RET) fread(TARGET, NUM_BYTES, 1, F);
#endif

/**
 * Try and read an u8 from a file in a given source endian and then convert into the destination endian
 */
ST_INLINE stBool stBinaryEndianFileReadU8Ex(FILE* file, u8* value, stEnum32 src, stEnum32 dst)
{
  ST_UNUSED(src);
  ST_UNUSED(dst);

  ST_FREAD_BYTES(file, value, sizeof(u8));
  return true;
}

/**
 * Try and read an s8 from a file in a given source endian and then convert into the destination endian
 */
ST_INLINE stBool stBinaryEndianFileReadS8Ex(FILE* file, s8* value, stEnum32 src, stEnum32 dst)
{
  ST_UNUSED(src);
  ST_UNUSED(dst);

  ST_FREAD_BYTES(file, value, sizeof(s8));
  return true;
}

/**
 * Convert a u16 in a given source endian and then convert into the destination endian to a byte array
 */
ST_INLINE stByte* stBinaryEndianWriteU16Ex(stByte* data, u16 value, stEnum32 src, stEnum32 dst)
{
  stU16Bytes b;

  if (src != dst)
    b.value = stSwapU16(value);
  else
    b.value = value;

  (*data++) = b.bytes[0];
  (*data++) = b.bytes[1];
  return data;
}

/**
 * Try and read an u16 from a file in a given source endian and then convert into the destination endian
 */
ST_INLINE stBool stBinaryEndianFileReadU16Ex(FILE* file, u16* value, stEnum32 src, stEnum32 dst)
{
  stU16Bytes b;

  ST_FREAD_BYTES(file, (&b.bytes[0]), sizeof(u16));

  if (src != dst)
    *value = stSwapU16(b.value);
  else
    *value = b.value;

  return true;
}

/**
 * Try and convert a u16 in a given source endian and then write into the destination endian to a file
 */
ST_INLINE void stBinaryEndianFileWriteU16Ex(FILE* file, u16 value, stEnum32 src, stEnum32 dst)
{
  stU16Bytes b;

  if (src != dst)
    b.value = stSwapU16(value);
  else
    b.value = value;

  fwrite(b.bytes, sizeof(u16), 1, file);
}

/**
 * Convert a s16 in a given source endian and then convert into the destination endian to a byte array
 */
ST_INLINE stByte* stBinaryEndianWriteS16Ex(stByte* data, s16 value, stEnum32 src, stEnum32 dst)
{
  stS16Bytes b;

  if (src != dst)
    b.value = stSwapS16(value);
  else
    b.value = value;

  (*data++) = b.bytes[0];
  (*data++) = b.bytes[1];
  return data;
}

/**
 * Try and convert a s16 in a given source endian and then write into the destination endian to a file
 */
ST_INLINE void stBinaryEndianFileWriteS16Ex(FILE* file, s16 value, stEnum32 src, stEnum32 dst)
{
  stU16Bytes b;

  if (src != dst)
    b.value = stSwapS16(value);
  else
    b.value = value;

  fwrite(b.bytes, sizeof(s16), 1, file);
}

/**
 * Try and read an s16 from a file in a given source endian and then convert into the destination endian
 */
ST_INLINE stBool stBinaryEndianFileReadS16Ex(FILE* file, s16* value, stEnum32 src, stEnum32 dst)
{
  stS16Bytes b;

  ST_FREAD_BYTES(file, (&b.bytes[0]), sizeof(s16));

  if (src != dst)
    *value = stSwapS16(b.value);
  else
    *value = b.value;

  return true;
}

/**
 * Try and convert a u32 in a given source endian and then write into the destination endian to a byte array
 */
ST_INLINE stByte* stBinaryEndianWriteU32Ex(stByte* data, u32 value, stEnum32 src, stEnum32 dst)
{
  stU32Bytes b;

  if (src != dst)
    b.value = stSwapU32(value);
  else
    b.value = value;

  (*data++) = b.bytes[0];
  (*data++) = b.bytes[1];
  (*data++) = b.bytes[2];
  (*data++) = b.bytes[3];
  return data;
}

/**
 * Try and convert a u32 in a given source endian and then write into the destination endian to a file
 */
ST_INLINE void stBinaryEndianFileWriteU32Ex(FILE* file, u32 value, stEnum32 src, stEnum32 dst)
{
  stU32Bytes b;

  if (src != dst)
    b.value = stSwapU32(value);
  else
    b.value = value;

  fwrite(b.bytes, sizeof(u32), 1, file);
}

/**
 * Try and convert a s32 in a given source endian and then write into the destination endian to a file
 */
ST_INLINE stByte* stBinaryEndianWriteS32Ex(stByte* data, s32 value, stEnum32 src, stEnum32 dst)
{
  stS32Bytes b;

  if (src != dst)
    b.value = stSwapS32(value);
  else
    b.value = value;

  (*data++) = b.bytes[0];
  (*data++) = b.bytes[1];
  (*data++) = b.bytes[2];
  (*data++) = b.bytes[3];
  return data;
}

/**
 * Try and read an u32 from a file in a given source endian and then convert into the destination endian
 */
ST_INLINE stBool stBinaryEndianFileReadU32Ex(FILE* file, u32* value, stEnum32 src, stEnum32 dst)
{
  stU32Bytes b;

  ST_FREAD_BYTES(file, (&b.bytes[0]), sizeof(u32));

  if (src != dst)
    *value = stSwapU32(b.value);
  else
    *value = b.value;

  return true;
}

/**
 * Try and convert a s32 in a given source endian and then write into the destination endian to a file
 */
ST_INLINE void stBinaryEndianFileWriteS32Ex(FILE* file, s32 value, stEnum32 src, stEnum32 dst)
{
  stS32Bytes b;

  if (src != dst)
    b.value = stSwapS32(value);
  else
    b.value = value;

  fwrite(b.bytes, sizeof(s32), 1, file);
}

/**
 * Try and read an s32 from a file in a given source endian and then convert into the destination endian
 */
ST_INLINE stBool stBinaryEndianFileReadS32Ex(FILE* file, s32* value, stEnum32 src, stEnum32 dst)
{
  stS32Bytes b;

  ST_FREAD_BYTES(file, (&b.bytes[0]), sizeof(s32));

  if (src != dst)
    *value = stSwapS32(b.value);
  else
    *value = b.value;

  return true;
}

/**
 * Try and convert a u64 in a given source endian and then write into the destination endian to a byte array
 */
ST_INLINE stByte* stBinaryEndianWriteU64Ex(stByte* data, u64 value, stEnum32 src, stEnum32 dst)
{
  stU64Bytes b;

  if (src != dst)
    b.value = stSwapU64(value);
  else
    b.value = value;

  (*data++) = b.bytes[0];
  (*data++) = b.bytes[1];
  (*data++) = b.bytes[2];
  (*data++) = b.bytes[3];
  (*data++) = b.bytes[4];
  (*data++) = b.bytes[5];
  (*data++) = b.bytes[6];
  (*data++) = b.bytes[7];
  return data;
}

/**
 * Try and convert a u64 in a given source endian and then write into the destination endian to a file
 */
ST_INLINE void stBinaryEndianFileWriteU64Ex(FILE* file, u64 value, stEnum32 src, stEnum32 dst)
{
  stU64Bytes b;

  if (src != dst)
    b.value = stSwapU64(value);
  else
    b.value = value;

  fwrite(b.bytes, sizeof(u64), 1, file);
}

/**
 * Try and read u64 from a file in a given source endian and then convert into the destination endian
 */
ST_INLINE stBool stBinaryEndianFileReadU64Ex(FILE* file, u64* value, stEnum32 src, stEnum32 dst)
{
  stU64Bytes b;

  ST_FREAD_BYTES(file, (&b.bytes[0]), sizeof(u64));

  if (src != dst)
    *value = stSwapU64(b.value);
  else
    *value = b.value;

  return true;
}

/**
 * Try and convert a s64 in a given source endian and then write into the destination endian to a byte array
 */
ST_INLINE stByte* stBinaryEndianWriteS64Ex(stByte* data, s64 value, stEnum32 src, stEnum32 dst)
{
  stS64Bytes b;

  if (src != dst)
    b.value = stSwapU64(value);
  else
    b.value = value;

  (*data++) = b.bytes[0];
  (*data++) = b.bytes[1];
  (*data++) = b.bytes[2];
  (*data++) = b.bytes[3];
  (*data++) = b.bytes[4];
  (*data++) = b.bytes[5];
  (*data++) = b.bytes[6];
  (*data++) = b.bytes[7];
  return data;
}

/**
 * Try and convert a s64 in a given source endian and then write into the destination endian to a file
 */
ST_INLINE void stBinaryEndianFileWriteS64Ex(FILE* file, s64 value, stEnum32 src, stEnum32 dst)
{
  stS64Bytes b;

  if (src != dst)
    b.value = stSwapS64(value);
  else
    b.value = value;

  fwrite(b.bytes, sizeof(s64), 1, file);
}

/**
 * Try and read s64 from a file in a given source endian and then convert into the destination endian
 */
ST_INLINE stBool stBinaryEndianFileReadS64Ex(FILE* file, s64* value, stEnum32 src, stEnum32 dst)
{
  stS64Bytes b;

  ST_FREAD_BYTES(file, (&b.bytes[0]), sizeof(s64));

  if (src != dst)
    *value = stSwapS64(b.value);
  else
    *value = b.value;

  return true;
}

/**
 * Write a u8 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryWriteU8(stByte* data, u8 value)
{
  (*data++) = value;
  return data;
}

/**
 * Write a s8 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryWriteS8(stByte* data, s8 value)
{
  (*data++) = value;
  return data;
}

/**
 * Write a u16 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryEndianWriteU16(stByte* data, u16 value, stEnum32 dst)
{
  return stBinaryEndianWriteU16Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Write a s16 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryEndianWriteS16(stByte* data, s16 value, stEnum32 dst)
{
  return stBinaryEndianWriteS16Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Write a u32 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryEndianWriteU32(stByte* data, u32 value, stEnum32 dst)
{
  return stBinaryEndianWriteU32Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Write a s32 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryEndianWriteS32(stByte* data, s32 value, stEnum32 dst)
{
  return stBinaryEndianWriteS32Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Write a u64 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryEndianWriteU64(stByte* data, u64 value, stEnum32 dst)
{
  return stBinaryEndianWriteU64Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Write a s64 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryEndianWriteS64(stByte* data, s64 value, stEnum32 dst)
{
  return stBinaryEndianWriteS64Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Write a u16 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryWriteU16(stByte* data, u16 value)
{
  return stBinaryEndianWriteU16Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Write a s16 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryWriteS16(stByte* data, s16 value)
{
  return stBinaryEndianWriteS16Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Write a u32 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryWriteU32(stByte* data, u32 value)
{
  return stBinaryEndianWriteU32Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Write a s32 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryWriteS32(stByte* data, s32 value)
{
  return stBinaryEndianWriteS32Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Write a u64 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryWriteU64(stByte* data, u64 value)
{
  return stBinaryEndianWriteU64Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Write a s64 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryWriteS64(stByte* data, s64 value)
{
  return stBinaryEndianWriteS64Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Write a f32 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryWriteF32(stByte* data, f32 value)
{
  memcpy(data, &value, sizeof(f32));
  return data + sizeof(f32);
}

/**
 * Write a f64 to a byte array
 * @return The array pointer after the value written
 */
ST_INLINE stByte* stBinaryWriteF64(stByte* data, f64 value)
{
  memcpy(data, &value, sizeof(f64));
  return data + sizeof(f64);
}




/**
 * Read a u16 from a byte array in the given endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadU16Ex(stByte* data, u16* value, stEnum32 src, stEnum32 dst)
{
  stU16Bytes b;

  b.bytes[0] = (*data++);
  b.bytes[1] = (*data++);

  if (src != dst)
    b.value = stSwapU16(b.value);

  *value = b.value;

  return data;
}

/**
 * Read a s16 from a byte array in the given endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadS16Ex(stByte* data, s16* value, stEnum32 src, stEnum32 dst)
{
  stS16Bytes b;

  b.bytes[0] = (*data++);
  b.bytes[1] = (*data++);

  if (src != dst)
    b.value = stSwapS16(b.value);

  *value = b.value;

  return data;
}

/**
 * Read a u32 from a byte array in the given endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadU32Ex(stByte* data, u32* value, stEnum32 src, stEnum32 dst)
{
  stU32Bytes b;

  b.bytes[0] = (*data++);
  b.bytes[1] = (*data++);
  b.bytes[2] = (*data++);
  b.bytes[3] = (*data++);

  if (src != dst)
    b.value = stSwapU32(b.value);

  *value = b.value;

  return data;
}

/**
 * Read a s32 from a byte array in the given endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadS32Ex(stByte* data, s32* value, stEnum32 src, stEnum32 dst)
{
  stS32Bytes b;

  b.bytes[0] = (*data++);
  b.bytes[1] = (*data++);
  b.bytes[2] = (*data++);
  b.bytes[3] = (*data++);

  if (src != dst)
    b.value = stSwapS32(b.value);

  *value = b.value;

  return data;
}

/**
 * Read a u64 from a byte array in the given endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadU64Ex(stByte* data, u64* value, stEnum32 src, stEnum32 dst)
{
  stU64Bytes b;

  b.bytes[0] = (*data++);
  b.bytes[1] = (*data++);
  b.bytes[2] = (*data++);
  b.bytes[3] = (*data++);
  b.bytes[4] = (*data++);
  b.bytes[5] = (*data++);
  b.bytes[6] = (*data++);
  b.bytes[7] = (*data++);

  if (src != dst)
    b.value = stSwapU64(b.value);

  *value = b.value;

  return data;
}

/**
 * Read a s64 from a byte array in the given endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadS64Ex(stByte* data, s64* value, stEnum32 src, stEnum32 dst)
{
  stS64Bytes b;

  b.bytes[0] = (*data++);
  b.bytes[1] = (*data++);
  b.bytes[2] = (*data++);
  b.bytes[3] = (*data++);
  b.bytes[4] = (*data++);
  b.bytes[5] = (*data++);
  b.bytes[6] = (*data++);
  b.bytes[7] = (*data++);

  if (src != dst)
    b.value = stSwapU64(b.value);
  
  *value = b.value;

  return data;
}

/**
 * Read a u8 from a byte array
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryReadU8(stByte* data, u8* value)
{
  *value = (*data++);
  return data;
}

/**
 * Read a s8 from a byte array
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryReadS8(stByte* data, s8* value)
{
  *value = (*data++);
  return data;
}

/**
 * Read a u16 from a byte array from system endian to a destination endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadU16(stByte* data, u16* value, stEnum32 dst)
{
  return stBinaryEndianReadU16Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Read a s16 from a byte array from system endian to a destination endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadS16(stByte* data, s16* value, stEnum32 dst)
{
  return stBinaryEndianReadS16Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Read a u32 from a byte array from system endian to a destination endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadU32(stByte* data, u32* value, stEnum32 dst)
{
  return stBinaryEndianReadU32Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Read a s32 from a byte array from system endian to a destination endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadS32(stByte* data, s32* value, stEnum32 dst)
{
  return stBinaryEndianReadS32Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Read a u64 from a byte array from system endian to a destination endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadU64(stByte* data, u64* value, stEnum32 dst)
{
  return stBinaryEndianReadU64Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Read a s64 from a byte array from system endian to a destination endian
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryEndianReadS64(stByte* data, s64* value, stEnum32 dst)
{
  return stBinaryEndianReadS64Ex(data, value, ST_ENDIAN, dst);
}

/**
 * Read a u16 from a byte array as-is.
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryReadU16(stByte* data, u16* value)
{
  return stBinaryEndianReadU16Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Read a s16 from a byte array as-is.
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryReadS16(stByte* data, s16* value)
{
  return stBinaryEndianReadS16Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Read a u32 from a byte array as-is.
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryReadU32(stByte* data, u32* value)
{
  return stBinaryEndianReadU32Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Read a s32 from a byte array as-is.
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryReadS32(stByte* data, s32* value)
{
  return stBinaryEndianReadS32Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Read a u64 from a byte array as-is.
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryReadU64(stByte* data, u64* value)
{
  return stBinaryEndianReadU64Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Read a s64 from a byte array as-is.
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryReadS64(stByte* data, s64* value)
{
  return stBinaryEndianReadS64Ex(data, value, ST_ENDIAN, ST_ENDIAN);
}

/**
 * Read a f32 from a byte array as-is.
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryReadF32(stByte* data, f32* value)
{
  stF32Bytes b;

  b.bytes[0] = (*data++);
  b.bytes[1] = (*data++);
  b.bytes[2] = (*data++);
  b.bytes[3] = (*data++);

  *value = b.value;

  return data;
}

/**
 * Read a f64 from a byte array as-is.
 * @return The array pointer after the value was read
 */
ST_INLINE stByte* stBinaryReadF64(stByte* data, f64* value)
{
  stF64Bytes b;

  b.bytes[0] = (*data++);
  b.bytes[1] = (*data++);
  b.bytes[2] = (*data++);
  b.bytes[3] = (*data++);
  b.bytes[4] = (*data++);
  b.bytes[5] = (*data++);
  b.bytes[6] = (*data++);
  b.bytes[7] = (*data++);

  *value = b.value;

  return data;
}

/**
 * Read a f32 from a file as-is.
 * @return If the value was read or not
 */
ST_INLINE stBool stBinaryReadF32Ex(FILE* file, f32* value, stEnum32 src, stEnum32 dst)
{
  ST_UNUSED(src);
  ST_UNUSED(dst);
  ST_FREAD_BYTES(file, value, sizeof(f32));
  return true;
}

/**
 * Read a f64 from a file as-is.
 * @return If the value was read or not
 */
ST_INLINE stBool stBinaryReadF64Ex(FILE* file, f64* value, stEnum32 src, stEnum32 dst)
{
  ST_UNUSED(src);
  ST_UNUSED(dst);
  ST_FREAD_BYTES(file, value, sizeof(f64));
  return true;
}

/**
 * Read a specificed number of bytes from a file as-is.
 * @return If the value was read or not
 */
ST_INLINE stBool stBinaryReadDataEx(FILE* file, u8* value, u32 size)
{
  ST_FREAD_BYTES(file, value, size);
  return true;
}

/**
 * Compare u16 against two bytes with respect to endianess
 * @return 0 if the bytes are identicial, or 1,2 of where it isn't.
 */
ST_INLINE int stU16_CompareBytes(u16 a, stByte b[2], stEnum32 bEndian)
{
  if (!(bEndian != ST_ENDIAN))
    a = stSwapU16(a);

  stU16Bytes t;
  t.value = a;

  for(int i=0;i < 2;i++)
  {
    if (t.bytes[i] != b[i])
      return i + 1;
  }
  return 0;
}

/**
 * Compare s16 against two bytes with respect to endianess
 * @return 0 if the bytes are identicial, or 1,2 of where it isn't.
 */
ST_INLINE int stS16_CompareBytes(s16 a, stByte b[2], stEnum32 bEndian)
{
  if (!(bEndian != ST_ENDIAN))
    a = stSwapS16(a);

  stS16Bytes t;
  t.value = a;

  for(int i=0;i < 2;i++)
  {
    if (t.bytes[i] != b[i])
      return i + 1;
  }
  return 0;
}

/**
 * Compare u32 against two bytes with respect to endianess
 * @return 0 if the bytes are identicial, or 1,2,3,4 of where it isn't.
 */
ST_INLINE int stU32_CompareBytes(u32 a, stByte b[4], stEnum32 bEndian)
{
  if (!(bEndian != ST_ENDIAN))
    a = stSwapU32(a);

  stU32Bytes t;
  t.value = a;

  for(int i=0;i < 4;i++)
  {
    if (t.bytes[i] != b[i])
      return i + 1;
  }
  return 0;
}

/**
 * Compare s32 against two bytes with respect to endianess
 * @return 0 if the bytes are identicial, or 1,2,3,4 of where it isn't.
 */
ST_INLINE int stS32_CompareBytes(s32 a, stByte b[8], stEnum32 bEndian)
{
  if (!(bEndian != ST_ENDIAN))
    a = stSwapS32(a);

  stS32Bytes t;
  t.value = a;

  for(int i=0;i < 4;i++)
  {
    if (t.bytes[i] != b[i])
      return i + 1;
  }
  return 0;
}

/**
 * Compare u64 against two bytes with respect to endianess
 * @return 0 if the bytes are identicial, or 1,2,3,4,5,6,7,8 of where it isn't.
 */
ST_INLINE int stU64_CompareBytes(u64 a, stByte b[8], stEnum32 bEndian)
{
  if (!(bEndian != ST_ENDIAN))
    a = stSwapU64(a);

  stU64Bytes t;
  t.value = a;

  for(int i=0;i < 8;i++)
  {
    if (t.bytes[i] != b[i])
      return i + 1;
  }
  return 0;
}

/**
 * Compare s64 against two bytes with respect to endianess
 * @return 0 if the bytes are identicial, or 1,2,3,4,5,6,7,8 of where it isn't.
 */
ST_INLINE int stS64_CompareBytes(s64 a, stByte b[8], stEnum32 bEndian)
{
  if (!(bEndian != ST_ENDIAN))
    a = stSwapS64(a);

  stS64Bytes t;
  t.value = a;

  for(int i=0;i < 8;i++)
  {
    if (t.bytes[i] != b[i])
      return i + 1;
  }
  return 0;
}
#endif
