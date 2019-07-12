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
 * FName.h - Primitive data type declarations
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

// short hand data types
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long long u64;

typedef signed char  i8;
typedef signed short i16;
typedef signed int   i32;
typedef signed long long i64;

#define MAX_UINT8  0xff
#define MAX_UINT16 0xffff
#define MAX_UINT32 0xffffffff
#define MAX_UINT64 0xfffffffffffffff

#define MAX_INT8  0x7f
#define MAX_INT16 0x7fff
#define MAX_INT32 0x7fffffff
#define MAX_INT64 0x7fffffffffffffff

#define MIN_INT8  0x80
#define MIN_INT16 0x8000
#define MIN_INT32 0x80000000
#define MIN_INT64 0x8000000000000000

#if defined LIBUNR_64BIT
  #define MAX_SIZE MAX_UINT64
  #if defined __MINGW32__
    typedef i64 ssize_t;
  #endif
#elif defined LIBUNR_32BIT
  #define MAX_SIZE MAX_UINT32
  #if defined __MINGW32__
    typedef i32 ssize_t;
  #endif
#endif


// TODO: make type 'idx' cast automatically to FCompactIndex
typedef i32 idx;

//========================================================================
// EOF
//========================================================================
