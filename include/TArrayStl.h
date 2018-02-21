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
 * TArrayStl.h - C++ STL "implementation" of TArray
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FUtil.h"
#include <vector>

template <class T> class TArray
{
private:
  std::vector<T> RealArray;
  
public:
  typedef typename std::vector<T>::iterator Iterator;
  typedef typename std::vector<T>::reverse_iterator ReverseIterator;
  
  TArray<T>() { new ( &RealArray ) std::vector<T>(); }
  TArray<T>( size_t n ) { new ( &RealArray ) std::vector<T>( n ); }
  TArray<T>( size_t n, const T& Value ) { new ( &RealArray ) std::vector<T>( n, Value ); }
  TArray<T>( const TArray<T>& x ) { new ( &RealArray ) std::vector<T>( x ); }
  TArray<T>( TArray<T>&& x ) { new ( &RealArray ) std::vector<T>( x ); }
  
  Iterator Begin() { return RealArray.begin(); }
  Iterator End() { return RealArray.end(); }
  ReverseIterator RBegin() { return RealArray.rbegin(); }
  ReverseIterator REnd() { return RealArray.rend(); }
  
  const Iterator CBegin() const { return RealArray.cbegin(); }
  const Iterator CEnd() const { return RealArray.cend(); }
  const ReverseIterator CRBegin() const { return RealArray.crbegin(); }
  const ReverseIterator CREnd() const { return RealArray.crend(); }
  
  size_t Size() { return RealArray.size(); }
  size_t MaxSize() { return RealArray.max_size(); }
  void   Resize( size_t n ) { RealArray.resize( n ); }
  void   Resize( size_t n, const T& Value ) { RealArray.resize( n, Value ); }
  size_t Capacity() { return RealArray.capacity(); }
  bool   IsEmpty() { return RealArray.empty(); }
  void   Reserve( size_t n ) { RealArray.reserve( n ); }
  void   Reclaim() { RealArray.shrink_to_fit(); }
  
  T& operator[]( size_t n ) { return RealArray[n]; }
  T& At( size_t n ) { return RealArray.at( n ); }
  T& Front() { return RealArray.front(); }
  T& Back() { return RealArray.back(); }
  T* Data() { return RealArray.data(); }
  
  const T& operator[]( size_t n ) const { return RealArray[n]; }
  const T& At( size_t n ) const { return RealArray.at( n ); }
  const T& Front() const { return RealArray.front(); }
  const T& Back() const { return RealArray.back(); }
  const T* Data() const { return RealArray.data(); }
  
  void Assign( size_t n, const T& Value ) { RealArray.assign( n, Value ); }
  void PushBack( const T& Val ) { RealArray.push_back( Val ); }
  void PushBack( T& Val ) { RealArray.push_back( Val ); }
  void PopBack() { RealArray.pop_back(); }
  
  Iterator Insert( const Iterator Pos, const T& Value ) { return RealArray.insert( Pos, Value ); }
  Iterator Insert( const Iterator Pos, size_t n, const T& Value ) { return RealArray.insert( Pos, n, Value ); }
  Iterator Insert( const Iterator Pos, T&& Value ) { return RealArray.insert( Pos, Value ); }
  
  Iterator Erase( const Iterator Pos ) { return RealArray.erase( Pos ); }
  Iterator Erase( const Iterator First, const Iterator Last ) { return RealArray.erase( First, Last ); }
  
  void Swap( TArray<T>& x ) { RealArray.swap( x ); }
  void Clear() { RealArray.clear(); }
};
