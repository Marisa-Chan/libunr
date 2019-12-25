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
}

UAudioSubsystem::~UAudioSubsystem()
{
}

bool UAudioSubsystem::Init()
{
  if ( MusicPlayer == NULL )
  {
    MusicPlayer = new FMusicPlayer();
    if ( MusicPlayer == NULL )
    {
      GLogf( LOG_CRIT, "Failed to initialize music player" );
      return false;
    }
  }

  return true;
}

void UAudioSubsystem::PlayMusic( UMusic* Music, int SongSection, EMusicTransition MusicTrans )
{
  MusicPlayer->Play( Music, SongSection, MusicTrans );
}

void UAudioSubsystem::StopMusic( EMusicTransition MusicTrans )
{
  MusicPlayer->Stop( MusicTrans );
  /*
  QueuedTrack = NULL;
  QueuedSection = 255;
  CurrentTransition = MusicTrans;
  bTransitioning = true;
  */
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UAudioSubsystem );

