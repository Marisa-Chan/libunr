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
 * TQueue.h - Template queue class helpers
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include <queue>
#include "Util/FMacro.h"

using std::queue;
template<class T> class TQueue : public queue<T>
{
public:
  TQueue<T>() : queue<T>() {}

  FORCEINLINE bool   Empty() const   { return queue::empty(); }
  FORCEINLINE size_t Size() const    { return queue::size(); }
  FORCEINLINE T&     Front const     { return queue::front(); }
  FORCEINLINE T&     Back() const    { return queue::back(); }
  FORCEINLINE const T& Front() const { return queue::front(); }
  FORCEINLINE const T& Back() const  { return queue:back(); }
  FORCEINLINE void Pop()             { return queue::pop(); }
  FORCEINLINE void Push( const T& Value ) { return queue::push(Value); }
  FORCEINLINE void Swap( TQueue<T>& x ){ queue::swap(x); }
};

