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
 * UAudio.h - Base audio subsystem functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Engine/UAudio.h"
#include "Engine/UEngine.h"

UAudioSubsystem::UAudioSubsystem()
  : USubsystem()
{
  MusicVolume = 0;
  OutputRate = 0;
  SoundVolume = 0;

  CurrentTrack = NULL;
  QueuedTrack = NULL;
  CurrentSection = 255;
  QueuedSection = 255;
  float CurrentVolume = 0.0f;
  float TargetVolume = 0.0f;
  CurrentTransition = MTRAN_None;
  bTransitioning = false;
}

UAudioSubsystem::~UAudioSubsystem()
{
}

bool UAudioSubsystem::Init()
{
  // Initialize music buffer and ring queue
  MusicBuffer = new i16[MusicBufferCount * MusicBufferSize];
  memset( MusicBuffer, 0, MusicBufferCount * MusicBufferSize * sizeof( i16 ) );

  MusicQueue = new TRingQueue<i16*>( MusicBufferCount );

  // Fill out each buffer slot in the queue
  for ( int i = 0; i < MusicBufferCount; i++ )
  {
    MusicQueue->Push( &MusicBuffer[MusicBufferSize * i] );
    MusicQueue->Pop();
  }
  return true;
}

void UAudioSubsystem::Tick( float DeltaTime )
{
  // Handle currently playing track
  if ( CurrentTrack != NULL )
  {
    while ( MusicQueue->Size() < MusicBufferCount )
    {
      // Render a chunk of music
      CurrentTrack->Stream->GetPCM( MusicQueue->GetNextFree(), MusicBufferSize );

      // Push buffer into the circular queue
      MusicQueue->Push( MusicQueue->GetNextFree() );
    }

    // Play a rendered buffer (or start playback)
    if ( !bPlaying )
    {
      StartMusicPlayback();
      bPlaying = true;
    }
    else if ( MusicQueue->Size() > 0 )
      PlayMusicBuffer();

    // Handle music transition if needed
    if ( bTransitioning )
    {
      // TODO: Settle on values that sound similar to UE1
      if ( FadeRate == 0.0f )
      {
        switch ( CurrentTransition )
        {
        case MTRAN_None:
        case MTRAN_Instant:
        case MTRAN_Segue:
          FadeRate = 10000000.0f;
          break;
        case MTRAN_Fade:
          FadeRate = 50.0f;
          break;
        case MTRAN_SlowFade:
          FadeRate = 25.0f;
          break;
        case MTRAN_FastFade:
          FadeRate = 75.0f;
          break;
        }

        if ( CurrentTrack != QueuedTrack || CurrentSection != QueuedSection )
          TargetVolume = 0.0f;
        else
          TargetVolume = 1.0f;
      }

      // Adjust volume
      if ( CurrentTrack != QueuedTrack || CurrentSection != QueuedSection )
        CurrentMusicVolume = MAX( (CurrentMusicVolume)-FadeRate * DeltaTime, 0.0f );
      else
        CurrentMusicVolume = MIN( (CurrentMusicVolume)+FadeRate * DeltaTime, 1.0f );

      // Adjust music state if fade in or out is completed
      if ( CurrentMusicVolume <= FLT_EPSILON )
      {
        if ( CurrentTrack != QueuedTrack || CurrentSection != QueuedSection )
        {
          // Unload current track and set up new track
          CurrentTrack->Stream->Exit();

          // Handle music stop if needed
          if ( QueuedTrack == NULL )
          {
            CurrentTrack = NULL;
            CurrentSection = 255;
            bPlaying = false;
            MusicQueue->Clear();
            return;
          }

          CurrentTrack = QueuedTrack;
          CurrentSection = QueuedSection;

          if ( !CurrentTrack->Stream->Init( CurrentTrack, CurrentSection ) )
          {
            GLogf( LOG_ERR, "Failed to initialize music stream for '%s'", CurrentTrack->Name.Data() );
            CurrentTrack = NULL;
            QueuedTrack = NULL;
            return;
          }

          CurrentStreamFormat = CurrentTrack->Stream->GetStreamFormat();
          CurrentStreamRate = CurrentTrack->Stream->GetStreamRate();
        }
        CurrentMusicVolume = 0.0f;
      }
      else if ( fabsf( CurrentMusicVolume - TargetVolume ) <= FLT_EPSILON )
      {
        // Fade in ended, thus ending fade sequence
        CurrentTransition = MTRAN_None;
        CurrentMusicVolume = 1.0f;
        FadeRate = 0.0f;
        bTransitioning = false;
      }
    }
  }

  // Handle queued track when no other track is playing
  else if ( QueuedTrack != NULL )
  {
    CurrentTrack = QueuedTrack;
    CurrentSection = QueuedSection;
    if ( !CurrentTrack->Stream->Init( CurrentTrack, CurrentSection ) )
    {
      GLogf( LOG_ERR, "Failed to initialize music stream for '%s'", CurrentTrack->Name.Data() );
      CurrentTrack = NULL;
      QueuedTrack = NULL;
      return;
    }
    CurrentStreamFormat = CurrentTrack->Stream->GetStreamFormat();
    CurrentStreamRate = CurrentTrack->Stream->GetStreamRate();

    CurrentSection = QueuedSection;
  }
}

void UAudioSubsystem::PlayMusic( UMusic* Music, int SongSection, EMusicTransition MusicTrans )
{
  QueuedTrack = Music;
  QueuedSection = SongSection;
  CurrentTransition = MusicTrans;
  bTransitioning = true;
}

void UAudioSubsystem::StopMusic( EMusicTransition MusicTrans )
{
  QueuedTrack = NULL;
  QueuedSection = 255;
  CurrentTransition = MusicTrans;
  bTransitioning = true;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UAudioSubsystem );

