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
 * TArrayNull.h - Template header for TArray implementations
 * The inclusion of this file should fail compilation
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FUtil.h"
#error "TArray needs an implementation!"

/*
 * Each class should have their own definition of an Iterator
 */

template <class T> class TArray
{
  TArray<T>();
  TArray<T>( size_t n );
  TArray<T>( size_t n, const T& Value );
  TArray<T>( const TArray<T>& x );
  TArray<T>( TArray<T>&& x );
  
  Iterator Begin();
  Iterator End();
  ReverseIterator RBegin();
  ReverseIterator REnd();
  
  const Iterator CBegin() const;
  const Iterator CEnd() const;
  const ReverseIterator CRBegin() const;
  const ReverseIterator CREnd() const;
  
  size_t Size();
  size_t MaxSize();
  void   Resize( size_t n );
  void   Resize( size_t n, const T& Value );
  size_t Capacity();
  bool   IsEmpty();
  void   Reserve( size_t n );
  void   Reclaim();
  
  T& operator[]( size_t n );
  T& At( size_t n );
  T& Front();
  T& Back();
  T* Data();
  
  const T& operator[]( size_t n ) const;
  const T& At( size_t n ) const;
  const T& Front() const;
  const T& Back() const;
  const T* Data() const;
  
  void Assign( size_t n, const T& Value );
  void PushBack( const T& Val );
  void PushBack( T& Val );
  void PopBack();
  
  Iterator Insert( const Iterator Pos, const T& Value );
  Iterator Insert( const Iterator Pos, size_t n, const T& Value );
  Iterator Insert( const Iterator Pos, T&& Value );
  
  Iterator Erase( const Iterator Pos );
  Iterator Erase( const Iterator First, const Iterator Last );
  
  void Swap( TArray<T>& x );
  void Clear();
};
