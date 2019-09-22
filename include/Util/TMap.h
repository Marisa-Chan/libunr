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
 * TMap.h - Template map class helpers
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include <map>
#include "Util/FMacro.h"

using std::map;
template <
  class Key,
  class T,
  class Compare = std::less<Key>,
  class Allocator = std::allocator<std::pair<const Key, T>>
> class TMap : public map<Key,T,Compare,Allocator>
{
public:
  typedef map<Key,T,Compare,Allocator> MapParent; 
  TMap<Key,T,Compare,Allocator>() : MapParent() {}

  FORCEINLINE bool   Empty()   { return MapParent::empty(); }
  FORCEINLINE size_t Size()    { return MapParent::size(); }
  FORCEINLINE size_t MaxSize() { return MapParent::max_size(); }

  FORCEINLINE T& At( const Key& K ) { return MapParent::At(K); }
  FORCEINLINE const T& At( const Key& K ) const { return MapParent::At(K); }

  void Insert( const T& Val, size_t Position = MAX_SIZE )
  {
    if ( Position == MAX_SIZE )
    {
      MapParent::insert( Val );
    }
    else
    {
      typename MapParent::iterator It;
      size_t i;
      for ( It = MapParent::begin(), i = 0; i < Position; i++, It++ );
      MapParent::insert( It, Val );
    }
  }
 
  size_t Erase( size_t Position )
  {
    typename MapParent::iterator It, PrevIt;
    size_t i;
    for ( It = MapParent::begin(), i = 0; i < Position; i++, It++ )
    {
      if ( PrevIt == MapParent::end() )
        return MAX_SIZE;

      PrevIt = It;
    }
    MapParent::erase( It );
    return i;
  }

  size_t Erase( const Key& K ) { return MapParent::erase(K); }

  size_t Erase( size_t First, size_t Last )
  {
    if ( Last > First )
      return 0;

    typename MapParent::iterator FirstIt, PrevFirstIt;
    typename MapParent::iterator LastIt,  PrevLastIt;
    size_t i;
    for ( FirstIt = MapParent::begin(), i = 0; i < First; i++, FirstIt++ )
    {
      if ( PrevFirstIt == MapParent::end() )
        return 0;

      PrevFirstIt = FirstIt;
    }

    for ( LastIt = FirstIt, i = 0; i < Last; i++, LastIt++ )
    {
      if ( PrevLastIt == MapParent::end() )
        return 0;

      PrevLastIt = LastIt;
    }

    MapParent::erase( FirstIt, LastIt );
    return Last - First;
  }

  FORCEINLINE void Swap( TMap<Key,T,Compare,Allocator>& InMap ) { return MapParent::swap(InMap); }
  FORCEINLINE void Clear() { return MapParent::clear(); }
};

