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
 * UMusic.cpp - Native music object type
 * See the 'Class Music' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UPackage.h"
#include "Core/USystem.h"
#include "Engine/UEngine.h"
#include "Engine/UMusic.h"

// Music stream implementations
#include "Engine/FDumbMusicStream.h"
#include "Engine/FOggMusicStream.h"
#include "Engine/FMp3MusicStream.h"
#include "Engine/FGmeMusicStream.h"

/*-----------------------------------------------------------------------------
 * UMusic
-----------------------------------------------------------------------------*/
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

  // Get music type from first name in the package
  // For some reason, "s3m" and "S3M" don't hash to the same thing
  // with a case insensitive SuperFastHash. Force upper case
  MusicType = FName( Pkg->GetNameTable()[0] );

  // Set up stream
  switch ( MusicType )
  {
    case NAME_It:
    case NAME_Xm:
    case NAME_S3M:
    case NAME_Stm:
    case NAME_Mod:
    case NAME_Ptm:
    case NAME_669:
    case NAME_Psm:
    case NAME_Mtm:
    case NAME_Riff:
    case NAME_Asy:
    case NAME_Amf:
    case NAME_Okt:
      Stream = new FDumbMusicStream();
      break;
    case NAME_Ogg:
      Stream = new FOggMusicStream();
      break;
    case NAME_Mp3:
      Stream = new FMp3MusicStream();
      break;
    case NAME_Ay:
    case NAME_Gbs:
    case NAME_Gym:
    case NAME_Hes:
    case NAME_Kss:
    case NAME_Nsf:
    case NAME_Nsfe:
    case NAME_Sap:
    case NAME_Spc:
    case NAME_Vgm:
      Stream = new FGmeMusicStream();
      break;
    default:
      GLogf( LOG_WARN, "Unsupported music type '%s' loaded", MusicType.Data() );
      break;
  }
}

bool UMusic::ExportToFile( const char* Dir, const char* Type )
{
  const char* Ext = MusicType.Data();

  // Set up filename
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += "/";

  Filename += Name;
  Filename += ".";
  Filename += Ext;

  GLogf( LOG_INFO, "Exporting %s.%s", Name.Data(), Ext );

  // Open file
  FFileArchiveOut* Out = new FFileArchiveOut();
  if ( Out->Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export music to file '%s'", Filename.Data() );
    return false;
  }
  
  // Write
  Out->Write( ChunkData, ChunkSize );
  
  // Close
  Out->Close();
  delete Out;
  return true;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UMusic );

