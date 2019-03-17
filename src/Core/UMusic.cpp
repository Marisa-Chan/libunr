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
 * UMusic.cpp - Native music object type
 * See the 'Class Music' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UMusic.h"
#include "Core/UPackage.h"

UMusic::UMusic()
{
  MusicType = 0; // Index 0 in a package's name table always points to music type
  ChunkSize = 0;
  ChunkData = NULL;
}

UMusic::~UMusic()
{
  if ( ChunkData != NULL )
    delete ChunkData;
}

void UMusic::Load()
{
  *PkgFile >> ChunkCount;
  
  if ( PkgFile->Ver > PKG_VER_UN_200 )
    *PkgFile >> _unknown0;
  
  *PkgFile >> CINDEX( ChunkSize );
  
  ChunkData = new u8[ChunkSize];
  PkgFile->Read( ChunkData, ChunkSize );
}

bool UMusic::ExportToFile( const char* Dir, const char* Type )
{
  // Set up filename
  String* Filename = new String( Dir );
#if defined LIBUNR_WIN32
  Filename->ReplaceChars( '\\', '/' );
#endif
  if ( Filename->Back() != '/' )
    Filename->Append( "/" );

  Filename->Append( Name );
  Filename->Append( "." );
  Filename->Append( Pkg->ResolveNameFromIdx( MusicType ) );

  // Open file
  FileStreamOut* Out = new FileStreamOut();
  if ( Out->Open( *Filename ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export music to file '%s'", Filename );
    return false;
  }
  
  // Write
  Out->Write( ChunkData, ChunkSize );
  
  // Close
  Out->Close();
  delete Out;
  delete Filename;  
  return true;
}
