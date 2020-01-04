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
 * FMp3MusicStream.h - Plays back music from Ogg Vorbis files
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <mpg123.h>

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
