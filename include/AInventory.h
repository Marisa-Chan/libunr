/*========================================================================*\
|*  This file is part of libunr.                                          *|
|*                                                                        *|
|*  libunr is free software: you can redistribute it and/or modify        *|
|*  it under the terms of the GNU General Public License as published by  *|
|*  the Free Software Foundation, either version 3 of the License, or     *|
|*  (at your option) any later version.                                   *|
|*                                                                        *|
|*  libunr is distributed in the hope that it will be useful,             *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *|
|*  GNU General Public License for more details.                          *|
|*                                                                        *|
|*  You should have received a copy of the GNU General Public License     *|
|*  along with libunr.  If not, see <http://www.gnu.org/licenses/>.       *|
|*                                                                        *|
\*========================================================================*/

/*========================================================================
 * AInventory.h - Base Inventory Class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "AActor.h"
#include "ANavigationPoint.h"

class AInventoryAttachment : public AActor
{
  DECLARE_NATIVE_CLASS( AInventoryAttachment, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AInventoryAttachment();
};

class AInventorySpot : public ANavigationPoint
{
  DECLARE_NATIVE_CLASS( AInventorySpot, ANavigationPoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AInventorySpot();

  AInventory* MarkedItem;
};

class AInventory : public AActor
{
  DECLARE_NATIVE_CLASS( AInventory, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AInventory();

  u8   AutoSwitchPriority;
  u8   InventoryGroup;
  bool bActivatable;
  bool bDisplayableInv;
  bool bActive;
  bool bSleepTouch;
  bool bHeldItem;
  bool bNoInventoryMarker;

  bool bRepMuzzleFlash;
  bool bRepPlayerView;

  bool bSteadyFlash3rd;
  bool bFirstFrame;
  bool bMuzzleFlashParticles;
  bool bToggleSteadyFlash;
  bool bSteadyToggle;

  bool bAmbientGlow;
  bool bInstantRespawn;
  bool bRotatingPickup;
  
  String* PickupMessage;
  String* ItemName;
  String* ItemArticle;
  float RespawnTime;
  idx   PlayerLastTouched;

  FVector PlayerViewOffset;
  UMesh*  PlayerViewMesh;
  float   PlayerViewScale;
  float   BobDamping;

  UMesh* PickupViewMesh;
  float  PickupViewScale;

  UMesh* ThirdPersonMesh;
  float  ThirdPersonScale;

  UTexture* StatusIcon;

  idx ProtectionType1;
  idx ProtectionType2;
  int Charge;
  int ArmorAbsorption;
  bool bIsAnArmor;
  int AbsorptionPriority;
  AInventory* NextArmor;

  float MaxDesireability;
  AInventorySpot* MyMarker;

  u8 FlashCount, OldFlashCount;
  ERenderStyle MuzzleFlashStyle;
  UMesh* MuzzleFlashMesh;
  float  MuzzleFlashScale;
  UTexture* MuzzleFlashTexture;

  USound* PickupSound;
  USound* ActivateSound;
  USound* DeActivateSound;
  USound* RespawnSound;

  UTexture* Icon;
  String*   M_Activated;
  String*   M_Selected;
  String*   M_Deactivated;
};


