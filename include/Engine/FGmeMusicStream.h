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
 * FGmeMusicStream.h - Plays back music files from various retro game systems
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <gme/gme.h>

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
