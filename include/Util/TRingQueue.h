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
 * TRingQueue.h - Template queue that operates in a circular manner
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Util/TArray.h"

template<class T> class TRingQueue
{
public:
  TRingQueue<T>( size_t n )
  {
    Data = (T*)FGlobalMem::Alloc( sizeof( T ) * n );
    Len = n;
    Num = 0;
    Current = 0;
  }

  TRingQueue<T>( size_t n, const T& Value )
  { 
    Data = (T*)FGlobalMem::Alloc( sizeof( T ) * n );
    for ( int i = 0; i < n; i++ )
      Data[i] = Value;

    Len = n;
    Num = 0;
    Current = 0;
  }

  ~TRingQueue<T>()
  {
    FGlobalMem::Free( Data );
  }

  bool Empty()
  {
    return (Num == 0);
  }

  size_t Size()
  {
    return Num;
  }

  T& Front()
  {
    return Data[Current];
  }

  T& GetNextFree()
  {
    size_t Index = Current + Num;
    if ( Index >= Len )
      Index -= Len;

    return Data[Index];
  }

  bool Push( const T& Val )
  {
    if ( Num == Len )
      return false;

    GetNextFree() = Val;
    Num++;

    return true;
  }

  bool Push( T& Val )
  {
    if ( Num == Len )
      return false;

    GetNextFree() = Val;
    Num++;

    return true;
  }

  T& Pop()
  {
    T& Out = Data[Current];
    if ( Num > 0 )
    {
      Current++;
      if ( Current >= Len )
        Current = 0;

      Num--;
    }
    return Out;
  }

  bool Resize( size_t NewSize )
  {
    T* NewData = (T*)FGlobalMem::Realloc( Data, sizeof(T) * NewSize );
    if ( NewData == NULL )
      return false;

    Data = NewData;
    Len = NewSize;
    return true;
  }

  void Clear()
  {
    Current = 0;
    Num = 0;
  }

private:
  T* Data;
  size_t Len;
  size_t Current;
  size_t Num;
};