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
 * FHash.h - SuperFastHash implementation
 * 
 * Original implementation by Paul Hsieh
 * http://www.azillionmonkeys.com/qed/hash.html
 *
 * adapted to libunr by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Util/FTypes.h"
#include <string.h>
#include <ctype.h>

LIBUNR_API u32 SuperFastHash( const char* Data, size_t Len );
LIBUNR_API u32 SuperFastHashString( const char* Text, size_t Len = 0 );
