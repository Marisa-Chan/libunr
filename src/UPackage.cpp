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

#include "UPackage.h"
#include "FArchiveFile.h"
#include "FMemory.h"

EPkgLoadOpts UPackage::LoadOpts = PO_OpenOnLoad;

int CalcObjRefValue( int ObjRef )
{
  if (ObjRef == 0)
    return ObjRef;
  
  else if (ObjRef < 0)
    ObjRef = -ObjRef;
  
  return ObjRef - 1;
}

FArchive& operator>>( FArchive& In, FExport& Export )
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
  
  return In;
}

FArchive& operator>>( FArchive& In, FImport& Import )
{
  In >> CINDEX( Import.ClassPackage );
  In >> CINDEX( Import.ClassName );
  In >> Import.Package;
  In >> CINDEX( Import.ObjectName );
  return In;
}

FArchive& operator>>( FArchive& In, UPackageHeader& Header )
{
  In >> Header.Signature;
  In >> Header.PackageVersion;
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

UPackage::UPackage()
{
  FMemory::Set ( &Header, 0, sizeof ( UPackageHeader ) );
  Names = new TArray<FNameEntry>();
  Exports = new TArray<FExport>();
  Imports = new TArray<FImport>();
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
  
  if (LastDirSlash != MAX_SIZE)
    Name = Path.Substr( Path.FindLastOf( "/" ) );
  else
    Name = Path;
  
  FileStream = new FArchiveFileIn();
  if ( !FileStream->Open( Path ) )
    return false;
  
  // read in the header
  *FileStream >> Header;
  
  // read in the name table
  Names->Resize( Header.NameCount );
  FileStream->Seek( Header.NameOffset, ESeekBase::Begin );
  for ( TArray<FNameEntry>::Iterator NameIt = Names->Begin(); NameIt != Names->End(); NameIt++ )
    NameIt->Read( *FileStream, Header.PackageVersion );
  
  // read in imports
  Imports->Resize( Header.ImportCount );
  FileStream->Seek( Header.ImportOffset, ESeekBase::Begin );
  for ( TArray<FImport>::Iterator Import = Imports->Begin(); Import != Imports->End(); Import++ )
    *FileStream >> *Import;
  
  // read in exports
  Exports->Resize( Header.ExportCount );
  FileStream->Seek( Header.ExportOffset, ESeekBase::Begin );
  for ( TArray<FExport>::Iterator Export = Exports->Begin(); Export != Exports->End(); Export++ )
    *FileStream >> *Export;
  
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
  return &(*Names)[Index];
}

FImport* UPackage::GetImport( size_t Index )
{
  return &(*Imports)[Index];
}

FExport* UPackage::GetExport( size_t Index )
{
  return &(*Exports)[Index];
}

const char* UPackage::GetFilePath()
{
  return Path.Data();
}

const char* UPackage::GetFileName()
{
  return Name.Data();
}

const char* UPackage::ResolveNameFromIdx( idx Index )
{
  return GetNameEntry( Index )->Name;
}

const char* UPackage::ResolveNameFromObjRef( int ObjRef )
{
  if (ObjRef == 0)
    return "None";
  else if (ObjRef < 0)
    return GetNameEntry( GetImport( CalcObjRefValue( ObjRef ) )->ObjectName )->Name;
  else
    return GetNameEntry( GetExport( CalcObjRefValue( ObjRef ) )->ObjectName )->Name;
}

bool UPackage::BeginLoad()
{
  if (FileStream == NULL)
  {
    FileStream = new FArchiveFileIn();
    return FileStream->Open( Path );
  }
  return true;
}

void UPackage::EndLoad()
{
  // Only release package handle if our load options say so
  if (LoadOpts == PO_OpenOnLoad)
  {
    FileStream->Close();
    delete FileStream;
    FileStream = NULL;
  }
}

bool UPackage::ReadRawObject( idx Index, void* Buffer )
{
  if (Index < 0 || !Buffer || !FileStream)
    return false;
  
  FExport* Export = &(*Exports)[Index];
  FileStream->Seek( Export->SerialOffset, ESeekBase::Begin );
  FileStream->Read( Buffer, Export->SerialSize );
  return true;
}
