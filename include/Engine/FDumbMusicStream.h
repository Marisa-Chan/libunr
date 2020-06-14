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
 * FDumbMusicStream.h - Plays back module tracker music
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <dumb.h>

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
      GLogf( LOG_ERR, "Failed to open dumbfile for music '%s'", Music->Name.Data() );
      return false;
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

    if ( Duh == NULL )
    {
      GLogf( LOG_ERR, "Failed to create DUH for tracker music '%s'", Music->Name.Data() );
      return false;
    }

    if ( bModuleSupportsSkipping )
      DuhRenderer = dumb_it_start_at_order( Duh, 2, Section );
    else
      DuhRenderer = duh_start_sigrenderer( Duh, 0, 2, 0 );

    if ( DuhRenderer == NULL )
    {
      GLogf( LOG_ERR, "Failed to create sigrenderer for music '%s", Music->Name.Data() );
      return false;
    }

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

  void GetPCM( void* Buffer, int Num )
  {
    // 4 comes from (Bits / 8) * NumChannels, which is (16 / 8) * 2 = 2 * 2 = 4;
    duh_render_int( DuhRenderer, &Samples, &NumSamples, 16, 0, 1.0f, 65536.0f / (float)StreamRate, Num / 4, Buffer );
  }
};
