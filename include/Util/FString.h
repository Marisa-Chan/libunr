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
#include <string>
#include "Util/FTypes.h"

class FName;
class FPackageFileIn;
class FPackageFileOut;

using std::string;
class FString : public string
{
public:
  FString() : string() {}
  FString( const string& Str ) : string( Str ) {}
  FString( const string& Str, size_t Pos, size_t Len = MAX_SIZE )
    : string( Str, Pos, Len ) {}
  FString( const FString& Str )
    : string( (string&)Str ) {}
  FString( const FString& Str, size_t Pos, size_t Len = MAX_SIZE )
    : string( (string&)Str, Pos, Len ) {}
  FString( const char* s ) : string( s ) {}
  FString( const char* s, size_t n ) : string( s, n ) {}
  FString( size_t n, char c ) : string( n, c ) {}
  FString( int I ) : string( std::to_string( (int)I )) {}
  FString( i64 I ) : string( std::to_string( (i64)I )) {}
  FString( u32 U ) : string( std::to_string( (u32)U )) {}
  FString( u64 U ) : string( std::to_string( (u64)U )) {}
  FString( float F ) : string( std::to_string( (float)F )) {}
  FString( double D ) : string( std::to_string( (double)D )) {}
  FString( bool B ) : string( std::to_string( (bool)B )) {}
  ~FString() {}

  FORCEINLINE size_t Size() const             { return string::size(); }
  FORCEINLINE size_t Length() const           { return string::length(); }
  FORCEINLINE void Resize( size_t n )         { string::resize( n ); }
  FORCEINLINE void Resize( size_t n, char c ) { string::resize( n, c ); }
  FORCEINLINE size_t Capacity() const         { return string::capacity(); }
  FORCEINLINE bool IsEmpty() const            { return string::empty(); }
  FORCEINLINE void Reserve( size_t n )        { string::reserve( n ); }
  FORCEINLINE void Reclaim()                  { string::shrink_to_fit(); }
  
  FORCEINLINE char At( size_t n ) { return string::at( n ); }
  FORCEINLINE char Front()        { return string::front(); }
  FORCEINLINE char Back()         { return string::back(); }
  FORCEINLINE char* Data()        { return (char*)string::data(); }
  
  FORCEINLINE const char At( size_t n ) const { return string::at( n ); }
  FORCEINLINE const char Front() const        { return string::front(); }
  FORCEINLINE const char Back() const         { return string::back(); }
  FORCEINLINE const char* Data() const        { return string::data(); }

  FORCEINLINE FString& Append( const FString& Str )
    { string::append( Str ); return *this; }
  FORCEINLINE FString& Append( const FString& Str, size_t SubPos, size_t SubLen = MAX_SIZE ) 
    { string::append ( Str, SubPos, SubLen ); return *this; }
  FORCEINLINE FString& Append( const char* s )
    { string::append ( s ); return *this; }
  FORCEINLINE FString& Append( const char* s, size_t n )
    { string::append( s, n ); return *this; }
  FORCEINLINE FString& Append( size_t n, char c )
    { string::append( n, c ); return *this; }
  
  FORCEINLINE void PushBack( char c ) { string::push_back( c ); }
  
  FORCEINLINE FString& Assign( const FString& Str )
    { string::assign( Str ); return *this; }
  FORCEINLINE FString& Assign( const FString& Str, size_t SubPos, size_t SubLen = MAX_SIZE )
    { string::assign( Str, SubPos, SubLen ); return *this; }
  FORCEINLINE FString& Assign( const char* s )
    { string::assign( s ); return *this; }
  FORCEINLINE FString& Assign( const char* s, size_t n )
    { string::assign( s, n ); return *this; }
  FORCEINLINE FString& Assign( size_t n, char c )
    { string::assign( n, c ); return *this; }

  FORCEINLINE FString& Insert( size_t Pos, const FString& Str )
    { string::insert( Pos, Str ); return *this; }
  FORCEINLINE FString& Insert( size_t Pos, const FString& Str, size_t SubPos, size_t SubLen )
    { string::insert( Pos, Str, SubPos, SubLen ); return *this; }
  FORCEINLINE FString& Insert( size_t Pos, const char* s )
    { string::insert( Pos, s ); return *this; }
  FORCEINLINE FString& Insert( size_t Pos, const char* s, size_t n )
    { string::insert( Pos, s, n ); return *this; }
  FORCEINLINE FString& Insert( size_t Pos, size_t n, char c )
    { string::insert( Pos, n, c ); return *this; }
  
  FORCEINLINE FString& Erase( size_t Pos = 0, size_t Len = MAX_SIZE )
    { erase( Pos, Len ); return *this; }
    
  FORCEINLINE FString& Replace( size_t Pos, size_t Len, const string& Str )
    { string::replace( Pos, Len, Str ); return *this; }
  FORCEINLINE FString& Replace( size_t Pos, size_t Len, const string& Str, size_t SubPos, size_t SubLen )
    { string::replace( Pos, Len, Str, SubPos, SubLen ); return *this; }
  FORCEINLINE FString& Replace( size_t Pos, size_t Len, const char* s )
    { string::replace( Pos, Len, s ); return *this; }
  FORCEINLINE FString& Replace( size_t Pos, size_t Len, const char* s, size_t n )
    { string::replace( Pos, Len, s, n ); return *this; }
  FORCEINLINE FString& Replace( size_t Pos, size_t Len, size_t n, char c )
    { string::replace( Pos, Len, n, c ); return *this; } 

  FORCEINLINE void Swap( FString& Str ) { string::swap( Str ); }
  
  FORCEINLINE void PopBack() { string::pop_back(); }
  
  FORCEINLINE size_t Find( const FString& Str, size_t Pos = 0 )  { return string::find( Str, Pos ); }
  FORCEINLINE size_t Find( const char* s, size_t Pos = 0 )       { return string::find( s, Pos ); }
  FORCEINLINE size_t Find( const char* s, size_t Pos, size_t n ) { return string::find( s, Pos, n ); }
  FORCEINLINE size_t Find( char  c, size_t Pos = 0 )             { return string::find( c, Pos ); }
  
  FORCEINLINE size_t RFind( const FString& Str, size_t Pos = 0 )  { return string::rfind( Str, Pos ); }
  FORCEINLINE size_t RFind( const char* s, size_t Pos = 0 )       { return string::rfind( s, Pos ); }
  FORCEINLINE size_t RFind( const char* s, size_t Pos, size_t n ) { return string::rfind( s, Pos, n ); }
  FORCEINLINE size_t RFind( char  c, size_t Pos = 0 )             { return string::rfind( c, Pos ); }
  
  FORCEINLINE size_t FindFirstOf( const FString& Str, size_t Pos = 0 )  { return string::find_first_of( Str, Pos ); }
  FORCEINLINE size_t FindFirstOf( const char* s, size_t Pos = 0 )       { return string::find_first_of( s, Pos ); }
  FORCEINLINE size_t FindFirstOf( const char* s, size_t Pos, size_t n ) { return string::find_first_of( s, Pos, n ); }
  FORCEINLINE size_t FindFirstOf( char  c, size_t Pos = 0 )             { return string::find_first_of( c, Pos ); }
 
  FORCEINLINE size_t FindLastOf( const FString& Str, size_t Pos = -1 )  { return string::find_last_of( Str, Pos ); }
  FORCEINLINE size_t FindLastOf( const char* s, size_t Pos = -1 )       { return string::find_last_of( s, Pos ); }
  FORCEINLINE size_t FindLastOf( const char* s, size_t Pos, size_t n )  { return string::find_last_of( s, Pos, n ); }
  FORCEINLINE size_t FindLastOf( char  c, size_t Pos = -1 )             { return string::find_last_of( c, Pos ); }
 
  FORCEINLINE size_t FindFirstNotOf( const FString& Str, size_t Pos = 0 )  { return string::find_first_not_of( Str, Pos ); }
  FORCEINLINE size_t FindFirstNotOf( const char* s, size_t Pos = 0 )       { return string::find_first_not_of( s, Pos ); }
  FORCEINLINE size_t FindFirstNotOf( const char* s, size_t Pos, size_t n ) { return string::find_first_not_of( s, Pos, n ); }
  FORCEINLINE size_t FindFirstNotOf( char  c, size_t Pos = 0 )             { return string::find_first_not_of( c, Pos ); }
 
  FORCEINLINE size_t FindLastNotOf( const FString& Str, size_t Pos = -1 )  { return string::find_last_not_of( Str, Pos ); }
  FORCEINLINE size_t FindLastNotOf( const char* s, size_t Pos = -1 )       { return string::find_last_not_of( s, Pos ); }
  FORCEINLINE size_t FindLastNotOf( const char* s, size_t Pos, size_t n )  { return string::find_last_not_of( s, Pos, n ); }  
  FORCEINLINE size_t FindLastNotOf( char  c, size_t Pos = -1 )             { return string::find_last_not_of( c, Pos ); }
   
  FORCEINLINE string Substr( size_t Pos = 0, size_t Len = MAX_SIZE ) const 
    { return FString( string::substr( Pos, Len ) ); }
  
  FORCEINLINE void ReplaceChars( char Old, char New )
  {
    for (std::string::iterator it = begin(); it != end(); ++it)
    {
      if (*it == Old)
        *it = New;
    }
  }

  FString& operator+=( const FString& Str );
  FString& operator+=( const string& Str );
  FString& operator+=( const char* s );
  FString& operator+=( char c );
  FString& operator+=( FName Name );
 
  friend LIBUNR_API FString operator+( FString& A, const FString& Str );
  friend LIBUNR_API FString operator+( FString& A, const string& Str );
  friend LIBUNR_API FString operator+( FString& A, const char* s );
  friend LIBUNR_API FString operator+( FString& A, char c );
  friend LIBUNR_API FString operator+( FString& A, FName Name );

  friend LIBUNR_API FPackageFileIn&  operator>>( FPackageFileIn& In, FString& Str );
  friend LIBUNR_API FPackageFileOut& operator<<( FPackageFileOut& Out, FString& Str );

  friend bool operator==( const FString& lhs, const FString& rhs );
  friend bool operator==( const char* lhs,    const FString& rhs );
  friend bool operator==( const FString& lhs, const char*   rhs );
  friend bool operator==( const FString& lhs, const string& rhs );
  friend bool operator==( const string& lhs,  const FString& rhs );

  friend bool operator!=( const FString& lhs, const FString& rhs );
  friend bool operator!=( const char* lhs,    const FString& rhs );
  friend bool operator!=( const FString& lhs, const char*   rhs );
  friend bool operator!=( const FString& lhs, const string& rhs );
  friend bool operator!=( const string& lhs,  const FString& rhs );
};
  
LIBUNR_API FString operator+( const char* lhs, const string& rhs );
LIBUNR_API FString operator+( const char* lhs, const FString& rhs );
LIBUNR_API FString operator+( char lhs, const string& rhs );
LIBUNR_API FString operator+( char lhs, const FString& rhs );

#ifdef LIBUNR_WIN32
  #pragma warning(disable:4273)
#endif

LIBUNR_API int stricmp ( const char* str1, const char* str2 );
LIBUNR_API int strnicmp( const char* str1, const char* str2, size_t count );

#ifdef LIBUNR_WIN32
  #pragma warning(pop)
#endif

LIBUNR_API char* strupper( const char* str );
LIBUNR_API char* GetDateString( const char* Fmt );

