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
 * FName.h - Global name table functionality
 * 
 * Nametable code from GZDoom, written by Randy Heit
 * adapted to libunr by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Util/FHash.h"
#include "Util/TArray.h"

enum EName
{
  #define xx(n) NAME_##n,
  #include "Util/FRegisteredNames.h"
  #undef xx
};

class FPackageFileIn;
class FPackageFileOut;

/*-----------------------------------------------------------------------------
 * FNameEntry
 * An entry into a name table
-----------------------------------------------------------------------------*/
#define NAME_LEN 64
class LIBUNR_API FNameEntry
{
public:
   FNameEntry();
   FNameEntry( const char* InStr, u32 InHash = 0, int InFlags = 0 );
  ~FNameEntry();
  
  friend FPackageFileIn&  operator>>( FPackageFileIn& In,  FNameEntry& Name );
  friend FPackageFileOut& operator<<( FPackageFileOut& In, FNameEntry& Name );

  friend bool operator==( FNameEntry& A, FNameEntry& B );
  friend bool operator!=( FNameEntry& A, FNameEntry& B );
  
  char Data[NAME_LEN];
  u32 Hash;
  u32 NextHash;
  int Flags;
};

/*-----------------------------------------------------------------------------
 * FName
 * An index into the global name table
-----------------------------------------------------------------------------*/
class LIBUNR_API FName
{
public:
  FName() { Index = 0; }
  FName( int Idx ) { Index = Idx; }
  FName( const char* Str, int Flags = 0, bool Create = true ) { Index = NameTableManager.FindName( Str, Flags, Create ); }
  FName( const char* Str, size_t Len, int Flags, bool Create = true ) { Index = NameTableManager.FindName( Str, Len, Flags, Create ); }
  FName( FNameEntry& Entry, bool Create = true ) { Index = NameTableManager.FindName( Entry, Create ); }

  FName( FNameEntry* Entry, bool Create = true ) 
  { 
    if ( Entry )
      Index = NameTableManager.FindName( *Entry, Create );
    else
      Index = NAME_None;
  }
  FName( EName Name ) { Index = Name; }

  static void StaticInit()
  {
    NameTableManager.Init();
  }

  static void StaticExit()
  {
    NameTableManager.Exit();
  }

  static inline bool IsNameNone( FName Name )
  {
    return (*(int*)Name.Data()) == NONE_STR;
  }

  inline operator u32()
  {
    return Index;
  }

  const char* Data() const;
  const u32 Hash() const;
  const FNameEntry* Entry() const;

  inline FName operator=( int Idx )
  {
    Index = Idx;
    return *this;
  }

  friend bool operator==( FName& A, FName& B );
  friend bool operator!=( FName& A, FName& B );
  friend LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FName& Name );

private:
  u32 Index;

  class FNameTableManager
  {
  public:
    FNameTableManager() {}
    ~FNameTableManager() {}

    enum { HASH_SIZE = 1024 };

    TArray<FNameEntry> Entries;
    int Buckets[HASH_SIZE];

    u32 FindName( FNameEntry& Entry, bool Create = true );
    u32 FindName( const char* Text, int Flags = 0, bool Create = true );
    u32 FindName( const char* Text, size_t Len, int Flags = 0, bool Create = true );

    u32 AddName( FNameEntry& Entry, u32 Bucket );
    u32 AddName( const char* Text, size_t Len, u32 Hash, int Flags, u32 Bucket );

    void SetNameFlags( FName Name, int NewFlags );

    void Init();
    void Exit();
  };

  static FNameTableManager NameTableManager;
};


