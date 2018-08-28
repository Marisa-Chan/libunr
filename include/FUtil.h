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

#pragma once

// #include <istream>
// #include <stddef.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "Types.h"
#include "Stream.h"
  
// TODO: architecture defines for non-x86 platforms
#if defined __GNUG__
  
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

// TODO: make type 'idx' cast automatically to FCompactIndex
typedef i32 idx;

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

#define LOG_INFO "Info"
#define LOG_WARN "Warning"
#define LOG_ERR  "Error"
#define LOG_CRIT "Critical"

typedef void(*DebugPrintFunc)( const char* Str, size_t Len );
extern DebugPrintFunc DebugPrint;

void Logf( const char* Type, const char* Str, ... );

#define FNV1A_HASH 0
#define FNV1_HASH  1

#if defined LIBUNR_64BIT
  #define FNV_PRIME 1099511628211ULL
  #define FNV_BASIS 14695981039346656037ULL

  struct FHash
  {
    union
    {
      u64  FnvHash[2]; 
      u128 FullHash;
    };
  };

#elif defined LIBUNR_32BIT
  #define FNV_PRIME 16777619
  #define FNV_BASIS 2166136261

  struct FHash
  {
    union
    {
      u32 FnvHash[2];
      u64 FullHash;
    };
  };

#endif

static bool operator==( FHash A, FHash B )
{
  return ( A.FullHash == B.FullHash );
}

static bool operator!=( FHash A, FHash B )
{
  return ( A.FullHash != B.FullHash );
}

static inline FHash FnvHash( const void* Data, size_t Len )
{
  FHash Hash;
  Hash.FnvHash[FNV1A_HASH] = Hash.FnvHash[FNV1_HASH] = FNV_BASIS;

  for (int i = 0; i < Len; i++) {
    Hash.FnvHash[FNV1A_HASH] ^= ((u8*)Data)[i];
    Hash.FnvHash[FNV1A_HASH] *= FNV_PRIME;

    Hash.FnvHash[FNV1_HASH] *= FNV_PRIME;
    Hash.FnvHash[FNV1_HASH] ^= ((u8*)Data)[i];
  }
  return Hash;
}

static inline FHash FnvHashString( const char* Data )
{
  FHash Hash;
  Hash.FnvHash[FNV1A_HASH] = Hash.FnvHash[FNV1_HASH] = FNV_BASIS;

  int Len = strlen( Data );
  for (int i = 0; i < Len; i++) {
    Hash.FnvHash[FNV1A_HASH] ^= ((u8*)Data)[i];
    Hash.FnvHash[FNV1A_HASH] *= FNV_PRIME;

    Hash.FnvHash[FNV1_HASH] *= FNV_PRIME;
    Hash.FnvHash[FNV1_HASH] ^= ((u8*)Data)[i];
  }
  return Hash;
}
 
#define ZERO_HASH {0, 0}

//========================================================================
// EOF
//========================================================================
