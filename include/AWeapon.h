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
 * AWeapon.h - Base Weapon Class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "AInventory.h"

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
  bool     bWeaponStay;
  bool     bOwnsCrosshair;
  bool     bHideWeapon;
  bool     bMeleeWeapon;
  bool     bRapidFire;
  bool     bTossedOut;
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

  String* MessageNoAmmo;
  String* DeathMessage;
  String* FemDeathMessage;
  String* FemKillMessage;

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
};

