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
 * FUtil.cpp - Utility functions
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/FUtil.h"
#include "Core/USystem.h"
#include "Core/UPackage.h"
#include "stdarg.h"

#define CI_6_BIT_LIMIT  64 - 1
#define CI_13_BIT_LIMIT 8192 - 1
#define CI_20_BIT_LIMIT 1048576 - 1
#define CI_27_BIT_LIMIT 134217728  - 1

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
  
  if( Out.Ver > PKG_VER_UN_220 )
  {
    int len = strlen( Name.Data );
    Out << CINDEX( len );
  }
  
  Out << Name;
  return Out;
}

/*-----------------------------------------------------------------------------
 * FName
-----------------------------------------------------------------------------*/
FName FName::CreateName( const char* InName, int InFlags )
{
  FNameEntry* NewEntry = new FNameEntry( InName, InFlags );
  FName Out = UObject::NameTable.size();
  UObject::NameTable.push_back( NewEntry );
  return Out;
}

const char* FName::Data() const
{
  return UObject::NameTable[Index]->Data;
}

const FHash& FName::Hash() const
{
  return UObject::NameTable[Index]->Hash;
}

bool operator==( FName& A, FName& B )
{
  return UObject::NameTable[A.Index]->Hash == UObject::NameTable[B.Index]->Hash;
}

bool operator!=( FName& A, FName& B )
{
  return UObject::NameTable[A.Index]->Hash != UObject::NameTable[B.Index]->Hash;
}

DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FName& Name )
{
  idx NameIdx;
  In >> CINDEX( NameIdx );
  Name = In.Pkg->GetGlobalName( NameIdx );
  return In;
}

/*-----------------------------------------------------------------------------
 * FString
-----------------------------------------------------------------------------*/
FString& FString::operator+=( const FString& Str )
{
  *(String*)this += Str;
  return *this;
}

FString& FString::operator+=( const String& Str )
{
  *(String*)this += Str;
  return *this;
}

FString& FString::operator+=( const char* s )
{
  *(String*)this += s;
  return *this;
}

FString& FString::operator+=( char c )
{
  *(String*)this += c;
  return *this;
}

FString& FString::operator+=( FName Name )
{
  *(String*)this += Name.Data();
  return *this;
}

bool operator!=( const FString& lhs, const FString& rhs )
{
  return (String&)lhs != (String&)rhs;
}

DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FString& Str )
{
  idx Size = 0;
  In >> CINDEX( Size );

  if ( Size > 0 )
  {
    Str.reserve( Size-1 );
    
    // Slow but secure
    for ( int i = 0; i < Size; i++ )
    {
      char C = '\0';
      In.Read( &C, 1 );

      if ( C == '\0' )
        break;

      Str += C;
    }
  }

  return In;
}

DLL_EXPORT FPackageFileOut& operator<<( FPackageFileOut& Out, FString& Str )
{
  size_t Length = Str.length();
  Out << CINDEX( Length );
  Out.Write( (void*)Str.data(), Length+1 );
  return Out;
}


