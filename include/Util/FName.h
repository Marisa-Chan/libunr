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
 * FName.h - Global name table functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Util/FHash.h"

class FPackageFileIn;
class FPackageFileOut;
/*-----------------------------------------------------------------------------
 * FNameEntry
 * An entry into a name table
-----------------------------------------------------------------------------*/
#define NAME_LEN 64
class DLL_EXPORT FNameEntry
{
public:
   FNameEntry();
   FNameEntry( const char* InStr, int InFlags = 0 );
  ~FNameEntry();
  
  friend FPackageFileIn&  operator>>( FPackageFileIn& In,  FNameEntry& Name );
  friend FPackageFileOut& operator<<( FPackageFileOut& In, FNameEntry& Name );
  
  char Data[NAME_LEN];
  FHash Hash;
  int Flags;
};

/*-----------------------------------------------------------------------------
 * FName
 * An index into the global name table
-----------------------------------------------------------------------------*/
class DLL_EXPORT FName
{
public:
  u32 Index;

  FName() { Index = 0; }
  FName( int Idx ) { Index = Idx; }

  static FName CreateName( const char* InName, int InFlags );
  static inline bool IsNameNone( FName Name )
  {
    return (*(int*)Name.Data()) == NONE_STR;
  }

  inline operator u32()
  {
    return Index;
  }

  const char* Data() const;
  const FHash& Hash() const;

  inline FName operator=( int Idx )
  {
    Index = Idx;
    return *this;
  }

  friend bool operator==( FName& A, FName& B );
  friend bool operator!=( FName& A, FName& B );
  friend DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FName& Name );
};


