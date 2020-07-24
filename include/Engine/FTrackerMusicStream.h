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

//#include <dumb.h>
#include <libopenmpt/libopenmpt_stream_callbacks_buffer.h>

enum EOpenMptCtls
{
  OCTL_LoadSkipSamples,
  OCTL_LoadSkipPatterns,
  OCTL_LoadSkipPlugins,
  OCTL_LoadSkipSubsongsInit,
  OCTL_SeekSyncSamples,
  OCTL_Subsong,
  OCTL_PlayAtEnd,
  OCTL_PlayTempoFactor,
  OCTL_PlayPitchFactor,
  OCTL_RenderResamplerEmulateAmiga,
  OCTL_RenderResamplerEmulateAmigaType,
  OCTL_RenderOplVolumeFactor,
  OCTL_Dither,
};

class FTrackerMusicStream : public FMusicStream
{
/*
  DUMBFILE* DumbFile;
  DUH* Duh;
  DUH_SIGRENDERER* DuhRenderer;
  sample_t** Samples;
*/
  //openmpt_stream_buffer* StreamBuffer;
  openmpt_module_initial_ctl Controls[13] = 
  {
    { "load.skip_samples", "0" },
    { "load.skip_patterns", "0" },
    { "load.skip_plugins", "0" },
    { "load.skip_subsongs_init", "0" },
    { "seek.sync_samples", "0" },
    { "subsong", "0" },
    { "play.at_end", "continue" },
    { "play.tempo_factor", "1.0" },
    { "play.pitch_factor", "1.0" },
    { "render.resampler.emulate_amiga", "1" },
    { "render.resampler.emulate_amiga_type", "auto" },
    { "render.opl.volume_factor", "1.0" },
    { "dither", "0" },
  };
  openmpt_module* Module;
  long NumSamples;
  static const char* ErrorString;

public:
  static void OpenMptLogFunc( const char* Msg, void* _Unused )
  {
    GLogf( LOG_INFO, Msg );
  }
  static int OpenMptErrFunc( int Error, void* _Unused )
  {
    GLogf( LOG_ERR, "OpenMPT error: %s (%i)", Error, ErrorString );
    return OPENMPT_ERROR_FUNC_RESULT_STORE;
  }

  bool Init( UMusic* Music, int Section )
  {
    bool bModuleSupportsSkipping = false;

    //openmpt_stream_buffer_init(StreamBuffer, Music->ChunkData, Music->ChunkSize);
    char SectionString[4];
    sprintf(SectionString, "%i", Section);
    Controls[OCTL_Subsong].value = strdup( SectionString );

    Module = openmpt_module_create_from_memory2
    (
      Music->ChunkData, 
      Music->ChunkSize, 
      OpenMptLogFunc,
      NULL,
      OpenMptErrFunc,
      NULL,
      NULL,
      &ErrorString,
      Controls
    );

    if ( !Module )
    {
      GLogf( LOG_ERR, "Failed to init tracker module '%s'", Music->Name.Data() );
      return false;
    }
    
    StreamFormat = STREAM_Stereo16;
    StreamRate = GEngine->Audio->OutputRate;

    return true;
  }

  void Exit()
  {
    openmpt_module_destroy( Module );
    Module = NULL;
  }

  void GetPCM( void* Buffer, int Num )
  {
    // 4 comes from (Bits / 8) * NumChannels, which is (16 / 8) * 2 = 2 * 2 = 4;
    //duh_render_int( DuhRenderer, &Samples, &NumSamples, 16, 0, 1.0f, 65536.0f / (float)StreamRate, Num / 4, Buffer );
    openmpt_module_read_interleaved_stereo( Module, StreamRate, Num / 4, (int16_t*)Buffer );
  }
};

const char* FTrackerMusicStream::ErrorString = NULL;
