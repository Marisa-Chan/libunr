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

#define MAX_MSG_LEN  512
#define MAX_TYPE_LEN 32
#define MAX_LINE_LEN (MAX_MSG_LEN + MAX_TYPE_LEN - 1)

#define CI_6_BIT_LIMIT  64 - 1
#define CI_13_BIT_LIMIT 8192 - 1
#define CI_20_BIT_LIMIT 1048576 - 1
#define CI_27_BIT_LIMIT 134217728  - 1

void Logf( int Type, const char* Str, ... )
{
  if ( Type < USystem::LogLevel )
    return;

  static char StrBuf[MAX_MSG_LEN];
  static char Msg[MAX_LINE_LEN];
  
  va_list vl;
  va_start( vl, Str );
  vsnprintf( StrBuf, MAX_MSG_LEN, Str, vl );
  va_end( vl );
  
  size_t MsgLen = snprintf( Msg, MAX_LINE_LEN, "[%s] %s\n", LogLevelStrings[Type], StrBuf );
  size_t WriteLen = MsgLen;
  if ( MsgLen > MAX_LINE_LEN ) 
  {
    Logf( LOG_CRIT, "Following log message exceeds maximum length" );
    WriteLen = MAX_LINE_LEN;
  }

  printf( "%s", Msg );
}

/*-----------------------------------------------------------------------------
 * FNameEntry
-----------------------------------------------------------------------------*/
FNameEntry::FNameEntry()
{
  xstl::Set( Data, 0, NAME_LEN );
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
  FName Out = UObject::NameTable->Size();
  UObject::NameTable->PushBack( NewEntry );
  return Out;
}

const char* FName::Data() const
{
  return ((*UObject::NameTable)[Index])->Data;
}

const FHash& FName::Hash() const
{
  return ((*UObject::NameTable)[Index])->Hash;
}

bool operator==( FName& A, FName& B )
{
  return (*UObject::NameTable)[A.Index]->Hash == (*UObject::NameTable)[B.Index]->Hash;
}

bool operator!=( FName& A, FName& B )
{
  return (*UObject::NameTable)[A.Index]->Hash != (*UObject::NameTable)[B.Index]->Hash;
}

/*-----------------------------------------------------------------------------
 * FCompactIndex
-----------------------------------------------------------------------------*/
DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FCompactIndex& Index )
{
  bool negate = false;
 
  Index.Value = 0;
  for (int i = 0; i < 5; i++)
  {
    u8 x = 0;
    if ( In.Read ((char*)&x, 1) == 0 )
    {
      Logf( LOG_WARN, "Failed to read byte for FCompactIndex" );
      return In;    
    }
    
    // First byte
    if (i == 0)
    {
      // Bit: X0000000
      if ((x & 0x80) > 0)
        negate = true;
      // Bits: 00XXXXXX
      Index.Value |= (x & 0x3f);
      // Bit: 0X000000
      if ((x & 0x40) == 0)
        break;
    }
    
    // Last byte
    else if (i == 4)
    {
      // Bits: 000XXXXX
      Index.Value |= (x & 0x1f) << (6 + (3 * 7));
    }
    
    // Middle bytes
    else
    {
      // Bits: 0XXXXXXX
      Index.Value |= (x & 0x7f) << (6 + ((i - 1) * 7));
      // Bit: X0000000
      if ((x & 0x80) == 0)
        break;
    }
  }
  
  if (negate)
    Index.Value = -Index.Value;
  
  return In;
}

DLL_EXPORT FPackageFileOut& operator<<( FPackageFileOut& Out, FCompactIndex& Index )
{
  // looks bad but it's faster than calling pow() potentially 4 times
  u8 num_bytes = 0;
  if (Index.Value <= (CI_6_BIT_LIMIT - 1) && Index.Value >= (-CI_6_BIT_LIMIT))
    num_bytes = 1;
  else if (Index.Value <= (CI_13_BIT_LIMIT - 1) && Index.Value >= (-CI_13_BIT_LIMIT))
    num_bytes = 2;
  else if (Index.Value <= (CI_20_BIT_LIMIT - 1) && Index.Value >= (-CI_20_BIT_LIMIT))
    num_bytes = 3;
  else if (Index.Value <= (CI_27_BIT_LIMIT - 1) && Index.Value >= (-CI_27_BIT_LIMIT))
    num_bytes = 4;
  else
    num_bytes = 5;
    
  u8 byte_out;
  for (int j = 0; j < num_bytes; j++)
  {
    byte_out = 0;
    // First byte
    if (j == 0)
    {
      if (Index.Value < 0)
        byte_out |= 0x80;
        
      if (j+1 < num_bytes)
        byte_out |= 0x40;
        
      byte_out |= (Index.Value & 0x3F);
    }
    
    // Last byte
    else if (j == 4)
    {
      byte_out |= ((Index.Value & 0x7F000000) >> 24);
    }
    
    // Middle bytes
    else
    {
      if (j+1 < num_bytes)
        byte_out |= 0x80;
        
      byte_out |= ((Index.Value >> (6 + ((j - 1) * 7))) & 0x7F);
    }
    
    Out << byte_out;
  }

  return Out;
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
    Str.Reserve( Size-1 );
    
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
  size_t Length = Str.Length();
  Out << CINDEX( Length );
  Out.Write( Str.Data(), Length+1 );
  return Out;
}


