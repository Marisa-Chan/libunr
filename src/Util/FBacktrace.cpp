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
 * FBacktrace.h - Helper code for getting backtrace info from the stack
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <stddef.h>
#include <string.h>
#include "Util/FLogFile.h"
#include "Util/FBacktrace.h"

#if defined LIBUNR_POSIX
  #include <dlfcn.h>
  const char* GetSymbolName( void* Addr )
  {
    Dl_info Info;
    if (dladdr( Addr, &Info ) == 0)
      return NULL;
  
    return Info.dli_sname;
  }
  
  const char* GetSymbolModule( void* Addr )
  {
    Dl_info Info;
    if (dladdr( Addr, &Info ) == 0)
      return NULL;
  
    return Info.dli_fname;
  }
  void DumpRegisters( mcontext_t* Regs, bool bPrintFpu )
  {
    GLogf( LOG_CRIT, "-------------------------------------\n");
    GLogf( LOG_CRIT, "REGISTERS:\n");
    GLogf( LOG_CRIT, "-------------------------------------\n");
  #if defined __x86_64__
    GLogf( LOG_CRIT, "RIP: 0x%016lx\n", Regs->gregs[REG_RIP]);
    GLogf( LOG_CRIT, "RSP: 0x%016lx  RBP: 0x%016lx\n", Regs->gregs[REG_RSP], Regs->gregs[REG_RBP]);
    GLogf( LOG_CRIT, "RAX: 0x%016lx  RBX: 0x%016lx\n", Regs->gregs[REG_RAX], Regs->gregs[REG_RBX]);
    GLogf( LOG_CRIT, "RCX: 0x%016lx  RDX: 0x%016lx\n", Regs->gregs[REG_RCX], Regs->gregs[REG_RDX]);
    GLogf( LOG_CRIT, "RSI: 0x%016lx  RDI: 0x%016lx\n", Regs->gregs[REG_RSI], Regs->gregs[REG_RDI]);
    GLogf( LOG_CRIT, "R8:  0x%016lx  R9:  0x%016lx\n", Regs->gregs[REG_R8],  Regs->gregs[REG_R9] );
    GLogf( LOG_CRIT, "R10: 0x%016lx  R11: 0x%016lx\n", Regs->gregs[REG_R10], Regs->gregs[REG_R11]);
    GLogf( LOG_CRIT, "R12: 0x%016lx  R13: 0x%016lx\n", Regs->gregs[REG_R12], Regs->gregs[REG_R13]);
    GLogf( LOG_CRIT, "R14: 0x%016lx  R15: 0x%016lx\n", Regs->gregs[REG_R14], Regs->gregs[REG_R15]);
  #endif
    GLogFile->Flush();
  }
  void DumpBacktrace( mcontext_t* Regs )
  {
    int i = 0;
    GLogf( LOG_CRIT, "-------------------------------------\n");
    GLogf( LOG_CRIT, "BACKTRACE:\n");
    GLogf( LOG_CRIT, "-------------------------------------\n");
    GLogFile->Flush();

    // Print PC
    void* PC = (void*)-1;

  #if defined __x86_64__
    PC = (void*)Regs->gregs[REG_RIP];
  #endif

    const char* FuncName = GetSymbolName(PC);
    const char* ExecName = GetSymbolModule(PC);

    GLogf( LOG_CRIT, "[%i] %s (%s) [%p]\n", i++, FuncName, ExecName, PC);
    GLogFile->Flush();

  #if defined __x86_64__
    uint64_t rbp = Regs->gregs[REG_RBP];
    void* ret = *(void**)(rbp+8);

    while ( strcmp( FuncName, "main" ) )
    {
      FuncName = GetSymbolName(ret);
      ExecName = GetSymbolModule(ret);
      GLogf( LOG_CRIT, "[%i] %s (%s) [%p]\n", i++, FuncName, ExecName, ret);
      GLogFile->Flush();
      rbp = *(uint64_t*)rbp;
      ret = *(void**)(rbp+8);
    } 
  #endif
    GLogf( LOG_CRIT, "-------------------------------------\n");
    GLogf( LOG_CRIT, "END BACKTRACE\n");
    GLogf( LOG_CRIT, "-------------------------------------\n");
    GLogFile->Flush();
}
#elif defined LIBUNR_WIN32
#else
  #error "Backtracing code cannot get symbols"
#endif


