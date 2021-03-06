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

class DLL_EXPORT AAmmo : public APickup
{
  DECLARE_NATIVE_CLASS( AAmmo, APickup, 0, Engine );
  EXPOSE_TO_USCRIPT()

  AAmmo();

  int AmmoAmount;
  int MaxAmmo;
  UClass* ParentAmmo;
  u8  UsedInWeaponSlot[10];
  AAmmo* PAmmo;
};

class DLL_EXPORT AWeaponMuzzleFlash : public AInventoryAttachment
{
  DECLARE_NATIVE_CLASS( AWeaponMuzzleFlash, AInventoryAttachment, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AWeaponMuzzleFlash();

  bool bConstantMuzzle;
  bool bStrobeMuzzle;
  bool bFlashTimer;
  bool bCurrentlyVisible;
};

class DLL_EXPORT AWeaponAttachment : public AInventoryAttachment
{
  DECLARE_NATIVE_CLASS( AWeaponAttachment, AInventoryAttachment, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AWeaponAttachment();

  bool                bCopyDisplay;
  AWeapon*            WeaponOwner;
  AWeaponMuzzleFlash* MyMuzzleFlash;
  float               LastUpdateTime;
};

class DLL_EXPORT AWeapon : public AInventory 
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

  // Rune Variables
  struct FSwipeHit
  {
	  AActor* Actor;
	  int LowMask;
	  int HighMask;
  };

  enum EMeleeType
  {
	  MELEE_SWORD,
	  MELEE_HAMMER,
	  MELEE_AXE,
	  MELEE_NON_STOW
  } MeleeType;

  bool bCrouchTwoHands;
  bool bPoweredUp;
  bool bCanBePoweredUp;
  bool bPlayedDropSound;
  bool bClientPoweredUp;
  int HitMatterSoundCount;
  u8 StowMesh;
  int Damage;
  FName DamageType;
  FName ThrownDamageType;
  UTexture* BloodTexture;
  int rating;
  float WeaponSweepExtent;
  int SweepJoint1;
  int SweepJoint2;
  float ExtendedLength;
  int RunePowerRequired;
  float RunePowerDuration;
  FString* PowerupMessage;
  u8 StabMesh;
  int TimerCount;
  FVector SweepVector;
  int FrameOfAttackAnim;
  FVector gB1;
  FVector gE1;
  FVector gB2;
  FVector gE2;
  AActor* StabbedActor;
  AActor* LastThrower;
  USound* ThroughAir[3];
  USound* ThroughAirBerserk[3];
  USound* HitFlesh[3];
  USound* HitWood[3];
  USound* HitStone[3];
  USound* HitMetal[3];
  USound* HitDirt[3];
  USound* HitShield;
  USound* HitWeapon;
  USound* HitBreakableWood;
  USound* HitBreakableStone;
  USound* SheathSound;
  USound* UnsheathSound;
  USound* ThrownSoundLOOP;
  USound* PowerUpSound;
  USound* PoweredUpSoundLOOP;
  USound* PoweredUpEndingSound;
  USound* PoweredUpEndSound;
  float PitchDeviation;
  UTexture* PowerupIcon;
  UTexture* PowerupIconAnim;
  FVector lastpos1;
  FVector lastpos2;
  int NumThroughAirSounds;
  int NumThroughAirBerserkSounds;
  int NumFleshSounds;
  int NumWoodSounds;
  int NumStoneSounds;
  int NumMetalSounds;
  int NumEarthSounds;
  FSwipeHit SwipeHits[16];
  class AWeaponSwipe* swipe;
  UClass* SwipeClass;
  UClass* PoweredUpSwipeClass;
  FName A_Idle;
  FName A_TurnLeft;
  FName A_TurnRight;
  FName A_Forward;
  FName A_Backward;
  FName A_Forward45Right;
  FName A_Forward45Left;
  FName A_Backward45Right;
  FName A_Backward45Left;
  FName A_StrafeRight;
  FName A_StrafeLeft;
  FName A_Jump;
  FName A_ForwardAttack;
  FName A_AttackA;
  FName A_AttackAReturn;
  FName A_AttackB;
  FName A_AttackBReturn;
  FName A_AttackC;
  FName A_AttackCReturn;
  FName A_AttackD;
  FName A_AttackDReturn;
  FName A_AttackStandA;
  FName A_AttackStandAReturn;
  FName A_AttackStandB;
  FName A_AttackStandBReturn;
  FName A_AttackBackupA;
  FName A_AttackBackupAReturn;
  FName A_AttackBackupB;
  FName A_AttackBackupBReturn;
  FName A_AttackStrafeRight;
  FName A_AttackStrafeLeft;
  FName A_JumpAttack;
  FName A_Throw;
  FName A_Powerup;
  FName A_Defend;
  FName A_DefendIdle;
  FName A_PainFront;
  FName A_PainBack;
  FName A_PainLeft;
  FName A_PainRight;
  FName A_PickupGroundLeft;
  FName A_PickupHighLeft;
  FName A_Taunt;
  FName A_PumpTrigger;
  FName A_LeverTrigger;
};

