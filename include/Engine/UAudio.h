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
 * UAudio.h - Base Audio Subsystem 
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/USystem.h"
#include "Engine/UMusic.h"
#include "Engine/USound.h"
#include "Engine/UMusic.h"
#include "Engine/UViewport.h"
#include "Actors/AActor.h"

/*-----------------------------------------------------------------------------
 * UAudioSubsystem
 * Defines the base interface for audio devices to play sounds and music
-----------------------------------------------------------------------------*/

class LIBUNR_API UAudioSubsystem : public USubsystem
{
  DECLARE_NATIVE_CLASS( UAudioSubsystem, USubsystem, CLASS_NoExport, Engine )

  UAudioSubsystem();

  virtual bool Init();
  virtual bool SetOutputDevice( const char* Name ) { return false; }
  virtual void SetViewport( UViewport* Viewport ) {}
  virtual void RegisterSound( USound* Sound ) {}
  virtual void UnregisterSound( USound* Sound ) {}
  virtual bool PlaySound( AActor* Actor, USound* Sound, FVector Location, float Volume, float Radius, float Pitch )  { return false; }
  virtual void PlayMusicBuffer( float* MusicBuffer, int NumSamples ) {}
  
  // Music playback logic should be consistent across all audio devices
  void PlayMusic( UMusic* Music, int SongSection, EMusicTransition MusicTrans );
  void StopMusic( EMusicTransition MusicTrans );
    
  u8 SoundVolume;
  u8 MusicVolume;
  u32 OutputRate;

private:
  FMusicPlayer* MusicPlayer;
};

