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
 * FGuid.cpp - Globally unique identifier implementation
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Util/FGuid.h"

#if defined LIBUNR_WIN32
  #include <Objbase.h>
#elif defined LIBUNR_POSIX
  #if defined __linux__ || __APPLE__
    #include <uuid/uuid.h>
  #elif defined _SYSTYPE_BSD
    #include <uuid.h>
  #endif
#endif

void FGuid::Generate()
{
#if defined LIBUNR_WIN32
  CoCreateGuid( (GUID*)Data );
#elif defined LIBUNR_POSIX
  #if defined __linux__  || __APPLE__
    uuid_generate( Data );
  #elif defined _SYSTYPE_BSD
    u32 Status;
    uuid_create( Data, &Status );
  #else
    #error "FGuid needs a POSIX implementation!"
  #endif
#else
  #error "FGuid needs an implementation!"
#endif
}
