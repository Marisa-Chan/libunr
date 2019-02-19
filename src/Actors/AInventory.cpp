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

IMPLEMENT_NATIVE_CLASS( AInventory );
IMPLEMENT_NATIVE_CLASS( AInventorySpot );
IMPLEMENT_NATIVE_CLASS( AInventoryAttachment );

BEGIN_PROPERTY_LINK( AInventory, 55 )
  LINK_NATIVE_PROPERTY( AInventory, AutoSwitchPriority );
  LINK_NATIVE_PROPERTY( AInventory, InventoryGroup );
  LINK_NATIVE_PROPERTY( AInventory, bActivatable );
  LINK_NATIVE_PROPERTY( AInventory, bDisplayableInv );
  LINK_NATIVE_PROPERTY( AInventory, bActive );
  LINK_NATIVE_PROPERTY( AInventory, bSleepTouch );
  LINK_NATIVE_PROPERTY( AInventory, bHeldItem );
  LINK_NATIVE_PROPERTY( AInventory, bNoInventoryMarker );
  LINK_NATIVE_PROPERTY( AInventory, bRepMuzzleFlash );
  LINK_NATIVE_PROPERTY( AInventory, bRepPlayerView );
  LINK_NATIVE_PROPERTY( AInventory, bSteadyFlash3rd );
  LINK_NATIVE_PROPERTY( AInventory, bFirstFrame );
  LINK_NATIVE_PROPERTY( AInventory, bMuzzleFlashParticles );
  LINK_NATIVE_PROPERTY( AInventory, bToggleSteadyFlash );
  LINK_NATIVE_PROPERTY( AInventory, bSteadyToggle );
  LINK_NATIVE_PROPERTY( AInventory, bAmbientGlow );
  LINK_NATIVE_PROPERTY( AInventory, bInstantRespawn );
  LINK_NATIVE_PROPERTY( AInventory, bRotatingPickup );
  LINK_NATIVE_PROPERTY( AInventory, PickupMessage );
  LINK_NATIVE_PROPERTY( AInventory, ItemName );
  LINK_NATIVE_PROPERTY( AInventory, ItemArticle );
  LINK_NATIVE_PROPERTY( AInventory, RespawnTime );
  LINK_NATIVE_PROPERTY( AInventory, PlayerLastTouched );
  LINK_NATIVE_PROPERTY( AInventory, PlayerViewOffset );
  LINK_NATIVE_PROPERTY( AInventory, PlayerViewMesh );
  LINK_NATIVE_PROPERTY( AInventory, PlayerViewScale );
  LINK_NATIVE_PROPERTY( AInventory, BobDamping );
  LINK_NATIVE_PROPERTY( AInventory, PickupViewMesh );
  LINK_NATIVE_PROPERTY( AInventory, PickupViewScale );
  LINK_NATIVE_PROPERTY( AInventory, ThirdPersonMesh );
  LINK_NATIVE_PROPERTY( AInventory, ThirdPersonScale );
  LINK_NATIVE_PROPERTY( AInventory, StatusIcon );
  LINK_NATIVE_PROPERTY( AInventory, ProtectionType1 );
  LINK_NATIVE_PROPERTY( AInventory, ProtectionType2 );
  LINK_NATIVE_PROPERTY( AInventory, Charge );
  LINK_NATIVE_PROPERTY( AInventory, ArmorAbsorption );
  LINK_NATIVE_PROPERTY( AInventory, bIsAnArmor );
  LINK_NATIVE_PROPERTY( AInventory, AbsorptionPriority );
  LINK_NATIVE_PROPERTY( AInventory, NextArmor );
  LINK_NATIVE_PROPERTY( AInventory, MaxDesireability );
  LINK_NATIVE_PROPERTY( AInventory, MyMarker );
  LINK_NATIVE_PROPERTY( AInventory, FlashCount );
  LINK_NATIVE_PROPERTY( AInventory, OldFlashCount );
  LINK_NATIVE_PROPERTY( AInventory, MuzzleFlashStyle );
  LINK_NATIVE_PROPERTY( AInventory, MuzzleFlashMesh );
  LINK_NATIVE_PROPERTY( AInventory, MuzzleFlashScale );
  LINK_NATIVE_PROPERTY( AInventory, MuzzleFlashTexture );
  LINK_NATIVE_PROPERTY( AInventory, PickupSound );
  LINK_NATIVE_PROPERTY( AInventory, ActivateSound );
  LINK_NATIVE_PROPERTY( AInventory, DeActivateSound );
  LINK_NATIVE_PROPERTY( AInventory, RespawnSound );
  LINK_NATIVE_PROPERTY( AInventory, Icon );
  LINK_NATIVE_PROPERTY( AInventory, M_Activated );
  LINK_NATIVE_PROPERTY( AInventory, M_Selected );
  LINK_NATIVE_PROPERTY( AInventory, M_Deactivated );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AInventorySpot, 1 )
  LINK_NATIVE_PROPERTY( AInventorySpot, MarkedItem );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AInventoryAttachment, 0 )
END_PROPERTY_LINK()

