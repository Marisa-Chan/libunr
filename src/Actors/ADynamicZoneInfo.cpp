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
 * ADynamicZoneInfo.h - DynamicZoneInfo functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/ADynamicZoneInfo.h"

ADynamicZoneInfo::ADynamicZoneInfo()
  : AZoneInfo()
{
}

ADynamicZoneInfo::~ADynamicZoneInfo()
{
}

IMPLEMENT_NATIVE_CLASS( ADynamicZoneInfo );

BEGIN_PROPERTY_LINK( ADynamicZoneInfo, 11 )
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, NextDynamicZone );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, ZoneAreaType );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, BoxMin );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, BoxMax );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, CylinderSize );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, SphereSize );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, MatchOnlyZone );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, bUseRelativeToRotation );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, bMovesForceTouchUpdate );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, bUpdateTouchers );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, OldPose );
END_PROPERTY_LINK()

