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

/*
std::istream& FExport::operator>> ( std::istream& In, FExport& Export )
{
  Class        = ReadCompactIndex ( In );
  Super        = ReadCompactIndex ( In );
  In          >> Group;
  ObjectName   = ReadCompactIndex ( In );
  In          >> ObjectFlags;
  SerialSize   = ReadCompactIndex ( In );
  SerialOffset = ReadCompactIndex ( In );
}
*/
FArchive& operator>> ( FArchive& In, UPackageHeader& Header )
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
  In.Read ( Header.GUID, 16 );
  return In;
}

UPackage::UPackage()
{
  FMemory::Set ( &Header, 0, sizeof ( UPackageHeader ) );
  Names = new TArray<FName>();
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
  
  FArchiveFileIn* FileStream = new FArchiveFileIn();
  if ( !FileStream->Open( Path ) )
    return false;
  
  // read in the header
  *FileStream >> Header;
  
  // read in name data
  Names->Resize( Header.NameCount );
  FileStream->Seek( Header.NameOffset, ESeekBase::Begin );
  for ( TArray<FName>::Iterator Name = Names->Begin(); Name != Names->End(); Name++ )
    Name->Read( *FileStream, Header.PackageVersion );

  printf( "***DEBUG***\n" );
  printf( "\tSignature = %p\n", Header.Signature );
  printf( "\tVersion   = %p\n", Header.PackageVersion );
  printf( "\tLicense   = %p\n", Header.LicenseMode );
  printf( "\tFlags     = %p\n", Header.PackageFlags );
  printf( "\tNameCnt   = %p\n", Header.NameCount );
  printf( "\tNameOff   = %p\n", Header.NameOffset );
  printf( "\tExportCnt = %p\n", Header.ExportCount );
  printf( "\tExportOff = %p\n", Header.ExportOffset );
  printf( "\tImportCnt = %p\n", Header.ImportCount );
  printf( "\tImportOff = %p\n", Header.ImportOffset );
}

bool UPackage::Save( const char* File )
{
  return false;
}

void UPackage::Close()
{
}
