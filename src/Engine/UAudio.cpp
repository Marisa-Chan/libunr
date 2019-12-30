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
  MusicBuffer = new i16[MUSIC_BUFFER_COUNT * MUSIC_BUFFER_SIZE];
  memset( MusicBuffer, 0, MUSIC_BUFFER_COUNT * MUSIC_BUFFER_SIZE * sizeof( i16 ) );

  MusicQueue = new TRingQueue<i16*>( MUSIC_BUFFER_COUNT );

  // Fill out each buffer slot in the queue
  for ( int i = 0; i < MUSIC_BUFFER_COUNT; i++ )
  {
    MusicQueue->Push( &MusicBuffer[MUSIC_BUFFER_SIZE * i] );
    MusicQueue->Pop();
  }
  return true;
}

void UAudioSubsystem::Tick( float DeltaTime )
{
  // Handle currently playing track
  if ( CurrentTrack != NULL )
  {
    while ( MusicQueue->Size() < MUSIC_BUFFER_COUNT )
    {
      // Render a chunk of music
      CurrentTrack->Stream->GetPCM( MusicQueue->GetNextFree(), MUSIC_BUFFER_SIZE );

      // It's kind of an abuse of how a ring queue should work, 
      // but it works fine for circular record keeping
      MusicQueue->Push( MusicQueue->GetNextFree() );
    }
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
          FadeRate = 0.5f;
          break;
        case MTRAN_SlowFade:
          FadeRate = 0.25f;
          break;
        case MTRAN_FastFade:
          FadeRate = 0.75f;
          break;
        }
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
          CurrentTrack->Stream->Exit();

          // Handle music stop if needed
          if ( QueuedTrack == NULL )
          {
            CurrentSection = 255;
            return;
          }

          CurrentTrack = QueuedTrack;
          CurrentTrack->Stream->Init( CurrentTrack );
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
        FadeRate = 0.0f;
        bTransitioning = false;
      }
    }
  }

  // Handle queued track when no other track is playing
  else if ( QueuedTrack != NULL )
  {
    QueuedTrack->Stream->Init( QueuedTrack );
    CurrentTrack = QueuedTrack;
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

