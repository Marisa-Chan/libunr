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
#include <ctype.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>
#include <string>

#if defined __linux__ || defined __unix__
  #define LIBUNR_POSIX
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <dirent.h>
  #include <pwd.h>
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
	# if defined __i386__
		#define ftello ftello64
		#define fseeko fseeko64
	#endif
  #else
    #define DLL_EXPORT
  #endif
 
#else
  #error "Unsupported compiler type, read more here"
  
  // At this point in time, only the g++ and Clang compilers
  // are supported. Compilers for less common architectures 
  // should also be supported where cross-platform compilers do
  // not have support or if they tend to generate slower code for
  // said platform. No version of Visual Studio will be officially 
  // supported.
  
#endif

// pointer arithmetic helpers
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

static inline int CalcObjRefValue( idx ObjRef )
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
class FPackageFileIn;
class FPackageFileOut;
/*-----------------------------------------------------------------------------
 * FNameEntry
 * An entry into a name table
-----------------------------------------------------------------------------*/
#define NAME_LEN 64
struct DLL_EXPORT FNameEntry
{
   FNameEntry();
   FNameEntry( const char* InStr, int InFlags = 0 );
  ~FNameEntry();
  
  friend FPackageFileIn&  operator>>( FPackageFileIn& In,  FNameEntry& Name );
  friend FPackageFileOut& operator<<( FPackageFileOut& In, FNameEntry& Name );
  
  char Data[NAME_LEN];
  FHash Hash;
  int Flags;
};

/*-----------------------------------------------------------------------------
 * FName
 * An index into the global name table
-----------------------------------------------------------------------------*/
struct DLL_EXPORT FName
{
  u32 Index;

  FName() { Index = 0; }
  FName( int Idx ) { Index = Idx; }

  static FName CreateName( const char* InName, int InFlags );
  static inline bool IsNameNone( FName Name )
  {
    return (*(int*)Name.Data()) == NONE_STR;
  }

  inline operator u32()
  {
    return Index;
  }

  const char* Data() const;
  const FHash& Hash() const;

  inline FName operator=( int Idx )
  {
    Index = Idx;
    return *this;
  }

  friend bool operator==( FName& A, FName& B );
  friend bool operator!=( FName& A, FName& B );
  friend FPackageFileIn& operator>>( FPackageFileIn& In, FName& Name );
};

/*-----------------------------------------------------------------------------
 * FString
 * A string with support for libunr data types
-----------------------------------------------------------------------------*/
typedef std::string String;
class FString : public String
{
public:
  FString() : String() {}
  FString( const String& Str ) : String( Str ) {}
  FString( const String& Str, size_t Pos, size_t Len = MAX_SIZE )
    : String( Str, Pos, Len ) {}
  FString( const FString& Str )
    : String( (String&)Str ) {}
  FString( const FString& Str, size_t Pos, size_t Len = MAX_SIZE )
    : String( (String&)Str, Pos, Len ) {}
  FString( const char* s ) : String( s ) {}
  FString( const char* s, size_t n ) : String( s, n ) {}
  FString( size_t n, char c ) : String( n, c ) {}
  FString( int I ) : String( std::to_string( (int)I )) {}
  FString( i64 I ) : String( std::to_string( (i64)I )) {}
  FString( u32 U ) : String( std::to_string( (u32)U )) {}
  FString( u64 U ) : String( std::to_string( (u64)U )) {}
  FString( float F ) : String( std::to_string( (float)F )) {}
  FString( double D ) : String( std::to_string( (double)D )) {}
  FString( bool B ) : String( std::to_string( (bool)B )) {}
  
  FString& operator+=( const FString& Str );
  FString& operator+=( const String& Str );
  FString& operator+=( const char* s );
  FString& operator+=( char c );
  FString& operator+=( FName Name );
 
  FString operator+( const FString& Str ) const;
  FString operator+( const String& Str ) const;
  FString operator+( const char* s ) const;
  FString operator+( char c ) const;
  FString& operator+( FName Name );

  friend FPackageFileIn&  operator>>( FPackageFileIn& In, FString& Str );
  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FString& Str );

  friend bool operator==( const FString& lhs, const FString& rhs );
  friend bool operator==( const char* lhs,    const FString& rhs );
  friend bool operator==( const FString& lhs, const char*   rhs );
  friend bool operator==( const FString& lhs, const String& rhs );
  friend bool operator==( const String& lhs,  const FString& rhs );

  friend bool operator!=( const FString& lhs, const FString& rhs );
  friend bool operator!=( const char* lhs,    const FString& rhs );
  friend bool operator!=( const FString& lhs, const char*   rhs );
  friend bool operator!=( const FString& lhs, const String& rhs );
  friend bool operator!=( const String& lhs,  const FString& rhs );
};
  
FString operator+( const char* lhs, const String& rhs );
FString operator+( const char* lhs, const FString& rhs );
FString operator+( char lhs, const String& rhs );
FString operator+( char lhs, const FString& rhs );

#ifndef __MINGW64__
DLL_EXPORT int stricmp ( const char* str1, const char* str2 );
DLL_EXPORT int strnicmp( const char* str1, const char* str2, size_t count ); 
#endif
DLL_EXPORT char* strupper( const char* str );
DLL_EXPORT char* GetDateString( const char* Fmt );

//========================================================================
// EOF
//========================================================================
