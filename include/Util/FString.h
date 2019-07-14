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
  using std::string::find_last_not_of;
  using std::string::substr;

  DECLARE_FUNCTION_ALIAS( size_t, Size )() const { return size(); }
  DECLARE_FUNCTION_ALIAS( size_t, Length )() const { return length(); }
  DECLARE_FUNCTION_ALIAS( void, Resize )( size_t n ) { resize( n ); }
  DECLARE_FUNCTION_ALIAS( void, Resize )( size_t n, char c ) { resize( n, c ); }
  DECLARE_FUNCTION_ALIAS( size_t, Capacity )() const { return capacity(); }
  DECLARE_FUNCTION_ALIAS( bool, IsEmpty )() const { return empty(); }
  DECLARE_FUNCTION_ALIAS( void, Reserve )( size_t n ) { reserve( n ); }
  DECLARE_FUNCTION_ALIAS( void, Reclaim )() { shrink_to_fit(); }
  
  DECLARE_FUNCTION_ALIAS( char, At )( size_t n ) { return at( n ); }
  DECLARE_FUNCTION_ALIAS( char, Front )() { return front(); }
  DECLARE_FUNCTION_ALIAS( char, Back )() { return back(); }
  DECLARE_FUNCTION_ALIAS( char*, Data )() { return (char*)data(); }
  
  DECLARE_FUNCTION_ALIAS( const char, At )( size_t n ) const { return at( n ); }
  DECLARE_FUNCTION_ALIAS( const char, Front )() const { return front(); }
  DECLARE_FUNCTION_ALIAS( const char, Back )() const { return back(); }
  DECLARE_FUNCTION_ALIAS( const char*, Data )() const { return data(); }

  DECLARE_FUNCTION_ALIAS( FString&, Append )( const FString& Str )       { append( Str ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Append )( const FString& Str, size_t SubPos, size_t SubLen = MAX_SIZE ) 
    { append ( Str, SubPos, SubLen ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Append )( const char* s )           { append ( s ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Append )( const char* s, size_t n ) { append( s, n ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Append )( size_t n, char c )        { append( n, c ); return *this; }
  
  DECLARE_FUNCTION_ALIAS( void, PushBack )( char c ) { push_back( c ); }
  
  DECLARE_FUNCTION_ALIAS( FString&, Assign )( const FString& Str )       { assign( Str ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Assign )( const FString& Str, size_t SubPos, size_t SubLen = MAX_SIZE )
    { assign( Str, SubPos, SubLen ); return *this; }

  DECLARE_FUNCTION_ALIAS( FString&, Assign )( const char* s )           { assign( s ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Assign )( const char* s, size_t n ) { assign( s, n ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Assign )( size_t n, char c )        { assign( n, c ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Insert )( size_t Pos, const FString& Str )        { insert( Pos, Str ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Insert )( size_t Pos, const FString& Str, size_t SubPos, size_t SubLen )
    { insert( Pos, Str, SubPos, SubLen ); return *this; }


  DECLARE_FUNCTION_ALIAS( FString&, Insert )( size_t Pos, const char* s )            { insert( Pos, s ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Insert )( size_t Pos, const char* s, size_t n )  { insert( Pos, s, n ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Insert )( size_t Pos, size_t n, char c )         { insert( Pos, n, c ); return *this; }
  
  DECLARE_FUNCTION_ALIAS( FString&, Erase )( size_t Pos = 0, size_t Len = MAX_SIZE ) { erase( Pos, Len ); return *this; }
    
  DECLARE_FUNCTION_ALIAS( FString&, Replace )( size_t Pos, size_t Len, const String& Str ){ replace( Pos, Len, Str ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Replace )( size_t Pos, size_t Len, const String& Str, size_t SubPos, size_t SubLen ) { replace( Pos, Len, Str, SubPos, SubLen ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Replace )( size_t Pos, size_t Len, const char* s )                   { replace( Pos, Len, s ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Replace )( size_t Pos, size_t Len, const char* s, size_t n )         { replace( Pos, Len, s, n ); return *this; }
  DECLARE_FUNCTION_ALIAS( FString&, Replace )( size_t Pos, size_t Len, size_t n, char c )                { replace( Pos, Len, n, c ); return *this; } 

  FORCEINLINE void Swap( FString& Str ) { swap( Str ); }
  
  FORCEINLINE void PopBack() { pop_back(); }
  
  FORCEINLINE size_t Find( const FString& Str, size_t Pos = 0 )  { return find( Str, Pos ); }
  FORCEINLINE size_t Find( const char* s, size_t Pos = 0 )       { return find( s, Pos ); }
  FORCEINLINE size_t Find( const char* s, size_t Pos, size_t n ) { return find( s, Pos, n ); }
  FORCEINLINE size_t Find( char  c, size_t Pos = 0 )             { return find( c, Pos ); }
  
  FORCEINLINE size_t RFind( const FString& Str, size_t Pos = 0 )  { return rfind( Str, Pos ); }
  FORCEINLINE size_t RFind( const char* s, size_t Pos = 0 )       { return rfind( s, Pos ); }
  FORCEINLINE size_t RFind( const char* s, size_t Pos, size_t n ) { return rfind( s, Pos, n ); }
  FORCEINLINE size_t RFind( char  c, size_t Pos = 0 )             { return rfind( c, Pos ); }
  
  FORCEINLINE size_t FindFirstOf( const FString& Str, size_t Pos = 0 )  { return find_first_of( Str, Pos ); }
  FORCEINLINE size_t FindFirstOf( const char* s, size_t Pos = 0 )       { return find_first_of( s, Pos ); }
  FORCEINLINE size_t FindFirstOf( const char* s, size_t Pos, size_t n ) { return find_first_of( s, Pos, n ); }
  FORCEINLINE size_t FindFirstOf( char  c, size_t Pos = 0 )             { return find_first_of( c, Pos ); }
 
  FORCEINLINE size_t FindLastOf( const FString& Str, size_t Pos = -1 )  { return find_last_of( Str, Pos ); }
  FORCEINLINE size_t FindLastOf( const char* s, size_t Pos = -1 )       { return find_last_of( s, Pos ); }
  FORCEINLINE size_t FindLastOf( const char* s, size_t Pos, size_t n )  { return find_last_of( s, Pos, n ); }
  FORCEINLINE size_t FindLastOf( char  c, size_t Pos = -1 )             { return find_last_of( c, Pos ); }
 
  FORCEINLINE size_t FindFirstNotOf( const FString& Str, size_t Pos = 0 )  { return find_first_not_of( Str, Pos ); }
  FORCEINLINE size_t FindFirstNotOf( const char* s, size_t Pos = 0 )       { return find_first_not_of( s, Pos ); }
  FORCEINLINE size_t FindFirstNotOf( const char* s, size_t Pos, size_t n ) { return find_first_not_of( s, Pos, n ); }
  FORCEINLINE size_t FindFirstNotOf( char  c, size_t Pos = 0 )             { return find_first_not_of( c, Pos ); }
 
  FORCEINLINE size_t FindLastNotOf( const FString& Str, size_t Pos = -1 )  { return find_last_not_of( Str, Pos ); }
  FORCEINLINE size_t FindLastNotOf( const char* s, size_t Pos = -1 )       { return find_last_not_of( s, Pos ); }
  FORCEINLINE size_t FindLastNotOf( const char* s, size_t Pos, size_t n )  { return find_last_not_of( s, Pos, n ); }  
  FORCEINLINE size_t FindLastNotOf( char  c, size_t Pos = -1 )             { return find_last_not_of( c, Pos ); }
   
  FORCEINLINE String Substr( size_t Pos = 0, size_t Len = MAX_SIZE ) const { return FString( substr( Pos, Len ) ); }
  FORCEINLINE void ReplaceChars( char Old, char New )
  {
    for (std::string::iterator it = begin(); it != end(); ++it)
    {
      if (*it == Old)
        *it = New;
    }
  }

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

