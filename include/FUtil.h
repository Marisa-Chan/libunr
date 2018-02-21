/*========================================================================*\
|*  This file is part of libunr.                                          *|
|*                                                                        *|
|*  libunr is free software: you can redistribute it and/or modify        *|
|*  it under the terms of the GNU General Public License as published by  *|
|*  the Free Software Foundation, either version 3 of the License, or     *|
|*  (at your option) any later version.                                   *|
|*                                                                        *|
|*  libunr is distributed in the hope that it will be useful,             *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *|
|*  GNU General Public License for more details.                          *|
|*                                                                        *|
|*  You should have received a copy of the GNU General Public License     *|
|*  along with libunr.  If not, see <http://www.gnu.org/licenses/>.       *|
|*                                                                        *|
\*========================================================================*/

/*========================================================================
 * FUtil.h - Utility functions, typedefs, and classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __FUTIL_H__
#define __FUTIL_H__

#define _FILE_OFFSET_BITS 64

#include <istream>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FORCEINLINE __attribute__((always_inline))
#define BLOCK_COPY_CTOR(cls) \
private: \
  cls (const cls & copy); \
  
// TODO: architecture defines for non-x86 platforms
#if defined __GNUG__

  #define LIKELY(condition) __builtin_expect(!!(condition), 1)
  #define UNLIKELY(condition) __builtin_expect(!!(condition), 0)
  #define DLL_EXPORT __attribute__((dllexport))
  
  #if defined __x86_64__
    #define LIBUNR_64BIT
  #elif defined __i386__
    #define LIBUNR_32BIT
  #else
    #error "Currently unsupported architecture"
  #endif
  
#else
  #error "Unsupported compiler type, read more here"
  
  // At this point in time, only the g++ compiler is supported
  // Clang may be supported in the future. Compilers for less
  // common architectures should also be supported where gcc does
  // not have support or if gcc tends to be slower on said arch. 
  // No version of Visual Studio will be officially supported.
  
#endif

#define LIBUNR_USE_STL

// standard memcpy and memmove are extremely unsafe! do not taint source with it's usage
#define memcpy DO_NOT_USE_STANDARD_MEMCPY
#define memmove DO_NOT_USE_STANDARD_MEMMOVE

// Package version defs
// Unreal Beta
#define PKG_VER_UB_OLD  21
#define PKG_VER_UB_0864 22
#define PKG_VER_UB_0867 23
#define PKG_VER_UB_098  42
#define PKG_VER_UB_099  42

// Unreal
#define PKG_VER_UN_200 61
#define PKG_VER_UN_219 61 // is that right?
#define PKG_VER_UN_220 63
#define PKG_VER_UN_222 65
#define PKG_VER_UN_224 68
#define PKG_VER_UN_227 69

// Unreal Tournament 99
#define PKG_VER_UT_400 68
#define PKG_VER_UT_432 69

/* short hand data types */
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long long u64;

typedef signed char  i8;
typedef signed short i16;
typedef signed int   i32;
typedef signed long long i64;

typedef i32 idx; // signifies that an int type should be read from an FArchive as a compact index

#define MAX_UINT8  0xff
#define MAX_UINT16 0xffff
#define MAX_UINT32 0xffffffff
#define MAX_UINT64 0xffffffffffffffff

#define MAX_INT8  0x7f
#define MAX_INT16 0x7fff
#define MAX_INT32 0x7fffffff
#define MAX_INT64 0x7fffffffffffffff

#define MIN_INT8  0x80
#define MIN_INT16 0x8000
#define MIN_INT32 0x80000000
#define MIN_INT64 0x8000000000000000

#ifdef LIBUNR_64BIT
  #define MAX_SIZE MAX_UINT64
#else
  #define MAX_SIZE MAX_UINT32
#endif

#define PtrAdd(ptr1, ptr2) \
  (void*)((u8*)ptr1 + (u8*)ptr2)
  
#define PtrSubtract(ptr1, ptr2) \
  (void*)((u8*)ptr1 - (u8*)ptr2)
  
int  ReadCompactIndex  (std::istream& In);
void WriteCompactIndex (std::ostream& Out, int i);
void StringReplaceChar (std::string& Str, char Old, char New);

#endif
//========================================================================
// EOF
//========================================================================
