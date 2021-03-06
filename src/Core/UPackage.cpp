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
using namespace xstl;

/*-----------------------------------------------------------------------------
 * Default Property Array Indices
-----------------------------------------------------------------------------*/
// Antonio's docs seemed to leave out the important detail that these 
// are stored in big endian, not little endian (need to see what indices
// >= 16384 look like)
int ReadArrayIndex( FPackageFileIn& PkgFile )
{
  u8 ArrayIdx[4] = { 0, 0 ,0 ,0 };
  PkgFile >> ArrayIdx[0];
  if ( ArrayIdx[0] >= 128 )
  {
    ArrayIdx[1] = ArrayIdx[0];
    PkgFile >> ArrayIdx[0];
    ArrayIdx[1] &= ~0x80;

    // TODO: This whole chunk is probably wrong, but I can't find
    // any examples that get to 16384 (make one!)
    if ( *((u16*)&ArrayIdx[0]) >= 16384 )
    {
      ArrayIdx[3] = ArrayIdx[1];
      ArrayIdx[2] = ArrayIdx[0];
      ArrayIdx[3] &= ~0xC0;

      PkgFile >> ArrayIdx[1];
      PkgFile >> ArrayIdx[0];
    }
  }

  int Idx = *((int*)&ArrayIdx);
  return Idx;
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
 * UPackage
-----------------------------------------------------------------------------*/
Array<UPackage*>* UPackage::Packages = NULL;

UPackage::UPackage()
{
  xstl::Set( &Header, 0, sizeof ( UPackageHeader ) );
  Names = Array<FNameEntry>();
  Exports = Array<FExport>();
  Imports = Array<FImport>();
  bIntrinsicPackage = false;
}

UPackage::~UPackage()
{
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
  NameTableStart = NameTable.Size();
  Names.Resize( Header.NameCount );
  PackageFile->Seek( Header.NameOffset, ESeekBase::Begin );
  for ( int i = 0; i < Header.NameCount; i++ )
  {
    FNameEntry* NameEntry = &Names[i];
    *PackageFile >> *NameEntry;
    NameTable.PushBack( NameEntry );
  }
  NoneNameEntry = &Names[FindName("None")];
  
  // read in imports
  Imports.Resize( Header.ImportCount );
  PackageFile->Seek( Header.ImportOffset, ESeekBase::Begin );
  for ( int i = 0; i < Header.ImportCount; i++ )
    *PackageFile >> Imports[i];
  
  // read in exports
  Exports.Resize( Header.ExportCount );
  PackageFile->Seek( Header.ExportOffset, ESeekBase::Begin );
  for ( int i = 0; i < Header.ExportCount; i++ )
  {
    *PackageFile >> Exports[i];
    Exports[i].Index = i;
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
    return &Names[Index];

  return NULL;
}

FNameEntry* UPackage::GetNameEntryByObjRef( int ObjRef )
{
  if ( ObjRef == 0 )
    return NoneNameEntry;

  int Index;
  if ( ObjRef < 0 )
    Index = Imports[(-ObjRef)-1].ObjectName;
  else if ( ObjRef > 0 )
    Index = Exports[ObjRef-1].ObjectName;
  
  return &Names[Index];
}

Array<FNameEntry>& UPackage::GetNameTable()
{
  return Names;
}

FImport* UPackage::GetImport( size_t Index )
{
  if ( LIKELY( Index >= 0 ) )
    return &Imports[Index];
  
  return NULL;
}

FExport* UPackage::GetExport( size_t Index )
{
  if ( LIKELY( Index >= 0 ) )
    return &Exports[Index];

  return NULL;
}

FExport* UPackage::GetExportByName( size_t Name )
{
  for ( int i = 0; i < Exports.Size(); i++ )
  {
    FExport* Export = &Exports[i];
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
  for ( int i = 0; i < Names.Size(); i++ )
  {
    if ( NameHash == Names[i].Hash )
    {
      NameIndex = i;
      break;
    }
  }
  if ( NameIndex < 0 )
    return NULL;

  // Now find the export associated with this name
  FExport* Export = NULL;
  for ( int i = 0; i < Exports.Size(); i++ )
  {
    // Index 0 in the name table is always None 
    if ( Exports[i].ObjectName == NameIndex && Exports[i].Class == 0 )
    {
      Export = &Exports[i];
      break;
    }
  }

  return Export;
}

Array<FExport>& UPackage::GetExportTable()
{
  return Exports;
}

u32 UPackage::GetGlobalName( u32 PkgNameIdx )
{
  return NameTableStart + PkgNameIdx;
}

Array<FImport>& UPackage::GetImportTable()
{
  return Imports;
}

const char* UPackage::GetFilePath()
{
  return Path.Data();
}

const char* UPackage::GetFileName()
{
  return Name.Data();
}

FString* UPackage::GetFullObjName( FExport* ObjExp )
{
  // Get all names into a stack
  static Stack<const char*>* Names = new Stack<const char*>();
  FExport* Exp = ObjExp;

  Names->Push( GetNameEntry( Exp->ObjectName )->Data );
  while ( Exp->Group != 0 )
  {
    Exp = GetExport( Exp->Group-1 );
    Names->Push( GetNameEntry( Exp->ObjectName )->Data );
  };

  Names->Push( Name.Data() );

  // Concat all names to a string
  static FString* ObjName = new FString();
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
  for ( int i = 0; i < Names.Size(); i++ )
  {
    if ( stricmp( Names[i].Data, Name ) == 0 )
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

FName UPackage::ResolveGlobalNameObjRef( int ObjRef )
{
  if (ObjRef == 0)
    return 0;
  else if (ObjRef < 0)
    return GetGlobalName( GetImport( CalcObjRefValue( ObjRef ) )->ObjectName );
  else
    return GetGlobalName( GetExport( CalcObjRefValue( ObjRef ) )->ObjectName ); 
}

FPackageFileIn* UPackage::GetStream()
{
  return (FPackageFileIn*)Stream;
}

FString UPackage::GetPackageName()
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

    if ( stricmp( Iter->Name.Data(), PkgName ) == 0 )
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

    size_t PkgNameIdx = Pkg->FindName( PkgName );
    Pkg->Name = (PkgNameIdx != MAX_SIZE) 
      ? FName( Pkg->GetGlobalName( PkgNameIdx ) )
      : FName::CreateName( PkgName, RF_LoadContextFlags );
    Packages->PushBack( Pkg );
  }

  return Pkg;
}

IMPLEMENT_NATIVE_CLASS( UPackage );

