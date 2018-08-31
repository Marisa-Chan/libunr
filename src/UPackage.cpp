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
 * UPackage.cpp - Functions for keeping track of a single UE1 package
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Memory.h"
#include "UClass.h"
#include "UPackage.h"
#include "USystem.h"
#include "UTexture.h"

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
  
  for (int i = 0; i < 5; i++)
  {
    u8 x = 0;
    In.Read ((char*)&x, 1);
    
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
  Hash = FnvHashString( Data );;
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
UPackage::UPackage()
{
  xstl::Set( &Header, 0, sizeof ( UPackageHeader ) );
  Names = new Array<FNameEntry>();
  Exports = new Array<FExport>();
  Imports = new Array<FImport>();
}

UPackage::~UPackage()
{
  delete Names;
  delete Exports;
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
  if ( !Stream->Open( Path ) )
    return false;
  
  // read in the header
  *((FPackageFileIn*)Stream) >> Header;
  
  // read in the name table
  Names->Resize( Header.NameCount );
  Stream->Seek( Header.NameOffset, ESeekBase::Begin );
  for ( int i = 0; i < Header.NameCount; i++ )
    *((FPackageFileIn*)Stream) >> (*Names)[i];
  
  // read in imports
  Imports->Resize( Header.ImportCount );
  Stream->Seek( Header.ImportOffset, ESeekBase::Begin );
  for ( int i = 0; i < Header.ImportCount; i++ )
    *((FPackageFileIn*)Stream) >> (*Imports)[i];
  
  // read in exports
  Exports->Resize( Header.ExportCount );
  Stream->Seek( Header.ExportOffset, ESeekBase::Begin );
  for ( int i = 0; i < Header.ExportCount; i++ )
  {
    *((FPackageFileIn*)Stream) >> (*Exports)[i];
    (*Exports)[i].Index = i;
  }
  
  EndLoad();
  
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

FExport* UPackage::GetExport( const char* ExportName )
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
    if ( (*Exports)[i].ObjectName == NameIndex )
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

bool UPackage::LoadObject( UObject** Obj, const char* ObjName, idx ObjRef )
{
  if ( Obj == NULL || *Obj == NULL )
    return false;
 
  FExport* Export = NULL;
  if ( ObjRef >= 0 )
    GetExport( CalcObjRefValue( ObjRef ) );

  if ( Export == NULL )
  {
    Export = GetExport( ObjName );
    if ( Export == NULL )
    {
      Logf( LOG_CRIT, "Failed to find export for object '%s' in package '%s'.", ObjName, Name );
      return false;
    }
  }
 
  // If we already loaded the object, return it
  if ( Export->Obj )
  {
    *Obj = Export->Obj;
    return true;
  }

  // Otherwise, prepare the package to load an object
  if ( !BeginLoad( Export ) )
    return false;
  
  // Load
  FPackageFileIn* PackageFile = (FPackageFileIn*)Stream;

  // Set export object properties
  (*Obj)->ExpIdx = Export->Index;
  (*Obj)->NameIdx = Export->ObjectName;
  (*Obj)->Pkg = this;
  (*Obj)->LoadFromPackage( *PackageFile );

  Export->Obj = *Obj;
  
  // Finalize load
  EndLoad();
  
  return true;
}

bool UPackage::StaticInit()
{
  if ( !Packages )
  {
    LoadOpts = PO_OpenOnLoad;
    Packages = new Array<UPackage*>( 8 );
  }
}

int UPackage::CalcObjRefValue( int ObjRef )
{
  if ( ObjRef == 0 )
    return ObjRef;
  
  else if ( ObjRef < 0 )
    ObjRef = -ObjRef;
  
  return ObjRef - 1;
}

UPackage* UPackage::StaticLoadPkg( const char* Filepath )
{
  UPackage* Pkg = new UPackage();
  if (!Pkg)
    return NULL;
  
  if ( !Pkg->Load( Filepath ) )
  {
    delete Pkg;
    return NULL;
  }
  
  Packages->PushBack( Pkg );
  return Pkg;
}

UObject* UPackage::StaticLoadObject( UPackage* Package, idx ObjRef, UClass* ObjClass, UObject* InOuter )
{
  if ( UNLIKELY( Package == NULL ) )
  {
    Logf( LOG_CRIT, "StaticLoadObject: NULL argument passed (ObjClass='%s')", ObjClass->Name );
    return NULL;
  }
  const char* ObjName = Package->ResolveNameFromObjRef( ObjRef );
  UPackage* ObjPkg = Package;
  FExport* Export = NULL;

  // Is our object in this package?
  if ( ObjRef < 0 )
  {
    // Nope, but we can figure out which package it should be in from the one we were given
    FImport* Import = &(*Package->Imports)[ CalcObjRefValue( ObjRef ) ];
    
    // We probably need to get the class too...
    if ( LIKELY( ObjClass == NULL ) )
    {
      const char* ClsName = Package->ResolveNameFromIdx( Import->ClassName );
      FHash ClsNameHash  = FnvHashString( ClsName );
      for ( size_t i = 0; i < UObject::ClassPool.Size() && i != MAX_SIZE; i++ )
      {
        UClass* ClsIter = UObject::ClassPool[i];
        if ( ClsIter->Hash == ClsNameHash )
        {
          ObjClass = ClsIter;
          break; // Already loaded the class apparently, great
        }
      }

      if ( ObjClass == NULL )
      {
        // Alright, it's not loaded at all. Grab it's package and lets load that
        const char* ClsPkgName = Package->ResolveNameFromIdx( Import->ClassPackage );
        UPackage* ClsPkg = UPackage::StaticLoadPkg( GSystem->ResolvePath( ClsPkgName ) );
        if ( Package == NULL )
        {
          // We can't find the package we need, bail out
          Logf( LOG_CRIT, "Can't load class '%s.%s', package is missing", ClsPkgName, ClsName );
          return NULL;
        }
        
        // Got it, now load the class
        ObjClass = (UClass*)UPackage::StaticLoadObject( ClsPkg, ClsName, UClass::StaticClass() );
        if ( UNLIKELY( ObjClass == NULL ) )
        {
          // ... but it's not there, bail out
          Logf( LOG_CRIT, "Can't load class '%s.%s', class does not exist", ClsPkgName, ClsName );
          return NULL;
        }
      }
    }

    // We have the class, now get the package it belongs in
    FImport* PkgImport = &(*Package->Imports)[ CalcObjRefValue( Import->Package ) ];
    const char* ObjPkgName = Package->ResolveNameFromObjRef( Import->Package );
    ObjPkg = UPackage::StaticLoadPkg( GSystem->ResolvePath( ObjPkgName ) );
    if ( UNLIKELY( ObjPkg == NULL ) )
    {
      Logf( LOG_CRIT, "Can't load object '%s.%s', package is missing", ObjPkgName, ObjName );
      return NULL;
    }

    // Then grab it's export
    Export = ObjPkg->GetExport( ObjName );
    if ( UNLIKELY( Export == NULL ) )
    {
      Logf( LOG_CRIT, "Can't load object '%s.%s', object does not exist", ObjPkgName, ObjName );
      return NULL;
    }
  }
  else
  {
    // Yup, just grab the export
    Export = &(*Package->Exports)[ CalcObjRefValue( ObjRef ) ];
 
    if ( ObjClass == NULL )
    {
      ObjClass = (UClass*)UPackage::StaticLoadObject( Package, Export->Class, UClass::StaticClass() );
      if ( UNLIKELY( ObjClass == NULL ) )
      {
        Logf( LOG_CRIT, "Can't load object '%s.%s', cannot load class", Package->Name, ObjName );
        return NULL;
      }
    }

    ObjPkg = Package;
  }

  // We are ready to construct and load the object
  UObject* Obj = UObject::StaticConstructObject( ObjName, ObjClass, InOuter );
  if ( Obj == NULL )
    return NULL;

  // In case of circular dependencies
  Export->Obj = Obj;
  if ( !ObjPkg->LoadObject( &Obj, ObjName, ObjRef ) )
    return NULL;

  return Obj;
}

// A partial class is a native class that has been allocated and initialized, but
// has not had it's UnrealScript counterpart loaded yet. Hence, it is "partially" initialized.
bool UPackage::StaticLoadPartialClass( const char* PkgName, UClass* NativeClass )
{
  UPackage* Package = NULL; 
  for ( int i = 0; i < Packages->Size(); i++ )
  {
    Package = Packages->Data()[i];
    if ( Package->GetPackageName() == PkgName )
      break;
  }
  
  // If we didn't find the package, load it
  if ( Package == NULL )
  {
    // FIXME: Use <Game>.ini to load packages from paths (../System/, ../Textures/, etc.)
    Package = UPackage::StaticLoadPkg( PkgName );

    // still didn't find it? error out
    if ( Package == NULL )
    {
      Logf( LOG_CRIT, "Failed to load package '%s' for class '%s'.", PkgName, NativeClass->Name );
      return false;
    }
  }
  
  // We already have a constructed class object, so just load it from the package
  UObject* Obj = (UObject*)NativeClass;
  if ( !Package->LoadObject( &Obj, NativeClass->Name, -1 ) )
    return false;

  return true;
}

bool UPackage::BeginLoad( FExport* Export )
{
  if ( Export == NULL )
    return false;
  
  if ( LoadOpts == PO_OpenOnLoad && Stream == NULL )
  {
    Stream = new FPackageFileIn();
    
    if ( Stream == NULL )
      return false;
    if ( !Stream->Open( Path ) )
      return false;
  }

  ((FPackageFileIn*)Stream)->Ver = Header.PackageVersion;
  Stream->Seek( Export->SerialOffset, Begin );
  return true;
}

void UPackage::EndLoad()
{
  if ( LoadOpts == PO_OpenOnLoad )
  {
    delete Stream;
    Stream = NULL;
  }
}
