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
 * AInventory.h - Base Inventory Class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"
#include "Actors/ANavigationPoint.h"

class LIBUNR_API AInventoryAttachment : public AActor
{
  DECLARE_NATIVE_CLASS( AInventoryAttachment, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AInventoryAttachment();
};

class LIBUNR_API AInventorySpot : public ANavigationPoint
{
  DECLARE_NATIVE_CLASS( AInventorySpot, ANavigationPoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AInventorySpot();

  AInventory* MarkedItem;
};

class LIBUNR_API AInventory : public AActor
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
  bool bTossedOut;

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
  
  FString* PickupMessage;
  FString* ItemName;
  FString* ItemArticle;
  float RespawnTime;
  idx   PlayerLastTouched;
  UClass* PickupMessageClass;
  UClass* ItemMessageClass;

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
  FString*   M_Activated;
  FString*   M_Selected;
  FString*   M_Deactivated;

  // Deus Ex Variables
  USound* LandSound;
  bool bCanUseObjectBelt;
  UTexture* LargeIcon;
  int LargeIconWidth;
  int LargeIconHeight;
  int InvSlotsX;
  int InvSlotsY;
  int InvPosX;
  int InvPosY;
  bool bInObjectBelt;
  int BeltPos;
  FString* Description;
  FString* BeltDescription;

  // Rune Variables
  bool bExpireWhenTossed;
  float ExpireTime;
  USound* DropSound;
};

class LIBUNR_API APickup : public AInventory
{
  DECLARE_NATIVE_CLASS( APickup, AInventory, 0, Engine )
  EXPOSE_TO_USCRIPT()

  APickup();

  AInventory* Inv;
  int NumCopies;
  bool bCanHaveMultipleCopies;
  bool bCanActivate;
  FString* ExpireMessage;
  bool bAutoActivate;
};

