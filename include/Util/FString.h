/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * FString.h - A string with support for libunr data types
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Util/FName.h"
#include "Util/TArray.h"

/*-----------------------------------------------------------------------------
 * FString
 * A basic string class
-----------------------------------------------------------------------------*/
class LIBUNR_API FString
{
public:
  FString();
  FString( const FString& Str );
  FString( const FString& Str, size_t Pos, size_t Len = MAX_SIZE );
  FString( const char* s );
  FString( const char* s, size_t n );
  FString( size_t n, char c );
  FString( int I );
  FString( i64 I );
  FString( u32 U );
  FString( u64 U );
  FString( float F );
  FString( double D );
  FString( bool B );
  ~FString();

  FORCEINLINE size_t Size() const
  {
    return NumElements;
  }

  FORCEINLINE size_t Length() const
  { 
    return NumElements;
  }

  FORCEINLINE size_t MaxSize() const
  {
    return MAX_UINT64;
  }

  FORCEINLINE size_t Capacity()
  {
    return NumReserved;
  }

  FORCEINLINE char* Data()
  {
    return Array;
  }

  FORCEINLINE const char* Data() const
  {
    return Array;
  }

  FORCEINLINE bool IsEmpty() const
  {
    return (NumElements == 0);
  }

  FORCEINLINE char At( size_t n ) const
  {
  #ifdef LIBUNR_DEBUG
    if ( n >= NumElements )
      DEBUGBREAK();
  #endif

    return Array[n];
  }

  FORCEINLINE char operator[]( size_t n ) const
  {
  #ifdef LIBUNR_DEBUG
    if ( n >= NumElements )
      DEBUGBREAK();
  #endif

    return Array[n];
  }

  FORCEINLINE char& Front() const
  {
  #ifdef LIBUNR_DEBUG
    if ( NumElements == 0 )
      DEBUGBREAK();
  #endif

    return Array[0];
  }

  FORCEINLINE char& Back() const
  {
  #ifdef LIBUNR_DEBUG
    if ( NumElements == 0 )
      DEBUGBREAK();
  #endif

    return Array[NumElements-1];
  }

  void Resize( size_t n );
  void Resize( size_t n, const char Value );
  void Reserve( size_t n );
  void Reclaim();

  FString& Append( const FString& Str );
  FString& Append( const FString& Str, size_t SubPos, size_t SubLen = MAX_SIZE );
  FString& Append( const char* s );
  FString& Append( const char* s, size_t n );
  FString& Append( size_t n, char c );
  void PushBack( char c );

  FString& Assign( const FString& Str );
  FString& Assign( const FString& Str, size_t SubPos, size_t SubLen = MAX_SIZE );
  FString& Assign( const char* s );
  FString& Assign( const char* s, size_t n );
  FString& Assign( size_t n, char c );

  FString& Insert( const FString& Str, size_t Pos );
  FString& Insert( const FString& Str, size_t Pos, size_t SubPos, size_t SubLen = MAX_SIZE );
  FString& Insert( const char* s, size_t Pos );
  FString& Insert( const char* s, size_t n, size_t Pos );
  FString& Insert( size_t Pos, size_t n, char c );

  FString& Replace( size_t Pos, size_t Len, const char* s );
  FString& Replace( size_t Pos, size_t Len, const char* s, size_t n );
  FString& Replace( size_t Pos, size_t Len, size_t n, char c );

  FString& Erase( size_t Pos = 0, size_t Len = MAX_SIZE );
  void Clear();
  void Swap( FString& Str );
  void PopBack();

  size_t Find( const FString& Str, size_t Pos = 0 );
  size_t Find( const char* s, size_t Pos = 0 );
  size_t Find( const char* s, size_t Pos, size_t n );
  size_t Find( char c, size_t Pos = 0 );

  size_t RFind( const FString& Str, size_t Pos = MAX_SIZE );
  size_t RFind( const char* s, size_t Pos = MAX_SIZE );
  size_t RFind( const char* s, size_t Pos, size_t n );
  size_t RFind( char c, size_t Pos = MAX_SIZE );

  size_t FindFirstOf( const FString& Str, size_t Pos = 0 );
  size_t FindFirstOf( const char* s, size_t Pos = 0 );
  size_t FindFirstOf( const char* s, size_t Pos, size_t n );
  size_t FindFirstOf( char c, size_t Pos = 0 );

  size_t FindLastOf( const FString& Str, size_t Pos = MAX_SIZE );
  size_t FindLastOf( const char* s, size_t Pos = MAX_SIZE );
  size_t FindLastOf( const char* s, size_t Pos, size_t n );
  size_t FindLastOf( char c, size_t Pos = MAX_SIZE );

  size_t FindFirstNotOf( const FString& Str, size_t Pos = 0 );
  size_t FindFirstNotOf( const char* s, size_t Pos = 0 );
  size_t FindFirstNotOf( const char* s, size_t Pos, size_t n );
  size_t FindFirstNotOf( char c, size_t Pos = 0 );

  size_t FindLastNotOf( const FString& Str, size_t Pos = -1 );
  size_t FindLastNotOf( const char* s, size_t Pos = MAX_SIZE );
  size_t FindLastNotOf( const char* s, size_t Pos, size_t n );
  size_t FindLastNotOf( char  c, size_t Pos = -1 );

  FString Substr( size_t Pos = 0, size_t Len = MAX_SIZE ) const;
  void ReplaceChars( char Old, char New );

  FString& operator+=( const FString& Str );
  FString& operator+=( const char* s );
  FString& operator+=( char c );
  FString& operator+=( FName Name );
 
  friend LIBUNR_API FString operator+( const FString& lhs, const FString& rhs );
  friend LIBUNR_API FString operator+( const FString& lhs, const char* rhs );
  friend LIBUNR_API FString operator+( const FString& lhs, char rhs );
  friend LIBUNR_API FString operator+( const FString& lhs, FName rhs );

  friend LIBUNR_API FPackageFileIn&  operator>>( FPackageFileIn& In, FString& Str );
  friend LIBUNR_API FPackageFileOut& operator<<( FPackageFileOut& Out, FString& Str );

  friend bool operator==( const FString& lhs, const FString& rhs );
  friend bool operator==( const char* lhs,    const FString& rhs );
  friend bool operator==( const FString& lhs, const char*   rhs );

  friend bool operator!=( const FString& lhs, const FString& rhs );
  friend bool operator!=( const char* lhs,    const FString& rhs );
  friend bool operator!=( const FString& lhs, const char*   rhs );

protected:
  char* Array;
  size_t NumElements;
  size_t NumReserved;
};

LIBUNR_API FString operator+( const char* lhs, const FString& rhs );
LIBUNR_API FString operator+( char lhs, const FString& rhs );

/*-----------------------------------------------------------------------------
 * FStringFilePath
 * An FString variant that ensures consistent state for a file path
-----------------------------------------------------------------------------*/
class LIBUNR_API FStringFilePath : public FString
{
public:
  FStringFilePath( const char* InDir, const char* InName, const char* InExt = NULL );
  FStringFilePath( const char* Path );
  ~FStringFilePath()
  {
    if ( Dir )
      free( Dir );

    if ( Name )
      free( Name );

    if ( Ext )
      free( Ext );
  }

  const char* GetDir();
  const char* GetName();
  const char* GetExt();

protected:
  char* Dir;
  char* Name;
  char* Ext;
};

#ifdef LIBUNR_WIN32
  #pragma warning(push)
  #pragma warning(disable:4273)
#endif

LIBUNR_API int stricmp ( const char* str1, const char* str2 );
LIBUNR_API int strnicmp( const char* str1, const char* str2, size_t count );

#ifdef LIBUNR_WIN32
  #pragma warning(pop)
#endif

LIBUNR_API char* strupper( const char* str );
LIBUNR_API char* GetDateString( const char* Fmt );

