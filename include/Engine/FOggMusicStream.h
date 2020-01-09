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
 * FOggMusicStream.h - Plays back music from Ogg Vorbis files
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <vorbis/vorbisfile.h>

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
      size_t RealNum = MIN( Size, Num );

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

    Callbacks.read_func = StaticVorbisRead;
    Callbacks.seek_func = StaticVorbisSeek;
    Callbacks.close_func = NULL;
    Callbacks.tell_func = StaticVorbisTell;

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
