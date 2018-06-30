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
 * UMusic.cpp - Native music object type
 * See the 'Class Music' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UMusic.h"
#include "UPackage.h"

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

void UMusic::LoadFromPackage( FPackageFileIn& Ar )
{
  Ar >> ChunkCount;
  
  if ( Ar.Ver > PKG_VER_UN_200 )
    Ar >> _unknown0;
  
  Ar >> CINDEX( ChunkSize );
  
  ChunkData = new u8[ChunkSize];
  Ar.Read( ChunkData, ChunkSize );
}

bool UMusic::ExportToFile()
{
  // Set up filename
  String* Filename = new String( Pkg->ResolveNameFromIdx( NameIdx ) );
  const char* Ext = Pkg->ResolveNameFromIdx( MusicType );
  Filename->Append( "." );
  Filename->Append( Ext );
  
  // Open file
  FileStreamOut* Out = new FileStreamOut();
  Out->Open( *Filename );
  
  // Write
  Out->Write( ChunkData, ChunkSize );
  
  // Close
  Out->Close();
  delete Out;
  delete Filename;  
  return true;
}
