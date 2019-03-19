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
 * AWeapon.h - Base Weapon Class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AInventory.h"

class AWeapon;

class AWeaponMuzzleFlash : public AInventoryAttachment
{
  DECLARE_NATIVE_CLASS( AWeaponMuzzleFlash, AInventoryAttachment, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AWeaponMuzzleFlash();

  bool bConstantMuzzle;
  bool bStrobeMuzzle;
  bool bFlashTimer;
  bool bCurrentlyVisible;
};

class AWeaponAttachment : public AInventoryAttachment
{
  DECLARE_NATIVE_CLASS( AWeaponAttachment, AInventoryAttachment, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AWeaponAttachment();

  bool                bCopyDisplay;
  AWeapon*            WeaponOwner;
  AWeaponMuzzleFlash* MyMuzzleFlash;
  float               LastUpdateTime;
};

class AWeapon : public AInventory 
{
  DECLARE_NATIVE_CLASS( AWeapon, AInventory, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AWeapon();

  float    MaxTargetRange;
  UClass*  AmmoName;
  u8       ReloadCount;
  int      PickupAmmoCount;
  UObject* AmmoType;
  bool     bPointing;
  bool     bInstantHit;
  bool     bAltInstantHit;
  bool     bWarnTarget;
  bool     bAltWarnTarget;
  bool     bWeaponUp;
  bool     bChangeWeapon;
  bool     bLockedOn;
  bool     bSplashDamage;
  bool     bCanThrow;
  bool     bRecommendSplashDamage;
  bool     bRecommendAltSplashDamage;
  bool     bWeaponStay;
  bool     bOwnsCrosshair;
  bool     bHideWeapon;
  bool     bMeleeWeapon;
  bool     bRapidFire;
  bool     bTossedOut;
  bool     bSpecialIcon;
  float    FiringSpeed;

  FVector  FireOffset;
  UClass*  ProjectileClass;
  UClass*  AltProjectileClass;
  idx      MyDamageType;
  idx      AltDamageType;
  float    ProjectileSpeed;
  float    AltProjectileSpeed;
  float    AimError;
  float    ShakeMag;
  float    ShakeTime;
  float    ShakeVert;
  float    AIRating;
  float    RefireRate;
  float    AltRefireRate;

  UClass* ThirdPersonAttachment;
  AWeaponAttachment* Attachment;

  USound* FireSound;
  USound* AltFireSound;
  USound* CockingSound;
  USound* SelectSound;
  USound* Misc1Sound;
  USound* Misc2Sound;
  USound* Misc3Sound;

  FString* MessageNoAmmo;
  FString* DeathMessage;
  FString* FemDeathMessage;
  FString* FemKillMessage;

  FRotator AdjustedAim;
  bool bBreakWeaponChange;

  bool bSetFlashTime;
  bool bDrawMuzzleFlash;
  bool bMuzzleFlash;
  float FlashTime;
  float MuzzleScale;
  float FlashY, FlashO, FlashC;
  float FlashLength;
  int FlashS;
  UTexture* MFTexture;
  UTexture* MuzzleFlare;
  float FlareOffset;

  FColor NameColor;
};

