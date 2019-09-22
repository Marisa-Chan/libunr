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
 * TStack.h - Template stack class helpers
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include <stack>
#include "Util/FMacro.h"

using std::stack;
template<class T> class TStack : public stack<T>
{
public:
  TStack<T>() : stack<T>() {}

  FORCEINLINE bool   Empty() const          { return stack<T>::empty(); }
  FORCEINLINE void   Pop()                  { stack<T>::pop(); }
  FORCEINLINE void   Push( const T& Value ) { stack<T>::push(Value); }
  FORCEINLINE size_t Size() const           { return stack<T>::size(); }
  FORCEINLINE void Swap( TStack<T>& x )     { stack<T>::swap(x); }
  FORCEINLINE T& Top()                      { return stack<T>::top(); }
  FORCEINLINE const T& Top() const          { return stack<T>::top(); }
};

