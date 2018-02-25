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
 * FStringStl.h - STL implementation of FString
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FUtil.h"

/*
 * Each class should have their own definition of an Iterator and ReverseIterator
 */

class FString
{
private:
  FString( std::string Str ) { RealStr = Str; }
  std::string RealStr;
  
public:
  typedef typename std::string::iterator Iterator;
  typedef typename std::string::reverse_iterator ReverseIterator;
  
  FString()                          { new ( &RealStr ) std::string(); }
  FString( const FString& Str )      { new ( &RealStr ) std::string( Str.RealStr ); }
  FString( const FString& Str, size_t Pos, size_t Len = MAX_SIZE ) 
                                     { new ( &RealStr ) std::string( Str.RealStr, Pos, Len ); }
  FString( const char* s )           { new ( &RealStr ) std::string( s ); }
  FString( const char* s, size_t n ) { new ( &RealStr ) std::string( s, n ); }
  FString( size_t n, char c )        { new ( &RealStr ) std::string( n, c ); }
  
  Iterator Begin() { return RealStr.begin(); }
  Iterator End() { return RealStr.end(); }
  ReverseIterator RBegin() { return RealStr.rbegin(); }
  ReverseIterator REnd() { return RealStr.rend(); }
/*  
  const Iterator CBegin() { return RealStr.cbegin(); }
  const Iterator CEnd() { return RealStr.cend(); }
  const ReverseIterator CRBegin() { return RealStr.crbegin(); }
  const ReverseIterator CREnd() { return RealStr.crend(); }
*/
  size_t Size() { return RealStr.size(); }
  size_t Length() { return RealStr.length(); }
  size_t MaxSize() { return RealStr.max_size(); }
  void   Resize( size_t n ) { RealStr.resize( n ); }
  void   Resize( size_t n, char c ) { RealStr.resize( n, c ); }
  size_t Capacity() { return RealStr.capacity(); }
  bool   IsEmpty() { return RealStr.empty(); }
  void   Reserve( size_t n ) { RealStr.reserve( n ); }
  void   Reclaim() { RealStr.shrink_to_fit(); }
  
  char operator[]( size_t n ) { return RealStr[n]; }
  char At( size_t n ) { return RealStr.at( n ); }
  char Front() { return RealStr.front(); }
  char Back() { return RealStr.back(); }
  
  const char operator[]( size_t n ) const { return RealStr[n]; }
  const char At( size_t n ) const { return RealStr.at( n ); }
  const char Front() const { return RealStr.front(); }
  const char Back() const { return RealStr.back(); }
  const char* Data() const { return RealStr.data(); }
  
  FString& operator+=( const FString& Str ) { RealStr += Str.RealStr; return *this; }
  FString& operator+=( const char* s ) { RealStr += s; return *this; }
  FString& operator+=( char c ) { RealStr += c; return *this; }
  
  FString& Append( const FString& Str ) { RealStr.append( Str.RealStr ); return *this; }
  FString& Append( const FString& Str, size_t SubPos, size_t SubLen = MAX_SIZE ) { RealStr.append ( Str.RealStr, SubPos, SubLen ); return *this; }
  FString& Append( const char* s ) { RealStr.append ( s ); return *this; }
  FString& Append( const char* s, size_t n ) { RealStr.append( s, n ); return *this; }
  FString& Append( size_t n, char c ) { RealStr.append( n, c ); return *this; }
  
  void PushBack( char c ) { RealStr.push_back( c ); }
  
  FString& Assign( const FString& Str ) { RealStr.assign( Str.RealStr ); return *this; }
  FString& Assign( const FString& Str, size_t SubPos, size_t SubLen = MAX_SIZE ) { RealStr.assign( Str.RealStr, SubPos, SubLen ); return *this; }
  FString& Assign( const char* s ) { RealStr.assign( s ); return *this; }
  FString& Assign( const char* s, size_t n ) { RealStr.assign( s, n ); return *this; }
  FString& Assign( size_t n, char c ) { RealStr.assign( n, c ); return *this; }
  
  FString& Insert( size_t Pos, const FString& Str ) { RealStr.insert( Pos, Str.RealStr ); return *this; }
  FString& Insert( size_t Pos, const FString& Str, size_t SubPos, size_t SubLen ) { RealStr.insert( Pos, Str.RealStr, SubPos, SubLen ); return *this; }
  FString& Insert( size_t Pos, const char* s ) { RealStr.insert( Pos, s ); return *this; }
  FString& Insert( size_t Pos, const char* s, size_t n ) { RealStr.insert( Pos, s, n ); return *this; }
  FString& Insert( size_t Pos, size_t n, char c ) { RealStr.insert( Pos, n, c ); return *this; }
  FString& Insert( const Iterator Pos, size_t n, char c ) { RealStr.insert( Pos, n ,c ); return *this; }
  FString& Insert( const Iterator Pos, char c ) { RealStr.insert( Pos, c ); return *this; }
  
  FString& Erase( size_t Pos = 0, size_t Len = MAX_SIZE ) { RealStr.erase( Pos, Len ); return *this; }
  Iterator Erase( Iterator p ) { return RealStr.erase( p ); }
  Iterator Erase( Iterator First, Iterator Last ) { return RealStr.erase( First, Last ); }
  
  FString& Replace( size_t Pos, size_t Len, const FString& Str ) { RealStr.replace( Pos, Len, Str.RealStr ); return *this; }
  FString& Replace( const Iterator i1, const Iterator i2, const FString& Str ) { RealStr.replace( i1, i2, Str.RealStr ); return *this; }
  FString& Replace( size_t Pos, size_t Len, const FString& Str, size_t SubPos, size_t SubLen ) { RealStr.replace( Pos, Len, Str.RealStr, SubPos, SubLen ); return *this; }
  FString& Replace( size_t Pos, size_t Len, const char* s ) { RealStr.replace( Pos, Len, s ); return *this; }
  FString& Replace( const Iterator i1, const Iterator i2, const char* s ) { RealStr.replace( i1, i2, s ); return *this; }
  FString& Replace( size_t Pos, size_t Len, const char* s, size_t n ) { RealStr.replace( Pos, Len, s, n ); return *this; }
  FString& Replace( const Iterator i1, const Iterator i2, const char* s, size_t n ) { RealStr.replace( i1, i2, s, n ); return *this; }
  FString& Replace( size_t Pos, size_t Len, size_t n, char c ) { RealStr.replace( Pos, Len, n, c ); return *this; } 
  FString& Replace( const Iterator i1, const Iterator i2, size_t n, char c ) { RealStr.replace( i1, i2, n, c ); return *this; }
  
  void Swap( FString& Str ) { RealStr.swap( Str.RealStr ); }
  void PopBack() { RealStr.pop_back(); }
  
  size_t Find( const FString& Str, size_t Pos = 0 ) { return RealStr.find( Str.RealStr, Pos ); }
  size_t Find( const char* s, size_t Pos = 0 ) { return RealStr.find( s, Pos ); }
  size_t Find( const char* s, size_t Pos, size_t n ) { return RealStr.find( s, Pos, n ); }
  size_t Find( char  c, size_t Pos = 0 ) { return RealStr.find( c, Pos ); }
  
  size_t RFind( const FString& Str, size_t Pos = 0 ) { return RealStr.rfind( Str.RealStr, Pos ); }
  size_t RFind( const char* s, size_t Pos = 0 ) { return RealStr.rfind( s, Pos ); }
  size_t RFind( const char* s, size_t Pos, size_t n ) { return RealStr.rfind( s, Pos, n ); }
  size_t RFind( char  c, size_t Pos = 0 ) { return RealStr.rfind( c, Pos ); }
  
  size_t FindFirstOf( const FString& Str, size_t Pos = 0 ) { return RealStr.find_first_of( Str.RealStr, Pos ); }
  size_t FindFirstOf( const char* s, size_t Pos = 0 ) { return RealStr.find_first_of( s, Pos ); }
  size_t FindFirstOf( const char* s, size_t Pos, size_t n ) { return RealStr.find_first_of( s, Pos, n ); }
  size_t FindFirstOf( char c, size_t Pos = 0 ) { return RealStr.find_first_of( c, Pos ); }
  
  size_t FindLastOf( const FString& Str, size_t Pos = 0 ) { return RealStr.find_last_of( Str.RealStr, Pos ); }
  size_t FindLastOf( const char* s, size_t Pos = 0 ) { return RealStr.find_last_of( s, Pos ); }
  size_t FindLastOf( const char* s, size_t Pos, size_t n ) { return RealStr.find_last_of( s, Pos, n ); }
  size_t FindLastOf( char c, size_t Pos = 0 ) { return RealStr.find_last_of( c, Pos ); }
  
  size_t FindFirstNotOf( const FString& Str, size_t Pos = 0 ) { return RealStr.find_first_not_of( Str.RealStr, Pos ); }
  size_t FindFirstNotOf( const char* s, size_t Pos = 0 ) { return RealStr.find_first_not_of( s, Pos ); }
  size_t FindFirstNotOf( const char* s, size_t Pos, size_t n ) { return RealStr.find_first_not_of( s, Pos, n ); }
  size_t FindFirstNotOf( char c, size_t Pos = 0 ) { return RealStr.find_first_not_of( c, Pos ); }
  
  size_t FindLastNotOf( const FString& Str, size_t Pos = 0 ) { return RealStr.find_last_not_of( Str.RealStr, Pos ); }
  size_t FindLastNotOf( const char* s, size_t Pos = 0 ) { return RealStr.find_last_not_of( s, Pos ); }
  size_t FindLastNotOf( const char* s, size_t Pos, size_t n ) { return RealStr.find_last_not_of( s, Pos, n ); }
  size_t FindLastNotOf( char c, size_t Pos = 0 ) { return RealStr.find_last_not_of( c, Pos ); }
  
  FString Substr( size_t Pos = 0, size_t Len = MAX_SIZE ) { return FString( RealStr.substr( Pos ) ); }
  void ReplaceChars( char Old, char New )
  {
    for (std::string::iterator it = RealStr.begin (); it != RealStr.end (); ++it)
    {
      if (*it == Old)
        *it = New;
    }
  }
  
  //friend FArchiveIn& operator<<( FArchiveIn& Ar, FString& Str );
  //friend FArchiveOut& operator>>( FArchiveOut& Ar, FString& Str );
};

