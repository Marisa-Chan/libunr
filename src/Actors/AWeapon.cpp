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
 * AWeapon.h - Weapon functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/AWeapon.h"

AAmmo::AAmmo()
  : APickup()
{
}

AAmmo::~AAmmo()
{
}

AWeaponMuzzleFlash::AWeaponMuzzleFlash()
  : AInventoryAttachment()
{
}

AWeaponMuzzleFlash::~AWeaponMuzzleFlash()
{
}

AWeaponAttachment::AWeaponAttachment()
  : AInventoryAttachment()
{
}

AWeaponAttachment::~AWeaponAttachment()
{
}

AWeapon::AWeapon()
  : AInventory()
{
}

AWeapon::~AWeapon()
{
}

IMPLEMENT_NATIVE_CLASS( AAmmo );
IMPLEMENT_NATIVE_CLASS( AWeapon );
IMPLEMENT_NATIVE_CLASS( AWeaponAttachment );
IMPLEMENT_NATIVE_CLASS( AWeaponMuzzleFlash );

BEGIN_PROPERTY_LINK( AAmmo, 5 )
  LINK_NATIVE_PROPERTY( AmmoAmount );
  LINK_NATIVE_PROPERTY( MaxAmmo );
  LINK_NATIVE_PROPERTY( ParentAmmo );
  LINK_NATIVE_ARRAY   ( UsedInWeaponSlot );
  LINK_NATIVE_PROPERTY( PAmmo );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AWeaponMuzzleFlash, 4 )
  LINK_NATIVE_PROPERTY( bConstantMuzzle );
  LINK_NATIVE_PROPERTY( bStrobeMuzzle );
  LINK_NATIVE_PROPERTY( bFlashTimer );
  LINK_NATIVE_PROPERTY( bCurrentlyVisible );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AWeaponAttachment, 4 )
  LINK_NATIVE_PROPERTY( bCopyDisplay );
  LINK_NATIVE_PROPERTY( WeaponOwner );
  LINK_NATIVE_PROPERTY( MyMuzzleFlash );
  LINK_NATIVE_PROPERTY( LastUpdateTime );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AWeapon, 177 )
  LINK_NATIVE_PROPERTY( MaxTargetRange );
  LINK_NATIVE_PROPERTY( AmmoName );
  LINK_NATIVE_PROPERTY( ReloadCount );
  LINK_NATIVE_PROPERTY( PickupAmmoCount );
  LINK_NATIVE_PROPERTY( AmmoType );
  LINK_NATIVE_PROPERTY( bPointing );
  LINK_NATIVE_PROPERTY( bInstantHit );
  LINK_NATIVE_PROPERTY( bAltInstantHit );
  LINK_NATIVE_PROPERTY( bWarnTarget );
  LINK_NATIVE_PROPERTY( bAltWarnTarget );
  LINK_NATIVE_PROPERTY( bWeaponUp );
  LINK_NATIVE_PROPERTY( bChangeWeapon );
  LINK_NATIVE_PROPERTY( bLockedOn );
  LINK_NATIVE_PROPERTY( bSplashDamage );
  LINK_NATIVE_PROPERTY( bCanThrow );
  LINK_NATIVE_PROPERTY( bRecommendSplashDamage );
  LINK_NATIVE_PROPERTY( bRecommendAltSplashDamage );
  LINK_NATIVE_PROPERTY( bWeaponStay );
  LINK_NATIVE_PROPERTY( bOwnsCrosshair );
  LINK_NATIVE_PROPERTY( bHideWeapon );
  LINK_NATIVE_PROPERTY( bMeleeWeapon );
  LINK_NATIVE_PROPERTY( bRapidFire );
  LINK_NATIVE_PROPERTY( bTossedOut );
  LINK_NATIVE_PROPERTY( bSpecialIcon );
  LINK_NATIVE_PROPERTY( FiringSpeed );
  LINK_NATIVE_PROPERTY( FireOffset );
  LINK_NATIVE_PROPERTY( ProjectileClass );
  LINK_NATIVE_PROPERTY( AltProjectileClass );
  LINK_NATIVE_PROPERTY( MyDamageType );
  LINK_NATIVE_PROPERTY( AltDamageType );
  LINK_NATIVE_PROPERTY( ProjectileSpeed );
  LINK_NATIVE_PROPERTY( AltProjectileSpeed );
  LINK_NATIVE_PROPERTY( AimError );
  LINK_NATIVE_PROPERTY( ShakeMag );
  LINK_NATIVE_PROPERTY( ShakeTime );
  LINK_NATIVE_PROPERTY( ShakeVert );
  LINK_NATIVE_PROPERTY( AIRating );
  LINK_NATIVE_PROPERTY( RefireRate );
  LINK_NATIVE_PROPERTY( AltRefireRate );
  LINK_NATIVE_PROPERTY( ThirdPersonAttachment );
  LINK_NATIVE_PROPERTY( Attachment );
  LINK_NATIVE_PROPERTY( FireSound );
  LINK_NATIVE_PROPERTY( AltFireSound );
  LINK_NATIVE_PROPERTY( CockingSound );
  LINK_NATIVE_PROPERTY( SelectSound );
  LINK_NATIVE_PROPERTY( Misc1Sound );
  LINK_NATIVE_PROPERTY( Misc2Sound );
  LINK_NATIVE_PROPERTY( Misc3Sound );
  LINK_NATIVE_PROPERTY( MessageNoAmmo );
  LINK_NATIVE_PROPERTY( DeathMessage );
  LINK_NATIVE_PROPERTY( FemDeathMessage );
  LINK_NATIVE_PROPERTY( FemKillMessage );
  LINK_NATIVE_PROPERTY( AdjustedAim );
  LINK_NATIVE_PROPERTY( bBreakWeaponChange );
  LINK_NATIVE_PROPERTY( bSetFlashTime );
  LINK_NATIVE_PROPERTY( bDrawMuzzleFlash );
  LINK_NATIVE_PROPERTY( bMuzzleFlash );
  LINK_NATIVE_PROPERTY( FlashTime );
  LINK_NATIVE_PROPERTY( MuzzleScale );
  LINK_NATIVE_PROPERTY( FlashY );
  LINK_NATIVE_PROPERTY( FlashO );
  LINK_NATIVE_PROPERTY( FlashC );
  LINK_NATIVE_PROPERTY( FlashLength );
  LINK_NATIVE_PROPERTY( FlashS );
  LINK_NATIVE_PROPERTY( MFTexture );
  LINK_NATIVE_PROPERTY( MuzzleFlare );
  LINK_NATIVE_PROPERTY( FlareOffset );
  LINK_NATIVE_PROPERTY( NameColor );
  LINK_NATIVE_PROPERTY( MeleeType );
  LINK_NATIVE_PROPERTY( bCrouchTwoHands );
  LINK_NATIVE_PROPERTY( bPoweredUp );
  LINK_NATIVE_PROPERTY( bCanBePoweredUp );
  LINK_NATIVE_PROPERTY( bPlayedDropSound );
  LINK_NATIVE_PROPERTY( bClientPoweredUp );
  LINK_NATIVE_PROPERTY( HitMatterSoundCount );
  LINK_NATIVE_PROPERTY( StowMesh );
  LINK_NATIVE_PROPERTY( Damage );
  LINK_NATIVE_PROPERTY( DamageType );
  LINK_NATIVE_PROPERTY( ThrownDamageType );
  LINK_NATIVE_PROPERTY( BloodTexture );
  LINK_NATIVE_PROPERTY( rating );
  LINK_NATIVE_PROPERTY( WeaponSweepExtent );
  LINK_NATIVE_PROPERTY( SweepJoint1 );
  LINK_NATIVE_PROPERTY( SweepJoint2 );
  LINK_NATIVE_PROPERTY( ExtendedLength );
  LINK_NATIVE_PROPERTY( RunePowerRequired );
  LINK_NATIVE_PROPERTY( RunePowerDuration );
  LINK_NATIVE_PROPERTY( PowerupMessage );
  LINK_NATIVE_PROPERTY( StabMesh );
  LINK_NATIVE_PROPERTY( TimerCount );
  LINK_NATIVE_PROPERTY( SweepVector );
  LINK_NATIVE_PROPERTY( FrameOfAttackAnim );
  LINK_NATIVE_PROPERTY( gB1 );
  LINK_NATIVE_PROPERTY( gE1 );
  LINK_NATIVE_PROPERTY( gB2 );
  LINK_NATIVE_PROPERTY( gE2 );
  LINK_NATIVE_PROPERTY( StabbedActor );
  LINK_NATIVE_PROPERTY( LastThrower );
  LINK_NATIVE_PROPERTY( ThroughAir );
  LINK_NATIVE_PROPERTY( ThroughAirBerserk );
  LINK_NATIVE_PROPERTY( HitFlesh );
  LINK_NATIVE_PROPERTY( HitWood );
  LINK_NATIVE_PROPERTY( HitStone );
  LINK_NATIVE_PROPERTY( HitMetal );
  LINK_NATIVE_PROPERTY( HitDirt );
  LINK_NATIVE_PROPERTY( HitShield );
  LINK_NATIVE_PROPERTY( HitWeapon );
  LINK_NATIVE_PROPERTY( HitBreakableWood );
  LINK_NATIVE_PROPERTY( HitBreakableStone );
  LINK_NATIVE_PROPERTY( SheathSound );
  LINK_NATIVE_PROPERTY( UnsheathSound );
  LINK_NATIVE_PROPERTY( ThrownSoundLOOP );
  LINK_NATIVE_PROPERTY( PowerUpSound );
  LINK_NATIVE_PROPERTY( PoweredUpSoundLOOP );
  LINK_NATIVE_PROPERTY( PoweredUpEndingSound );
  LINK_NATIVE_PROPERTY( PoweredUpEndSound );
  LINK_NATIVE_PROPERTY( PitchDeviation );
  LINK_NATIVE_PROPERTY( PowerupIcon );
  LINK_NATIVE_PROPERTY( PowerupIconAnim );
  LINK_NATIVE_PROPERTY( lastpos1 );
  LINK_NATIVE_PROPERTY( lastpos2 );
  LINK_NATIVE_PROPERTY( NumThroughAirSounds );
  LINK_NATIVE_PROPERTY( NumThroughAirBerserkSounds );
  LINK_NATIVE_PROPERTY( NumFleshSounds );
  LINK_NATIVE_PROPERTY( NumWoodSounds );
  LINK_NATIVE_PROPERTY( NumStoneSounds );
  LINK_NATIVE_PROPERTY( NumMetalSounds );
  LINK_NATIVE_PROPERTY( NumEarthSounds );
  LINK_NATIVE_PROPERTY( SwipeHits );
  LINK_NATIVE_PROPERTY( swipe );
  LINK_NATIVE_PROPERTY( SwipeClass );
  LINK_NATIVE_PROPERTY( PoweredUpSwipeClass );
  LINK_NATIVE_PROPERTY( A_Idle );
  LINK_NATIVE_PROPERTY( A_TurnLeft );
  LINK_NATIVE_PROPERTY( A_TurnRight );
  LINK_NATIVE_PROPERTY( A_Forward );
  LINK_NATIVE_PROPERTY( A_Backward );
  LINK_NATIVE_PROPERTY( A_Forward45Right );
  LINK_NATIVE_PROPERTY( A_Forward45Left );
  LINK_NATIVE_PROPERTY( A_Backward45Right );
  LINK_NATIVE_PROPERTY( A_Backward45Left );
  LINK_NATIVE_PROPERTY( A_StrafeRight );
  LINK_NATIVE_PROPERTY( A_StrafeLeft );
  LINK_NATIVE_PROPERTY( A_Jump );
  LINK_NATIVE_PROPERTY( A_ForwardAttack );
  LINK_NATIVE_PROPERTY( A_AttackA );
  LINK_NATIVE_PROPERTY( A_AttackAReturn );
  LINK_NATIVE_PROPERTY( A_AttackB );
  LINK_NATIVE_PROPERTY( A_AttackBReturn );
  LINK_NATIVE_PROPERTY( A_AttackC );
  LINK_NATIVE_PROPERTY( A_AttackCReturn );
  LINK_NATIVE_PROPERTY( A_AttackD );
  LINK_NATIVE_PROPERTY( A_AttackDReturn );
  LINK_NATIVE_PROPERTY( A_AttackStandA );
  LINK_NATIVE_PROPERTY( A_AttackStandAReturn );
  LINK_NATIVE_PROPERTY( A_AttackStandB );
  LINK_NATIVE_PROPERTY( A_AttackStandBReturn );
  LINK_NATIVE_PROPERTY( A_AttackBackupA );
  LINK_NATIVE_PROPERTY( A_AttackBackupAReturn );
  LINK_NATIVE_PROPERTY( A_AttackBackupB );
  LINK_NATIVE_PROPERTY( A_AttackBackupBReturn );
  LINK_NATIVE_PROPERTY( A_AttackStrafeRight );
  LINK_NATIVE_PROPERTY( A_AttackStrafeLeft );
  LINK_NATIVE_PROPERTY( A_JumpAttack );
  LINK_NATIVE_PROPERTY( A_Throw );
  LINK_NATIVE_PROPERTY( A_Powerup );
  LINK_NATIVE_PROPERTY( A_Defend );
  LINK_NATIVE_PROPERTY( A_DefendIdle );
  LINK_NATIVE_PROPERTY( A_PainFront );
  LINK_NATIVE_PROPERTY( A_PainBack );
  LINK_NATIVE_PROPERTY( A_PainLeft );
  LINK_NATIVE_PROPERTY( A_PainRight );
  LINK_NATIVE_PROPERTY( A_PickupGroundLeft );
  LINK_NATIVE_PROPERTY( A_PickupHighLeft );
  LINK_NATIVE_PROPERTY( A_Taunt );
  LINK_NATIVE_PROPERTY( A_PumpTrigger );
  LINK_NATIVE_PROPERTY( A_LeverTrigger );
END_PROPERTY_LINK()

