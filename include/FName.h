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
 * FName.h - Name table stuff
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __FNAME_H__
#define __FNAME_H__

#include "FArchive.h"
#define NAME_LEN 64

struct FNameEntry
{
   FNameEntry();
   FNameEntry(const char* InStr);
  ~FNameEntry();
  
  friend FArchive& operator>>( FArchive& Ar, FNameEntry& Name );
  friend FArchive& operator<<( FArchive& Ar, FNameEntry& Name );
  
  char Data[NAME_LEN];
  u32 Index;
  int Flags;
};

// this is gonna be pretty unused until loading maps kicks off
struct FName
{
  FName();
  FName( size_t InIndex );
  
  size_t Index;
};

#endif
