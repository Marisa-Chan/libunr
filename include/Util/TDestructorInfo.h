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
 * TDestructorInfo.h - Helpers for determining type destructability
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Util/FTypes.h"

// For determining if a type needs a destructor called or not
template<class T> struct TDestructorInfo   { static FORCEINLINE bool NeedsDestructor() { return true; } };
template <> struct TDestructorInfo<u8>     { static FORCEINLINE bool NeedsDestructor() { return false; } };
template <> struct TDestructorInfo<u16>    { static FORCEINLINE bool NeedsDestructor() { return false; } };
template <> struct TDestructorInfo<u32>    { static FORCEINLINE bool NeedsDestructor() { return false; } };
template <> struct TDestructorInfo<u64>    { static FORCEINLINE bool NeedsDestructor() { return false; } };
template <> struct TDestructorInfo<i8>     { static FORCEINLINE bool NeedsDestructor() { return false; } };
template <> struct TDestructorInfo<i16>    { static FORCEINLINE bool NeedsDestructor() { return false; } };
template <> struct TDestructorInfo<i32>    { static FORCEINLINE bool NeedsDestructor() { return false; } };
template <> struct TDestructorInfo<i64>    { static FORCEINLINE bool NeedsDestructor() { return false; } };
template <> struct TDestructorInfo<float>  { static FORCEINLINE bool NeedsDestructor() { return false; } };
template <> struct TDestructorInfo<double> { static FORCEINLINE bool NeedsDestructor() { return false; } };
