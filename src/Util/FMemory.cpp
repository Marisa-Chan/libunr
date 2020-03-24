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
 * FMemory.h - Memory allocators
 *
 * Written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <stdlib.h>
#include "Util/FLogFile.h"
#include "Util/FMemory.h"

#if defined LIBUNR_WIN32
  #include <Windows.h>
#elif defined LIBUNR_POSIX
#endif

/*-----------------------------------------------------------------------------
 * operator new overloads
-----------------------------------------------------------------------------*/
void* operator new(size_t n) { return FGlobalMem::Alloc( n ); }
void* operator new(size_t n, EStandardAllocType AllocType) { return FStandardAllocator::Alloc( n ); }
void* operator new(size_t n, EPageAllocType AllocType) { return FPoolAllocator::Alloc( n ); }
void* operator new(size_t n, EHugePageAllocType AllocType) { return FHugePageAllocator::Alloc( n ); }

void* operator new[]( size_t n ) { return FGlobalMem::Alloc( n ); }
void* operator new[]( size_t n, EStandardAllocType AllocType ) { return FStandardAllocator::Alloc( n ); }
void* operator new[]( size_t n, EPageAllocType AllocType ) { return FPoolAllocator::Alloc( n ); }
void* operator new[]( size_t n, EHugePageAllocType AllocType ) { return FHugePageAllocator::Alloc( n ); }

/*-----------------------------------------------------------------------------
 * operator delete overloads
-----------------------------------------------------------------------------*/
void operator delete(void* Ptr) { FGlobalMem::Free( Ptr ); }
void operator delete(void* Ptr, EStandardAllocType AllocType) { FStandardAllocator::Free( Ptr ); }
void operator delete(void* Ptr, EPageAllocType AllocType) { FPoolAllocator::Free( Ptr ); }
void operator delete(void* Ptr, EHugePageAllocType AllocType) { FHugePageAllocator::Free( Ptr ); }

void operator delete[](void* Ptr) { FGlobalMem::Free( Ptr ); }
void operator delete[](void* Ptr, EStandardAllocType AllocType) { FStandardAllocator::Free( Ptr ); }
void operator delete[](void* Ptr, EPageAllocType AllocType) { FPoolAllocator::Free( Ptr ); }
void operator delete[](void* Ptr, EHugePageAllocType AllocType) { FHugePageAllocator::Free( Ptr ); }

/*-----------------------------------------------------------------------------
 * FStandardAllocator
-----------------------------------------------------------------------------*/
bool FStandardAllocator::Init()
{
  return true;
}

bool FStandardAllocator::Exit()
{
  return true;
}

void* FStandardAllocator::Alloc( size_t Num )
{
  return malloc( Num );
}

void* FStandardAllocator::Realloc( void* Ptr, size_t Num )
{
  return realloc( Ptr, Num );
}

void* FStandardAllocator::Calloc( size_t Blocks, size_t Num )
{
  return calloc( Blocks, Num );
}

void FStandardAllocator::Free( void* Ptr )
{
  free( Ptr );
}

/*-----------------------------------------------------------------------------
 * FPoolAllocator
-----------------------------------------------------------------------------*/
bool FPoolAllocator::Init()
{
  return true;
}

bool FPoolAllocator::Exit()
{
  return true;
}

void* FPoolAllocator::Alloc( size_t Num )
{
  return NULL;
}

void* FPoolAllocator::Realloc( void* Ptr, size_t Num )
{
  return NULL;
}

void* FPoolAllocator::Calloc( size_t Blocks, size_t Num )
{
  return Alloc( Blocks * Num );
}

void FPoolAllocator::Free( void* Ptr )
{
}

/*-----------------------------------------------------------------------------
 * FHugePageAllocator
-----------------------------------------------------------------------------*/
bool FHugePageAllocator::Init()
{
  return true;
}

bool FHugePageAllocator::Exit()
{
  return true;
}

void* FHugePageAllocator::Alloc( size_t Num )
{
  return NULL;
}

void* FHugePageAllocator::Realloc( void* Ptr, size_t Num )
{
  return NULL;
}

void* FHugePageAllocator::Calloc( size_t Blocks, size_t Size )
{
  return NULL;
}

void FHugePageAllocator::Free( void* Ptr )
{
}

/*-----------------------------------------------------------------------------
 * FStackAllocator
-----------------------------------------------------------------------------*/
bool FStackAllocator::Init( size_t InSize )
{
  return CreateStack( PAGE_SIZE );
}

bool FStackAllocator::Exit()
{
  return FreeStack();
}

void* FStackAllocator::Alloc( size_t Num )
{
  if ( (size_t)PtrSubtract( CurrentPtr, BasePtr ) + Num >= StackSize )
    return NULL;

  void* Ptr = CurrentPtr;
  CurrentPtr = PtrAdd( CurrentPtr, Num );
  return Ptr;
}

void FStackAllocator::Free( void* Ptr )
{
}

bool FStackAllocator::CreateStack( size_t Size )
{
  return false;
}

bool FStackAllocator::ExtendStack( size_t NewSize )
{
  return false;
}

bool FStackAllocator::FreeStack()
{
  return false;
}

/*-----------------------------------------------------------------------------
 * Static declarations
-----------------------------------------------------------------------------*/
LIBUNR_API void* (*FGlobalMem::Alloc)(size_t) = FStandardAllocator::Alloc;
LIBUNR_API void* (*FGlobalMem::Realloc)(void*, size_t) = FStandardAllocator::Realloc;
LIBUNR_API void* (*FGlobalMem::Calloc)(size_t, size_t) = FStandardAllocator::Calloc;
LIBUNR_API void  (*FGlobalMem::Free)(void*) = FStandardAllocator::Free;

LIBUNR_API void* (*FObjectMem::Alloc)(size_t) = NULL;
LIBUNR_API void* (*FObjectMem::Realloc)(void*, size_t) = NULL;
LIBUNR_API void* (*FObjectMem::Calloc)(size_t, size_t) = NULL;
LIBUNR_API void  (*FObjectMem::Free)(void*) = NULL;
