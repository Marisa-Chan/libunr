/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * UEngine.h - Base Engine Subsystem
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UAudio.h"
#include "Core/URender.h"
#include "Core/USystem.h"
#include "Core/UPrimitive.h"

class UClient : public UObject
{
  DECLARE_NATIVE_CLASS( UClient, UObject, CLASS_NoExport, Engine )

  UClient();

  // TODO:
};

class UEngine : public USubsystem
{
  DECLARE_NATIVE_CLASS( UEngine, USubsystem, CLASS_Transient, Engine )
  EXPOSE_TO_USCRIPT()

  UEngine();

  UClass* GameRenderDevice;
  UClass* AudioDevice;
  UClass* Console;
  UClass* NetworkDevice;
  UClass* Language;

  UPrimitive*      Cylinder; // but why?
  UClient*         Client;
  URenderBase*     Render;
  UAudioSubsystem* Audio;

  int TickCycles;
  int GameCycles;
  int ClientCycles;

  int CacheSizeMegs;
  bool UseSound;

  float CurrentTickRate;
};



