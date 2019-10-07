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
#include "Engine/UViewport.h"
#include "Actors/AActor.h"

class LIBUNR_API UAudioSubsystem : public USubsystem
{
  DECLARE_NATIVE_CLASS( UAudioSubsystem, USubsystem, CLASS_NoExport, Engine )

  UAudioSubsystem();

  virtual bool SetOutputDevice( const char* Name ) { return false; }
  virtual void SetViewport( UViewport* Viewport ) {}
  virtual void RegisterSound( USound* Sound ) {}
  virtual void RegisterMusic( UMusic* Music ) {}
  virtual void UnregisterSound( USound* Sound ) {}
  virtual void UnregisterMusic( UMusic* Music ) {}
  virtual bool PlaySound( AActor* Actor, USound* Sound, FVector Location, float Volume, float Radius, float Pitch ) 
  {
    return false;
  }
  virtual bool DoMusicTransition( UMusic* Music, EMusicTransition MusicTrans ) 
  {
    return false;
  }
};

