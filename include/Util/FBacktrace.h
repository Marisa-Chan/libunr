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
 * FBacktrace.h - Helper code for getting backtrace info from the stack
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Util/FMacro.h"

#ifdef LIBUNR_POSIX
  #include <ucontext.h>

  LIBUNR_API const char* GetSymbolName( void* Addr );
  LIBUNR_API const char* GetSymbolModule( void* Addr );
  LIBUNR_API void DumpRegisters( mcontext_t* Regs, bool bPrintFpu );
  LIBUNR_API void DumpBacktrace( mcontext_t* Regs );
#endif

