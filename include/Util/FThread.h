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

/*
 * A note about threading
 * 
 * Many systems in libunr are not thread safe at the moment. FThread is provided
 * such that programmers can enable multi-threading of specific applications
 * (BSP/Light building, music playback, etc). It is up to the programmer to ensure
 * that whatever system they are enhancing with multi-threading is thread safe to
 * begin with.
*/

#include "Util/FMacro.h"
#include "Util/FTypes.h"

#if defined LIBUNR_WIN32
  #include <Windows.h>
  typedef u32 ThreadReturnType;
#elif defined LIBUNR_POSIX
  #include <pthread.h>
  typedef void* ThreadReturnType;
#else
  #error "Unknown operating system! Please add a section for FThread"
#endif

class LIBUNR_API FThread
{
public:
  FThread();

  static bool StaticInit();

  bool Run( ThreadReturnType( *ThreadFunc )(void*), void* Args );
  bool Stop();

private:
  static u32 MasterThreadId;
  u32 ThreadId;
};
