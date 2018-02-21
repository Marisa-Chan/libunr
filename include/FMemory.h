/*========================================================================*\
|*  This file is part of libunr.                                          *|
|*                                                                        *|
|*  libunr is free software: you can redistribute it and/or modify        *|
|*  it under the terms of the GNU General Public License as published by  *|
|*  the Free Software Foundation, either version 3 of the License, or     *|
|*  (at your option) any later version.                                   *|
|*                                                                        *|
|*  libunr is distributed in the hope that it will be useful,             *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *|
|*  GNU General Public License for more details.                          *|
|*                                                                        *|
|*  You should have received a copy of the GNU General Public License     *|
|*  along with libunr.  If not, see <http://www.gnu.org/licenses/>.       *|
|*                                                                        *|
\*========================================================================*/

/*========================================================================
 * FMemory.h - Class for memory based operations
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __FMEMORY_H__
#define __FMEMORY_H__

#include "FUtil.h"

// TODO: Implement some way of swapping memory allocators at compile time
class FMemory
{
public:
  // Allocation
  // These functions should have enforcement with regard to invalid parameters
  static void* Malloc (size_t Num);
  static void* Realloc(void* Ptr, size_t Num);
  static void* Calloc (size_t Num, size_t Size);
  static void  Free   (void* Ptr);
  
  // Manipulation
  static void* Copy(void* Dest, size_t DestLen, const void* Src, size_t Num);
  static void* Move(void* Dest, size_t DestLen, const void* Src, size_t Num);
  static void* Set (void* Ptr, u8 Value, size_t Num);
  
  // Compare
  static bool Compare(const void* Ptr1, const void* Ptr2, size_t Num);
  static const void* Search(const void* Ptr, u8 Value, size_t Num);
  static void* Search(void* Ptr, u8 Value, size_t Num);
};

#endif
