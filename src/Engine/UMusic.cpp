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

// Dependency headers
#include <dumb.h>
#include <vorbis/vorbisfile.h>
#include <mpg123.h>
#include <gme.h>

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
  bool Init( UMusic* Music, int Section )
  {
    bool bModuleSupportsSkipping = false;
    DumbFile = dumbfile_open_memory( (const char*)Music->ChunkData, Music->ChunkSize );
    if ( DumbFile == NULL )
    {

    }

    // We can't rely on the music type because some tracks decided
    // to have an incorrect name while the actual music is ImpulseTracker

    // Check for ImpulseTracker, most likely scenario
    if ( strnicmp( (const char*)Music->ChunkData, "IMPM", 4 ) == 0 )
    {
      Duh = dumb_read_it_quick( DumbFile );
      bModuleSupportsSkipping = true;
    }

    // Check for XM, second most likely
    else if ( strnicmp( (const char*)Music->ChunkData, "Extended module:", 16 ) == 0 )
    {
      Duh = dumb_read_xm_quick( DumbFile );
      bModuleSupportsSkipping = true;
    }

    // At this point, we have to just hope that the type matches the actual format
    else
    {
      switch ( Music->MusicType )
      {
        case NAME_S3M:
          Duh = dumb_read_s3m_quick( DumbFile );
          bModuleSupportsSkipping = true;
          break;
        case NAME_Mod:
          Duh = dumb_read_mod_quick( DumbFile, 0 );
          bModuleSupportsSkipping = true;
          break;
        case NAME_Stm:
          Duh = dumb_read_stm_quick( DumbFile );
        case NAME_669:
          Duh = dumb_read_669_quick( DumbFile );
          break;
        case NAME_Ptm:
          Duh = dumb_read_ptm_quick( DumbFile );
          break;
        case NAME_Psm:
          Duh = dumb_read_psm_quick( DumbFile, 0 );
          if ( Duh == NULL )
            Duh = dumb_read_old_psm_quick( DumbFile );
          break;
        case NAME_Mtm:
          Duh = dumb_read_mtm_quick( DumbFile );
          break;
        case NAME_Riff:
          Duh = dumb_read_riff_quick( DumbFile );
          break;
        case NAME_Asy:
          Duh = dumb_read_asy_quick( DumbFile );
          break;
        case NAME_Amf:
          Duh = dumb_read_amf_quick( DumbFile );
          break;
        case NAME_Okt:
          Duh = dumb_read_okt_quick( DumbFile );
          break;
      }
    }

    if ( bModuleSupportsSkipping )
      DuhRenderer = dumb_it_start_at_order( Duh, 2, Section );
    else
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
};

/*-----------------------------------------------------------------------------
 * FOggMusicStream
 * Plays back music from Ogg Vorbis files
-----------------------------------------------------------------------------*/
class FOggMusicStream : public FMusicStream
{
  struct PrivateData
  {
    FOggMusicStream* Stream;
    UMusic* Music;
  };
  PrivateData PrivData;

  OggVorbis_File VorbisFile;
  vorbis_info* VorbisInfo;
  ov_callbacks Callbacks;

  size_t Pos;
  size_t Len;

  /*-----------------------------------------------------------------------------
   * Class Methods
  -----------------------------------------------------------------------------*/

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

  int VorbisSeek( i64 Offset, int Whence )
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

  /*-----------------------------------------------------------------------------
   * Static Methods
  -----------------------------------------------------------------------------*/

  static size_t StaticVorbisRead( void* Dest, size_t Size, size_t Num, void* Data )
  {
    PrivateData* PrivDataPtr = (PrivateData*)Data;
    return PrivDataPtr->Stream->VorbisRead( Dest, Size, Num, PrivDataPtr->Music->ChunkData );
  }

  static int StaticVorbisSeek( void* Data, ogg_int64_t Offset, int Whence )
  {
    PrivateData* PrivDataPtr = (PrivateData*)Data;
    return PrivDataPtr->Stream->VorbisSeek( Offset, Whence );
  }

  static long StaticVorbisTell( void* Data )
  {
    PrivateData* PrivDataPtr = (PrivateData*)Data;
    return PrivDataPtr->Stream->Pos;
  }

public:
  bool Init( UMusic* Music, int Section )
  {
    PrivData.Stream = this;
    PrivData.Music = Music;

    Callbacks.read_func  = StaticVorbisRead;
    Callbacks.seek_func  = StaticVorbisSeek;
    Callbacks.close_func = NULL;
    Callbacks.tell_func  = StaticVorbisTell;

    Pos = 0;
    Len = Music->ChunkSize;

    if ( ov_open_callbacks( &PrivData, &VorbisFile, NULL, 0, Callbacks ) < 0 )
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
      GLogf( LOG_ERR, "Failed to get PCM from ogg stream for music '%s'", PrivData.Music->Name.Data() );
  }
};

/*-----------------------------------------------------------------------------
 * FMp3MusicStream
 * Plays back MP3 music
-----------------------------------------------------------------------------*/
class FMp3MusicStream : public FMusicStream
{
  struct PrivateData
  {
    FMp3MusicStream* Stream;
    UMusic* Music;
  };
  PrivateData PrivData;

  mpg123_handle* Handle;
  size_t Pos;
  size_t Len;

  static ssize_t MpgRead( void* Data, void* Dest, size_t Num )
  {
    PrivateData* PrivData = (PrivateData*)Data;
    FMp3MusicStream* Stream = PrivData->Stream;

    ssize_t Ret = 0;

    if ( Stream->Pos < Stream->Len )
    {
      size_t RealSize = MIN( Num, Stream->Len - Stream->Pos );
      void* Src = PrivData->Music->ChunkData;

      memcpy( Dest, PtrAdd( Src, Stream->Pos ), RealSize );
      Stream->Pos += Num;
      Ret += Num;
    }

    return Ret;
  }

  static off_t MpgSeek( void* Data, off_t Offset, int Whence )
  {
    PrivateData* PrivData = (PrivateData*)Data;
    FMp3MusicStream* Stream = PrivData->Stream;

    switch ( Whence )
    {
    case SEEK_SET:
      if ( Offset > Stream->Len )
        Stream->Pos = Stream->Len;
      else
        Stream->Pos = Offset;
      break;
    case SEEK_CUR:
      if ( Offset > 0 && Stream->Pos + Offset > Stream->Len )
        Stream->Pos = Stream->Len;
      else if ( Offset < 0 && Stream->Pos - Offset < 0 )
        Stream->Pos = 0;
      else
        Stream->Pos += Offset;
      break;
    case SEEK_END:
      if ( Offset > Stream->Len )
        Stream->Pos = 0;
      else
        Stream->Pos = Stream->Len - Offset;
      break;
    }

    return -1;
  }

  static void MpgClose( void* Data )
  {
    // No cleanup needed
  }

public:
  bool Init( UMusic* Music, int Section )
  {
    // Init mpg123
    if ( mpg123_init() != MPG123_OK )
    {
      GLogf( LOG_ERR, "Failed to initialize mpg123 library, cannot play mp3 music '%s'", Music->Name.Data() );
      return false;
    }

    PrivData.Stream = this;
    PrivData.Music = Music;
    
    Pos = 0;
    Len = Music->ChunkSize;
    
    // Create mp3 handle
    int Error = 0;
    Handle = mpg123_new( "generic", &Error );
    if ( Handle == NULL )
    {
      GLogf( LOG_ERR, "Failed to initialize mpg123 decoder, cannot play mp3 music '%s'", Music->Name.Data() );
      return false;
    }

    // Replace reader handles
    if ( mpg123_replace_reader_handle( Handle, MpgRead, MpgSeek, MpgClose ) != MPG123_OK )
    {
      GLogf( LOG_ERR, "Failed to replace reader functions for mp3 file '%s'", Music->Name.Data() );
      return false;
    }

    // Open the buffer
    if ( mpg123_open_handle( Handle, &PrivData ) != MPG123_OK )
    {
      GLogf( LOG_ERR, "Failed to register MP3 with mpg123, cannot play mp3 music '%s'", Music->Name.Data() );
      return false;
    }

    mpg123_seek( Handle, 0, SEEK_SET );
    Pos = 0;

    // Set stream properties
    StreamFormat = STREAM_Stereo16;
    mpg123_format( Handle, 0, 2, MPG123_ENC_SIGNED_16 );

    mpg123_frameinfo FrameInfo;
    mpg123_info( Handle, &FrameInfo );

    StreamRate = FrameInfo.rate;

    return true;
  }

  void Exit()
  {
    mpg123_close( Handle );
    mpg123_delete( Handle );
    mpg123_exit();
  }

  void GetPCM( void* Buffer, size_t Num )
  {
    size_t Bytes = 0;
    if ( mpg123_read( Handle, (u8*)Buffer, Num, &Bytes ) != MPG123_OK )
      GLogf( LOG_ERR, "Failed to read mp3 bytes for music '%s'", PrivData.Music->Name.Data() );
  }
};

/*-----------------------------------------------------------------------------
 * FGmeMusicStream
 * Plays back music files from various retro game systems
-----------------------------------------------------------------------------*/
class FGmeMusicStream : public FMusicStream
{
  UMusic* Music;
  Music_Emu* MusicEmu;

public:
  bool Init( UMusic* InMusic, int Section )
  {
    MusicEmu = NULL;
    Music = InMusic;
    StreamFormat = STREAM_Stereo16;
    StreamRate = GEngine->Audio->OutputRate;

    // Open music data
    const char* Err = gme_open_data( Music->ChunkData, Music->ChunkSize, &MusicEmu, StreamRate );
    if ( Err )
    {
      GLogf( LOG_ERR, "Failed to initialize game-music-emu, cannot play music '%s' (%s)", Music->Name.Data(), Err );
      return false;
    }

    // Start playback
    Err = gme_start_track( MusicEmu, Section );
    if ( Err )
    {
      GLogf( LOG_ERR, "Failed to start music '%s' (%s)", Music->Name.Data(), Err );
      return false;
    }

    return true;
  }

  void Exit()
  {
    gme_delete( MusicEmu );
  }

  void GetPCM( void* Buffer, size_t Num )
  {
    // Samples are always in stereo at 16-bit, divide Num by 2
    const char* Err = gme_play( MusicEmu, Num / 2, (i16*)Buffer );
    if ( Err )
      GLogf( LOG_ERR, "Failed to play gme sample from music '%s' (%s)", Music->Name.Data(), Err );
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

