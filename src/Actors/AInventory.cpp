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
 * AInventory.cpp - Base Inventory Functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/AInventory.h"

AInventoryAttachment::AInventoryAttachment()
  : AActor()
{
}

AInventoryAttachment::~AInventoryAttachment()
{
}

AInventorySpot::AInventorySpot()
  : ANavigationPoint()
{
  MarkedItem = NULL;
}

AInventorySpot::~AInventorySpot()
{
}

AInventory::AInventory()
  : AActor()
{
}

AInventory::~AInventory()
{
}

APickup::APickup()
  : AInventory()
{
}

APickup::~APickup()
{
}

IMPLEMENT_NATIVE_CLASS( AInventory );
IMPLEMENT_NATIVE_CLASS( AInventorySpot );
IMPLEMENT_NATIVE_CLASS( AInventoryAttachment );
IMPLEMENT_NATIVE_CLASS( APickup );

BEGIN_PROPERTY_LINK( AInventory, 58 )
  LINK_NATIVE_PROPERTY( AutoSwitchPriority );
  LINK_NATIVE_PROPERTY( InventoryGroup );
  LINK_NATIVE_PROPERTY( bActivatable );
  LINK_NATIVE_PROPERTY( bDisplayableInv );
  LINK_NATIVE_PROPERTY( bActive );
  LINK_NATIVE_PROPERTY( bSleepTouch );
  LINK_NATIVE_PROPERTY( bHeldItem );
  LINK_NATIVE_PROPERTY( bNoInventoryMarker );
  LINK_NATIVE_PROPERTY( bTossedOut );
  LINK_NATIVE_PROPERTY( bRepMuzzleFlash );
  LINK_NATIVE_PROPERTY( bRepPlayerView );
  LINK_NATIVE_PROPERTY( bSteadyFlash3rd );
  LINK_NATIVE_PROPERTY( bFirstFrame );
  LINK_NATIVE_PROPERTY( bMuzzleFlashParticles );
  LINK_NATIVE_PROPERTY( bToggleSteadyFlash );
  LINK_NATIVE_PROPERTY( bSteadyToggle );
  LINK_NATIVE_PROPERTY( bAmbientGlow );
  LINK_NATIVE_PROPERTY( bInstantRespawn );
  LINK_NATIVE_PROPERTY( bRotatingPickup );
  LINK_NATIVE_PROPERTY( PickupMessage );
  LINK_NATIVE_PROPERTY( ItemName );
  LINK_NATIVE_PROPERTY( ItemArticle );
  LINK_NATIVE_PROPERTY( RespawnTime );
  LINK_NATIVE_PROPERTY( PlayerLastTouched );
  LINK_NATIVE_PROPERTY( PickupMessageClass );
  LINK_NATIVE_PROPERTY( ItemMessageClass );
  LINK_NATIVE_PROPERTY( PlayerViewOffset );
  LINK_NATIVE_PROPERTY( PlayerViewMesh );
  LINK_NATIVE_PROPERTY( PlayerViewScale );
  LINK_NATIVE_PROPERTY( BobDamping );
  LINK_NATIVE_PROPERTY( PickupViewMesh );
  LINK_NATIVE_PROPERTY( PickupViewScale );
  LINK_NATIVE_PROPERTY( ThirdPersonMesh );
  LINK_NATIVE_PROPERTY( ThirdPersonScale );
  LINK_NATIVE_PROPERTY( StatusIcon );
  LINK_NATIVE_PROPERTY( ProtectionType1 );
  LINK_NATIVE_PROPERTY( ProtectionType2 );
  LINK_NATIVE_PROPERTY( Charge );
  LINK_NATIVE_PROPERTY( ArmorAbsorption );
  LINK_NATIVE_PROPERTY( bIsAnArmor );
  LINK_NATIVE_PROPERTY( AbsorptionPriority );
  LINK_NATIVE_PROPERTY( NextArmor );
  LINK_NATIVE_PROPERTY( MaxDesireability );
  LINK_NATIVE_PROPERTY( MyMarker );
  LINK_NATIVE_PROPERTY( FlashCount );
  LINK_NATIVE_PROPERTY( OldFlashCount );
  LINK_NATIVE_PROPERTY( MuzzleFlashStyle );
  LINK_NATIVE_PROPERTY( MuzzleFlashMesh );
  LINK_NATIVE_PROPERTY( MuzzleFlashScale );
  LINK_NATIVE_PROPERTY( MuzzleFlashTexture );
  LINK_NATIVE_PROPERTY( PickupSound );
  LINK_NATIVE_PROPERTY( ActivateSound );
  LINK_NATIVE_PROPERTY( DeActivateSound );
  LINK_NATIVE_PROPERTY( RespawnSound );
  LINK_NATIVE_PROPERTY( Icon );
  LINK_NATIVE_PROPERTY( M_Activated );
  LINK_NATIVE_PROPERTY( M_Selected );
  LINK_NATIVE_PROPERTY( M_Deactivated );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AInventorySpot, 1 )
  LINK_NATIVE_PROPERTY( MarkedItem );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AInventoryAttachment, 0 )
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( APickup, 6 )
  LINK_NATIVE_PROPERTY( Inv );
  LINK_NATIVE_PROPERTY( NumCopies );
  LINK_NATIVE_PROPERTY( bCanHaveMultipleCopies );
  LINK_NATIVE_PROPERTY( bCanActivate );
  LINK_NATIVE_PROPERTY( ExpireMessage );
  LINK_NATIVE_PROPERTY( bAutoActivate );
END_PROPERTY_LINK()
