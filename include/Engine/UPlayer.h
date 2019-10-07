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
 * UPlayer.h - Player object class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UObject.h"

class APlayerPawn;
class UConsole;

class LIBUNR_API UPlayer : public UObject
{
  DECLARE_NATIVE_CLASS( UPlayer, UObject, CLASS_Transient | CLASS_Config, Engine )
  EXPOSE_TO_USCRIPT()

  UPlayer();

  APlayerPawn* Actor;
  UConsole*    Console;

  bool  bWindowsMouseAvailable;
  bool  bShowWindowsMouse;
  float WindowsMouseX;
  float WindowsMouseY;
  u8    SelectedCursor;

  // UT99 Variables
  bool bSuspendPrecaching;
  int  CurrentNetSpeed;
  int  ConfiguredInternetSpeed;
  int  ConfiguredLanSpeed;

  // Deus Ex Variables
  float StaticUpdateInterval;
  float DynamicUpdateInterval;
};

