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

#pragma once

#include "Util/FMacro.h"
#include "Util/FTypes.h"

struct FMemPage
{
  FMemPage* Next;
  FMemPage* Prev;
};

#define LIST_SIGNATURE 0xf0f0f0f0
#define PAGE_SIGNATURE 0x7f7f7f7f
#define BLOC_SIGNATURE 0xa0a1a2a3

#define PAGE_SIZE      4096
#define HUGE_PAGE_SIZE 2097152

enum EStandardAllocType { EStandardAlloc };
enum EPageAllocType     { EPageAlloc };
enum EHugePageAllocType { EHugePageAlloc };

/*-----------------------------------------------------------------------------
 * operator new overloads 
-----------------------------------------------------------------------------*/
void* operator new(size_t n);
void* operator new(size_t n, EStandardAllocType AllocType);
void* operator new(size_t n, EPageAllocType AllocType);
void* operator new(size_t n, EHugePageAllocType AllocType);

void* operator new[](size_t n);
void* operator new[](size_t n, EStandardAllocType AllocType);
void* operator new[](size_t n, EPageAllocType AllocType);
void* operator new[](size_t n, EHugePageAllocType AllocType);

/*-----------------------------------------------------------------------------
 * operator delete overloads
-----------------------------------------------------------------------------*/
void operator delete(void* Ptr);
void operator delete(void* Ptr, EStandardAllocType AllocType);
void operator delete(void* Ptr, EPageAllocType AllocType);
void operator delete(void* Ptr, EHugePageAllocType AllocType);

void operator delete[](void* Ptr);
void operator delete[](void* Ptr, EStandardAllocType AllocType);
void operator delete[](void* Ptr, EPageAllocType AllocType);
void operator delete[](void* Ptr, EHugePageAllocType AllocType);

/*-----------------------------------------------------------------------------
 * FStandardAllocator
 * A memory allocation system that uses standard C functions
-----------------------------------------------------------------------------*/
class FStandardAllocator
{
public:
  static bool Init();
  static bool Exit();

  static void* Alloc( size_t Num );
  static void* Realloc( void* Ptr, size_t Num );
  static void* Calloc( size_t Blocks, size_t Size );
  static void  Free( void* Ptr );
};

/*-----------------------------------------------------------------------------
 * FPoolAllocator
 * An allocation system that divides OS pages into pools of fixed size blocks
-----------------------------------------------------------------------------*/
class FPoolAllocator
{
public:
  static bool Init();
  static bool Exit();

  static void* Alloc( size_t Num );
  static void* Realloc( void* Ptr, size_t Num );
  static void* Calloc( size_t Blocks, size_t Size );
  static void  Free( void* Ptr );
};

/*-----------------------------------------------------------------------------
 * FHugePageAllocator
 * A memory allocation system that breaks up huge 2MB pages for use with 
 * Unreal objects
-----------------------------------------------------------------------------*/
class FHugePageAllocator
{
public:
  static bool Init();
  static bool Exit();

  static void* Alloc( size_t Num );
  static void* Realloc( void* Ptr, size_t Num );
  static void* Calloc( size_t Blocks, size_t Size );
  static void  Free( void* Ptr );
};

/*-----------------------------------------------------------------------------
 * FStackAllocator
 * A memory allocation system that allocates memory in a linear fashion
 * Meant for allocations that are cleared every tick
-----------------------------------------------------------------------------*/
class FStackAllocator
{
public:
  bool Init( size_t InSize );
  bool Exit();

  void FreeAll();

  void* Alloc( size_t Num );
  void  Free( void* Ptr );

private:
  bool CreateStack( size_t Size );
  bool ExtendStack( size_t NewSize );
  bool FreeStack();

  void* BasePtr;
  void* CurrentPtr;
  size_t StackSize;
};

/*-----------------------------------------------------------------------------
 * FGlobalMem
 * The memory allocation system that is used for all general allocations
-----------------------------------------------------------------------------*/
class LIBUNR_API FGlobalMem
{
public:
  static void* (*Alloc)( size_t Num );
  static void* (*Realloc)( void* Ptr, size_t Num );
  static void* (*Calloc)( size_t Blocks, size_t Size );
  static void  (*Free)( void* Ptr );
};

/*-----------------------------------------------------------------------------
 * FObjectMem
 * The memory allocation system that is used for all Unreal objects
-----------------------------------------------------------------------------*/
class LIBUNR_API FObjectMem
{
public:
  static void* (*Alloc)(size_t Num);
  static void* (*Realloc)(void* Ptr, size_t Num);
  static void* (*Calloc)(size_t Blocks, size_t Size);
  static void  (*Free)(void* Ptr);
};
