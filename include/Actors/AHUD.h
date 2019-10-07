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
 * AHUD.h - Heads-Up Display Class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"

class AHUDOverlay;
class AMenu;

class LIBUNR_API AHUD : public AActor
{
  DECLARE_NATIVE_CLASS( AHUD, AActor, CLASS_Abstract, Engine )
  EXPOSE_TO_USCRIPT()

  AHUD();

  int HudMode;
  int Crosshair;
  UClass* MainMenuType;
  FString* HUDConfigWindowType;

  AMenu* MainMenu;
  TArray<AHUDOverlay*>* Overlays;

  // UT99 Variables
  FColor WhiteColor;
  AMutator* HUDMutator;
  APlayerPawn* PlayerOwner;
};

class LIBUNR_API AHUDOverlay : public AActor
{
  DECLARE_NATIVE_CLASS( AHUDOverlay, AActor, CLASS_Abstract, Engine )
  EXPOSE_TO_USCRIPT()

  AHUDOverlay();

  AHUD* myHUD;
  bool bPostRender;
  bool bPreRender;
};

