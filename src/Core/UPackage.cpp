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
 * UPackage.cpp - Functions for keeping track of a single UE1 package
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UPackage.h"
#include "Core/USystem.h"

#define CI_6_BIT_LIMIT  64 - 1
#define CI_13_BIT_LIMIT 8192 - 1
#define CI_20_BIT_LIMIT 1048576 - 1
#define CI_27_BIT_LIMIT 134217728  - 1

EPkgLoadOpts UPackage::LoadOpts = PO_OpenOnLoad;

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
 * FPackageFileIn
-----------------------------------------------------------------------------*/
FPackageFileIn::FPackageFileIn()
{
  Ver = 0;
  Pkg = NULL;
}

/*-----------------------------------------------------------------------------
 * FPackageFileOut
-----------------------------------------------------------------------------*/
FPackageFileOut::FPackageFileOut()
{
  Ver = 0;
  Pkg = NULL;
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
void UPackageHeader::Initialize()
{
  Signature = UE1_PKG_SIG;
  PackageVersion = GSystem->MaxPackageVersion;
  LicenseMode = 0;
  PackageFlags = PKG_Default;
  NameCount = 0;
  NameOffset = 0;
  ExportCount = 0;
  ExportOffset = 0;
  ImportCount = 0;
  ImportOffset = 0;
  HeritageCount = 0;
  HeritageOffset = 0;
  GUID.Generate();
}

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
  if ( Header.PackageVersion < PKG_VER_UT_400 )
  {
    In >> Header.HeritageCount;
    In >> Header.HeritageOffset;
  }
  In.Read ( Header.GUID, 16 );
  return In;
}

/*-----------------------------------------------------------------------------
 * FCompactIndex
-----------------------------------------------------------------------------*/
LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FCompactIndex& Index )
{
  bool negate = false;
 
  Index.Value = 0;
  for (int i = 0; i < 5; i++)
  {
    u8 x = 0;
    if ( In.Read ((char*)&x, 1) == 0 )
    {
      GLogf( LOG_WARN, "Failed to read byte for FCompactIndex" );
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

LIBUNR_API FPackageFileOut& operator<<( FPackageFileOut& Out, FCompactIndex& Index )
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
 * UPackage
-----------------------------------------------------------------------------*/
TArray<UPackage*>* UPackage::Packages = NULL;
UPackage* UPackage::OpenUEPkg = NULL;

UPackage::UPackage()
  : UObject()
{
  Names = TArray<FNameEntry>();
  Exports = TArray<FExport>();
  Imports = TArray<FImport>();
  bIntrinsicPackage = false;

  memset( &Header, 0, sizeof( UPackageHeader ) );
}

UPackage::UPackage( const char* InName, UNativeModule* InNativeModule )
  : UObject()
{
  Names = TArray<FNameEntry>();
  Exports = TArray<FExport>();
  Imports = TArray<FImport>();
  bIntrinsicPackage = false;
  Stream = NULL;

  Name = FName( InName, RF_LoadContextFlags );
  NativeModule = InNativeModule;
  Header.Initialize();
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
  
  Stream = new FPackageFileIn();
  if ( Stream->Open( Path ) != 0 )
    return false;
  
  FPackageFileIn* PackageFile = (FPackageFileIn*)Stream;
  PackageFile->Pkg = this;

  // read in the header
  *PackageFile >> Header;

  if ( Header.Signature != UE1_PKG_SIG )
  {
    GLogf( LOG_WARN, "File '%s' has a bad package signature", File );
    PackageFile->Close();
    return false;
  }

  // read in the name table
  Names.Resize( Header.NameCount );
  PackageFile->Seek( Header.NameOffset, ESeekBase::Begin );
  for ( u32 i = 0; i < Header.NameCount; i++ )
  {
    FORTIFY_LOOP( i, MAX_UINT32 );
    FNameEntry* NameEntry = &Names[i];
    *PackageFile >> *NameEntry;
  }
  
  // read in imports
  Imports.Resize( Header.ImportCount );
  PackageFile->Seek( Header.ImportOffset, ESeekBase::Begin );
  for ( u32 i = 0; i < Header.ImportCount; i++ )
  {
    FORTIFY_LOOP( i, MAX_UINT32 );
    *PackageFile >> Imports[i];
    Imports[i].Index = i;
  }
  
  // read in exports
  Exports.Resize( Header.ExportCount );
  PackageFile->Seek( Header.ExportOffset, ESeekBase::Begin );
  for ( u32 i = 0; i < Header.ExportCount; i++ )
  {
    FORTIFY_LOOP( i, MAX_UINT32 );
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
    return (FNameEntry*)FName( NAME_None ).Entry();

  int Index;
  if ( ObjRef < 0 )
    Index = Imports[(size_t)(-ObjRef)-1].ObjectName;
  else if ( ObjRef > 0 )
    Index = Exports[(size_t)ObjRef-1].ObjectName;
  
  return &Names[Index];
}

TArray<FNameEntry>& UPackage::GetNameTable()
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

FImport* UPackage::FindImport( UObject* Obj )
{
  size_t ReturnIndex = FAILURE;

  // Try to find the name first
  size_t ImportName = FindLocalName( Obj->Name.Data() );
  if ( ImportName == FAILURE )
  {
    // If we don't have the name in our own package, then we definitely don't have the import
    ReturnIndex = AddImport( Obj );
    if ( ReturnIndex == FAILURE )
      return NULL;

    return &Imports[ReturnIndex];
  }

  // Now try to find the class name
  size_t ClassName = FindLocalName( Obj->Class->Name.Data() );
  if ( ClassName == FAILURE )
  {
    // We don't have the particular type we're looking for, so we don't have the import
    ReturnIndex = AddImport( Obj );
    if ( ReturnIndex == FAILURE )
      return NULL;

    return &Imports[ReturnIndex];
  }

  // Get the class package name
  size_t ClassPkgName = FindLocalName( Obj->Class->Pkg->Name.Data() );
  if ( ClassPkgName == FAILURE )
  {
    // We don't have the package of the type we're looking for, so we don't have the import
    ReturnIndex = AddImport( Obj );
    if ( ReturnIndex == FAILURE )
      return NULL;

    return &Imports[ReturnIndex];
  }

  // Now get package reference (if the object itself is a package, then it will be 0)
  idx PkgRef = 0;
  if ( Obj->Class != UPackage::StaticClass() )
  {
    FImport* PkgImport = FindImport( Obj->Pkg );
    if ( PkgImport == NULL )
      return NULL;
    PkgRef = -(PkgImport->Index + 1);
  }
    
  // Now search through all imports to find our match
  for ( size_t i = 0; i < Imports.Size(); i++ )
  {
    FORTIFY_LOOP_SLOW( i, MAX_SIZE );
    FImport& Import = Imports[i];
    if ( Import.ObjectName == ImportName &&
         Import.ClassName == ClassName &&
         Import.ClassPackage == ClassPkgName &&
         Import.Package == PkgRef )
      return &Import;
  }

  // Add the import if we don't have it
  ReturnIndex = AddImport( Obj );
  return &Imports[ReturnIndex];
}

FExport* UPackage::GetExportByNameAndType( size_t Name, UClass* Type )
{
  // Get the obj ref we're looking for
  idx TypeObjRef = 0;
  if ( Type != UClass::StaticClass() )
  {
    if ( Type->Pkg == this && !(Type->ClassFlags & CLASS_NoExport) )
    {
      for ( TypeObjRef = 0; TypeObjRef < Exports.Size(); TypeObjRef++ )
      {
        FExport& Export = Exports[TypeObjRef];
        if ( Export.Class == 0 )
        {
          if ( stricmp( Names[Exports[TypeObjRef].ObjectName].Data, Type->Name.Data() ) == 0 )
          {
            TypeObjRef += 1;
            break;
          }
        }
      }
    }
    else
    {
      for ( TypeObjRef = 0; TypeObjRef < Imports.Size(); TypeObjRef++ )
      {
        FImport& Import = Imports[TypeObjRef];
        if ( stricmp( Names[Import.ClassName].Data, "Class" ) == 0 )
        {
          if ( stricmp( Names[Import.ObjectName].Data, Type->Name.Data() ) == 0 )
          {
            TypeObjRef = -(TypeObjRef + 1);
            break;
          }
        }
      }
    }
  }

  for ( int i = 0; i < Exports.Size(); i++ )
  {
    FExport* Export = &Exports[i];
    if ( Export->ObjectName == Name && Export->Class == TypeObjRef )
      return Export;
  }

  return NULL;
}

FExport* UPackage::GetClassExport( const char* ExportName )
{
  // Find the name in this package's name table
  int NameIndex = -1;
  u32 NameHash = SuperFastHashString( ExportName );
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

TArray<FExport>& UPackage::GetExportTable()
{
  return Exports;
}

TArray<FImport>& UPackage::GetImportTable()
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
  static TStack<const char*>* Names = new TStack<const char*>();
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

size_t UPackage::FindLocalName( const char* Name )
{
  for ( int i = 0; i < Names.Size(); i++ )
  {
    if ( stricmp( Names[i].Data, Name ) == 0 )
      return i;
  }

  return MAX_SIZE;
}

size_t UPackage::AddName( const char* Name, int Flags )
{
  FNameEntry NewName( Name, 0, Flags );
  Names.PushBack( NewName );
  return Names.Size()-1;
}

size_t UPackage::AddImport( UObject* Obj )
{
  if ( !Obj )
  {
    GLogf( LOG_ERR, "NULL object in UPackage::AddImport (%s)", Name.Data() );
    return FAILURE;
  }

  // An object that belongs to this package can't be an import
  if ( Obj->Pkg == this )
  {
    GLogf( LOG_ERR, "Tried to add local object as import (Obj: %s | Obj->Pkg: %s | Pkg: %s)",
      Obj->Name.Data(), Obj->Pkg->Name.Data(), Name.Data() );
    return FAILURE;
  }

  // FIXME: Should each instance of this be it's own function?
  // Make sure all relevant names are added before adding this import
  int Flags = RF_TagExp | RF_LoadContextFlags;
  size_t ClassPackageName = FindLocalName( Obj->Class->Pkg->Name.Data() );
  if ( ClassPackageName == FAILURE )
  {
    if ( Obj->Class->Pkg->ObjectFlags & RF_Native )
      Flags |= RF_Native;

    ClassPackageName = AddName( Obj->Class->Pkg->Name.Data(), Flags );
  }

  Flags = RF_TagExp | RF_LoadContextFlags;
  size_t ClassName = FindLocalName( Obj->Class->Name.Data() );
  if ( ClassName == FAILURE )
  {
    if ( Obj->Class->ObjectFlags & RF_Native )
      Flags |= RF_Native;

    ClassName = AddName( Obj->Class->Name.Data(), Flags );
  }

  size_t PackageName = 0;
  if ( Obj->Pkg )
  {
    Flags = RF_TagExp | RF_LoadContextFlags;
    PackageName = FindLocalName( Obj->Pkg->Name.Data() );
    if ( PackageName == FAILURE )
    {
      if ( Obj->Pkg->ObjectFlags & RF_Native )
        Flags |= RF_Native;

      PackageName = AddName( Obj->Pkg->Name.Data(), Flags );
    }
  }

  Flags = RF_TagExp | RF_LoadContextFlags;
  size_t ImportName = FindLocalName( Obj->Name.Data() );
  if ( ImportName == FAILURE )
  {
    if ( Obj->ObjectFlags & RF_Native )
      Flags |= RF_Native;

    ImportName = AddName( Obj->Name.Data(), Flags );
  }

  // Create the new import
  FImport NewImport;
  NewImport.ClassPackage = ClassPackageName;
  NewImport.ClassName    = ClassName;
  NewImport.Package      = PackageName;
  NewImport.ObjectName   = ImportName;
  Imports.PushBack( NewImport );

  return Imports.Size() - 1;
}

size_t UPackage::AddExport( UObject* Obj )
{
  if ( !Obj )
  {
    GLogf( LOG_ERR, "NULL object in UPackage::AddExport (%s)", Name.Data() );
    return FAILURE;
  }

  // An object that doesn't belong to this package can't be an export
  if ( Obj->Pkg == this )
  {
    GLogf( LOG_ERR, "Tried to add external object as export (Obj: %s | Obj->Pkg: %s | Pkg: %s)",
      Obj->Name.Data(), Obj->Pkg->Name.Data(), Name.Data() );
    return FAILURE;
  }

  // Get class object reference value
  // Figure out if the object class is within this package or not
  idx ClassIdx = 0;
  if ( Obj->Class->Pkg == this )
  {
    ClassIdx = Obj->Class->Export->Index + 1;
  }
  else
  {
    FImport* ClassImport = FindImport( Obj->Class );

  }
}

void UPackage::Optimize()
{
  // TODO
  //
  // Wow, this one is going to be a real pain in the ass. 
  //
  // Over time, packages tend to accumulate a bunch of "None" exports or
  // redundant entries for names (usually None) in the name table. This
  // happens because it's not really nice to have to recalculate every
  // offset for every object because the user wanted to delete something.
  //
  // This method should let you optimize a package by removing any redundant
  // references and additionally sorting non-script related objects by type so that
  // loading the package is a bit more efficient if you're loading it in the editor.
  //
  // The main problem is that, in order to properly do this, every single object
  // must be rearranged, meaning that every class will need to be recompiled so
  // that everything points to the new object indices instead.
  //
  // Needless to say, this isn't gonna happen for awhile until the UScript compiler
  // is stable. Running this should make loading the package a bit faster, which
  // is always a good thing.
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

void UPackage::LoadEditableTypes()
{
  bool bDoGroupPathExport = false;
  const char* ClassName;
  const char* ObjName;
  //u32 ClassHash;
  const char* Types[] =
  {
    "None",
    "Texture",
    "Sound",
    "Music",
    "Mesh",
    "LodMesh",
    "SkeletalMesh",
    "Animation",
    "Level"
  };

  for ( int i = 0; i < Exports.Size(); i++ )
  {
    FExport* Export = &Exports[i];
    ObjName = ResolveNameFromIdx( Export->ObjectName );
    if ( stricmp( ObjName, "None" ) == 0 )
      continue;

    ClassName = ResolveNameFromObjRef( Export->Class );
    for ( int j = 0; j < (sizeof(Types)/sizeof(const char*)); j++ )
    {
      if ( stricmp( ClassName, Types[j] ) == 0 )
      {
        UObject::StaticLoadObject( this, Export, NULL, NULL, true );
        break;
      }
    }
  }
}

bool UPackage::StaticInit()
{
  if ( !Packages )
  {
    LoadOpts = PO_OpenOnLoad;
    Packages = new TArray<UPackage*>();
    if ( Packages == NULL )
      return false;
    Packages->Reserve( 8 );
  }
  return true;
}

void UPackage::StaticExit( bool bCrashExit )
{
  if ( UPackage::Packages != NULL && !bCrashExit )
    delete Packages;
}

UPackage* UPackage::StaticLoadPackage( const char* PkgName, bool bSearch )
{
  if ( PkgName == NULL )
    return NULL;

  char* ActualName = (char*)PkgName;
  if ( !bSearch )
  {
    ActualName = strdup( PkgName );
    ActualName = strrchr( ActualName, DIRECTORY_SEPARATOR );
    ActualName++;
    
    char* Dot = (char*)strchr( ActualName, '.' );
    if ( Dot == NULL )
    {
      GLogf( LOG_ERR, "Package name does not have file extension" );
      FGlobalMem::Free( ActualName );
      return NULL;
    }

    *Dot = '\0';
  }

  // See if the package has been loaded before
  UPackage* Pkg = NULL;
  for ( int i = 0; i < Packages->Size(); i++ )
  {
    UPackage* Iter = Packages->Data()[i];
    if ( Iter == NULL )
      break;

    if ( stricmp( Iter->Name.Data(), ActualName ) == 0 )
    {
      Pkg = Iter;
      break;
    }
  }

  const char* Path = (bSearch) ? GSystem->ResolvePath( ActualName ) : PkgName;
  if ( Pkg == NULL )
  {
    if ( Path == NULL )
      return NULL;
    
    Pkg = new UPackage();
    if ( Pkg == NULL )
      return NULL;

    if ( !Pkg->Load( Path ) )
    {
      delete Pkg;
      return NULL;
    }

    Pkg->Name = FName( ActualName, RF_LoadContextFlags );
    Packages->PushBack( Pkg );
  }

  return Pkg;
}

UPackage* UPackage::StaticCreatePackage( const char* InName, UNativeModule* InNativeModule )
{
  UPackage* Pkg = new UPackage( InName, InNativeModule );
  Packages->PushBack( Pkg );

  // Initialize names that we know we'll need
  Pkg->AddName( "None", RF_TagExp | RF_HighlightedName | RF_LoadContextFlags | RF_Native );
  Pkg->AddName( "InternalTime", RF_TagExp | RF_LoadContextFlags );  // I don't know if we need this one or not
  Pkg->AddName( InName, RF_TagExp | RF_LoadContextFlags );

  // We'll at least need to mark Engine.u as an import
  UPackage* EnginePkg = StaticLoadPackage( "Engine" );
  if ( !EnginePkg )
  {
    GLogf( LOG_CRIT, "Failed to load Engine.u in StaticCreatePackage???" );
    GSystem->Exit( -1 );
  }

  Pkg->AddImport( EnginePkg );

  return Pkg;
}

TArray<UPackage*>* UPackage::GetLoadedPackages()
{
  return Packages;
}

IMPLEMENT_NATIVE_CLASS( UPackage );
