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
 * UEngine.h - Base Engine Subsystem
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/USystem.h"
#include "Core/UPrimitive.h"
#include "Engine/UAudio.h"
#include "Engine/URender.h"

class UEngine;

/*-----------------------------------------------------------------------------
 * UClient
 * An object representing a client which interacts with the engine
-----------------------------------------------------------------------------*/
class LIBUNR_API UClient : public UObject
{
  DECLARE_NATIVE_CLASS( UClient, UObject, CLASS_NoExport, Engine )

  UClient();

  virtual bool Init();
  virtual bool Exit();

  virtual UViewport* OpenViewport( int Width, int Height ) { return NULL; }
  virtual bool CloseViewport( UViewport* Viewport ) { return false; }
  virtual void Tick( float DeltaTime ) {}

  UClass* ViewportClass;

  UEngine* Engine;
  TArray<UViewport*> Viewports;
};

/*-----------------------------------------------------------------------------
 * UEngine
 * The base engine class which enables basic elements of game and editor logic
-----------------------------------------------------------------------------*/
class LIBUNR_API UEngine : public USubsystem
{
  DECLARE_NATIVE_CLASS( UEngine, USubsystem, CLASS_Transient, Engine )
  EXPOSE_TO_USCRIPT()

  UEngine();

  virtual bool Init();
  virtual bool Exit();
  virtual void Tick( float DeltaTime );

  UClass* ClientClass;
  UClass* GameRenderDevice;
  UClass* AudioDevice;
  UClass* Console;
  UClass* NetworkDevice;
  UClass* Language;

  UPrimitive*      Cylinder; // but why?
  UClient*         Client;
  URenderDevice*   Render;
  UAudioSubsystem* Audio;

  int TickCycles;
  int GameCycles;
  int ClientCycles;
  int MinDesiredFramerate;

  int CacheSizeMegs;
  bool UseSound;

  float CurrentTickRate;

protected:
  UNativeModule* AudioModule;
  UNativeModule* RenderModule;
};

extern LIBUNR_API UEngine* GEngine;
