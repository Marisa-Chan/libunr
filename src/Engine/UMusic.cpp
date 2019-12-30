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
#include <dumb.h>

class FDumbMusicStream : public FMusicStream
{
public:
  DUMBFILE* DumbFile;
  DUH* Duh;
  DUH_SIGRENDERER* DuhRenderer;

  bool Init( UMusic* Music )
  {
    DumbFile = dumbfile_open_memory( (const char*)Music->ChunkData, Music->ChunkSize );
    
    // We can't rely on the music type because some tracks decided
    // to have an incorrect name while the actual music is ImpulseTracker

    // Check for ImpulseTracker, most likely scenario
    if ( strnicmp( (const char*)Music->ChunkData, "IMPM", 4 ) == 0 )
      Duh = dumb_read_it_quick( DumbFile );

    // Check for XM, second most likely
    else if ( strnicmp( (const char*)Music->ChunkData, "Extended module:", 16 ) == 0 )
      Duh = dumb_read_xm_quick( DumbFile );

    // At this point, we have to just hope that the type matches the actual format
    else if ( Music->MusicType == NAME_S3M )
      Duh = dumb_read_s3m_quick( DumbFile );

    else if ( Music->MusicType == NAME_Mod )
      Duh = dumb_read_mod_quick( DumbFile, 0 );

    DuhRenderer = duh_start_sigrenderer( Duh, 0, 2, 0 );
    return true;
  }

  void Exit()
  {
    duh_end_sigrenderer( DuhRenderer );
    unload_duh( Duh );
    dumbfile_close( DumbFile );

    Duh = NULL;
    DumbFile = NULL;
    DuhRenderer = NULL;
  }

  void GetPCM( void* Buffer, size_t Num )
  {
    duh_sigrenderer_generate_samples( DuhRenderer, 1.0, 65536.0 / GEngine->Audio->OutputRate, Num, (sample_t**)&Buffer );
  }

  void GoToSection( int Section )
  {

  }
};

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

  // For some reason, "s3m" and "S3M" don't hash to the same thing
  // with a case insensitive SuperFastHash. Force upper case
  FNameEntry& MusicTypeEntry = Pkg->GetNameTable()[0];
  for ( int i = 0; i < 64; i++ )
  {
    if ( MusicTypeEntry.Data[i] == '\0' )
      break;

    MusicTypeEntry.Data[i] = toupper( MusicTypeEntry.Data[i] );
  }
  MusicTypeEntry.Hash = SuperFastHashString( MusicTypeEntry.Data );
  MusicType = FName( Pkg->GetNameTable()[0] );

  // Set up stream
  switch ( MusicType )
  {
    case NAME_It:
    case NAME_Xm:
    case NAME_S3M:
    case NAME_Mod:
      Stream = new FDumbMusicStream();
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

