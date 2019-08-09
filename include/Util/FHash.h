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
 * FHash.h - FNV1 hash functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Util/FTypes.h"
#include <string.h>
#include <ctype.h>

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

