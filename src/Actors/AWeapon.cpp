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
  LINK_NATIVE_PROPERTY( AWeaponMuzzleFlash, bConstantMuzzle );
  LINK_NATIVE_PROPERTY( AWeaponMuzzleFlash, bStrobeMuzzle );
  LINK_NATIVE_PROPERTY( AWeaponMuzzleFlash, bFlashTimer );
  LINK_NATIVE_PROPERTY( AWeaponMuzzleFlash, bCurrentlyVisible );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AWeaponAttachment, 4 )
  LINK_NATIVE_PROPERTY( AWeaponAttachment, bCopyDisplay );
  LINK_NATIVE_PROPERTY( AWeaponAttachment, WeaponOwner );
  LINK_NATIVE_PROPERTY( AWeaponAttachment, MyMuzzleFlash );
  LINK_NATIVE_PROPERTY( AWeaponAttachment, LastUpdateTime );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AWeapon, 65 )
  LINK_NATIVE_PROPERTY( AWeapon, MaxTargetRange );
  LINK_NATIVE_PROPERTY( AWeapon, AmmoName );
  LINK_NATIVE_PROPERTY( AWeapon, ReloadCount );
  LINK_NATIVE_PROPERTY( AWeapon, PickupAmmoCount );
  LINK_NATIVE_PROPERTY( AWeapon, AmmoType );
  LINK_NATIVE_PROPERTY( AWeapon, bPointing );
  LINK_NATIVE_PROPERTY( AWeapon, bInstantHit );
  LINK_NATIVE_PROPERTY( AWeapon, bAltInstantHit );
  LINK_NATIVE_PROPERTY( AWeapon, bWarnTarget );
  LINK_NATIVE_PROPERTY( AWeapon, bAltWarnTarget );
  LINK_NATIVE_PROPERTY( AWeapon, bWeaponUp );
  LINK_NATIVE_PROPERTY( AWeapon, bChangeWeapon );
  LINK_NATIVE_PROPERTY( AWeapon, bLockedOn );
  LINK_NATIVE_PROPERTY( AWeapon, bSplashDamage );
  LINK_NATIVE_PROPERTY( AWeapon, bCanThrow );
  LINK_NATIVE_PROPERTY( AWeapon, bRecommendSplashDamage );
  LINK_NATIVE_PROPERTY( AWeapon, bWeaponStay );
  LINK_NATIVE_PROPERTY( AWeapon, bOwnsCrosshair );
  LINK_NATIVE_PROPERTY( AWeapon, bHideWeapon );
  LINK_NATIVE_PROPERTY( AWeapon, bMeleeWeapon );
  LINK_NATIVE_PROPERTY( AWeapon, bRapidFire );
  LINK_NATIVE_PROPERTY( AWeapon, bTossedOut );
  LINK_NATIVE_PROPERTY( AWeapon, FiringSpeed );
  LINK_NATIVE_PROPERTY( AWeapon, FireOffset );
  LINK_NATIVE_PROPERTY( AWeapon, ProjectileClass );
  LINK_NATIVE_PROPERTY( AWeapon, AltProjectileClass );
  LINK_NATIVE_PROPERTY( AWeapon, MyDamageType );
  LINK_NATIVE_PROPERTY( AWeapon, AltDamageType );
  LINK_NATIVE_PROPERTY( AWeapon, ProjectileSpeed );
  LINK_NATIVE_PROPERTY( AWeapon, AltProjectileSpeed );
  LINK_NATIVE_PROPERTY( AWeapon, AimError );
  LINK_NATIVE_PROPERTY( AWeapon, ShakeMag );
  LINK_NATIVE_PROPERTY( AWeapon, ShakeTime );
  LINK_NATIVE_PROPERTY( AWeapon, ShakeVert );
  LINK_NATIVE_PROPERTY( AWeapon, AIRating );
  LINK_NATIVE_PROPERTY( AWeapon, RefireRate );
  LINK_NATIVE_PROPERTY( AWeapon, AltRefireRate );
  LINK_NATIVE_PROPERTY( AWeapon, ThirdPersonAttachment );
  LINK_NATIVE_PROPERTY( AWeapon, Attachment );
  LINK_NATIVE_PROPERTY( AWeapon, FireSound );
  LINK_NATIVE_PROPERTY( AWeapon, AltFireSound );
  LINK_NATIVE_PROPERTY( AWeapon, CockingSound );
  LINK_NATIVE_PROPERTY( AWeapon, SelectSound );
  LINK_NATIVE_PROPERTY( AWeapon, Misc1Sound );
  LINK_NATIVE_PROPERTY( AWeapon, Misc2Sound );
  LINK_NATIVE_PROPERTY( AWeapon, Misc3Sound );
  LINK_NATIVE_PROPERTY( AWeapon, MessageNoAmmo );
  LINK_NATIVE_PROPERTY( AWeapon, DeathMessage );
  LINK_NATIVE_PROPERTY( AWeapon, FemDeathMessage );
  LINK_NATIVE_PROPERTY( AWeapon, FemKillMessage );
  LINK_NATIVE_PROPERTY( AWeapon, AdjustedAim );
  LINK_NATIVE_PROPERTY( AWeapon, bBreakWeaponChange );
  LINK_NATIVE_PROPERTY( AWeapon, bSetFlashTime );
  LINK_NATIVE_PROPERTY( AWeapon, bDrawMuzzleFlash );
  LINK_NATIVE_PROPERTY( AWeapon, bMuzzleFlash );
  LINK_NATIVE_PROPERTY( AWeapon, FlashTime );
  LINK_NATIVE_PROPERTY( AWeapon, MuzzleScale );
  LINK_NATIVE_PROPERTY( AWeapon, FlashY );
  LINK_NATIVE_PROPERTY( AWeapon, FlashO );
  LINK_NATIVE_PROPERTY( AWeapon, FlashC );
  LINK_NATIVE_PROPERTY( AWeapon, FlashLength );
  LINK_NATIVE_PROPERTY( AWeapon, FlashS );
  LINK_NATIVE_PROPERTY( AWeapon, MFTexture );
  LINK_NATIVE_PROPERTY( AWeapon, MuzzleFlare );
  LINK_NATIVE_PROPERTY( AWeapon, FlareOffset );
END_PROPERTY_LINK()

