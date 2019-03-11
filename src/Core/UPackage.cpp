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
 * UPackage.cpp - Functions for keeping track of a single UE1 package
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "XMemory.h"
#include "Core/UClass.h"
#include "Core/UPackage.h"
#include "Core/USystem.h"
#include "Core/UTexture.h"

EPkgLoadOpts UPackage::LoadOpts = PO_OpenOnLoad;

#define CI_6_BIT_LIMIT  64 - 1
#define CI_13_BIT_LIMIT 8192 - 1
#define CI_20_BIT_LIMIT 1048576 - 1
#define CI_27_BIT_LIMIT 134217728  - 1

using namespace xstl;

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
}

/*-----------------------------------------------------------------------------
 * FString
-----------------------------------------------------------------------------*/
DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, String& Str )
{
  idx Size = 0;
  In >> CINDEX( Size );

  if ( Size > 0 )
  {
    Str.Reserve( Size-1 );
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

DLL_EXPORT FPackageFileOut& operator<<( FPackageFileOut& Out, String& Str )
{
  size_t Length = Str.Length();
  Out << CINDEX( Length );
  Out.Write( Str.Data(), Length+1 );
  return Out;
}

/*-----------------------------------------------------------------------------
 * FExport
-----------------------------------------------------------------------------*/
FPackageFileIn& operator>>( FPackageFileIn& In, FExport& Export )
{
  In >> CINDEX( Export.Class );
  In >> CINDEX( Export.Super );
  In >> Export.Group;
  In >> CINDEX( Export.ObjectName );
  In >> Export.ObjectFlags;
  In >> CINDEX( Export.SerialSize );
  
  if (Export.SerialSize > 0)
    In >> CINDEX( Export.SerialOffset );
  else
    Export.SerialOffset = -1;
  
  Export.Obj = NULL;
  Export.bNeedsFullLoad = true;

  return In;
}

/*-----------------------------------------------------------------------------
 * FImport
-----------------------------------------------------------------------------*/
FPackageFileIn& operator>>( FPackageFileIn& In, FImport& Import )
{
  In >> CINDEX( Import.ClassPackage );
  In >> CINDEX( Import.ClassName );
  In >> Import.Package;
  In >> CINDEX( Import.ObjectName );
  return In;
}

/*-----------------------------------------------------------------------------
 * UPackageHeader
-----------------------------------------------------------------------------*/
FPackageFileIn& operator>>( FPackageFileIn& In, UPackageHeader& Header )
{
  In >> Header.Signature;
  In >> Header.PackageVersion;
  In.Ver = Header.PackageVersion;
  In >> Header.LicenseMode;
  In >> Header.PackageFlags;
  In >> Header.NameCount;
  In >> Header.NameOffset;
  In >> Header.ExportCount;
  In >> Header.ExportOffset;
  In >> Header.ImportCount;
  In >> Header.ImportOffset;
  if (Header.PackageVersion < PKG_VER_UT_400)
  {
    In >> Header.HeritageCount;
    In >> Header.HeritageOffset;
  }
  In.Read ( Header.GUID, 16 );
  return In;
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

FNameEntry::FNameEntry( const char* InStr )
{
  strncpy( Data, InStr, NAME_LEN );
  Data[NAME_LEN-1] = '\0';
  Flags = 0;
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
}

/*-----------------------------------------------------------------------------
 * UPackage
-----------------------------------------------------------------------------*/
Array<UPackage*>* UPackage::Packages = NULL;

UPackage::UPackage()
{
  xstl::Set( &Header, 0, sizeof ( UPackageHeader ) );
  Names = new Array<FNameEntry>();
  Exports = new Array<FExport>();
  Imports = new Array<FImport>();
}

UPackage::~UPackage()
{
  if ( Names != NULL )
    delete Names;

  if ( Exports != NULL )
    delete Exports;

  if ( Imports != NULL )
    delete Imports;
}

bool UPackage::Load( const char* File )
{  
  if ( !File )
    return false;
 
  Path.Assign( File );
  Path.ReplaceChars( '\\', '/' );
  
  u64 LastDirSlash = Path.FindLastOf( "/" );
 
  String PkgName;
  if (LastDirSlash != MAX_SIZE)
    PkgName = Path.Substr( Path.FindLastOf( "/" ) );
  else
    PkgName = Path;
  
  
  PkgName.Erase( PkgName.FindLastOf( "." ) );
  
  Stream = new FPackageFileIn();
  if ( Stream->Open( Path ) != 0 )
    return false;
  
  FPackageFileIn* PackageFile = (FPackageFileIn*)Stream;
  PackageFile->Pkg = this;

  // read in the header
  *PackageFile >> Header;

  if ( Header.Signature != UE1_PKG_SIG )
  {
    Logf( LOG_WARN, "File '%s' has a bad package signature", File );
    PackageFile->Close();
    return false;
  }

  // read in the name table
  Names->Resize( Header.NameCount );
  PackageFile->Seek( Header.NameOffset, ESeekBase::Begin );
  for ( int i = 0; i < Header.NameCount; i++ )
    *PackageFile >> (*Names)[i];
  
  // read in imports
  Imports->Resize( Header.ImportCount );
  PackageFile->Seek( Header.ImportOffset, ESeekBase::Begin );
  for ( int i = 0; i < Header.ImportCount; i++ )
    *PackageFile >> (*Imports)[i];
  
  // read in exports
  Exports->Resize( Header.ExportCount );
  PackageFile->Seek( Header.ExportOffset, ESeekBase::Begin );
  for ( int i = 0; i < Header.ExportCount; i++ )
  {
    *PackageFile >> (*Exports)[i];
    (*Exports)[i].Index = i;
  }

  PackageFile->Seek( 0, ESeekBase::Begin );

  return true;
}

bool UPackage::Save( const char* File )
{
  return false;
}

void UPackage::Close()
{
}

UPackageHeader* UPackage::GetHeader()
{
  return &Header;
}

FNameEntry* UPackage::GetNameEntry( size_t Index )
{
  if ( LIKELY( Index >= 0 ) )
    return &(*Names)[Index];

  return NULL;
}

FNameEntry* UPackage::GetNameEntryByObjRef( int ObjRef )
{
  int Index = CalcObjRefValue( ObjRef );
  if ( ObjRef < 0 )
    Index = (*Imports)[Index].ObjectName;
  else if ( ObjRef > 0 )
    Index = (*Exports)[Index].ObjectName;

  return &(*Names)[Index];
}

FImport* UPackage::GetImport( size_t Index )
{
  if ( LIKELY( Index >= 0 ) )
    return &(*Imports)[Index];
  
  return NULL;
}

FExport* UPackage::GetExport( size_t Index )
{
  if ( LIKELY( Index >= 0 ) )
    return &(*Exports)[Index];

  return NULL;
}

FExport* UPackage::GetExportByName( size_t Name )
{
  for ( size_t i = 0; i < Exports->Size() && i != MAX_SIZE; i++ )
  {
    FExport* Export = &(*Exports)[i];
    if ( Export->ObjectName == Name )
      return Export;
  }

  return NULL;
}

FExport* UPackage::GetClassExport( const char* ExportName )
{
  // Find the name in this package's name table
  int NameIndex = -1;
  FHash NameHash = FnvHashString( ExportName );
  for ( int i = 0; i < Names->Size() && i < MAX_SIZE; i++ )
  {
    if ( NameHash == (*Names)[i].Hash )
    {
      NameIndex = i;
      break;
    }
  }
  if ( NameIndex < 0 )
    return NULL;

  // Now find the export associated with this name
  FExport* Export = NULL;
  for ( int i = 0; i < Exports->Size() && i < MAX_SIZE; i++ )
  {
    // Index 0 in the name table is always None 
    if ( (*Exports)[i].ObjectName == NameIndex && (*Exports)[i].Class == 0 )
    {
      Export = &(*Exports)[i];
      break;
    }
  }

  return Export;
}

Array<FExport>* UPackage::GetExportTable()
{
  return Exports;
}

Array<FImport>* UPackage::GetImportTable()
{
  return Imports;
}

const char* UPackage::GetFilePath()
{
  return Path.Data();
}

const char* UPackage::GetFileName()
{
  return Name;
}

String* UPackage::GetFullObjName( FExport* ObjExp )
{
  // Get all names into a stack
  static Stack<char*>* Names = new Stack<char*>();
  FExport* Exp = ObjExp;

  Names->Push( GetNameEntry( Exp->ObjectName )->Data );
  while ( Exp->Group != 0 )
  {
    Exp = GetExport( Exp->Group-1 );
    Names->Push( GetNameEntry( Exp->ObjectName )->Data );
  };

  Names->Push( (char*)Name );

  // Concat all names to a string
  static String* ObjName = new String();
  ObjName->Erase();
  while ( Names->Size() > 0 )
  {
    (*ObjName) += Names->Top();
    (*ObjName) += '.';
    Names->Pop();
  }

  if ( ObjName->Length() > 0 )
    ObjName->PopBack();

  return ObjName;
}

size_t UPackage::FindName( const char* Name )
{
  FHash NameHash = FnvHashString( Name );
  for ( size_t i = 0; i < Names->Size() && i != MAX_SIZE; i++ )
  {
    if ( (*Names)[i].Hash == NameHash )
      return i;
  }
  
  return MAX_SIZE;
}

const char* UPackage::ResolveNameFromIdx( idx Index )
{
  return GetNameEntry( Index )->Data;
}

const char* UPackage::ResolveNameFromObjRef( int ObjRef )
{
  if (ObjRef == 0)
    return "None";
  else if (ObjRef < 0)
    return GetNameEntry( GetImport( CalcObjRefValue( ObjRef ) )->ObjectName )->Data;
  else
    return GetNameEntry( GetExport( CalcObjRefValue( ObjRef ) )->ObjectName )->Data;
}

FPackageFileIn* UPackage::GetStream()
{
  return (FPackageFileIn*)Stream;
}

String UPackage::GetPackageName()
{
  return Path.Substr( Path.FindLastOf( '/' ) );
}

bool UPackage::StaticInit()
{
  if ( !Packages )
  {
    LoadOpts = PO_OpenOnLoad;
    Packages = new Array<UPackage*>();
    if ( Packages == NULL )
      return false;
    Packages->Reserve( 8 );
  }
  return true;
}

void UPackage::StaticExit( bool bCrashExit )
{
  if ( UPackage::Packages == NULL )
    delete Packages;
}

UPackage* UPackage::StaticLoadPackage( const char* PkgName )
{
  if ( PkgName == NULL )
    return NULL;

  // See if the package has been loaded before
  UPackage* Pkg = NULL;
  for ( int i = 0; i < Packages->Size(); i++ )
  {
    UPackage* Iter = Packages->Data()[i];
    if ( Iter == NULL )
      break;

    if ( strnicmp( Iter->Name, PkgName, strlen( Iter->Name ) ) == 0 )
    {
      Pkg = Iter;
      break;
    }
  }

  if ( Pkg == NULL )
  {
    Pkg = new UPackage();
    if ( Pkg == NULL )
      return NULL;

    if ( !Pkg->Load( GSystem->ResolvePath( PkgName ) ) )
    {
      delete Pkg;
      return NULL;
    }
 
    Pkg->Name = StringDup( PkgName );
    Pkg->Hash = FnvHashString( PkgName );
    Packages->PushBack( Pkg );
  }

  return Pkg;
}

