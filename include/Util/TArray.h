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
 * TArray.h - Template array class helpers
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include <vector>
#include "Util/FMacro.h"

#define DECLARE_FUNCTION_ALIAS(rettype, newf) \
  FORCEINLINE rettype newf

template<class T> class TArray : public std::vector<T>
{
public:
  TArray<T>() : std::vector<T>() 
  { ElementSize = sizeof(T); }

  TArray<T>( size_t n ) : std::vector<T>( n ) 
  { ElementSize = sizeof(T); }
  
  TArray<T>( size_t n, const T& Value ) : std::vector<T>( n, Value ) 
  { ElementSize = sizeof(T); }

  using std::vector<T>::size;
  using std::vector<T>::max_size;
  using std::vector<T>::resize;
  using std::vector<T>::capacity;
  using std::vector<T>::empty;
  using std::vector<T>::reserve;
  using std::vector<T>::shrink_to_fit;
  using std::vector<T>::at;
  using std::vector<T>::front;
  using std::vector<T>::back;
  using std::vector<T>::data;
  using std::vector<T>::pop_back;
  using std::vector<T>::clear;

  DECLARE_FUNCTION_ALIAS( size_t, Size )()    { return size(); }
  DECLARE_FUNCTION_ALIAS( size_t, MaxSize )() { return max_size(); }
  DECLARE_FUNCTION_ALIAS( void, Resize )( size_t n ) { resize(n); }
  DECLARE_FUNCTION_ALIAS( void, Resize )( size_t n, const T& Value ) { resize(n,Value); }
  DECLARE_FUNCTION_ALIAS( size_t, Capacity )() { return capacity(); }
  DECLARE_FUNCTION_ALIAS( bool, IsEmpty )() { return empty(); }
  DECLARE_FUNCTION_ALIAS( void, Reserve )( size_t n ) { reserve(n); }
  DECLARE_FUNCTION_ALIAS( void, Reclaim )() { shrink_to_fit(); }
  DECLARE_FUNCTION_ALIAS( T&, At )( size_t n ) { return at(n); }
  DECLARE_FUNCTION_ALIAS( T&, Front )() { return front(); }
  DECLARE_FUNCTION_ALIAS( T&, Back )() { return back(); }
  DECLARE_FUNCTION_ALIAS( T*, Data )() { return data(); }
  DECLARE_FUNCTION_ALIAS( void, Assign )( size_t n, const T& Value ) { assign(n,Value); }
  DECLARE_FUNCTION_ALIAS( void, PushBack )( const T& Val ) { push_back(Val); }
  DECLARE_FUNCTION_ALIAS( void, PushBack )( T& Val ) { push_back(Val); }
  DECLARE_FUNCTION_ALIAS( void, PopBack )() { pop_back(); }
  DECLARE_FUNCTION_ALIAS( void, Swap )( TArray<T>& x ) { swap((std::vector<T>&)x); }
  DECLARE_FUNCTION_ALIAS( void, Clear )() { clear(); }

  size_t ElementSize;
};

class FGenericArray : public TArray<unsigned char>
{
public:
  size_t Size() { return std::vector<unsigned char>::size() / ElementSize; }
};
