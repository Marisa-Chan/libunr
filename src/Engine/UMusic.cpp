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

class FDumbMusic
{
public:
  static DUMBFILE* DumbFile;
  static DUH* Duh;
  static DUH_SIGRENDERER* DuhRenderer;
  static sample_t** Samples;
  static long NumSamples;

  static void RegisterMusic( UMusic* Music )
  {
    DumbFile = dumbfile_open_memory( (const char*)Music->ChunkData, Music->ChunkSize );
    
    switch ( Music->MusicType )
    {
    case NAME_It:
      Duh = dumb_read_it_quick( DumbFile );
      break;
    case NAME_Xm:
      Duh = dumb_read_xm_quick( DumbFile );
      break;
    case NAME_S3M:
      Duh = dumb_read_s3m_quick( DumbFile );
      break;
    case NAME_Mod:
      Duh = dumb_read_mod_quick( DumbFile, 0 );
      break;
    }

    DuhRenderer = duh_start_sigrenderer( Duh, 0, 2, 0 );
  }

  static void UnregisterMusic( UMusic* Music )
  {
    duh_end_sigrenderer( DuhRenderer );
    unload_duh( Duh );
    dumbfile_close( DumbFile );

    Duh = NULL;
    DumbFile = NULL;
    DuhRenderer = NULL;
  }

  static void RenderMusic( float* Buf, size_t Size )
  {
    duh_render_float( DuhRenderer, &Samples, &NumSamples, 16, 1.0, 65536.0f / GEngine->Audio->OutputRate, Size, Buf );
  }
};

DUMBFILE* FDumbMusic::DumbFile;
DUH* FDumbMusic::Duh;
DUH_SIGRENDERER* FDumbMusic::DuhRenderer;
sample_t** FDumbMusic::Samples;
long FDumbMusic::NumSamples;

/*-----------------------------------------------------------------------------
 * FMusicPlayer
-----------------------------------------------------------------------------*/
FMusicPlayer::FMusicPlayer()
{
  CurrentTrack = NULL;
  QueuedTrack = NULL;
  CurrentSection = 255;
  QueuedSection = 255;
  float CurrentVolume = 0.0f;
  float TargetVolume = 0.0f;
  CurrentTransition = MTRAN_None;
  bTransitioning = false;
  RequestExit = 0;
  ThreadErr = 0;

  MusicThread = GSystem->RunThread( StaticThreadFunc, this );
  if ( MusicThread == NULL )
    GLogf( LOG_CRIT, "Failed to initialize music player thread" );
}

FMusicPlayer::~FMusicPlayer()
{
  RequestExit = 1;
 // while ( GSystem->IsThreadActive( MusicThread ) );
}

int FMusicPlayer::GetStatus()
{
  return ThreadErr;
}

void FMusicPlayer::ClearStatus()
{
  ThreadErr = 0;
}

void FMusicPlayer::Play( UMusic* Music, int SongSection, EMusicTransition Transition )
{
  QueuedTrack = Music;
  QueuedSection = SongSection;
  CurrentTransition = Transition;
  bTransitioning = true;
}

void FMusicPlayer::Stop( EMusicTransition Transition )
{
  QueuedTrack = NULL;
  QueuedSection = 255;
  CurrentTransition = Transition;
  bTransitioning = true;
}

float FMusicPlayer::GetCurrentVolume()
{
  return CurrentVolume;
}

ThreadReturnType FMusicPlayer::StaticThreadFunc( void* Args )
{
  return ((FMusicPlayer*)Args)->PlayerThread();
}

#define FRAME_COUNT 32
ThreadReturnType FMusicPlayer::PlayerThread()
{
  bool bSongChanged = false;
  double LastTime = USystem::GetSeconds();
  double CurrentTime = 0.0;
  float* RenderBuffer = new float[FRAME_COUNT];

  UAudioSubsystem* Audio = GEngine->Audio;
  if ( Audio == NULL )
    ThreadErr = ERR_NO_AUDIO_DEVICE;

  // Don't exit until RequestExit has been set
  while ( !RequestExit )
  {
    if ( ThreadErr < 0 )
      continue;

    LastTime = CurrentTime;
    CurrentTime = USystem::GetSeconds();
    float DeltaTime = CurrentTime - LastTime;
    bool bSetFadeTime = false;

    // Handle currently playing track
    if ( CurrentTrack != NULL )
    {
      // Render music frames and play back
      RenderMusic( RenderBuffer, FRAME_COUNT );
      GEngine->Audio->PlayMusicBuffer( RenderBuffer, FRAME_COUNT );

      // Handle music transition if needed
      if ( bTransitioning )
      {
        // TODO: Settle on values that sound similar to UE1
        float FadeRate = 0.0f;
        if ( !bSetFadeTime )
        {
          switch ( CurrentTransition )
          {
          case MTRAN_None:
          case MTRAN_Instant:
          case MTRAN_Segue:
            FadeRate = 1000.0f;
            break;
          case MTRAN_Fade:
            FadeRate = 0.5f;
            break;
          case MTRAN_SlowFade:
            FadeRate = 0.25f;
            break;
          case MTRAN_FastFade:
            FadeRate = 0.75f;
            break;
          }
          bSetFadeTime = true;
        }

        // Adjust volume
        if ( CurrentTrack != QueuedTrack || CurrentSection != QueuedSection )
          CurrentVolume -= FadeRate * DeltaTime;
        else
          CurrentVolume += FadeRate * DeltaTime;

        // Adjust music state if fade in or out is completed
        if ( CurrentVolume <= FLT_EPSILON )
        {
          if ( CurrentTrack != QueuedTrack )
          {
            // Unload current track and set up new track
            UnregisterMusic( CurrentTrack );

            // Handle music stop if needed
            if ( QueuedTrack == NULL )
            {
              CurrentSection = 255;
              continue;
            }

            CurrentTrack = QueuedTrack;
            RegisterMusic( CurrentTrack );
          }
          else if ( CurrentSection != QueuedSection )
          {
            // !!! Handle section change
          }
          CurrentVolume = 0.0f;
        }
        else if ( fabsf( CurrentVolume - TargetVolume ) <= FLT_EPSILON )
        {
          // Fade in ended, thus ending fade sequence
          CurrentTransition = MTRAN_None;
          CurrentVolume = 1.0f;
          bSetFadeTime = false;
          bTransitioning = false;
        }
      }
    }

    // Handle queued track when no other track is playing
    else if ( QueuedTrack != NULL )
    {
      RegisterMusic( QueuedTrack );
      CurrentTrack = QueuedTrack;
      CurrentSection = QueuedSection;
      bTransitioning = true;
    }
  }

  // TODO: Exit thread
  delete[] RenderBuffer;
  return THREAD_SUCCESS;
}

void FMusicPlayer::RegisterMusic( UMusic* Music )
{
  switch ( Music->MusicType )
  {
    case NAME_It:
    case NAME_Xm:
    case NAME_S3M:
    case NAME_Mod:
      FDumbMusic::RegisterMusic( Music );
      break;
  }
}

void FMusicPlayer::UnregisterMusic( UMusic* Music )
{
  switch ( Music->MusicType )
  {
    case NAME_It:
    case NAME_Xm:
    case NAME_S3M:
    case NAME_Mod:
      FDumbMusic::UnregisterMusic( Music );
      break;
  }
}

void FMusicPlayer::RenderMusic( float* Buf, size_t Size )
{
  switch ( CurrentTrack->MusicType )
  {
  case NAME_It:
  case NAME_Xm:
  case NAME_S3M:
  case NAME_Mod:
    FDumbMusic::RenderMusic( Buf, Size );
    break;
  }
}

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

