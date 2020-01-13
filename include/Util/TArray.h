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
 * TArray.h - Template array class helpers
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include <vector>
#include "Util/FMacro.h"
#include "Util/FTypes.h"
#include "Util/FFileArchive.h"

using std::vector;
template<class T> class TArray : public vector<T>
{
public:
  TArray<T>() : vector<T>() 
  { ElementSize = sizeof(T); }

  TArray<T>( size_t n ) : vector<T>( n ) 
  { ElementSize = sizeof(T); }
  
  TArray<T>( size_t n, const T& Value ) : vector<T>( n, Value ) 
  { ElementSize = sizeof(T); }

  FORCEINLINE size_t Size() const                     { return vector<T>::size(); }
  FORCEINLINE size_t MaxSize() const                  { return vector<T>::max_size(); }
  FORCEINLINE void Resize( size_t n )                 { vector<T>::resize(n); }
  FORCEINLINE void Resize( size_t n, const T& Value ) { vector<T>::resize(n,Value); }
  FORCEINLINE size_t Capacity()                       { return vector<T>::capacity(); }
  FORCEINLINE bool IsEmpty() const                    { return vector<T>::empty(); }
  FORCEINLINE void Reserve( size_t n )                { vector<T>::reserve(n); }
  FORCEINLINE void Reclaim ()                         { vector<T>::shrink_to_fit(); }
  FORCEINLINE T& At ( size_t n )                      { return vector<T>::at(n); }
  FORCEINLINE T& Front ()                             { return vector<T>::front(); }
  FORCEINLINE T& Back ()                              { return vector<T>::back(); }
  FORCEINLINE T* Data ()                              { return vector<T>::data(); }
  FORCEINLINE void Assign( size_t n, const T& Value ) { vector<T>::assign(n,Value); }
  FORCEINLINE void PushBack( const T& Val )           { vector<T>::push_back(Val); }
  FORCEINLINE void PushBack( T& Val )                 { vector<T>::push_back(Val); }
  FORCEINLINE void PopBack()                          { vector<T>::pop_back(); }
  FORCEINLINE void Swap( TArray<T>& x )               { vector<T>::swap(x); }
  FORCEINLINE void Clear()                            { vector<T>::clear(); }
  FORCEINLINE void Append( TArray<T>& x )
  {
    vector<T>::insert( vector<T>::end(), x.begin(), x.end() );
  }
  FORCEINLINE void Append( T* x, size_t n )
  {
    for ( size_t i = 0; i < n; i++ )
      vector<T>::push_back( x[i] );
  }
  FORCEINLINE T& Erase( size_t Index )
  {
    // Get iterator by doing some math on the array
    typename vector<T>::iterator Out = vector<T>::erase( vector<T>::begin() + Index );
    return *Out;
  }

  friend FPackageFileIn& operator>>( FPackageFileIn& In, TArray<T>& Array )
  {
    idx ArrCount;
    In >> CINDEX( ArrCount );
    Array.Resize( ArrCount );
    for ( int i = 0; i < ArrCount; i++ )
      In >> Array[i];

    return In;
  }

  friend FPackageFileOut& operator>>( FPackageFileOut& Out, TArray<T>& Array )
  {
    Out << CINDEX( Array.Size() );
    for ( int i = 0; i < Array.Size(); i++ )
      Out << Array[i];
  }

  friend FFileArchiveOut& operator>>( FFileArchiveOut& Out, TArray<T>& Array )
  {
    for ( int i = 0; i < Array.Size(); i++ )
      Out << Array[i];
  }

  size_t ElementSize;
};

template<class T> class TArrayNotify : public TArray<T>
{
public:
  TArrayNotify<T>() : TArray<T>() {}
  TArrayNotify<T>( size_t n ) : TArray<T>( n ) {}
  TArrayNotify<T>( size_t n, const T& Value ) : TArray<T>( n, Value ) {}

  typedef void (*NotifyCallback)(int);

protected:
  NotifyCallback Callback;
};

class FGenericArray : public TArray<unsigned char>
{
public:
  FORCEINLINE size_t Size() const    { return vector<unsigned char>::size() / ElementSize; }
  FORCEINLINE size_t MaxSize() const { return vector<unsigned char>::max_size() / ElementSize; }
  FORCEINLINE void   Resize( size_t n )  { vector<unsigned char>::resize( n * ElementSize ); }
  FORCEINLINE void   Reserve( size_t n ) { vector<unsigned char>::reserve( n * ElementSize ); }
  
  FORCEINLINE void* operator[]( size_t n ) { return PtrAdd( Data(), n * ElementSize ); }

private:
  void  Resize( size_t n, const char& Value );
  void  Assign( size_t n, const char& Value );
  void  PushBack( const char& Val );
  void  PushBack( char& Val );
  void  PopBack();
};

