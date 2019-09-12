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

#if defined __linux__ || defined __unix__
  #define LIBUNR_POSIX
  #define DIRECTORY_SEPARATOR '/'
#elif defined _WIN32
  #define LIBUNR_WIN32
  #define DIRECTORY_SEPARATOR '\\'
#endif

// TODO: architecture defines for non-x86 platforms
#if defined __GNUG__
 
  #define FORCEINLINE __attribute__((always_inline))
  #define LIKELY(condition) __builtin_expect(!!(condition), 1)
  #define UNLIKELY(condition) __builtin_expect(!!(condition), 0)
  
  #if defined __x86_64__
    #define LIBUNR_64BIT
  #elif defined __i386__
    #define LIBUNR_32BIT
  #else
    #error "Currently unsupported architecture"
  #endif
 
  // MinGW
  #if defined __MINGW32__ && !defined __MINGW64__
    #define DLL_EXPORT __declspec(dllexport)
	#elif defined __i386__
		#define ftello ftello64
		#define fseeko fseeko64
  #else
    #define DLL_EXPORT
  #endif
 
#elif defined _MSC_VER
	
  #pragma warning(disable:4251) // Non dll-interface class 'type' used as based for dll-interface class 'type2'
  #pragma warning(disable:4275) // Class 'type' needs to have dll-interface to be used by clients of class 'type2'

  #define DLL_EXPORT __declspec(dllexport)
  #define FORCEINLINE __forceinline

  // Likely/unlikely don't exist on MSVC
  #define LIKELY(condition) (condition)
  #define UNLIKELY(condition) (condition)

  #if defined _WIN64
    #define LIBUNR_64BIT
    #define fseeko _fseeki64
    #define ftello _ftelli64
  #elif defined _WIN32
    #define LIBUNR_32BIT
  #else
    #error "Currently unsupported architecture"
  #endif

  #define S_ISDIR(mode) (mode & _S_IFDIR)

#else
  #error "Unsupported compiler type, read more here"
  
  // At this point in time, only the g++ and Clang compilers
  // are supported. Compilers for less common architectures 
  // should also be supported where cross-platform compilers do
  // not have support or if they tend to generate slower code for
  // said platform.
  
#endif

// pointer arithmetic helpers
#include <stdint.h>
#define PtrAdd(ptr1, ptr2) \
  (void*)((uintptr_t)ptr1 + (uintptr_t)ptr2)
  
#define PtrSubtract(ptr1, ptr2) \
  (void*)((uintptr_t)ptr1 - (uintptr_t)ptr2)

#define PtrDistance(ptr1, ptr2) \
  ((uintptr_t)ptr1 - (uintptr_t)ptr2)

#define MIN(a, b) \
  ((a < b) ? a : b)

#define MAX(a, b) \
  ((a > b) ? a : b)

#define OFFSET_OF(cls, member) \
 ((uintptr_t)&(((cls*)0)->member))

#define BLOCK_COPY_CTOR(cls) \
private: \
  cls (const cls & copy); \

// Timers
#include <time.h>
#if defined __linux__
#define TIMER_DECLARE(name) \
  timespec name##_start; \
  timespec name##_end; \

#define TIMER_START(name) \
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &name##_start)

#define TIMER_END(name) \
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &name##_end)

#define TIMER_PRINT(name) \
  printf("Time for %s: %ld.%.9ld\n", #name, \
    name##_end.tv_sec - name##_start.tv_sec, \
    name##_end.tv_nsec - name##_start.tv_nsec) \

#endif

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

// Error codes
#define ERR_FILE_NOT_EXIST 1
#define ERR_BAD_DATA       2
#define ERR_FILE_CREATE    3

static inline int CalcObjRefValue( int ObjRef )
{
  if ( ObjRef == 0 )
    return ObjRef;
  
  else if ( ObjRef < 0 )
    ObjRef = -ObjRef;
  
  return ObjRef - 1;
}

#if defined LIBUNR_BIG_ENDIAN
  #define NONE_STR 0x4e6f6e65
#else
  #define NONE_STR 0x656e6f4e
#endif


//========================================================================
// EOF
//========================================================================
