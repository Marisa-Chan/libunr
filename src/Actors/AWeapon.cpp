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
 * AWeapon.h - Weapon functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/AWeapon.h"

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

IMPLEMENT_NATIVE_CLASS( AWeapon );
IMPLEMENT_NATIVE_CLASS( AWeaponAttachment );
IMPLEMENT_NATIVE_CLASS( AWeaponMuzzleFlash );

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

BEGIN_PROPERTY_LINK( AWeapon, 67 )
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
END_PROPERTY_LINK()

