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
 * FName.cpp - Global name table implementation
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <stdarg.h>
#include "Util/FName.h"
#include "Util/FHash.h"
#include "Core/USystem.h"
#include "Core/UPackage.h"

/*-----------------------------------------------------------------------------
 * FNameEntry
-----------------------------------------------------------------------------*/
FNameEntry::FNameEntry()
{
  memset( Data, 0, NAME_LEN );
  Hash = ZERO_HASH;
  Flags = 0;
}

FNameEntry::FNameEntry( const char* InStr, int InFlags )
{
  strncpy( Data, InStr, NAME_LEN );
  Data[NAME_LEN-1] = '\0';
  Flags = InFlags;
  Hash = FnvHashString( Data );
}

FNameEntry::~FNameEntry()
{
}

FPackageFileIn& operator>>( FPackageFileIn& In, FNameEntry& Name )
{
  if( In.Ver <= PKG_VER_UN_220 )
  {
    u8 b;
    char* ptr = Name.Data;
    do
    {
      In >> b;
      *ptr++ = b;
      
    } while( b && ptr < (Name.Data + NAME_LEN ) );
    *ptr = '\0'; // in case we ran up against the name size limit
  }
  else
  {
    int len = 0;
    In >> CINDEX( len );
    if( len > 0 && len < NAME_LEN )
      In.Read( Name.Data, len );
  }
  In >> Name.Flags;
  Name.Hash = FnvHashString( Name.Data );
  return In;
}

FPackageFileOut& operator<<( FPackageFileOut& Out, FNameEntry& Name )
{
  Name.Data[NAME_LEN-1] = '\0'; // just in case
  
  int len = 0;
  if( Out.Ver > PKG_VER_UN_220 )
  {
    len = strlen( Name.Data );
    Out << CINDEX( len );
  }
  
  Out.Write( Name.Data, len );
  return Out;
}

/*-----------------------------------------------------------------------------
 * FName
-----------------------------------------------------------------------------*/
FName FName::CreateName( const char* InName, int InFlags )
{
  TArray<FNameEntry*>* GNameTable = UObject::GetGlobalNameTable();
  FNameEntry* NewEntry = new FNameEntry( InName, InFlags );
  FName Out = GNameTable->Size();
  GNameTable->PushBack( NewEntry );
  return Out;
}

const char* FName::Data() const
{
  return (*UObject::GetGlobalNameTable())[Index]->Data;
}

const FHash& FName::Hash() const
{
  return (*UObject::GetGlobalNameTable())[Index]->Hash;
}

bool operator==( FName& A, FName& B )
{
  return (*UObject::GetGlobalNameTable())[A.Index]->Hash == (*UObject::GetGlobalNameTable())[B.Index]->Hash;
}

bool operator!=( FName& A, FName& B )
{
  return (*UObject::GetGlobalNameTable())[A.Index]->Hash != (*UObject::GetGlobalNameTable())[B.Index]->Hash;
}

LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FName& Name )
{
  idx NameIdx;
  In >> CINDEX( NameIdx );
  Name = In.Pkg->GetGlobalName( NameIdx );
  return In;
}


