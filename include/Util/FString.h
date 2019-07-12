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
 * FString.h - A string with support for libunr data types
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include <string>

class FName;
class FPackageFileIn;
class FPackageFileOut;

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

  using std::string::size;
  using std::string::length;
  using std::string::resize;
  using std::string::capacity;
  using std::string::empty;
  using std::string::reserve;
  using std::string::shrink_to_fit;
  using std::string::at;
  using std::string::front;
  using std::string::back;
  using std::string::data;
  using std::string::append;
  using std::string::insert;
  using std::string::erase;
  using std::string::replace;
  using std::string::swap;
  using std::string::pop_back;
  using std::string::push_back;
  using std::string::find;
  using std::string::rfind;
  using std::string::find_first_of;
  using std::string::find_last_of;
  using std::string::find_first_not_of;
  using std::string::first_last_not_of;
  using std::string::substr;

  DECLARE_FUNCTION_ALIAS( size_t, Size )() const { return Internal.size(); }
  DECLARE_FUNCTION_ALIAS( size_t, Length )() const { return Internal.length(); }
  DECLARE_FUNCTION_ALIAS( void, Resize )( size_t n ) { Internal.resize( n ); }
  DECLARE_FUNCTION_ALIAS( void, Resize )( size_t n, char c ) { Internal.resize( n, c ); }
  DECLARE_FUNCTION_ALIAS( size_t, Capacity )() const { return Internal.capacity(); }
  DECLARE_FUNCTION_ALIAS( bool, IsEmpty )() const { return Internal.empty(); }
  DECLARE_FUNCTION_ALIAS( void, Reserve )( size_t n ) { Internal.reserve( n ); }
  DECLARE_FUNCTION_ALIAS( void, Reclaim )() { Internal.shrink_to_fit(); }
  
  DECLARE_FUNCTION_ALIAS( char&, operator[] )( size_t n ) { return Internal[n]; }
  DECLARE_FUNCTION_ALIAS( char, At )( size_t n ) { return Internal.at( n ); }
  DECLARE_FUNCTION_ALIAS( char, Front )() { return Internal.front(); }
  DECLARE_FUNCTION_ALIAS( char, Back )() { return Internal.back(); }
  DECLARE_FUNCTION_ALIAS( char*, Data )() { return (char*)Internal.data(); }
  
  DECLARE_FUNCTION_ALIAS( const char&, operator[] )( size_t n ) const { return Internal[n]; }
  DECLARE_FUNCTION_ALIAS( const char, At )( size_t n ) const { return Internal.at( n ); }
  DECLARE_FUNCTION_ALIAS( const char, Front )() const { return Internal.front(); }
  DECLARE_FUNCTION_ALIAS( const char, Back )() const { return Internal.back(); }
  DECLARE_FUNCTION_ALIAS( const char*, Data )() const { return Internal.data(); }
  
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


