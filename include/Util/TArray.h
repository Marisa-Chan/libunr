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
#include <new>
#include <string.h>
#include "Util/FMacro.h"
#include "Util/FTypes.h"
#include "Util/FFileArchive.h"
#include "Util/TDestructorInfo.h"

class FPackageFileIn;
class FPackageFileOut;

template<class T> class TArray
{
public:
  TArray<T>() 
  { 
    ElementSize = sizeof(T);
    NumBytes = 0;
    NumElements = 0;
    NumReserved = 0;
    Array = NULL;
  }

  TArray<T>( size_t n )
  { 
    ElementSize = sizeof(T); 
    NumBytes = n * sizeof( T );
    NumElements = n;
    NumReserved = n * 2;
    Array = (T*)malloc( NumReserved * sizeof(T) );
    memset( Array, 0, NumReserved * sizeof( T ) );
  }
  
  TArray<T>( size_t n, const T& Value )
  { 
    ElementSize = sizeof(T);
    NumBytes = n * sizeof( T );
    NumElements = n;
    NumReserved = n * 2;
    Array = (T*)malloc( NumReserved * sizeof(T) );
    
    for ( int i = 0; i < n; i++ )
      Array[i] = Value;
  }

  TArray<T>( const T* Ptr, size_t n )
  {
    ElementSize = sizeof( T );
    NumBytes = n * sizeof( T );
    NumElements = n;
    NumReserved = n * 2;
    Array = (T*)malloc( NumReserved * sizeof( T ) );

    memcpy( Array, Ptr, n * sizeof( T ) );
  }

  TArray<T>( TArray<T>& x )
  {
    ElementSize = sizeof( T );
    NumBytes = x.NumBytes;
    NumElements = x.NumElements;
    NumReserved = x.NumReserved;
    Array = (T*)malloc( NumReserved * sizeof( T ) );

    for ( int i = 0; i < NumElements; i++ )
      Array[i] = x[i];
  }

  TArray<T>( const TArray<T>& x )
  {
    ElementSize = sizeof( T );
    NumBytes = x.NumBytes;
    NumElements = x.NumElements;
    NumReserved = x.NumReserved;
    Array = (T*)malloc( NumReserved * sizeof( T ) );

    for ( int i = 0; i < NumElements; i++ )
      Array[i] = x[i];
  }

  TArray<T>& operator=( const TArray<T>& Rhs )
  {
    ElementSize = sizeof( T );
    NumBytes = Rhs.NumBytes;
    NumElements = Rhs.NumElements;
    NumReserved = Rhs.NumReserved;
    Array = (T*)malloc( NumReserved * sizeof( T ) );

    for ( int i = 0; i < NumElements; i++ )
      Array[i] = Rhs[i];

    return *this;
  }

  FORCEINLINE size_t Size() const                     
  { 
    return NumElements;
  }

  FORCEINLINE size_t MaxSize() const                  
  { 
    return MAX_UINT64 / sizeof( T );
  }

  FORCEINLINE void Resize( size_t n )                 
  { 
    Reserve( n );
    NumBytes = n * sizeof( T );

    if ( n > NumElements )
    {
      if ( TDestructorInfo<T>::NeedsDestructor() )
        for ( int i = NumElements; i < n; i++ )
          new (&Array[i]) T();
      else
        memset( &Array[NumElements], 0, (n - NumElements) * sizeof( T ) );
    }

    NumElements = n;
  }

  FORCEINLINE void Resize( size_t n, const T& Value ) 
  { 
    Reserve( n );

    if ( n > NumElements )
    {
      for ( int i = NumElements; i < n; i++ )
        Array[i] = Value;
    }
    NumBytes = n * sizeof( T );
    NumElements = n;
    NumReserved = n;
  }

  FORCEINLINE size_t Capacity()
  {
    return NumReserved;
  }

  FORCEINLINE bool IsEmpty() const                    
  { 
    return (NumElements == 0);
  }

  FORCEINLINE void Reserve( size_t n )                
  { 
    // Call destructors if needed
    if ( TDestructorInfo<T>::NeedsDestructor() && n < NumElements )
    {
      for ( int i = n; i < NumElements; i++ )
        Array[i].~T();

      NumElements = n;
    }

    T* NewArray = NULL;
    if ( n == 0 )
    {
      free( Array );
    }
    else
    {
      NewArray = (T*)realloc( Array, n * sizeof( T ) );
      if ( NewArray == NULL )
      {
        // throw an error here if we fail
        return;
      }
    }

    Array = NewArray;
    NumReserved = n;
  }

  FORCEINLINE void Reclaim ()                         
  { 
    Reserve( NumElements );
  }

  FORCEINLINE T& At ( size_t n ) const
  { 
#ifdef LIBUNR_DEBUG
    if ( n >= NumElements )
      DEBUGBREAK();
#endif
    return Array[n];
  }

  FORCEINLINE T& operator[]( size_t n ) const
  {
#ifdef LIBUNR_DEBUG
    if ( n >= NumElements )
      DEBUGBREAK();
#endif
    return Array[n];
  }

  FORCEINLINE T& Front ()
  { 
  #ifdef LIBUNR_DEBUG
    if ( NumElements == 0 )
      DEBUGBREAK();
  #endif
    return Array[0];
  }

  FORCEINLINE T& Back ()
  { 
  #ifdef LIBUNR_DEBUG
    if ( NumElements == 0 )
      DEBUGBREAK();
  #endif
    return Array[NumElements - 1];
  }

  FORCEINLINE const T& Front() const
  { 
  #ifdef LIBUNR_DEBUG
    if ( NumElements == 0 )
      DEBUGBREAK();
  #endif
    return Array[0];
  }

  FORCEINLINE const T& Back() const
  { 
  #ifdef LIBUNR_DEBUG
    if ( NumElements == 0 )
      DEBUGBREAK();
  #endif
    return Array[NumElements - 1];
  }

  FORCEINLINE T* Data ()
  { 
    return Array;
  }

  FORCEINLINE const T* Data() const
  {
    return Array;
  }

  FORCEINLINE void Assign( size_t n, const T& Value ) 
  { 
  #ifdef LIBUNR_DEBUG
    if ( n >= NumElements )
      DEBUGBREAK();
  #endif
    Array[n] = Value;
  }

  FORCEINLINE void PushBack( const T& Val )
  { 
    if ( !Array || NumElements == NumReserved )
      Reserve( (NumReserved * 2) + 4 );

    NumBytes += sizeof( T );
    Array[NumElements] = Val;
    NumElements++;
  }

  FORCEINLINE void PushBack( T& Val )
  {
    if ( !Array || NumElements == NumReserved )
      Reserve( (NumReserved * 2) + 4 );

    NumBytes += sizeof( T );
    Array[NumElements] = Val;
    NumElements++;
  }

  FORCEINLINE void PopBack()
  { 
    if ( NumElements > 0 )
    {
      NumBytes -= sizeof( T );
      NumElements--;
      if ( TDestructorInfo<T>::NeedsDestructor() )
        Array[NumElements].~T();
    }
  }

  FORCEINLINE void Swap( TArray<T>& x )
  { 
    T* TmpArray = Array;
    size_t TmpBytes = NumBytes;
    size_t TmpElements = NumElements;
    size_t TmpReserved = NumReserved;

    Array = x.Array;
    NumBytes = x.NumBytes;
    NumElements = x.NumElements;
    NumReserved = x.NumReserved;

    x.Array = TmpArray;
    x.NumBytes = TmpBytes;
    x.NumElements = TmpElements;
    x.NumReserved = TmpReserved;
  }

  FORCEINLINE void Clear()
  { 
    if ( TDestructorInfo<T>::NeedsDestructor() )
    {
      for ( int i = 0; i < NumElements; i++ )
        Array[i].~T();
    }

    NumElements = 0;
    NumBytes = 0;
  }

  FORCEINLINE void Append( TArray<T>& x )
  {
    if ( !Array || x.NumElements > NumReserved - NumElements )
      Reserve( (NumReserved + x.NumElements) * 2 );

    for ( int i = 0; i < x.NumElements; i++ )
      Array[i + NumElements] = x[i];

    NumBytes += x.NumElements * sizeof( T );
    NumElements += x.NumElements;
  }

  FORCEINLINE void Append( T* x, size_t n )
  {
    if ( !Array || n > NumReserved - NumElements )
      Reserve( ( NumReserved + n ) * 2 );

    for ( int i = 0; i < n; i++ )
      Array[i + NumElements] = x[i];

    NumBytes += n * sizeof( T );
    NumElements += n;
  }

  FORCEINLINE void Append( const T* x, size_t n )
  {
    if ( !Array || n > NumReserved - NumElements )
      Reserve( (NumReserved + n) * 2 );

    for ( int i = 0; i < n; i++ )
      Array[i + NumElements] = x[i];

    NumBytes += n * sizeof( T );
    NumElements += n;
  }

  FORCEINLINE void Erase( size_t Index )
  {
  #ifdef LIBUNR_DEBUG
    if ( Index >= NumElements )
      DEBUGBREAK();
  #endif

    // Destroy selected index
    if ( TDestructorInfo<T>::NeedsDestructor() )
      Array[Index].~T();

    // Copy everything backwards by one
    NumBytes -= sizeof( T );
    NumElements--;
    
    if ( Index < NumElements )
      memmove( &Array[Index], &Array[Index + 1], ElementSize * (NumElements - Index) );
  }

  FORCEINLINE void Erase( size_t Index, size_t Num )
  {
  #ifdef LIBUNR_DEBUG
    if ( Index >= NumElements )
      DEBUGBREAK();
  #endif

    NumBytes -= Num * sizeof( T );
    NumElements -= Num;

    if ( Index < NumElements )
      memmove( &Array[Index], &Array[Index + Num], ElementSize * (NumElements - Index) );
  }

  FORCEINLINE void Insert( T& x, size_t Pos )
  {
    if ( NumElements + 1 >= NumReserved )
      Reserve( (NumElements * 2) + 4 );

    // Copy everything forward by one
    NumBytes += sizeof( T );
    memmove( &Array[Pos + 1], &Array[Pos], ElementSize * (NumElements - Pos) );
    NumElements++;
    Array[Pos] = x;
  }

  FORCEINLINE void Insert( TArray<T>& x, size_t Pos )
  {
    if ( NumElements + x.NumElements >= NumReserved )
      Reserve( (NumElements + x.NumElements) * 2 );

    NumBytes += x.NumElements * sizeof( T );
    memmove( &Array[Pos + x.NumElements], &Array[Pos], ElementSize * (NumElements - Pos) );
    NumElements += x.NumElements;
    memcpy( &Array[Pos], x.Array, ElementSize * x.NumElements );
  }

  FORCEINLINE void Insert( const T* x, size_t n, size_t Pos )
  {
    if ( NumElements + n >= NumReserved )
      Reserve( (NumElements + n) * 2 );

    NumBytes += n * sizeof( T );
    memmove( &Array[Pos + n], &Array[Pos], ElementSize * (NumElements - Pos) );
    NumElements += n;
    memcpy( &Array[Pos], x, ElementSize * n );
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

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, TArray<T>& Array )
  {
    Out << CINDEX( Array.NumElements );
    for ( int i = 0; i < Array.NumElements; i++ )
      Out << Array[i];
    return Out;
  }

  friend FFileArchiveOut& operator<<( FFileArchiveOut& Out, TArray<T>& Array )
  {
    for ( int i = 0; i < Array.NumElements; i++ )
      Out << Array[i];
    return Out;
  }

  size_t ElementSize;
protected:
  T* Array;
  size_t NumBytes;
  size_t NumElements;
  size_t NumReserved;
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
  FORCEINLINE size_t Size() const    { return NumBytes / ElementSize; }
  FORCEINLINE void   Resize( size_t n )  { Resize( n * ElementSize ); }
  FORCEINLINE void   Reserve( size_t n ) { Resize( n * ElementSize ); }
  
  FORCEINLINE void* operator[]( size_t n ) { return PtrAdd( Data(), n * ElementSize ); }

private:
  void  Resize( size_t n, const char& Value );
  void  Assign( size_t n, const char& Value );
  void  PushBack( const char& Val );
  void  PushBack( char& Val );
  void  PopBack();
};

