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
#include "Core/UObject.h"
#include "Util/FName.h"
#include "Util/FHash.h"
#include "Core/USystem.h"
#include "Core/UPackage.h"

static const char* PredefinedNames[] =
{
  #define xx(n) #n,
  #include "Util/FRegisteredNames.h"
  #undef xx
};

FName::FNameTableManager FName::NameTableManager;

/*-----------------------------------------------------------------------------
 * FNameEntry
-----------------------------------------------------------------------------*/
FNameEntry::FNameEntry()
{
  memset( Data, 0, NAME_LEN );
  Hash = 0;
  Flags = 0;
}

FNameEntry::FNameEntry( const char* InStr, u32 InHash, int InFlags )
{
  Init( InStr, InHash, InFlags );
}

FNameEntry::~FNameEntry()
{
}

void FNameEntry::Init( const char* InStr, u32 InHash, int InFlags )
{
  strncpy( Data, InStr, NAME_LEN );
  Data[NAME_LEN - 1] = '\0';
  Flags = InFlags;
  Hash = (InHash) ? InHash : SuperFastHashString( InStr );
  NextHash = 0;
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
  Name.Hash = SuperFastHashString( Name.Data );
  return In;
}

FPackageFileOut& operator<<( FPackageFileOut& Out, FNameEntry& Name )
{
  Name.Data[NAME_LEN-1] = '\0'; // just in case
  
  u32 len = 0;
  if( Out.Ver > PKG_VER_UN_220 )
  {
    len = (u32)strlen( Name.Data );
    Out << CINDEX( len );
  }
  
  Out.Write( Name.Data, len );
  return Out;
}

bool operator==( FNameEntry& A, FNameEntry& B )
{
  return (A.Hash == B.Hash) && (stricmp( A.Data, B.Data ) == 0);
}

bool operator!=( FNameEntry& A, FNameEntry& B )
{
  return (A.Hash != B.Hash) || (stricmp( A.Data, B.Data ) != 0);
}

/*-----------------------------------------------------------------------------
 * FName
-----------------------------------------------------------------------------*/

const char* FName::Data() const
{
  return NameTableManager.Entries[Index].Data;
}

const u32 FName::Hash() const
{
  return NameTableManager.Entries[Index].Hash;
}

const FNameEntry* FName::Entry() const
{
  return &NameTableManager.Entries[Index];
}

bool operator==( FName& A, FName& B )
{
  return A.Index == B.Index;
}

bool operator!=( FName& A, FName& B )
{
  return A.Index != B.Index;
}

LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FName& Name )
{
  idx NameIdx;
  In >> CINDEX( NameIdx );

  FNameEntry& NameEntry = (In.Pkg->GetNameTable())[NameIdx];
  Name = FName( NameEntry, true );
  return In;
}

/*-----------------------------------------------------------------------------
 * FNameTableManager
-----------------------------------------------------------------------------*/
u32 FName::FNameTableManager::FindName( FNameEntry& Entry, bool Create )
{
  u32 Bucket;
  int Scanner;

  Bucket = Entry.Hash % HASH_SIZE;
  Scanner = Buckets[Bucket];

  while ( Scanner >= 0 )
  {
    if ( Entries[Scanner] == Entry )
      return Scanner;

    Scanner = Entries[Scanner].NextHash;
  }

  if ( !Create )
    return NAME_None;

  return AddName( Entry, Bucket );
}

u32 FName::FNameTableManager::FindName( const char* Text, int Flags, bool Create )
{
  if ( Text == NULL )
    return NAME_None;

  size_t Len = strlen( Text );
  if ( Len == 0 )
    return NAME_None;

  u32 Hash = SuperFastHashString( Text, Len );
  u32 Bucket = Hash % HASH_SIZE;
  int Scanner = Buckets[Bucket];

  while ( Scanner >= 0 )
  {
    if ( Entries[Scanner].Hash == Hash && stricmp( Entries[Scanner].Data, Text ) == 0 )
      return Scanner;

    Scanner = Entries[Scanner].NextHash;
  }

  if ( !Create )
    return NAME_None;

  return AddName( Text, Len, Hash, Flags, Bucket );
}

u32 FName::FNameTableManager::FindName( const char* Text, size_t Len, int Flags, bool Create )
{
  if ( Text == NULL )
    return NAME_None;

  if ( Len == 0 )
    return NAME_None;

  u32 Hash = SuperFastHashString( Text, Len );
  u32 Bucket = Hash % HASH_SIZE;
  int Scanner = Buckets[Bucket];

  while ( Scanner >= 0 )
  {
    if ( Entries[Scanner].Hash == Hash && stricmp( Entries[Scanner].Data, Text ) == 0 )
      return Scanner;

    Scanner = Entries[Scanner].NextHash;
  }

  if ( !Create )
    return NAME_None;

  return AddName( Text, Len, Hash, Flags, Bucket );
}

u32 FName::FNameTableManager::AddName( FNameEntry& Entry, u32 Bucket )
{
  Entry.NextHash = Buckets[Bucket];
  Buckets[Bucket] = (int)Entries.Size();

  Entries.PushBack( Entry );
  return (u32)(Entries.Size() - 1);
}

u32 FName::FNameTableManager::AddName( const char* Text, size_t Len, u32 Hash, int Flags, u32 Bucket )
{
  FNameEntry TmpEntry( Text, Hash, Flags );
  return AddName( TmpEntry, Bucket );
}

void FName::FNameTableManager::SetNameFlags( FName Name, int NewFlags )
{
  FNameEntry& NameEntry = Entries[Name];
  NameEntry.Flags = NewFlags;
}

void FName::FNameTableManager::Init()
{
  memset( Buckets, -1, sizeof( Buckets ) );

  // Add hardcoded names to table
  for ( size_t i = 0; i < ARRAY_SIZE( PredefinedNames ); i++ )
    FindName( PredefinedNames[i], (int)RF_Keep, true );
}

void FName::FNameTableManager::Exit()
{
  Entries.Clear();
}

