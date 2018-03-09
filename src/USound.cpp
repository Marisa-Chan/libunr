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
 * USound.cpp - Native sound object type
 * See the 'Class Sound' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "USound.h"
#include "UPackage.h"
#include "FArchiveFile.h"

USound::USound()
{
  SoundFormat = 0;
  SoundSize = 0;
  SoundData = NULL;
}

USound::~USound()
{
  if (SoundData != NULL)
    delete SoundData;
}

void USound::LoadFromPackage( FArchive& Ar )
{
  // Read the properties, but since it always starts with None, just eat it
  idx NoneProp = 0;
  Ar >> CINDEX( NoneProp );
  
  Ar >> CINDEX( SoundFormat );
  if (Ar.Ver >= PKG_VER_UN_220)
    Ar >> OffsetNext;
  
  Ar >> CINDEX( SoundSize );
  
  SoundData = new u8[SoundSize];
  Ar.Read( SoundData, SoundSize );
}

bool USound::ExportToFile()
{
  // Set up filename
  FString* Filename = new FString( Pkg->ResolveNameFromIdx( NameIdx ) );
  const char* Ext = Pkg->ResolveNameFromIdx( SoundFormat );
  Filename->Append( "." );
  Filename->Append( Ext );
  
  // Open file
  FArchiveFileOut* Out = new FArchiveFileOut();
  Out->Open( *Filename );
  
  // Write
  Out->Write( SoundData, SoundSize );
  
  // Close
  Out->Close();
  delete Out;
  delete Filename;  
  return true;
}
