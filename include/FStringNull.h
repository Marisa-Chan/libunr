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
 * FStringNull.h - Template header for FString
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FUtil.h"
#include "FArchive.h"
#error "FString needs an implementation!"

/*
 * Each class should have their own definition of an Iterator and ReverseIterator
 */

class FString
{
public:
  FString();
  FString( const FString& Str );
  FString( const FString& Str, size_t Pos, size_t len = MAX_SIZE );
  FString( const char* s );
  FString( const char* s, size_t n );
  FString( size_t n, char c );
  
  Iterator Begin();
  Iterator End();
  ReverseIterator RBegin();
  ReverseIterator REnd();
  
  const Iterator CBegin();
  const Iterator CEnd();
  const ReverseIterator CRBegin();
  const ReverseIterator CREnd();
  
  size_t Size();
  size_t Length();
  size_t MaxSize();
  void   Resize( size_t n );
  void   Resize( size_t n, char c );
  size_t Capacity();
  bool   IsEmpty();
  void   Reserve( size_t n );
  void   Reclaim();
  
  char operator[]( size_t n );
  char At( size_t n );
  char Front();
  char Back();
  char* Data();
  
  const char operator[]( size_t n ) const;
  const char At( size_t n ) const;
  const char Front() const;
  const char Back() const;
  const char* Data() const;
  
  FString& operator+=( const FString& Str );
  FString& operator+=( const char* s );
  FString& operator+=( char c );
  
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
  
  FString& Insert( size_t Pos, const FString& Str );
  FString& Insert( size_t Pos, const FString& Str, size_t SubPos, size_t SubLen );
  FString& Insert( size_t Pos, const char* s );
  FString& Insert( size_t Pos, const char* s, size_t n );
  FString& Insert( size_t Pos, size_t n, char c );
  FString& Insert( const Iterator Pos, size_t n, char c );
  FString& Insert( const Iterator Pos, char c );
  
  FString& Erase( size_t Pos = 0, size_t Len = MAX_SIZE );
  Iterator Erase( Iterator p );
  Iterator Erase( Iterator First, Iterator Last );
  
  FString& Replace( size_t Pos, size_t Len, const FString& Str );
  FString& Replace( const Iterator i1, const Iterator i2, const FString& Str );
  FString& Replace( size_t Pos, size_t Len, const FString& Str, size_t SubPos, size_t SubLen );
  FString& Replace( size_t Pos, size_t Len, const char* s );
  FString& Replace( const Iterator i1, const Iterator i2, const char* s );
  FString& Replace( size_t Pos, size_t Len, const char* s, size_t n );
  FString& Replace( const Iterator i1, const Iterator i2, const char* s, size_t n );
  FString& Replace( size_t Pos, size_t Len, size_t n, char c );
  FString& Replace( const Iterator i1, const Iterator i2, size_t n, char c );
  
  void Swap( FString& Str );
  
  void PopBack();
  
  size_t Find( const FString& Str, size_t Pos = 0 );
  size_t Find( const char* s, size_t Pos = 0 );
  size_t Find( const char* s, size_t Pos, size_t n );
  size_t Find( char  c, size_t Pos = 0 );
  
  size_t RFind( const FString& Str, size_t Pos = 0 );
  size_t RFind( const char* s, size_t Pos = 0 );
  size_t RFind( const char* s, size_t Pos, size_t n );
  size_t RFind( char  c, size_t Pos = 0 );
  
  size_t FindFirstOf( const FString& Str, size_t Pos = 0 );
  size_t FindFirstOf( const char* s, size_t Pos = 0 );
  size_t FindFirstOf( const char* s, size_t Pos, size_t n );
  size_t FindFirstOf( char c, size_t Pos = 0 );
  
  size_t FindLastOf( const FString& Str, size_t Pos = 0 );
  size_t FindLastOf( const char* s, size_t Pos = 0 );
  size_t FindLastOf( const char* s, size_t Pos, size_t n );
  size_t FindLastOf( char c, size_t Pos = 0 );
  
  size_t FindFirstNotOf( const FString& Str, size_t Pos = 0 );
  size_t FindFirstNotOf( const char* s, size_t Pos = 0 );
  size_t FindFirstNotOf( const char* s, size_t Pos, size_t n );
  size_t FindFirstNotOf( char c, size_t Pos = 0 );
  
  size_t FindLastNotOf( const FString& Str, size_t Pos = 0 );
  size_t FindLastNotOf( const char* s, size_t Pos = 0 );
  size_t FindLastNotOf( const char* s, size_t Pos, size_t n );
  size_t FindLastNotOf( char c, size_t Pos = 0 );
  
  FString Substr( size_t Pos = 0, size_t Len = MAX_SIZE );
  void ReplaceChars( char Old, char New );
  
  friend FArchiveIn& operator<<( FArchiveIn& Ar, FString& Str );
  friend FArchiveOut& operator>>( FArchiveOut& Ar, FString& Str );
};
