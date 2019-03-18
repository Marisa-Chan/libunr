/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * FUtil.h - Utility functions, typedefs, and classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include <unistd.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include <libxstl/XTypes.h>
#include <libxstl/XStream.h>

#if defined LIBUNR_LINUX
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <dirent.h>
  #include <pwd.h>
  #define DIRECTORY_SEPARATOR '/'
#elif defined LIBUNR_WIN32
  #define DIRECTORY_SEPARATOR '\\'
#endif

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
#define PKG_VER_UN_V60 60
#define PKG_VER_UN_200 61
#define PKG_VER_UN_219 61 // is that right?
#define PKG_VER_UN_220 63
#define PKG_VER_UN_222 65
#define PKG_VER_UN_224 68
#define PKG_VER_UN_227 69

// Unreal Tournament 99
#define PKG_VER_UT_400 68
#define PKG_VER_UT_432 69

// Log levels
#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERR  2
#define LOG_CRIT 3

const char* const LogLevelStrings[] =
{
  "I",
  "W",
  "E",
  "!"
};

// Error codes
#define ERR_FILE_NOT_EXIST 1
#define ERR_BAD_DATA       2
#define ERR_FILE_CREATE    3

DLL_EXPORT void Logf( int Type, const char* Str, ... );

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
    };
  };

#endif

static bool operator==( FHash A, FHash B )
{
  return ( A.FnvHash[0] == B.FnvHash[0] &&
           A.FnvHash[1] == B.FnvHash[1] );
}

static bool operator!=( FHash A, FHash B )
{
  return ( A.FnvHash[0] != B.FnvHash[0] || 
           A.FnvHash[1] != B.FnvHash[1] );
}

static inline FHash FnvHash( const void* Data, size_t Len )
{
  FHash Hash;
  Hash.FnvHash[FNV1A_HASH] = Hash.FnvHash[FNV1_HASH] = FNV_BASIS;

  for (int i = 0; i < Len; i++) 
  {
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
  for (int i = 0; i < Len; i++) 
  {
    u8 ConvData = isalpha( Data[i] ) ? toupper( Data[i] ) : Data[i];
    Hash.FnvHash[FNV1A_HASH] ^= ConvData;
    Hash.FnvHash[FNV1A_HASH] *= FNV_PRIME;

    Hash.FnvHash[FNV1_HASH] *= FNV_PRIME;
    Hash.FnvHash[FNV1_HASH] ^= ConvData;
  }
  return Hash;
}
 
#define ZERO_HASH {0, 0}

//========================================================================
// EOF
//========================================================================
