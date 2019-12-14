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
 * FThread.h - Helper class for facilitating multi-threaded execution
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Util/FThread.h"

u32 FThread::MasterThreadId = MAX_UINT32;

bool FThread::StaticInit()
{
  if ( MasterThreadId == MAX_UINT32 )
  {
  #if defined LIBUNR_WIN32
    MasterThreadId = GetCurrentThreadId();
  #elif defined LIBUNR_POSIX
    MasterThreadId = 0;
  #endif
  }
}

bool FThread::Run( ThreadReturnType( *ThreadFunc )(void*), void* Args )
{
#if defined LIBUNR_WIN32
  LPTHREAD_START_ROUTINE

  ThreadId = CreateThread( NULL, 0, ThreadFunc, Args, )
#elif defined LIBUNR_POSIX
#endif
}

bool Stop();