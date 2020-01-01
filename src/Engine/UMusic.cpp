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
#include <vorbis/vorbisfile.h>

/*-----------------------------------------------------------------------------
 * FDumbMusicStream
 * Plays back module tracker music
-----------------------------------------------------------------------------*/
class FDumbMusicStream : public FMusicStream
{
  DUMBFILE* DumbFile;
  DUH* Duh;
  DUH_SIGRENDERER* DuhRenderer;
  sample_t** Samples;
  long NumSamples;

public:
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

    Samples = allocate_sample_buffer( 2, GEngine->Audio->MusicBufferSize );

    StreamFormat = STREAM_Stereo16;
    StreamRate = GEngine->Audio->OutputRate;

    return true;
  }

  void Exit()
  {
    duh_end_sigrenderer( DuhRenderer );
    unload_duh( Duh );
    dumbfile_close( DumbFile );

    if ( Samples )
      destroy_sample_buffer( Samples );

    Duh = NULL;
    DumbFile = NULL;
    DuhRenderer = NULL;
  }

  void GetPCM( void* Buffer, size_t Num )
  {
    // 4 comes from (Bits / 8) * NumChannels, which is (16 / 8) * 2 = 2 * 2 = 4;
    duh_render_int( DuhRenderer, &Samples, &NumSamples, 16, 0, 1.0f, 65536.0f / (float)StreamRate, Num / 4, Buffer );
  }

  void GoToSection( int Section )
  {

  }
};

/*-----------------------------------------------------------------------------
 * FOggMusicStream
 * Plays back music from Ogg Vorbis files
-----------------------------------------------------------------------------*/
class FOggMusicStream : public FMusicStream
{
  static FOggMusicStream* StaticStream;
  OggVorbis_File VorbisFile;
  vorbis_info* VorbisInfo;
  ov_callbacks Callbacks;

  size_t Pos;
  size_t Len;

  size_t VorbisRead( void* Dest, size_t Size, size_t Num, void* Src )
  {
    size_t Ret = 0;

    if ( Pos < Len )
    {
      // Figure out which is larger, size or num. Ideally, we want to memcpy more at a time
      // We don't want to memcpy 1 byte 2048 times, but instead memcpy 2048 bytes 1 time
      Src = PtrAdd( Src, Pos );

      size_t RealSize = MAX( Size, Num );
      size_t RealNum  = MIN( Size, Num );

      for ( size_t i = 0; i < RealNum; i++ )
      {
        if ( Pos + RealSize > Len )
        {
          RealSize = Len - Pos;
          Num = 0;
        }

        memcpy( Dest, Src, RealSize );
        Dest = PtrAdd( Dest, RealSize );
        Src = PtrAdd( Src, RealSize );

        Pos += RealSize;
        Ret += RealSize;
      }
    }

    return Ret;
  }

  int VorbisSeek( void* Src, i64 Offset, int Whence )
  {
    switch ( Whence )
    {
    case SEEK_SET:
      if ( Offset > Len )
        Pos = Len;
      else
        Pos = Offset;
      break;
    case SEEK_CUR:
      if ( Offset > 0 && Pos + Offset > Len )
        Pos = Len;
      else if ( Offset < 0 && Pos - Offset < 0 )
        Pos = 0;
      else
        Pos += Offset;
      break;
    case SEEK_END:
      if ( Offset > Len )
        Pos = 0;
      else
        Pos = Len - Offset;
      break;
    default:
      return -1;
    }

    return 0;
  }

  long VorbisTell( void* Src )
  {
    return Pos;
  }

  static size_t StaticVorbisRead( void* Dest, size_t Size, size_t Num, void* Src )
  {
    return StaticStream->VorbisRead( Dest, Size, Num, Src );
  }

  static int StaticVorbisSeek( void* Src, ogg_int64_t Offset, int Whence )
  {
    return StaticStream->VorbisSeek( Src, Offset, Whence );
  }

  static long StaticVorbisTell( void* Src )
  {
    return StaticStream->VorbisTell( Src );
  }

public:
  bool Init( UMusic* Music )
  {
    StaticStream = this;
    Callbacks.read_func  = StaticVorbisRead;
    Callbacks.seek_func  = StaticVorbisSeek;
    Callbacks.close_func = NULL;
    Callbacks.tell_func  = StaticVorbisTell;

    Pos = 0;
    Len = Music->ChunkSize;

    if ( ov_open_callbacks( Music->ChunkData, &VorbisFile, NULL, 0, Callbacks ) < 0 )
    {
      GLogf( LOG_ERR, "Tried to init ogg vorbis stream for non-ogg music '%s'", Music->Name.Data() );
      return false;
    }

    VorbisInfo = ov_info( &VorbisFile, -1 );
    StreamFormat = VorbisInfo->channels == 1 ? STREAM_Mono16 : STREAM_Stereo16;
    StreamRate = VorbisInfo->rate;

    return true;
  }

  void Exit()
  {
    ov_clear( &VorbisFile );
  }

  void GetPCM( void* Buffer, size_t Num )
  {
    long IsBigEndian = 0;
    #if defined LIBUNR_BIG_ENDIAN
      IsBigEndian = 1;
    #endif

    long Ret = ov_read( &VorbisFile, (char*)Buffer, Num, IsBigEndian, 2, 1, NULL );
    if ( Ret < 0 )
      GLogf( LOG_ERR, "Failed to get PCM from ogg stream" );
  }

  void GoToSection( int SongSection )
  {
  }
};

FOggMusicStream* FOggMusicStream::StaticStream = NULL;

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
    case NAME_Mod:
      Stream = new FDumbMusicStream();
      break;
    case NAME_Ogg:
      Stream = new FOggMusicStream();
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

