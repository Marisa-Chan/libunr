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
